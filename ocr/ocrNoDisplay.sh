#!/bin/bash
./ocrNoDisplay ../data/Propofol.mp4 ../tests/ocrNoDisplayPropofol.csv
./ocrNoDisplay ../data/Rocuronium.mp4 ../tests/ocrNoDisplayRocuronium.csv
./ocrNoDisplay ../data/Phenylephrine.mp4 ../tests/ocrNoDisplayPhenylephrine.csv
./ocrNoDisplay ../data/Lidocaine.mp4 ../tests/ocrNoDisplayLidocaine.csv
./ocrNoDisplay ../data/Dexamethasone.mp4 ../tests/ocrNoDisplayDexamethasone.csv
./ocrNoDisplay ../data/Odanestron.mp4 ../tests/ocrNoDisplayOdanestron.csv
./ocrNoDisplay ../data/2_label.mp4 ../tests/ocrNoDisplay2_label.csv
./ocrNoDisplay ../data/3_label.mp4 ../tests/ocrNoDisplay3_label.csv
./ocrNoDisplay ../data/4_label.mp4 ../tests/ocrNoDisplay4_label.csv
./ocrNoDisplay ../data/5_label.mp4 ../tests/ocrNoDisplay5_label.csv
./ocrNoDisplay ../data/6_label.mp4 ../tests/ocrNoDisplay6_label.csv
./ocrNoDisplay ../data/no_color.mp4 ../tests/ocrNoDisplayno_color.csv
./ocrNoDisplay ../data/blank.mp4 ../tests/ocrNoDisplayblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/ocrNoDisplayPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/ocrNoDisplayRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/ocrNoDisplayPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/ocrNoDisplayLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/ocrNoDisplayDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/ocrNoDisplayOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/ocrNoDisplay2_label.csv
./scorestamps ../data/3_label.csv ../tests/ocrNoDisplay3_label.csv
./scorestamps ../data/4_label.csv ../tests/ocrNoDisplay4_label.csv
./scorestamps ../data/5_label.csv ../tests/ocrNoDisplay5_label.csv
./scorestamps ../data/6_label.csv ../tests/ocrNoDisplay6_label.csv
./scorestamps ../data/no_color.csv ../tests/ocrNoDisplayno_color.csv
./scorestamps ../data/blank.csv ../tests/ocrNoDisplayblank.csv
