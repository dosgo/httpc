#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fsm.h"
#include "downloader.h"

typedef struct downloader_ctx {
    int current_state;
    char* uri;
} downloader_ctx;

void
downloader_on_event(
    downloader_ctx *ctx,
    int event_code,
    machine_event * event
);

enum downloader_state_codes {
    INITIALIZED
};

enum downloader_event_codes {
    START
};

typedef struct downloader_start_event
{
} downloader_start_event;

int dl_initialized_start(machine_ctx* ctx, machine_event* event);

state downloader_states[] = {
    {INITIALIZED, START, dl_initialized_start}
};


downloader_ctx *
create_downloader(
    const char * uri
)
{
    downloader_ctx *ctx =
        (downloader_ctx*)malloc(sizeof(downloader_ctx));

    ctx->uri = strdup(uri);
    ctx->current_state = INITIALIZED;

    return ctx;
}

void
delete_downloader(
    downloader_ctx * ctx
)
{
    free(ctx->uri);
    free(ctx);
}

void
downloader_on_event(
    downloader_ctx *ctx,
    int event_code,
    machine_event * event
)
{
    int state_idx;

    for (state_idx = 0; state_idx < sizeof(downloader_states) / sizeof(state); ++state_idx)
    {
        if (downloader_states[state_idx].state_code == ctx->current_state &&
            downloader_states[state_idx].event_code == event_code)
        {
            ctx->current_state =
                (downloader_states[state_idx].callback)((machine_ctx*)ctx, event);
        }
    }
}

void
downloader_start(
    downloader_ctx* ctx
)
{
    downloader_start_event event;

    downloader_on_event(ctx, START, (machine_event*)&event);
}

int
dl_initialized_start(
    machine_ctx* ctx,
    machine_event* event
)
{

}
