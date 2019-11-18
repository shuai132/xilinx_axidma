#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "adc_ctrl.h"
#include "adc_ctrl_def.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liushuai");
MODULE_DESCRIPTION("adc ctrl driver for set adc registers");

/*ADC channel name */
static const char adc_channels[][20] =
        {
                {"adc0"},
                {"adc1"},
                {"adc2"},
                {"adc3"},
                {"adc4"},
                {"adc5"},
                {"adc6"},
                {"adc7"},
                {"adc8"},
                {"adc9"},
                {"adc10"},
                {"adc11"},
                {"adc12"},
                {"adc13"},
                {"adc14"},
                {"adc15"},
        };

static struct axi_adc_dev *axi_adc_dev[16];
static int dev_index = 0;
static dev_t devno;
static struct cdev adc_cdev;
static struct class *axi_adc_class;

int axi_adc_dma_open(struct inode *inode, struct file *filp)
{
    unsigned int mn;
    mn = iminor(inode);
    /*Assign minor number for later reference */
    filp->private_data = (void *) mn;
    return SUCCESS;
}

int axi_adc_dma_release(struct inode *inode, struct file *filp)
{
    return SUCCESS;
}

static long axi_adc_dma_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int minor = (int) file->private_data;
    switch (cmd)
    {
        case ADC_SET_SAMPLE_NUM:
            writel(arg, axi_adc_dev[minor]->adc_virtaddr + 4);
            break;
        case ADC_START:
            writel(1, axi_adc_dev[minor]->adc_virtaddr);
            break;
        default:
            return -EOPNOTSUPP;
    }
    return SUCCESS;
}

struct file_operations axi_adc_fops =
        {
                .owner = THIS_MODULE,
                .open = axi_adc_dma_open,
                .unlocked_ioctl = axi_adc_dma_ioctl,
                .release = axi_adc_dma_release
        };

static int axi_adc_remove(struct platform_device *pdev)
{
    int i;
    for(i = 0;i < dev_index;i++)
    {
        device_destroy(axi_adc_class,MKDEV(MAJOR(devno), i));

        /* Unmap the adc I/O memory */
        if (axi_adc_dev[i]->adc_virtaddr)
            iounmap(axi_adc_dev[i]->adc_virtaddr);

        if (axi_adc_dev[i])
        {
            kfree(axi_adc_dev[i]);
        }
        dev_info(&pdev->dev, "adc DMA Unload :: Success\n");
    }
    class_destroy(axi_adc_class);
    cdev_del(&adc_cdev);
    unregister_chrdev_region(devno, ADC_MINOR_COUNT);
    return SUCCESS;
}

static int axi_adc_probe(struct platform_device *pdev)
{
    int status = SUCCESS;

    struct device_node *node = NULL;

    /*Allocate device node */
    node = pdev->dev.of_node;

    /* Allocate a private structure to manage this device */
    axi_adc_dev[dev_index] = kmalloc(sizeof(struct axi_adc_dev), GFP_KERNEL);
    if (axi_adc_dev[dev_index] == NULL)
    {
        dev_err(&pdev->dev, "unable to allocate device structure\n");
        return -ENOMEM;
    }
    memset(axi_adc_dev[dev_index], 0, sizeof(struct axi_adc_dev));

    /* IOMAP adc registers */
    axi_adc_dev[dev_index]->adc_virtaddr = of_iomap(node, 0);
    if (!axi_adc_dev[dev_index]->adc_virtaddr)
    {
        dev_err(&pdev->dev, "unable to IOMAP adc registers\n");
        status = -ENOMEM;
        goto fail1;
    }

    axi_adc_dev[dev_index]->pdev = pdev;
    mutex_init(&axi_adc_dev[dev_index]->mutex);

    if(dev_index == 0)
    {
        status =alloc_chrdev_region(&devno, 0, ADC_MINOR_COUNT, MODULE_NAME);
        if (status < 0)
        {
            dev_err(&pdev->dev, "unable to alloc chrdev\n");
            goto fail2;
        }

        /* Register with the kernel as a character device */
        cdev_init(&adc_cdev, &axi_adc_fops);
        adc_cdev.owner = THIS_MODULE;
        adc_cdev.ops = &axi_adc_fops;
        status = cdev_add(&adc_cdev, devno, ADC_MINOR_COUNT);
        axi_adc_class = class_create(THIS_MODULE, MODULE_NAME);
    }

    // Creating device node for each ADC channel
    device_create(axi_adc_class, NULL,
                  MKDEV(MAJOR(devno), dev_index),
                  NULL, adc_channels[dev_index]);


    dev_info(&pdev->dev, "PL adc added successfully\n");
    dev_index++;
    return SUCCESS;

    fail2:
    iounmap(axi_adc_dev[dev_index]->adc_virtaddr);
    fail1:
    kfree(axi_adc_dev[dev_index]);
    return status;
}

static const struct of_device_id axi_adc_dma_of_ids[] =
        {
                {.compatible = "alinx,axi-adc-dma",},
        };

static struct platform_driver axi_adc_dma_of_driver =
        {
                .driver = {
                        .name = MODULE_NAME,
                        .owner = THIS_MODULE,
                        .of_match_table = axi_adc_dma_of_ids,
                },
                .probe = axi_adc_probe,
                .remove = axi_adc_remove,
        };

module_platform_driver(axi_adc_dma_of_driver);
