#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/uaccess.h>

// Information about a single process
struct proc_info 
{ 
	pid_t pid; // Pid of the process
	char name[16];
};

// Information about processes we need
struct procinfos 
{ 
	long studentID;
	struct proc_info proc; // Process with pid has given or current process
	struct proc_info parent_proc; // Parent process
	struct proc_info oldest_child_proc; // Oldest child process
};

SYSCALL_DEFINE2(get_proc_info, pid_t, pid, struct procinfos *, info) {
	struct procinfos tempProc;
	tempProc.studentID = 1910752; // Assign MSSV to studentID
	if (pid == -1) 
	{ 
		// Current process with pid = -1
		// Get current process information
		tempProc.proc.pid = current->pid;
		strcpy(tempProc.proc.name, current->comm);
		// Check if parent process exist or not
		if(current->parent != NULL)
		{
			// Get parent process information
			tempProc.parent_proc.pid = current->parent->pid;
			strcpy(tempProc.parent_proc.name, current->parent->comm);
		}
		struct task_struct *oldestChild;
		oldestChild = list_first_entry_or_null(&current->children, struct task_struct, sibling);
		// Check if child process exist or not 
		if(oldestChild != NULL) 
		{
			// Get oldest child process information
			tempProc.oldest_child_proc.pid = oldestChild->pid;
			strcpy(tempProc.oldest_child_proc.name, oldestChild->comm);
		}
		copy_to_user(info, &tempProc, sizeof(tempProc));
		printk("Student ID: %ld\n", tempProc.studentID); // Print studentID to drive device 
		return 0;
	}
	else 
	{	// Process with pid has given
		struct task_struct *task;
		for_each_process(task)
		{ 
			// Check process in loop
			if (task->pid == pid)
			{ 
				// Check if process is matched or not
				// Get matched process information
				tempProc.proc.pid = task->pid;
				strcpy(tempProc.proc.name, task->comm);
				// Check if parent process exist or not
				if(task->parent != NULL) 
				{
					tempProc.parent_proc.pid = task->parent->pid;
					strcpy(tempProc.parent_proc.name, task->parent->comm);
				}		
				struct task_struct *oldestChild;
				oldestChild = list_first_entry_or_null(&task->children, struct task_struct, sibling);
				// Check if child process exist or not 
				if(oldestChild != NULL) 
				{
					// Get oldest child process information
					tempProc.oldest_child_proc.pid = oldestChild->pid;
					strcpy(tempProc.oldest_child_proc.name, oldestChild->comm);
				}
				copy_to_user(info, &tempProc, sizeof(tempProc));
				printk("Student ID: %ld\n", tempProc.studentID); // Print studentID to drive device
				return 0;
			}
		}
	}
	printk("My student ID: %ld\n", tempProc.studentID); // Print studentID to drive device
	return EINVAL;
}

