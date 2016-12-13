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
	std::string page_html;
	std::vector<HyperLink> links;
	HyperLink link;
	int x = 0;
	do {
		std::cout << "début : " << this->current_url << std::endl;
		page_html = this->navigator.get_body_html(this->current_url);
		this->navigator.select_hyperlinks_from_html(page_html, links);
		this->current_url = select_diff_random_in_vector(links,this->current_url).url;
		this->current_url = this->navigator.navigate(this->current_url);
		if(this->current_url == "failed") {
			this->current_url = select_diff_random_in_vector(links,this->current_url).url;
			//TODO : mettre l'url en question en blacklist
		}
		std::cout << "fin : " << this->current_url << std::endl;
	} while(x++ <= 150);
}

HyperLink select_random_in_vector(std::vector<HyperLink> &links)
{
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
