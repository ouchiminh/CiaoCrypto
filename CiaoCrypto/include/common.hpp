﻿#pragma once
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <cassert>

#ifdef _MSC_VER
#include <stdlib.h>
#endif

namespace ciao {
template<class Int>
inline constexpr auto rotr(Int x, unsigned nbit) noexcept
-> std::enable_if_t<std::is_integral_v<Int> && std::is_unsigned_v<Int>, Int>
{
    constexpr unsigned w = sizeof(Int) * CHAR_BIT;
    return (x >> nbit) | (x << (w - nbit));
}
template<class Int>
inline constexpr auto rotl(Int x, unsigned nbit) noexcept
-> std::enable_if_t<std::is_integral_v<Int> && std::is_unsigned_v<Int>, Int>
{
    constexpr unsigned w = sizeof(Int) * CHAR_BIT;
    return (x << nbit) | (x >> (w - nbit));
}

namespace detail {

inline static const union { std::uint16_t w; std::uint8_t b[2]; } endian{(std::uint16_t)1};
inline static const bool is_big_endian = endian.b[1];

template<class T>
void prefetch(const T *table, size_t count) noexcept
{
    assert(sizeof(T) * count % 8  == 0);
    const volatile std::uint64_t* t = reinterpret_cast<const volatile std::uint64_t*>(table);
    volatile std::uint64_t ret;
    std::uint64_t sum = 0;
    for (size_t i = 0; i < count * sizeof(T) / sizeof(std::uint64_t); ++i)
        sum ^= t[i];
    ret = sum;
}

template<class T, size_t S>
auto prefetch(const T(&table)[S]) noexcept
-> std::enable_if_t<(sizeof(T)*S) % sizeof(std::uint64_t) == 0 && std::is_trivial_v<T>, void>
{
    prefetch(table, S);
}

template<class Int, size_t DestSize, size_t ...S>
inline constexpr void unpack_impl(Int src, std::uint8_t* dest, std::index_sequence<S...>)
{
    ((dest[S] = static_cast<std::uint8_t>(src >> (8 * (DestSize - S - 1)))), ...);
}
template<class Int, size_t DestSize, size_t ...S>
inline Int pack_impl(Int src, std::index_sequence<S...>) noexcept
{
    constexpr union {
        std::uint16_t d;
        std::uint8_t b[2];
    } endian{ (std::uint16_t)1 };
    if (endian.b[1]) {
        return src;
    }
#ifdef _MSC_VER
    else if constexpr (std::is_same_v<std::uint16_t, Int>) {
        return _byteswap_ushort(src);
    } else if constexpr (std::is_same_v<std::uint32_t, Int> || std::is_same_v<unsigned long, Int>) {
        static_assert(sizeof(Int) == 4);
        return _byteswap_ulong(src);
    } else if constexpr (std::is_same_v<std::uint64_t, Int>) {
        return _byteswap_uint64(src);
    }
#endif
    else {
        return (((0xFF & (src >> (S * 8))) << (8 * (DestSize - S - 1))) | ...);
    }
}
} // namespace detail

template<class Int, size_t S = sizeof(Int)>
inline auto unpack(Int src, void* dest)
-> std::enable_if_t<std::is_integral_v<Int>, void>
{
    //detail::unpack_impl<Int, S>(src, reinterpret_cast<std::uint8_t*>(dest),
    //                            std::make_index_sequence<S>{});
    *reinterpret_cast<Int*>(dest) =
        detail::pack_impl<Int, S>(src, std::make_index_sequence<S>{});
}

template<class Int, size_t S = sizeof(Int)>
inline auto pack(const void* src) noexcept
-> std::enable_if_t<std::is_integral_v<Int>, Int>
{
    return detail::pack_impl<Int, S>(*reinterpret_cast<const Int*>(src),
                                     std::make_index_sequence<S>{});
}

template<class Int>
inline constexpr auto lower_half_bits(Int i) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>, Int>
{
    constexpr Int mask = ~(~((Int)1 << (CHAR_BIT * sizeof(Int)/2)) + 1);
    return i & mask;
}
template<class Int>
inline constexpr auto higher_half_bits(Int i) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>, Int>
{
    constexpr Int mask = (~((Int)1 << (CHAR_BIT * sizeof(Int)/2)) + 1);
    return (i & mask);
}

inline std::uint32_t memassign32(std::uint8_t a,
                                 std::uint8_t b,
                                 std::uint8_t c,
                                 std::uint8_t d) noexcept
{
    union { 
        std::uint8_t b[4];
        std::uint32_t i;
    } r;
    r.b[0] = a;
    r.b[1] = b;
    r.b[2] = c;
    r.b[3] = d;
    return r.i;
}

namespace big_endian {

template<class Int, size_t S>
inline auto shiftr_array(Int(&data)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    assert(S * CHAR_BIT >= bit_width && CHAR_BIT == 8);

    auto elm_shift_width = (int)bit_width >> 3;
    auto bit_shift_mod_width = (int)bit_width & (CHAR_BIT - 1);    // bit_width mod 8

    Int buf[S] = {};

    auto f = ~((Int)1 << bit_shift_mod_width) + 1;
    auto s = ~f;

    for (unsigned i = 0; i < S - elm_shift_width; ++i) {
        if (i + elm_shift_width + 1 < S)
            buf[i + elm_shift_width + 1] |= (data[i] & s) << (CHAR_BIT - bit_shift_mod_width);
        buf[i + elm_shift_width] |= (data[i] & f) >> bit_shift_mod_width;
    }
    std::memcpy(data, buf, S);
}

template<class Int, size_t S>
inline auto shiftl_array(Int(&data)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    assert(S * CHAR_BIT >= bit_width && CHAR_BIT == 8);

    auto elm_shift_width = (int)bit_width >> 3;
    auto bit_shift_mod_width = (int)bit_width & (CHAR_BIT - 1);    // bit_width mod 8

    Int buf[S] = {};

    Int s = ~((Int)1 << (8-bit_shift_mod_width)) + 1;
    Int f = ~s;

    for (int i = elm_shift_width; i < (int)S; ++i) {
        if (i - elm_shift_width - 1 >= 0)
            buf[i - elm_shift_width - 1] |= (data[i] & s) >> (CHAR_BIT - bit_shift_mod_width);
        buf[i - elm_shift_width] |= (data[i] & f) << bit_shift_mod_width;
    }
    std::memcpy(data, buf, S);
}

template<class Int, size_t S>
inline auto rotl_array(Int(&bits)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    constexpr auto w = CHAR_BIT * S;
    //return (x << nbit) | (x >> (w - nbit));
    Int buf[S];
    std::memcpy(buf, bits, S);
    shiftl_array(bits, bit_width);
    shiftr_array(buf, w - bit_width);
    for (unsigned i = 0; i < S; ++i) bits[i] |= buf[i];
}

template<class Int, size_t S>
inline auto rotr_array(Int(&bits)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    constexpr auto w = CHAR_BIT * S;
    //return (x << nbit) | (x >> (w - nbit));
    Int buf[S];
    std::memcpy(buf, bits, S);
    shiftr_array(bits, bit_width);
    shiftl_array(buf, w - bit_width);
    for (unsigned i = 0; i < S; ++i) bits[i] |= buf[i];
}

template<class Int, size_t S>
inline auto rotl_array(const Int(&bits)[S], Int(&dest)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    constexpr auto w = CHAR_BIT * S;
    Int buf[S];
    std::memcpy(buf, bits, S);
    std::memcpy(dest, bits, S);
    shiftl_array(buf, bit_width);
    shiftr_array(dest, w - bit_width);
    for (unsigned i = 0; i < S; ++i) dest[i] |= buf[i];
}

template<class Int, size_t S>
inline auto rotr_array(const Int(&bits)[S], Int(&dest)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    constexpr auto w = CHAR_BIT * S;
    Int buf[S];
    std::memcpy(buf, bits, S);
    std::memcpy(dest, bits, S);
    shiftr_array(buf, bit_width);
    shiftl_array(dest, w - bit_width);
    for (unsigned i = 0; i < S; ++i) dest[i] |= buf[i];
}
} // namespace big_endian

namespace native_endian {

template<class Int, size_t S>
inline auto shiftr_array(Int(&bits)[S], unsigned bit_shift_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>>
{
    static_assert(CHAR_BIT == 8);
    assert(bit_shift_width < sizeof(bits) * CHAR_BIT);

    using detail::is_big_endian;
    constexpr unsigned wbit = CHAR_BIT * sizeof(Int);
    unsigned bit_shift_mod_width = bit_shift_width & (wbit - 1);
    unsigned elm_shift_width = bit_shift_width / wbit;

    Int f = (~((Int)1 << bit_shift_mod_width))+1;   // high
    Int s = ~f;
    Int buf[S] = {};
    if (is_big_endian) {
        for (int i = 0; i < S - elm_shift_width; ++i) {
            if (i + elm_shift_width + 1 < (int)S)
                buf[i + elm_shift_width + 1] |= (bits[i] & s) << (wbit - bit_shift_mod_width);
            buf[i + elm_shift_width] |= (bits[i] & f) >> bit_shift_mod_width;
        }
    } else {
        for (int i = elm_shift_width; i < (int)S; ++i) {
            if (i - (int)elm_shift_width - 1 >= 0)
                buf[i - elm_shift_width - 1] |= (bits[i] & s) << (wbit - bit_shift_mod_width);
            buf[i - elm_shift_width] |= (bits[i] & f) >> bit_shift_mod_width;
        }
    }
    std::memcpy(bits, buf, S * sizeof(Int));
}

template<class Int, size_t S>
inline auto shiftl_array(Int(&bits)[S], unsigned bit_shift_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>>
{
    static_assert(CHAR_BIT == 8);
    assert(bit_shift_width < sizeof(bits) * CHAR_BIT);

    using detail::is_big_endian;
    constexpr unsigned wbit = CHAR_BIT * sizeof(Int);
    unsigned bit_shift_mod_width = bit_shift_width & (wbit - 1);
    unsigned elm_shift_width = bit_shift_width / wbit;

    Int s = (~((Int)1 << (wbit - bit_shift_mod_width))) + 1;   // high
    Int f = ~s;
    Int buf[S] = {};
    if (is_big_endian) {
        for (int i = elm_shift_width; i < (int)S; ++i) {
            if (i - (int)elm_shift_width - 1 >= 0)
                buf[i - elm_shift_width - 1] |= (bits[i] & s) >> (wbit - bit_shift_mod_width);
            buf[i - elm_shift_width] |= (bits[i] & f) << bit_shift_mod_width;
        }
    } else {
        for (int i = 0; i < (int)(S - elm_shift_width); ++i) {
            if (i + elm_shift_width + 1 < S)
                buf[i + elm_shift_width + 1] |= (bits[i] & s) >> (wbit - bit_shift_mod_width);
            buf[i + elm_shift_width] |= (bits[i] & f) << bit_shift_mod_width;
        }
    }
    std::memcpy(bits, buf, S * sizeof(Int));
}

template<class Int, size_t S>
inline auto rotl_array(Int(&bits)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>>
{
    constexpr auto w = sizeof(Int) * CHAR_BIT * S;
    //return (x << nbit) | (x >> (w - nbit));
    Int buf[S];
    std::memcpy(buf, bits, w/8);
    shiftl_array(bits, bit_width);
    shiftr_array(buf, w - bit_width);
    for (unsigned i = 0; i < S; ++i) bits[i] |= buf[i];
}

template<class Int, size_t S>
inline auto rotr_array(Int(&bits)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>>
{
    constexpr auto w = sizeof(Int) * CHAR_BIT * S;
    //return (x << nbit) | (x >> (w - nbit));
    Int buf[S];
    std::memcpy(buf, bits, w/8);
    shiftr_array(bits, bit_width);
    shiftl_array(buf, w - bit_width);
    for (unsigned i = 0; i < S; ++i) bits[i] |= buf[i];
}

template<class Int, size_t S>
inline auto rotl_array(const Int(&bits)[S], Int(&dest)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>>
{
    constexpr auto w = sizeof(Int) * CHAR_BIT * S;
    Int buf[S];
    std::memcpy(buf, bits, w / 8);
    std::memcpy(dest, bits, w / 8);
    shiftl_array(buf, bit_width);
    shiftr_array(dest, w - bit_width);
    for (unsigned i = 0; i < S; ++i) dest[i] |= buf[i];
}

template<class Int, size_t S>
inline auto rotr_array(const Int(&bits)[S], Int(&dest)[S], unsigned bit_width) noexcept
-> std::enable_if_t<std::is_unsigned_v<Int>>
{
    constexpr auto w = sizeof(Int) * CHAR_BIT * S;
    Int buf[S];
    std::memcpy(buf, bits, w/8);
    std::memcpy(dest, bits, w/8);
    shiftr_array(buf, bit_width);
    shiftl_array(dest, w - bit_width);
    for (unsigned i = 0; i < S; ++i) dest[i] |= buf[i];
}
} // namespace native_endian

}

