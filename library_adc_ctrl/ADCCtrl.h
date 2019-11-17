#pragma once

#include <cstdint>

class ADCCtrl {
public:
    ADCCtrl();

    ~ADCCtrl();

public:
    void init(uint32_t sampleNum);

    void start();

private:
    const char* ADC_DMA_DEV = "/dev/adc_ctrl";

    int fd_ = -1;
};
