#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/epoll.h>
#include <string.h>

#include "dns.h"

typedef struct dns_request
{
    const char *hostname;
    struct dns_request *next;
} dns_request;

typedef struct dns_service
{
    int request_pipe;
    int response_pipe;
    int child_pid;

    dns_request *requests;
} dns_service;


void
dns_resolve(FILE * response, const char * host)
{
    int rc = 0;
    struct addrinfo *addr = 0;
    struct addrinfo hints;
    char addrstr[16];
    void* ptr;

    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    rc = getaddrinfo(host, NULL, &hints, &addr);
    if (rc != 0)
    {
        fprintf(stderr, "%s: %s\n", host, gai_strerror(rc));
        rc = -1;
        goto on_error;
    }

    for (;addr != NULL; addr = addr->ai_next)
    {
        inet_ntop (addr->ai_family, addr->ai_addr->sa_data, addrstr, 16);
        ptr = &((struct sockaddr_in *) addr->ai_addr)->sin_addr;
        inet_ntop (addr->ai_family, ptr, addrstr, 16);

        fprintf(response, "%s\1%s\n", host, addrstr);
    }

 on_error:
    printf("error\n");
 on_success:
    printf("success\n");

}

void
dns_resolve_loop(dns_service* service)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *req = fdopen(service->request_pipe, "r");
    FILE *res = fdopen(service->response_pipe, "w");

    int pollfd = epoll_create(1);
    int nfds = 0;

    if (pollfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev, events[1];
    ev.events = EPOLLIN;
    ev.data.fd = service->request_pipe;

    if (epoll_ctl(pollfd, EPOLL_CTL_ADD, service->request_pipe, &ev) == -1) {
        perror("epoll_ctl: request_pipe");
        exit(EXIT_FAILURE);
    }


    while(1)
    {
        nfds = epoll_wait(pollfd, events, 1, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        while ((read = getline(&line, &len, req)) != -1)
        {
            dns_resolve(res, line);

            free(line);
            len = 0;
            line = NULL;
        }
    }

}

dns_service*
create_dns_service(void)
{
    dns_service *service = 0;
    int rc = 0;
    int request_pipe[2];
    int response_pipe[2];

    service = (dns_service*)malloc(sizeof(dns_service));

    rc = pipe(request_pipe);

    if (rc == -1)
        goto on_error;

    rc = pipe(response_pipe);

    if (rc == -1)
        goto on_error;

    rc = fork();

    if (rc == -1)
        goto on_error;


    if (rc == 0) /* child */
    {
        close(request_pipe[1]);
        close(response_pipe[0]);

        service->request_pipe = request_pipe[0];
        service->response_pipe = response_pipe[1];

        dns_resolve_loop(service);
    }
    else /* parent */
    {
        close(request_pipe[0]);
        close(response_pipe[1]);

        service->request_pipe = request_pipe[1];
        service->response_pipe = response_pipe[0];

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
    close(service->request_pipe);
    close(service->response_pipe);

    waitpid(service->child_pid, &rc, 0);
    printf("DNS STOP\n");
    free(service);
}

void
resolve_host(
    dns_service * service,
    const char* host
)
{
    int host_length = strlen(host);
    char * nl_host = (char*)malloc(host_length + 2);

    sprintf(nl_host, "%s\n", host);

    write(service->request_pipe, host, strlen(host));

    free(nl_host);
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
