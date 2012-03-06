#ifndef http_uri_h
#define http_uri_h

int
parse_uri(
    char** scheme,
    char** host,
    char** port,
    char** path,
    const char* uri
);

#endif
