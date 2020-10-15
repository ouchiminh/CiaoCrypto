#pragma once
#include "../common.hpp"
#include <ouchilib/math/gf.hpp>
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <utility>

namespace ciao {

namespace detail {

struct sbox_t {
    static constexpr std::uint8_t data[] = {
        0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
    };
    constexpr std::uint8_t operator[](int i) const noexcept { return data[i]; }
};

struct inv_sbox_t {
    static constexpr std::uint8_t data[256] = {
        0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
        0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
        0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
        0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
        0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
        0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
        0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
        0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
        0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
        0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
        0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
        0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
        0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
        0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
        0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
        0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
    };
    constexpr std::uint8_t operator[](int i) const noexcept { return data[i]; }
};

inline constexpr sbox_t sbox;
inline constexpr inv_sbox_t inv_sbox;

struct gf_mul {
    using gf = ouchi::math::gf256<0x1b>;
    std::uint32_t data2113[256];
    std::uint32_t data3211[256];
    std::uint32_t data1321[256];
    std::uint32_t data1132[256];
    std::uint32_t datae9db[256];
    std::uint32_t databe9d[256];
    std::uint32_t datadbe9[256];
    std::uint32_t data9dbe[256];
    gf_mul()
    {
        constexpr sbox_t sbox;
        constexpr inv_sbox_t inv_sbox;
        for (int i = 0; i < 256; ++i) {
            auto buf = sbox[i];
            std::uint8_t a = gf::mul(buf, 2), b = gf::mul(buf, 3);
            data2113[i] = memassign32(a, buf, buf, b);
            data3211[i] = memassign32(b, a, buf, buf);
            data1321[i] = memassign32(buf, b, a, buf);
            data1132[i] = memassign32(buf, buf, b, a);
            std::uint8_t d9 = gf::mul(i, 9), db = gf::mul(i, 0xb), dd = gf::mul(i, 0xd), de = gf::mul(i, 0xe);
            datae9db[i] = memassign32(de, d9, dd, db);
            databe9d[i] = memassign32(db, de, d9, dd);
            datadbe9[i] = memassign32(dd, db, de, d9);
            data9dbe[i] = memassign32(d9, dd, db, de);
        }
    }
};
inline gf_mul gf;

template<class T, size_t S>
auto prefetch(const T(&table)[S]) noexcept
-> std::enable_if_t<S % sizeof(std::uint64_t) == 0, void>
{
    const volatile std::uint64_t* t = reinterpret_cast<const volatile std::uint64_t*>(&*table);
    volatile std::uint64_t ret;
    std::uint64_t sum;
    for (size_t i = 0; i < S * sizeof(T) / sizeof(std::uint64_t); ++i)
        sum ^= t[i];
    ret = sum;
}

} // namespace detail;

template<size_t K, class = void>
struct aes;

template<size_t K>
struct aes<K, std::enable_if_t<K == 16 || K ==24 || K ==32>> {
    static constexpr size_t block_size = 16;
    static constexpr size_t key_size = K;
    static constexpr unsigned int nb = 4;
    static constexpr unsigned int nr = K / 4 + 6;
    static constexpr unsigned int nk = K / 4;
    static constexpr std::uint32_t Rcon[10] = {
        0x0100'0000,0x0200'0000,0x0400'0000,0x0800'0000,0x100'00000,
        0x2000'0000,0x4000'0000,0x8000'0000,0x1b00'0000,0x3600'0000
    };

    aes() noexcept = default;
    aes(const void* key) noexcept
    {
        key_expansion(key);
    }
    ~aes()
    {
        std::fill(const_cast<volatile std::uint32_t*>(w_),
                  const_cast<volatile std::uint32_t*>(w_+sizeof(w_)/sizeof(*w_)),
                  0u);
        std::fill(const_cast<volatile std::uint8_t*>(w8_),
                  const_cast<volatile std::uint8_t*>(w8_+sizeof(w8_)/sizeof(*w8_)),
                  0u);
    }

