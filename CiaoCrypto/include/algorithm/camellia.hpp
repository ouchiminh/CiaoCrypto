﻿#pragma once
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <utility>
#include "../common.hpp"

namespace ciao {

namespace detail {

alignas(8) inline constexpr std::uint8_t camellia_sbox[][256] = {
    {
        0x70, 0x82, 0x2c, 0xec, 0xb3, 0x27,
        0xc0, 0xe5, 0xe4, 0x85, 0x57, 0x35,
        0xea, 0x0c, 0xae, 0x41, 0x23, 0xef,
        0x6b, 0x93, 0x45, 0x19, 0xa5, 0x21,
        0xed, 0x0e, 0x4f, 0x4e, 0x1d, 0x65,
        0x92, 0xbd, 0x86, 0xb8, 0xaf, 0x8f,
        0x7c, 0xeb, 0x1f, 0xce, 0x3e, 0x30,
        0xdc, 0x5f, 0x5e, 0xc5, 0x0b, 0x1a,
        0xa6, 0xe1, 0x39, 0xca, 0xd5, 0x47,
        0x5d, 0x3d, 0xd9, 0x01, 0x5a, 0xd6,
        0x51, 0x56, 0x6c, 0x4d, 0x8b, 0x0d,
        0x9a, 0x66, 0xfb, 0xcc, 0xb0, 0x2d,
        0x74, 0x12, 0x2b, 0x20, 0xf0, 0xb1,
        0x84, 0x99, 0xdf, 0x4c, 0xcb, 0xc2,
        0x34, 0x7e, 0x76, 0x05, 0x6d, 0xb7,
        0xa9, 0x31, 0xd1, 0x17, 0x04, 0xd7,
        0x14, 0x58, 0x3a, 0x61, 0xde, 0x1b,
        0x11, 0x1c, 0x32, 0x0f, 0x9c, 0x16,
        0x53, 0x18, 0xf2, 0x22, 0xfe, 0x44,
        0xcf, 0xb2, 0xc3, 0xb5, 0x7a, 0x91,
        0x24, 0x08, 0xe8, 0xa8, 0x60, 0xfc,
        0x69, 0x50, 0xaa, 0xd0, 0xa0, 0x7d,
        0xa1, 0x89, 0x62, 0x97, 0x54, 0x5b,
        0x1e, 0x95, 0xe0, 0xff, 0x64, 0xd2,
        0x10, 0xc4, 0x00, 0x48, 0xa3, 0xf7,
        0x75, 0xdb, 0x8a, 0x03, 0xe6, 0xda,
        0x09, 0x3f, 0xdd, 0x94, 0x87, 0x5c,
        0x83, 0x02, 0xcd, 0x4a, 0x90, 0x33,
        0x73, 0x67, 0xf6, 0xf3, 0x9d, 0x7f,
        0xbf, 0xe2, 0x52, 0x9b, 0xd8, 0x26,
        0xc8, 0x37, 0xc6, 0x3b, 0x81, 0x96,
        0x6f, 0x4b, 0x13, 0xbe, 0x63, 0x2e,
        0xe9, 0x79, 0xa7, 0x8c, 0x9f, 0x6e,
        0xbc, 0x8e, 0x29, 0xf5, 0xf9, 0xb6,
        0x2f, 0xfd, 0xb4, 0x59, 0x78, 0x98,
        0x06, 0x6a, 0xe7, 0x46, 0x71, 0xba,
        0xd4, 0x25, 0xab, 0x42, 0x88, 0xa2,
        0x8d, 0xfa, 0x72, 0x07, 0xb9, 0x55,
        0xf8, 0xee, 0xac, 0x0a, 0x36, 0x49,
        0x2a, 0x68, 0x3c, 0x38, 0xf1, 0xa4,
        0x40, 0x28, 0xd3, 0x7b, 0xbb, 0xc9,
        0x43, 0xc1, 0x15, 0xe3, 0xad, 0xf4,
        0x77, 0xc7, 0x80, 0x9e
    },
    {
        0xe0, 0x05, 0x58, 0xd9, 0x67, 0x4e,
        0x81, 0xcb, 0xc9, 0x0b, 0xae, 0x6a,
        0xd5, 0x18, 0x5d, 0x82, 0x46, 0xdf,
        0xd6, 0x27, 0x8a, 0x32, 0x4b, 0x42,
        0xdb, 0x1c, 0x9e, 0x9c, 0x3a, 0xca,
        0x25, 0x7b, 0x0d, 0x71, 0x5f, 0x1f,
        0xf8, 0xd7, 0x3e, 0x9d, 0x7c, 0x60,
        0xb9, 0xbe, 0xbc, 0x8b, 0x16, 0x34,
        0x4d, 0xc3, 0x72, 0x95, 0xab, 0x8e,
        0xba, 0x7a, 0xb3, 0x02, 0xb4, 0xad,
        0xa2, 0xac, 0xd8, 0x9a, 0x17, 0x1a,
        0x35, 0xcc, 0xf7, 0x99, 0x61, 0x5a,
        0xe8, 0x24, 0x56, 0x40, 0xe1, 0x63,
        0x09, 0x33, 0xbf, 0x98, 0x97, 0x85,
        0x68, 0xfc, 0xec, 0x0a, 0xda, 0x6f,
        0x53, 0x62, 0xa3, 0x2e, 0x08, 0xaf,
        0x28, 0xb0, 0x74, 0xc2, 0xbd, 0x36,
        0x22, 0x38, 0x64, 0x1e, 0x39, 0x2c,
        0xa6, 0x30, 0xe5, 0x44, 0xfd, 0x88,
        0x9f, 0x65, 0x87, 0x6b, 0xf4, 0x23,
        0x48, 0x10, 0xd1, 0x51, 0xc0, 0xf9,
        0xd2, 0xa0, 0x55, 0xa1, 0x41, 0xfa,
        0x43, 0x13, 0xc4, 0x2f, 0xa8, 0xb6,
        0x3c, 0x2b, 0xc1, 0xff, 0xc8, 0xa5,
        0x20, 0x89, 0x00, 0x90, 0x47, 0xef,
        0xea, 0xb7, 0x15, 0x06, 0xcd, 0xb5,
        0x12, 0x7e, 0xbb, 0x29, 0x0f, 0xb8,
        0x07, 0x04, 0x9b, 0x94, 0x21, 0x66,
        0xe6, 0xce, 0xed, 0xe7, 0x3b, 0xfe,
        0x7f, 0xc5, 0xa4, 0x37, 0xb1, 0x4c,
        0x91, 0x6e, 0x8d, 0x76, 0x03, 0x2d,
        0xde, 0x96, 0x26, 0x7d, 0xc6, 0x5c,
        0xd3, 0xf2, 0x4f, 0x19, 0x3f, 0xdc,
        0x79, 0x1d, 0x52, 0xeb, 0xf3, 0x6d,
        0x5e, 0xfb, 0x69, 0xb2, 0xf0, 0x31,
        0x0c, 0xd4, 0xcf, 0x8c, 0xe2, 0x75,
        0xa9, 0x4a, 0x57, 0x84, 0x11, 0x45,
        0x1b, 0xf5, 0xe4, 0x0e, 0x73, 0xaa,
        0xf1, 0xdd, 0x59, 0x14, 0x6c, 0x92,
        0x54, 0xd0, 0x78, 0x70, 0xe3, 0x49,
        0x80, 0x50, 0xa7, 0xf6, 0x77, 0x93,
        0x86, 0x83, 0x2a, 0xc7, 0x5b, 0xe9,
        0xee, 0x8f, 0x01, 0x3d
    },
    {
        0x38, 0x41, 0x16, 0x76, 0xd9, 0x93,
        0x60, 0xf2, 0x72, 0xc2, 0xab, 0x9a,
        0x75, 0x06, 0x57, 0xa0, 0x91, 0xf7,
        0xb5, 0xc9, 0xa2, 0x8c, 0xd2, 0x90,
        0xf6, 0x07, 0xa7, 0x27, 0x8e, 0xb2,
        0x49, 0xde, 0x43, 0x5c, 0xd7, 0xc7,
        0x3e, 0xf5, 0x8f, 0x67, 0x1f, 0x18,
        0x6e, 0xaf, 0x2f, 0xe2, 0x85, 0x0d,
        0x53, 0xf0, 0x9c, 0x65, 0xea, 0xa3,
        0xae, 0x9e, 0xec, 0x80, 0x2d, 0x6b,
        0xa8, 0x2b, 0x36, 0xa6, 0xc5, 0x86,
        0x4d, 0x33, 0xfd, 0x66, 0x58, 0x96,
        0x3a, 0x09, 0x95, 0x10, 0x78, 0xd8,
        0x42, 0xcc, 0xef, 0x26, 0xe5, 0x61,
        0x1a, 0x3f, 0x3b, 0x82, 0xb6, 0xdb,
        0xd4, 0x98, 0xe8, 0x8b, 0x02, 0xeb,
        0x0a, 0x2c, 0x1d, 0xb0, 0x6f, 0x8d,
        0x88, 0x0e, 0x19, 0x87, 0x4e, 0x0b,
        0xa9, 0x0c, 0x79, 0x11, 0x7f, 0x22,
        0xe7, 0x59, 0xe1, 0xda, 0x3d, 0xc8,
        0x12, 0x04, 0x74, 0x54, 0x30, 0x7e,
        0xb4, 0x28, 0x55, 0x68, 0x50, 0xbe,
        0xd0, 0xc4, 0x31, 0xcb, 0x2a, 0xad,
        0x0f, 0xca, 0x70, 0xff, 0x32, 0x69,
        0x08, 0x62, 0x00, 0x24, 0xd1, 0xfb,
        0xba, 0xed, 0x45, 0x81, 0x73, 0x6d,
        0x84, 0x9f, 0xee, 0x4a, 0xc3, 0x2e,
        0xc1, 0x01, 0xe6, 0x25, 0x48, 0x99,
        0xb9, 0xb3, 0x7b, 0xf9, 0xce, 0xbf,
        0xdf, 0x71, 0x29, 0xcd, 0x6c, 0x13,
        0x64, 0x9b, 0x63, 0x9d, 0xc0, 0x4b,
        0xb7, 0xa5, 0x89, 0x5f, 0xb1, 0x17,
        0xf4, 0xbc, 0xd3, 0x46, 0xcf, 0x37,
        0x5e, 0x47, 0x94, 0xfa, 0xfc, 0x5b,
        0x97, 0xfe, 0x5a, 0xac, 0x3c, 0x4c,
        0x03, 0x35, 0xf3, 0x23, 0xb8, 0x5d,
        0x6a, 0x92, 0xd5, 0x21, 0x44, 0x51,
        0xc6, 0x7d, 0x39, 0x83, 0xdc, 0xaa,
        0x7c, 0x77, 0x56, 0x05, 0x1b, 0xa4,
        0x15, 0x34, 0x1e, 0x1c, 0xf8, 0x52,
        0x20, 0x14, 0xe9, 0xbd, 0xdd, 0xe4,
        0xa1, 0xe0, 0x8a, 0xf1, 0xd6, 0x7a,
        0xbb, 0xe3, 0x40, 0x4f
    },
    {
        0x70, 0x2c, 0xb3, 0xc0, 0xe4, 0x57,
        0xea, 0xae, 0x23, 0x6b, 0x45, 0xa5,
        0xed, 0x4f, 0x1d, 0x92, 0x86, 0xaf,
        0x7c, 0x1f, 0x3e, 0xdc, 0x5e, 0x0b,
        0xa6, 0x39, 0xd5, 0x5d, 0xd9, 0x5a,
        0x51, 0x6c, 0x8b, 0x9a, 0xfb, 0xb0,
        0x74, 0x2b, 0xf0, 0x84, 0xdf, 0xcb,
        0x34, 0x76, 0x6d, 0xa9, 0xd1, 0x04,
        0x14, 0x3a, 0xde, 0x11, 0x32, 0x9c,
        0x53, 0xf2, 0xfe, 0xcf, 0xc3, 0x7a,
        0x24, 0xe8, 0x60, 0x69, 0xaa, 0xa0,
        0xa1, 0x62, 0x54, 0x1e, 0xe0, 0x64,
        0x10, 0x00, 0xa3, 0x75, 0x8a, 0xe6,
        0x09, 0xdd, 0x87, 0x83, 0xcd, 0x90,
        0x73, 0xf6, 0x9d, 0xbf, 0x52, 0xd8,
        0xc8, 0xc6, 0x81, 0x6f, 0x13, 0x63,
        0xe9, 0xa7, 0x9f, 0xbc, 0x29, 0xf9,
        0x2f, 0xb4, 0x78, 0x06, 0xe7, 0x71,
        0xd4, 0xab, 0x88, 0x8d, 0x72, 0xb9,
        0xf8, 0xac, 0x36, 0x2a, 0x3c, 0xf1,
        0x40, 0xd3, 0xbb, 0x43, 0x15, 0xad,
        0x77, 0x80, 0x82, 0xec, 0x27, 0xe5,
        0x85, 0x35, 0x0c, 0x41, 0xef, 0x93,
        0x19, 0x21, 0x0e, 0x4e, 0x65, 0xbd,
        0xb8, 0x8f, 0xeb, 0xce, 0x30, 0x5f,
        0xc5, 0x1a, 0xe1, 0xca, 0x47, 0x3d,
        0x01, 0xd6, 0x56, 0x4d, 0x0d, 0x66,
        0xcc, 0x2d, 0x12, 0x20, 0xb1, 0x99,
        0x4c, 0xc2, 0x7e, 0x05, 0xb7, 0x31,
        0x17, 0xd7, 0x58, 0x61, 0x1b, 0x1c,
        0x0f, 0x16, 0x18, 0x22, 0x44, 0xb2,
        0xb5, 0x91, 0x08, 0xa8, 0xfc, 0x50,
        0xd0, 0x7d, 0x89, 0x97, 0x5b, 0x95,
        0xff, 0xd2, 0xc4, 0x48, 0xf7, 0xdb,
        0x03, 0xda, 0x3f, 0x94, 0x5c, 0x02,
        0x4a, 0x33, 0x67, 0xf3, 0x7f, 0xe2,
        0x9b, 0x26, 0x37, 0x3b, 0x96, 0x4b,
        0xbe, 0x2e, 0x79, 0x8c, 0x6e, 0x8e,
        0xf5, 0xb6, 0xfd, 0x59, 0x98, 0x6a,
        0x46, 0xba, 0x25, 0x42, 0xa2, 0xfa,
        0x07, 0x55, 0xee, 0x0a, 0x49, 0x68,
        0x38, 0xa4, 0x28, 0x7b, 0xc9, 0xc1,
        0xe3, 0xf4, 0xc7, 0x9e
    }
};

struct camellia_sigma_t {
    union {
        std::uint64_t q;
        std::uint8_t b[8];
    } sigma[6];
    camellia_sigma_t()
    {
        unpack(0xA09E667F3BCC908B, sigma[0].b);
        unpack(0xB67AE8584CAA73B2, sigma[1].b);
        unpack(0xC6EF372FE94F82BE, sigma[2].b);
        unpack(0x54FF53A5F1D36F1C, sigma[3].b);
        unpack(0x10E527FADE682D1D, sigma[4].b);
        unpack(0xB05688C2B3E6C1FD, sigma[5].b);
    }
    std::uint64_t operator[](unsigned int i) const noexcept { return sigma[i].q; }
} const camellia_sigma;
}

template<size_t S, class = void>
class camellia;

template<size_t S>
class camellia<S, std::enable_if_t<S == 16 || S == 24 || S == 32>> {
public:
    static constexpr unsigned int t = 4;
    static constexpr unsigned int u = std::min((unsigned)S+2u, 24u);
    static constexpr unsigned int v = std::min((unsigned)S / 4u, 6u);
    static constexpr unsigned int nr = std::min((unsigned)S+2u, 24u);
    static constexpr size_t block_size = 16;

