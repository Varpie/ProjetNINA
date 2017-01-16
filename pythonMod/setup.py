# -*- coding: utf-8 -*-
from distutils.core import setup, Extension
# define the extension module
cos_module = Extension('cos_module', sources=['cos_module.c'])
# run the setup
setup(ext_modules=[cos_module])

#python setup.py build_ext --inplace