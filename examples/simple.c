#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct msg {
	char *content;
	queue_handle qh;
};

int main(void) {
	struct msg *msgs; // message queue
	struct msg m1, *m2;

	msgs = malloc(sizeof (struct msg));
	QUEUE_INIT(msgs);

	m1.content = "abc";
	QUEUE_PUSH(msgs, &m1);

	printf("msgs size: %d\n", QUEUE_SIZE(msgs)); // msgs size: 1

	QUEUE_POP(msgs, m2);
	printf("m2 content: %s\n", m2->content); // m2 content: abc

	QUEUE_FREE(msgs);

	return EXIT_SUCCESS;
}
