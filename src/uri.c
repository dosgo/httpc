#include <string.h>
#include <stdlib.h>

typedef struct yy_buffer_state yy_buffer_state;

extern int
yyparse (
    char** scheme,
    char** host,
    char** port,
    char** path
);

extern yy_buffer_state*
yy_scan_string(
    const char *
);

int
parse_uri(
    char** scheme_out,
    char** host_out,
    char** port_out,
    char** path_out,
    const char* uri
)
{
    int rc = 0;
    size_t uri_length = strlen(uri);

    // arguments to flex should be double zero-terminated
    char * flex_arg = (char*) malloc(uri_length + 2);

    strcpy(flex_arg, uri);
    flex_arg[uri_length+1] = '\0';

    yy_buffer_state * state = yy_scan_string(flex_arg);
    yy_switch_to_buffer( state );

    char *scheme = 0;
    char *host = 0;
    char *port = 0;
    char *path = 0;


    int code = yyparse(&scheme, &host, &port, &path);

    if (code != 0)
    {
        rc = -1;
        goto on_error;
    }

    *scheme_out = scheme;
    *host_out = host;
    *port_out = port;
    *path_out = path;

    goto on_success;

on_error:
    free(scheme);
    free(host);
    free(port);
    free(path);

on_success:
    free(state);
    free(flex_arg);


    return rc;
}
