#pragma once
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/random.hpp"
#include <random>

namespace ciao {
template<unsigned Bits>
boost::multiprecision::number<boost::multiprecision::cpp_int_backend<Bits, Bits, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void> >
random_choice()
{
    using uint_t = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<Bits, Bits, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void> >;
    namespace mp = boost::multiprecision;
    boost::random::independent_bits_engine<std::mt19937, Bits, uint_t> rand;
    rand.seed(std::random_device{}());
    uint_t p;
    bool fp = false;
    while (!fp) {
        p = rand();
        fp = mp::miller_rabin_test(p, 20);
    }
    return p;
}

}


