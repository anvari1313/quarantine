//
// Created by ahmad on 6/3/20.
//

#ifndef QUARANTINE_CONN_Q_H
#define QUARANTINE_CONN_Q_H

#include <stdbool.h>
#include <pthread.h>

struct conn_q_item_s {
    struct conn_q_item_s *last;
    struct conn_q_item_s *next;
    int cid;
};

typedef struct conn_q_item_s conn_q_item;

void conn_q_item_dispose(conn_q_item *);

struct conn_q_s {
    conn_q_item *first;
    conn_q_item *last;
    pthread_mutex_t lock;
};

typedef struct conn_q_s *conn_q;

conn_q new_conn_q();
void conn_q_enqueue(conn_q, int);
int conn_q_dequeue(conn_q);
bool conn_q_is_empty(conn_q);
void conn_q_dispose(conn_q);

#endif //QUARANTINE_CONN_Q_H
