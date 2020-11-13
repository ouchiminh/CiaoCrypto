#include "../ouchitest/include/ouchitest.hpp"
#include "algorithm/rsa.hpp"

OUCHI_TEST_CASE(test_rsa_builtin_type)
{
    ciao::rsa_key<int> k = ciao::rsa_key_gen<int, short, std::mt19937>();
    ciao::rsa<int, long long> rsa(k);
    short m = 0x0fee;

    auto c = rsa.encrypt(m);
    OUCHI_CHECK_EQUAL(rsa.decrypt(c), m);
}

OUCHI_TEST_CASE(test_rsa)
{
    namespace mp = boost::multiprecision;
    mp::cpp_int m = INT_MAX;
    auto k = ciao::rsa_key_gen(2048);
    ciao::rsa<mp::cpp_int> rsa(k);
    auto c = rsa.encrypt(m);
    OUCHI_CHECK_EQUAL(rsa.decrypt(c), m);
}

