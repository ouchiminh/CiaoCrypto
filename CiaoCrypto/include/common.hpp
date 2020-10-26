#pragma once
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <cassert>

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
template<class Int, size_t ...S>
inline constexpr void unpack_impl(Int src, std::uint8_t* dest, std::index_sequence<S...>)
{
    ((dest[S] = static_cast<std::uint8_t>(src >> (8 * (sizeof(Int) - S - 1)))), ...);
}
template<class Int, size_t ...S>
inline constexpr Int pack_impl(const std::uint8_t* src, std::index_sequence<S...>) noexcept
{
    return ((static_cast<Int>(src[S]) << (8 * (sizeof(Int) - S - 1))) | ...);
}
} // namespace detail

template<class Int>
inline constexpr auto unpack(Int src, void* dest)
-> std::enable_if_t<std::is_integral_v<Int>, void>
{
    auto* ptr = reinterpret_cast<std::uint8_t*>(dest);
    detail::unpack_impl(src, ptr, std::make_index_sequence<sizeof(Int)>{});
}

template<class Int>
inline constexpr auto pack(const void* src) noexcept
-> std::enable_if_t<std::is_integral_v<Int>, Int>
{
    return detail::pack_impl<Int>(reinterpret_cast<const std::uint8_t*>(src),
                                  std::make_index_sequence<sizeof(Int)>{});
}

inline std::uint32_t memassign32(std::uint8_t a,
                                 std::uint8_t b,
                                 std::uint8_t c,
                                 std::uint8_t d)
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

template<class Int, size_t S>
inline auto shiftr_array(Int (&data)[S], unsigned bit_width)
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    assert(S * CHAR_BIT >= bit_width && CHAR_BIT == 8);

    auto elm_shift_width = (int)bit_width >> 3;
    auto bit_shift_mod_width = (int)bit_width & (CHAR_BIT - 1);    // bit_width mod 8

    Int buf[S] = {};

    constexpr std::uint8_t lowbit[] = { 0,1,3,7,15,31,63,127 };
    auto s = lowbit[bit_shift_mod_width];
    auto f = ~s;

    for (unsigned i = 0; i < S - elm_shift_width; ++i) {
        if(i + elm_shift_width + 1 < S)
            buf[i + elm_shift_width + 1] |= (data[i] & s) << (CHAR_BIT - bit_shift_mod_width);
        buf[i + elm_shift_width] |= (data[i] & f) >> bit_shift_mod_width;
    }
    std::memcpy(data, buf, S);
}

template<class Int, size_t S>
inline auto shiftl_array(Int (&data)[S], unsigned bit_width)
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    assert(S * CHAR_BIT >= bit_width && CHAR_BIT == 8);

    auto elm_shift_width = (int)bit_width >> 3;
    auto bit_shift_mod_width = (int)bit_width & (CHAR_BIT - 1);    // bit_width mod 8

    Int buf[S] = {};

    constexpr std::uint8_t highbit[] = { 0,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff };
    auto s = highbit[bit_shift_mod_width];
    auto f = ~s;

    for (int i = elm_shift_width; i < (int)S; ++i) {
        if(i - elm_shift_width - 1 >= 0)
            buf[i - elm_shift_width - 1] |= (data[i] & s) >> (CHAR_BIT - bit_shift_mod_width);
        buf[i - elm_shift_width] |= (data[i] & f) << bit_shift_mod_width;
    }
    std::memcpy(data, buf, S);
}

template<class Int, size_t S>
inline auto rotl_array(Int(&bits)[S], unsigned bit_width)
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
inline auto rotr_array(Int(&bits)[S], unsigned bit_width)
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
inline auto rotl_array(Int(&bits)[S], Int(&dest)[S], unsigned bit_width)
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    constexpr auto w = CHAR_BIT * S;
    //return (x << nbit) | (x >> (w - nbit));
    std::memcpy(dest, bits, S);
    shiftl_array(bits, bit_width);
    shiftr_array(dest, w - bit_width);
    for (unsigned i = 0; i < S; ++i) dest[i] |= bits[i];
}

template<class Int, size_t S>
inline auto rotr_array(Int(&bits)[S], Int(&dest)[S], unsigned bit_width)
-> std::enable_if_t<std::is_unsigned_v<Int> && sizeof(Int) == 1>
{
    constexpr auto w = CHAR_BIT * S;
    //return (x << nbit) | (x >> (w - nbit));
    Int buf[S];
    std::memcpy(dest, bits, S);
    shiftr_array(bits, bit_width);
    shiftl_array(dest, w - bit_width);
    for (unsigned i = 0; i < S; ++i) dest[i] |= bits[i];
}


}

