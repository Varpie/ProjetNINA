#!/bin/bash

printf "setup python modules..."

cd src/ctrl_keyboard
sudo python setup.py install
cd -

if cat /etc/*release | grep ^NAME | grep Debian ; then
  sudo cp src/navigation/nina.py /usr/local/lib/python2.7/dist-packages/
elif cat /etc/*release | grep ^NAME | grep Debian ; then
  sudo cp src/navigation/nina.py /usr/local/lib/python2.7/dist-packages/
else
  sudo cp src/navigation/nina.py /usr/local/lib/python2.7/site-packages/
fi

printf " done.\n"
