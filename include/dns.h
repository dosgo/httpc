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


typedef struct connection_ctx connection_ctx;

typedef void (*connection_callback)(int fd, connection_ctx* ctx);


#endif
