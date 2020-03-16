#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#define DEVICE_NAME "cdriver" // name of device
#define CLASS_NAME "N" // class name

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Nishant");


static int major; //!< variable to store major number of driver
static struct class* myclass = NULL;
static struct device* mydevice = NULL;
//static char msg[100] = "Hello World from Kernel" ;

///////////////////////// function prototype //////////////////////////////////
static int device_open(struct inode *, struct file *); 
static int device_close(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t , loff_t *);
static ssize_t device_write(struct file *,const char *, size_t, loff_t *);
///////////////////////////////////////////////////////////////////////////////

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
	mydevice = device_create(myclass, NULL, MKDEV(major, 0), 
			NULL, DEVICE_NAME);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when module is unloaded
///////////////////////////////////////////////////////////////////////////////
static void exi_module(void) 
{
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
	int len = strlen(data);
	raw_copy_to_user(buf, data, len);
	printk(KERN_INFO "Reading...");
       	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//@brief  Function which executed when write into file
///////////////////////////////////////////////////////////////////////////////
static ssize_t device_write(struct file *file, const char __user *buf, 
		size_t count, loff_t *offset)
{
	char *data= '\0';
	raw_copy_from_user(data, buf, 100);
	printk(KERN_INFO "the data is %s", data);
	return 0;
}


module_init(ini_module); // call ini_module when module is loaded
module_exit(exi_module); // call exi_module when module is unloaded

/////////////////////////////////////// End ///////////////////////////////////
