#include <linux/module.h>
#include <linux/init.h>

static int ini_module(void)
{
	printk(KERN_INFO "Insterting module");
	}
static void exi_module(void)
{

	printk(KERN_INFO "removing module");
}

module_init(ini_module);
module_exit(exi_module);
