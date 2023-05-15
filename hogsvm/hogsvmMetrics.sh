#!/bin/bash
FILE=../results/hogsvmNoDisplayResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/hogsvmNoDisplayResults.csv
fi
echo "hogsvm Times"
time ./hogsvmNoDisplay.sh >> ../results/hogsvmNoDisplayResults.csv

