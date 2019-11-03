cmake_minimum_required(VERSION 2.8)

# driver: if ON, you should set kernel dir, eg: set(xilinx_axidma_KERNEL_DIR "")
option(xilinx_axidma_BUILD_DRIVER   "build driver"  ON)
# example
option(xilinx_axidma_BUILD_EXAMPLE  "build example" ON)
# cross
option(xilinx_axidma_CROSS_ARM      "cross compile" ON)
# library
option(xilinx_axidma_STATIC_BUILD   "library type"  OFF)

# cross config
if(xilinx_axidma_CROSS_ARM)
    set(CMAKE_SYSTEM_NAME       Linux)
    set(CMAKE_SYSTEM_PROCESSOR  arm)
endif()

project(xilinx_axidma)

# driver
if(xilinx_axidma_BUILD_DRIVER)
    add_subdirectory(driver)
    add_subdirectory(driver_adc_ctrl)
endif()

# library
if(xilinx_axidma_STATIC_BUILD)
    add_library(axidma STATIC library/libaxidma.c)
else()
    add_library(axidma SHARED library/libaxidma.c)
endif()
target_include_directories(axidma PUBLIC include)

# example
if(xilinx_axidma_BUILD_EXAMPLE)
    set(UTILS ./examples/util.c)
    add_executable(axidma_benchmark     ./examples/axidma_benchmark.c       ${UTILS})
    add_executable(axidma_display_image ./examples/axidma_display_image.c   ${UTILS})
    add_executable(axidma_transfer      ./examples/axidma_transfer.c        ${UTILS})

    target_link_libraries(axidma_benchmark      axidma)
    target_link_libraries(axidma_display_image  axidma)
    target_link_libraries(axidma_transfer       axidma)
endif()