    void key_expansion(const void* key) noexcept
    {
        unsigned i = 0;
        while (i < nk) {
            w_[i++] = pack<std::uint32_t>((std::uint8_t*)key + i * sizeof(std::uint32_t));
        }
        for (i = nk; i < sizeof(w_)/sizeof(*w_); i += nk) {
            w_[i] = w_[i-nk] ^ subword(rotword(w_[i-1])) ^ Rcon[i/nk-1];
            w_[i+1] = w_[i+1-nk] ^ w_[i+1-1];
            w_[i+2] = w_[i+2-nk] ^ w_[i+2-1];
            w_[i+3] = w_[i+3-nk] ^ w_[i+3-1];
            if constexpr (nk != 4) {
                if (i+4 >= sizeof(w_)/sizeof(*w_)) break;
            }
            if constexpr (nk == 6) {
                w_[i+4] = w_[i+4-nk] ^ w_[i+4-1];
                w_[i+5] = w_[i+5-nk] ^ w_[i+5-1];
            }
            if constexpr (nk == 8) {
                w_[i+4] = w_[i+4-nk] ^ subword(w_[i+4-1]);
                w_[i+5] = w_[i+5-nk] ^ w_[i+5-1];
                w_[i+6] = w_[i+6-nk] ^ w_[i+6-1];
                w_[i+7] = w_[i+7-nk] ^ w_[i+7-1];
            }
        }
        for (i = 0; i < sizeof(w_)/sizeof(*w_); ++i) {
            unpack(w_[i], w8_+i*nb);
        }
    }
    void cipher(std::uint8_t* data) const noexcept
    {
        add_roundkey(data, 0);
        enc_round(data, std::make_index_sequence<nr-1>{});
        enc_final_round(data);
    }
    void inv_cipher(std::uint8_t* data) const noexcept
    {
        add_roundkey(data, nr);
        for (auto i = nr - 1u; i > 0; --i) {
            dec_round(data, i);
        }
        inv_sub_bytes_shift_rows(data);
        add_roundkey(data, 0);
    }

    template<class SBox = detail::sbox_t>
    inline static std::uint32_t subword(std::uint32_t i) noexcept
    {
        constexpr SBox sbox;
        union { std::uint32_t l; std::uint8_t b[4]; } tmp;
        tmp.l = i;
        tmp.b[0] = sbox[tmp.b[0]];
        tmp.b[1] = sbox[tmp.b[1]];
        tmp.b[2] = sbox[tmp.b[2]];
        tmp.b[3] = sbox[tmp.b[3]];
        return tmp.l;
    }

