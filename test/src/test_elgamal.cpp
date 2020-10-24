#include "algorithm/elgamal.hpp"
#include "algorithm/dh_key_exchange.hpp"
#include "primality.hpp"
#include "../ouchitest/include/ouchitest.hpp"

OUCHI_TEST_CASE(test_elgamal_enc_dec_int)
{
    using namespace ciao;
    auto p = 11;
    auto g = 2;
    dh_key_exchange<decltype(p)> dha(g, p, 40u);
    dh_key_exchange<decltype(p)> dhb(g, p, 13u);
    auto k = dha.calc_secret(dhb.get_public_key());
    OUCHI_CHECK_EQUAL(elgamal_dec(g, p, k, elgamal_enc<decltype(p)>(p, k, 9)), 9);
}

OUCHI_TEST_CASE(test_elgamal_enc_dec_big_int)
{
    using namespace ciao;
    auto p = random_prime<1024>(24);
    auto g = find_generator(p);
    using value_type = decltype(p);
    using internal_type = decltype(random_prime<2048>());
    dh_key_exchange<value_type, internal_type> dha(g, p, 40u);
    dh_key_exchange<value_type, internal_type> dhb(g, p, 13u);
    auto k = dha.calc_secret(dhb.get_public_key());
    OUCHI_CHECK_EQUAL((elgamal_dec<value_type, internal_type>(g, p, k, elgamal_enc<value_type, internal_type>(p, k, ~0ull))), ~0ull);
}

