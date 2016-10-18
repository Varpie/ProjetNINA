#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Python.h>

char * get_bodyhtml_from_url(char *url);

//Run browser as a child process and store it's pid in var child_pid
int main()
{
	pid_t pid;
	//parameters for execvp
	char *parmList[] = {"firefox", "google.com", NULL};
	char *url,*bodyhtml;
	int a,child_pid;


	if ((pid = fork()) == -1)
		perror("fork failed");
	if (pid == 0) {
		a = execvp("/usr/bin/firefox", parmList);
	} else {
		child_pid = pid;
		//Create command to kill process and execute it with bash
		//char command[20] = {0};
		//sprintf(command,"kill -9 %d",child_pid);
		//sleep(10);
		//system(command);
	    
		url = "https://www.youtube.com";
		bodyhtml = get_bodyhtml_from_url(url);
		printf("%s\n",bodyhtml);
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
        return ;
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