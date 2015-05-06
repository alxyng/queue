#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h> /* malloc, free */

typedef struct {
	void *front;
	void *back;

	void *frontqh;
	void *backqh;

	unsigned int size;
} queue_t;

typedef struct queue_handle {
	queue_t *q;
	void *next;
} queue_handle_t;

#define QUEUE_PUSH(queue, element)											\
	do {																	\
		if (!(queue)) {														\
			(element)->qh.q = malloc(sizeof (queue_t));						\
			queue_t *q = (element)->qh.q;									\
																			\
			q->front = q->back = (element);									\
			q->frontqh = q->backqh = &((element)->qh);						\
			q->size = 0;													\
			(element)->qh.next = NULL;										\
			(queue) = (element);											\
		} else {															\
			queue_t *q = (queue)->qh.q;										\
			queue_handle_t *backqh = q->backqh;								\
			queue_handle_t *frontqh = q->frontqh;							\
																			\
			(element)->qh.q = q;											\
			(element)->qh.next = NULL;										\
																			\
			backqh->next = (element);										\
																			\
			q->back = (element);											\
			backqh = &((element)->qh);										\
																			\
			q->backqh = backqh;												\
			q->frontqh = frontqh;											\
																			\
		}																	\
		(queue)->qh.q->size++;												\
	} while (0)

#define QUEUE_POP(queue, element)											\
	do {																	\
		(element) = NULL;													\
		if (queue) {														\
			if ((queue)->qh.q->front != NULL) {								\
				(element) = (queue)->qh.q->front;							\
				(queue)->qh.q->front = (element)->qh.next;					\
				(queue)->qh.q->size--;										\
			}																\
		}																	\
	} while (0)

#define QUEUE_SIZE(queue)													\
	((queue) ? queue->qh.q->size : 0U)

#define QUEUE_FREE(queue)													\
	do {																	\
		free(queue->qh.q);													\
	} while (0)

#endif /* QUEUE_H_ */
