#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

	// pid_t pid;
 //  	char *parmList[] = {"firefox", "google.com", NULL};
 //  	int a;

 //  	if ((pid = fork()) == -1)
 //  	  perror("fork failed");

 // 	 if (pid == 0) {
 // 	   a = execvp("/usr/bin/firefox", parmList);
 // 	   printf("%d",getpid());
 // 	 }
 // 	 else {
 // 	   waitpid(pid, 0, 0);
 // 	 }

	int a;

	char *parmList[] = {"firefox", "google.com", NULL};
	a = execvp("/usr/bin/firefox", parmList);
	printf("%d\n", getpid());
 	return 0;
}

//http://stackoverflow.com/questions/985051/what-is-the-purpose-of-fork