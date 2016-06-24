#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *lang = "en";
char *layout = "en";
char *browser = "firefox";
bool visible = true;

void printHelp() {
	printf("Yet to be done");
}

void getConfig() {
	FILE *configFile = fopen("config.conf", "r");
	
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while((read = getline(&line, &len, configFile)) != -1) {
		if(line[0] != '#' && line[0] != '\n') {
			char *var = strtok(line, "=");
			if(strcmp(var,"lang") == 0) {
				lang = strtok(NULL, "=");
			} else if(strcmp(var,"layout") == 0) {
				layout = strtok(NULL, "=");
			} else if(strcmp(var,"browser") == 0) {
				browser = strtok(NULL, "=");
			} else if(strcmp(var,"visible") == 0) {
				if(strcmp(strtok(NULL, "="),"true") == 0)
					visible = true;
				else visible = false;
			} else {	
				printf("Mistake on this line : %s\n", line);
			}
		}
	}

	fclose(configFile);
}

int main(int argc, char *argv[])  {
	if(argc > 1) {
		if(strcmp(argv[1], "--config") == 0) {
			system("vim config.conf");
		} else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			printHelp();
		}
	} else {
		getConfig();
	}
	// char *lang = getenv("LANG");
	// printf("lang : %s, layout : %s, browser : %s", lang, layout, browser);
	return 0;
}
