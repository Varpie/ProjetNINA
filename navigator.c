#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

	//version fork
 pid_t pid;
  	char *parmList[] = {"firefox", "google.com", NULL};
  	int a,b;

  	if ((pid = fork()) == -1)
  	  perror("fork failed");
 	 if (fork() == 0) {
 	   a = execvp("/usr/bin/firefox", parmList);

 	 }
 	 else {
     //printf("pid : %d",pid);
 	 }

 	return 0;
}

//http://stackoverflow.com/questions/985051/what-is-the-purpose-of-fork
//http://www.yolinux.com/TUTORIALS/ForkExecProcesses.html
