#ifndef ADC_CTRL_DEF_H
#define ADC_CTRL_DEF_H

/* IOCTL defines */
#define ADC_IOCTL_BASE              'W'
#define ADC_SET_SAMPLE_NUM          _IO(ADC_IOCTL_BASE, 0)
#define ADC_START                   _IO(ADC_IOCTL_BASE, 1)

#endif
