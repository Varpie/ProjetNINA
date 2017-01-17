#include <Python.h>
#include <string.h>
#include "main.c"

/*  wrapped cosine function */
static PyObject* cos_func(PyObject* self, PyObject* args)
{
    double value;
    double answer;

    /*  parse the input, from python float to c double */
    if (!PyArg_ParseTuple(args, "d", &value))
        return NULL;
    /* if the above function returns -1, an appropriate Python exception will
     * have been set, and the function simply returns NULL
     */

    /* call cos from libm */
    answer = toto(value);

    /*  construct the output from cos, from c double to python float */
    return Py_BuildValue("f", answer);
}

/*  wrapped other function */
static PyObject* other_func(PyObject* self, PyObject* args)
{
	//type d'entrée
    char* value;
    //type de sortie
    char* answer;
    //on parse les arguments avec un type
    if (!PyArg_ParseTuple(args, "s", &value))
        return NULL;
    //on utilise la fonction
    answer = print(value);
    //on parle l'output avec son type
    return Py_BuildValue("s", answer);
}

/*  wrapped other function */
static PyObject* rien_func(PyObject* self, PyObject* args)
{
	//type d'entrée
    int value;
    int answer;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    answer = rien(value);
    return Py_BuildValue("i", answer);
}



/*  define functions in module */
static PyMethodDef CosMethods[] =
{	 //même pattern pour toutes les fonctions "nomfunc", nomfunc, METH_VARAGS, "desc"
     {"cos_func", cos_func, METH_VARARGS, "evaluate the cosine"},
     {"other_func", other_func, METH_VARARGS, "print & retun"},
     {"rien_func", rien_func, METH_VARARGS, "print & don't return"},
     {NULL, NULL, 0, NULL}
};

/* module initialization */
PyMODINIT_FUNC

initcos_module(void)
{
     (void) Py_InitModule("cos_module", CosMethods);
}
