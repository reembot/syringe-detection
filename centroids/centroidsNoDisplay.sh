#!/bin/bash
./centroids ../data/Propofol.mp4 ../tests/centroids-Propofol.csv
./centroids ../data/Rocuronium.mp4 ../tests/centroids-Rocuronium.csv
./centroids ../data/Phenylephrine.mp4 ../tests/centroids-Phenylephrine.csv
./centroids ../data/Lidocaine.mp4 ../tests/centroids-Lidocaine.csv
./centroids ../data/Dexamethasone.mp4 ../tests/centroids-Dexamethasone.csv
./centroids ../data/Odanestron.mp4 ../tests/centroids-Odanestron.csv
./centroids ../data/2_label.mp4 ../tests/centroids-2_label.csv
./centroids ../data/3_label.mp4 ../tests/centroids-3_label.csv
./centroids ../data/4_label.mp4 ../tests/centroids-4_label.csv
./centroids ../data/5_label.mp4 ../tests/centroids-5_label.csv
./centroids ../data/6_label.mp4 ../tests/centroids-6_label.csv
./centroids ../data/no_color.mp4 ../tests/centroids-no_color.csv
./centroids ../data/blank.mp4 ../tests/centroids-blank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/centroids-Propofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/centroids-Rocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/centroids-Phenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/centroids-Lidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/centroids-Dexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/centroids-Odanestron.csv
./scorestamps ../data/2_label.csv ../tests/centroids-2_label.csv
./scorestamps ../data/3_label.csv ../tests/centroids-3_label.csv
./scorestamps ../data/4_label.csv ../tests/centroids-4_label.csv
./scorestamps ../data/5_label.csv ../tests/centroids-5_label.csv
./scorestamps ../data/6_label.csv ../tests/centroids-6_label.csv
./scorestamps ../data/no_color.csv ../tests/centroids-no_color.csv
./scorestamps ../data/blank.csv ../tests/centroids-blank.csv
