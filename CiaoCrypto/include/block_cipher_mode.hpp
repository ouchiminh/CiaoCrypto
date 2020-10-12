#pragma once
#include <utility>
#include <type_traits>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <concepts>

#include <ouchilib/result/result.hpp>

#include "error_code.hpp"
#include "bytes.hpp"

namespace ciao {

namespace detail {
template<class A>
concept block_cipher_algorithm = requires(const A a)
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

    virtual ouchi::result::result<rsize_t, error_code> cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) = 0;
    virtual ouchi::result::result<rsize_t, error_code> inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) = 0;
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
    using block_cipher<A>::block_cipher;
    static constexpr bool parallel_encrypt = true;
    static constexpr bool parallel_decrypt = true;

    virtual ouchi::result::result<rsize_t, error_code> cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) override
    {
        auto actual_size = block_cipher<A>::pad(src, srcsize, dest, destsize, A::block_size);
        if (srcsize > RSIZE_MAX) return ouchi::result::err(error_code(error_value::invalid_arguments));
        if (actual_size == SIZE_MAX) return ouchi::result::err(error_code(error_value::too_short_buffer));

        for (rsize_t i = 0; i < actual_size; i += A::block_size) {
            block_cipher<A>::algorithm_.cipher((std::uint8_t*)dest + i);
        }
        return ouchi::result::ok(actual_size);
    }
    virtual ouchi::result::result<rsize_t, error_code> inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) override
    {
        if (srcsize > RSIZE_MAX && srcsize % A::block_size != 0) return ouchi::result::err(error_code(error_value::invalid_arguments));
        if (srcsize > destsize) return ouchi::result::err(error_code(error_value::too_short_buffer));
        std::memcpy(dest, src, srcsize);
        
        for (rsize_t i = 0; i < srcsize; i += A::block_size) {
            block_cipher<A>::algorithm_.inv_cipher((std::uint8_t*)dest + i);
        }
        auto pad_size = ((std::uint8_t*)dest)[srcsize - 1];
        for (size_t i = 0; i < pad_size; ++i) {
            if (((std::uint8_t*)dest)[srcsize - i - 1] != pad_size) return ouchi::result::err(error_code(error_value::corrupted_data));
        }
        return ouchi::result::ok(srcsize - pad_size);
    }

};

template<detail::block_cipher_algorithm A>
class cbc : public block_cipher<A> {
public:
    static constexpr bool parallel_encrypt = false;
    static constexpr bool parallel_decrypt = true;
    cbc(const std::uint8_t* initialization_vector,
        const std::uint8_t* key)
        : block_cipher<A>(key)
    {
        set_iv(initialization_vector);
    }
    void set_iv(const std::uint8_t* iv)
    {
        std::memcpy(&state_, iv, sizeof(state_));
    }
    
    virtual ouchi::result::result<rsize_t, error_code> cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) override
    {
        auto actual_size = block_cipher<A>::pad(src, srcsize, dest, destsize, A::block_size);
        if (srcsize > RSIZE_MAX) return ouchi::result::err(error_code(error_value::invalid_arguments));
        if (actual_size == SIZE_MAX) return ouchi::result::err(error_code(error_value::too_short_buffer));

        for (rsize_t i = 0; i < actual_size; i += A::block_size) {
            state_.xor_op(bytes<A::block_size>((std::uint8_t*)dest + i));
            block_cipher<A>::algorithm_.cipher(state_.byte_data);
            std::memcpy(((std::uint8_t*)dest + i), state_.byte_data, A::block_size);
        }
        return ouchi::result::ok(actual_size);
    }
    virtual ouchi::result::result<rsize_t, error_code> inv_cipher(const void* src, rsize_t srcsize, void* dest, rsize_t destsize) override
    {
        bytes<A::block_size> prev;
        if (srcsize > RSIZE_MAX && srcsize % A::block_size != 0) return ouchi::result::err(error_code(error_value::invalid_arguments));
        if (srcsize > destsize) return ouchi::result::err(error_code(error_value::too_short_buffer));
        std::memcpy(dest, src, srcsize);
        
        for (rsize_t i = 0; i < srcsize; i += A::block_size) {
            std::memcpy(prev.byte_data, (std::uint8_t*)dest + i, A::block_size);
            block_cipher<A>::algorithm_.inv_cipher((std::uint8_t*)dest + i);
            bytes<A::block_size>::xor_op((std::uint8_t*)dest + i, state_.byte_data);
            state_ = prev;
        }
        auto pad_size = ((std::uint8_t*)dest)[srcsize - 1];
        for (size_t i = 0; i < pad_size; ++i) {
            if (((std::uint8_t*)dest)[srcsize - i - 1] != pad_size) return ouchi::result::err(error_code(error_value::corrupted_data));
        }
        return ouchi::result::ok(srcsize - pad_size);
    }
private:
    bytes<A::block_size> state_;
};

template<detail::block_cipher_algorithm A>
class ctr;

}

