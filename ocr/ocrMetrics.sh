#!/bin/bash
FILE=../results/ocrNoDisplayResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/ocrNoDisplayResults.csv
fi
FILE=../results/ocrOpenMPResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/ocrOpenMPResults.csv
fi
FILE=../results/ocrPthreadResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/ocrPthreadResults.csv
fi
echo "ocr Times"
time ./ocrNoDisplay.sh >> ../results/ocrNoDisplayResults.csv
echo -e "\nocr OpenMP Times\n"
time ./ocrOpenMP.sh >> ../results/ocrOpenMPResults.csv
echo -e "\nocr Pthread Times\n"
time ./ocrPthread.sh >> ../results/ocrPthreadResults.csv
