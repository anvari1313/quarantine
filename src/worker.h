//
// Created by ahmad on 6/3/20.
//

#ifndef QUARANTINE_WORKER_H
#define QUARANTINE_WORKER_H

#include "conn_q.h"

typedef enum {INIT, RUNNING} worker_state;

typedef struct {
    conn_q cq;
    pthread_t tid;
    worker_state state;
} worker_s;

typedef worker_s *worker;

worker new_worker();
void worker_start(worker);
void worker_enq_connection(worker, int );

void *client_connection_handler(void *input);

#endif //QUARANTINE_WORKER_H
