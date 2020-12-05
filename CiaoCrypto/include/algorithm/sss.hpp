#pragma once
#include <type_traits>
#include <utility>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <random>
#include "ouchilib/math/gf.hpp"
#include "ouchilib/math/matrix.hpp"

namespace ciao {
template<class T, class = T>
class sss;

template<class T>
class sss<
    T,
    std::common_type_t<
        decltype(std::declval<T>() + std::declval<T>()),
        decltype(std::declval<T>() - std::declval<T>()),
        decltype(std::declval<T>() * std::declval<T>()),
        decltype(std::declval<T>() / std::declval<T>())
    >>
{
public:
    sss(unsigned threshold) { generate_key(threshold); }
    sss() = default;
    size_t threshold() const noexcept { return polynominal_.size(); }
    T generate_share(T n, T secret) const
    {
        return f(n, secret);
    }
    template<class RandomGen = std::mt19937>
    auto generate_key(RandomGen&& random = RandomGen{})
        ->std::enable_if_t<std::is_invocable_r_v<int, RandomGen>>
    {
        generate_key(polynominal_.size(), random);
    }
    template<class RandomGen = std::mt19937>
    auto generate_key(unsigned threshold, RandomGen&& random = RandomGen{})
        ->std::enable_if_t<std::is_invocable_r_v<int, RandomGen>>
    {
        polynominal_.clear();
        polynominal_.resize(threshold);
        polynominal_[0] = T{ 0 };
        for (unsigned i = 1; i < polynominal_.size(); ++i) {
            polynominal_[i] = (T)random();
        }
    }
    // shares:
    // {x y}
    // {. .}
    // {. .}
    // {. .}
    // where 'x' is n of generate_share, y is return value of generate_share.
    static T decode(const ouchi::math::vl_matrix<T>& shares)
    {
        size_t n = shares.total_size() / 2;
        ouchi::math::vl_matrix<T> data;
        data.resize(n, n + 1);

        for (unsigned i = 0; i < n; ++i) {
            T x = T{ 1 };
            assert(x != 0);
            for (int j = 0; j < n; ++j) {
                data(i, j) = x;
                x *= shares(i, 0);
            }
            data(i, n) = shares(i, 1);
        }
        /* data is like...
        r                     q
        | 1 x^2 x^3 ... x^n y |
        L                     j
        */
        solve(data);
        return data(0, n);
    }
private:
    static void solve(ouchi::math::vl_matrix<T>& data)
    {
        auto [r, c] = data.size();
        for (int i = 0; i < r; ++i) {
            T buf = T{ 1 } / data(i, i);
            for (int k = 0; k < c; ++k) {
                data(i, k) *= buf;
            }
            for (int j = 0; j < r; ++j) {
                if (i == j) continue;
                buf = data(j, i);
                for (int k = i; k < c; ++k) {
                    data(j, k) -= data(i, k) * buf;
                }
            }
        }
    }
    T f(T x, T secret) const
    {
        T res{ secret };
        T buf = (T)1;
        for (auto v : polynominal_) {
            res += v * buf;
            buf *= x;
        }
        return res;
    }
    std::vector<T> polynominal_;
};


}

