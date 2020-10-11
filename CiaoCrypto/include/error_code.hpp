#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>

namespace ciao {

enum class error_value : std::int32_t {
    normal = 0,
    too_short_buffer = 1, invalid_arguments,
    corrupted_data,
    internal_error = (int)0x80000000
};

class error_code {
    error_value value_;
    std::stringstream message_;

public:
    error_code()
        : value_{error_value::normal}
        , message_{}
    {}
    error_code(error_value v, std::string mes)
        : value_{v}
        , message_{mes}
    {}
    explicit error_code(error_value v)
        : value_{v}
        , message_{}
    {}
    error_value value() const noexcept { return value_; }
    std::string message() const { return message_.str(); }
    std::stringstream& message_stream() noexcept { return message_; }
};

}

