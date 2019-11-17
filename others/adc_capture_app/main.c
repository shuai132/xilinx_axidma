#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "adc_capture.h"

#define ADC_SAMPLE_NUM      10                          // adc采样次数
#define ADC_DATA_BYTES      (ADC_SAMPLE_NUM * 2 * 4)    // 四个通道 每个通道2字节

#define ZERO_NUM            10                          // FOR TEST
uint8_t RxBuffer[ADC_DATA_BYTES + ZERO_NUM];

int main(int argc, char *argv[])
{
    int fd0;

    int ret = adc_capture_init(&fd0, "/dev/adc0", ADC_SAMPLE_NUM, ADC_DATA_BYTES);
    if (ret != 0) {
        printf("open adc failed!\n");
        return ret;
    }

    for (;;) {
        uint32_t size = 0;
        if(adc_capture_start(fd0, ADC_SAMPLE_NUM, RxBuffer, &size)) {
            exit(1);
        }

        const uint16_t *data = (uint16_t*)RxBuffer;
        printf("Got adc: len:%u, data:\n", size);
        for (int i=0; i<size / 2 + ZERO_NUM; i++) {
            printf("%04x ", data[i]);
            if ((i+1)%10 == 0) printf("\n");
        }
        printf("\n");

        usleep(1000 * 100);
    }
}
