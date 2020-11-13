#pragma once
#include <type_traits>
#include <tuple>
#include <numeric>
#include "../primality.hpp"
#include "../common.hpp"
#include "ouchilib/math/modint.hpp"
#include "ouchilib/math/gf.hpp"

namespace ciao {

template<class Int>
struct rsa_key {
    Int decrypt_key;
    Int encrypt_key;
    Int n;
};

template<class Rnd = std::mt19937>
auto rsa_key_gen(unsigned int bits)
-> std::enable_if_t<std::is_same_v<typename Rnd::result_type, std::uint32_t>, rsa_key<boost::multiprecision::cpp_int>>
{
    namespace mp = boost::multiprecision;
    mp::cpp_int p = random_prime(bits / 2, 20 + bits / 4),
        q = random_prime(bits / 2, 20 + bits / 4);
    mp::cpp_int n = p * q, phi = (p-1) * (q-1);
    mp::cpp_int e = 0, d = 0;
    Rnd rnd{ std::random_device{}() };
    auto fill_bits = [&rnd, r = bits / 32 / 2]() mutable {
        mp::cpp_int v = 0;
        for (auto i = 0u; i < r; ++i) {
            v <<= 32;
            v |= (std::uint32_t)rnd();
        }
        return v;
    };
    mp::cpp_int buf;
    do {
        e = fill_bits() % phi;
        std::tie(buf, d, std::ignore) = ouchi::math::ex_gcd(e, phi);
    } while (buf != 1);
    if (d < 0) d += phi;
    assert((e*d)%phi == 1);
    return { d, e, n };
}
template<class T, class HT, class Rnd = std::mt19937, class UT = std::make_unsigned_t<T>, class UHT = std::make_unsigned_t<HT>>
auto rsa_key_gen()
-> std::enable_if_t<std::is_convertible_v<typename Rnd::result_type, HT>, rsa_key<T>>
{
    namespace mp = boost::multiprecision;
    Rnd rnd{ std::random_device{}() };
    auto prime_gen = [&rnd]() mutable {
        HT a;
        do {
            a = rand() & ~(UHT(1) << (sizeof(UHT) * 8 - 1));
        } while (mp::miller_rabin_test(a, 20) == false);
        return a;
    };
    HT p = prime_gen(), q = prime_gen();
    T n = p * q, phi = (p - 1) * (q - 1);
    T e = 0, d = 0;
    T buf;
    do {
        e = rnd() % phi;
        if (e < 0) e += phi;
        std::tie(buf, d, std::ignore) = ouchi::math::ex_gcd(e, phi);
    } while (buf != 1);
    if (d < 0) d += phi;
    assert((e*d)%phi == 1);
    return { d, e, n };
}

template<class Int, class Internal = Int>
class rsa {
public:
    rsa(const rsa_key<Int>& key)
        : key_{key}
    {}
    rsa(const Int& private_key, const Int& public_key, const Int& n)
        : key_{private_key, public_key, n}
    {}
    Int decrypt(const Int& c) const
    {
        ouchi::math::modint<Int, Internal> mc(c, key_.n);
        return (Int)ouchi::math::pow(mc, key_.decrypt_key);
    }
    Int sign(const Int& m) const
    {
        return decrypt(m);
    }
    bool verify(const Int signed_message, const Int raw_message)
    {
        return encrypt(signed_message) == raw_message;
    }
    Int encrypt(const Int& m) const
    {
        ouchi::math::modint<Int, Internal> mc(m, key_.n);
        return (Int)ouchi::math::pow(mc, key_.encrypt_key);
    }
private:
    rsa_key<Int> key_;
};

}

