#include <chrono>
#include "../ouchitest/include/ouchitest.hpp"
#include "algorithm/sha.hpp"

OUCHI_TEST_CASE(test_sha256)
{
    using namespace ciao;
    sha<256> sha256;
    sha256.update("abc");
    auto res = sha256.finalize();
    OUCHI_CHECK_EQUAL(res.byte_data[0], 0xba);
    sha256.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    sha256.finalize(res.byte_data);
    OUCHI_CHECK_EQUAL(res.byte_data[0], 0x24);
}

#ifdef NDEBUG
OUCHI_TEST_CASE(benchmark_sha256)
{
    namespace chrono = std::chrono;
    using namespace ciao;
    constexpr int w = 1024 * 1024 * 64;
    static char data[16384] = {};
    sha<256> sha256;
    {
        constexpr int c = 16;
        constexpr int r = w / c;
        auto beg = chrono::steady_clock::now();
        for (int i = 0; i < r; ++i) {
            sha256.update(data, c);
            (void)sha256.finalize();
        }
        auto end = chrono::steady_clock::now();
        chrono::duration<double> diff = end - beg;
        std::cout << "sha256 " << c << "bytes " << w / diff.count() / 1024 << "kB/s\n";
    }
    {
        constexpr int c = 1024;
        constexpr int r = w / c;
        auto beg = chrono::steady_clock::now();
        for (int i = 0; i < r; ++i) {
            sha256.update(data, c);
            (void)sha256.finalize();
        }
        auto end = chrono::steady_clock::now();
        chrono::duration<double> diff = end - beg;
        std::cout << "sha256 " << c << "bytes " << w / diff.count() / 1024 << "kB/s\n";
    }
    {
        constexpr int c = 16384;
        constexpr int r = w / c;
        auto beg = chrono::steady_clock::now();
        for (int i = 0; i < r; ++i) {
            sha256.update(data, c);
            (void)sha256.finalize();
        }
        auto end = chrono::steady_clock::now();
        chrono::duration<double> diff = end - beg;
        std::cout << "sha256 " << c << "bytes " << w / diff.count() / 1024 << "kB/s\n";
    }
}
#endif

