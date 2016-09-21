#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Run browser as a child process and store it's pid in var child_pid
int main()
{
  pid_t pid;
  //parameters for execvp
  char *parmList[] = {"firefox", "google.com", NULL};
  char command[50] = {0};

  int a,child_pid;

  if ((pid = fork()) == -1)
    perror("fork failed");
  if (pid == 0) {
    a = execvp("/usr/bin/firefox", parmList);
  } else {
    child_pid = pid;

    //Create command to kill process and execute it with bash
    sprintf(command,"kill -9 %d",child_pid);
    sleep(10);
    system(command);
  return 0;
}
