# queue

A type-agnostic header-only macro-based struct queue implementation in C.

## Usage

To create a queueable struct, include the `queue_handle qh` member in your
struct.

Before using QUEUE_PUSH, ensure that the queue struct is set to NULL, this is
the only initialisation that needs to be done.

This queue implementation is not thread safe.

Freeing the queue does not free every element in the queue if they have been
dynamically allocated. This has to be done by popping all the elements in the
queue and freeing them manually.

Pushing the same element (at the same address) into the queue is not supported.
This is because the `next` member of the queue's `queue_handle` will be
overwitten. This will case undefined behaviour when pushing or popping to or
from the queue (see commented out test case).

```c
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

	msgs = NULL;

	m1.content = "abc";
	QUEUE_PUSH(msgs, &m1);

	printf("msgs size: %d\n", QUEUE_SIZE(msgs)); // msgs size: 1

	QUEUE_POP(msgs, m2);
	printf("m2 content: %s\n", m2->content); // m2 content: abc

	QUEUE_FREE(msgs);

	return EXIT_SUCCESS;
}
```
