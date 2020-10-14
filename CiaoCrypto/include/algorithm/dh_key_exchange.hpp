#include "ouchilib/math/modint.hpp"
#include <type_traits>

namespace ciao {

template<class Int>
class dh_key_exchange {
public:
    dh_key_exchange(Int g, Int p, Int private_key)
        : g_{g, p}, private_key_{private_key}
    {
        namespace mp = boost::multiprecision;
        mp::miller_rabin()
    }

    Int get_public_key() const
    {
        return pow(g_, private_key_)
    }
    Int calc_secret(Int bobs_public_key) const
    {
        return pow(modint<Int>(bobs_public_key, g_.mod()), private_key_);
    }
    

private:
    ouchi::math::modint<Int> g_;
    Int private_key_;
};

}

