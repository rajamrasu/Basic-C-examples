#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>

static int __init ofd_init(void)
{
	printk(KERN_INFO "Namaskar: OFD registered");
	return 0;
}

static void __exit ofd_exit(void)
{
	printk(KERN_INFO "Dhanywadd: OFD unregistered");
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(" Ananadraj ");
MODULE_DESCRIPTION(" My hello india driver");

