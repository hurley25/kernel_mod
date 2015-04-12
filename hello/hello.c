/*
 * =====================================================================================
 *
 *       Filename:  hello.c
 *
 *    Description:  hello world 内核模块
 *
 *        Version:  1.0
 *        Created:  2014年12月26日 12时27分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
 *        Company:  Alibaba-Inc Aliyun
 *
 * =====================================================================================
 */

#include <linux/module.h> 
#include <linux/init.h> 

static int __init hello_init(void)
{
	printk("<1> Hello, Kernel!\n");

	return 0;
}

static void __exit hello_exit(void)
{
	printk("<1> Goodbye, Kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("qianyi.lh");
MODULE_DESCRIPTION("Hello Kernel.");
MODULE_LICENSE("GPL");

