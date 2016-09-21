#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Run browser as a child process and store it's pid in var child_pid
int main()
{
  pid_t pid;
  //parameters for execvp
  char *parmList[] = {"firefox", "google.com", NULL};

  int a,child_pid;

  if ((pid = fork()) == -1)
    perror("fork failed");
  if (pid == 0) {
    a = execvp("/usr/bin/firefox", parmList);
  } else {
    child_pid = pid;
  }
  return 0;
}
