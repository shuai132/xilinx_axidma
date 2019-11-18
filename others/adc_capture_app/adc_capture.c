#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "adc_capture.h"

/* Thread capture ADC samples and put it to Circular buffer */
int adc_capture_init(int *fd, char *adc_dev, int adc_sample_num, int dma_len_bytes) {
    adc_sample_num *= 2;
    *fd = open(adc_dev, O_RDONLY);
    if (*fd < 0) {
        printf("open %s failed: %s\n", adc_dev, strerror(errno));
        return -1;
    }

    if (ioctl(*fd, AXI_ADC_SET_SAMPLE_NUM, adc_sample_num)) {
        printf("AXI_ADC_SET_SAMPLE_NUM failed: %s\n", strerror(errno));
        return -2;
    }

    if (ioctl(*fd, AXI_ADC_SET_DMA_LEN_BYTES, dma_len_bytes)) {
        printf("AXI_ADC_SET_DMA_LEN_BYTES failed: %s\n", strerror(errno));
        return -2;
    }

    if (ioctl(*fd, AXI_ADC_DMA_INIT)) {
        printf("AXI_ADC_DMA_INIT failed: %s\n", strerror(errno));
        return -2;
    }

    return 0;
}

int adc_capture_start(int fd, uint32_t samples_num, uint8_t *buf, uint32_t *buf_len) {
    if (ioctl(fd, AXI_ADC_DMA_START) == 0) {
        ioctl(fd, AXI_ADC_ADC_START);
        samples_num *= 2;
        *buf_len = samples_num * 4; // 四个通道
        read(fd, buf, *buf_len);
    }
    else {
        printf("AXI_ADC_DMA_START failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
