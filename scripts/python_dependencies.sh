#!/bin/bash
printf "checking for python dependencies..."

sel=$(pip list --format columns | grep selenium)
lxml=$(pip list --format columns | grep lxml)
gd=$(find /usr/local/bin/ -name geckodriver)

if [ -z "$lxml" ]; then
  sudo pip install lxml
fi

if [ -z "$sel" ]; then
  sudo pip install selenium
fi

if [ -z "$gd" ]; then
  wget https://github.com/mozilla/geckodriver/releases/download/v0.11.1/geckodriver-v0.11.1-linux64.tar.gz && tar -xvf geckodriver-v0.11.1-linux64.tar.gz && rm geckodriver-v0.11.1-linux64.tar.gz && mv geckodriver /usr/local/bin/
fi

printf " done.\n"
