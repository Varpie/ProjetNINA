#include "write_keyboard.h"

int setup_uinput_device()
{
/* Temporary variable */
	int i=0;
/* Open the input device */
	uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (uinp_fd == NULL)
	{
		printf("Unable to open /dev/uinput\n");
		return -1;
	}
	memset(&uinp,0,sizeof(uinp)); // Intialize the uInput device to NULL
	strncpy(uinp.name, "Custom Keyboard", UINPUT_MAX_NAME_SIZE);
	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;
	// Setup the uinput device
	ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
	ioctl(uinp_fd, UI_SET_EVBIT, EV_REP);
	ioctl(uinp_fd, UI_SET_RELBIT, REL_X);
	ioctl(uinp_fd, UI_SET_RELBIT, REL_Y);
	for (i=0; i < 256; i++) {
		ioctl(uinp_fd, UI_SET_KEYBIT, i);
	}
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TOUCH);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_RIGHT);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_FORWARD);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_BACK);
	/* Create input device into input sub-system */
	write(uinp_fd, &uinp, sizeof(uinp));
	if (ioctl(uinp_fd, UI_DEV_CREATE))
	{
		printf("Unable to create UINPUT device.");
		return -1;
	}
	return 1;
}


void send_click_events( )
{
// Move pointer to (0,0) location
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_REL;
	event.code = REL_X;
	event.value = 100;
	write(uinp_fd, &event, sizeof(event));
	event.type = EV_REL;
	event.code = REL_Y;
	event.value = 100;
	write(uinp_fd, &event, sizeof(event));
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));
// Report BUTTON CLICK - PRESS event
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_KEY;
	event.code = BTN_LEFT;
	event.value = 1;
	write(uinp_fd, &event, sizeof(event));
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));
// Report BUTTON CLICK - RELEASE event
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_KEY;
	event.code = BTN_LEFT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));
}

void press_a_button(int key){
	// Report BUTTON CLICK - PRESS event
		memset(&event, 0, sizeof(event));
		gettimeofday(&event.time, NULL);
		event.type = EV_KEY;
		event.code = key;
		event.value = 1;
		write(uinp_fd, &event, sizeof(event));
		event.type = EV_SYN;
		event.code = SYN_REPORT;
		event.value = 0;
		write(uinp_fd, &event, sizeof(event));
}

void release_a_button(int key){
	// Report BUTTON CLICK - RELEASE event
		memset(&event, 0, sizeof(event));
		gettimeofday(&event.time, NULL);
		event.type = EV_KEY;
		event.code = key;
		event.value = 0;
		write(uinp_fd, &event, sizeof(event));
		event.type = EV_SYN;
		event.code = SYN_REPORT;
		event.value = 0;
		write(uinp_fd, &event, sizeof(event));
}
void send_a_button(int key, int modifier)
{
	if(modifier == 0)
	{
	press_a_button(key);
	release_a_button(key);
}else{
	press_a_button(modifier);
	press_a_button(key);
	release_a_button(key);
	release_a_button(modifier);
}

}
void send_a_button_default(int key){
	send_a_button(key,0);
}




