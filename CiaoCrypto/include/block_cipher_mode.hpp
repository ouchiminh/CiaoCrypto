#pragma once
#include <utility>
#include <type_traits>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <concepts>

#include <mitama/result/result.hpp>

#include "error_code.hpp"

namespace ciao {

namespace detail {
template<class A>
concept block_cipher_algorithm = requires(A a)
{
    A{ std::declval<const std::uint8_t*>() };
    (size_t)A::block_size;
    a.cipher(std::declval<std::uint8_t*>());
    a.inv_cipher(std::declval<std::uint8_t*>());
};
}

template<detail::block_cipher_algorithm A>
class block_cipher {
public:
    block_cipher() = delete;
    block_cipher(const std::uint8_t* key)
        : algorithm_{ key }
    {}
    virtual ~block_cipher() = default;

    virtual mitama::result<rsize_t, error_code> cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) = 0;
    virtual mitama::result<rsize_t, error_code> inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) = 0;
protected:
    static rsize_t pad(const void* src, size_t srcsize, void* dest, size_t destsize, std::uint8_t width) noexcept
    {
        assert(width > 0);
        auto res_size = (srcsize / width + 1) * width;
        auto pad_size = (std::uint8_t)(0xFF & (res_size - srcsize));
        if (res_size > destsize) return SIZE_MAX;
        std::memcpy(dest, src, srcsize);
        for (size_t i = srcsize; i < res_size; ++i)
            *((std::uint8_t*)dest + i) = pad_size;
        return res_size;
    }
    A algorithm_;
};

template<detail::block_cipher_algorithm A>
class ecb : public block_cipher<A> {
public:
    using block_cipher::block_cipher;

    virtual mitama::result<rsize_t, error_code> cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) override
    {
        auto actual_size = pad(src, srcsize, dest, destsize, A::block_size);
        if (srcsize > RSIZE_MAX) return mitama::failure(error_code(error_value::invalid_arguments));
        if (actual_size == SIZE_MAX) return mitama::failure(error_code(error_value::too_short_buffer));

        for (rsize_t i = 0; i < actual_size; ++i) {
            algorithm_.cipher(dest + i);
        }
        return mitama::success(actual_size);
        
    }
    virtual mitama::result<rsize_t, error_code> inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) override
    {
        if (srcsize > RSIZE_MAX && srcsize % A::block_size != 0) return mitama::failure(error_code(error_value::invalid_arguments));
        if (srcsize > destsize) return mitama::failure(error_code(error_value::too_short_buffer));
        std::memcpy(dest, src, srcsize);
        
        for (rsize_t i = 0; i < srcsize; ++i) {
            algorithm_.inv_cipher(dest + i);
        }
        auto pad_size = dest[srcsize - 1];
        for (size_t i = 0; i < pad_size; ++i) {
            if (dest[srcsize - i - 1] != pad_size) return mitama::failure(error_code(error_value::corrupted_data));
        }
        return mitama::success(srcsize - pad_size);
    }

};

template<detail::block_cipher_algorithm A>
class cbc;

template<detail::block_cipher_algorithm A>
class ctr;

}

