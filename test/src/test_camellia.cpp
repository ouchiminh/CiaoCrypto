#pragma once
#include <chrono>
#include <vector>
#include "../ouchitest/include/ouchitest.hpp"
#include "common.hpp"
#include "block_cipher_mode.hpp"
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
    OUCHI_CHECK_EQUAL((unsigned)block[15], 0x10);
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
    OUCHI_CHECK_EQUAL((unsigned)block[15], 0x10);
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
    OUCHI_CHECK_EQUAL((unsigned)block[15], 0x10);
}

#if 0

#include <iostream>
OUCHI_TEST_CASE(camellia_sp_table_gen)
{
    int tbl[] = { 1,2,3,4,2,3,4,1 };
    std::uint64_t mask[] = {
        0xFFFFFF00FF0000FF,0x00FFFFFFFFFF0000,0xFF00FFFF00FFFF00,0xFFFF00FF0000FFFF,
        0x00FFFFFF00FFFFFF,0xFF00FFFFFF00FFFF,0xFFFF00FFFFFF00FF,0xFFFFFF00FFFFFF00
    };

    for (int i = 0; i < 8; ++i) {
        std::uint64_t q;
        std::cout << '{' << '\n';
        for (int j = 0; j < 256; ++j) {
            std::memset(&q, ciao::detail::camellia_sbox[tbl[i]-1][j], sizeof(q));
            std::printf("0x%016llx, ", q & mask[i]);
            if (((j+1) & 0x3) == 0) std::cout << '\n';
        }
        std::cout << "\n},\n";
    }
}

#endif

#ifdef NDEBUG

OUCHI_TEST_CASE(benchmark_camellia128)
{
    using namespace std::chrono;
    constexpr int r = 1024 * 16;
    constexpr int c = 16 * 1024;
    alignas(16) static std::uint8_t data[c] = {};
    ciao::camellia<16> encoder{ key };
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < c; i += 16) {
                encoder.cipher(data + i);
            }
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "camellia-128 enc " <<  c / dur.count() / 1000*r << " k\n";
    }
}

OUCHI_TEST_CASE(benchmark_camellia128_ctr)
{
    using namespace std::chrono;
    const unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 8;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    std::vector<std::uint8_t> dest(16 * 1025);
    ciao::ctr<ciao::camellia<16>> encoder(key, 0ull, key);
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            (void)encoder.cipher(data.data(), data.size(), dest.data(), dest.size());
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "camellia-128 ctr enc " <<  data.size() / dur.count() *r/ 1000 << " k\n";
    }

}
OUCHI_TEST_CASE(benchmark_camellia256_ctr)
{
    using namespace std::chrono;
    const unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 8;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    std::vector<std::uint8_t> dest(16 * 1025);
    ciao::ctr<ciao::camellia<32>> encoder(key, 0ull, key);
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            (void)encoder.cipher(data.data(), data.size(), dest.data(), dest.size());
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "camellia-256 ctr enc " <<  data.size() / dur.count() *r/ 1000 << " k\n";
    }

}

OUCHI_TEST_CASE(benchmark_camellia128_ctr_raw)
{
    using namespace std::chrono;
    const unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 8;
    constexpr int c = 32 * 1024;
    ciao::camellia<16> encoder{ key };
    union {
        std::uint64_t q[2];
        __m128i o;
    }counter{};
    __m128i buf;
    __m128i block{};
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (counter.q[1] = 0; counter.q[1] < c/16; ++counter.q[1]) {
                buf = counter.o;
                encoder.cipher((std::uint8_t*)&buf);
                block = _mm_xor_si128(block, buf);
            }
                
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "camellia-128 ctr raw enc " <<  c / dur.count() *r/ 1000 << " k\n";
    }

}
#endif