void cvrtChar(int *a, int c){
	if(sizeof(a)/sizeof(a[0]) > 2){
		printf("erreur, tableau de taille sup à 2");
		return;
	}else{
		switch (c) {
			case 97:
			a[0] = 16;
			a[1] = 0;
			break;
			case 98:
			a[0] = 48;
			a[1] = 0;
			break;
			case 99:
			a[0] = 46;
			a[1] = 0;
			break;
			case 100:
			a[0] = 32;
			a[1] = 0;
			break;
			case 101:
			a[0] = 18;
			a[1] = 0;
			break;
			case 102:
			a[0] = 33;
			a[1] = 0;
			break;
			case 103:
			a[0] = 34;
			a[1] = 0;
			break;
			case 104:
			a[0] = 35;
			a[1] = 0;
			break;
			case 105:
			a[0] = 23;
			a[1] = 0;
			break;
			case 106:
			a[0] = 36;
			a[1] = 0;
			break;
			case 107:
			a[0] = 37;
			a[1] = 0;
			break;
			case 108:
			a[0] = 38;
			a[1] = 0;
			break;
			case 109:
			a[0] = 39;
			a[1] = 0;
			break;
			case 110:
			a[0] = 49;
			a[1] = 0;
			break;
			case 111:
			a[0] = 24;
			a[1] = 0;
			break;
			case 112:
			a[0] = 25;
			a[1] = 0;
			break;
			case 113:
			a[0] = 31;
			a[1] = 0;
			break;
			case 114:
			a[0] = 19;
			a[1] = 0;
			break;
			case 115:
			a[0] = 31;
			a[1] = 0;
			break;
			case 116:
			a[0] = 20;
			a[1] = 0;
			break;
			case 117:
			a[0] = 22;
			a[1] = 0;
			break;
			case 118:
			a[0] = 47;
			a[1] = 0;
			break;
			case 119:
			a[0] = 44;
			a[1] = 0;
			break;
			case 120:
			a[0] = 45;
			a[1] = 0;
			break;
			case 121:
			a[0] = 21;
			a[1] = 0;
			break;
			case 122:
			a[0] = 17;
			a[1] = 0;
			break;
			case 65:
			a[0] = 16;
			a[1] = 42;
			break;
			case 66:
			a[0] = 48;
			a[1] = 42;
			break;
			case 67:
			a[0] = 46;
			a[1] = 42;
			break;
			case 68:
			a[0] = 32;
			a[1] = 42;
			break;
			case 69:
			a[0] = 18;
			a[1] = 42;
			break;
			case 70:
			a[0] = 33;
			a[1] = 42;
			break;
			case 71:
			a[0] = 34;
			a[1] = 42;
			break;
			case 72:
			a[0] = 35;
			a[1] = 42;
			break;
			case 73:
			a[0] = 23;
			a[1] = 42;
			break;
			case 74:
			a[0] = 36;
			a[1] = 42;
			break;
			case 75:
			a[0] = 37;
			a[1] = 42;
			break;
			case 76:
			a[0] = 38;
			a[1] = 42;
			break;
			case 77:
			a[0] = 39;
			a[1] = 42;
			break;
			case 78:
			a[0] = 49;
			a[1] = 42;
			break;
			case 79:
			a[0] = 24;
			a[1] = 42;
			break;
			case 80:
			a[0] = 25;
			a[1] = 42;
			break;
			case 81:
			a[0] = 31;
			a[1] = 42;
			break;
			case 82:
			a[0] = 19;
			a[1] = 42;
			break;
			case 83:
			a[0] = 31;
			a[1] = 42;
			break;
			case 84:
			a[0] = 20;
			a[1] = 42;
			break;
			case 85:
			a[0] = 22;
			a[1] = 42;
			break;
			case 86:
			a[0] = 47;
			a[1] = 42;
			break;
			case 87:
			a[0] = 44;
			a[1] = 42;
			break;
			case 88:
			a[0] = 45;
			a[1] = 42;
			break;
			case 89:
			a[0] = 21;
			a[1] = 42;
			break;
			case 90:
			a[0] = 17;
			a[1] = 42;
			break;
			default:
				a[0] = 57;
				a[1] =0;
		}
	}
}

void writeChar(char c){
	int a[2];
	cvrtChar(a,c);
	send_a_button(a[0],a[1]);
}

void loadRandom(double stat[]){
    FILE * fp;
    char * line = NULL;
		char ** ptr;
    size_t len = 0;
    ssize_t read;
    fp = fopen(name_conf, "r");
    if (fp == NULL){
			printf("No file found");
		}else{
			int i=0;
	    while ((read = getline(&line, &len, fp)) != -1) {
				stat [i] = strtod(line,ptr);
				i++;
	    }
		}
    fclose(fp);
    if (line)
        free(line);
}

double box_muller(double mean, double sig){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	srand((time_t)ts.tv_nsec);
	double u = rand()/(double)RAND_MAX;
	//printf("u : %f\n",u);
	double v = rand()/(double)RAND_MAX;
	//printf("v : %f\n",v);
	double x = sqrt(-2.0*log(u))*cos(2.0*M_PI*v);
	return mean+x*sig;
}


void writeArray(char array[], int size, double r[]){
	int i;
	loadRandom(r);
	double t = 0;
	for(i=0; i<=size; i++){
			writeChar(array[i-1]);
			double t = box_muller(r[0],r[1]);
			printf("%f\n",t);
			nanosleep((const struct timespec[]){{0, (int)(1000*t)}}, NULL);
	}

}
/* you have to load uinput into the kernel */
int main(int argc, char *argv[])
{
// error if device not found.
	if (setup_uinput_device() < 0)
	{
		printf("Unable to find uinput device\n");
		return -1;
	}

double r[2];
writeArray(argv[1], strlen(argv[1]),r);
int i;

/* Destroy the input device */
ioctl(uinp_fd, UI_DEV_DESTROY);
/* Close the UINPUT device */
close(uinp_fd);
}
