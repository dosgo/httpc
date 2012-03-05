#ifndef Http_downloader_h
#define Http_downloader_h

#include "fsm.h"

typedef struct downloader_ctx downloader_ctx;

downloader_ctx *
create_downloader(const char * uri);

void
delete_downloader(downloader_ctx * ctx);

void
downloader_start(downloader_ctx * ctx);



#endif
