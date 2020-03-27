#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/cdev.h>

#define DEVICE_NAME "led_drive"
#define CLASS_NAME "led"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishant");

//GPFSL0 = 0x1000; //pin4 as out 
static int major = 0; //major number of module
static struct class* myclass = NULL;
static struct device* mydevice = NULL;
static unsigned PORT = 0x7E200000;
static unsigned RANGE =0x40;
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

u8 *addr;
u32 cmd;
/*struct raspi_gpio_dev {
	struct cdev cdev;
	struct gpio pin;
//	enum state state;
//	enum direction dir;
	bool irq_perm;
	unsigned long irq_flag;
	unsigned int irq_counter;
	spinlock_t lock;
};
*/
static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = device_open,
	.release = device_release
};

static int ini_module(void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if( major < 0) {
		printk(KERN_ALERT "Unable to register the device");
		return 0;
	}
	printk(KERN_INFO "Device Register successfully");
	myclass = class_create(THIS_MODULE, CLASS_NAME);
	mydevice = device_create(myclass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
//	GPSET4; // set the pin 4 
/*	static int flag;
	flag  = check_region(PORT, RANGE);
	if(flag < 0) {
		printk(KERN_ALERT "Unable to find free memory ");
		return 0;
	}
*/
	
	if(request_mem_region(PORT, RANGE, DEVICE_NAME) == NULL) {
		printk(KERN_ALERT "Unable to allocate memory");
		return 0;
	}
	addr = ioremap(PORT, RANGE);
//	sprintf("value of addr is %u", addr);
	
	return 0;
}

static void exi_module(void)
{
//	GPCLR4; //to clear pin 4
	
	release_mem_region(PORT, RANGE);
	printk(KERN_INFO "removing module....");
	device_destroy(myclass, MKDEV(major, 0));
	printk(KERN_INFO "Device unregister...");
	class_unregister(myclass);
	class_destroy(myclass);
	unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "Opening file..");
	cmd = 1 <<12;
	iowrite32(cmd, addr); //pin4 as output
	cmd = 1<<4;
	iowrite32(cmd, (addr+0x28)); // clear
	cmd = 1<<4;
	iowrite32(cmd, (addr+0x1c)); //set pin4 as high
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "closing file..");
	cmd=  1<<4;
	iowrite32(cmd, (addr+0x28));
	return 0;
}

module_init(ini_module);
module_exit(exi_module);
