#!/bin/bash
./ocrPthread ../data/Propofol.mp4 ../tests/ocrPthreadPropofol.csv
./ocrPthread ../data/Rocuronium.mp4 ../tests/ocrPthreadRocuronium.csv
./ocrPthread ../data/Phenylephrine.mp4 ../tests/ocrPthreadPhenylephrine.csv
./ocrPthread ../data/Lidocaine.mp4 ../tests/ocrPthreadLidocaine.csv
./ocrPthread ../data/Dexamethasone.mp4 ../tests/ocrPthreadDexamethasone.csv
./ocrPthread ../data/Odanestron.mp4 ../tests/ocrPthreadOdanestron.csv
./ocrPthread ../data/2_label.mp4 ../tests/ocrPthread2_label.csv
./ocrPthread ../data/3_label.mp4 ../tests/ocrPthread3_label.csv
./ocrPthread ../data/4_label.mp4 ../tests/ocrPthread4_label.csv
./ocrPthread ../data/5_label.mp4 ../tests/ocrPthread5_label.csv
./ocrPthread ../data/6_label.mp4 ../tests/ocrPthread6_label.csv
./ocrPthread ../data/no_color.mp4 ../tests/ocrPthreadno_color.csv
./ocrPthread ../data/blank.mp4 ../tests/ocrPthreadblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/ocrPthreadPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/ocrPthreadRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/ocrPthreadPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/ocrPthreadLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/ocrPthreadDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/ocrPthreadOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/ocrPthread2_label.csv
./scorestamps ../data/3_label.csv ../tests/ocrPthread3_label.csv
./scorestamps ../data/4_label.csv ../tests/ocrPthread4_label.csv
./scorestamps ../data/5_label.csv ../tests/ocrPthread5_label.csv
./scorestamps ../data/6_label.csv ../tests/ocrPthread6_label.csv
./scorestamps ../data/no_color.csv ../tests/ocrPthreadno_color.csv
./scorestamps ../data/blank.csv ../tests/ocrPthreadblank.csv
