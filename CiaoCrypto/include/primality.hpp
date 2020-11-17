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
template<class Int, class UInt>
Int random_prime(unsigned int r = 20)
{
    using uint_t = UInt;
    using int_t = Int;
    static_assert(sizeof(int_t) == sizeof(uint_t));
    namespace mp = boost::multiprecision;
    boost::random::independent_bits_engine<std::mt19937, sizeof(uint_t), uint_t> rand;
    rand.seed(std::random_device{}());
    uint_t p;
    bool fp = false;
    while (!fp) {
        p = rand() & ~(uint_t(1) << (8*sizeof(Int) - 1));
        fp = mp::miller_rabin_test(p, r);
    }
    return (int_t)p;
}

template<class Rnd = std::mt19937>
auto random_prime(unsigned int bits, unsigned int r = 20)
-> std::enable_if_t<std::is_same_v<typename Rnd::result_type, std::uint32_t>, boost::multiprecision::cpp_int>
{
    namespace mp = boost::multiprecision;
    assert(bits % 32 == 0);
    mp::cpp_int p;
    Rnd rnd{ std::random_device{}() };
    auto fill_bits = [&rnd, r = bits / 32]() mutable {
        mp::cpp_int v = 0;
        for (auto i = 0u; i < r; ++i) {
            v <<= 32;
            v |= (std::uint32_t)rnd();
        }
        return v;
    };
    do {
        p = fill_bits();
    } while (mp::miller_rabin_test(p, r) == false);
    return p;
}

template<class Int>
Int find_generator(Int p)
{
    ouchi::math::modint<Int> g{ 2, p };
    while ((Int)ouchi::math::pow<Int, Int, Int>(g, (p-1)/2) == 1) {
        g += 1;
    }
    return (Int)g;
}

}


