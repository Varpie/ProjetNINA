#include "getTimedKeystrokes.h"

void writeConfFile(int t[]){
  int j = 0;
  FILE* f = fopen("conf","w+");
  for(j=0;j<sizeText-1;j++){
    if((t[j+1]-t[j])<0){
      fprintf(f,"%i\n", 1000000+t[j+1]-t[j]);
    }else{
      fprintf(f,"%i\n", t[j+1]-t[j]);
   }
  }
  fclose(f);
}

void clean_stdin(void)
{
    int c;

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


int main(int argc, char ** argv){
 int fd = open("/dev/input/event0", O_RDONLY);
 struct input_event ev;
 int t[sizeText];
 int i=0;
 printf("Please write the following text : \n%s\n", testText);
 while(i<sizeText){
    read(fd, &ev, sizeof(struct input_event));
    if(ev.type == EV_KEY && ev.value == 1){
        t[i] = (ev.time.tv_sec%1000)*1000000+ev.time.tv_usec;
        i++;
    }
 }
 nanosleep((const struct timespec[]){{0, 100L}}, NULL);
 printf("\nAppuyez sur entrée pour continuer");
 clean_stdin();
 writeConfFile(t);

}
