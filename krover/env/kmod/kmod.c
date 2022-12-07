#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

//MODULE_DESCRIPTION("kernel module");
//MODULE_AUTHOR("pp");
MODULE_LICENSE("GPL");

static int kmod_init(void)
{
        printk("foR kroveR \n");
        /*unsigned long crtAddr = 0xffffffff81e00c60;
        int i = 0;
        while( i < 10)
        {
            printk("%02x ", *((uint8_t*)(crtAddr+i)));
            i++;
        }
        printk("\n");*/       
        return 0;
}

static void kmod_exit(void)
{
        printk("donE\n");
}

module_init(kmod_init);
module_exit(kmod_exit);
