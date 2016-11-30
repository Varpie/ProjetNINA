#include "main.h"
#include "html_work/navigator.h"

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
	std::string page_html = get_bodyhtml_from_url(url);
	std::cout << page_html << std::endl;
	return 0;
}
