#pragma once
#include "primality.hpp"

namespace cn {

public ref class bigint {
public:
    bigint()
        : value_{ new boost::multiprecision::cpp_int() }
    {}
    explicit bigint(const boost::multiprecision::cpp_int& v)
        : value_{ new boost::multiprecision::cpp_int(v) }
    {}
    void import_bit(cli::array<const System::Byte>^ bits)
    {
        using namespace System;
        auto s = bits->Length;
        for (int i = 0; i < s; ++i) {
            (*value_) <<= 8;
            (*value_) |= (unsigned char)(unsigned short)bits[i];
        }
    }
    void export_bit(cli::array<System::Byte>^ bits)
    {
        using namespace System;
        
        auto s = bits->Length;
        for (int i = 0; i < s; ++i) {
            bits[i] = (std::uint8_t)(*value_ >> ((s - i - 1) * 8));
        }
    }
    boost::multiprecision::cpp_int& value()
    {
        return *value_;
    }
private:
    boost::multiprecision::cpp_int* value_;
};

}

