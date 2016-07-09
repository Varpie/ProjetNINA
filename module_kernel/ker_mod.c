#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init init(void) {
		printk(KERN_INFO "Hello world !\n");
		return 0;
}

static void __exit exit_function(void) {
		printk(KERN_INFO "Adios amigos.\n");
}

module_init(init);
module_exit(exit_function);
