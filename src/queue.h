#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <pthread.h>

typedef struct {
	pthread_mutex_t mutex; // here or in queue_handle so locks/waits can be done
	pthread_cond_t cond;   //  with as little stuff as pos being read
	void *front;
	void *back;
	void *backqh;
	unsigned int size;
} queue_core;

typedef struct queue_handle {
	queue_core *qc;
	void *next;
} queue_handle;

#define QUEUE_INIT(q)														\
	do {																	\
		(q)->qh.qc = malloc(sizeof (queue_core));							\
		queue_core *qc = (q)->qh.qc;										\
		pthread_mutex_init(&qc->mutex, NULL);								\
		pthread_cond_init(&qc->cond, NULL);									\
		qc->front = qc->back = NULL;										\
		qc->backqh = NULL;													\
		qc->size = 0;														\
		(q)->qh.next = NULL;												\
	} while (0)

#define QUEUE_PUSH(q, e)													\
	do {																	\
		if (q && (q)->qh.qc) {												\
			queue_core *qc = (q)->qh.qc;									\
			queue_handle *backqh;											\
			pthread_mutex_lock(&qc->mutex);									\
			(e)->qh.qc = qc;												\
			(e)->qh.next = NULL;											\
			backqh = qc->backqh;											\
			if (!qc->front) { /* empty queue */								\
				qc->front = qc->back = (e);									\
			} else { /* non-empty queue */									\
				backqh->next = (e);											\
				qc->back = (e);												\
			}																\
			backqh = &(e)->qh;												\
			qc->size++;														\
			pthread_mutex_unlock(&qc->mutex);								\
			pthread_cond_signal(&qc->cond); /* broadcast to all? */			\
		}																	\
	} while (0)

#define QUEUE_POP(q, e)														\
	do {																	\
		(e) = NULL;															\
		if (q && (q)->qh.qc) {												\
			queue_core *qc = (q)->qh.qc;									\
			pthread_mutex_lock(&qc->mutex);									\
			while (QUEUE_SIZE(q) == 0) {									\
				pthread_cond_wait(&qc->cond, &qc->mutex);					\
			}																\
			if ((q)->qh.qc->front != NULL) {								\
				(e) = (q)->qh.qc->front;									\
				(q)->qh.qc->front = (e)->qh.next;							\
				(q)->qh.qc->size--;											\
			}																\
			pthread_mutex_unlock(&qc->mutex);								\
		}																	\
	} while (0)

#define QUEUE_LOCK(q)														\
	pthread_mutex_lock(&q->qh.qc->mutex);

#define QUEUE_SIZE(q)														\
	((q) ? (q)->qh.qc->size : 0U)

#define QUEUE_UNLOCK(q)														\
	pthread_mutex_unlock(&q->qh.qc->mutex);

#define QUEUE_FREE(q)														\
	do {																	\
		if ((q) && (q)->qh.qc) {											\
			queue_core *qc = (q)->qh.qc;									\
			pthread_cond_destroy(&qc->cond);								\
			pthread_mutex_destroy(&qc->mutex);								\
			free(qc);														\
		}																	\
	} while (0)

#endif /* QUEUE_H_ */
