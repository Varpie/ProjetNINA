# -*- coding: utf-8 -*-
from distutils.core import setup, Extension
# define the extension module
uinput_wrapping_module = Extension('uinput_wrapping_module', sources=['uinput_wrapping_module.c'])
# run the setup
setup(ext_modules=[uinput_wrapping_module])
#python setup.py build_ext --inplace
#sudo python setup.py install
