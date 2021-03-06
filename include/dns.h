#ifndef Http_dns_h
#define Http_dns_h

/*
 * The dns service will use a chind process to resolve
 * domain names, which will return addresses asynchronously.
 */

typedef struct dns_service dns_service;

dns_service*
create_dns_service(void);

void
delete_dns_service(
    dns_service*
);

int
get_dns_fd(
    dns_service*
);

typedef struct connection_ctx connection_ctx;

typedef void (*connection_callback)(int fd, connection_ctx* ctx);


void
resolve_host(
    dns_service*,
    const char* host
);

#endif
