set(TARGET_NAME adc_lib)

add_library(${TARGET_NAME} STATIC
        ADC.cpp
        ADCCtrl.cpp
        )
target_include_directories(${TARGET_NAME} PUBLIC ../driver_adc_ctrl)

add_executable(example_adc_ctrl example.cpp)
target_link_libraries(example_adc_ctrl
        adc_lib
        )
