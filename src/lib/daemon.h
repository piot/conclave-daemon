/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_DAEMON_DAEMON_H
#define CONCLAVE_DAEMON_DAEMON_H

#include <conclave-server/server.h>
#include <udp-server/udp_server.h>

typedef struct ClvDaemon {
    ClvServer server;
    UdpServerSocket socket;
} ClvDaemon;

int clvDaemonInit(ClvDaemon* self);

#endif
