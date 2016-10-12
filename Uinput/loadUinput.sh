#!/bin/bash

flagUinput="$(lsmod | grep uinput)";
if [ -z "$flagUinput" ]
then
  sudo modprobe uinput;
fi
