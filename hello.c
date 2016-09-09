/*
*
*	Task01 Eudyptula Challenge
*/

#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{

	printk(KERN_DEBUG "Hello World!\n");

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_DEBUG "Goodbye world!\n");
}

MODULE_AUTHOR("Thomas D. Turner <thomastdt@gmail.com>");
MODULE_DESCRIPTION("Module that prints to the kernel debug log level");
MODULE_LICENSE("GLP");
