#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <check.h>
#include "queue.h"

struct msg {
	char *content;
	queue_handle qh;
};

START_TEST(test_queue_size_null) {
	struct msg *msgs; // message queue

	msgs = NULL;

	ck_assert(msgs == NULL);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 0);
} END_TEST

START_TEST(test_queue_free_null) {
	struct msg *msgs; // message queue

	QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_push) {
	struct msg *msgs; // message queue
	struct msg m;

	m.content = "abc";

	QUEUE_PUSH(msgs, &m);

	ck_assert(msgs != NULL);

	ck_assert(msgs->qh.q == m.qh.q);

	ck_assert(msgs->qh.q->front == &m);
	ck_assert(msgs->qh.q->front == msgs->qh.q->back);

	ck_assert(msgs->qh.q->backqh == &((struct msg *)msgs->qh.q->back)->qh);

	ck_assert_int_eq(msgs->qh.q->size, 1);
	ck_assert(msgs->qh.next == NULL);

	QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_size) {
	struct msg *msgs; // message queue
	struct msg m1, m2;

	m1.content = "abc";
	m2.content = "def";

	QUEUE_PUSH(msgs, &m1);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 1);
	QUEUE_PUSH(msgs, &m2);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 2);

	QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_push_2) {
	struct msg *msgs; // message queue
	struct msg m1, *m2;

	m2 = malloc(sizeof (struct msg));

	m1.content = "abc";
	m2->content = "def";

	QUEUE_PUSH(msgs, &m1);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 1);
	QUEUE_PUSH(msgs, m2);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 2);

	ck_assert(msgs->qh.q == m1.qh.q);
	ck_assert(msgs->qh.q == m2->qh.q);

	ck_assert(msgs->qh.q->front == &m1);

	ck_assert(m1.qh.next == m2);
	ck_assert(m2->qh.next == NULL);

	ck_assert(msgs->qh.q->back == m2);
	ck_assert(msgs->qh.q->backqh == &m2->qh);

	QUEUE_FREE(msgs);

	free(m2);
} END_TEST

START_TEST(test_queue_push_5) {
	struct msg *msgs; // message queue
	struct msg m1, *m2, *m3, m4, m5;

	m2 = malloc(sizeof (struct msg));
	m3 = malloc(sizeof (struct msg));

	m1.content = "abc";
	m2->content = "def";
	m3->content = "ghi";
	m4.content = "jkl";
	m5.content = "mno";

	QUEUE_PUSH(msgs, &m1);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 1);
	QUEUE_PUSH(msgs, m2);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 2);
	QUEUE_PUSH(msgs, m3);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 3);
	QUEUE_PUSH(msgs, &m4);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 4);
	QUEUE_PUSH(msgs, &m5);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 5);

	ck_assert(msgs->qh.q == m1.qh.q);
	ck_assert(msgs->qh.q == m2->qh.q);
	ck_assert(msgs->qh.q == m3->qh.q);
	ck_assert(msgs->qh.q == m4.qh.q);
	ck_assert(msgs->qh.q == m5.qh.q);

	ck_assert(msgs->qh.q->front == &m1);

	ck_assert(m1.qh.next == m2);
	ck_assert(m2->qh.next == m3);
	ck_assert(m3->qh.next == &m4);
	ck_assert(m4.qh.next == &m5);
	ck_assert(m5.qh.next == NULL);

	ck_assert(msgs->qh.q->back == &m5);
	ck_assert(msgs->qh.q->backqh == &m5.qh);

	QUEUE_FREE(msgs);

	free(m2);
	free(m3);
} END_TEST

