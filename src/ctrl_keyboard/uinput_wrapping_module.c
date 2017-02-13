#include <Python.h>
#include <string.h>
#include "write_keyboard.h"
#include "timed_keystrokes.h"

//https://www.tutorialspoint.com/python/python_further_extensions.htm
//int send_a_button_default(int key){
static PyObject* send_a_button_default_func(PyObject* self, PyObject* args)
{
  int value;
  if (!PyArg_ParseTuple(args, "i", &value))
    return NULL;
  send_a_button_default(value);
  Py_RETURN_NONE;
}

static PyObject* send_a_button_func(PyObject* self, PyObject* args)
{
  int a;
  int b;
  if (!PyArg_ParseTuple(args, "ii", &a,&b))
    return NULL;
  send_a_button(a,b);
  Py_RETURN_NONE;
}

//void write_array(char array[], int size){
static PyObject* write_array_func(PyObject* self, PyObject* args)
{
  char* value;
  if (!PyArg_ParseTuple(args, "s", &value))
    return NULL;
  write_array(value,strlen(value));
  Py_RETURN_NONE;
}

static PyObject* setup_uinput_device_func(PyObject* self)
{
  setup_uinput_device();
  Py_RETURN_NONE;
}

static PyObject* destroy_uinput_device_func(PyObject* self)
{
  destroy_uinput_device();
  Py_RETURN_NONE;
}

static PyMethodDef UinputMethods[] =
{	 //même pattern pour toutes les fonctions "nomfunc", nomfunc, METH_VARAGS, "desc"
  {"send_a_button_default_func",(PyCFunction)send_a_button_default_func, METH_VARARGS, "send a single kb output"},
  {"send_a_button_func",(PyCFunction)send_a_button_func, METH_VARARGS, "send a single kb output with a modifier"},
  {"write_array_func",(PyCFunction)write_array_func, METH_VARARGS, "write a string"},
  {"setup_uinput_device_func",(PyCFunction)setup_uinput_device_func,METH_NOARGS,"setup uinput"},
  {"destroy_uinput_device_func",(PyCFunction)destroy_uinput_device_func,METH_NOARGS,"destroy uinput"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
inituinput_wrapping_module(void)
{
  (void) Py_InitModule("uinput_wrapping_module", UinputMethods);
}
// void send_a_button_default(int key)
// void write_array(char array[], int size)
