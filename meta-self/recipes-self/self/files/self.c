#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

static int ini_module(void) {
	printk(KERN_INFO "Hello Working ..");
}

static void exi_module(void) {
	printk(KERN_INFO "Bye bye removing..");
}

module_init(ini_module);
module_exit(exi_module);
