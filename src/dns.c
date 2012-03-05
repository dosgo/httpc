#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "dns.h"

int
connect_to_host(
    const char* hostname,
    int port,
    connection_callback callback,
    connection_ctx* ctx
)
{
    int rc = 0;
    struct addrinfo *addr = 0;

    rc = getaddrinfo(hostname, NULL, NULL, &addr);
    if (rc != 0)
    {
        fprintf(stderr, "%s: %s\n", hostname, gai_strerror(rc));
        rc = -1;
        goto on_error;
    }

    while (addr)
    {

        addr = addr->ai_next;
    }

on_error:

on_success:

    return rc;
}
