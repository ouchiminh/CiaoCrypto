#pragma once
#include "ouchilib/math/modint.hpp"
#include "boost/multiprecision/miller_rabin.hpp"
#include <type_traits>
#include <cassert>

namespace ciao {

template<class Int, class Internal = Int>
class dh_key_exchange {
public:
    using int_type = std::remove_cv_t<std::remove_reference_t<Int>>;
    using internal_type = std::remove_cv_t<std::remove_reference_t<Internal>>;
    dh_key_exchange(const int_type& g, const int_type& p, const int_type& private_key)
        : g_{ g, p }, private_key_{ private_key }
    {
        //namespace mp = boost::multiprecision;
        //if (!mp::miller_rabin_test(p, 20))
        //    throw std::invalid_argument("p is not a prime.");
    }

    int_type get_public_key() const
    {
        return pow(g_, private_key_);
    }
    int_type get_generator() const noexcept { return (int_type)g_; }
    int_type get_prime() const noexcept { return g_.mod(); }
    int_type calc_secret(const int_type& bobs_public_key) const
    {
        return pow(ouchi::math::modint<int_type>(bobs_public_key, g_.mod()), private_key_);
    }
    

private:
    ouchi::math::modint<int_type, internal_type> g_;
    int_type private_key_;
};

}

