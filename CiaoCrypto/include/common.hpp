#pragma once
#include <type_traits>
#include <cstdint>

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

inline std::uint32_t pack32(std::uint8_t a,
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

}

