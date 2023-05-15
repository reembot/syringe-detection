#!/bin/bash
FILE=../results/ocrCUDAInterframeResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/ocrCUDAInterframeResults.csv
fi
FILE=../results/ocrCUDAIntraframeResults.csv
if [[ -f "$FILE" ]]; then
  rm ../results/ocrCUDAIntraframeResults.csv
fi
echo "ocr CUDA Interframe Times"
time ./ocrCUDAInterframe.sh >> ../results/ocrCUDAInterframeResults.csv
echo -e "\nocr CUDA Intraframe Times\n"
time ./ocrCUDAIntraframe.sh >> ../results/ocrCUDAIntraframeResults.csv
