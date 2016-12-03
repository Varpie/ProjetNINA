#include "intelligence.hpp"
#include "navigator.hpp"

Intelligence::Intelligence(Navigator *nav,std::string &start_url) 
{
	this->current_url=start_url;
	this->navigator=&nav;
}

void Intelligence::roam()
{
	//	int c = 0;
	std::string page_html = this->navigator.get_body_html(this->current_url);
	//	std::vector<HyperLink> links;
	//	this->navigator.select_hyperlinks_from_html(page_html, links);
	//	HyperLink link = select_random_in_vector(links);
	//	std::string current_url = this->navigator.navigate(link.url);
	//	c = c+1;
}


HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
    int size = links.size();
    int rand = (std::rand() * (int)(size) / RAND_MAX);
    HyperLink link = links.at(rand);
    return link;
}