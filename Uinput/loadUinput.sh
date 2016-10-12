#!/bin/bash

flagUinput="$(lsmod | grep uinput)";
if [ -z "$flagUinput" ]
then
  sudo modprobe uinput;
  echo "uinput loaded";
else
  echo "uinput already loaded";
fi

sudo chmod 666 /dev/uinput;
echo "uinput chmod set to 666";
