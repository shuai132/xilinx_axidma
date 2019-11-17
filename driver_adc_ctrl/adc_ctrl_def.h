#ifndef ADC_CTRL_DEF_H
#define ADC_CTRL_DEF_H

/* IOCTL defines */
#define AXI_ADC_IOCTL_BASE              'W'
#define AXI_ADC_SET_SAMPLE_NUM          _IO(AXI_ADC_IOCTL_BASE, 0)
#define AXI_ADC_ADC_START               _IO(AXI_ADC_IOCTL_BASE, 1)

#endif