    camellia(const std::uint8_t* key)
    {
        key_schedule(key);
    }
    camellia() = default;
    void key_schedule(const std::uint8_t* key) noexcept
    {
        std::uint64_t klr[4];
        std::uint64_t ka[2];
        std::uint64_t kb[2];
        std::memcpy(klr, key, S);
        if constexpr (S == 16)
            std::memset(klr+2, 0, 16);
        else if constexpr (S == 24)
            klr[3] = ~klr[2];

        ka[0] = klr[0] ^ klr[2];
        ka[1] = klr[1] ^ klr[3];

        ka[1] ^= f(ka[0], detail::camellia_sigma[0]);
        ka[0] ^= f(ka[1], detail::camellia_sigma[1]);
        ka[0] ^= klr[0];
        ka[1] ^= klr[1];
        ka[1] ^= f(ka[0], detail::camellia_sigma[2]);
        ka[0] ^= f(ka[1], detail::camellia_sigma[3]);
        // 副鍵
        std::uint8_t kl8[16], kr8[16], ka8[16], kb8[16], kbuf[16];
        std::memcpy(kl8, klr, 16);
        std::memcpy(kw_, klr, 16); // init
        std::memcpy(kr8, klr+2, 16);
        std::memcpy(ka8, ka, 16);

        if constexpr (S > 16) {
            std::memcpy(kb, klr+2, 16);
            kb[0] ^= ka[0];
            kb[1] ^= ka[1];

            kb[1] ^= f(kb[0], detail::camellia_sigma[4]);
            kb[0] ^= f(kb[1], detail::camellia_sigma[5]);
            std::memcpy(kb8, kb, 16);
            
            // 副鍵 192 256 bit
            // kb
            std::memcpy(k_, kb8, 16); // 1,2
            rotl_array(kb8, kbuf, 30); // 7,8
            std::memcpy(k_+6, kbuf, 16);
            rotl_array(kb8, kbuf, 60); // 15,16
            std::memcpy(k_+14, kbuf, 16);
            rotl_array(kb8, kbuf, 111); // last
            std::memcpy(kw_+2, kbuf, 16);
            // ka
            rotl_array(ka8, kbuf, 15); // 5,6
            std::memcpy(k_+4, kbuf, 16);
            rotl_array(ka8, kbuf, 45); // 11,12
            std::memcpy(k_+10, kbuf, 16);
            rotl_array(ka8, kbuf, 77); // fl,fl_inv
            std::memcpy(kl_+4, kbuf, 16);
            rotl_array(ka8, kbuf, 94); // 21,22
            std::memcpy(k_+20, kbuf, 16);
            // kl
            rotl_array(kl8, kbuf, 45);  // 9,10
            std::memcpy(k_+8, kbuf, 16);
            rotl_array(kl8, kbuf, 60);  // fl,fl_inv
            std::memcpy(kl_+2, kbuf, 16);
            rotl_array(kl8, kbuf, 77);  // 17,18
            std::memcpy(k_+16, kbuf, 16);
            rotl_array(kl8, kbuf, 111);  // 23,24
            std::memcpy(k_+22, kbuf, 16);
            // kr
            rotl_array(kr8, kbuf, 15); // 3,4
            std::memcpy(k_+2, kbuf, 16);
            rotl_array(kr8, kbuf, 30); // fl, fl_inv
            std::memcpy(kl_, kbuf, 16);
            rotl_array(kr8, kbuf, 60); // 13,14
            std::memcpy(k_+12, kbuf, 16);
            rotl_array(kr8, kbuf, 94); // 19,20
            std::memcpy(k_+18, kbuf, 16);
        } else {
            // 副鍵 128 bit
            // ka
            std::memcpy(k_, ka8, 16); // 1,2
            rotl_array(ka8, kbuf, 15); // 5,6
            std::memcpy(k_ + 4, kbuf, 16);
            rotl_array(ka8, kbuf, 30); // fl,fl_inv
            std::memcpy(kl_, kbuf, 16);
            rotl_array(ka8, kbuf, 45); // 9
            std::memcpy(k_ + 8, kbuf, 8);
            rotl_array(ka8, kbuf, 60); // 11,12
            std::memcpy(k_ + 10, kbuf, 16);
            rotl_array(ka8, kbuf, 94); // 15,16
            std::memcpy(k_ + 14, kbuf, 16);
            rotl_array(ka8, kbuf, 111); // last
            std::memcpy(kw_ + 2, kbuf, 16);
            // kl
            rotl_array(kl8, kbuf, 15); // 3,4
            std::memcpy(k_ + 2, kbuf, 16);
            rotl_array(kl8, kbuf, 45); // 7,8
            std::memcpy(k_ + 6, kbuf, 16);
            rotl_array(kl8, kbuf, 60); // 10
            std::memcpy(k_ + 9, kbuf + 8, 8);
            rotl_array(kl8, kbuf, 77); // fl, fl_inv
            std::memcpy(kl_ + 2, kbuf, 16);
            rotl_array(kl8, kbuf, 94); // 13,14
            std::memcpy(k_ + 12, kbuf, 16);
            rotl_array(kl8, kbuf, 111); // 17,18
            std::memcpy(k_ + 16, kbuf, 16);
        }
    }

