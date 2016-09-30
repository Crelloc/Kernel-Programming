/**
 * @file	char.c
 * @author	Thomas D Turner
 * @date	26 September 2016
 * @version	0.1
 * @brief	A simple example character device-driver
**/

#include <linux/init.h>		//Macros used to mark up functions e.g. __init _exit
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/blkdev.h>
#include <linux/device.h> 
#include <asm/uaccess.h>
#include <linux/string.h>

#define DEVICE_NAME	"phonychar"
#define CLASS_NAME	"plp"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas D Turner <thomastdt@gmail.com>");
MODULE_DESCRIPTION("A simple example character device");
MODULE_VERSION("0.1");

/* global variables */

static char message[256] = {0};
static short size_of_message;
static __u32 majorNumber;		/* dynamically assigned at registration. */
static struct device *char_dev = NULL;	/* dynamically allocated at runtime */
static struct class *char_class = NULL;	/* pretend /sys/class */

/* function prototypes */ 

static int 	dev_open(struct inode * inode, struct file * file);
static int 	dev_release(struct inode * inode, struct file * file);
static ssize_t	dev_read(struct file * file, char __user * buf, size_t count,loff_t *ppos);
static ssize_t	dev_write(struct file * file, const char * buf, size_t count, loff_t *ppos);

/* file operations */

static struct file_operations dev_fops = {
	.read 	= dev_read,
	.write 	= dev_write,
	.open 	= dev_open,
	.release= dev_release,
	.owner	= THIS_MODULE,
};

/*
 * dev_open: open the phony char device
 */

static int dev_open(struct inode * inode, struct file * file)
{
	printk(KERN_INFO DEVICE_NAME ": device file opened.\n");
	return 0;
}

/*
 * dev_release: close (release) the phony char device
 */

static int dev_release(struct inode * inode, struct file * file)
{
	printk(KERN_INFO DEVICE_NAME ": device file released.\n");
	return 0;
}

/*
 * dev_read: read the phony char device
 */

static ssize_t	dev_read(struct file * file, char __user * buf, size_t count,loff_t *ppos)
{

	if (count < size_of_message)
        	return -EFAULT;

    	if(copy_to_user((void __user *)buf, &message, size_of_message)){
    		printk(KERN_INFO DEVICE_NAME "Failed to send characters to the user\n");
        	return -EFAULT;
    	}

        printk(KERN_INFO DEVICE_NAME ": Sent %d characters to user\n", size_of_message);

    	*ppos += size_of_message;
    	return size_of_message;

}

/*
 * dev_write: write to the phony char device
 */

static ssize_t	dev_write(struct file * file, const char * buf, size_t count, loff_t *ppos)
{
	sprintf(message, "%s(%d letters)", buf, (int)count);
	size_of_message = strlen(message);
	printk(KERN_INFO DEVICE_NAME ": Received %d characters from the user\n", (int)count);
	return count;
}

static int __init char_init(void)
{
	printk(KERN_INFO "%s: Initializing %s LKM\n", DEVICE_NAME, DEVICE_NAME);

	majorNumber = register_chrdev(0, DEVICE_NAME, &dev_fops);
	if(majorNumber<0){
		printk(KERN_ALERT DEVICE_NAME " failed to register a major number.\n");
		return majorNumber;
	}

	printk(KERN_INFO "%s: registered correctly with major number %d\n", DEVICE_NAME, majorNumber);

	char_class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(char_class)){
		printk(KERN_ALERT "Failed to register device class\n");
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(char_class);
	}

   	printk(KERN_INFO "%s: device class registered correctly\n", DEVICE_NAME);


	char_dev = device_create(char_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if(IS_ERR(char_dev)){
		printk(KERN_ALERT "Failed to create the device\n");
		class_destroy(char_class);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(char_dev);
	}
	
	printk(KERN_INFO "%s: device class created correctly\n", DEVICE_NAME); // Made it! device was initialized
   
   	return 0;

}

/*
 * char_exit: uninitialize the phony device
 */

 static void __exit char_exit(void)
 {
 	device_destroy(char_class, MKDEV(majorNumber, 0));     // remove the device
	class_unregister(char_class);                          // unregister the device class
	class_destroy(char_class);                             // remove the device class
	unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
	printk(KERN_INFO "%s: Goodbye from the LKM!\n", DEVICE_NAME);
 }

 module_init(char_init);
 module_exit(char_exit);

