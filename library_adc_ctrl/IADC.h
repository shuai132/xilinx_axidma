#pragma once

#include <functional>
#include <future>
#include <tuple>

namespace adc {

using ADCNum_t = uint8_t;
static const ADCNum_t ADCNum = 4;

class IADC {
    using OnDataHandle = std::function<void(uint8_t* data, size_t size, int ch)>;

public:
    explicit IADC(uint32_t sampleNum = 1) : sampleNum_(sampleNum) {}

    virtual ~IADC() = default;

public:
    virtual std::future<std::tuple<uint8_t*, size_t>> captureAsync(ADCNum_t ch) = 0;

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
