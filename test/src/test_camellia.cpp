#pragma once
#include <chrono>
#include <vector>
#include "../ouchitest/include/ouchitest.hpp"
#include "common.hpp"
#include "algorithm/camellia.hpp"

namespace {
    const std::uint8_t key[] =
    {
        0x01,0x23,0x45,0x67,
        0x89,0xab,0xcd,0xef,
        0xfe,0xdc,0xba,0x98,
        0x76,0x54,0x32,0x10,
        0x00,0x11,0x22,0x33,
        0x44,0x55,0x66,0x77,
        0x88,0x99,0xaa,0xbb,
        0xcc,0xdd,0xee,0xff
    };
}

OUCHI_TEST_CASE(test_camellia128)
{
    ciao::camellia<16> encoder;
    std::uint8_t block[16];
    encoder.key_schedule(key);
    std::memcpy(block, key, 16);
    encoder.cipher(block);
    OUCHI_CHECK_EQUAL((unsigned)block[0], 0x67u);
    OUCHI_CHECK_EQUAL((unsigned)block[1], 0x67u);
    OUCHI_CHECK_EQUAL((unsigned)block[15], 0x43u);
    encoder.inv_cipher(block);
    OUCHI_CHECK_EQUAL((unsigned)block[0], 0x01);
}

OUCHI_TEST_CASE(test_camellia192)
{
    ciao::camellia<24> encoder;
    std::uint8_t block[16];
    encoder.key_schedule(key);
    std::memcpy(block, key, 16);
    encoder.cipher(block);
    OUCHI_CHECK_EQUAL((unsigned)block[0], 0xb4u);
    OUCHI_CHECK_EQUAL((unsigned)block[1], 0x99u);
    OUCHI_CHECK_EQUAL((unsigned)block[15], 0xb9u);
    encoder.inv_cipher(block);
    OUCHI_CHECK_EQUAL((unsigned)block[0], 0x01);
}

OUCHI_TEST_CASE(test_camellia256)
{
    ciao::camellia<32> encoder;
    std::uint8_t block[16];
    encoder.key_schedule(key);
    std::memcpy(block, key, 16);
    encoder.cipher(block);
    OUCHI_CHECK_EQUAL((unsigned)block[0], 0x9au);
    OUCHI_CHECK_EQUAL((unsigned)block[1], 0xccu);
    OUCHI_CHECK_EQUAL((unsigned)block[15], 0x09u);
    encoder.inv_cipher(block);
    OUCHI_CHECK_EQUAL((unsigned)block[0], 0x01);
}

#ifdef NDEBUG

OUCHI_TEST_CASE(benchmark_camellia128)
{
    using namespace std::chrono;
    constexpr int r = 1024 * 8;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    ciao::camellia<16> encoder{ key };
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16) {
                encoder.cipher(data.data() + i);
            }
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "camellia-128 enc " <<  data.size()*r / dur.count() / 1000 << " k\n";
    }
}

#endif

