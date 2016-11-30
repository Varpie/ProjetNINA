#include "getTimedKeystrokes.h"

void writeConfFile(int t[]){
  int j = 0;
  FILE* f = fopen("../conf","w+");
  for(j=0;j<sizeText-2;j++){
    if((t[j+1]-t[j])<0){
      fprintf(f,"%i\n", 1000000+t[j+1]-t[j]);
    }else{
      fprintf(f,"%i\n", t[j+1]-t[j]);
   }
  }
  fclose(f);
}


int main(int argc, char ** argv){
 int fd = open("/dev/input/event0", O_RDONLY);
 struct input_event ev;
 int t[sizeText];
 int i =0;
 printf("Please write the following text : \n%s\n", testText);
 while(i<size-1){
    read(fd, &ev, sizeof(struct input_event));
    if(ev.type == 1 && ev.value == 1){
        t[i] = (ev.time.tv_sec%1000)*1000000+ev.time.tv_usec;
        i++;
    }
 }
 writeConfFile(t);

}
