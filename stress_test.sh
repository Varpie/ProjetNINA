#!/bin/bash

mkdir -p log
./main --verbose 1>>log/log.txt 2>>log/error.txt
