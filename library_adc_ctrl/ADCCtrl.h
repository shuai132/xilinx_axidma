#pragma once

#include <cstdint>

class ADCCtrl {
public:
    ADCCtrl();

    ~ADCCtrl();

public:
    void start(uint32_t sampleNum);

private:
    const char* ADC_DMA_DEV = "/dev/adc0";

    int fd_ = -1;
};
