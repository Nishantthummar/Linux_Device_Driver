#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>

#define DEVICE_NAME "cdriver" // name of device
#define CLASS_NAME "N" // class name
#define IRQ 1 // 1 for keyboard interrupt
unsigned int i = 0;

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Nishant");


static int major; //!< variable to store major number of driver
static struct class* myclass = NULL;
static struct device* mydevice = NULL;
static char msg[100] = "Hello World from Kernel" ;

///////////////////////// function prototype //////////////////////////////////
static int device_open(struct inode *, struct file *); 
static int device_close(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t , loff_t *);
static ssize_t device_write(struct file *,const char *, size_t, loff_t *);
static irqreturn_t irq_handler(int irq, void *dev_id)
{
	printk(KERN_INFO "Keyboard: Interrupt occured %d\n", i);
	i++;
	return IRQ_HANDLED;
}

/////////// File Operation which driver can support////////////////////////////
static struct file_operations fops = 
{

	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_close
};

///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when module is load
///////////////////////////////////////////////////////////////////////////////
static int ini_module(void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if( major < 0 ) {
		printk(KERN_ALERT "Unable to register device");
		return 0;
	}
	
	printk(KERN_INFO "Device Register Successfully");
	myclass = class_create(THIS_MODULE, CLASS_NAME);
	if(myclass == NULL) {
		printk(KERN_ALERT "class is not created\n");
		return -1;
	}

	mydevice = device_create(myclass, NULL, MKDEV(major, 0), 
			NULL, DEVICE_NAME);
	if(mydevice == NULL) {
		printk(KERN_ALERT "device is not created\n");
		return -1;
	}
	request_irq(IRQ_HANDLED, irq_handler, IRQF_SHARED, "interrupt", (void*)(irq_handler));

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when module is unloaded
///////////////////////////////////////////////////////////////////////////////
static void exi_module(void) 
{
	free_irq(IRQ, (void*)(irq_handler));
	device_destroy(myclass, MKDEV(major, 0));
	printk(KERN_INFO "Device unregister..");
	class_unregister(myclass);
	class_destroy(myclass);
	unregister_chrdev(major, DEVICE_NAME);
}

///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when file is open
///////////////////////////////////////////////////////////////////////////////
static int device_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "File is opened");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when file is close
///////////////////////////////////////////////////////////////////////////////
static int device_close(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "File is closed");
	return 0; 
}

///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when file is read
///////////////////////////////////////////////////////////////////////////////
static ssize_t device_read(struct file *file, char *buf, 
		size_t count, loff_t *offset)
{
	char *data = "Hello World from kernel";
	int len = 100;
	raw_copy_to_user(buf, data, len);
	//while(len >=0) {
	//	put_user(*(data++), buf++);
	//	len--;

	//}
	printk(KERN_INFO "Reading...");
	printk(KERN_INFO "The data is %s", buf);
       	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when write into file
///////////////////////////////////////////////////////////////////////////////
static ssize_t device_write(struct file *file, const char __user *buf, 
		size_t count, loff_t *offset)
{
	//char data;
	raw_copy_from_user(msg, buf, 100);
	//get_user(*msg, buf);
	printk(KERN_INFO "the data is %s", msg);
	return 0;
}


module_init(ini_module); // call ini_module when module is loaded
module_exit(exi_module); // call exi_module when module is unloaded

/////////////////////////////////////// End ///////////////////////////////////
