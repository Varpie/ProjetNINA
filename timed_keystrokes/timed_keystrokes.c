#include "timed_keystrokes.h"

void writeConfFile(double m, double sig){
  FILE* f = fopen("conf","w+");
  fprintf(f,"%f\n",m);
  fprintf(f,"%f\n",sig);
  fclose(f);
}

double esperance(double t[]){
  int i;
  double sum=0;
  for(i=0;i<sizeText-1;i++){
    sum += t[i+1]-t[i];
 }
  return sum/(double)sizeText;
}



double std_dev(double t[]) {
    double sum = 0;
    double sq_sum = 0;
    for(int i = 0; i < sizeText-1; ++i) {
        sum += t[i+1]-t[i];
        sq_sum += (t[i+1]-t[i])*(t[i+1]-t[i]);
    }
    double mean = sum / (double)sizeText;

    double variance = (sq_sum / (double)sizeText) - (mean * mean);
    //printf("m:%f\nsq:%f\nv:%f\n", mean, sq_sum, variance);
    return sqrt(variance);
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
 double t[sizeText];
 int i=0;
 printf("Please write the following text : \n%s\n", testText);
 while(i<sizeText){
    read(fd, &ev, sizeof(struct input_event));
    if(ev.type == EV_KEY && ev.value == 1){
        t[i] = ev.time.tv_sec*1000000+ev.time.tv_usec;
        i++;
    }
 }
 nanosleep((const struct timespec[]){{0, 1000L}}, NULL);
 printf("\nAppuyez sur entrée pour continuer");
 getchar();
 double m = esperance(t);
 double sig = std_dev(t);
 writeConfFile(m,sig);
}
