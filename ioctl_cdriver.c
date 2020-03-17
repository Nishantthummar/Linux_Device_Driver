#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_cdriver"
#define CLASS_NAME "ioctl"
#define IOCTL_READ _IOR('N', 'a', char *)
#define IOCTL_WRITE _IOW('N', 'b', char *)

MODULE_AUTHOR("Nishant");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

char *value = 0;
static int major;
static char *msg;
static struct class *myclass = NULL;
static struct device *mydevice = NULL;

///////////// Function Prototype /////////////////////////////
static int device_open(struct inode *, struct file *_);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *,const char *, size_t, loff_t *);
static long etx_ioctl(struct file *, unsigned int , unsigned long );

//////////// File operations supported by device /////////////
static struct file_operations fops = {
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = etx_ioctl
};

// __init function execute when module is load into kernel
static int __init ini_module(void) 
{
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0 ) {
		printk(KERN_ALERT "unable to register module..");
		return major;
	}
	printk(KERN_INFO "Device Register successfully with major number is %d", major);
	myclass = class_create(THIS_MODULE, CLASS_NAME);
	if(myclass == NULL) {
		printk(KERN_ALERT "class not created");
		return -1;
	}
	printk(KERN_INFO "class created ");
	mydevice = device_create(myclass, NULL, MKDEV(major, 0),
			NULL, DEVICE_NAME);
	if(mydevice == NULL) {
		printk(KERN_ALERT "device not created");
		return -1;
	}
	printk(KERN_INFO "device created");
	return 0;
}

// __exit function execute when module is unload from kernel
static void __exit exi_module(void)
{
	device_destroy(myclass, MKDEV(major, 0));
	class_unregister(myclass);
	class_destroy(myclass);
	unregister_chrdev(major, DEVICE_NAME);
	printk(KERN_INFO "Device unregister");
	return;
}
static int device_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "File open");
	return 0;
}


static int device_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "File close");
	return 0;
}

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		case IOCTL_READ:
			raw_copy_to_user((char*) arg, &value, sizeof(value));
		       	break;
		case IOCTL_WRITE:
			raw_copy_from_user(&value, (char*) arg, sizeof(value));
			printk(KERN_INFO "value = %s", value);
			break;

		default:
			printk(KERN_ALERT "please Invalid command");
	}
	return 0;
}




static ssize_t device_read(struct file *file, char *buf, size_t count, loff_t *offset)
{
	char *data = "Hello World from kernel";
	int len =100;
	raw_copy_to_user(buf, data, len);
	printk(KERN_INFO "Reading...");
	printk(KERN_INFO "the data is %s", buf);
	return 0;
}

static ssize_t device_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	raw_copy_from_user(msg, buf, 100);
	printk(KERN_INFO "The data is %s", msg);
	return 0;
}

module_init(ini_module);
module_exit(exi_module);



