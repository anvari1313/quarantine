//
// Created by ahmad on 6/3/20.
//

#include "conn_q.h"
#include <stdlib.h>

conn_q new_conn_q()
{
    conn_q cq = (conn_q) malloc(sizeof(struct conn_q_s));

    cq->first = NULL;
    cq->last = NULL;
    pthread_mutex_init(&cq->lock, NULL);
    pthread_mutex_init(&cq->is_empty_lock, NULL);
    pthread_mutex_lock(&(cq->is_empty_lock)); // Queue is empty at first

    return cq;
}

void conn_q_enqueue(conn_q cq, int cid)
{
    pthread_mutex_lock(&(cq->lock));
    conn_q_item *item = malloc(sizeof(conn_q_item));
    item->next = cq->first;
    item->last = NULL;
    item->cid = cid;

    if (cq->first != NULL)
    {
        cq->first->last = item;
        pthread_mutex_unlock(&(cq->is_empty_lock));
    }
    else
    {
        cq->last = item;
    }

    cq->first = item;
    pthread_mutex_unlock(&(cq->lock));
}

int conn_q_dequeue(conn_q cq)
{
    pthread_mutex_lock(&(cq->lock));
    conn_q_item *item = cq->last;
    int cid = item->cid;

    if (cq->first != item) // Queue will NOT be empty after dequeue
    {
        cq->last = item->last;
    }
    else // Queue will be empty after dequeue
    {
        cq->first = cq->last = NULL;
        pthread_mutex_lock(&(cq->is_empty_lock));
    }

    pthread_mutex_unlock(&(cq->lock));
    conn_q_item_dispose(item);

    return cid;
}

bool conn_q_is_empty(conn_q cq)
{
    return cq->last == NULL;
}


void conn_q_item_dispose(conn_q_item *item)
{
    free(item);
}

void conn_q_dispose(conn_q cq)
{
    conn_q_item *item = cq->first;

    while (item != NULL)
    {
        conn_q_item *nex_item = item->next;
        conn_q_item_dispose(item);
        item = nex_item;
    }

    pthread_mutex_destroy(&(cq->lock));
    pthread_mutex_destroy(&(cq->is_empty_lock));

    free(cq);
}

void conn_q_wait_on_empty(conn_q cq)
{
    pthread_mutex_lock(&(cq->is_empty_lock));
    pthread_mutex_unlock(&(cq->is_empty_lock));
}

