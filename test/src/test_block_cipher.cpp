#include "../ouchitest/include/ouchitest.hpp"
#include "block_cipher_mode.hpp"
#include "algorithm/aes.hpp"

OUCHI_TEST_CASE(test_ecb_encode_decode)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17";
    unsigned char dest[32];

    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::ecb<ciao::aes<24>> encoder((const std::uint8_t*)key);
    OUCHI_REQUIRE_TRUE(encoder.cipher(src, 16, dest, 32));
    OUCHI_CHECK_EQUAL(dest[0], 0xdd);
    OUCHI_CHECK_EQUAL(dest[1], 0xa9);
    OUCHI_CHECK_EQUAL(dest[15], 0x91);
    OUCHI_REQUIRE_EQUAL(encoder.inv_cipher(dest, 32, dest, 32).unwrap(), 16);
    OUCHI_CHECK_EQUAL(dest[0], 0x00);
    OUCHI_CHECK_EQUAL(dest[1], 0x11);
    OUCHI_CHECK_EQUAL(dest[15], 0xff);
}

OUCHI_TEST_CASE(test_encoder_padding)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17";
    unsigned char dest[32];

    std::uint8_t src[16];
    std::memcpy(src, block, 15);
    ciao::ecb<ciao::aes<24>> encoder((const std::uint8_t*)key);
    OUCHI_REQUIRE_EQUAL(encoder.cipher(src, 15, dest, 16).unwrap(), 16);
    OUCHI_REQUIRE_EQUAL(encoder.inv_cipher(dest, 16, dest, 32).unwrap(), 15);
    OUCHI_CHECK_EQUAL(dest[0], 0x00);
    OUCHI_CHECK_EQUAL(dest[14], 0xee);
}

OUCHI_TEST_CASE(test_cbc_encode_decode)
{
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
        "\x10\x11\x12\x13\x14\x15\x16\x17";
    unsigned char dest[32];

    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ciao::cbc<ciao::aes<24>> encoder((const std::uint8_t*)block, (const std::uint8_t*)key);
    ciao::cbc<ciao::aes<24>> decoder((const std::uint8_t*)block, (const std::uint8_t*)key);
    OUCHI_REQUIRE_TRUE(encoder.cipher(src, 16, dest, 32));
    OUCHI_REQUIRE_EQUAL(decoder.inv_cipher(dest, 32, dest, 32).unwrap(), 16);
    OUCHI_CHECK_EQUAL(dest[0], 0x00);
    OUCHI_CHECK_EQUAL(dest[1], 0x11);
    OUCHI_CHECK_EQUAL(dest[15], 0xff);
}

OUCHI_TEST_CASE(test_stream_ctr_encode_decode)
{
    using namespace ciao;
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    unsigned int dest[32] = {0,1,2,3,4,5,6,7};
    size_t bn = 0;
    stream_like_ctr<aes<16>, unsigned int> ctr((const std::uint8_t*)block, 0ull, (const std::uint8_t*)key);
    stream_like_ctr<aes<16>, unsigned int> ctr_inv((const std::uint8_t*)block, 0ull, (const std::uint8_t*)key);
    for (int i = 0; i < 32; ++i) {
        dest[i] ^= ctr.next();
    }
    for (int i = 0; i < 16; ++i) {
        OUCHI_CHECK_TRUE(dest[i] != dest[i + 16]);
    }
    for (int i = 0; i < 32; ++i) {
        dest[i] ^= ctr_inv.next();
    }
    for (unsigned int i = 0u; i < 8u; ++i) {
        OUCHI_CHECK_EQUAL(dest[i], i);
    }
}

OUCHI_TEST_CASE(test_ctr_encode_decode)
{
    using namespace ciao;
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    unsigned char dest[32] = {0,1,2,3,4,5,6,7};
    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ctr<aes<16>> encoder((const std::uint8_t*)block, 0ull, (const std::uint8_t*)key);
    ctr<aes<16>> decoder((const std::uint8_t*)block, 0ull, (const std::uint8_t*)key);
    OUCHI_REQUIRE_TRUE(encoder.cipher(src, 16, dest, 32));
    OUCHI_REQUIRE_EQUAL(decoder.inv_cipher(dest, 32, dest, 32).unwrap(), 16);
    OUCHI_CHECK_EQUAL(dest[0], 0x00);
    OUCHI_CHECK_EQUAL(dest[1], 0x11);
    OUCHI_CHECK_EQUAL(dest[15], 0xff);
}

OUCHI_TEST_CASE(test_ctr_compatibility)
{
    using namespace ciao;
    const char block[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
    const char key[] =
        "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    unsigned char dest[32] = {0,1,2,3,4,5,6,7};
    std::uint8_t src[16];
    std::memcpy(src, block, 16);
    ctr<aes<16>> encoder((const std::uint8_t*)block, 0ull, (const std::uint8_t*)key);
    stream_like_ctr<aes<16>, unsigned char> sencoder((const std::uint8_t*)block, 0ull, (const std::uint8_t*)key);

    OUCHI_REQUIRE_TRUE(encoder.cipher(src, 16, dest, 32).is_ok());
    for (auto i = 0u; i < 16; ++i) {
        OUCHI_CHECK_EQUAL(sencoder.next() ^ src[i], dest[i]);
    }
}

