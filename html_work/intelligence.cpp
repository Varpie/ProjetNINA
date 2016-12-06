#include "intelligence.hpp"
#include "navigator.hpp"

Intelligence::Intelligence(Navigator &nav,std::string &start_url)
{
	this->current_url = start_url;
	this->navigator = nav;
}

Intelligence::~Intelligence() { }

void Intelligence::roam()
{
	std::string page_html = this->navigator.get_body_html(this->current_url);
	std::vector<HyperLink> links;
	HyperLink link;
	int x = 0;
	do {
		this->navigator.select_hyperlinks_from_html(page_html, links);
		link = select_diff_random_in_vector(links,this->current_url);
		std::string nav_return = this->navigator.navigate(link.url);
		if(nav_return == "failed") {
			link = select_diff_random_in_vector(links,this->current_url);
			this->current_url = link.url;
		} else {
			this->current_url = nav_return;
		}
	} while(x++ < 30);
}

HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
    int rand = (int)(std::rand() % links.size());
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
