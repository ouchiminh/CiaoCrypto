#pragma once
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <utility>
#include "../common.hpp"

namespace ciao {

template<size_t S, class = void>
class camellia;

template<size_t S>
class camellia<S, std::enable_if_t<S == 16 || S == 24 || S == 32>> {
    static constexpr unsigned int t = 4;
    static constexpr unsigned int u = std::min((unsigned)S+2u, 18u);
    static constexpr unsigned int v = std::min((unsigned)S / 4u, 6u);
    static constexpr unsigned int nr = std::min((unsigned)S+2u, 24u);
    static constexpr size_t block_size = 16;

    camellia(const std::uint8_t* key)
    {
        key_schedule(key);
    }
    void key_schedule(const std::uint8_t* key) noexcept
    {
        std::uint64_t klr[4];
        std::uint64_t kab[2];
        std::memcpy(klr, key, S);
        if constexpr (S == 16)
            std::memset(klr+2, 0, 16);
        else if constexpr (S == 24)
            klr[3] = ~klr[2];

        kab[0] = klr[0] ^ klr[2];
        kab[1] = klr[1] ^ klr[3];

        kab[1] = f(kab[0], 0xA09E667F3BCC908B);
        kab[0] = f(kab[1], 0xB67AE8584CAA73B2);
        kab[0] ^= klr[0];
        kab[1] ^= klr[1];
        kab[1] = f(kab[0], 0xC6EF372FE94F82BE);
        kab[0] = f(kab[1], 0x54FF53A5F1D36F1C);
    }

    inline static std::uint64_t f(std::uint64_t x, std::uint64_t k) noexcept;
    inline static std::uint64_t fl(std::uint64_t x, std::uint64_t k) noexcept;
    inline static std::uint64_t inv_fl(std::uint64_t x, std::uint64_t k) noexcept;
    inline static std::uint64_t s(std::uint64_t x) noexcept;
    inline static std::uint64_t p(std::uint64_t x) noexcept;
    template<int I>
    inline static std::uint8_t sub_byte(std::uint8_t x) noexcept;

    void enc_normal_round(std::uint64_t* dp, unsigned i) const noexcept
    {
        std::uint64_t l;
        l = dp[0];
        dp[0] = dp[1] ^ f(dp[0], k_[i]);
        dp[1] = l;
    }
    void enc_special_round(std::uint64_t* dp, unsigned i) const noexcept
    {
            std::uint64_t
                l = dp[1] ^ f(dp[0], k_[i]),
                r = dp[0];
            dp[0] = fl(l, kl_[(i+1)/3-2]);
            dp[1] = inv_fl(r, kl_[(i+1)/3-2]);
    }

    void cipher(std::uint8_t* data) const noexcept
    {
        auto dp = reinterpret_cast<std::uint64_t*>(data);
        dp[0] ^= kw_[0];
        dp[1] ^= kw_[1];

        for (unsigned int i = 0u; i < nr;) {
            enc_normal_round(dp, i++);
            enc_normal_round(dp, i++);
            enc_normal_round(dp, i++);
            enc_normal_round(dp, i++);
            enc_normal_round(dp, i++);
            enc_special_round(dp, i++);
        }
        dp[0] ^= kw_[2];
        dp[1] ^= kw_[3];

    }
    void inv_cipher(std::uint8_t* data) noexcept;
private:
    std::uint64_t kw_[t];
    std::uint64_t k_[u];
    std::uint64_t kl_[v];
};

}

