#include "write_keyboard.h"

act_mean map[SIZE_MAP][SIZE_MAP];

void writeConfFile(double m, double sig) {
        FILE *f = fopen(name_conf, "w+");
        fprintf(f, "%f\n", m);
        fprintf(f, "%f\n", sig);
        fclose(f);
}

double esperance(double t[]) {
        int i;
        double sum = 0;
        for (i = 0; i < size_text - 1; i++) {
                sum += t[i + 1] - t[i];
        }
        return sum / (double)size_text;
}

double std_dev(double t[]) {
        double sum = 0;
        double sq_sum = 0;
        for (int i = 0; i < size_text - 1; ++i) {
                sum += t[i + 1] - t[i];
                sq_sum += (t[i + 1] - t[i]) * (t[i + 1] - t[i]);
        }
        double mean = sum / (double)size_text;

        double variance = (sq_sum / (double)size_text) - (mean * mean);
        /* printf("m:%f\nsq:%f\nv:%f\n", mean, sq_sum, variance); */
        return sqrt(variance);
}

void ask_keystrokes() {
        int fd = open("/dev/input/event0", O_RDONLY);
        struct input_event ev;
        double t[size_text];
        int i = 0;
        printf("Please write the following text : \n%s\n", test_text);
        while (i < size_text) {
                read(fd, &ev, sizeof(struct input_event));
                if (ev.type == EV_KEY && ev.value == 1) {
                        t[i] = ev.time.tv_sec * 1000000 + ev.time.tv_usec;
                        i++;
                }
        }
        nanosleep((const struct timespec[]){{0, 1000L}}, NULL);
        printf("\nAppuyez sur entrée pour continuer");
        getchar();
        double m = esperance(t);
        double sig = std_dev(t);
        writeConfFile(m, sig);
        close(fd);
}

void keystroke_time(int timer) {
        int fd = open("/dev/input/event0", O_RDONLY);
        struct input_event ev;
        short *k = calloc(2, sizeof(short));
        double *t = calloc(2, sizeof(double));
        printf("The program will listen %d seconds your inputs\n", timer);
        time_t endwait = time(NULL) + timer;
        while (time(NULL) < endwait) {
                read(fd, &ev, sizeof(struct input_event));
                if (ev.type == EV_KEY && ev.value == 1) {
                        k[0] = k[1];
                        k[1] = ev.code;
                        t[0] = t[1];
                        t[1] = ev.time.tv_sec * 1000000 + ev.time.tv_usec;
                        if (t[1] - t[0] < time_limit) {
                                map[k[1]][k[0]].n +=1;
                                map[k[1]][k[0]].mean = (((map[k[1]][k[0]].mean * (map[k[1]][k[0]].n - 1))+(t[1]-t[0])) / map[k[1]][k[0]].n);
                        }
                }
        }
        close(fd);
}
void create_mapconf(){
        int i,j;
        FILE *f = fopen(name_conf, "w+");
        for(i=0;i<SIZE_MAP;i++){
            for(j=0;j<SIZE_MAP;j++){
                fprintf(f, "%d;%d;%f;%d\n", i,j,base_time,base_n);
            }
        }
        fclose(f);
}

void update_mapconf(){
  int i,j;
  FILE *f = fopen(name_conf, "w+");
  for(i=0;i<SIZE_MAP;i++){
      for(j=0;j<SIZE_MAP;j++){
          fprintf(f, "%d;%d;%f;%d\n", i,j,map[i][j].mean,map[i][j].n);
      }
  }
  fclose(f);
}


void load_map() {
        FILE *fp;
        char *line = NULL;
        char **ptr = NULL;
        size_t len = 0;
        ssize_t read;
        int actkey = 0;
        int prevkey = 0;
        fp = fopen(name_conf, "r");
        if (fp == NULL) {
              create_mapconf();
              fp = fopen(name_conf, "r");
        }
        while ((read = getline(&line, &len, fp)) != -1) {
                char *token;
                token = strtok(line, delimiter);
                prevkey = strtol(token, ptr, 10);
                token = strtok(NULL, delimiter);
                actkey = strtol(token, ptr, 10);
                token = strtok(NULL, delimiter);
                map[actkey][prevkey].mean = strtod(token, ptr);
                token = strtok(NULL, delimiter);
                map[actkey][prevkey].n = strtol(token, ptr, 10);
        }
        fclose(fp);
        if (line)
            free(line);
        if (ptr)
            free(ptr);
}
