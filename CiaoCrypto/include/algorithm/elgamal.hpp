#pragma once
#include "../primality.hpp"
#include "dh_key_exchange.hpp"
#include "ouchilib/math/modint.hpp"

namespace ciao {

template<class Int, class Internal = Int>
Int elgamal_enc(Int p, Int k, Int message)
{
    ouchi::math::modint<Int, Internal> mk{ k, p };
    mk *= message;
    return (Int)mk;
}

template<class Int, class Internal = Int>
Int elgamal_dec(Int g, Int p, Int k, Int crypto)
{
    ouchi::math::modint<Int, Internal> ck{ crypto, p };
    ck /= k;
    return (Int)ck;
}

}

