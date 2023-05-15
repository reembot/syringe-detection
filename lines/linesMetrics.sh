#!/bin/bash
FILE=../results/linesNoDisplayResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/linesNoDisplayResults.csv
fi
echo "lines Times"
time ./linesNoDisplay.sh >> ../results/linesNoDisplayResults.csv
