#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>       // Required for character driver support
#include <linux/uaccess.h>  // Required for copy_to_user and copy_from_user


#define DEVICE_NAME "my_char_device"
#define BUF_LEN 80


static int major_number;
static char message[BUF_LEN];
static short message_size;
static int device_open_count = 0;


static int device_open(struct inode *inode, struct file *file) {
   if (device_open_count)
       return -EBUSY;
   device_open_count++;
   return 0;
}


static int device_release(struct inode *inode, struct file *file) {
   device_open_count--;
   return 0;
}


static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset) {
   int bytes_read = 0;
   if (*offset >= message_size)
       return 0;
   while (length && message_size) {
       put_user(message[bytes_read], buffer++);
       length--;
       message_size--;
       bytes_read++;
   }
   *offset += bytes_read;
   return bytes_read;
}


static ssize_t device_write(struct file *file, const char *buffer, size_t length, loff_t *offset) {
   int i;
   for (i = 0; i < length && i < BUF_LEN; i++)
       get_user(message[i], buffer + i);
   message_size = i;
   return i;
}


static struct file_operations fops = {
   .open = device_open,
   .read = device_read,
   .write = device_write,
   .release = device_release
};


static int __init char_device_init(void) {
   major_number = register_chrdev(0, DEVICE_NAME, &fops);
   if (major_number < 0) {
       printk(KERN_ALERT "Failed to register a major number\n");
       return major_number;
   }
   printk(KERN_INFO "Registered correctly with major number %d\n", major_number);
   return 0;
}


static void __exit char_device_exit(void) {
   unregister_chrdev(major_number, DEVICE_NAME);
   printk(KERN_INFO "Unregistered the device\n");
}


module_init(char_device_init);
module_exit(char_device_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver");