#!/bin/bash
# Run in scripts/
sudo rm /usr/local/lib/python2.7/dist-packages/uinput_wrapping_module.so
sudo rm /usr/local/lib/python2.7/dist-packages/Uinput_wrapping_module-1.0.egg-info
cd ../src/ctrl_keyboard
rm -r build/
sudo python setup.py install
