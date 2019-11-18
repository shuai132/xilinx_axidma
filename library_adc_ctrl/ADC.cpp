/**
 * 对于硬件来讲 会有较多的异常
 */
#include "ADC.h"
#include "ADCCtrl.h"
#include "Exception.h"
#include "log.h"

namespace adc {

ADC::ADC() {
    initAdc();
}

void ADC::capture(uint8_t*& data, size_t& size) {
    LOGD("capture...");
    initAdc();
    adcCtrl_.start(SAMPLE_NUM);
    int rc = axidma_oneway_transfer(axidmaDev_, rxChannel_, rxBuf_, rxSize_, true);

    errorIf(rc < 0);
    LOGD("capture data: size:%u", rxSize_);

    data = rxBuf_;
    size = rxSize_;
}

void ADC::captureAsync() {
    LOGD("captureAsync...");
    initAdc();
    adcCtrl_.start(SAMPLE_NUM);
    int rc = axidma_oneway_transfer(axidmaDev_, rxChannel_, rxBuf_, rxSize_, false);
    errorIf(rc < 0);
}

ADC::~ADC() {
    axidma_stop_transfer(axidmaDev_, rxChannel_);
    freeRes();
}

void ADC::initAdc(bool force) {
    if (not force and initOk_) return;

    freeRes();

    axidmaDev_ = axidma_init();
    errorIf(axidmaDev_ == nullptr, "Failed to initialize the AXI DMA device");

    rxBuf_ = reinterpret_cast<uint8_t*>(axidma_malloc(axidmaDev_, rxSize_));
    errorIf(rxBuf_ == nullptr, "Unable to allocate receive buffer from the AXI DMA device");

    const array_t *rxChans = axidma_get_dma_rx(axidmaDev_);
    LOGD("rxChans->len=%d", rxChans->len);
    errorIf(rxChans->len < 1, "Error: No receive channels were found");

    rxChannel_ = rxChans->data[0];
    LOGD("Using receive channel %d", rxChannel_);

    axidma_set_callback(axidmaDev_, rxChannel_, axidma_callback, this);

    initOk_ = true;
}

void ADC::freeRes() {
    if (rxBuf_) {
        axidma_free(axidmaDev_, rxBuf_, rxSize_);
    }

    if (axidmaDev_) {
        axidma_destroy(axidmaDev_);
    }

    initOk_ = false;
}

void ADC::errorIf(bool error, const std::string& errorMsg) {
    if (error) LOGE("%s", errorMsg.c_str());
    throw_if(error, std::bind(&ADC::freeRes, this));
}

void ADC::axidma_callback(int channel_id, void *user_data) {
    LOGD("axidma_callback: channel_id:%d, user_data:%p", channel_id, user_data);
    auto adc = reinterpret_cast<ADC*>(user_data);

    auto& handle = adc->onDataHandle_;
    if (handle) {
        handle(adc->rxBuf_, adc->rxSize_);
    }
}

}
