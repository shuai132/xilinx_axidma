#pragma once

#include <stdexcept>
#include <functional>

namespace adc {

const std::string DEFAULT_ERROR_MSG = "adc error"; // NOLINT(cert-err58-cpp)

class Exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

template<typename T>
inline void throw_if(T b, const std::string& msg = DEFAULT_ERROR_MSG) {
    if (b) {
        throw Exception(msg);
    }
}

template<typename T>
inline void throw_if(T b, const std::function<void()>& cb, const std::string& msg = DEFAULT_ERROR_MSG) {
    if (b) {
        if (cb) {
            cb();
        }
        throw Exception(msg);
    }
}

#define throw_exception() throw_if(true)

}
