#pragma once

#include "IADC.h"
#include "base/noncopyable.h"
#include "ADCCtrl.h"

extern "C" {
#include "libaxidma.h"
}

class ADC : app::noncopyable, public IADC {
public:
    ADC();

    ~ADC() override;

public:
    void capture(uint8_t*& data, size_t& size) override;

    void captureAsync() override;

private:
    static void axidma_callback(int channel_id, void *user_data);

    void initAdc(bool force = false);

    void freeRes();

    void errorIf(bool error, const std::string& errorMsg = "ADC error");

private:
    uint32_t SAMPLE_NUM = 32;
    uint32_t rxSize_ = SAMPLE_NUM * 4 * 2;

    int rxChannel_ = -1;
    axidma_dev_t axidmaDev_ = nullptr;
    uint8_t* rxBuf_ = nullptr;

    ADCCtrl adcCtrl_;

    bool initOk_ = false;
};
