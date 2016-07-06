#include "main.h"

char *lang = "en\0";
char *layout = "en\0";
char *browser = "firefox\0";
bool visible = true;

void printHelp() {
	printf("Yet to be done\n");
}

void getConfig() {
	FILE *configFile = fopen("config.conf", "r");
	
	char *line = NULL;
	size_t len = 100;
	ssize_t read;

	while((read = getline(&line, &len, configFile)) != -1) {
		if(line[0] != '#' && line[0] != '\n') {
			char *var = strtok(line, "=");
			
			if(strcmp(var,"lang") == 0) {
				lang = strdup(strtok(NULL, "="));
			} else if(strcmp(var,"layout") == 0) {
				layout = strdup(strtok(NULL, "="));
			} else if(strcmp(var,"browser") == 0) {
				browser = strdup(strtok(NULL, "="));
			} else if(strcmp(var,"visible") == 0) {
	//			if(strcmp(strtok(NULL, "="),"true") == 0)
	//				visible = true;
	//			else visible = false;
			} else {	
				printf("Mistake on this line : %s\n", line);
			}
		}
	}

	fclose(configFile);
}

int main(int argc, char **argv)  {
	getConfig();

	printf("lang : %s\nlayout : %s\nbrowser : %s", lang, layout, browser);
	int c;
	
	while(1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		/* Listing options */
		static struct option longOptions[] = {
			/* name, has_arg, flag, val */
			{"help", no_argument, 0, 'h'},
			{"config", no_argument, 0, 0},
			{"language", required_argument, 0, 0},
			/* That last line is necessary, but useless. */
			{0,0,0,0}
		};

		c = getopt_long(argc, argv, "h", longOptions, &option_index);
		if(c == -1)
			break;
		switch (c) {
			case 0:
				if(strcmp(longOptions[option_index].name, "config") == 0)
					system("vim config.conf");
				else if(strcmp(longOptions[option_index].name, "language") == 0)
					lang = optarg;				
				break;
			case 'h':
				printHelp();
				break;
			case '?':
				/* character not in the optstring. 
				   (3rd arg of getopt_long, where we put short options) */
				break;
			default:
				printf("Uhh-uhh.");
		}
	}


	if(optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s", argv[optind++]);
		printf("\n");
	}
	// char *lang = getenv("LANG");
	return 0;
}
