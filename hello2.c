#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <hello1.h>

static unsigned int count = 1;

MODULE_AUTHOR("Osypenko Kostiantyn <osypenko.kostyantin@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello2 module. Printing number of times 'Hello, world!'");
MODULE_LICENSE("Dual BSD/GPL");
module_param(count, uint, 0644);
MODULE_PARM_DESC(count, "Number of times to print 'Hello, world!'");

static int __init hello_init(void)
{
	int ret;
	ret = print_hello(count);

	return ret;
}

static void __exit hello_exit(void)
{
	// Nothing
}

module_init(hello_init);
module_exit(hello_exit);