#pragma once

#include "IADC.h"
#include "base/noncopyable.h"
#include "ADCCtrl.h"

extern "C" {
#include "libaxidma.h"
}

namespace adc {

class ADC : noncopyable, public IADC {
public:
    explicit ADC(uint32_t sampleNum = 1);

    ~ADC() override;

public:
    void capture(uint8_t*& data, size_t& size) override;

    void captureAsync() override;

    void setSampleNum(uint32_t sampleNum) override;

private:
    static void axidma_callback(int channel_id, void* user_data);

    void initAdc(bool force = false);

    void freeRes();

    void errorIf(bool error, const std::string& errorMsg = "ADC error");

    void updateRxSize();

private:
    uint32_t rxSize_ = 0;

    int rxChannel_ = -1;
    axidma_dev_t axidmaDev_ = nullptr;
    uint8_t* rxBuf_ = nullptr;

    ADCCtrl adcCtrl_;

    bool initOk_ = false;
};

}
