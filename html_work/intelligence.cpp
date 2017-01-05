#include "intelligence.hpp"

Intelligence::Intelligence(Navigator &nav,std::string &start_url)
{
	logging::vout("Creating Intelligence object");
	this->current_url = start_url;
	this->navigator = nav;
	this->load_blacklist();
	/* Blacklist content
	for(auto const& line: this->blacklist) {
		 std::cout << line << std::endl;
	}*/
	srand(time(NULL));
}

Intelligence::~Intelligence() {
	this->dump_blacklist();
	logging::vout("Deleting Intelligence object");
}

void Intelligence::load_blacklist() {
	std::ifstream file("dictionaries/blacklist.txt");
	std::string line;
	std::string file_contents;
	while (std::getline(file, line))
	  this->blacklist.push_back(line);
}
void Intelligence::dump_blacklist() {
	std::ofstream blacklist;
	blacklist.open("dictionaries/blacklist.txt", std::ofstream::out | std::ofstream::trunc);
	for(auto const& value: Intelligence::blacklist) {
     /* std::cout << value; ... */
		 blacklist << value+"\n";
	 }
	blacklist.close();
}

void Intelligence::roam()
{
	std::string page_html;
	std::vector<HyperLink> links;
	std::vector<std::string> whiteList = initWhiteList("./word_list/word_list.txt");
	HyperLink link;
	int x = 0;
	this->current_url = this->navigator.navigate(this->current_url);
	do {
		page_html = this->navigator.get_body_html();
		this->navigator.select_hyperlinks_from_html(page_html, links);
		this->current_url = select_from_word_list(links,this->current_url,whiteList).url;
		std::string navigate_res = this->navigator.navigate(this->current_url);
		if(navigate_res == "failed") {
			blacklist.push_back(this->current_url);
			this->current_url = select_from_word_list(links,this->current_url,whiteList).url;
		} else {
			this->current_url = navigate_res;
		}
	} while(x++ <= 20);
}

HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
		int random;
		if(links.size() <= 1) {
			random = 0;
		} else {
			random = (int)(std::rand() % links.size());
		}
    HyperLink link = links.at(random);
    return link;
}

HyperLink select_diff_random_in_vector(std::vector<HyperLink> &links,std::string url)
{
	HyperLink link;
	do  {
		link = select_random_in_vector(links);
	} while (link.url == url);
	return link;
}

HyperLink select_from_word_list(std::vector<HyperLink> &links,std::string url, std::vector<std::string> whiteList)
{
	HyperLink link;
	std::string line;
	std::string text;
	bool not_in_link = true;
	int c = 0;
	link = select_random_in_vector(links);
	do {
		link = select_diff_random_in_vector(links, link.url);
		for(int i=0; i<whiteList.size(); i++) {
			text = " "+link.text+" ";
			line = whiteList[i];
			if(text.find(" "+line+" ") != std::string::npos) {
				not_in_link = false;
				std::cout << "#LIEN VALIDE : " << line << std::endl;
				std::cout << "TXT == " << link.text << std::endl;
				std::cout << "URL == " << link.url << std::endl;
			}
		}
	} while(not_in_link && c++<50);
	if(c==50)
		link.url = url;
	return link;
}

std::vector<std::string> initWhiteList(std::string name) {
	std::string line;
	std::ifstream file(name);
	std::vector<std::string> whiteList;
	if(file) {
		while(std::getline(file, line)) {
			whiteList.push_back(line);
		}
	} else {
		logging::vout("Impossible d'ouvrir le fichier.");
	}
	for(int i=0; i<whiteList.size(); i++) {
		std::cout << whiteList[i] << std::endl;
	}
	file.close();
	return whiteList;
}

void add_to_blacklist(std::string wrong_url) {
	std::cout << "TODO";
}
