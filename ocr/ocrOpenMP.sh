#!/bin/bash
./ocrOpenMP ../data/Propofol.mp4 ../tests/ocrOpenMPPropofol.csv
./ocrOpenMP ../data/Rocuronium.mp4 ../tests/ocrOpenMPRocuronium.csv
./ocrOpenMP ../data/Phenylephrine.mp4 ../tests/ocrOpenMPPhenylephrine.csv
./ocrOpenMP ../data/Lidocaine.mp4 ../tests/ocrOpenMPLidocaine.csv
./ocrOpenMP ../data/Dexamethasone.mp4 ../tests/ocrOpenMPDexamethasone.csv
./ocrOpenMP ../data/Odanestron.mp4 ../tests/ocrOpenMPOdanestron.csv
./ocrOpenMP ../data/2_label.mp4 ../tests/ocrOpenMP2_label.csv
./ocrOpenMP ../data/3_label.mp4 ../tests/ocrOpenMP3_label.csv
./ocrOpenMP ../data/4_label.mp4 ../tests/ocrOpenMP4_label.csv
./ocrOpenMP ../data/5_label.mp4 ../tests/ocrOpenMP5_label.csv
./ocrOpenMP ../data/6_label.mp4 ../tests/ocrOpenMP6_label.csv
./ocrOpenMP ../data/no_color.mp4 ../tests/ocrOpenMPno_color.csv
./ocrOpenMP ../data/blank.mp4 ../tests/ocrOpenMPblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/ocrOpenMPPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/ocrOpenMPRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/ocrOpenMPPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/ocrOpenMPLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/ocrOpenMPDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/ocrOpenMPOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/ocrOpenMP2_label.csv
./scorestamps ../data/3_label.csv ../tests/ocrOpenMP3_label.csv
./scorestamps ../data/4_label.csv ../tests/ocrOpenMP4_label.csv
./scorestamps ../data/5_label.csv ../tests/ocrOpenMP5_label.csv
./scorestamps ../data/6_label.csv ../tests/ocrOpenMP6_label.csv
./scorestamps ../data/no_color.csv ../tests/ocrOpenMPno_color.csv
./scorestamps ../data/blank.csv ../tests/ocrOpenMPblank.csv
