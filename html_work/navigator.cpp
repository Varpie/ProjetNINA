#include "navigator.h"
int main()
{
	std::string url, bodyhtml;
	std::list<HyperLink> links;
	url = "https://wikipedia.org";
	//url = "https://en.wikipedia.org/wiki/Computer";
	//url = "http://dahunicorn.xyz";
	bodyhtml = get_bodyhtml_from_url(url);
	//std::cout << bodyhtml << std::endl;
	select_hyperlinks_from_html(bodyhtml, links);
	// for(std::list<HyperLink>::iterator it = links.begin(); it != links.end();++it) {
	// 	std::cout << (*it)->url << std::endl;
	// 	std::cout << (*it)->text << std::endl;
	// }

	for(auto const& i : links) {
		std::cout << "Url : " << i.url << std::endl;
		std::cout << "Text : " << i.text << std::endl;
	}
	return 0;
}

std::string get_bodyhtml_from_url(std::string url)
{
    char *resultat;
    PyObject *retour, *module, *fonction, *arguments;

    Py_Initialize();
    //PySys_SetPath(".");
    if(PyRun_SimpleString("import sys;sys.path.insert(0, '.')")) {
    	std::string error;
        error = "path expansion failed\n";
        return error;
    }

    module = PyImport_ImportModule("main");
    if(module == NULL) {
    	std::string error;
        error = "import failed\n";
        PyErr_Print();
        return error;
    }

    fonction = PyObject_GetAttrString(module, "get_html_from_url");
    if(fonction == NULL) {
    	std::string error;
        error = "could not find function\n";
        return error;
    }

    arguments = Py_BuildValue("(s)", url.c_str());
    if(arguments == NULL) {
    	std::string error;
        error = "arg parsing failed\n";
        return error;
    }

    printf("Calling\n");
    retour = PyEval_CallObject(fonction, arguments);
    printf("Returned\n");

    // note: need to release arguments
    Py_DECREF(arguments);

    if(retour == NULL) {
        printf("It all went wrong\n");
        PyErr_Print();
        return url;
    }

    PyArg_Parse(retour, "s", &resultat);

    Py_Finalize();
    std::string cpp_str = resultat;
    return cpp_str;
}

void select_hyperlinks_from_html(std::string html,std::list<HyperLink> &links)
{
	//nps
	// std::string b_tag_a = "<a ";
	// std::string a_txt = ">";
	// std::string a_href = "href\"";
	// std::string end_href = "\"";
	// std::string end_a = "</a>";
	size_t pos = 0;
	bool loop = true;
//std::string::npos
	while(loop) {
		size_t b_tag_a = html.find("<a ");
		if(b_tag_a != std::string::npos) {
			HyperLink lk;
			size_t e_tag_a = html.find("</a>");
			std::string tag_a = html.substr(b_tag_a+3,(e_tag_a-1-b_tag_a));
			html.erase(0,e_tag_a+4);


			size_t b_href = tag_a.find("href=\"");
			size_t e_href = tag_a.find("\"");
			size_t b_txt_a = tag_a.find(">");
			lk.url = tag_a.substr(b_href+6,e_href-b_href);
			lk.text = tag_a.substr(b_txt_a+1,(e_tag_a-b_txt_a)-10);
			links.push_back(lk);
		} else {
			loop = false;
		}
	}

	// HyperLink lk1;
	// lk1.url = "https://foo.com";
	// lk1.text = "foo";
	// HyperLink lk2;
	// lk2.url = "http://bar.fr";
	// lk2.text = "bar";
	// links.push_back(lk1);
	// links.push_back(lk2);
}


/*
int cpt= 0,ihr=0,itx=0;
bool in_tag_a=false, in_href=false;
bool in_txt=false, rubbish_tag=false;
std::string buff_txt, buff_href;
std::cout << "2" << std::endl;
for(int i=0;i<html.length();i++) {
	std::cout << "3" << std::endl;
	if(html[i-3] == '<' && html[i-2] == 'a' && html[i-1] == ' ') {
		std::cout << "4" << std::endl;
		in_tag_a = true;
	}

	if(in_tag_a) {
		if(html[i-6]=='h' && html[i-5]=='r' && html[i-4]=='e' && html[i-3]=='f'
		&& html[i-2]=='=' && html[i-1]=='"') {
			in_href=true;
		}
		if(in_href) {
			// links[cpt].url[ihr]=html[i];
			buff_href[ihr]=html[i];
			ihr++;
			if(html[i+1]=='"') {
				in_href = true;
				//links[cpt].url[ihr+1]='\0';
				//buff_href[ihr]='\0';
				ihr=0;
			}
		}
		if(html[i-1]=='>') {
			in_txt = true;
		}
		if(in_txt) {
			if(html[i-1] == '>') {
				rubbish_tag=false;
			}
			if(html[i] == '<') {
				rubbish_tag=true;
			}
			//links[cpt].text[itx]=html[i];
			if(!rubbish_tag) {
				if(html[i] != '\n') {
					buff_txt[itx] = html[i];
				}
				itx++;
				if(html[i+1]=='<' && html[i+2]=='/' && html[i+3]=='a'
				&& html[i+4]=='>') {
					in_txt=false;
					//links[cpt].text[itx+1]='\0';
					//buff_txt[itx] = '\0';
					itx=0;
				}
			}
		}
	}
	if(html[i+1] == '<' && html[i+2] == '/' && html[i+3] == 'a' ) {
		std::cout << "4" << std::endl;
		HyperLink link;
		link.setUrl(buff_href);
		link.setText(buff_txt);
		cpt++;
		in_tag_a = 0;
	}*/
