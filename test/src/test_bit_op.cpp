#pragma once
#include "../ouchitest/include/ouchitest.hpp"
#include "common.hpp"

OUCHI_TEST_CASE(test_shiftr_array)
{
    unsigned char data[] = { 1, 127 << 1, 0 };
    ciao::shiftr_array(data, 9);
    OUCHI_CHECK_EQUAL((unsigned)data[2], 255u);
    OUCHI_CHECK_EQUAL(data[1], 0u);
    OUCHI_CHECK_EQUAL(data[0], 0u);
}

OUCHI_TEST_CASE(test_shiftl_array)
{
    unsigned char data[] = { 0xf1,0xff };
    ciao::shiftl_array(data, 9);
    OUCHI_CHECK_EQUAL((unsigned)data[0], 0b1111'1110);
    OUCHI_CHECK_EQUAL((unsigned)data[1], 0u);
}

OUCHI_TEST_CASE(test_rot_array)
{
    unsigned char data[16];
    std::memset(data, 0b1010'1010, 16);
    ciao::rotl_array(data, 111);
    for (int i = 0; i < 16; ++i) {
        OUCHI_REQUIRE_EQUAL((unsigned)data[i], 0b0101'0101U);
    }
}

