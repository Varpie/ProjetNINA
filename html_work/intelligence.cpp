#include "intelligence.hpp"

Intelligence::Intelligence(Navigator &nav,std::string &start_url)
{
	logging::vout("Creating Intelligence object");
	this->current_url = start_url;
	this->navigator = nav;
}

Intelligence::~Intelligence() {
	logging::vout("Deleting Intelligence object");
}

void Intelligence::roam()
{
	std::string page_html;
	std::vector<HyperLink> links;
	HyperLink link;
	int x = 0;
	this->current_url = this->navigator.navigate(this->current_url);
	do {
		logging::vout("début : " + this->current_url);
		page_html = this->navigator.get_body_html();
		this->navigator.select_hyperlinks_from_html(page_html, links);
		this->current_url = select_diff_random_in_vector(links,this->current_url).url;
		this->current_url = this->navigator.navigate(this->current_url);
		if(this->current_url == "failed") {
			this->current_url = select_diff_random_in_vector(links,this->current_url).url;
			//TODO : mettre l'url en question en blacklist
		}
		logging::vout("fin : ", this->current_url);
	} while(x++ <= 5);
}

HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
  	std::cout << "rand" << std::endl;
		srand (time(NULL));
    int rand = (int)(std::rand() % links.size());
    //std::cout << "rand : " << rand << " / " << links.size() << std::endl;
    HyperLink link = links.at(rand);
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

HyperLink select_from_word_list(std::vector<HyperLink> &links,std::string url)
{
	HyperLink link;
	std::string line;
	bool not_in_link = true;
	int c = 0;
	std::ifstream file("html_work/word_list.txt");
	do {
		link = select_random_in_vector(links);
		std::cout << "Lien " << c << " : " << link.text << std::endl;
		while(std::getline(file, line) && not_in_link) {
			if(link.text.find(line) != std::string::npos) {
				not_in_link = false;
				std::cout << "Lien valide : " << link.text << std::endl;
			}
		}
	} while(not_in_link && c++<50);
	std::cout << "Lien retourné : " << link.url << std::endl;
	return link;
}
