#include "main.h"

char *lang = "en\0";
char *layout = "en\0";
char *browser = "firefox\0";
bool visible = true;

void print_help() {
	printf("Yet to be done\n");
}

void parse_config() {
	FILE *configFile = fopen("config.conf", "r");
	
	char *line = NULL;
	size_t len = 100;
	ssize_t read;

	/* Reading file line by line */
	while((read = getline(&line, &len, configFile)) != -1) {
		/* Removing commented and empty lines */
		if(line[0] != '#' && line[0] != '\n') {
			/* Splitting lines at '=' character */
			char *var = strtok(line, "=");
			

			if(strcmp(var,"lang") == 0) {
				/* Duplicating string and assigning it to variable*/
				lang = strdup(strtok(NULL, "="));
				/* Adding \0 char to make a well-made CString */
				lang[strlen(lang)-1] = '\0';
			} else if(strcmp(var,"layout") == 0) {
				layout = strdup(strtok(NULL, "="));
				layout[strlen(layout)-1]= '\0';
			} else if(strcmp(var,"browser") == 0) {
				browser = strdup(strtok(NULL, "="));
				browser[strlen(browser)-1]= '\0';
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
	parse_config();

	int character;
	
	while(1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		/* Listing options */
		static struct option long_options[] = {
			/* name, has_arg, flag, val */
			{"help", no_argument, 0, 'h'},
			{"config", no_argument, 0, 0},
			{"language", required_argument, 0, 0},
			/* That last line is necessary, but useless. */
			{0,0,0,0}
		};

		character = getopt_long(argc, argv, "h", long_options, &option_index);
		if(character == -1)
			break;
		switch (character) {
			case 0:
				if(strcmp(long_options[option_index].name, "config") == 0)
					system("vim config.conf");
				else if(strcmp(long_options[option_index].name, "language") == 0)
					lang = optarg;				
				break;
			case 'h':
				print_help();
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

	printf("lang : %s\nlayout : %s\nbrowser : %s", lang, layout, browser);

	while(1) {
			sleep(1);
	}
	printf("test");
	// char *lang = getenv("LANG");
	return 0;
}
