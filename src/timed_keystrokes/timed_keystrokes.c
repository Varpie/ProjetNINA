#include "timed_keystrokes.h"

void writeConfFile(double m, double sig) {
        FILE *f = fopen("conf", "w+");
        fprintf(f, "%f\n", m);
        fprintf(f, "%f\n", sig);
        fclose(f);
}

double esperance(double t[]) {
        int i;
        double sum = 0;
        for (i = 0; i < sizeText - 1; i++) {
                sum += t[i + 1] - t[i];
        }
        return sum / (double)sizeText;
}

double std_dev(double t[]) {
        double sum = 0;
        double sq_sum = 0;
        for (int i = 0; i < sizeText - 1; ++i) {
                sum += t[i + 1] - t[i];
                sq_sum += (t[i + 1] - t[i]) * (t[i + 1] - t[i]);
        }
        double mean = sum / (double)sizeText;

        double variance = (sq_sum / (double)sizeText) - (mean * mean);
        /* printf("m:%f\nsq:%f\nv:%f\n", mean, sq_sum, variance); */
        return sqrt(variance);
}

void clean_stdin(void) {
        int c;
        do {
                c = getchar();
        } while (c != '\n' && c != EOF);
}

void ask_keystrokes() {
        int fd = open("/dev/input/event0", O_RDONLY);
        struct input_event ev;
        double t[sizeText];
        int i = 0;
        printf("Please write the following text : \n%s\n", testText);
        while (i < sizeText) {
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
}

void keystroke_time(int timer) {
        int fd = open("/dev/input/event0", O_RDONLY);
        //FILE *f = fopen("conf", "w+");
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
                                map[k[1]][k[0]].mean = ((map[k[1]][k[0]].mean * map[k[1]][k[0]].n - 1)*(t[1]-t[0]) / map[k[1]][k[0]].n);
                                //fprintf(f, "%d;%d;%f\n", k[1], k[0], t[1] - t[0]);
                        }
                }
        }
        //fclose(f);
}
void create_mapconf(){
        int i,j;
        FILE *f = fopen("conf", "w+");
        for(i=0;i<256;i++){
            for(j=0;j<256;j++){
                fprintf(f, "%d;%d;%f\n", i,j,map[i][j].mean);
            }
        }
}
void create_map() {
        FILE *fp;
        char *line = NULL;
        char **ptr = NULL;
        size_t len = 0;
        ssize_t read;
        int actkey = 0;
        int prevkey = 0;
        fp = fopen("conf", "r");
        if (fp == NULL) {
                printf("No file found\n");
        } else {
                while ((read = getline(&line, &len, fp)) != -1) {
                        char *token;
                        token = strtok(line, delimiter);
                        /* Read the actual keycode */
                        prevkey = strtol(token, ptr, 10);
                        token = strtok(NULL, delimiter);
                        actkey = strtol(token, ptr, 10);
                        token = strtok(NULL, delimiter);
                        /* Add 1 to the number of terms of the mean */
                        map[actkey][prevkey].n += 1;
                        /* Recompute the mean */
                        map[actkey][prevkey].mean = (((map[actkey][prevkey].mean * (map[actkey][prevkey].n - 1)) + strtod(token, ptr)) / map[actkey][prevkey].n);
                        if (map[actkey][prevkey].mean != 0) {
                                printf("%d\n", map[actkey][prevkey].n);
                                printf("%g\n", map[actkey][prevkey].mean);
                        }
                }
        }
        fclose(fp);
        if (line)
                free(line);
        if (ptr)
                free(ptr);
}
