#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <Python.h>
#include "navigator.h"

//Run browser as a child process and store it's pid in var child_pid
int main()
{
	//parameters for execvp
	char *url,*bodyhtml;
	int nbLi;
	HyperLink *links;
	links = malloc(0);

	url = "https://en.wikipedia.org/wiki/Computer";
	bodyhtml = get_bodyhtml_from_url(url);

	nbLi = select_hyperlinks_from_html(bodyhtml, links);

	//printf("ouiiiii : %d",nbLi);
	/*for(int n=1;n<=nbLi;n++) {
		printf("Url%d : ",n);
		for(int z=0;z<strlen(links[n].url);z++) {
			printf("%c",links[n].url[z]);
		}
		printf("\n");
		printf("Txt%d : ",n);
		for(int t=0;t<strlen(links[n].text);t++) {
			printf("%c",links[n].text[t]);
		}
		printf("\n");
	}*/
	return 0;
}

char * get_bodyhtml_from_url(char *url) {
    char *resultat;
    PyObject *retour, *module, *fonction, *arguments;

    Py_Initialize();
    //PySys_SetPath(".");
    if(PyRun_SimpleString("import sys;sys.path.insert(0, '.')")) {
    	char *error;
        error = "path expansion failed\n";
        return error;
    }

    module = PyImport_ImportModule("main");
    if(module == NULL) {
    	char *error;
        error = "import failed\n";
        PyErr_Print();
        return error;
    }

    fonction = PyObject_GetAttrString(module, "get_html_from_url");
    if(fonction == NULL) {
    	char *error;
        error = "could not find function\n";
        return error;
    }

    arguments = Py_BuildValue("(s)", url);
    if(arguments == NULL) {
    	char *error;
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
    return resultat;
}

int select_hyperlinks_from_html(char *html,HyperLink *links) {
	int cpt= 0,ihr=0,itx=0;
	char in_tag_a=0,in_href=0,in_txt=0;
	char buff_txt[512],buff_href[2048];

	for(int i=0;i<strlen(html);i++) {
		if(html[i-3] == '<' && html[i-2] == 'a' && html[i-1] == ' ') {
			in_tag_a = 1;
		}

		if(in_tag_a) {
			if(html[i-6]=='h' && html[i-5]=='r' && html[i-4]=='e' && html[i-3]=='f'
			&& html[i-2]=='=' && html[i-1]=='"') {
				in_href=1;
			}
			if(in_href) {
				// links[cpt].url[ihr]=html[i];
				buff_href[ihr]=html[i];
				ihr++;
				if(html[i+1]=='"') {
					in_href = 0;
					//links[cpt].url[ihr+1]='\0';
					buff_href[ihr+1]='\0';
					ihr=0;
				}
			}
			if(html[i-1]=='>') {
				in_txt = 1;
			}
			if(in_txt) {
				//links[cpt].text[itx]=html[i];
				buff_txt[itx] = html[i];
				itx++;
				if(html[i+1]=='<' && html[i+2]=='/' && html[i+3]=='a'
				&& html[i+4]=='>') {
					in_txt=0;
					//links[cpt].text[itx+1]='\0';
					buff_txt[itx+1] = '\0';
					itx=0;
				}
			}
		}
		if(html[i+1] == '<' && html[i+2] == '/' && html[i+3] == 'a' ) {
			HyperLink link;
			link.url = malloc(sizeof(buff_href));
			link.url = buff_href;
			link.text = malloc(sizeof(buff_txt));
			link.text = buff_txt;
			//printf("Add : %d\n",(cpt+1)*sizeof(link));
			links = (HyperLink*)calloc(cpt+1,sizeof(link));
			links[cpt] = link;
			printf("Url%d : ",cpt);
			for(int z=0;z<strlen(links[cpt].url);z++) {
				printf("%c",links[cpt].url[z]);
			}
			printf("\n");
			printf("Txt%d : ",cpt);
			for(int t=0;t<strlen(links[cpt].text);t++) {
				printf("%c",links[cpt].text[t]);
			}

			cpt++;
			in_tag_a = 0;
		}
	}
	return cpt;
}
