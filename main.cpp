#include "main.h"

std::string lang = "en";
std::string layout = "en";
std::string browser = "firefox";
std::string url = "http://www.google.com";

void print_help()
{
	printf("Yet to be done\n");
}

void parse_config()
{
	std::ifstream configFile("config.conf");
	std::string line = "";
	int i = 0;
	/* Reading file line by line */
	while((std::getline(configFile, line))) {
		i++;
		/* Removing commented and empty lines */
		if(line[0] != '#' && line[0] != '\n') {
			/* Splitting lines at '=' character */
			std::string var = line.substr(0, line.find("="));
			std::string value = line.substr(var.length()+1);

			 if(var == "lang") {
			 	/* Getting value */
				lang = value;
			} else if(var == "layout") {
				layout = value;
			} else if(var == "browser") {
				browser = value;
			} else {
			 	std::cout << "Mistake on line " << i << ": " << line << std::endl;
			}
		}
	}
}

void parse_arguments(int argc, char **argv)
{
	int character;

	while(1) {
		int option_index = 0;
		/* Listing options */
		static struct option long_options[] = {
			/* name, has_arg, flag, val */
			{"help", no_argument, 0, 'h'},
			{"config", no_argument, 0, 0},
			{"language", required_argument, 0, 0},
			{"url", required_argument, 0, 0},
			/* That last line is necessary, but useless. */
			{0,0,0,0}
		};

		character = getopt_long(argc, argv, "h", long_options, &option_index);
		if(character == -1)
			break;
		switch (character) {
			case 0:
				if(long_options[option_index].name == "config")
					system("vim config.conf");
				else if(long_options[option_index].name == "language")
					lang = optarg;
				else if(long_options[option_index].name == "url")
					url = optarg;
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

void exec_pycode(std::string url)
{
	Py_Initialize();
	PyRun_SimpleString("from selenium import webdriver");
	PyRun_SimpleString("driver = webdriver.Firefox()");
	url = "driver.get('" + url + "')";
	PyRun_SimpleString(url.c_str());
	Py_Finalize();
}

int main(int argc, char **argv)
{
	parse_config();
	parse_arguments(argc, argv);
	exec_pycode(url);
	return 0;
}


/*Prend un url et fait appel a la spider get_html.py*/
std::string  get_bodyhtml_from_url(std::string url) {
    std::string resultat;
    PyObject *retour, *module, *fonction, *arguments;

    Py_Initialize();
    //PySys_SetPath(".");
    if(PyRun_SimpleString("import sys;sys.path.insert(0, '.')")) {
    	std::string error;
        error = "path expansion failed\n";
        return error;
    }

    module = PyImport_ImportModule("get_html");
    if(module == NULL) {
    	std::string error;
        error = "import failed\n";
        PyErr_Print();
        return error;
    }

    fonction = PyObject_GetAttrString(module, "runspider_with_url");
    if(fonction == NULL) {
    	std::string error;
        error = "could not find function\n";
        return error;
    }

    arguments = Py_BuildValue("(s)", url);
    if(arguments == NULL) {
    	std::string error;
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

int select_hyperlinks_from_html(std::string html,HyperLink *links) {
	int cpt= 0,ihr=0,itx=0;
	char in_tag_a=0,in_href=0,in_txt=0;

	for(int i=0;i<html.length();i++) {
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
