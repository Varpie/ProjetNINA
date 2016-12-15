#!/bin/bash

mkdir -p log
make && ./main --verbose 1>>log/log.txt 2>>log/error.txt
ps -o pmem,pcpu,cputime,cp $(ps -e | grep firefox | awk '{print $1}')