    inline static std::uint64_t f(std::uint64_t x, std::uint64_t k) noexcept
    {
        return p(s(x^k));
    }
    inline static std::uint64_t fl(std::uint64_t x, std::uint64_t k) noexcept
    {
        union {
            union {
                std::uint32_t d;
                std::uint8_t b[4];
            } dw[2];
            std::uint64_t q;
        }xd,kd, y;
        xd.q = x;
        kd.q = k;
        // yr
        y.dw[1].d = xd.dw[0].d & kd.dw[0].d;
        rotl_array(y.dw[1].b, 1);
        y.dw[1].d ^= xd.dw[1].d;
        // yl
        y.dw[0].d = (y.dw[1].d | kd.dw[1].d) ^ xd.dw[0].d;
        return y.q;
    }
    inline static std::uint64_t inv_fl(std::uint64_t y, std::uint64_t k) noexcept
    {
        union {
            union {
                std::uint32_t d;
                std::uint8_t b[4];
            } dw[2];
            std::uint64_t q;
        }yd,kd, x;
        yd.q = y;
        kd.q = k;
        // xl
        x.dw[0].d = (yd.dw[1].d | kd.dw[1].d) ^ yd.dw[0].d;
        // xr
        x.dw[1].d = x.dw[0].d & kd.dw[0].d;
        rotl_array(x.dw[1].b, 1);
        x.dw[1].d ^= yd.dw[1].d;
        return x.q;
    }
    inline static std::uint64_t s(std::uint64_t x) noexcept
    {
        std::uint8_t* p8 = (std::uint8_t*)&x;
        p8[0] = s<1>(p8[0]);
        p8[1] = s<2>(p8[1]);
        p8[2] = s<3>(p8[2]);
        p8[3] = s<4>(p8[3]);
        p8[4] = s<2>(p8[4]);
        p8[5] = s<3>(p8[5]);
        p8[6] = s<4>(p8[6]);
        p8[7] = s<1>(p8[7]);
        return x;
    }
    inline static std::uint64_t p(std::uint64_t x) noexcept
    {
        std::uint8_t* p8 = (std::uint8_t*)&x;
        union { std::uint64_t q; std::uint8_t b[8]; } r;
        r.b[0] = p8[0] ^ p8[2] ^ p8[3] ^ p8[5] ^ p8[6] ^ p8[7];
        r.b[1] = p8[0] ^ p8[1] ^ p8[3] ^ p8[4] ^ p8[6] ^ p8[7];
        r.b[2] = p8[0] ^ p8[1] ^ p8[2] ^ p8[4] ^ p8[5] ^ p8[7];
        r.b[3] = p8[1] ^ p8[2] ^ p8[3] ^ p8[4] ^ p8[5] ^ p8[6];
        r.b[4] = p8[0] ^ p8[1] ^ p8[5] ^ p8[6] ^ p8[7];
        r.b[5] = p8[1] ^ p8[2] ^ p8[4] ^ p8[6] ^ p8[7];
        r.b[6] = p8[2] ^ p8[3] ^ p8[4] ^ p8[5] ^ p8[7];
        r.b[7] = p8[0] ^ p8[3] ^ p8[4] ^ p8[5] ^ p8[6];
        return r.q;
    }
    template<int I>
    inline static std::uint8_t s(std::uint8_t x) noexcept
    {
        return detail::camellia_sbox[I-1][x];
    }

