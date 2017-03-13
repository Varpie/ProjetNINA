# -*- coding: utf-8 -*-
from distutils.core import setup, Extension
# define the extension module
uinput_wrapping_module = Extension('uinput_wrapping_module', sources=['uinput_wrapping_module.c','write_keyboard.c','timed_keystrokes.c'])
# run the setup
setup(ext_modules=[uinput_wrapping_module],
    name='Uinput wrapping module',
    version='2.0',
    description='Wrapped C to call uinput methods from python',
    author='Nina',
    author_email='don\'t contact us',
    url='nope.com',
    )
#python setup.py build_ext --inplace
#sudo python setup.py install