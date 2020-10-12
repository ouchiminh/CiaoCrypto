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

