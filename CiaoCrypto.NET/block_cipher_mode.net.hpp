#pragma once
#include <cstdint>
#include <cstring>
#include <limits>
#include "block_cipher_algorithm.hpp"

using namespace System;

namespace cn {

namespace detail {
inline rsize_t pad(const void* src, size_t srcsize, void* dest, size_t destsize, std::uint8_t width) noexcept
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
}

public enum error_code : int64_t {
    too_short_dest = std::numeric_limits<int64_t>::min(),
    too_big_data, corrupted_data
};

public ref class block_cipher abstract {
public:
    virtual std::int64_t cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                                cli::array<Byte>^ dest, std::size_t destsize) = 0;
    virtual std::int64_t inv_cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                                    cli::array<Byte>^ dest, std::size_t destsize) = 0;
};

generic<class A>
    where A : block_cipher_algorithm
public ref class ecb : public block_cipher {
public:
    ecb(A encoder)
        : encoder_{encoder}
    {}
    std::int64_t cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                        cli::array<Byte>^ dest, std::size_t destsize) override
    {
        if (srcsize > RSIZE_MAX) return too_big_data;
        if (destsize < (srcsize / encoder_->block_size() + 1) * encoder_->block_size()) return too_short_dest;
        cli::pin_ptr<const Byte> sptr = (&src[0]);
        cli::pin_ptr<Byte> dptr = (&dest[0]);
        auto actualsize = detail::pad(sptr, srcsize, dptr, destsize, (std::uint8_t)encoder_->block_size());
        Byte* end = dptr + actualsize;
        while (dptr != end) {
            encoder_->cipher(dptr);
            dptr += encoder_->block_size();
        }
        return actualsize;
    }
    std::int64_t inv_cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                            cli::array<Byte>^ dest, std::size_t destsize) override
    {
        if (srcsize > RSIZE_MAX) return too_big_data;
        if (destsize < srcsize) return too_short_dest;
        if (srcsize % encoder_->block_size() != 0) corrupted_data;
        cli::pin_ptr<const Byte> sptr = (&src[0]);
        cli::pin_ptr<Byte> dptr = (&dest[0]);
        Byte* const end = dptr + srcsize;
        std::memcpy(dptr, sptr, srcsize);
        while (dptr != end) {
            encoder_->inv_cipher(dptr);
            dptr += encoder_->block_size();
        }
        Byte padded = *--dptr;
        for (dptr = end - padded; dptr != end; ++dptr)
            if (*dptr != padded) return corrupted_data;

        return srcsize - padded;
    }
private:
    A encoder_;
};

// ブロックサイズが16 byteのブロック暗号にしか対応していません。
generic<class A>
    where A : block_cipher_algorithm
public ref class ctr : public block_cipher {
public:
    ctr(A encoder, cli::array<const Byte>^ nonce)
        : ctr{ encoder, nonce, 0 }
    {}
    ctr(A encoder, cli::array<const Byte>^ nonce, size_t cnt)
        : encoder_{ encoder }
        , state_{ new std::uint64_t[2] }
    {
        cli::pin_ptr<const Byte> pnonce = &nonce[0];
        std::memcpy(state_, pnonce, 8);
        state_[1] = cnt;
        assert(encoder_->block_size() == 16);
    }

    std::int64_t cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                        cli::array<Byte>^ dest, std::size_t destsize) override
    {
        if (srcsize > RSIZE_MAX) return too_big_data;
        if (destsize < srcsize) return too_short_dest;

        cli::pin_ptr<const Byte> sptr = (&src[0]);
        cli::pin_ptr<Byte> dptr = (&dest[0]);
        auto end = dptr + (srcsize / 16) * 16;
        auto fin = sptr + srcsize;
        std::memmove(dptr, sptr, srcsize);

        std::uint64_t data[2] = { state_[0] };
        for (; dptr < end; dptr += 16) {
            ciao::unpack(state_[1]++, &data[1]);
            encoder_->cipher((std::uint8_t*)data);
            exclusive_or(dptr, (std::uint8_t*)data);
        }
        ciao::unpack(state_[1]++, &data[1]);
        encoder_->cipher((std::uint8_t*)data);
        for (auto* s = (std::uint8_t*)data; dptr < fin; ++dptr, ++s) {
            *dptr ^= *s;
        }
        return srcsize;
    }
    std::int64_t inv_cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                            cli::array<Byte>^ dest, std::size_t destsize) override
    {
        return cipher(src, srcsize, dest, destsize);
    }
private:
    static void exclusive_or(std::uint8_t* srcdest, const std::uint8_t* src)
    {
        std::uint64_t* pd = (std::uint64_t*)srcdest;
        const std::uint64_t* ps = (std::uint64_t*)src;
        pd[0] ^= ps[0];
        pd[1] ^= ps[1];
    }
    A encoder_;
    std::uint64_t *state_;
};

}

