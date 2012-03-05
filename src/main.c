#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "uri.h"
#include "dns.h"

void
on_connected(
    int fd,
    connection_ctx* ctx
)
{
    printf("Connection established\n");
}


int main(int argc, char *argv[])
{
    char opt;
    int option_index;
    int rc;

    const char* short_options = "h";

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {NULL,0,NULL,0}
    };

    while ((opt=getopt_long(
                argc,
                argv,
                short_options,
                long_options,
                &option_index))!=-1)
    {
        switch(opt){
        case 'h': {
            printf("httpc [-h] <uri>\n");
            exit(EXIT_SUCCESS);
            break;
        }
        case '?':
        default:
            break;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "URI is not specified\n");
        exit(EXIT_FAILURE);
    }

    const char* uri = argv[optind];

    char *scheme = 0;
    char *host = 0;
    char *port = 0;
    char *path = 0;

    rc = parse_uri(&scheme, &host, &port, &path, uri);

    if (rc)
    {
        printf("%s: Malformed URI.\n", uri);
        exit(EXIT_FAILURE);
    }
    else
        printf("scheme: %s, host: %s, port: %s, path: %s\n", scheme, host, port, path);

    rc = connect_to_host(host, atoi(port), on_connected, 0);

    exit(EXIT_SUCCESS);
}
