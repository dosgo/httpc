#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "dns.h"

typedef struct dns_service
{
    int data_pipe[2];
    int child_pid;
} dns_service;

void
dns_resolve_loop(dns_service* service)
{
    while(1)
    {
        printf("RESOLVE\n");

        sleep(2);
    }
}

dns_service*
create_dns_service(void)
{
    dns_service *service = 0;
    int rc = 0;

    service = (dns_service*)malloc(sizeof(dns_service));

    rc = pipe(service->data_pipe);

    if (rc == -1)
        goto on_error;

    rc = fork();

    if (rc == -1)
        goto on_error;


    if (rc == 0) /* child */
    {
        close(service->data_pipe[0]);

        dns_resolve_loop(service);
    }
    else /* parent */
    {
        close(service->data_pipe[1]);
        service->child_pid = rc;
    }

    goto on_success;

on_error:
    free(service);
    service = 0;

on_success:
    return service;
}

void
delete_dns_service(
    dns_service* service
)
{
    int rc;
    close(service->data_pipe[0]);

    waitpid(service->child_pid, &rc, 0);
    free(service);
}


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
