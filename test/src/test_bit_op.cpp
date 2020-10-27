#pragma once
#include "../ouchitest/include/ouchitest.hpp"
#include "common.hpp"


OUCHI_TEST_CASE(test_shiftr_array)
{
    using namespace ciao::big_endian;
    unsigned char data[] = { 1, 127 << 1, 0 };
    shiftr_array(data, 9);
    OUCHI_CHECK_EQUAL((unsigned)data[2], 255u);
    OUCHI_CHECK_EQUAL(data[1], 0u);
    OUCHI_CHECK_EQUAL(data[0], 0u);
}

OUCHI_TEST_CASE(test_shiftl_array)
{
    using namespace ciao::big_endian;
    unsigned char data[] = { 0xf1,0xff };
    shiftl_array(data, 9);
    OUCHI_CHECK_EQUAL((unsigned)data[0], 0b1111'1110);
    OUCHI_CHECK_EQUAL((unsigned)data[1], 0u);
}

OUCHI_TEST_CASE(test_shiftl_array2)
{
    using namespace ciao::big_endian;
    unsigned char data[] = { 0xf1,0xff };
    shiftl_array(data, 8);
    OUCHI_CHECK_EQUAL((unsigned)data[0], 0b1111'1111);
    OUCHI_CHECK_EQUAL((unsigned)data[1], 0u);
}

OUCHI_TEST_CASE(test_rot_array)
{
    using namespace ciao::big_endian;
    unsigned char data[16];
    std::memset(data, 0b1010'1010, 16);
    rotl_array(data, 111);
    for (int i = 0; i < 16; ++i) {
        OUCHI_REQUIRE_EQUAL((unsigned)data[i], 0b0101'0101U);
    }
}

OUCHI_TEST_CASE(test_shiftr_array_native)
{
    using namespace ciao::native_endian;
    constexpr std::uint64_t c = 0x0123'4567'89ab'cdefu;
    union {
        std::uint64_t q;
        std::uint16_t w[4];
    }n;
    for (int i = 0; i < 63; ++i) {
        n.q = c;
        shiftr_array(n.w, i);
        OUCHI_CHECK_EQUAL(n.q, c >> i);
    }

}

OUCHI_TEST_CASE(test_shiftl_array_native)
{
    using namespace ciao::native_endian;
    union {
        std::uint64_t q;
        std::uint16_t w[4];
    }n;
    constexpr std::uint64_t c = 0x0123'4567'89ab'cdefu;
    for (int i = 0; i < 63; ++i) {
        n.q = c;
        shiftl_array(n.w, i);
        OUCHI_CHECK_EQUAL(n.q, c << i);
    }
}

