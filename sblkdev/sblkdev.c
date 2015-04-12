/*
 * =====================================================================================
 *
 *       Filename:  sblkdev.c
 *
 *    Description:  一个简单的块设备驱动
 *
 *        Version:  1.0
 *        Created:  2014年12月27日 15时07分48秒
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
#include <linux/blkdev.h>

// 块设备名称
#define SBLKDEV_DISKNAME "sblkdev"

// 块设备号
#define SBLKDEV_DEVICEMAJOR COMPAQ_SMART2_MAJOR

// 块设备大小
#define SBLKDEV_BYTES (16*1024*1024)

static struct gendisk *sblkdev_disk;

// fops 结构
struct block_device_operations sblkdev_fops = {
        .owner = THIS_MODULE,
};

unsigned char sblkdev_data[SBLKDEV_BYTES];

// 块设备请求队列
static struct request_queue *sblkdev_queue;

// 块设备请求队列处理函数
static void sblkdev_do_request(struct request_queue *q)
{
        struct request *req;
        char *disk_mem;
        struct bio_vec *bvec;
        struct req_iterator ri;
        char *buffer;

        while ((req = blk_fetch_request(q)) != NULL) {

                // 请求的开始扇区 + 本次请求的扇区数量 不得超出设备范围
                if (blk_rq_pos(req) + blk_rq_sectors(req) > SBLKDEV_BYTES >> 9) {
                        printk(KERN_ERR SBLKDEV_DISKNAME ": bad request: block=%llu, count=%u\n",
                                        (unsigned long long)blk_rq_pos(req), blk_rq_sectors(req));
                        blk_end_request_all(req, -EIO);
                        continue;
                }

                // 操作位置 = 设备存储的起始位置 + 操作的扇区号 * 512 (<< 9)
                disk_mem = sblkdev_data + (blk_rq_pos(req) << 9);

                // 检测是读还是写
                switch (rq_data_dir(req)) {
                        case READ:
                                rq_for_each_segment(bvec, req, ri) {
                                        buffer = kmap(bvec->bv_page) + bvec->bv_offset;
                                        memcpy(buffer, disk_mem, bvec->bv_len);
                                        kunmap(bvec->bv_page);
                                        disk_mem += bvec->bv_len;
                                }
                                __blk_end_request_all(req, 0);
                                break;
                        case WRITE:
                                rq_for_each_segment(bvec, req, ri) {
                                        buffer = kmap(bvec->bv_page) + bvec->bv_offset;
                                        memcpy(disk_mem, buffer, bvec->bv_len);
                                        kunmap(bvec->bv_page);
                                        disk_mem += bvec->bv_len;
                                }
                                __blk_end_request_all(req, 0);
                                break;
                }
        }
}

static int __init sblkdev_init(void)
{
        int ret = 0;

        sblkdev_queue = blk_init_queue(sblkdev_do_request, NULL);
        if (!sblkdev_queue) {
                ret = -ENOMEM;
                goto err_init_queue;
        }

        sblkdev_disk = alloc_disk(1);
        if (!sblkdev_disk) {
                ret = -ENOMEM;
                goto err_alloc_disk;
        }

        // 设备名称
        strcpy(sblkdev_disk->disk_name, SBLKDEV_DISKNAME);

        // "借用" 一个不怎么用的设备的设备号...
        sblkdev_disk->major = SBLKDEV_DEVICEMAJOR;

        sblkdev_disk->first_minor = 0;
        sblkdev_disk->fops = &sblkdev_fops;
        sblkdev_disk->queue = sblkdev_queue;
        set_capacity(sblkdev_disk, SBLKDEV_BYTES >> 9);

        add_disk(sblkdev_disk);

	return 0;

err_alloc_disk:
        blk_cleanup_queue(sblkdev_queue);

err_init_queue:
        return ret;
}

static void __exit sblkdev_exit(void)
{
        del_gendisk(sblkdev_disk);
        put_disk(sblkdev_disk);
        blk_cleanup_queue(sblkdev_queue);
}

module_init(sblkdev_init);
module_exit(sblkdev_exit);

MODULE_AUTHOR("qianyi.lh");
MODULE_DESCRIPTION("Block Drivers.");
MODULE_LICENSE("GPL");

