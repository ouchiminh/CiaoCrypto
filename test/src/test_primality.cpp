#include "../ouchitest/include/ouchitest.hpp"
#include "primality.hpp"
#include "algorithm/dh_key_exchange.hpp"

#if !defined NDEBUG
OUCHI_TEST_CASE(test_prime_gen)
{
    using namespace ciao;
    auto p = random_prime<128>();
    auto g = find_generator(p);
    dh_key_exchange<decltype(p)> dha(g, p, 40);
    dh_key_exchange<decltype(p)> dhb(g, p, 13);
    //std::cout << p << '\n';
    OUCHI_CHECK_EQUAL(
        dhb.calc_secret(dha.get_public_key()),
        dha.calc_secret(dhb.get_public_key()));
}
OUCHI_TEST_CASE(test_prime_gen2)
{
    using namespace ciao;
    auto p = random_prime<boost::multiprecision::int128_t, boost::multiprecision::uint128_t>();
    auto g = find_generator(p);
    dh_key_exchange<decltype(p)> dha(g, p, 40);
    dh_key_exchange<decltype(p)> dhb(g, p, 13);
    //std::cout << p << '\n';
    OUCHI_CHECK_EQUAL(
        dhb.calc_secret(dha.get_public_key()),
        dha.calc_secret(dhb.get_public_key()));
}
OUCHI_TEST_CASE(test_prime_gen3)
{
    namespace mp = boost::multiprecision;
    using namespace ciao;
    auto p = random_prime(128);
    auto g = find_generator<mp::cpp_int>(p);
    dh_key_exchange<mp::cpp_int> dha(g, p, 40);
    dh_key_exchange<mp::cpp_int> dhb(g, p, 13);
    //std::cout << p << '\n';
    OUCHI_CHECK_EQUAL(
        dhb.calc_secret(dha.get_public_key()),
        dha.calc_secret(dhb.get_public_key()));
}
#endif

