#include "main.hpp"
bool flag = true;
static int forked_pid = 0;
int logging::verbose = 0;
bool dict::whitelist = false;
std::string dict::whitefile;
bool dict::blacklist = false;
std::string dict::blackfile;
bool dict::other = false;
std::string dict::otherfile;
bool countdown::timeout = false;
long countdown::time;
bool countdown::links = false;
long countdown::number;
std::string lang = "en";
std::string layout = "en";
std::string browser = "firefox";
//std::string url = "http://www.google.com";
//std::string url = "http://arstechnica.com/";
//std::string url = "http://dahunicorn.xyz/";
std::string url = "http://www.wikipedia.org/wiki/Special:Random";
std::atomic<bool> threading::running(true);


void print_help()
{
	printf("Yet to be done\n");
}

void daemonize()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();
	logging::vout("forked");
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
	logging::vout("succeded");
}

void stop_daemon() {
	std::ofstream file("/proc/nina");
	if (!file.is_open())
	    throw std::runtime_error("Could not open the file");
	file << "kill";
	file.close();
}

void stopping_detection() {
	int fd;
	char pos = 0;
	struct input_event ie;
	Display *dpy;
	Window root, child;
	int rootX, rootY, winX, winY, dispX, dispY;
	unsigned int mask;

	dpy = XOpenDisplay(NULL);
	XQueryPointer(dpy,DefaultRootWindow(dpy),&root,&child,
	          &rootX,&rootY,&winX,&winY,&mask);
	dispY = DefaultScreenOfDisplay(dpy)->height;
	dispX = DefaultScreenOfDisplay(dpy)->width;

	if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}

	while(read(fd, &ie, sizeof(struct input_event)) && threading::running) {
		if (ie.type == EV_ABS) {
			XQueryPointer(dpy,DefaultRootWindow(dpy),&root,&child,
					&rootX,&rootY,&winX,&winY,&mask);
			if(pos == 1 && rootX == 0 && rootY == 0) {
				logging::vout(2,"Top left corner");
				pos++;
			} else if(pos == 2 && rootX == 0 && rootY == dispY -1) {
				pos++;
				logging::vout(2,"Bottom left corner");
			} else if(pos == 0 && rootX == dispX - 1 && rootY == 0) {
				pos++;
				logging::vout(2,"Top right corner");
			} else if(pos == 3 && rootX == dispX-1 && rootY == dispY-1) {
				logging::vout(2,"Bottom right corner");
				threading::running = false;
			}
		}
	}
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
			 	std::cout << "Mistake on line " << i << ": " << line << std::endl;
			}
		}
	}
}

bool parse_arguments(int argc, char **argv)
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
			{"timedkey", no_argument,0,'k'},
			{"verbose", required_argument, 0, 0},
			{"whitelist", no_argument, 0, 0},
			{"blacklist", no_argument, 0, 0},
			{"daemonize", no_argument, 0, 'd'},
			{"stop", no_argument, 0, 's'},
			{"timeout", required_argument, 0, 0},
			{"links", required_argument, 0, 0},
			/* That last line is necessary, but useless. */
			{0,0,0,0}
		};

		character = getopt_long(argc, argv, "hkds", long_options, &option_index);
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
					logging::verbose = std::stoi(optarg);
					logging::vout("Verbose is active");
				} else if(long_options[option_index].name == "whitelist"){
					logging::vout("Using whitelist");
					dict::whitelist = true;
					dict::whitefile = "./config/dictionaries/whitelist.txt";
				} else if(long_options[option_index].name == "blacklist"){
					logging::vout("Using blacklist");
					dict::blacklist = true;
					dict::blackfile = "./config/dictionaries/blacklist.txt";
				} else if(long_options[option_index].name == "daemonize") {
					daemonize();
					logging::vout("Process daemonized");
				} else if(long_options[option_index].name == "stop") {
					stop_daemon();
					flag = false;
				}else if(long_options[option_index].name == "timeout"){
					logging::vout("Using time countdown");
					countdown::timeout = true;
					countdown::time = std::stod(optarg);
				}else if(long_options[option_index].name == "links"){
					logging::vout("Using links countdown");
					countdown::links = true;
					countdown::number = std::stod(optarg);
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
			case 'd':
				daemonize();
				break;
			case 's':
				stop_daemon();
				flag = false;
				break;
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
	Intelligence intel(url);
	// Creating thread to detect mouse and stop program
	std::thread t1(stopping_detection);
	// Creating thread calling intel.roam()
	std::thread t2(&Intelligence::roam, &intel);

	t1.join();
	t2.join();
	logging::vout("Program finished");
	return 0;
}
