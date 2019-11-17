include_directories(${xilinx_axidma_KERNEL_DIR}/include)

add_custom_target(dirver_adc_capture
        COMMAND cd ${CMAKE_CURRENT_LIST_DIR} && make && cp *.ko ${CMAKE_BINARY_DIR}
        )

# 为了IDE自动提示 添加一个target
include_directories(../include)
add_library(ignore_dirver_adc_capture
        axi_adc_dma.c
        )
