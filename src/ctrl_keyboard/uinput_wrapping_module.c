#include <Python.h>
#include "write_keyboard.h"

static PyObject* write_string_func(PyObject* self, PyObject* args)
{
  char *value;
  int result;
  PyObject *ret;
  if (!PyArg_ParseTuple(args, "s", &value))
    return NULL;
  result = write_string(value);
  ret = PyInt_FromLong((long)result);
  return ret;
}

static PyObject* send_key_func(PyObject* self, PyObject* args)
{
  int value;
  if (!PyArg_ParseTuple(args, "i", &value))
    return NULL;
  send_key(value);
  Py_RETURN_NONE;
}

static PyObject* send_key_with_shift_func(PyObject* self, PyObject* args)
{
  int value;
  if (!PyArg_ParseTuple(args, "i", &value))
    return NULL;
  send_key_with_shift(value);
  Py_RETURN_NONE;
}

static PyObject* send_key_with_altgr_func(PyObject* self, PyObject* args)
{
  int value;
  if (!PyArg_ParseTuple(args, "i", &value))
    return NULL;
  send_key_with_altgr(value);
  Py_RETURN_NONE;
}

static PyObject* send_key_with_altgr_shift_func(PyObject* self, PyObject* args)
{
  int value;
  if (!PyArg_ParseTuple(args, "i", &value))
    return NULL;
  send_key_with_altgr_shift(value);
  Py_RETURN_NONE;
}

static PyObject* send_key_with_ctrl_func(PyObject* self, PyObject* args)
{
  int value;
  if (!PyArg_ParseTuple(args, "i", &value))
    return NULL;
  send_key_with_ctrl(value);
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
{
  {"write_string_func",(PyCFunction)write_string_func, METH_VARARGS, "write a string"},
  {"send_key_func",(PyCFunction)send_key_func, METH_VARARGS, "send a single key"},
  {"send_key_with_shift_func",(PyCFunction)send_key_with_shift_func, METH_VARARGS, "send a single key with shift modifier"},
  {"send_key_with_ctrl_func",(PyCFunction)send_key_with_ctrl_func, METH_VARARGS, "send a single key with ctrl modifier"},
  {"send_key_with_altgr_func",(PyCFunction)send_key_with_altgr_func, METH_VARARGS, "send a single key with altgr modifier"},
  {"send_key_with_altgr_shift_func",(PyCFunction)send_key_with_altgr_shift_func, METH_VARARGS, "send a single key with altgr & shift modifiers"},
  {"setup_uinput_device_func",(PyCFunction)setup_uinput_device_func,METH_NOARGS,"setup uinput"},
  {"destroy_uinput_device_func",(PyCFunction)destroy_uinput_device_func,METH_NOARGS,"destroy uinput"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
inituinput_wrapping_module(void)
{
  (void) Py_InitModule("uinput_wrapping_module", UinputMethods);
}