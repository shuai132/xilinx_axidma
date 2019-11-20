#pragma once

#include <functional>

namespace adc {

class IADC {
    using OnDataHandle = std::function<void(uint8_t* data, size_t size)>;

public:
    explicit IADC(uint32_t sampleNum = 1) : sampleNum_(sampleNum) {}

    virtual ~IADC() = default;

public:
    virtual void capture(uint8_t*& data, size_t& size) = 0;

    virtual void captureAsync() = 0;

    void setDataHandle(const OnDataHandle& handle) {
        onDataHandle_ = handle;
    }

    virtual void setSampleNum(uint32_t sampleNum) {
        sampleNum_ = sampleNum;
    }

protected:
    OnDataHandle onDataHandle_;

    uint32_t sampleNum_;
};

}
