#include <Python.h>
#include "write_keyboard.c"

static PyObject* send_a_button_default_func(PyObject* self, PyObject* args)
{
    int value;
    int answer;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    answer = send_a_button_default(value);
    return Py_BuildValue("i", answer);
}

static PyMethodDef UinputMethods[] =
{	 //même pattern pour toutes les fonctions "nomfunc", nomfunc, METH_VARAGS, "desc"
     {"send_a_button_default_func", send_a_button_default_func, METH_VARARGS, "send a single kb output"},
     {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
inituinput_wrapping_module(void)
{
     (void) Py_InitModule("uinput_wrapping_module", UinputMethods);
}
// void send_a_button_default(int key)
// void write_array(char array[], int size)