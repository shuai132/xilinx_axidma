#pragma once

#include <functional>

class IADC {
    using OnDataHandle = std::function<void(uint8_t* data, size_t size)>;

public:
    virtual ~IADC() = default;

public:
    virtual void capture(uint8_t*& data, size_t& size) = 0;

    virtual void captureAsync() = 0;

    void setDataHandle(const OnDataHandle& handle) {
        onDataHandle_ = handle;
    }

    const OnDataHandle& getDataHandle() {
        return onDataHandle_;
    }

protected:
    OnDataHandle onDataHandle_;
};
