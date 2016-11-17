#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <Python.h>
#include "navigator.h"



//Run browser as a child process and store it's pid in var child_pid
int main()
{
	pid_t pid;
	//parameters for execvp
	char *parmList[] = {"firefox", "google.com", NULL};
	char *url,*bodyhtml;
	int a,child_pid;
	int nbLi;
	HyperLink links[500];

	if ((pid = fork()) == -1)
		perror("fork failed");
	if (pid == 0) {
		//a = execvp("/usr/bin/firefox", parmList);
	} else {
		child_pid = pid;
		//Create command to kill process and execute it with bash
		//char command[20] = {0};
		//sprintf(command,"kill -9 %d",child_pid);
		//sleep(10);
		//system(command);

		url = "https://www.wikipedia.org/";//https://www.wikipedia.org/
		//https://www.youtube.com
		bodyhtml = get_bodyhtml_from_url(url);
		nbLi = select_hyperlinks_from_html(bodyhtml, links);

		for(int n=0;n<nbLi;n++) {
			printf("Url%d : ",n);
			for(int z=0;z<strlen(links[n].url);z++) {
				printf("%c",links[n].url[z]);
			}
			printf("\n");
			printf("Txt%d : ",n);
			for(int z=0;z<strlen(links[n].url);z++) {
				printf("%c",links[n].url[z]);
			}
			printf("\n");
		}
		//printf("%s\n",bodyhtml);
	}

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

    module = PyImport_ImportModule("get_html");
    if(module == NULL) {
    	char *error;
        error = "import failed\n";
        PyErr_Print();
        return error;
    }

    fonction = PyObject_GetAttrString(module, "runspider_with_url");
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
				if(html[i+1]=='>') {
					in_txt = 1;
				}
				if(in_txt) {
					if(html[i+1]=='<' && html[i+2]=='/' && html[i+3]=='a'
					&& html[i+4]=='>') {
						in_txt=0;
						links[cpt].text[itx+1]='\0';
						itx=0;
					}
					links[cpt].text[itx]=html[i];
				}
				links[cpt].url[ihr]=html[i];
				ihr++;
				if(html[i+1]=='"') {
					in_href = 0;
					links[cpt].url[ihr+1]='\0';
					ihr=0;
				}
			}
		}
		if(html[i+1] == '<' && html[i+2] == '/' && html[i+3] == 'a' ) {
			cpt++;
			in_tag_a = 0;
		}
	}
	return cpt;
}
