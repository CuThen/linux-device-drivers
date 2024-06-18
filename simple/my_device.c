#include <my_device.h>

static int __init hello_init(void) {
  printk(KERN_INFO "Hello world from the kernel!\n");
  return 0;
}

static void __exit hello_exit(void) {
  printk(KERN_INFO "Goodbye from the kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phuc Phan Son");
MODULE_DESCRIPTION("A simple hello world kernel module");