START_TEST(test_queue_pop_empty) {
	struct msg *msgs; // message queue
	struct msg *dst;

	QUEUE_POP(msgs, dst);
	ck_assert(dst == NULL);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 0);

	QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_pop) {
	struct msg *msgs; // message queue
	struct msg src, *dst;

	src.content = "abc";

	QUEUE_PUSH(msgs, &src);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 1);

	QUEUE_POP(msgs, dst);

	ck_assert(msgs->qh.q->front == NULL);

	ck_assert(dst == &src);
	ck_assert_str_eq(dst->content, src.content);
	ck_assert(&dst->qh == &src.qh);

	ck_assert_int_eq(QUEUE_SIZE(msgs), 0);

	QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_push_pop_push_pop) {
	struct msg *msgs; // message queue
	struct msg src1, src2, *dst1, *dst2;

	src1.content = "abc";

	QUEUE_PUSH(msgs, &src1);
	QUEUE_POP(msgs, dst1);
	QUEUE_PUSH(msgs, &src2);
	QUEUE_POP(msgs, dst2);

	ck_assert(dst1 == &src1);
	ck_assert(dst2 == &src2);

	ck_assert_int_eq(QUEUE_SIZE(msgs), 0);

	QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_pop_5) {
	struct msg *msgs; // message queue
	struct msg src1, *src2, *src3, src4, src5;
	struct msg *dst1, *dst2, *dst3, *dst4, *dst5;

	src2 = malloc(sizeof (struct msg));
	src3 = malloc(sizeof (struct msg));

	src1.content = "abc";
	src2->content = "def";
	src3->content = "ghi";
	src4.content = "jkl";
	src5.content = "mno";

	QUEUE_PUSH(msgs, &src1);
	QUEUE_PUSH(msgs, src2);
	QUEUE_PUSH(msgs, src3);
	QUEUE_PUSH(msgs, &src4);
	QUEUE_PUSH(msgs, &src5);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 5);
	ck_assert(msgs->qh.q->front == &src1);

	QUEUE_POP(msgs, dst1);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 4);
	ck_assert(dst1 == &src1);
	ck_assert(msgs->qh.q->front == src2);

	QUEUE_POP(msgs, dst2);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 3);
	ck_assert(dst2 == src2);
	ck_assert(msgs->qh.q->front == src3);

	QUEUE_POP(msgs, dst3);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 2);
	ck_assert(dst3 == src3);
	ck_assert(msgs->qh.q->front == &src4);

	QUEUE_POP(msgs, dst4);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 1);
	ck_assert(dst4 == &src4);
	ck_assert(msgs->qh.q->front == &src5);

	QUEUE_POP(msgs, dst5);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 0);
	ck_assert(dst5 == &src5);
	ck_assert(msgs->qh.q->front == NULL);

	QUEUE_FREE(msgs);
} END_TEST

/* Expected failure due to 'next' being overwritten
START_TEST(test_queue_push_push_pop_same_element) {
	struct msg *msgs; // message queue
	struct msg src1, *src2, *dst;

	src1.content = "abc";
	src2 = &src1;

	QUEUE_PUSH(msgs, &src1);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 1);
	QUEUE_PUSH(msgs, src2);
	ck_assert_int_eq(QUEUE_SIZE(msgs), 2);

	ck_assert(msgs->qh.q == src1.qh.q);
	ck_assert(msgs->qh.q == src2->qh.q);

	ck_assert(msgs->qh.q->front == &src1);
	ck_assert(msgs->qh.q->back == src2);
	ck_assert(msgs->qh.q->backqh == &src2->qh);

	ck_assert(src1.qh.next == &src1);
	ck_assert(src2->qh.next == NULL);

	QUEUE_FREE(msgs);
} END_TEST
*/

Suite *queue_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("queue");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_queue_size_null);
	tcase_add_test(tc_core, test_queue_free_null);
	tcase_add_test(tc_core, test_queue_size);
	tcase_add_test(tc_core, test_queue_push);
	tcase_add_test(tc_core, test_queue_push_2);
	tcase_add_test(tc_core, test_queue_push_5);
	tcase_add_test(tc_core, test_queue_pop_empty);
	tcase_add_test(tc_core, test_queue_pop);
	tcase_add_test(tc_core, test_queue_pop_5);
	//tcase_add_test(tc_core, test_queue_push_push_pop_same_element);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = queue_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
