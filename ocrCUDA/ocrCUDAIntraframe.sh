#!/bin/bash
./ocrCUDAIntraframe ../data/Propofol.mp4 ../tests/ocrCUDAIntraframePropofol.csv
./ocrCUDAIntraframe ../data/Rocuronium.mp4 ../tests/ocrCUDAIntraframeRocuronium.csv
./ocrCUDAIntraframe ../data/Phenylephrine.mp4 ../tests/ocrCUDAIntraframePhenylephrine.csv
./ocrCUDAIntraframe ../data/Lidocaine.mp4 ../tests/ocrCUDAIntraframeLidocaine.csv
./ocrCUDAIntraframe ../data/Dexamethasone.mp4 ../tests/ocrCUDAIntraframeDexamethasone.csv
./ocrCUDAIntraframe ../data/Odanestron.mp4 ../tests/ocrCUDAIntraframeOdanestron.csv
./ocrCUDAIntraframe ../data/2_label.mp4 ../tests/ocrCUDAIntraframe2_label.csv
./ocrCUDAIntraframe ../data/3_label.mp4 ../tests/ocrCUDAIntraframe3_label.csv
./ocrCUDAIntraframe ../data/4_label.mp4 ../tests/ocrCUDAIntraframe4_label.csv
./ocrCUDAIntraframe ../data/5_label.mp4 ../tests/ocrCUDAIntraframe5_label.csv
./ocrCUDAIntraframe ../data/6_label.mp4 ../tests/ocrCUDAIntraframe6_label.csv
./ocrCUDAIntraframe ../data/no_color.mp4 ../tests/ocrCUDAIntraframeno_color.csv
./ocrCUDAIntraframe ../data/blank.mp4 ../tests/ocrCUDAIntraframeblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/ocrCUDAIntraframePropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/ocrCUDAIntraframeRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/ocrCUDAIntraframePhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/ocrCUDAIntraframeLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/ocrCUDAIntraframeDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/ocrCUDAIntraframeOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/ocrCUDAIntraframe2_label.csv
./scorestamps ../data/3_label.csv ../tests/ocrCUDAIntraframe3_label.csv
./scorestamps ../data/4_label.csv ../tests/ocrCUDAIntraframe4_label.csv
./scorestamps ../data/5_label.csv ../tests/ocrCUDAIntraframe5_label.csv
./scorestamps ../data/6_label.csv ../tests/ocrCUDAIntraframe6_label.csv
./scorestamps ../data/no_color.csv ../tests/ocrCUDAIntraframeno_color.csv
./scorestamps ../data/blank.csv ../tests/ocrCUDAIntraframeblank.csv
