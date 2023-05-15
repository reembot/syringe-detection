#!/bin/bash
FILE=../results/centroidsNoDisplayResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/centroidsNoDisplayResults.csv
fi
echo "centroids Times"
time ./centroidsNoDisplay.sh >> ../results/centroidsNoDisplayResults.csv
