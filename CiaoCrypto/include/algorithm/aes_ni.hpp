#pragma once
#include "aes.hpp"
#include "../bytes.hpp"

#include <utility>
#include <wmmintrin.h>

namespace ciao {


template<size_t K, class = void>
struct aes_ni;

template<size_t K>
struct aes_ni<K, std::enable_if_t<K == 16 || K ==24 || K ==32>> : private aes<K> {
    static constexpr size_t block_size = 16;
    static constexpr size_t key_size = K;
    static constexpr unsigned int nb = 4;
    static constexpr unsigned int nr = K / 4 + 6;
    static constexpr unsigned int nk = K / 4;
    static constexpr std::uint32_t Rcon[10] = {
        0x0100'0000,0x0200'0000,0x0400'0000,0x0800'0000,0x100'00000,
        0x2000'0000,0x4000'0000,0x8000'0000,0x1b00'0000,0x3600'0000
    };
    aes_ni() = default;
    aes_ni(const void* key) noexcept
    {
        expand_key(key);
    }
    ~aes_ni()
    {
        constexpr __m128i zero{};
        std::fill(w128_, w128_ + nr + 1, zero);
        std::fill(dw128_, dw128_ + nr + 1, zero);
    }

    void expand_key(const void* key) noexcept
    {
        aes<K>::expand_key(key);
        for (auto i = 0u; i < nr+1; ++i) {
            alignas(16) std::uint8_t tmp[16];
            unpack(aes<K>::w_[i*4], tmp);
            unpack(aes<K>::w_[i*4+1], tmp+4);
            unpack(aes<K>::w_[i*4+2], tmp+8);
            unpack(aes<K>::w_[i*4+3], tmp+12);
            w128_[i] = _mm_load_si128(reinterpret_cast<__m128i*>(tmp));
        }
        dw128_[0] = w128_[0];
        for (auto i = 1u; i < nr; ++i) {
            dw128_[i] = _mm_aesimc_si128(w128_[i]);
        }
        dw128_[nr] = w128_[nr];
    }

    void cipher(std::uint8_t* data) const noexcept
    {
        auto state = _mm_loadu_si128(reinterpret_cast<__m128i*>(data));
        state = _mm_xor_si128(state, w128_[0]);
        enc_round(state, std::make_index_sequence<nr-1>{});
        state = _mm_aesenclast_si128(state, w128_[nr]);
        _mm_store_si128(reinterpret_cast<__m128i*>(data), state);
    }

    void inv_cipher(std::uint8_t* data) const noexcept
    {
        __m128i state = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
        state = _mm_xor_si128(state, dw128_[nr]);
        dec_round(state, std::make_index_sequence<nr-1>{});
        state = _mm_aesdeclast_si128(state, dw128_[0]);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(data), state);
    }

    template<size_t ...S>
    inline void enc_round(__m128i& state, std::index_sequence<S...>) const noexcept
    {
        ((state = _mm_aesenc_si128(state, w128_[S + 1])), ...);
    }
    template<size_t ...S>
    inline void dec_round(__m128i& state, std::index_sequence<S...>) const noexcept
    {
        ((state = _mm_aesdec_si128(state, dw128_[nr - S - 1])), ...);
    }

    alignas(16) __m128i w128_[nr+1];
    alignas(16) __m128i dw128_[nr+1];
};

}

