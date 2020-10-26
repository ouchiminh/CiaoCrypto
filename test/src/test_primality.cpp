#include "../ouchitest/include/ouchitest.hpp"
#include "primality.hpp"
#include "algorithm/dh_key_exchange.hpp"

#ifdef NDEBUG
OUCHI_TEST_CASE(test_prime_gen)
{
    using namespace ciao;
    auto p = random_prime<1024>();
    dh_key_exchange<decltype(p)> dha(2, p, 40);
    dh_key_exchange<decltype(p)> dhb(2, p, 13);
    //std::cout << p << '\n';
    OUCHI_CHECK_EQUAL(
        dhb.calc_secret(dha.get_public_key()),
        dha.calc_secret(dhb.get_public_key()));
}
#endif

