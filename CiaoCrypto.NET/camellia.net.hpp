#pragma once

#include "algorithm/camellia.hpp"
#include "block_cipher_algorithm.hpp"

using namespace System::Collections::Generic;
using namespace System;

namespace cn {

ref class camellia128 : public block_cipher_algorithm {
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

ref class camellia192 : public block_cipher_algorithm{
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

ref class camellia256 : public block_cipher_algorithm{
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

