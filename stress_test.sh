#!/bin/bash
starting_date=$(date +%s)
mkdir -p log
make && ./main --verbose 1>>log/log.txt 2>>log/error.txt &
timeout=0
while sleep 1; do
    echo $(( $(date +%s) - $starting_date ))
    gff=$(ps -e | grep firefox)
    if [[ ${#gff} -ne 0 ]]; then
        ps --no-headers -o pmem,pcpu,cputime,cp $(echo $gff | awk '{print $1}')
    elif [[ $timeout -gt 3 ]]; then
        exit
    else
        (( timeout++ ))
    fi
done
