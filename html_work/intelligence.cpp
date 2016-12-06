#include "intelligence.hpp"
#include "navigator.hpp"

Intelligence::Intelligence(Navigator &nav,std::string &start_url)
{
	this->current_url = start_url;
	this->navigator = nav;
}

void Intelligence::roam()
{
	std::string page_html;
	std::vector<HyperLink> links;
	int x = 0;
	do {
		page_html = this->navigator.get_body_html(this->current_url);
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
	} while(x++ < 5);
}


HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
    int rand = (int)(std::rand() % links.size());
    //std::cout << "rand : " << rand << " / " << links.size() << std::endl;
    HyperLink link = links.at(rand);
    return link;
}