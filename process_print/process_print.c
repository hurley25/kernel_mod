/*
 * =====================================================================================
 *
 *       Filename:  process_priont.c
 *
 *    Description:  内核打印进程列表
 *
 *        Version:  1.0
 *        Created:  2013年07月29日 11时49分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/proc_fs.h> 
#include <linux/init.h> 
#include <linux/sched.h>

static int __init print_init(void)
{
	struct task_struct *task, *thread;
	
	printk("process info:\n");

	do_each_thread(task, thread)

	printk("%s   pid:%d  tgid:%d  father pid:%d\n",
				thread->comm,
				thread->pid,
				thread->tgid,
				thread->parent->pid);

	while_each_thread(task, thread);

	return 0;
}

static void __exit print_exit(void)
{
	printk("Goodbye, process_print!\n");
}

module_init(print_init);
module_exit(print_exit);
MODULE_AUTHOR("hurley");
MODULE_DESCRIPTION("Print Process Info.");
MODULE_LICENSE("GPL");

