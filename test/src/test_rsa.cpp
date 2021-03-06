﻿#include "../ouchitest/include/ouchitest.hpp"
#include "algorithm/rsa.hpp"

OUCHI_TEST_CASE(test_rsa_builtin_type)
{
    ciao::rsa_key<int> k = ciao::rsa_key_gen<int, short, std::mt19937>();
    ciao::rsa<int, long long> rsa(k);
    short m = 0x0fee;

    auto c = rsa.encrypt(m);
    OUCHI_CHECK_EQUAL(rsa.decrypt(c), m);
}

#ifdef NDEBUG

OUCHI_TEST_CASE(test_rsa)
{
    namespace mp = boost::multiprecision;
    mp::cpp_int m = INT_MAX;
    // ビット数を指定してランダムなRSA鍵を生成
    auto k = ciao::rsa_key_gen(2048);
    ciao::rsa<mp::cpp_int> rsa(k);
    // 暗号化
    auto c = rsa.encrypt(m);
    // 復号して検証
    assert(rsa.decrypt(c) == m);
    OUCHI_CHECK_EQUAL(rsa.decrypt(c), m);
}

#include <string_view>
#include "algorithm/sha.hpp"

OUCHI_TEST_CASE(test_rsa_signature)
{
    namespace mp = boost::multiprecision;
    std::string_view m = "hogehoge_fugafuga";
    auto k = ciao::rsa_key_gen(2048);
    std::uint8_t buf[32];
    ciao::sha256 hash;
    ciao::rsa<mp::cpp_int> rsa(k);
    mp::cpp_int hash_int = 0;
    hash.update(m);
    hash.finalize(buf);
    for (int i = 0; i < 32; ++i) {
        hash_int <<= 8;
        hash_int |= buf[i];
    }
    auto signed_hash = rsa.sign(hash_int);
    OUCHI_CHECK_TRUE(rsa.verify(signed_hash, hash_int));
}
#endif

