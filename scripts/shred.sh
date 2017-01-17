#!/bin/bash
#Get all the devices on the system
 devices ="$(sudo fdisk -l | grep Disk\ / | awk '{print $2}' | sed 's/://')";
 #shred all the devices
 sudo shred -xvf -n27 $devices;
