#pragma once
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/random.hpp"
#include "ouchilib/math/modint.hpp"
#include <random>

namespace ciao {
template<unsigned Bits>
boost::multiprecision::number<boost::multiprecision::cpp_int_backend<Bits, Bits, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void> >
random_prime(unsigned int r = 20)
{
    using uint_t = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<Bits, Bits, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void> >;
    using int_t = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<Bits, Bits, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void> >;
    namespace mp = boost::multiprecision;
    boost::random::independent_bits_engine<std::mt19937, Bits, uint_t> rand;
    rand.seed(std::random_device{}());
    uint_t p;
    bool fp = false;
    while (!fp) {
        p = rand() & ~(uint_t(1) << (Bits - 1));
        fp = mp::miller_rabin_test(p, r);
    }
    return (int_t)p;
}

template<class Int>
Int find_generator(Int p)
{
    ouchi::math::modint<Int> g{ 2, p };
    while ((Int)ouchi::math::pow(g, (p-1)/2) == 1) {
        g += 1;
    }
    return (Int)g;
}

}


