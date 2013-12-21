/*
 * =====================================================================================
 *
 *       Filename:  printk.c
 *
 *    Description:  内核级 helloworld
 *
 *        Version:  1.0
 *        Created:  2013年07月26日 08时55分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <linux/module.h>

static int __init hello_init(void)
{
	printk("<1>Hello Kernel World!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("<1>Goodbye, Kernel World!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("hurley");
MODULE_DESCRIPTION("An example of device driver!");
MODULE_LICENSE("GPL");

