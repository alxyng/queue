#include <stdlib.h>
#include <check.h>
#include "queue.h"

struct msg {
    char *content;
    queue_handle qh;
};

START_TEST(test_queue_init) {
    struct msg *msgs;

    QUEUE_INIT(struct msg, msgs);

    ck_assert_ptr_ne(msgs, NULL);
    ck_assert_ptr_ne(msgs->qh.qc, NULL);

    ck_assert_ptr_eq(msgs->qh.qc->front, NULL);
    ck_assert_ptr_eq(msgs->qh.qc->back, NULL);
    ck_assert_ptr_eq(msgs->qh.qc->backqh, NULL);

    ck_assert_uint_eq(msgs->qh.qc->size, 0);

    ck_assert_ptr_eq(msgs->qh.next, NULL);

    QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_push) {
    struct msg *msgs;
    struct msg m1, m2, m3;

    QUEUE_INIT(struct msg, msgs);

    m1.content = "abc";
    m2.content = "def";
    m3.content = "ghi";

    QUEUE_PUSH(msgs, &m1);

    ck_assert_ptr_eq(msgs->qh.qc, m1.qh.qc);

    ck_assert_ptr_eq(msgs->qh.qc->front, &m1);
    ck_assert_ptr_eq(msgs->qh.qc->back, &m1);
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &m1.qh);
    ck_assert_uint_eq(msgs->qh.qc->size, 1);
    ck_assert_ptr_eq(msgs->qh.next, NULL);

    QUEUE_PUSH(msgs, &m2);

    ck_assert_ptr_eq(msgs->qh.qc, m2.qh.qc);
    ck_assert_ptr_eq(msgs->qh.qc->front, &m1);
    ck_assert_ptr_eq(msgs->qh.qc->back, &m2);
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &m2.qh);
    ck_assert_uint_eq(msgs->qh.qc->size, 2);
    ck_assert_ptr_eq(m1.qh.next, &m2);
    ck_assert_ptr_eq(m2.qh.next, NULL);

    QUEUE_PUSH(msgs, &m3);

    ck_assert_ptr_eq(msgs->qh.qc, m3.qh.qc);
    ck_assert_ptr_eq(msgs->qh.qc->front, &m1);
    ck_assert_ptr_eq(msgs->qh.qc->back, &m3);
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &m3.qh);
    ck_assert_uint_eq(msgs->qh.qc->size, 3);
    ck_assert_ptr_eq(m1.qh.next, &m2);
    ck_assert_ptr_eq(m2.qh.next, &m3);
    ck_assert_ptr_eq(m3.qh.next, NULL);

    QUEUE_FREE(msgs);
} END_TEST

START_TEST(test_queue_size) {
    struct msg *msgs;
    struct msg m1, m2, m3;

    QUEUE_INIT(struct msg, msgs);

    m1.content = "abc";
    m2.content = "def";
    m3.content = "ghi";

    ck_assert_uint_eq(QUEUE_SIZE(msgs), 0);

    QUEUE_PUSH(msgs, &m1);
    ck_assert_uint_eq(QUEUE_SIZE(msgs), 1);

    QUEUE_PUSH(msgs, &m2);
    ck_assert_uint_eq(QUEUE_SIZE(msgs), 2);

    QUEUE_PUSH(msgs, &m3);
    ck_assert_uint_eq(QUEUE_SIZE(msgs), 3);
} END_TEST

