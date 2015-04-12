/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年01月04日 16时46分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
 *        Company:  Alibaba-Inc Aliyun
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
        int testdev;

        testdev = open("/dev/chardev0", O_RDWR);
        
        if (testdev == -1) {
                perror("open:");
                exit(0);
        }

        char buff[] = "I'm fron user space";
        
        if (write(testdev, buff, sizeof(buff)) < 0) {
                perror("write:");
        }

        close(testdev);

        return 0;
}
