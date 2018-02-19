#include <vector>

extern "C" {
#include <check.h>
}

#include "../src/block.h"

START_TEST(test_set_big_block_array)
{
    Big_block_array x;

    // 0
    set_big_block_array(&x, 0);
    ck_assert_uint_eq(0, x.data.size());

    // 1
    set_big_block_array(&x, 1);
    ck_assert_uint_eq(1, x.data.size());
    ck_assert_uint_eq(uint8_t(1), x.data[0]);

    // 255
    set_big_block_array(&x, 255);
    ck_assert_uint_eq(1, x.data.size());
    ck_assert_uint_eq(uint8_t(255), x.data[0]);

    // 256
    set_big_block_array(&x, 256);
    ck_assert_uint_eq(2, x.data.size());
    ck_assert_uint_eq(uint8_t(0), x.data[0]);
    ck_assert_uint_eq(uint8_t(1), x.data[1]);

    // somewhat random number
    set_big_block_array(&x, 14613451);
    ck_assert_uint_eq(3, x.data.size());
    ck_assert_uint_eq(uint8_t(0xCB), x.data[0]);
    ck_assert_uint_eq(uint8_t(0xFB), x.data[1]);
    ck_assert_uint_eq(uint8_t(0xDE), x.data[2]);
}
END_TEST

START_TEST(test_get_big_block_array)
{
    Big_block_array x;
    uint64_t val;

    // 0
    x.data = {};
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(0, val);

    // 1
    x.data = {1};
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(1, val);

    // 255
    x.data = {255};
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(255, val);

    // 256
    x.data = {0, 1};
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(256, val);
}
END_TEST

START_TEST(test_increment_big_block_array)
{
    Big_block_array x;

    // 0
    set_big_block_array(&x, 0);
    increment_big_block_array(&x);
    ck_assert_uint_eq(1, x.data.size());
    ck_assert_uint_eq(uint8_t(1), x.data[0]);

    // 255
    set_big_block_array(&x, 255);
    increment_big_block_array(&x);
    ck_assert_uint_eq(2, x.data.size());
    ck_assert_uint_eq(uint8_t(0), x.data[0]);
    ck_assert_uint_eq(uint8_t(1), x.data[1]);

    // 256
    set_big_block_array(&x, 256);
    increment_big_block_array(&x);
    ck_assert_uint_eq(2, x.data.size());
    ck_assert_uint_eq(uint8_t(1), x.data[0]);
    ck_assert_uint_eq(uint8_t(1), x.data[1]);
}
END_TEST

START_TEST(test_serialize_big_block_array)
{
    Big_block_array x;
    std::vector<byte> vec;

    // 0
    set_big_block_array(&x, 0);
    serialize_big_block_array(&vec, &x);
    ck_assert_uint_eq(2, vec.size());
    ck_assert_uint_eq(1, vec[0]);
    ck_assert_uint_eq(0, vec[1]);

    // 1
    set_big_block_array(&x, 1);
    serialize_big_block_array(&vec, &x);
    ck_assert_uint_eq(2, vec.size());
    ck_assert_uint_eq(1, vec[0]);
    ck_assert_uint_eq(1, vec[1]);

    // 256
    set_big_block_array(&x, 256);
    serialize_big_block_array(&vec, &x);
    ck_assert_uint_eq(3, vec.size());
    ck_assert_uint_eq(2, vec[0]);
    ck_assert_uint_eq(0, vec[1]);
    ck_assert_uint_eq(1, vec[2]);

    // Not tested: serialization of integers longer than 2048 bits.
}
END_TEST

START_TEST(test_deserialize_big_block_array)
{
    Big_block_array x;
    std::vector<byte> vec;
    uint64_t val;

    // 0
    vec = {1, 0};
    deserialize_big_block_array(&x, &vec);
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(0, val);

    // 1
    vec = {1, 1};
    deserialize_big_block_array(&x, &vec);
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(1, val);

    // 255
    vec = {1, 255};
    deserialize_big_block_array(&x, &vec);
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(255, val);

    // 256
    vec = {2, 0, 1};
    deserialize_big_block_array(&x, &vec);
    get_big_block_array(&val, &x);
    ck_assert_uint_eq(256, val);

    // Not tested: serialization of integers longer than 2048 bits.
}
END_TEST

Suite *clockchain_suite() {
    Suite *s;
    TCase *tc_block;

    s = suite_create("Clockchain");

    tc_block = tcase_create("Block");
    tcase_add_test(tc_block, test_set_big_block_array);
    tcase_add_test(tc_block, test_get_big_block_array);
    tcase_add_test(tc_block, test_increment_big_block_array);
    tcase_add_test(tc_block, test_serialize_big_block_array);
    tcase_add_test(tc_block, test_deserialize_big_block_array);
    suite_add_tcase(s, tc_block);

    return s;
}

int main() {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = clockchain_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
