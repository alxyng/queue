#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    void* front;
    void* back;
    void* backqh;
    unsigned int size;
} queue_core;

typedef struct queue_handle {
    queue_core* qc;
    void* next;
} queue_handle;

#define QUEUE_INIT(qt, q)                                                      \
    do {                                                                       \
        queue_core* qc;                                                        \
        (q) = malloc(sizeof (qt));                                             \
        if (q) {                                                               \
            (q)->qh.qc = malloc(sizeof (queue_core));                          \
            if ((q)->qh.qc) {                                                  \
                qc = (q)->qh.qc;                                               \
                pthread_mutex_init(&qc->mutex, NULL);                          \
                pthread_cond_init(&qc->cond, NULL);                            \
                qc->front = qc->back = NULL;                                   \
                qc->backqh = NULL;                                             \
                qc->size = 0;                                                  \
                (q)->qh.next = NULL;                                           \
            } else {                                                           \
                free(q);                                                       \
                (q) = NULL;                                                    \
            }                                                                  \
        }                                                                      \
    } while (false)

#define QUEUE_PUSH(q, e)                                                       \
    do {                                                                       \
        queue_core* qc;                                                        \
        queue_handle* backqh;                                                  \
        qc = (q)->qh.qc;                                                       \
        pthread_mutex_lock(&qc->mutex);                                        \
        (e)->qh.qc = qc;                                                       \
        (e)->qh.next = NULL;                                                   \
        backqh = qc->backqh;                                                   \
        if (!qc->front) {                                                      \
            qc->front = qc->back = (e);                                        \
        } else {                                                               \
            backqh->next = (e);                                                \
            qc->back = (e);                                                    \
        }                                                                      \
        backqh = &((e)->qh);                                                   \
        qc->backqh = backqh;                                                   \
        qc->size++;                                                            \
        pthread_mutex_unlock(&qc->mutex);                                      \
        pthread_cond_signal(&qc->cond);                                        \
    } while (false)

#define QUEUE_POP(q, e)                                                        \
    do {                                                                       \
        queue_core* qc;                                                        \
        (e) = NULL;                                                            \
        qc = (q)->qh.qc;                                                       \
        pthread_mutex_lock(&qc->mutex);                                        \
        while (QUEUE_SIZE(q) == 0) {                                           \
            pthread_cond_wait(&qc->cond, &qc->mutex);                          \
        }                                                                      \
        if ((q)->qh.qc->front != NULL) {                                       \
            (e) = (q)->qh.qc->front;                                           \
            (q)->qh.qc->front = (e)->qh.next;                                  \
            (q)->qh.qc->size--;                                                \
        }                                                                      \
        pthread_mutex_unlock(&qc->mutex);                                      \
    } while (false)

#define QUEUE_LOCK(q)                                                          \
    pthread_mutex_lock(&q->qh.qc->mutex);

#define QUEUE_SIZE(q)                                                          \
    ((q)->qh.qc->size)

#define QUEUE_UNLOCK(q)                                                        \
    pthread_mutex_unlock(&q->qh.qc->mutex);

#define QUEUE_FREE(q)                                                          \
    do {                                                                       \
        queue_core* qc;                                                        \
        if ((q) && (q)->qh.qc) {                                               \
            qc = (q)->qh.qc;                                                   \
            pthread_cond_destroy(&qc->cond);                                   \
            pthread_mutex_destroy(&qc->mutex);                                 \
            free(qc);                                                          \
            free(q);                                                           \
        }                                                                      \
    } while (false)

#endif /* QUEUE_H */
