#pragma once

#include "algorithm/camellia.hpp"
#include "block_cipher_algorithm.hpp"
#include "block_cipher_mode.net.hpp"
#include "block_cipher_mode.hpp"

using namespace System::Collections::Generic;
using namespace System;

namespace cn {

public ref class camellia128ctr : public block_cipher {
public:
    camellia128ctr(cli::array<const Byte>^ key, cli::array<const Byte>^ nonce)
        : camellia128ctr(key, nonce, 0)
    {}
    camellia128ctr(cli::array<const Byte>^ key, cli::array<const Byte>^ nonce, size_t cnt)
    {
        cli::pin_ptr<const Byte> kp = &key[0];
        cli::pin_ptr<const Byte> np = &nonce[0];
        encoder_ = new ciao::ctr<ciao::camellia<16>>(np, cnt, kp);
    }
    
    std::int64_t cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                        cli::array<Byte>^ dest, std::size_t destsize) override
    {
        cli::pin_ptr<const Byte> sp = &src[0];
        cli::pin_ptr<Byte> dp = &dest[0];

        if (auto r = encoder_->cipher(sp, srcsize, dp, destsize); r.is_ok()) {
            return r.unwrap();
        } else return (std::uint64_t)r.unwrap_err().value();
    }
    std::int64_t inv_cipher(cli::array<const Byte>^ src, std::size_t srcsize,
                            cli::array<Byte>^ dest, std::size_t destsize) override
    {
        return cipher(src, srcsize, dest, destsize);
    }
private:
    ciao::ctr<ciao::camellia<16>>* encoder_;
};

public ref class camellia128 : public block_cipher_algorithm {
    ciao::camellia<16>* encoder_;
public:
    camellia128(cli::array<const Byte>^ key)
        : encoder_{new ciao::camellia<16>()}
    {
        cli::pin_ptr<const Byte> keyptr = &key[0];
        encoder_->key_schedule(keyptr);
    }
    ~camellia128()
    {
        delete encoder_;
    }

    void cipher(Byte* dptr) override
    {
        encoder_->cipher(dptr);
    }
    void inv_cipher(Byte* dptr) override
    {
        encoder_->inv_cipher(dptr);
    }

    size_t block_size() override
    {
        return ciao::camellia<16>::block_size;
    }
};

public ref class camellia192 : public block_cipher_algorithm{
    ciao::camellia<24>* encoder_;
public:
    camellia192(cli::array<const Byte>^ key)
        : encoder_{new ciao::camellia<24>()}
    {
        cli::pin_ptr<const Byte> keyptr = &key[0];
        encoder_->key_schedule(keyptr);
    }
    ~camellia192()
    {
        delete encoder_;
    }

    void cipher(Byte* dptr) override
    {
        encoder_->cipher(dptr);
    }
    void inv_cipher(Byte* dptr) override
    {
        encoder_->inv_cipher(dptr);
    }


    size_t block_size() override
    {
        return ciao::camellia<24>::block_size;
    }
};

public ref class camellia256 : public block_cipher_algorithm{
    ciao::camellia<32>* encoder_;
public:
    camellia256(cli::array<const Byte>^ key)
        : encoder_{new ciao::camellia<32>()}
    {
        cli::pin_ptr<const Byte> keyptr = &key[0];
        encoder_->key_schedule(keyptr);
    }
    ~camellia256()
    {
        delete encoder_;
    }

    void cipher(Byte* dptr) override
    {
        encoder_->cipher(dptr);
    }
    void inv_cipher(Byte* dptr) override
    {
        encoder_->inv_cipher(dptr);
    }

    size_t block_size() override
    {
        return ciao::camellia<32>::block_size;
    }
};
}

