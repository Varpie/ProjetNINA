#include "intelligence.hpp"

Intelligence::Intelligence(std::string &start_url)
{
	logging::vout("Creating Intelligence object");
	this->current_url = start_url;
	this->navigator = new Navigator();
	srand(time(NULL));
}

Intelligence::~Intelligence() {
	/* important, not destroyed dynamically */
	delete(this->navigator);
	logging::vout("Deleting Intelligence object");
}

void Intelligence::roam()
{
	std::string page_html;
	std::vector<HyperLink> links;
	std::vector<std::string> whitelist;
	std::vector<std::string> blacklist;
	tuple_list otherlist;
	time_t begin,end;
	long timeout = timeout::time;
	std::vector<std::string> keywords = init_list("./config/dictionaries/whitelist.txt");
	if(keywords.size() == 0) {
		logging::vout("Keywords load failed");
	}
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
		otherlist = init_otherlist(dict::otherfile);
	}
	logging::vout("Program began");
	HyperLink link;
	int x = 0;
	bool timer = false;
	bool overflow = false;
	this->current_url = this->navigator->navigate(this->current_url);
	do {
		time(&begin);
		page_html = this->navigator->get_body_html();
		this->navigator->select_hyperlinks_from_html(page_html, links);
		if(dict::whitelist)
			this->current_url = select_whitelist(links,this->current_url,whitelist).url;
		else if(dict::blacklist)
			this->current_url = select_blacklist(links,this->current_url,blacklist).url;
		else if(dict::other)
			this->current_url = select_otherlist(links,this->current_url,otherlist).url;
		else
			this->current_url = select_diff_random_in_vector(links,this->current_url).url;
		std::string navigate_res = this->navigator->navigate(this->current_url);
		if(navigate_res == "failed") {
		// 	if(dict::whitelist)
		// 		this->current_url = select_whitelist(links,this->current_url,whitelist).url;
		// 	else if(dict::blacklist)
		// 		this->current_url = select_blacklist(links,this->current_url,blacklist).url;
		// 	else
		// 		this->current_url = select_diff_random_in_vector(links,this->current_url).url;
			std::string kw = select_keyword(keywords);
			this->current_url = this->navigator->write_search(kw);
		} else {
			this->current_url = navigate_res;
		}
		time(&end);
		timeout -= (long)difftime(end,begin);
		if(timeout::timeout) {
			logging::vout("Countdown : " + std::to_string(timeout));
		}
		timer = (timeout::timeout && (timeout > 0));
		overflow = (!timeout::timeout && x++<10);
	} while(timer || overflow);
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
				logging::vout("--White : " + line);
				logging::vout("--Texte : " + link.text);
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
				logging::vout("--Black : " + line);
				logging::vout("--Texte : " + link.text);
				not_in_link = false;
			}
		}
	} while(not_in_link && c++<50);
	if(c==50)
		link.url = url;
	return link;
}

HyperLink select_otherlist(std::vector<HyperLink> &links,std::string url, tuple_list list)
{
	HyperLink link;
	std::string line;
	std::string text;
	bool test = true;
	int c = 0;
	link = select_random_in_vector(links);
	do {
		link = select_diff_random_in_vector(links, link.url);
		for(int i=0; i<list.size(); i++) {
			text = " "+link.text+" ";
			line = std::get<1>(list[i]);
			if((text.find(" "+line+" ")!=std::string::npos)&&(std::get<0>(list[i])==1)) {
				test = false;
			}
			else if(std::get<0>(list[i])==0){
				test = false;
			}
		}
	} while(test && c++<50);
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

tuple_list init_otherlist(std::string name) {
	std::string line;
	int value;
	std::string word;
	std::ifstream file(name);
	tuple_list list;
	if(file) {
		while(std::getline(file, line)) {
			value = std::stoi(line.substr(0, line.find(";")));
			word = line.substr(2);
			list.emplace_back(std::tuple<int,std::string> (value,word));
		}
	} else {
		logging::vout("Impossible d'ouvrir le fichier.");
	}
	file.close();
	return list;
}

std::string select_keyword(std::vector<std::string> list)
{
	int random;
	if(list.size() <= 1) {
		random = 0;
	} else {
		random = (int)(std::rand() % list.size());
	}
	std::string res = list.at(random);
	return res;
}
