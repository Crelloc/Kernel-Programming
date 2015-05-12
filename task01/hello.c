/*
*
*	Task01 Eudyptula Challenge
*	Author-	Thomas Turner
	Date Modified-	05/07/2015	
*/

#include <linux/module.h>	//Needed by all modules
#include <linux/kernel.h>	//Needed for KERN_INFO

int init_module(void){	printk(KERN_DEBUG "Hello World!\n");
	
	return 0;
}

void cleanup_module(void){	printk(KERN_DEBUG "Goodbye world!\n");}
