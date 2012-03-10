#ifndef Httpc_registry_h
#define Httpc_registry_h


typedef struct service service;
typedef struct registry registry;
typedef void (*service_callback)(void*, int);

registry *
create_registry();

void
delete_registry(
    void*
);

void
register_service(
    registry *,
    void *,
    service_callback
);

void
unregister_service(
    registry*,
    void*
);

void
service_watch_fd(
    registry*,
    void *,
    int
);

void
service_unwatch_fd(
    registry*,
    void *,
    int
);

void registry_do_process(
    registry *
);

#endif // Httpc_registry_h