    void normal_round(std::uint64_t* dp, unsigned i) const noexcept
    {
        std::uint64_t l;
        l = dp[0];
        dp[0] = dp[1] ^ f(l, k_[i]);
        dp[1] = l;
    }
    void enc_special_round(std::uint64_t* dp, unsigned i) const noexcept
    {
            std::uint64_t
                l = dp[1] ^ f(dp[0], k_[i]),
                r = dp[0];
            dp[0] = fl(l, kl_[(i+1)/3-2]);
            dp[1] = inv_fl(r, kl_[(i+1)/3-1]);
    }
    void dec_special_round(std::uint64_t* dp, unsigned i) const noexcept
    {
            std::uint64_t
                l = dp[1] ^ f(dp[0], k_[i]),
                r = dp[0];
            dp[0] = fl(l, kl_[(i)/3-1]);
            dp[1] = inv_fl(r, kl_[(i)/3-2]);
    }

    void cipher(std::uint8_t* data) const noexcept
    {
        auto dp = reinterpret_cast<std::uint64_t*>(data);
        dp[0] ^= kw_[0];
        dp[1] ^= kw_[1];

        for (unsigned int i = 0u; i < nr - 6;) {
            normal_round(dp, i++);
            normal_round(dp, i++);
            normal_round(dp, i++);
            normal_round(dp, i++);
            normal_round(dp, i++);
            enc_special_round(dp, i++);
        }
        normal_round(dp, nr-6);
        normal_round(dp, nr-5);
        normal_round(dp, nr-4);
        normal_round(dp, nr-3);
        normal_round(dp, nr-2);
        normal_round(dp, nr-1);
        std::swap(dp[0], dp[1]);
        dp[0] ^= kw_[2];
        dp[1] ^= kw_[3];

    }
    void inv_cipher(std::uint8_t* data) const noexcept
    {
        auto dp = reinterpret_cast<std::uint64_t*>(data);
        dp[0] ^= kw_[2];
        dp[1] ^= kw_[3];

        for (int i = nr; i > 6;) {
            normal_round(dp, --i);
            normal_round(dp, --i);
            normal_round(dp, --i);
            normal_round(dp, --i);
            normal_round(dp, --i);
            dec_special_round(dp, --i);
        }
        normal_round(dp, 5);
        normal_round(dp, 4);
        normal_round(dp, 3);
        normal_round(dp, 2);
        normal_round(dp, 1);
        normal_round(dp, 0);
        std::swap(dp[0], dp[1]);
        dp[0] ^= kw_[0];
        dp[1] ^= kw_[1];
    }
//private:
    std::uint64_t kw_[t];
    std::uint64_t k_[u];
    std::uint64_t kl_[v];
};

}
