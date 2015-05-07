#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h> /* malloc, free */

typedef struct {
	void *front;
	void *back;
	void *backqh;
	unsigned int size;
} queue_t;

typedef struct queue_handle {
	queue_t *q;
	void *next;
} queue_handle;

#define QUEUE_PUSH(queue, element)											\
	do {																	\
		if (!(queue)) {														\
			(element)->qh.q = malloc(sizeof (queue_t));						\
			queue_t *q = (element)->qh.q;									\
																			\
			q->front = q->back = (element);									\
			q->backqh = &((element)->qh);									\
			q->size = 0;													\
			(element)->qh.next = NULL;										\
			(queue) = (element);											\
		} else {															\
			queue_t *q = (queue)->qh.q;										\
			queue_handle *backqh = q->backqh;								\
																			\
			(element)->qh.q = q;											\
			(element)->qh.next = NULL;										\
																			\
			/* set current back's next to the new element */				\
			backqh->next = (element);										\
																			\
			/* set back to the new element */								\
			q->back = (element);											\
																			\
			/* set the new backs queue handle to backqh */					\
			backqh = &((element)->qh);										\
																			\
			/* set the queue's backqh to the new elements queue handle */	\
			q->backqh = backqh;												\
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
		if (queue && queue->qh.q) {											\
			free(queue->qh.q);												\
		}																	\
	} while (0)

#endif /* QUEUE_H_ */
