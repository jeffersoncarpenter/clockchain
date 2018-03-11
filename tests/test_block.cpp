#include <vector>
using std::vector;

#include "../src/block.h"

#include "test_block.h"

void test_set_big_block_array_helper(uint64_t testNum, vector<uint8_t> expectedBytes) {
    Big_block_array x;
    uint8_t minSize = expectedBytes.size();

    set_big_block_array(x, testNum);
    uint8_t size = x.data.size();
    ck_assert_uint_le(minSize, size);
    for (uint8_t i = 0; i < minSize; i++) {
        ck_assert_uint_eq(expectedBytes[i], x.data[i]);
    }
    for (uint8_t i = minSize; i < size; i++) {
        ck_assert_uint_eq(uint8_t(0), x.data[i]);
    }
}

START_TEST(test_set_big_block_array)
{
    test_set_big_block_array_helper(0, {});
    test_set_big_block_array_helper(1, {1});
    test_set_big_block_array_helper(255, {255});
    test_set_big_block_array_helper(256, {0, 1});
    test_set_big_block_array_helper(14613451, {0xCB, 0xFB, 0xDE});
}
END_TEST

START_TEST(test_get_big_block_array)
{
    Big_block_array x;
    uint64_t val;

    // 0
    x.data = {};
    get_big_block_array(val, x);
    ck_assert_uint_eq(0, val);

    // 1
    x.data = {1};
    get_big_block_array(val, x);
    ck_assert_uint_eq(1, val);

    // 255
    x.data = {255};
    get_big_block_array(val, x);
    ck_assert_uint_eq(255, val);

    // 256
    x.data = {0, 1};
    get_big_block_array(val, x);
    ck_assert_uint_eq(256, val);
}
END_TEST

START_TEST(test_increment_big_block_array)
{
    Big_block_array x;

    // 0
    set_big_block_array(x, 0);
    increment_big_block_array(x);
    ck_assert_uint_eq(1, x.data.size());
    ck_assert_uint_eq(uint8_t(1), x.data[0]);

    // 255
    set_big_block_array(x, 255);
    increment_big_block_array(x);
    ck_assert_uint_eq(2, x.data.size());
    ck_assert_uint_eq(uint8_t(0), x.data[0]);
    ck_assert_uint_eq(uint8_t(1), x.data[1]);

    // 256
    set_big_block_array(x, 256);
    increment_big_block_array(x);
    ck_assert_uint_eq(2, x.data.size());
    ck_assert_uint_eq(uint8_t(1), x.data[0]);
    ck_assert_uint_eq(uint8_t(1), x.data[1]);
}
END_TEST

START_TEST(test_serialize_big_block_array)
{
    Big_block_array x;
    byte *vec;

    // 0
    set_big_block_array(x, 0);
    serialize_big_block_array(vec, x);
    ck_assert_uint_eq(1, vec[0]);
    ck_assert_uint_eq(0, vec[1]);
    delete[] vec;

    // 1
    set_big_block_array(x, 1);
    serialize_big_block_array(vec, x);
    ck_assert_uint_eq(1, vec[0]);
    ck_assert_uint_eq(1, vec[1]);
    delete[] vec;

    // 256
    set_big_block_array(x, 256);
    serialize_big_block_array(vec, x);
    ck_assert_uint_eq(2, vec[0]);
    ck_assert_uint_eq(0, vec[1]);
    ck_assert_uint_eq(1, vec[2]);
    delete[] vec;

    // Not tested: serialization of integers longer than 2048 bits.
}
END_TEST

START_TEST(test_deserialize_big_block_array)
{
    Big_block_array x;
    uint64_t val;

    // 0
    uint8_t vec_1[] = {1, 0};
    deserialize_big_block_array(x, vec_1);
    get_big_block_array(val, x);
    ck_assert_uint_eq(0, val);

    // 1
    uint8_t vec_2[] = {1, 1};
    deserialize_big_block_array(x, vec_2);
    get_big_block_array(val, x);
    ck_assert_uint_eq(1, val);

    // 255
    uint8_t vec_3[] = {1, 255};
    deserialize_big_block_array(x, vec_3);
    get_big_block_array(val, x);
    ck_assert_uint_eq(255, val);

    // 256
    uint8_t vec_4[] = {2, 0, 1};
    deserialize_big_block_array(x, vec_4);
    get_big_block_array(val, x);
    ck_assert_uint_eq(256, val);

    // Not tested: serialization of integers longer than 2048 bits.
}
END_TEST

Suite *block_suite() {
    Suite *s;
    TCase *tc_block;

    s = suite_create("Clockchain");

    tc_block = tcase_create("Block");
    // big block array
    tcase_add_test(tc_block, test_set_big_block_array);
    tcase_add_test(tc_block, test_get_big_block_array);
    tcase_add_test(tc_block, test_increment_big_block_array);
    tcase_add_test(tc_block, test_serialize_big_block_array);
    tcase_add_test(tc_block, test_deserialize_big_block_array);
    suite_add_tcase(s, tc_block);

    return s;
}
