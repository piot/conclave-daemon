/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-daemon
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include "daemon.h"

int clvDaemonInit(ClvDaemon* self, uint16_t port)
{
    int err = udpServerStartup();
    if (err < 0) {
        return err;
    }

    return udpServerInit(&self->socket, port, true);
}