    inline static std::uint32_t rotword(std::uint32_t i) noexcept
    {
        return rotl(i, 8);
    }
    template<size_t ...S>
    inline void enc_round(std::uint8_t* data, std::index_sequence<S...>) const noexcept
    {
        (enc_round(data, S+1), ...);
    }
    inline void enc_round(std::uint8_t* data, size_t r) const noexcept
    {
        std::uint8_t buf[block_size];
        std::uint32_t* p = reinterpret_cast<std::uint32_t*>(data);
        buf[0]  = data[0];
        buf[13] = data[1];
        buf[10] = data[2];
        buf[7]  = data[3];
        buf[4]  = data[4];
        buf[1]  = data[5];
        buf[14] = data[6];
        buf[11] = data[7];
        buf[8]  = data[8];
        buf[5]  = data[9];
        buf[2]  = data[10];
        buf[15] = data[11];
        buf[12] = data[12];
        buf[9]  = data[13];
        buf[6]  = data[14];
        buf[3]  = data[15];
        std::memcpy(data, w8_ + nb*nb*r, block_size);
        p[0] ^= detail::gf.data2113[buf[0]] ^ detail::gf.data3211[buf[1]] ^ detail::gf.data1321[buf[2]] ^ detail::gf.data1132[buf[3]];
        p[1] ^= detail::gf.data2113[buf[4]] ^ detail::gf.data3211[buf[5]] ^ detail::gf.data1321[buf[6]] ^ detail::gf.data1132[buf[7]];
        p[2] ^= detail::gf.data2113[buf[8]] ^ detail::gf.data3211[buf[9]] ^ detail::gf.data1321[buf[10]] ^ detail::gf.data1132[buf[11]];
        p[3] ^= detail::gf.data2113[buf[12]] ^ detail::gf.data3211[buf[13]] ^ detail::gf.data1321[buf[14]] ^ detail::gf.data1132[buf[15]];
    }
    inline void enc_final_round(std::uint8_t* data) const noexcept
    {
        std::uint8_t buf[block_size];
        std::memcpy(buf, data, block_size);
        std::memcpy(data, w8_+nb*nb*nr, block_size);
        data[0]  ^= detail::sbox[buf[0]];
        data[1]  ^= detail::sbox[buf[5]];
        data[2]  ^= detail::sbox[buf[10]];
        data[3]  ^= detail::sbox[buf[15]];
        data[4]  ^= detail::sbox[buf[4]];
        data[5]  ^= detail::sbox[buf[9]];
        data[6]  ^= detail::sbox[buf[14]];
        data[7]  ^= detail::sbox[buf[3]];
        data[8]  ^= detail::sbox[buf[8]];
        data[9]  ^= detail::sbox[buf[13]];
        data[10] ^= detail::sbox[buf[2]];
        data[11] ^= detail::sbox[buf[7]];
        data[12] ^= detail::sbox[buf[12]];
        data[13] ^= detail::sbox[buf[1]];
        data[14] ^= detail::sbox[buf[6]];
        data[15] ^= detail::sbox[buf[11]];
    }
    inline void dec_round(std::uint8_t* data, unsigned r) const noexcept
    {
        std::uint8_t buf[block_size];
        buf[0]  = detail::inv_sbox[data[0]];
        buf[1]  = detail::inv_sbox[data[13]];
        buf[2]  = detail::inv_sbox[data[10]];
        buf[3]  = detail::inv_sbox[data[7]];
        buf[4]  = detail::inv_sbox[data[4]];
        buf[5]  = detail::inv_sbox[data[1]];
        buf[6]  = detail::inv_sbox[data[14]];
        buf[7]  = detail::inv_sbox[data[11]];
        buf[8]  = detail::inv_sbox[data[8]];
        buf[9]  = detail::inv_sbox[data[5]];
        buf[10] = detail::inv_sbox[data[2]];
        buf[11] = detail::inv_sbox[data[15]];
        buf[12] = detail::inv_sbox[data[12]];
        buf[13] = detail::inv_sbox[data[9]];
        buf[14] = detail::inv_sbox[data[6]];
        buf[15] = detail::inv_sbox[data[3]];
        unsigned i4 = 0;
        auto* p = reinterpret_cast<std::uint32_t*>(data);
        add_roundkey(buf, r);

        for (auto i = 0u; i < 4; ++i) {
            i4 = i << 2;
            p[i] = detail::gf.datae9db[buf[i4]] ^
                detail::gf.databe9d[buf[i4+1]] ^
                detail::gf.datadbe9[buf[i4+2]] ^
                detail::gf.data9dbe[buf[i4+3]];
        }
    }
    inline static void inv_sub_bytes_shift_rows(std::uint8_t* data) noexcept
    {
        std::uint8_t buf[block_size];
        std::memcpy(buf, data, block_size);
        data[0]  = detail::inv_sbox[buf[0]];
        data[1]  = detail::inv_sbox[buf[13]];
        data[2]  = detail::inv_sbox[buf[10]];
        data[3]  = detail::inv_sbox[buf[7]];
        data[4]  = detail::inv_sbox[buf[4]];
        data[5]  = detail::inv_sbox[buf[1]];
        data[6]  = detail::inv_sbox[buf[14]];
        data[7]  = detail::inv_sbox[buf[11]];
        data[8]  = detail::inv_sbox[buf[8]];
        data[9]  = detail::inv_sbox[buf[5]];
        data[10] = detail::inv_sbox[buf[2]];
        data[11] = detail::inv_sbox[buf[15]];
        data[12] = detail::inv_sbox[buf[12]];
        data[13] = detail::inv_sbox[buf[9]];
        data[14] = detail::inv_sbox[buf[6]];
        data[15] = detail::inv_sbox[buf[3]];
    }

//private:
    inline void add_roundkey(std::uint8_t* data, unsigned r) const noexcept
    {
        auto* dp = reinterpret_cast<std::uint64_t*>(data);
        auto* kp = reinterpret_cast<const std::uint64_t*>(w8_ + nb*nb*r);

        dp[0] ^= kp[0];
        dp[1] ^= kp[1];
    }
    std::uint32_t w_[nb*(nr+1)];
    std::uint8_t w8_[nb*nb*(nr+1)];
};

}

