#!/bin/bash
./linesNoDisplay ../data/Propofol.mp4 ../tests/linesNoDisplayPropofol.csv
./linesNoDisplay ../data/Rocuronium.mp4 ../tests/linesNoDisplayRocuronium.csv
./linesNoDisplay ../data/Phenylephrine.mp4 ../tests/linesNoDisplayPhenylephrine.csv
./linesNoDisplay ../data/Lidocaine.mp4 ../tests/linesNoDisplayLidocaine.csv
./linesNoDisplay ../data/Dexamethasone.mp4 ../tests/linesNoDisplayDexamethasone.csv
./linesNoDisplay ../data/Odanestron.mp4 ../tests/linesNoDisplayOdanestron.csv
./linesNoDisplay ../data/2_label.mp4 ../tests/linesNoDisplay2_label.csv
./linesNoDisplay ../data/3_label.mp4 ../tests/linesNoDisplay3_label.csv
./linesNoDisplay ../data/4_label.mp4 ../tests/linesNoDisplay4_label.csv
./linesNoDisplay ../data/5_label.mp4 ../tests/linesNoDisplay5_label.csv
./linesNoDisplay ../data/6_label.mp4 ../tests/linesNoDisplay6_label.csv
./linesNoDisplay ../data/no_color.mp4 ../tests/linesNoDisplayno_color.csv
./linesNoDisplay ../data/blank.mp4 ../tests/linesNoDisplayblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/linesNoDisplayPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/linesNoDisplayRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/linesNoDisplayPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/linesNoDisplayLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/linesNoDisplayDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/linesNoDisplayOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/linesNoDisplay2_label.csv
./scorestamps ../data/3_label.csv ../tests/linesNoDisplay3_label.csv
./scorestamps ../data/4_label.csv ../tests/linesNoDisplay4_label.csv
./scorestamps ../data/5_label.csv ../tests/linesNoDisplay5_label.csv
./scorestamps ../data/6_label.csv ../tests/linesNoDisplay6_label.csv
./scorestamps ../data/no_color.csv ../tests/linesNoDisplayno_color.csv
./scorestamps ../data/blank.csv ../tests/linesNoDisplayblank.csv
