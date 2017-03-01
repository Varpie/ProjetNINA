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
long timer;
bool countdown::links = false;
long countdown::number;
std::string lang = "en";
std::string layout = "en";
std::string browser = "firefox";
//std::string url = "http://www.google.com";
//std::string url = "http://arstechnica.com/";
//std::string url = "http://fnac.com/";
std::string url = "http://www.wikipedia.org/wiki/Special:Random";
std::atomic<bool> threading::running(true);

void print_help()
{
	std::cout
		<< "Usage : nina [options]" << std::endl << std::endl
		<< "   -h, --help        "
		<< "Afficher l'aide" << std::endl
		<< "       --config      "
		<< "Afficher et modifier le fichier de configuration" << std::endl
		<< "       --url URL     "
		<< "Démarrer depuis URL" << std::endl
		<< "   -k, --timedkey    "
		<< "Je ne sais pas :/" << std::endl
		<< "       --verbose X   "
		<< "Lance l'application avec une verbose de niveau X" << std::endl
		<< "       --whitelist   "
		<< "Démarrer l'application avec la whitelist" << std::endl
		<< "       --blacklist   "
		<< "Démarrer l'application avec la whitelist" << std::endl
		<< "       --otherlist   "
		<< "Démarrer l'application avec la liste personnelle" << std::endl
		<< "   -d, --daemonize   "
		<< "Détache l'application du terminal" << std::endl
		<< "   -s, --stop        "
		<< "Stoppe l'execution de l'application" << std::endl
		<< "       --timeout X   "
		<< "Arrête l'application après X secondes d'execution" << std::endl
		<< "       --links X     "
		<< "Arrête l'application après X liens parcourus" << std::endl
	<< std::endl;
}

void daemonize()
{
	pid_t pid;

	/* Fork off the parent process */
	pid = fork();
	logging::vout(1,"forked");
	/* An error occurred */
	if (pid < 0)
		exit(EXIT_FAILURE);

	/* Success: Let the parent terminate */
	if (pid > 0)//queue
		exit(EXIT_SUCCESS);

	/* On success: The child process becomes session leader */
	if (setsid() < 0)
		exit(EXIT_FAILURE);
	logging::vout(1,"succeded");
}

void stop_daemon()
{
	std::ofstream file("/proc/nina");
	if (!file.is_open())
		throw std::runtime_error("Could not open the file");
	file << "kill";
	file.close();
}

void stopping_detection()
{
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

	if((fd = open(MOUSEFILE.c_str(), O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}

	while(read(fd, &ie, sizeof(struct input_event)) && threading::running) {
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

void timeout(long timer)
{
	std::this_thread::sleep_for(std::chrono::seconds(timer));
	logging::vout(2, "Program finished by timeout");
	threading::running = false;
}

void handle_sigquit(int signum)
{
	threading::running = false;
}

void parse_config()
{
	std::string config_path = "/home/" + std::string(getenv("SUDO_USER")) + "/.config/nina.conf";
	std::ifstream configFile(config_path);
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

			if(var == "device") {
				MOUSEFILE = value;
			} else if(var == "links") {
				timer = std::stod(value);
			} else if(var == "time") {
				countdown::links = true;
				countdown::number = std::stod(value);
			} else if(var == "url") {
				url = value;
			} else {
				std::cerr << "Mistake on line " << i << ": " << line << std::endl;
			}
		}
	}
}

bool parse_arguments(int argc, char **argv)
{
	int character;
	while(flag) {
		int option_index = 0;
		/* Listing options */
		static struct option long_options[] = {
			/* name, has_arg, flag, val */
			{"help", no_argument, 0, 'h'},
			{"config", no_argument, 0, 0},
			{"url", required_argument, 0, 0},
			{"timedkey", no_argument,0,'k'},
			{"verbose", required_argument, 0, 0},
			{"whitelist", no_argument, 0, 0},
			{"blacklist", no_argument, 0, 0},
			{"otherlist", no_argument, 0, 0},
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
				if(long_options[option_index].name == "help"){
					flag = false;
					print_help();
				} else if(long_options[option_index].name == "config"){
					flag = false;
					system("vim config.conf");
				} else if(long_options[option_index].name == "url"){
					url = optarg;
				} else if(long_options[option_index].name == "timedkey"){
					ask_keystrokes();
					flag = false;
				} else if(long_options[option_index].name == "verbose"){
					logging::verbose = std::stoi(optarg);
					if(logging::verbose > 0){
						logging::vout("Verbose is active.");
						logging::vout(" Level : " + std::to_string(logging::verbose));
					} else {
						logging::vout("Select a correct verbose level.");
						flag = false;
					}
				} else if(long_options[option_index].name == "whitelist"){
					logging::vout(1,"Using whitelist");
					dict::whitelist = true;
					dict::whitefile = "./config/dictionaries/whitelist.txt";
				} else if(long_options[option_index].name == "blacklist"){
					logging::vout(1,"Using blacklist");
					dict::blacklist = true;
					dict::blackfile = "./config/dictionaries/blacklist.txt";
				} else if(long_options[option_index].name == "otherlist"){
					logging::vout(1,"Using otherlist");
					dict::other = true;
					dict::otherfile = "./config/dictionaries/otherlist.txt";
				} else if(long_options[option_index].name == "daemonize") {
					daemonize();
					logging::vout(1,"Process daemonized");
				} else if(long_options[option_index].name == "stop") {
					stop_daemon();
					flag = false;
				} else if(long_options[option_index].name == "timeout"){
					logging::vout(1,"Using time countdown");
					timer = std::stod(optarg);
				} else if(long_options[option_index].name == "links"){
					logging::vout(1,"Using links countdown");
					countdown::links = true;
					countdown::number = std::stod(optarg);
				}
				break;
			case 'h':
				flag = false;
				print_help();
				break;
			case 'k':
			{
				ask_keystrokes();
				std::string word = "cool un Test";
				double r[2];
				write_string(const_cast<char*>(word.c_str()));
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
	if(getuid()) {
		std::cout << "This program should be run with sudo" << std::endl;
		return 0;
	}
	parse_config();
	if(!parse_arguments(argc, argv)){
		return 0;
	}
	signal(SIGQUIT, handle_sigquit);
	Intelligence intel(url);
	// Creating thread to detect mouse and stop program
	std::thread mouse_thread(stopping_detection);
	mouse_thread.detach();

	// Creating thread calling intel.roam()
	std::thread main_thread(&Intelligence::roam, &intel);

	if(timer) {
		std::thread timeout_thread(timeout,timer);
		timeout_thread.detach();
	}

	main_thread.join();
	logging::vout(1,"Program finished");
	return 0;
}
