#!/bin/bash
./hogsvmNoDisplay ../data/Propofol.mp4 ../tests/hogsvmNoDisplayPropofol.csv
./hogsvmNoDisplay ../data/Rocuronium.mp4 ../tests/hogsvmNoDisplayRocuronium.csv
./hogsvmNoDisplay ../data/Phenylephrine.mp4 ../tests/hogsvmNoDisplayPhenylephrine.csv
./hogsvmNoDisplay ../data/Lidocaine.mp4 ../tests/hogsvmNoDisplayLidocaine.csv
./hogsvmNoDisplay ../data/Dexamethasone.mp4 ../tests/hogsvmNoDisplayDexamethasone.csv
./hogsvmNoDisplay ../data/Odanestron.mp4 ../tests/hogsvmNoDisplayOdanestron.csv
./hogsvmNoDisplay ../data/2_label.mp4 ../tests/hogsvmNoDisplay2_label.csv
./hogsvmNoDisplay ../data/3_label.mp4 ../tests/hogsvmNoDisplay3_label.csv
./hogsvmNoDisplay ../data/4_label.mp4 ../tests/hogsvmNoDisplay4_label.csv
./hogsvmNoDisplay ../data/5_label.mp4 ../tests/hogsvmNoDisplay5_label.csv
./hogsvmNoDisplay ../data/6_label.mp4 ../tests/hogsvmNoDisplay6_label.csv
./hogsvmNoDisplay ../data/no_color.mp4 ../tests/hogsvmNoDisplayno_color.csv
./hogsvmNoDisplay ../data/blank.mp4 ../tests/hogsvmNoDisplayblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/hogsvmNoDisplayPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/hogsvmNoDisplayRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/hogsvmNoDisplayPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/hogsvmNoDisplayLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/hogsvmNoDisplayDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/hogsvmNoDisplayOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/hogsvmNoDisplay2_label.csv
./scorestamps ../data/3_label.csv ../tests/hogsvmNoDisplay3_label.csv
./scorestamps ../data/4_label.csv ../tests/hogsvmNoDisplay4_label.csv
./scorestamps ../data/5_label.csv ../tests/hogsvmNoDisplay5_label.csv
./scorestamps ../data/6_label.csv ../tests/hogsvmNoDisplay6_label.csv
./scorestamps ../data/no_color.csv ../tests/hogsvmNoDisplayno_color.csv
./scorestamps ../data/blank.csv ../tests/hogsvmNoDisplayblank.csv
