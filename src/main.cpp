#include "main.hpp"

bool logging::verbose = false;
bool dict::whitelist = false;
std::string dict::whitefile;
bool dict::blacklist = false;
std::string dict::blackfile;
bool dict::other = false;
std::string dict::otherfile;
std::string lang = "en";
std::string layout = "en";
std::string browser = "firefox";
//std::string url = "http://www.google.com";
//std::string url = "http://arstechnica.com/";
//std::string url = "http://dahunicorn.xyz/";
std::string url = "http://www.wikipedia.org/wiki/Special:Random";


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
		if(line[0] != '#' && line[0] != '\n' && line.length() > 0) {
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

bool parse_arguments(int argc, char **argv)
{
	int character;
	bool flag = true;
	while(1) {
		int option_index = 0;
		/* Listing options */
		static struct option long_options[] = {
			/* name, has_arg, flag, val */
			{"help", no_argument, 0, 'h'},
			{"config", no_argument, 0, 0},
			{"language", required_argument, 0, 0},
			{"url", required_argument, 0, 0},
			{"timedkey", no_argument,0,'k'},
			{"verbose", no_argument, 0, 0},
			{"dict", required_argument, 0, 0},
			/* That last line is necessary, but useless. */
			{0,0,0,0}
		};

		character = getopt_long(argc, argv, "hk", long_options, &option_index);
		if(character == -1){
			return true;
		}
		switch (character) {
			case 0:
				if(long_options[option_index].name == "config"){
					flag = false;
					system("vim config.conf");
				}
				else if(long_options[option_index].name == "language"){
					lang = optarg;
				}else if(long_options[option_index].name == "url"){
					url = optarg;
				}else if(long_options[option_index].name == "timedkey"){
					ask_keystrokes();
					flag = false;
				}else if(long_options[option_index].name == "verbose"){
					logging::verbose = true;
					logging::vout("Verbose is active");
				}else if(long_options[option_index].name == "dict"){
					if(!strcmp(optarg,"whitelist")){
						logging::vout("Using whitelist");
						dict::whitelist = true;
						dict::whitefile = "./dictionaries/whitelist.txt";
					}else if(!strcmp(optarg,"whitelist")){
						logging::vout("Using blacklist");
						dict::blacklist = true;
						dict::blackfile = "./dictionaries/blacklist.txt";
					}else{
						logging::vout("Using list");
						dict::other = true;
						dict::otherfile = optarg;
					}
				}
				break;
			case 'h':
				print_help();
				flag = false;
				break;
			case 'k':
			{
				ask_keystrokes();
				std::string word = "cool un Test";
				double r[2];
				write_array(const_cast<char*>(word.c_str()),word.length());
				flag = false;
				break;
			}
			case '?':
				/* character not in the optstring.
				(3rd arg of getopt_long, where we put short options) */
				print_help();
				flag = false;
				break;
			default:
				logging::verr("Argument parsing error");
				flag = false;
		}
	}

	if(optind < argc) {
		std::cerr << "non-option ARGV-elements: " << std::endl;
		while (optind < argc)
			std::cerr << argv[optind++] << std::endl;
	}
	return flag;
}

int main(int argc, char **argv)
{
	parse_config();
	if(!parse_arguments(argc, argv))
		return 0;
	// setup_uinput_device();
	Intelligence intel(url);
	intel.roam();
	// destroy_uinput_device();
	logging::vout("Program finished");
	return 0;
}
