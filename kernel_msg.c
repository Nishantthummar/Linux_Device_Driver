#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/kernel.h>
MODULE_LICENSE("Dual BSD/GPL");

int i;
static int howmany = 1;
static char *who = "ABC";
module_param(howmany, int, S_IRUGO);
module_param(who, charp, S_IRUGO);

static int hello_init(void) {
	printk(KERN_INFO "HELLO WORLD!\n");
	//printk(KERN_INFO "The process is \"%s\" (pid %i)\n", current->comm, current->pid)

	for(i=0; i<howmany; i++) {
		printk("Hello %s for %d time \n", who, i);
	}
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Good BYE\n");
}

module_init(hello_init);
module_exit(hello_exit);

