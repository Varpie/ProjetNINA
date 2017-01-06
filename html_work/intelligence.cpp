#include "intelligence.hpp"

Intelligence::Intelligence(Navigator &nav,std::string &start_url)
{
	logging::vout("Creating Intelligence object");
	this->current_url = start_url;
	this->navigator = nav;
	/* this->load_blacklist(); */
	/* Blacklist content
	for(auto const& line: this->blacklist) {
		 std::cout << line << std::endl;
	}*/
	srand(time(NULL));
}

Intelligence::~Intelligence() {
	/* this->dump_blacklist(); */
	logging::vout("Deleting Intelligence object");
}

/*void Intelligence::load_blacklist() {
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
     /* std::cout << value; ... *//*
		 blacklist << value+"\n";
	 }
	blacklist.close();
}*/

void Intelligence::roam()
{
	std::string page_html;
	std::vector<HyperLink> links;
	std::vector<std::string> whitelist;
	if(dict::whitelist){
		logging::vout("Load whitelist");
		whitelist = init_list(dict::whitefile);
	}
	if(dict::blacklist){
		logging::vout("Load blacklist");
		blacklist = init_list(dict::blackfile);
	}
	if(dict::other){
		logging::vout("Load list");
		otherlist = init_list(dict::otherfile);
	}
	HyperLink link;
	int x = 0;
	this->current_url = this->navigator.navigate(this->current_url);
	do {
		page_html = this->navigator.get_body_html();
		this->navigator.select_hyperlinks_from_html(page_html, links);
		if(dict::whitelist)
			this->current_url = select_whitelist(links,this->current_url,whitelist).url;
		else if(dict::blacklist)
			this->current_url = select_blacklist(links,this->current_url,blacklist).url;
		else if(dict::other)
			this->current_url = select_blacklist(links,this->current_url,otherlist).url;
		else
			this->current_url = select_diff_random_in_vector(links,this->current_url).url;
		std::string navigate_res = this->navigator.navigate(this->current_url);
		if(navigate_res == "failed") {
			blacklist.push_back(this->current_url);
			if(dict::whitelist)
				this->current_url = select_whitelist(links,this->current_url,whitelist).url;
			else if(dict::blacklist)
				this->current_url = select_blacklist(links,this->current_url,blacklist).url;
			else
				this->current_url = select_diff_random_in_vector(links,this->current_url).url;
		} else {
			this->current_url = navigate_res;
		}
	} while(x++ <= 5);
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

HyperLink select_whitelist(std::vector<HyperLink> &links,std::string url, std::vector<std::string> whitelist)
{
	HyperLink link;
	std::string line;
	std::string text;
	bool not_in_link = true;
	int c = 0;
	link = select_random_in_vector(links);
	do {
		link = select_diff_random_in_vector(links, link.url);
		for(int i=0; i<whitelist.size(); i++) {
			text = " "+link.text+" ";
			line = whitelist[i];
			if(text.find(" "+line+" ") != std::string::npos) {
				not_in_link = false;
			}
		}
	} while(not_in_link && c++<50);
	if(c==50)
		link.url = url;
	return link;
}

HyperLink select_blacklist(std::vector<HyperLink> &links,std::string url, std::vector<std::string> blacklist)
{
	HyperLink link;
	std::string line;
	std::string text;
	bool not_in_link = true;
	int c = 0;
	link = select_random_in_vector(links);
	do {
		link = select_diff_random_in_vector(links, link.url);
		for(int i=0; i<blacklist.size(); i++) {
			text = " "+link.text+" ";
			line = blacklist[i];
			if(text.find(" "+line+" ") != std::string::npos) {
				not_in_link = false;
			}
		}
	} while(not_in_link && c++<50);
	if(c==50)
		link.url = url;
	return link;
}

std::vector<std::string> init_list(std::string name) {
	std::string line;
	std::ifstream file(name);
	std::vector<std::string> list;
	if(file) {
		while(std::getline(file, line)) {
			list.push_back(line);
		}
	} else {
		logging::vout("Impossible d'ouvrir le fichier.");
	}
	file.close();
	return list;
}
/*
tuple_list<int,std::string> init_otherlist(std::string name) {
	std::string line;
	int value;
	std::string word;
	std::ifstream file(name);
	tuple_list<int,std::string> list;
	if(file) {
		while(std::getline(file, line)) {
			value = line.substr(0, line.find(";"));
			word = line.substr(value.length()+1);
			list.push_back(tuple<int,std::string> (value,word))
		}
	} else {
		logging::vout("Impossible d'ouvrir le fichier.");
	}
	file.close();
	return list;
}*/
/*
void add_to_blacklist(std::string wrong_url) {
	std::cout << "TODO";
}
*/
