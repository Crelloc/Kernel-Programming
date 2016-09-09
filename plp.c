#include <linux/module.h>
#include <linux/init.h>

int __init hello_init(void)
{
	printk("plp: hello reader!\n");
	return 0;
}

void __exit hello_exit(void)
{
	printk("plp: bye reader!\n");
}


MODULE_AUTHOR("Thomas D. Turner <thomastdt@gmail.com>");
MODULE_DESCRIPTION("Module that prints to the kernel log upon loading and unloading");
MODULE_LICENSE("GLP");

module_init(hello_init);
module_exit(hello_exit);