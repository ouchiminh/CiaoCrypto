#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <intrin.h>

namespace ciao {

template<size_t Size, class = void>
union bytes;


template<size_t Size>
union bytes<Size, std::enable_if_t<Size == 16>> {
    std::uint8_t byte_data[Size];
    __m128i data;

    inline void xor(__m128i src){
        data = _mm_xor_si128(data, src);
    }
};
static_assert(sizeof(bytes<16>) == 16);
}

