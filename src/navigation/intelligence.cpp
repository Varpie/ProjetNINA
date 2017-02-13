#include "intelligence.hpp"

Intelligence::Intelligence(std::string &start_url)
{
	logging::vout("Creating Intelligence object");
	this->current_url = start_url;
	this->navigator = new Navigator();
	srand(time(NULL));
	load_lists();
}

Intelligence::~Intelligence()
{
	/* important, not destroyed dynamically */
	delete(this->navigator);
	dump_lists();
	logging::vout("Deleting Intelligence object");
}

void Intelligence::roam()
{
	// for(auto const& lk: links){
	// 	std::cout << lk.url << std::endl;
	logging::vout(2,"Entering Intelligence::roam");
	logging::vout("Program began");
	std::string page_html;
	std::vector<HyperLink> links;
	time_t begin,end;
	HyperLink link;
	std::string navigate_res = "";
	long x = 0;
	bool timer = false;
	bool overflow = false;
	bool search = false;
	logging::vout(3,"Get current url");
	this->current_url = this->navigator->navigate(this->current_url);
	do {
		if (countdown::links) {
			logging::vout("Links countdown : " + std::to_string(countdown::number-x));
		}
		if(countdown::timeout) {
			logging::vout("Time countdown : " + std::to_string(countdown::time));
		}
		time(&begin);
		logging::vout(3,"Get page's html");
		page_html = this->navigator->get_body_html();
		if(!search) {
			logging::vout(3,"Get page's links");
			this->navigator->select_hyperlinks_from_html(page_html, links);
		} else {
		logging::vout(3,"Add current url to the automatic blacklist");
			this->navigator->select_hyperlinks_from_html(page_html, links, this->rubbish_links);
			search = false;
		}
		if(links.size() != 0){
			logging::vout(3,"Select link");
			current_url = select_link(links,this->current_url).url;
			logging::vout(3,"Navigate to the link's url");
			navigate_res = this->navigator->navigate(this->current_url);
		} else {
			logging::vout(3,"Search keyword in the adress bar");
			search_keyword();
		}
		/* we get out if we passed more than 10 links on the same domain
		 	 or if python met an error */
		if(navigate_res == "failed" || current_domain_occurences() > 10) {
			logging::vout(3,"Search keyword in the adress bar");
			search_keyword();
			search = true;
		} else {
			this->current_url = navigate_res;
		}
		time(&end);
		countdown::time -= (long)difftime(end,begin);
		timer = (countdown::timeout && (countdown::time <= 0));
		overflow = (countdown::links && (x++ >= countdown::number));
		logging::vout(3,"Add current url to the history");
		append_vector(this->history,this->current_url,HISTORY_MAX);
	} while( !( timer || overflow || !threading::running ));
	logging::vout(2,"Leaving Intelligence::roam");
	threading::running = false;
}

/*=================================KEYWORDS===================================*/

std::string select_keyword(std::vector<std::string> list)
{
	logging::vout(2,"Entering select_keyword");
	int random;
	if(list.size() <= 1) {
		random = 0;
	} else {
		random = (int)(std::rand() % list.size());
	}
	std::string res = list.at(random);
	logging::vout(2,"Leaving select_keyword");
	return res;
}

void Intelligence::search_keyword()
{
	logging::vout(2,"Entering Intelligence::search_keyword");
	logging::vout(3,"Add current url to automatic blacklist");
	append_vector(this->auto_blacklist,this->current_url,AUTO_BL_MAX);
	logging::vout(3,"Select keyword");
	std::string kw = select_keyword(keywords);
	logging::vout(3,"Write keyword in the adress bar");
	this->current_url = this->navigator->write_search(kw);
	logging::vout(2,"Leaving Intelligence::search_keyword");
}

int Intelligence::current_domain_occurences()
{
	logging::vout(2,"Entering Intelligence::current_domain_occurences");
	int res = 0;
	logging::vout(3,"Get current domain");
	std::string domain = this->current_url.substr(0,this->current_url.find("/",9));
	for(auto const& url: this->history) {
		if(url.substr(0,url.find("/",9)) == domain) {
			res++;
		}
	}
	logging::vout(2,"Leaving Intelligence::current_domain_occurences");
	return res;
}

void append_vector(std::vector<std::string> &list,std::string param,int limit)
{
	logging::vout(2,"Entering append_vector");
	if(std::find(list.begin(), list.end(), param)==list.end()){
		if(list.size() < limit) {
			list.push_back(param);
		} else {
			list.erase(list.begin());
			list.push_back(param);
		}
	}
	logging::vout(2,"Leaving append_vector");
}

