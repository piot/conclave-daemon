/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "daemon.h"
#include "version.h"
#include <clog/console.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-client-udp/client.h>
#include <guise-client-udp/read_secret.h>
#include <imprint/default_setup.h>

#if !defined TORNADO_OS_WINDOWS
#include <time.h>
#include <unistd.h>
#endif

clog_config g_clog;

typedef struct UdpServerSocketSendToAddress {
    struct sockaddr_in* sockAddrIn;
    UdpServerSocket* serverSocket;
} UdpServerSocketSendToAddress;

static int sendToAddress(void* self_, const uint8_t* buf, size_t count)
{
    UdpServerSocketSendToAddress* self = (UdpServerSocketSendToAddress*)self_;

    return udpServerSend(self->serverSocket, buf, count, self->sockAddrIn);
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    g_clog.log = clog_console;

    CLOG_OUTPUT("conclave daemon v%s starting up", CONCLAVE_DAEMON_VERSION)

    ImprintDefaultSetup memory;
    imprintDefaultSetupInit(&memory, 16 * 1024 * 1024);

    ClvDaemon daemon;
    int err = clvDaemonInit(&daemon);
    if (err < 0) {
        return err;
    }

    GuiseClientUdp guiseClient;

    Clog guiseClientLog;
    guiseClientLog.config = &g_clog;
    guiseClientLog.constantPrefix = "GuiseClient";

    const char* guiseHost = "127.0.0.1";
    uint16_t guisePort = 27004;

    GuiseClientUdpSecret secret;
    guiseClientUdpReadSecret(&secret);

    guiseClientUdpInit(&guiseClient, &memory.tagAllocator.info, guiseHost, guisePort, &secret);
    // guiseClientUdpReInit(&guiseClient, &guiseTransport, secret.userId, secret.passwordHash);
    GuiseClientState reportedState = GuiseClientStateIdle;

    UdpServerSocketSendToAddress socketSendToAddress;
    socketSendToAddress.serverSocket = &daemon.socket;

    DatagramTransportOut transportOut;
    transportOut.self = &socketSendToAddress;
    transportOut.send = sendToAddress;

    ClvResponse response;
    response.transportOut = &transportOut;

    ClvServer server;

    // TODO:    ConclaveSerializeVersion applicationVersion = {0x10, 0x20, 0x30};

    Clog serverLog;
    serverLog.constantPrefix = "ClvServer";
    serverLog.config = &g_clog;

#define UDP_MAX_SIZE (1200)

    uint8_t buf[UDP_MAX_SIZE];
    size_t size;
    struct sockaddr_in address;

#define UDP_REPLY_MAX_SIZE (UDP_MAX_SIZE)

    uint8_t reply[UDP_REPLY_MAX_SIZE];
    FldOutStream outStream;
    fldOutStreamInit(&outStream, reply, UDP_REPLY_MAX_SIZE);

    CLOG_OUTPUT("ready for incoming UDP packets")
    bool hasCreatedConclaveDaemon = false;
    while (true) {

        struct timespec ts;

        ts.tv_sec = 0;
        ts.tv_nsec = 16 * 1000000;
        nanosleep(&ts, &ts);

        if (!hasCreatedConclaveDaemon) {
            MonotonicTimeMs now = monotonicTimeMsNow();
            guiseClientUdpUpdate(&guiseClient, now);
        }

        if (reportedState != guiseClient.guiseClient.state) {
            reportedState = guiseClient.guiseClient.state;
            if (reportedState == GuiseClientStateLoggedIn && !hasCreatedConclaveDaemon) {
                clvServerInit(&server, guiseClient.transport,
                    guiseClient.guiseClient.mainUserSessionId, &memory.tagAllocator.info,
                    serverLog);
                CLOG_C_INFO(&server.log, "server authenticated")
                hasCreatedConclaveDaemon = true;
                //guiseClientUdpDestroy(&guiseClient);
            }
        }
        if (!hasCreatedConclaveDaemon) {
            continue;
        }

        size = UDP_MAX_SIZE;
        ssize_t errorCode = udpServerReceive(&daemon.socket, buf, size, &address);
        if (errorCode < 0) {
            CLOG_WARN("problem with receive %zd", errorCode)
        } else {
            socketSendToAddress.sockAddrIn = &address;

            fldOutStreamRewind(&outStream);
#if 0
            nimbleSerializeDebugHex("received", buf, size);
#endif
            errorCode = clvServerFeed(&server, &address, buf, size, &response);
            if (errorCode < 0) {
                CLOG_WARN("clvServerFeed: error %zd", errorCode)
            }
        }
    }

    // imprintDefaultSetupDestroy(&memory);

    // return 0;
}
