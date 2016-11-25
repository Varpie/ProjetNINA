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

void parse_arguments(int argc, char **argv) {
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
}

void exec_pycode() {
	FILE* python_file = fopen("selenium/main.py", "r");
	Py_Initialize();
	PyRun_SimpleFile(python_file, "main.py");
	Py_Finalize();
}

int main(int argc, char **argv)  {
	parse_config();
	parse_arguments(argc, argv);
	exec_pycode();
	// char *lang = getenv("LANG");
	return 0;
}


/*Prend un url et fait appel a la spider get_html.py*/
char * get_bodyhtml_from_url(char *url) {
    char *resultat;
    PyObject *retour, *module, *fonction, *arguments;

    Py_Initialize();
    //PySys_SetPath(".");
    if(PyRun_SimpleString("import sys;sys.path.insert(0, '.')")) {
    	char *error;
        error = "path expansion failed\n";
        return error;
    }

    module = PyImport_ImportModule("get_html");
    if(module == NULL) {
    	char *error;
        error = "import failed\n";
        PyErr_Print();
        return error;
    }

    fonction = PyObject_GetAttrString(module, "runspider_with_url");
    if(fonction == NULL) {
    	char *error;
        error = "could not find function\n";
        return error;
    }

    arguments = Py_BuildValue("(s)", url);
    if(arguments == NULL) {
    	char *error;
        error = "arg parsing failed\n";
        return error;
    }

    printf("Calling\n");
    retour = PyEval_CallObject(fonction, arguments);
    printf("Returned\n");

    // note: need to release arguments
    Py_DECREF(arguments);

    if(retour == NULL) {
        printf("It all went wrong\n");
        PyErr_Print();
        return url;
    }

    PyArg_Parse(retour, "s", &resultat);

    Py_Finalize();
    return resultat;
}

int select_hyperlinks_from_html(char *html,HyperLink *links) {
	int cpt= 0,ihr=0,itx=0;
	char in_tag_a=0,in_href=0,in_txt=0;

	for(int i=0;i<strlen(html);i++) {
		if(html[i-3] == '<' && html[i-2] == 'a' && html[i-1] == ' ') {
			in_tag_a = 1;
		}

		if(in_tag_a) {
			if(html[i-6]=='h' && html[i-5]=='r' && html[i-4]=='e' && html[i-3]=='f'
			&& html[i-2]=='=' && html[i-1]=='"') {
				in_href=1;
			}
			if(in_href) {
				links[cpt].url[ihr]=html[i];
				ihr++;
				if(html[i+1]=='"') {
					in_href = 0;
					links[cpt].url[ihr+1]='\0';
					ihr=0;
				}
			}
			if(html[i-1]=='>') {
				in_txt = 1;
			}
			if(in_txt) {
				links[cpt].text[itx]=html[i];
				itx++;
				if(html[i+1]=='<' && html[i+2]=='/' && html[i+3]=='a'
				&& html[i+4]=='>') {
					in_txt=0;
					links[cpt].text[itx+1]='\0';
					itx=0;
				}
			}
		}
		if(html[i+1] == '<' && html[i+2] == '/' && html[i+3] == 'a' ) {
			cpt++;
			in_tag_a = 0;
		}
	}
	return cpt;
}
