#ifndef Http_fsm_h
#define Http_fsm_h

typedef struct machine_context machine_ctx;
typedef struct machine_event machine_event;

typedef struct state
{
    int state_code;
    int event_code;
    int (*callback)(machine_ctx*, machine_event*);
} state;


#endif
