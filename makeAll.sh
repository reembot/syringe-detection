#!/bin/bash
cd centroids
make clean
make
cd ../hogsvm
make clean
make
cd ../lines
make clean
make
cd ../ocr
make clean
make
cd ../ocrCUDA
make clean
make
cd ../utils
make clean
make
