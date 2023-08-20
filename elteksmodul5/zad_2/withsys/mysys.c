#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>

static struct kobject *test_kobj;
static int testnum;

static ssize_t foo_show(struct kobject *kodj, struct kobj_attribute *attr,
char *buf){
		return sprintf(buf, "%d\n",testnum);
	}
	
static ssize_t foo_store(struct kobject *kodj, struct kobj_attribute *attr,
const char *buf, size_t count){
		sscanf(buf, "%du", &testnum);
		return count;
	}
	
static struct kobj_attribute foo_attribute =__ATTR(testnum, 0660, foo_show, foo_store);


static int __init sys_init(void){
	
		int myerror = 0;
		
		pr_debug("All done. Module work \n");
		
		test_kobj = kobject_create_and_add("systest", kernel_kobj);
		
		if(!test_kobj){
			return -ENOMEM;
			}
		
		myerror = sysfs_create_file(test_kobj, &foo_attribute.attr);
		
		
		if(myerror){
				pr_debug("failed to create\n");
			}
			
		return myerror;
		
		}
	
static void __exit sys_exit(void){
		pr_debug("All done. Now module do not work \n");
		kobject_put(test_kobj);
	}
	
MODULE_LICENSE("GPL");
module_init(sys_init);
module_exit(sys_exit);
