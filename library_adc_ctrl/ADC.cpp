/**
 * 对于硬件来讲 会有较多的异常
 */
#include <cassert>

#include "ADC.h"
#include "ADCCtrl.h"
#include "Exception.h"
#include "log.h"

#define FOR(i, n)   for(std::remove_reference<std::remove_const<typeof(n)>::type>::type i = 0; i < n; i++)

namespace adc {

ADC::ADC(uint32_t sampleNum)
    : IADC(sampleNum)
    , adcCtrl0_(Dev::MASTER)
    , adcCtrl1_(Dev::SLAVER1)
    , adcCtrl2_(Dev::SLAVER2)
    , adcCtrl3_(Dev::SLAVER3)
    {
    adcCtrl_[0] = &adcCtrl0_;
    adcCtrl_[1] = &adcCtrl1_;
    adcCtrl_[2] = &adcCtrl2_;
    adcCtrl_[3] = &adcCtrl3_;
    initAdc();
}

std::future<std::tuple<uint8_t*, size_t>> ADC::captureAsync(ADCNum_t adcNum) {
    assert(sampleNum_ != 0);
    assert(0 <= adcNum && adcNum < ADCNum);

    LOGD("captureAsync...");
    initAdc();

    auto userData = new UserData;
    userData->adc = this;
    userData->ch = adcNum;
    userData_[adcNum].reset(userData);
    LOGD("set cb: %p", userData);
    axidma_set_callback(axidmaDev_, adcNum, axidma_callback, userData);

    adcCtrl_[adcNum]->start(sampleNum_);
    int rc = axidma_oneway_transfer(axidmaDev_, adcNum, rxBuf_[adcNum], rxSize_, false);
    errorIf(rc < 0);

    return userData->promise.get_future();
}

ADC::~ADC() {
    if (axidmaDev_) {
        FOR(i, ADCNum) {
            axidma_stop_transfer(axidmaDev_, i);
        }
    }
    freeRes();
}

void ADC::initAdc(bool force) {
    if (not force and initOk_) return;

    freeRes();

    axidmaDev_ = axidma_init();
    errorIf(axidmaDev_ == nullptr, "Failed to initialize the AXI DMA device");

    updateRxSize();

    const array_t *rxChans = axidma_get_dma_rx(axidmaDev_);
    LOGD("rxChans->len=%d", rxChans->len);
    FOR(i, rxChans->len) {
        LOGD("rxChans[%d]: %d", i, rxChans->data[i]);
    }
    errorIf(rxChans->len < 4, "Error: No receive channels were found");

    FOR(i, ADCNum) {
        auto rxBuf = reinterpret_cast<uint8_t*>(axidma_malloc(axidmaDev_, rxSize_));
        errorIf(rxBuf == nullptr, "Unable to allocate receive buffer from the AXI DMA device");
        rxBuf_[i] = rxBuf;
        LOGD("rxBuf_[%d]:%p", i, rxBuf);
    }

    initOk_ = true;
}

void ADC::freeRes() {
    for(const auto& buf : rxBuf_) {
        if (buf) {
            axidma_free(axidmaDev_, buf, rxSize_);
        }
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

void ADC::axidma_callback(int channel_id, void* user_data) {
    LOGD("axidma_callback: ch:%d, user_data:%p", channel_id, user_data);
    auto userData = reinterpret_cast<UserData*>(user_data);

    const auto& ch = userData->ch;
    auto* adc = userData->adc;
    auto& handle = adc->onDataHandle_;
    auto data = adc->rxBuf_[ch];
    auto size = adc->rxSize_;

    assert(userData == adc->userData_[ch].get());

    if (handle) {
        handle(data, size, ch);
    }
    userData->promise.set_value(std::make_tuple(data, size));
}

void ADC::setSampleNum(uint32_t sampleNum) {
    IADC::setSampleNum(sampleNum);
    updateRxSize();
    initAdc(true);
}

void ADC::updateRxSize() {
    // 每个ADC有4个通道 每个通道2字节
    rxSize_ = sampleNum_ * 4 * 2;
}

}
