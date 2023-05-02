#!/bin/bash
./lines ../data/Propofol.mp4 ../tests/linesPropofol.csv
./lines ../data/Rocuronium.mp4 ../tests/linesRocuronium.csv
./lines ../data/Phenylephrine.mp4 ../tests/linesPhenylephrine.csv
./lines ../data/Lidocaine.mp4 ../tests/linesLidocaine.csv
./lines ../data/Dexamethasone.mp4 ../tests/linesDexamethasone.csv
./lines ../data/Odanestron.mp4 ../tests/linesOdanestron.csv
./lines ../data/2_label.mp4 ../tests/lines2_label.csv
./lines ../data/3_label.mp4 ../tests/lines3_label.csv
./lines ../data/4_label.mp4 ../tests/lines4_label.csv
./lines ../data/5_label.mp4 ../tests/lines5_label.csv
./lines ../data/6_label.mp4 ../tests/lines6_label.csv
./lines ../data/no_color.mp4 ../tests/linesno_color.csv
./lines ../data/blank.mp4 ../tests/linesblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/linesPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/linesRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/linesPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/linesLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/linesDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/linesOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/lines2_label.csv
./scorestamps ../data/3_label.csv ../tests/lines3_label.csv
./scorestamps ../data/4_label.csv ../tests/lines4_label.csv
./scorestamps ../data/5_label.csv ../tests/lines5_label.csv
./scorestamps ../data/6_label.csv ../tests/lines6_label.csv
./scorestamps ../data/no_color.csv ../tests/linesno_color.csv
./scorestamps ../data/blank.csv ../tests/linesblank.csv
