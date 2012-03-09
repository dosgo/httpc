#include "registry.h"
#include <stdlib.h>

typedef struct registry
{

}
registry;

typedef struct service
{

}
service;

registry *
create_registry()
{
    registry * reg = (registry*)malloc(sizeof(registry));

    return reg;
}

void
delete_registry(
    registry* reg
)
{

}

service*
register_service(
    registry * reg,
    service_ctx * svc
)
{

}

void
unregister_service(
    service * svc
)
{

}

void
service_watch_fd(
    service * svc,
    int fd
)
{

}

void
service_unwatch_fd(
    service * svc,
    int fd
)
{

}

void registry_do_process(
    registry * reg
)
{

}
