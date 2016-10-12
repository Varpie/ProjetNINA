#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
/* Globals */
static int uinp_fd = -1;

struct uinput_user_dev uinp; /* uInput device structure */
struct input_event event; /* Input device structure */

/* Setup the uinput device */
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
	strncpy(uinp.name, "PolyVision Touch Screen", UINPUT_MAX_NAME_SIZE);
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
	if(modifier = 0)
	{
	press_a_button(key);
	//release_a_button(key);
}else{
	press_a_button(modifier);
	press_a_button(key);
	release_a_button(key);
	release_a_button(modifier);
}

}


int cvrtChar(int c){
	switch (c) {
		case 97: return 16;
		case 98: return 48;
		case 99: return 46;
		case 100: return 32;
		case 101: return 18;
		case 102: return 33;
		case 103: return 34;
		case 104: return 35;
		case 105: return 23;
		case 106: return 36;
		case 107: return 37;
		case 108: return 38;
		case 109: return 39;
		case 110: return 49;
		case 111: return 24;
		case 112: return 25;
		case 113: return 31;
		case 114: return 19;
		case 115: return 31;
		case 116: return 20;
		case 117: return 22;
		case 118: return 47;
		case 119: return 44;
		case 120: return 45;
		case 121: return 21;
		case 122: return 17;
		default: return 57;
	}
}

void writeArray(char array[], int size){

	int i;
	for(i =0; i<size-1; i++){
			send_a_button(cvrtChar(array[i]),42);
			nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
	}

}
/* This function will open the uInput device. Please make
sure that you have inserted the uinput.ko into kernel. */
int main()
{
// Return an error if device not found.
	if (setup_uinput_device() < 0)
	{
		printf("Unable to find uinput device\n");
		return -1;
	}

char mot[] = " minuscule";
int size = sizeof(mot)/sizeof(mot[0]);

int i = 0;
while(i<5){
	writeArray(mot, size);
	i++;
}


/* Destroy the input device */
ioctl(uinp_fd, UI_DEV_DESTROY);
/* Close the UINPUT device */
close(uinp_fd);
}
