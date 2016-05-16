#ifndef _COOP_SCHED_H_
#define _COOP_SCHED_H_

typedef void (*task_fn_t)(uint8_t pid);

typedef struct {
    task_fn_t     entry;
} task_t;

#endif
