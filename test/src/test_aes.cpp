#include <cstdint>
#include <vector>
#include <chrono>

#include "../ouchitest/include/ouchitest.hpp"
#include "algorithm/aes.hpp"
#include "algorithm/aes_ni.hpp"
#include "block_cipher_mode.hpp"

OUCHI_TEST_CASE(aes128_key_expansion){
    std::uint8_t key[] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };
    ciao::aes<16> encoder;
    encoder.key_expansion(key);

    OUCHI_CHECK_EQUAL(encoder.w_[0], 0x2b7e1516);
    OUCHI_CHECK_EQUAL(encoder.w_[4], 0xa0fafe17);
    OUCHI_CHECK_EQUAL(encoder.w_[43], 0xb6630ca6);
}

OUCHI_TEST_CASE(aes192_key_expansion){
    std::uint8_t key[] = {
        0x8e, 0x73, 0xb0, 0xf7,
        0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b,
        0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2,
        0x52, 0x2c, 0x6b, 0x7b
    };
    ciao::aes<24> encoder;
    encoder.key_expansion(key);

    OUCHI_CHECK_EQUAL(encoder.w_[51], 0x01002202);
}

OUCHI_TEST_CASE(aes256_key_expansion){
    std::uint8_t key[] = {
        0x60, 0x3d, 0xeb, 0x10,
        0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0,
        0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07,
        0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3,
        0x09, 0x14, 0xdf, 0xf4
    };
    ciao::aes<32> encoder;
    encoder.key_expansion(key);

    OUCHI_CHECK_EQUAL(encoder.w_[59], 0x706c631e);
}

OUCHI_TEST_CASE(aes128_cipher)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::aes<16> encoder(key);
    encoder.cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x69);
    OUCHI_CHECK_EQUAL(src[1], 0xc4);
    OUCHI_CHECK_EQUAL(src[15], 0x5a);
    encoder.inv_cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x00);
    OUCHI_CHECK_EQUAL(src[1], 0x11);
    OUCHI_CHECK_EQUAL(src[15], 0xff);
}

OUCHI_TEST_CASE(aes192_cipher)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17";

    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::aes<24> encoder(key);
    encoder.cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0xdd);
    OUCHI_CHECK_EQUAL(src[1], 0xa9);
    OUCHI_CHECK_EQUAL(src[15], 0x91);
    encoder.inv_cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x00);
    OUCHI_CHECK_EQUAL(src[1], 0x11);
    OUCHI_CHECK_EQUAL(src[15], 0xff);
}

OUCHI_TEST_CASE(aes256_cipher)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";

    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::aes<32> encoder(key);
    encoder.cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x8e);
    OUCHI_CHECK_EQUAL(src[1], 0xa2);
    OUCHI_CHECK_EQUAL(src[15], 0x89);
    encoder.inv_cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x00);
    OUCHI_CHECK_EQUAL(src[1], 0x11);
    OUCHI_CHECK_EQUAL(src[15], 0xff);
}

OUCHI_TEST_CASE(aesni128_cipher)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::aes_ni<16> encoder(key);
    encoder.cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x69);
    OUCHI_CHECK_EQUAL(src[1], 0xc4);
    OUCHI_CHECK_EQUAL(src[15], 0x5a);
    encoder.inv_cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x00);
    OUCHI_CHECK_EQUAL(src[1], 0x11);
    OUCHI_CHECK_EQUAL(src[15], 0xff);
}

OUCHI_TEST_CASE(aesni192_cipher)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17";

    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::aes_ni<24> encoder(key);
    encoder.cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0xdd);
    OUCHI_CHECK_EQUAL(src[1], 0xa9);
    OUCHI_CHECK_EQUAL(src[15], 0x91);
    encoder.inv_cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x00);
    OUCHI_CHECK_EQUAL(src[1], 0x11);
    OUCHI_CHECK_EQUAL(src[15], 0xff);
}

