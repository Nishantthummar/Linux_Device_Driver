#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

#define DEVICE_NAME "noname" 
#define CLASS_NAME "Yes"
#define BUF_LEN 80


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishant");
MODULE_VERSION("0.1");

static int major;
static char msg[BUF_LEN];
static struct class* myclass = NULL;
static struct device* mydevice = NULL;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *,const char *, size_t, loff_t *);


static struct file_operations fops = 
{
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release 
};

static int init_module1(void) 
{
	
	major = register_chrdev(0, DEVICE_NAME, &fops);

	if(major < 0) {
		printk(KERN_ALERT "Uanble to register Device");
		return major;
	}
	printk(KERN_INFO "Register the device with major number is %d\n", major);
	
	myclass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(myclass)) {
		unregister_chrdev(major, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(myclass);
	}
	printk(KERN_INFO "Mychardev: device class register correctly\n");

	mydevice = device_create(myclass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	if(IS_ERR(mydevice)) {
		class_destroy(myclass);
		unregister_chrdev(major, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create device\n");
		return PTR_ERR(mydevice);
	}
	printk(KERN_INFO "Mychardev: device class created correctly\n");
	return 0;
}

static void exit_module1(void) 
{
	device_destroy(myclass, MKDEV(major, 0));
	class_unregister(myclass);
	class_destroy(myclass);
	unregister_chrdev(major, DEVICE_NAME);
	printk(KERN_INFO "Unregister the device");
}

static int device_open(struct inode *inode, struct file *file)
{
	sprintf(msg, "Opening file");
	printk(KERN_ALERT "Opening file");
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{	
	sprintf(msg,"Closing..");
	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{	
	uint8_t *data = "Hello from kernel\n";
	size_t datalen = strlen(data);
	printk(KERN_ALERT "Some one is reading file");
	if(raw_copy_to_user(buffer, data, datalen)) {
		return -EFAULT;
	}
	return 0;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
	uint8_t data[BUF_LEN];	
	raw_copy_from_user(data, buffer, BUF_LEN);
	printk("data from the user is %s\n", data);
	return 0;
}
module_init(init_module1);
module_exit(exit_module1);



