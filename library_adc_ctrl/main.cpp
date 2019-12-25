#include <thread>

#include "ADC.h"
#include "log.h"

static void printData(uint8_t* data, uint32_t size){
    uint32_t num = size / 2;
    LOGI("Got adc: bytes:%u(adc_num:%u, samples:%u), data:\n", size, num, num / 4);
    for (uint32_t i=0; i<num; i++) {
        printf("%04x ", reinterpret_cast<uint16_t*>(data)[i]);
        if ((i+1)%10 == 0) printf("\n");
    }
    printf("\n");
}

int main() {
    LOGI("main...");

    adc::ADC adc(32);

    LOGI("同步模式...");
    auto res = adc.capture();
    printData(std::get<0>(res), std::get<1>(res));

    LOGI("异步模式...");
    adc.setDataHandle([](uint8_t* data, size_t size) {
        printData(data, size);
    });

    for(;;) {
        adc.captureAsync();
        std::this_thread::sleep_for(std::chrono::microseconds(1000 * 1000));
    }

    return 0;
}
