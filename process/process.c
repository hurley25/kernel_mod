/*****************************************************************
  文件名：mem.c
  输入参数：
  pid 接收待查询进程的PID
  va 接收待查询的虚拟地址
 *****************************************************************/
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/sched.h> 
#include <linux/mm.h> 
#include <asm/pgtable.h> 
#include <asm/page.h> 

MODULE_LICENSE("GPL"); 

static int pid; 
static unsigned long va; 

module_param(pid, int, 0644); 
module_param(va, ulong, 0644); 

static int find_pgd_init(void) 
{ 
	unsigned long pa = 0; 
	struct task_struct *pcb_tmp = NULL; 

	pgd_t *pgd_tmp = NULL; 
	pud_t *pud_tmp = NULL; 
	pmd_t *pmd_tmp = NULL; 
	pte_t *pte_tmp = NULL; 

	printk("<0>""PAGE_OFFSET = 0x%lx\n", PAGE_OFFSET); 
	printk("<0>""PGDIR_SHIFT = %d\n", PGDIR_SHIFT); 
	printk("<0>""PUD_SHIFT = %d\n", PUD_SHIFT); 
	printk("<0>""PMD_SHIFT = %d\n", PMD_SHIFT); 
	printk("<0>""PAGE_SHIFT = %d\n", PAGE_SHIFT); 
	printk("<0>""PTRS_PER_PGD = %d\n", PTRS_PER_PGD); 
	printk("<0>""PTRS_PER_PUD = %d\n", PTRS_PER_PUD); 
	printk("<0>""PTRS_PER_PMD = %d\n", PTRS_PER_PMD); 
	printk("<0>""PTRS_PER_PTE = %d\n", PTRS_PER_PTE); 
	printk("<0>""PAGE_MASK = 0x%lx\n", PAGE_MASK); 

	if (!(pcb_tmp = pid_task(find_vpid(pid), PIDTYPE_PID))) {
		printk("<0>""Can't find the task %d .\n", pid);
		return 0; 
	} 
	printk("<0>""pgd = 0x%p\n", pcb_tmp->mm->pgd); 

	if (!find_vma(pcb_tmp->mm, va)) {
		printk("<0>""virt_addr 0x%lx not available.\n",va); 
		return 0; 
	} 
	pgd_tmp = pgd_offset(pcb_tmp->mm, va);
	printk("<0>""pgd_tmp = 0x%p\n", pgd_tmp);
	printk("<0>""pgd_val(*pgd_tmp) = 0x%lx\n",pgd_val(*pgd_tmp)); 

	if (pgd_none(*pgd_tmp)) {
		printk("<0>""Not mapped in pgd.\n"); 
		return 0; 
	} 

	pud_tmp = pud_offset(pgd_tmp, va); 
	printk("<0>""pud_tmp = 0x%p\n", pud_tmp); 
	printk("<0>""pud_val(*pud_tmp) = 0x%lx\n", pud_val(*pud_tmp)); 
	if (pud_none(*pud_tmp)) { 
		printk("<0>""Not mapped in pud.\n"); 
		return 0; 
	} 

	pmd_tmp = pmd_offset(pud_tmp, va); 
	printk("<0>""pmd_tmp = 0x%p\n", pmd_tmp); 
	printk("<0>""pmd_val(*pmd_tmp) = 0x%lx\n", pmd_val(*pmd_tmp)); 
	if (pmd_none(*pmd_tmp)) {
		printk("<0>""Not mapped in pmd.\n"); 
		return 0; 
	} 

	pte_tmp = pte_offset_kernel(pmd_tmp,va); 
	printk("<0>""pte_tmp = 0x%p\n", pte_tmp); 
	printk("<0>""pte_val(*pte_tmp) = 0x%lx\n", pte_val(*pte_tmp)); 

	if (pte_none(*pte_tmp)) {
		printk("<0>""Not mapped in pte.\n"); 
		return 0; 
	} 

	if (!pte_present(*pte_tmp)) {
		printk("<0>""pte not in RAM.\n"); 
		return 0; 
	} 

	pa = (pte_val(*pte_tmp)& PAGE_MASK) |(va & ~PAGE_MASK); 
	printk("<0>""virt_addr 0x%lx in RAM is 0x%lx .\n",va, pa); 
	printk("<0>""contect in 0x%lx is 0x%lx\n", pa, *(unsigned long *)((char *)pa + PAGE_OFFSET)); 

	return 0; 
} 

static void find_pgd_exit(void) 
{ 
	printk("<0>""Goodbye!\n"); 
} 

module_init(find_pgd_init); 
module_exit(find_pgd_exit);

