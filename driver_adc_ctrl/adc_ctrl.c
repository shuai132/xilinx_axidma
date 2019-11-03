#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include "adc_ctrl.h"

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("liushuai");

#define DEVICE_NAME "adc_ctrl"

static int on_open(struct inode *inode, struct file *file) {
    LOGI();
    return 0;
}

static int on_close(struct inode *inode, struct file *file) {
    LOGI();
    return 0;
}

static long on_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    LOGI("cmd: %u", cmd);
    return -EMSGSIZE;
}

static struct file_operations adc_ctrl_fops = {
        .owner          = THIS_MODULE,
        .open           = on_open,
        .release        = on_close,
        .unlocked_ioctl = on_ioctl,
};

static struct miscdevice adc_ctrl_dev = {
        .minor  = MISC_DYNAMIC_MINOR,
        .name   = DEVICE_NAME,
        .fops   = &adc_ctrl_fops,
};

static int adc_ctrl_init(void) {
    int ret = 0;
    LOGI();
    ret = misc_register(&adc_ctrl_dev);
    return ret;
}

static void adc_ctrl_exit(void) {
    misc_deregister(&adc_ctrl_dev);
    LOGI();
}

module_init(adc_ctrl_init);
module_exit(adc_ctrl_exit);

