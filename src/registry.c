#include "registry.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>

typedef struct service
{
    service_callback callback;
    void *ctx;

    int *fds;
    int nfds;
}
service;

typedef struct registry
{
    service ** services;
    int nservices;

    int pollfd;
    int nfds;
}
registry;


registry *
create_registry()
{
    registry * reg = (registry*)malloc(sizeof(registry));
    reg->nservices = 0;
    reg->services = NULL;

    reg->nfds = 0;
    reg->pollfd = epoll_create(1);

    if (reg->pollfd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }


    return reg;
}

void
delete_registry(
    void* reg
)
{

}

void
register_service(
    registry * reg,
    void * svc_ctx,
    service_callback callback
)
{
    service * svc = (service*)malloc(sizeof(service));

    svc->ctx = svc_ctx;
    svc->callback = callback;
    svc->fds = NULL;
    svc->nfds = 0;


    ++reg->nservices;
    reg->services = realloc(reg->services, reg->nservices * sizeof(service*));

    reg->services[reg->nservices-1] = svc;
}

service*
find_service_by_ctx(
    registry* reg,
    void* ctx
)
{
    int i;
    service *svc = NULL;

    for (i = 0; i < reg->nservices; ++i)
    {
        if (reg->services[i]->ctx == ctx)
            svc = reg->services[i];
    }

    return svc;
}

service*
find_service_by_fd(
    registry* reg,
    int fd
)
{
    int ns;
    int nfd;

    for (ns = 0; ns < reg->nservices; ++ns)
    {
        for (nfd = 0; nfd < reg->services[ns]->nfds; ++nfd)
        {
            if (reg->services[ns]->fds[nfd] == fd)
                return reg->services[ns];
        }
    }

    return NULL;
}


void
unregister_service(
    registry * reg,
    void *ctx
)
{

}

void
service_watch_fd(
    registry * reg,
    void * ctx,
    int fd
)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    service * svc =
        find_service_by_ctx(reg, ctx);

    if (svc == NULL)
        return;

    ++svc->nfds;
    svc->fds = realloc(svc->fds, svc->nfds * sizeof(int));

    svc->fds[svc->nfds-1] = fd;

    printf("FD = %d\n", fd);

    if (epoll_ctl(reg->pollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl: service");
        exit(EXIT_FAILURE);
    }
}

void
service_unwatch_fd(
    registry * reg,
    void * svc,
    int fd
)
{

}

void registry_do_process(
    registry * reg
)
{
    struct epoll_event events[1];
    service * svc;
    int nfds;

    nfds = epoll_wait(reg->pollfd, events, 1, -1);

    if (nfds == 0)
        return;

    svc = find_service_by_fd(reg, events[0].data.fd);

    if (svc == NULL)
        return;

    svc->callback(svc->ctx, events[0].data.fd);
}
