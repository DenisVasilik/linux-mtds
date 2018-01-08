#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "xparameters.h"
#include "proto_defs.h"
#include "xspi.h"
#include "xil_io.h"
#include "mtds_hal.h"
#include "mtds_drv.h"

#define MTDS_NAME "mtds"

static struct cdev *mtds_cdev;
static struct class *mtds_class;
static struct device *mtds_device;
static dev_t mtds_dev;

static int mtds_setup_cdev(void);

static int mtds_init(void) {
	int ret = 0;

	ret = alloc_chrdev_region(&mtds_dev, 0, 1, MTDS_NAME);

	if (ret < 0) {
		pr_err("Unable to allocate major number for device %s - %d\n", MTDS_NAME, ret);
		return ret;
	}

	mtds_class = class_create(THIS_MODULE, MTDS_NAME);

	if (mtds_class == NULL) {
		pr_err("Unable to create class for %s\n", MTDS_NAME);
		unregister_chrdev_region(mtds_dev, 1);
		return -ENODEV;
	}

	mtds_device = device_create(mtds_class, NULL, mtds_dev, NULL, MTDS_NAME);

	if (mtds_device == NULL) {
		pr_err("Unable to create device for class %s - %d\n", MTDS_NAME, ret);
		class_destroy(mtds_class);
		unregister_chrdev_region(mtds_dev, 1);
		return ret;
	}

	ret = mtds_setup_cdev();

	if (ret < 0) {
		device_destroy(mtds_class, mtds_dev);
		class_destroy(mtds_class);
		unregister_chrdev_region(mtds_dev, 1);
		return ret;
	}

    return 0;
}

static long mtds_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	struct mtds_data __user *argp = (struct mtds_data __user *)arg;
	struct mtds_data* mtdsData = NULL;

	switch (cmd)
	{
		case IOC_INIT:
			MtdsHalInit(pinMtdsSelStd);
			MtdsHalInitSpi(_SPI_BASE, frqMtdsSpiDefault);
		break;
		case IOC_ENABLE_SLAVE:
			mtdsData = kmalloc(sizeof(struct mtds_data), GFP_KERNEL);
			
			if (!mtdsData)
			{
				pr_err("Failed to allocate memory\n");
				ret = -ENOMEM;
				goto finally;
			}

			if (copy_from_user(mtdsData, argp, sizeof(struct mtds_data)))
			{
				pr_err("Failed to copy from user\n");
				ret = -EACCES;
				goto finally;
			}

			MtdsHalEnableSlave(mtdsData->slave);
		break;
		case IOC_READY:
			mtdsData = kmalloc(sizeof(struct mtds_data), GFP_KERNEL);
			
			if (!mtdsData)
			{
				pr_err("Failed to allocate memory\n");
				ret = -ENOMEM;
				goto finally;
			}

			if (copy_from_user(mtdsData, argp, sizeof(struct mtds_data)))
			{
				pr_err("Failed to copy from user\n");
				ret = -EACCES;
				goto finally;
			}

			mtdsData->isReady = MtdsHalSpiReady();

			if (copy_to_user(argp, mtdsData, sizeof(struct mtds_data)))
			{
				pr_err("Failed to copy to user\n");
				ret = -EACCES;
				goto finally;
			}
		break;
		case IOC_WRITE:		
			mtdsData = kmalloc(sizeof(struct mtds_data), GFP_KERNEL);
			
			if (!mtdsData)
			{
				pr_err("Failed to allocate memory\n");
				ret = -ENOMEM;
				goto finally;
			}

			if (copy_from_user(mtdsData, argp, sizeof(struct mtds_data)))
			{
				pr_err("Failed to copy from user\n");
				ret = -EACCES;
				goto finally;
			}

			mtdsData->bRcv = MtdsHalPutSpiByte(mtdsData->bSnd);

			if (copy_to_user(argp, mtdsData, sizeof(struct mtds_data)))
			{
				pr_err("Failed to copy to user\n");
				ret = -EACCES;
				goto finally;
			}
		break;
		default:
			pr_info("[mtds_ioctl]: Invalid ioctl!\n");
			ret = -EINVAL;
	}

finally:
	if(mtdsData) {
		kfree(mtdsData);
	}

	return ret;
}

static int mtds_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int mtds_release(struct inode *inode, struct file *file)
{
	return 0;
}

static int mtds_mmap(struct file *file, struct vm_area_struct *vma)
{
	return 0;
}

static const struct file_operations mtds_fops = {
	   .owner = THIS_MODULE,
	   .unlocked_ioctl = mtds_ioctl,
	   .open = mtds_open,
	   .release = mtds_release,
	   .mmap = mtds_mmap,
};

static int mtds_setup_cdev(void)
{
	  int ret = 0;

	  mtds_cdev = cdev_alloc();

	  if (mtds_cdev == NULL) {
		pr_err("Could not allocate cdev structure\n");
		return -ENODEV;
	  }

	  mtds_cdev->owner = THIS_MODULE;
	  mtds_cdev->ops = &mtds_fops;

	  ret = cdev_add(mtds_cdev, mtds_dev, 1);

	  if (ret < 0) {
		pr_err("Could not add cdev - %d\n", ret);
	  }

	  return ret;
}

static void mtds_exit(void) {
	cdev_del(mtds_cdev);
	device_destroy(mtds_class, mtds_dev);
	class_destroy(mtds_class);
	unregister_chrdev_region(mtds_dev, 1);
}

MODULE_LICENSE("GPL");

module_init(mtds_init);
module_exit(mtds_exit);