OUCHI_TEST_CASE(aesni256_cipher)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";

    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::aes_ni<32> encoder(key);
    encoder.cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x8e);
    OUCHI_CHECK_EQUAL(src[1], 0xa2);
    OUCHI_CHECK_EQUAL(src[15], 0x89);
    encoder.inv_cipher(src);
    OUCHI_CHECK_EQUAL(src[0], 0x00);
    OUCHI_CHECK_EQUAL(src[1], 0x11);
    OUCHI_CHECK_EQUAL(src[15], 0xff);
}

#ifdef NDEBUG
OUCHI_TEST_CASE(aes128_benchmark)
{
    using namespace std::chrono;
    const char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 8;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    ciao::aes<16> encoder{ key };
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16) {
                encoder.cipher(data.data() + i);
            }
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aes-128 ecb enc " <<  data.size()*r / dur.count() / 1000 << " k\n";
    }
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16)
                encoder.inv_cipher(data.data() + i);
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aes-128 ecb dec " <<  data.size()*r / dur.count() / 1000 << " k\n";
    }

}
OUCHI_TEST_CASE(aes128cbc_benchmark)
{
    using namespace std::chrono;
    const unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 8;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    std::vector<std::uint8_t> dest(16 * 1025);
    ciao::cbc<ciao::aes<16>> encoder(key, key);
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            (void)encoder.cipher(data.data(), data.size(), dest.data(), dest.size());
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aes-128 cbc enc " <<  data.size()*r / dur.count() / 1000 << " k\n";
    }

}

OUCHI_TEST_CASE(aes256_benchmark)
{
    using namespace std::chrono;
    const char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 8;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    ciao::aes<32> encoder{ key };
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16) {
                encoder.cipher(data.data() + i);
            }
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aes-256 ecb enc " <<  data.size()*r / dur.count() / 1000 << " k\n";
    }
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16)
                encoder.inv_cipher(data.data() + i);
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aes-256 ecb dec " <<  data.size()*r / dur.count() / 1000 << " k\n";
    }

}
OUCHI_TEST_CASE(aesni128_benchmark)
{
    using namespace std::chrono;
    const char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr auto r = 1024*1024;
    std::vector<std::uint8_t> data(1024*16, 0xc5);
    ciao::aes_ni<16> encoder{ key };
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16)
                encoder.cipher(data.data() + i);
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aesni-128 ecb enc " <<  data.size()*r / dur.count() / 1000'000 << " M\n";
    }
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            for (auto i = 0ull; i < data.size(); i += 16)
                encoder.inv_cipher(data.data() + i);
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aesni-128 ecb dec " <<  data.size()*r / dur.count() / 1000'000 << " M\n";
    }
}
OUCHI_TEST_CASE(aesni128cbc_benchmark)
{
    using namespace std::chrono;
    const unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 256;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    std::vector<std::uint8_t> dest(16 * 1025);
    ciao::cbc<ciao::aes_ni<16>> encoder(key, key);
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            (void)encoder.cipher(data.data(), data.size(), dest.data(), dest.size());
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aesni-128 cbc enc " <<  data.size()*r / dur.count() / 1000'000 << " M\n";
    }

}
OUCHI_TEST_CASE(aesni128ecb_benchmark)
{
    using namespace std::chrono;
    const unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    constexpr int r = 1024 * 1024;
    std::vector<std::uint8_t> data(16 * 1024, 0xc5);
    std::vector<std::uint8_t> dest(16 * 1025);
    ciao::ecb<ciao::aes_ni<16>> encoder(key);
    {
        auto beg = std::chrono::steady_clock::now();
        for (auto k = 0ull; k < r; ++k) {
            (void)encoder.cipher(data.data(), data.size(), dest.data(), dest.size());
        }

        duration<double, std::ratio<1, 1>> dur = std::chrono::steady_clock::now() - beg;
        std::cout << "aesni-128 ecb enc " <<  data.size()*r / dur.count() / 1000'000 << " M\n";
    }

}
#endif

