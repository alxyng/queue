#include <stdio.h>
#include "queue.h"

typedef struct msg {
	char *content;
	queue_handle_t qh;
} msg_t;

int main(void) {
	msg_t *msgs;		// queue
	msg_t test_data[4]; // individual elements
	msg_t *current;		// current message
	int i;

	msgs = NULL; // needs to be null before adding elements

	test_data[0].content = "abc";
	test_data[1].content = "def";
	test_data[2].content = "ghi";
	test_data[3].content = "jkl";

	printf("--%d--\n", QUEUE_SIZE(msgs));

	for (i = 0; i < 2; i++) {
		printf("msg addr: %p\n", &test_data[i]);
		QUEUE_PUSH(msgs, &test_data[i]);
		printf("front: %p, back: %p, size: %d, "
			   "frontqh: %p, backqh: %p\n",
					msgs->qh.q->front,
					msgs->qh.q->back,
					QUEUE_SIZE(msgs),
					(queue_handle_t *)(msgs->qh.q->frontqh),
					(queue_handle_t *)(msgs->qh.q->backqh));
	}

	printf("\n");

	for (i = 0; i < 1; i++) {
		QUEUE_POP(msgs, current);
		printf("got message with addr: %p - content: %s\n", current, current->content);
		printf("msgs debug: %p %p %d\n", msgs->qh.q->front, msgs->qh.q->back, msgs->qh.q->size);
	}

	for (i = 2; i < 4; i++) {
		printf("msg addr: %p\n", &test_data[i]);
		QUEUE_PUSH(msgs, &test_data[i]);
		printf("front: %p, back: %p, size: %d, "
			   "frontqh: %p, backqh: %p\n",
					msgs->qh.q->front,
					msgs->qh.q->back,
					msgs->qh.q->size,
					(queue_handle_t *)(msgs->qh.q->frontqh),
					(queue_handle_t *)(msgs->qh.q->backqh));
	}

	for (i = 2; i < 4; i++) {
		QUEUE_POP(msgs, current);
		printf("got message with addr: %p - content: %s\n", current, current->content);
		printf("msgs debug: %p %p %d\n", msgs->qh.q->front, msgs->qh.q->back, msgs->qh.q->size);
	}

	printf("\n");

/*
	msg = malloc(sizeof (msg_t));
	msg->content = "scuse juice";
	QUEUE_PUSH(msgs, msg);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));

	QUEUE_POP(msgs, msg);
	printf("%s\n", msg->content);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));
	QUEUE_POP(msgs, msg);
	printf("%s\n", msg->content);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));

	msg = malloc(sizeof (msg_t));
	msg->content = "ooyou";
	QUEUE_PUSH(msgs, msg);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));

	msg_t *lada = malloc(sizeof (msg_t));
	lada->content = "ooyou";

	QUEUE_POP(msgs, lada);
	printf("%s\n", lada->content);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));

	msg_t *bein;

	QUEUE_POP(msgs, bein);
	printf("%s\n", bein->content);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));
	QUEUE_POP(msgs, bein);
	printf("%s\n", bein->content);
	printf("queue size: %d\n", QUEUE_SIZE(msgs));
*/

	// freeing queue does not free queue elements - this has to be done by user
	QUEUE_FREE(msgs);

	return 0;
}
