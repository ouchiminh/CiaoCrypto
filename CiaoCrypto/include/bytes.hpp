#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <intrin.h>

namespace ciao {

template<size_t Size, class = void>
union bytes {
    std::uint8_t byte_data[Size];
};


template<size_t Size>
union alignas(__m128i) bytes<Size, std::enable_if_t<Size == 16>> {
    std::uint8_t byte_data[Size];
    __m128i data;

    bytes(const void* ptr)
    {
        std::memcpy(byte_data, ptr, Size);
    }
    bytes() = default;

    inline void xor_op(const bytes<Size>& src) noexcept {
        data = _mm_xor_si128(data, src.data);
    }
    inline void xor_op(__m128i src) noexcept {
        data = _mm_xor_si128(data, src);
    }
    inline static void xor_op(std::uint8_t* srcdest, const std::uint8_t* src) noexcept
    {
        __m128i a = _mm_loadu_si128((__m128i*)srcdest),
            b = _mm_loadu_si128((const __m128i*)src);
        a = _mm_xor_si128(a, b);
        _mm_storeu_si128((__m128i*)srcdest, a);
    }
};
static_assert(sizeof(bytes<16>) == 16);


}

