#include "navigator.h"
//Run browser as a child process and store it's pid in var child_pid
int main()
{
	//parameters for execvp
	std::string url, bodyhtml;
	int nbLi;
	// HyperLink *links;
	std::list<HyperLink> links;
	url = "https://wikipedia.org";
	//url = "https://en.wikipedia.org/wiki/Computer";
	//url = "http://dahunicorn.xyz";
	bodyhtml = get_bodyhtml_from_url(url);
	std::cout << "Test1" <<std::endl;
	std::cout << bodyhtml << std::endl;
	//nbLi = select_hyperlinks_from_html(bodyhtml, links);
	//printf("Nb Links : %d\n",nbLi);

	// for(int n=0;n<nbLi;n++) {
	// 	printf("Url%d : ",n);
	// 	// for(int z=0;z<strlen(links[n].url);z++) {
	// 	// 	printf("%c",links[n].url[z]);
	// 	// }
	// 	printf("\n");
	// 	printf("Txt%d : ",n);
	// 	// for(int t=0;t<strlen(links[n].text);t++) {
	// 	// 	printf("%c",links[n].text[t]);
	// 	// }
	// 	printf("\n");
	// }

	//FILE* file = fopen("word_list/word_list.txt", "r");

	return 0;
}

std::string get_bodyhtml_from_url(std::string url) {
    std::string resultat;
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
		std::cout << "Test2" <<std::endl;
		std::string str(resultat);
    return str;
}

int select_hyperlinks_from_html(std::string html,std::list<HyperLink> links) {
	int cpt= 0,ihr=0,itx=0;
	bool in_tag_a=false, in_href=false;
	bool in_txt=false, rubbish_tag=false;
	std::string buff_txt, buff_href;

	for(unsigned int i=0;i<html.length();i++) {
		if(html[i-3] == '<' && html[i-2] == 'a' && html[i-1] == ' ') {
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
					buff_href[ihr]='\0';
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
						buff_txt[itx] = '\0';
						itx=0;
					}
				}
			}
		}
		if(html[i+1] == '<' && html[i+2] == '/' && html[i+3] == 'a' ) {
			HyperLink link;
			link.setUrl(buff_href);
			link.setText(buff_txt);
			cpt++;
			in_tag_a = 0;
		}
	}
	return cpt;
}
