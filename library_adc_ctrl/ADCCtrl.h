/**
 * 控制ADC采样的开始、采样数等
 * 具体实现与FPGA有关，当前的实现是：
 * 1. 为了同步采样，adc0是master，它将把一些信号传递给其他adc，也就是说，adc0这个设备要接入。
 * 2. 每个adc都要各自设置，从设备的start命令无效，全由master设备控制。
 */
#pragma once

#include <cstdint>

namespace adc {

struct Dev {
    static const char* MASTER;
    static const char* SLAVER1;
    static const char* SLAVER2;
    static const char* SLAVER3;
};

class ADCCtrl {
public:
    explicit ADCCtrl(const char* adcDev);

    ~ADCCtrl();

public:
    void start(uint32_t sampleNum);

private:
    const char* adcDev_;

    int fd_ = -1;
};

}
