#!/bin/bash
printf "checking for geckodriver..."

gd=$(find /usr/local/bin/ -name geckodriver)
if[ -z "$gd" ]; then
	wget https://github.com/mozilla/geckodriver/releases/download/v0.11.1/geckodriver-v0.11.1-linux64.tar.gz && tar -xvf geckodriver-v0.11.1-linux64.tar.gz && rm geckodriver-v0.11.1-linux64.tar.gz && mv geckodriver /usr/local/bin/
fi

printf " done."