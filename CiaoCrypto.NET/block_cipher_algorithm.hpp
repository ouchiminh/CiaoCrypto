#pragma once

using namespace System;

namespace cn {

ref class block_cipher_algorithm abstract {
public:

    virtual void cipher(Byte* dptr) = 0;
    virtual void inv_cipher(Byte* dptr) = 0;
    virtual size_t block_size() = 0;
};

}

