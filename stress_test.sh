#!/bin/bash
current_date=$(date +%s)
mkdir -p log
make && ./main --verbose 1>>log/log.txt 2>>log/error.txt &
while sleep 1; do
    echo $(( $(date +%s) - $current_date ))
    ps --no-headers -o pmem,pcpu,cputime,cp $(ps -e | grep firefox | awk 'BEGIN{if(!$1){print "nope"}else{print $1}}') # à finir
done
