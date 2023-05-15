#!/bin/bash
./ocrCUDAInterframe ../data/Propofol.mp4 ../tests/ocrCUDAInterframePropofol.csv
./ocrCUDAInterframe ../data/Rocuronium.mp4 ../tests/ocrCUDAInterframeRocuronium.csv
./ocrCUDAInterframe ../data/Phenylephrine.mp4 ../tests/ocrCUDAInterframePhenylephrine.csv
./ocrCUDAInterframe ../data/Lidocaine.mp4 ../tests/ocrCUDAInterframeLidocaine.csv
./ocrCUDAInterframe ../data/Dexamethasone.mp4 ../tests/ocrCUDAInterframeDexamethasone.csv
./ocrCUDAInterframe ../data/Odanestron.mp4 ../tests/ocrCUDAInterframeOdanestron.csv
./ocrCUDAInterframe ../data/2_label.mp4 ../tests/ocrCUDAInterframe2_label.csv
./ocrCUDAInterframe ../data/3_label.mp4 ../tests/ocrCUDAInterframe3_label.csv
./ocrCUDAInterframe ../data/4_label.mp4 ../tests/ocrCUDAInterframe4_label.csv
./ocrCUDAInterframe ../data/5_label.mp4 ../tests/ocrCUDAInterframe5_label.csv
./ocrCUDAInterframe ../data/6_label.mp4 ../tests/ocrCUDAInterframe6_label.csv
./ocrCUDAInterframe ../data/no_color.mp4 ../tests/ocrCUDAInterframeno_color.csv
./ocrCUDAInterframe ../data/blank.mp4 ../tests/ocrCUDAInterframeblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/ocrCUDAInterframePropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/ocrCUDAInterframeRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/ocrCUDAInterframePhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/ocrCUDAInterframeLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/ocrCUDAInterframeDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/ocrCUDAInterframeOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/ocrCUDAInterframe2_label.csv
./scorestamps ../data/3_label.csv ../tests/ocrCUDAInterframe3_label.csv
./scorestamps ../data/4_label.csv ../tests/ocrCUDAInterframe4_label.csv
./scorestamps ../data/5_label.csv ../tests/ocrCUDAInterframe5_label.csv
./scorestamps ../data/6_label.csv ../tests/ocrCUDAInterframe6_label.csv
./scorestamps ../data/no_color.csv ../tests/ocrCUDAInterframeno_color.csv
./scorestamps ../data/blank.csv ../tests/ocrCUDAInterframeblank.csv
