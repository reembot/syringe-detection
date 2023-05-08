#!/bin/bash
python3 skeletal.py data/Propofol.mp4 tests/skeletalPropofol.csv resultvids/skeletalPropofol.mp4
python3 skeletal.py data/Rocuronium.mp4 tests/skeletalRocuronium.csv resultvids/skeletalRocuronium.mp4
python3 skeletal.py data/Phenylephrine.mp4 tests/skeletalPhenylephrine.csv resultvids/skeletalPhenylephrine.mp4
python3 skeletal.py data/Lidocaine.mp4 tests/skeletalLidocaine.csv resultvids/skeletalLidocaine.mp4
python3 skeletal.py data/Dexamethasone.mp4 tests/skeletalDexamethasone.csv resultvids/skeletalDexamethasone.mp4
python3 skeletal.py data/Odanestron.mp4 tests/skeletalOdanestron.csv resultvids/skeletalOdanestron.mp4
python3 skeletal.py data/2_label.mp4 tests/skeletal2_label.csv resultvids/skeletal2_label.mp4
python3 skeletal.py data/3_label.mp4 tests/skeletal3_label.csv resultvids/skeletal3_label.mp4
python3 skeletal.py data/4_label.mp4 tests/skeletal4_label.csv resultvids/skeletal4_label.mp4
python3 skeletal.py data/5_label.mp4 tests/skeletal5_label.csv resultvids/skeletal5_label.mp4
python3 skeletal.py data/6_label.mp4 tests/skeletal6_label.csv resultvids/skeletal6_label.mp4
python3 skeletal.py data/no_color.mp4 tests/skeletalno_color.csv resultvids/skeletalno_color.mp4
python3 skeletal.py data/blank.mp4 tests/skeletalblank.csv resultvids/skeletalblank.mp4
./scorestamps data/Propofol.csv tests/skeletalPropofol.csv
./scorestamps data/Rocuronium.csv tests/skeletalRocuronium.csv
./scorestamps data/Phenylephrine.csv tests/skeletalPhenylephrine.csv
./scorestamps data/Lidocaine.csv tests/skeletalLidocaine.csv
./scorestamps data/Dexamethasone.csv tests/skeletalDexamethasone.csv
./scorestamps data/Odanestron.csv tests/skeletalOdanestron.csv
./scorestamps data/2_label.csv tests/skeletal2_label.csv
./scorestamps data/3_label.csv tests/skeletal3_label.csv
./scorestamps data/4_label.csv tests/skeletal4_label.csv
./scorestamps data/5_label.csv tests/skeletal5_label.csv
./scorestamps data/6_label.csv tests/skeletal6_label.csv
./scorestamps data/no_color.csv tests/skeletalno_color.csv
./scorestamps data/blank.csv tests/skeletalblank.csv
