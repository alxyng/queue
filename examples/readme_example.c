#include <stdio.h>
#include "queue.h"

struct msg {
	char *content;
	queue_handle qh;
}

int main(void) {
	struct msg *queue;
	struct msg m1, m2;

	queue = NULL;

	m1.content = "abc";
	QUEUE_PUSH(queue, &m1);

	printf("queue size: %d\n", QUEUE_SIZE(queue)); // queue size: 1

	QUEUE_POP(queue, &m2);
	printf("m2 content: %s\n", m2.content); // m2 content: abc

	QUEUE_FREE(queue);

	return 0;
}
