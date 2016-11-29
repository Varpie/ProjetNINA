#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv){
 int fd = open("/dev/input/event0", O_RDONLY);
 struct input_event ev;
 char text[] = "this is a simple test to get your typing speed"; // there is no tricks just words that you have to type so we can use this information later thank you for your involvment in this typing test we really appreciate it";
 int size = sizeof(text)/sizeof(char);
 int t[size];
 int i =0;
 printf("Please write the following text : \n%s\n", text);
 while(i<size-1){
    read(fd, &ev, sizeof(struct input_event));
    if(ev.type == 1 && ev.value == 1){
        t[i] = (ev.time.tv_sec%1000)*1000000+ev.time.tv_usec;
        i++;
    }
 }
 int j = 0;
 FILE* f = fopen("../conf","w+");
 for(j=0;j<size-2;j++){
   if((t[j+1]-t[j])<0){
     fprintf(f,"%i\n", 1000000+t[j+1]-t[j]);
   }else{
     fprintf(f,"%i\n", t[j+1]-t[j]);
  }
   //printf("\nDiff Temps %i : %i",j,t[j+1]-t[j]);
 }
 fclose(f);
 //printf("\n");
//fflush(stdin);
}
