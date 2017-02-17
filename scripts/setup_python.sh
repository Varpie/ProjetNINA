#!/bin/bash

printf "setup python modules..."

pyMod=$(pip list --format columns | grep Uinput)
pyMain=$(find /usr/local/lib/python2.7/site-packages/ | grep nina.py)

if [ -z "$pyMod" ]; then
  cd src/ctrl_keyboard
  sudo python setup.py install
  cd -
fi

if [ -z "$pyMain" ]; then
  sudo cp src/navigation/nina.py /usr/local/lib/python2.7/site-packages/
fi

printf " done.\n"
