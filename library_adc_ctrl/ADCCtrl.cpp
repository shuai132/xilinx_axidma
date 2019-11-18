#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ADCCtrl.h"
#include "Exception.h"
#include "adc_ctrl_def.h"
#include "log.h"

namespace adc {

ADCCtrl::ADCCtrl() {
    fd_ = open(ADC_DMA_DEV, O_RDONLY);
    if (fd_ < 0) {
        LOGE("open %s failed: %s", ADC_DMA_DEV, strerror(errno));
        throw_exception();
    }
}

ADCCtrl::~ADCCtrl() {
    if (fd_) close(fd_);
}

void ADCCtrl::start(uint32_t sampleNum) {
    if (ioctl(fd_, ADC_SET_SAMPLE_NUM, sampleNum)) {
        LOGE("AXI_ADC_SET_SAMPLE_NUM failed: %s", strerror(errno));
        throw_exception();
    }

    if (ioctl(fd_, ADC_START) != 0) {
        LOGE("AXI_ADC_DMA_START failed: %s", strerror(errno));
        throw_exception();
    }
}

}
