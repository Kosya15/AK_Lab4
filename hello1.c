#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>
#include <hello1.h>

MODULE_AUTHOR("Osypenko Kostiantyn <osypenko.kostyantin@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello1 module. Works with data_list.");
MODULE_LICENSE("Dual BSD/GPL");

struct data_list {
	struct list_head list_node;
	ktime_t timestamp;
	ktime_t drt_print;
};

static LIST_HEAD(head);

int print_hello(int count)
{
	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Warning: param is 0 or between 5 and 10\n");
	}
	else if (count > 10) {
		pr_err("Error: param is greater that 10\n");
		return -EINVAL;
	}

	int i;
	for (i = 0; i < count; i++) {
		struct data_list *data = kmalloc(sizeof(*data), GFP_KERNEL);

		data->timestamp = ktime_get();

		pr_info("Hello, world!\n");

		data->drt_print = ktime_sub(ktime_get(), data->timestamp);
		list_add(&data->list_node, &head);
	}

	return 0;
}

EXPORT_SYMBOL(print_hello);

static int __init hello_init(void)
{
	return 0;
}

static void __exit hello_exit(void)
{
	struct data_list *pos, *next;

	list_for_each_entry_safe(pos, next, &head, list_node) {
		ktime_t drt = ktime_sub(ktime_get(), pos->timestamp);
		ktime_t drt_prnt = pos->drt_print;

		pr_info("Timestamp: %lld ns; Duration print: %lld ns\n", ktime_to_ns(drt), ktime_to_ns(drt_prnt));

		list_del(&pos->list_node);
		kfree(pos);
	}

}

module_init(hello_init);
module_exit(hello_exit);