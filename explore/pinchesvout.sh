#!/bin/bash
python3 pinches.py ../data/Propofol.mp4 tests/pinchesPropofol.csv resultvids/pinchesPropofol.mp4
python3 pinches.py ../data/Rocuronium.mp4 tests/pinchesRocuronium.csv resultvids/pinchesRocuronium.mp4
python3 pinches.py ../data/Phenylephrine.mp4 tests/pinchesPhenylephrine.csv resultvids/pinchesPhenylephrine.mp4
python3 pinches.py ../data/Lidocaine.mp4 tests/pinchesLidocaine.csv resultvids/pinchesLidocaine.mp4
python3 pinches.py ../data/Dexamethasone.mp4 tests/pinchesDexamethasone.csv resultvids/pinchesDexamethasone.mp4
python3 pinches.py ../data/Odanestron.mp4 tests/pinchesOdanestron.csv resultvids/pinchesOdanestron.mp4
python3 pinches.py ../data/2_label.mp4 tests/pinches2_label.csv resultvids/pinches2_label.mp4
python3 pinches.py ../data/3_label.mp4 tests/pinches3_label.csv resultvids/pinches3_label.mp4
python3 pinches.py ../data/4_label.mp4 tests/pinches4_label.csv resultvids/pinches4_label.mp4
python3 pinches.py ../data/5_label.mp4 tests/pinches5_label.csv resultvids/pinches5_label.mp4
python3 pinches.py ../data/6_label.mp4 tests/pinches6_label.csv resultvids/pinches6_label.mp4
python3 pinches.py ../data/no_color.mp4 tests/pinchesno_color.csv resultvids/pinchesno_color.mp4
python3 pinches.py ../data/blank.mp4 tests/pinchesblank.csv resultvids/pinchesblank.mp4
./scorestamps ../data/Propofol.csv tests/pinchesPropofol.csv
./scorestamps ../data/Rocuronium.csv tests/pinchesRocuronium.csv
./scorestamps ../data/Phenylephrine.csv tests/pinchesPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv tests/pinchesLidocaine.csv
./scorestamps ../data/Dexamethasone.csv tests/pinchesDexamethasone.csv
./scorestamps ../data/Odanestron.csv tests/pinchesOdanestron.csv
./scorestamps ../data/2_label.csv tests/pinches2_label.csv
./scorestamps ../data/3_label.csv tests/pinches3_label.csv
./scorestamps ../data/4_label.csv tests/pinches4_label.csv
./scorestamps ../data/5_label.csv tests/pinches5_label.csv
./scorestamps ../data/6_label.csv tests/pinches6_label.csv
./scorestamps ../data/no_color.csv tests/pinchesno_color.csv
./scorestamps ../data/blank.csv tests/pinchesblank.csv
