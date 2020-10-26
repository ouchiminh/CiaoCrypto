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
    unsigned char data[] = { 0xf1,0xfc };
    ciao::shiftl_array(data, 4);
    OUCHI_CHECK_EQUAL((unsigned)data[0], 0x1fu);
    OUCHI_CHECK_EQUAL((unsigned)data[1], 0xc0u);
}

