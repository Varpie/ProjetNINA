#!/bin/bash

xinput create-master "Generic Device"

pointer=$(xinput list | grep "Generic Device pointer" | awk -F"=" '{print $2}' | awk '{print $1}')
keyboard=$(xinput list | grep "Generic Device keyboard" | awk -F"=" '{print $2}' | awk '{print $1}')

ckb=$(xinput list | grep "Custom Device" | grep "keyboard" | awk -F"=" '{print $2}' | awk '{print $1}')
cp=$(xinput list | grep "Custom Device" | grep "pointer" | awk -F"=" '{print $2}' | awk '{print $1}')

xinput reattach $ckb $keyboard
xinput reattach $cp $pointer
