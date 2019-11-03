include_directories(${xilinx_axidma_KERNEL_DIR}/include)

add_custom_target(dirver
        COMMAND cd ${CMAKE_CURRENT_LIST_DIR}/.. && make driver && cp ${CMAKE_CURRENT_LIST_DIR}/*.ko ${CMAKE_BINARY_DIR}
        )

# 为了IDE自动提示 添加一个target
include_directories(../include)
add_library(ignore_dirver
        axidma_chrdev.c
        axidma_dma.c
        axidma_of.c
        axi_dma.c
        )
