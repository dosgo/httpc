#ifndef Httpc_registry_h
#define Httpc_registry_h

typedef struct registry registry;

typedef struct service service;

typedef struct service_ctx service_ctx;

registry *
create_registry();

void
delete_registry(
    registry*
);

service *
register_service(
    registry *,
    service_ctx *
);

void
unregister_service(
    service*
);

void
service_watch_fd(
    service *,
    int
);

void
service_unwatch_fd(
    service *,
    int
);

void registry_do_process(
    registry *
);

#endif // Httpc_registry_h
