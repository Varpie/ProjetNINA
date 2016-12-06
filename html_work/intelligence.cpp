#include <fstream>
#include "intelligence.hpp"
#include "navigator.hpp"

Intelligence::Intelligence(Navigator &nav,std::string &start_url)
{
	this->current_url = start_url;
	this->navigator = nav;
}

void Intelligence::roam()
{
	std::string page_html = this->navigator.get_body_html(this->current_url);
	std::vector<HyperLink> links;
	int x = 0;
	do {
		this->navigator.select_hyperlinks_from_html(page_html, links);
		HyperLink link = select_random_in_vector(links);
		if(link.url == current_url) {
			HyperLink link = select_random_in_vector(links);
		}
		this->current_url = this->navigator.navigate(link.url);
		if(this->current_url == "failed") {
			HyperLink link = select_random_in_vector(links);
			this->current_url = link.url;
		}
	} while(x++ < 15);
}


HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
    int rand = (int)(std::rand() % links.size());
    HyperLink link = links.at(rand);
    return link;
}

HyperLink select_with_dictionary(std::vector<HyperLink> &links)
{
	HyperLink link;
	ifstream fichier("../word_list/word_list.txt", ois::in);
	if(fichier) {
		bool in_dictionary;
		do {
			fichier.clear();
			HyperLink link = select_random_in_vector(links);
			std::string line;
			do {
				getline(fichier, line);
				in_dictionary = (link.text.find(line) != std::str::npos);
			} while (!in_dictionary && !fichier.eof());
		} while (!in_dictionary);
	} else {
		cerr << "Impossible d'ouvrir le fichier word_list.txt" << endl;
	}
	return link;
}
