#pragma once
#include "algorithm/rsa.hpp"
#include "primality.hpp"
#include "bigint.hpp"

namespace cn {

public ref class rsa_key {
public:
    rsa_key() {}
    bigint^ decrypt_key;
    bigint^ encrypt_key;
    bigint^ n;
    ciao::rsa_key<boost::multiprecision::cpp_int> raw()
    {
        ciao::rsa_key<boost::multiprecision::cpp_int> rawkey;
        rawkey.decrypt_key = this->decrypt_key->value();
        rawkey.encrypt_key = this->encrypt_key->value();
        rawkey.n = this->n->value();
        return rawkey;
    }
};

public ref class rsa {
public:
    rsa(rsa_key^ k)
        :rsa_{ new ciao::rsa<boost::multiprecision::cpp_int>(k->raw()) }
    {}

    bigint^ encrypt(bigint data)
    {
        return gcnew bigint(rsa_->encrypt(data.value()));
    }
    bigint^ decrypt(bigint data)
    {
        return gcnew bigint(rsa_->decrypt(data.value()));
    }
    bigint^ sign(bigint data)
    {
        return gcnew bigint(rsa_->sign(data.value()));
    }
    bool verify(bigint signed_message, bigint raw_message)
    {
        return rsa_->verify(signed_message.value(), raw_message.value());
    }


    static rsa_key^ rsa_keygen(int bitsize)
    {
        auto rawkey = ciao::rsa_key_gen(bitsize);
        rsa_key^ k = gcnew rsa_key();
        k->decrypt_key = gcnew bigint(rawkey.decrypt_key);
        k->encrypt_key = gcnew bigint(rawkey.encrypt_key);
        k->n = gcnew bigint(rawkey.n);
        return k;
    }
private:
    ciao::rsa<boost::multiprecision::cpp_int>* rsa_;
};



}