/*==================================SELECT====================================*/

HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
	logging::vout(3,"Entering select_random_in_vector");
	int random;
	random = (int)(std::rand() % links.size());
	HyperLink link = links.at(random);
	logging::vout(3,"Leaving select_random_in_vector");
  return link;
}

HyperLink Intelligence::select_link(std::vector<HyperLink> &links,std::string url)
{
	logging::vout(2,"Entering Intelligence::select_link");
	HyperLink link;
	if(links.size() == 1){
		logging::vout(2,"Leaving Intelligence::select_link");
		return links.at(0);
	}
	bool res;
	bool whitelisted;
	bool found = true;
	if(dict::whitelist) {
	  found = false;
		std::vector<HyperLink> wlfound_list;
		for(auto const& lk: links){
			whitelisted = false;
			std::string text = " "+lk.text+" ";
			for(auto const& wl: this->whitelist){
				if(text.find(" "+wl+" ") != std::string::npos){
					res = Intelligence::test_link(link,url);
					if(res){
						whitelisted = true;
						found = true;
						logging::vout(3,"--Find wl : " + wl);
						logging::vout(3,"--In text :" + text);
					}
				}
			}
			if(whitelisted)
				wlfound_list.push_back(lk);
		}
		if(found){
			link = select_random_in_vector(wlfound_list);
		}
	}
	if( !dict::whitelist || !found ){
		int cpt=0;
	  do {
	    link = select_random_in_vector(links);
	    res = Intelligence::test_link(link,url);
	  } while (!res || cpt++<50);
		if(cpt == 50){
			logging::vout("--No link found");
			//TODO : Do something to handle
		}
	}
	logging::vout(2,"Leaving Intelligence::select_link");
	logging::vout(1,"--Chosen link : " + link.text);
	logging::vout(1,"          url : " + link.url);
	return link;
}

bool Intelligence::test_link(HyperLink &link,std::string &url)
{
	/* return 2 : continue, return 1 : passed the test*/
	std::string text = " "+link.text+" ";
	if(dict::blacklist){
		for(auto const& bl: this->blacklist){
			if(text.find(" "+bl+" ") != std::string::npos){
				logging::vout(4,"Leaving test_link");
				return false;
			}
		}
	}
	if(link.url == url || std::find(this->auto_blacklist.begin()
	, auto_blacklist.end(), link.url) != auto_blacklist.end()){
		logging::vout(4,"Leaving test_link");
		return false;
	}
	logging::vout(4,"Leaving test_link");
	return true;
}

/*==================================LIST======================================*/

void Intelligence::load_lists()
{
	logging::vout(2,"Entering Intelligence::load_lists");
	logging::vout(3,"Add current url to history");
	this->history.push_back(this->current_url);
	logging::vout(3,"Initialize keywords, rubbish list and automatic blacklist");
	this->keywords = init_list("./config/dictionaries/keywords.txt");
	this->rubbish_links = init_list("./config/dictionaries/rubbish_links.txt");
	this->auto_blacklist = init_list("./config/dictionaries/auto_blacklist.txt");
	if(keywords.size() == 0) {
		logging::vout("Keywords load failed");
	}
	if(rubbish_links.size() == 0) {
		logging::vout("Rubbish Links load failed");
	}
	if(dict::whitelist){
		logging::vout("Loading whitelist");
		this->whitelist = init_list(dict::whitefile);
	}
	if(dict::blacklist){
		logging::vout("Loading blacklist");
		this->blacklist = init_list(dict::blackfile);
	}
	logging::vout(2,"Leaving Intelligence::load_lists");
}

void Intelligence::dump_lists()
{
	logging::vout(2,"Entering Intelligence::dump_lists");
	std::ofstream file("./config/dictionaries/auto_blacklist.txt");
	for(auto const& line: this->auto_blacklist){
		file << line << "\n";
	}
	file.close();
	logging::vout(2,"Leaving Intelligence::dump_lists");
}

std::vector<std::string> init_list(std::string name)
{
	logging::vout(2,"Entering init_list");
	std::string line;
	std::ifstream file(name);
	std::vector<std::string> list;
	if(file) {
		while(std::getline(file, line)) {
			if(!line.empty())
				list.push_back(line);
		}
	} else {
		logging::vout("Error : file not opened");
	}
	file.close();
	logging::vout(2,"Leaving init_list");
	return list;
}

tuple_list init_otherlist(std::string name)
{
	logging::vout(2,"Entering init_otherlist");
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
		logging::vout("Error : file not opened");
	}
	file.close();
	logging::vout(2,"Leaving init_otherlist");
	return list;
}
