#pragma once
#include <utility>
#include <type_traits>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <concepts>

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

    virtual rsize_t cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) = 0;
    virtual rsize_t inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) = 0;
protected:
    static rsize_t pad_src(const void* src, rsize_t srcsize, void* dest, rsize_t destsize, std::uint8_t width) noexcept
    {
        assert(width > 0);
        auto res_size = (srcsize / width + 1) * width;
        auto pad_size = (std::uint8_t)(0xFF & (res_size - srcsize));
        if (res_size > destsize) return SIZE_MAX;
        std::memcpy(dest, src, srcsize);
        for (rsize_t i = srcsize; i < res_size; ++i)
            *((std::uint8_t*)dest + i) = pad_size;
        return res_size;
    }
    A algorithm_;
};

template<detail::block_cipher_algorithm A>
class ecb : public block_cipher<A> {
public:
    using block_cipher::block_cipher;

    virtual rsize_t cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) override
    {
        if (pad_src(src, srcsize, dest, destsize, A::block_size) > SIZE_MAX) return SIZE_MAX;
    }
    virtual rsize_t inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) noexcept(noexcept(algorithm_.cipher(std::declval<std::uint8_t*>()))) = 0;

};

template<detail::block_cipher_algorithm A>
class cbc;

template<detail::block_cipher_algorithm A>
class ctr;

}

