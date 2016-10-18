#include <stdio.h>
#include <stdlib.h>
#include <Python.h>

int main() {
    PyObject *retour, *module, *fonction, *arguments;
    char *resultat;
    char *url;
    url = "https://www.wikipedia.org";
    printf("starting\n");

    Py_Initialize();
    //PySys_SetPath(".");
    if(PyRun_SimpleString("import sys;sys.path.insert(0, '.')")) {
        printf("path expansion failed\n");
        return(2);
    }

    module = PyImport_ImportModule("get_html");
    if(module == NULL) {
        printf("import failed\n");
        PyErr_Print();
        return(2);
    }

    fonction = PyObject_GetAttrString(module, "runspider_with_url");
    if(fonction == NULL) {
        printf("could not find function\n");
        return(2);
    }

    arguments = Py_BuildValue("(s)", url);
    if(arguments == NULL) {
        printf("arg parsing failed\n");
        return(2);
    }

    printf("Calling\n");
    retour = PyEval_CallObject(fonction, arguments);
    printf("Returned\n");

    // note: need to release arguments
    Py_DECREF(arguments);

    if(retour == NULL) {
        printf("It all went wrong\n");
        PyErr_Print();
        return(2);
    }

    PyArg_Parse(retour, "s", &resultat);
    printf("Resultat: %s\n", resultat);

    Py_Finalize();
    return 0;
}