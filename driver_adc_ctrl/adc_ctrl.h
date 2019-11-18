#ifndef ADC_CTRL_H_
#define ADC_CTRL_H_

#define MODULE_NAME                     "adc_ctrl"

#define ADC_MINOR_START                 0
#define ADC_MINOR_COUNT                 16
#define SUCCESS                         0
#define FAILURE                         -1

struct axi_adc_dev
{
    struct mutex mutex;
    struct platform_device *pdev;
    /* ADC Hardware device constants */
    void *adc_virtaddr;
    uint32_t adc_sample_num;
};

#endif
