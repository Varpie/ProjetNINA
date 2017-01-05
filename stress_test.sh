#!/bin/bash
starting_date=$(date +%s)
mkdir -p log
make && ./main --verbose 1>>log/log.txt 2>>log/error.txt &
timeout=0
# Init gnuplot files
echo "" > ps.dat
while sleep 1; do
    current_date=$(( $(date +%s) - $starting_date ))
    gff=$(ps -e | grep firefox)
    # Check if firefox process exists
    if [[ ${#gff} -ne 0 ]]; then
        echo "$current_date $(ps --no-headers -o pmem,pcpu $(echo $gff | awk '{print $1}'))" >> ps.dat
    elif [[ $timeout -gt 3 ]]; then
        gnuplot -p -e "plot 'ps.dat' using 2 title 'pmem' with lines, \
                            'ps.dat' using 3 title 'pcpu' with lines"
        exit
    else
        (( timeout++ ))
    fi
done
