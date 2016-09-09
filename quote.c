/* * Steps to use this file:
 *
 * $ make
 * $ sudo insmod quote.ko
 * $ sudo mknod /dev/quote c 60 0
 * $ sudo chmod 666 /dev/quote
 * $ cat /dev/quote
 * $ ctrl-c
 * $ sudo rmmod quote

*/

/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/string.h>

MODULE_AUTHOR("Thomas D. Turner <thomastdt@gmail.com>");
MODULE_DESCRIPTION("Char device driver for read only");
MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of quote.c functions */
static int quote_open(struct inode *inode, struct file *filp);
static int quote_release(struct inode *inode, struct file *filp);
static ssize_t quote_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
//static ssize_t quote_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static void quote_exit(void);
static int quote_init(void);

/* Structure that declares the usual file */
/* access functions */
static struct file_operations quote_fops = {
  read: quote_read,
//  write: quote_write,
  open: quote_open,
  release: quote_release
};

/* Declaration of the init and exit functions */
module_init(quote_init);
module_exit(quote_exit);

/* Global variables of the driver */
/* Major number */
static int quote_major = 60;
/* Buffer to store data */

static int quote_init(void) {
  int result;
  /* Registering device */
  result = register_chrdev(quote_major, "quote", &quote_fops);
  if (result < 0) {
    printk(
      "<1>quote: cannot obtain major number %d\n", quote_major);
    return result;
  }

  printk("<1>Inserting quote module\n" ); 
  return 0;

}

static void quote_exit(void) {
  /* Freeing the major number */
  unregister_chrdev(quote_major, "quote");

 
  printk("<1>Removing quote module\n");

}

static int quote_open(struct inode *inode, struct file *filp) {

  /* Success */
  return 0;
}

static int quote_release(struct inode *inode, struct file *filp) {
 
  /* Success */
  return 0;
}

static ssize_t quote_read(struct file *filp, char __user *buf, 
            size_t count, loff_t *f_pos) { 
    char message[] = "It's not what you know, it's what you can prove --Lonzo\n";
    ssize_t len = strlen(message);

    /* Transfering data to user space */ 
    if (count < len)
        return -EFAULT;

    if(copy_to_user((void __user *)buf, &message, len))
        return -EFAULT;

    *f_pos += len;
    return len;
}