START_TEST(test_queue_pop) {
    struct msg *msgs;
    struct msg src1, src2, src3, *dst1, *dst2, *dst3;

    QUEUE_INIT(struct msg, msgs);

    src1.content = "abc";
    src2.content = "def";
    src3.content = "ghi";

    QUEUE_PUSH(msgs, &src1);

    QUEUE_POP(msgs, dst1);
    ck_assert_ptr_eq(dst1, &src1);
    ck_assert_ptr_eq(msgs->qh.qc->front, NULL);
    ck_assert_ptr_eq(msgs->qh.qc->back, &src1); // unchanged w/ pop
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &src1.qh); // unchanged w/ pop
    ck_assert_uint_eq(msgs->qh.qc->size, 0);
    ck_assert_ptr_eq(dst1->qh.next, msgs->qh.qc->front);
    ck_assert_str_eq(src1.content, dst1->content);

    QUEUE_PUSH(msgs, &src1);
    QUEUE_PUSH(msgs, &src2);
    QUEUE_POP(msgs, dst1);
    ck_assert_ptr_eq(dst1, &src1);
    ck_assert_ptr_eq(msgs->qh.qc->front, &src2);
    ck_assert_ptr_eq(msgs->qh.qc->back, &src2); // unchanged w/ pop
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &src2.qh); // unchanged w/ pop
    ck_assert_uint_eq(msgs->qh.qc->size, 1);
    ck_assert_ptr_eq(dst1->qh.next, msgs->qh.qc->front);
    ck_assert_str_eq(src1.content, dst1->content);
    QUEUE_POP(msgs, dst2);
    ck_assert_ptr_eq(dst2, &src2);
    ck_assert_ptr_eq(msgs->qh.qc->front, NULL);
    ck_assert_ptr_eq(msgs->qh.qc->back, &src2); // unchanged w/ pop
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &src2.qh); // unchanged w/ pop
    ck_assert_uint_eq(msgs->qh.qc->size, 0);
    ck_assert_ptr_eq(dst2->qh.next, msgs->qh.qc->front);
    ck_assert_str_eq(src2.content, dst2->content);

    QUEUE_PUSH(msgs, &src1);
    QUEUE_PUSH(msgs, &src2);
    QUEUE_PUSH(msgs, &src3);
    QUEUE_POP(msgs, dst1);
    ck_assert_ptr_eq(dst1, &src1);
    ck_assert_ptr_eq(msgs->qh.qc->front, &src2);
    ck_assert_ptr_eq(msgs->qh.qc->back, &src3); // unchanged w/ pop
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &src3.qh); // unchanged w/ pop
    ck_assert_uint_eq(msgs->qh.qc->size, 2);
    ck_assert_ptr_eq(dst1->qh.next, msgs->qh.qc->front);
    ck_assert_str_eq(src1.content, dst1->content);
    QUEUE_POP(msgs, dst2);
    ck_assert_ptr_eq(dst2, &src2);
    ck_assert_ptr_eq(msgs->qh.qc->front, &src3);
    ck_assert_ptr_eq(msgs->qh.qc->back, &src3); // unchanged w/ pop
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &src3.qh); // unchanged w/ pop
    ck_assert_uint_eq(msgs->qh.qc->size, 1);
    ck_assert_ptr_eq(dst2->qh.next, msgs->qh.qc->front);
    ck_assert_str_eq(src2.content, dst2->content);
    QUEUE_POP(msgs, dst3);
    ck_assert_ptr_eq(dst3, &src3);
    ck_assert_ptr_eq(msgs->qh.qc->front, NULL);
    ck_assert_ptr_eq(msgs->qh.qc->back, &src3); // unchanged w/ pop
    ck_assert_ptr_eq(msgs->qh.qc->backqh, &src3.qh); // unchanged w/ pop
    ck_assert_uint_eq(msgs->qh.qc->size, 0);
    ck_assert_ptr_eq(dst3->qh.next, msgs->qh.qc->front);
    ck_assert_str_eq(src3.content, dst3->content);

    QUEUE_FREE(msgs);
} END_TEST

/* Expected failure due to 'next' being overwritten
START_TEST(test_queue_push_same_element) {
    struct msg *msgs;
    struct msg src1, *src2, *dst;

    QUEUE_INIT(struct msg, msgs);

    src1.content = "abc";
    src2 = &src1;

    QUEUE_PUSH(msgs, &src1);
    QUEUE_PUSH(msgs, src2);

    ck_assert(msgs->qh.qc == src1.qh.qc);
    ck_assert(msgs->qh.qc == src2->qh.qc);

    ck_assert(msgs->qh.qc->front == &src1);
    ck_assert(msgs->qh.qc->back == src2);
    ck_assert(msgs->qh.qc->backqh == &src2->qh);

    ck_assert(src1.qh.next == src2);
    ck_assert(src2->qh.next == NULL);

    QUEUE_FREE(msgs);
} END_TEST
*/

Suite *queue_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("queue");

    // Core test case
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_queue_init);
    tcase_add_test(tc_core, test_queue_push);
    tcase_add_test(tc_core, test_queue_size);
    tcase_add_test(tc_core, test_queue_pop);
    //tcase_add_test(tc_core, test_queue_push_same_element);
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
