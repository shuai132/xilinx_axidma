#pragma once

#include "IADC.h"
#include "base/noncopyable.h"
#include "ADCCtrl.h"

extern "C" {
#include "libaxidma.h"
}

namespace adc {

class ADC : noncopyable, public IADC {
    struct UserData {
        ADC* adc = nullptr;
        int ch = -1;
        std::promise<std::tuple<uint8_t*, size_t>> promise;
    };

public:
    explicit ADC(uint32_t sampleNum = 1);

    ~ADC() override;

public:
    std::future<std::tuple<uint8_t*, size_t>> captureAsync(ADCNum_t adcNum) override ;

    void setSampleNum(uint32_t sampleNum) override;

private:
    static void axidma_callback(int channel_id, void* user_data);

    void initAdc(bool force = false);

    void freeRes();

    void errorIf(bool error, const std::string& errorMsg = "ADC error");

    void updateRxSize();

private:
    uint32_t rxSize_ = 0;

    axidma_dev_t axidmaDev_ = nullptr;  // 唯一
    uint8_t* rxBuf_[4] = {};

    ADCCtrl adcCtrl0_;
    ADCCtrl adcCtrl1_;
    ADCCtrl adcCtrl2_;
    ADCCtrl adcCtrl3_;
    ADCCtrl* adcCtrl_[ADCNum]{};

    bool initOk_ = false;

    std::unique_ptr<UserData> userData_[ADCNum]; // 只用于防止内存泄漏 不直接使用
};

}
