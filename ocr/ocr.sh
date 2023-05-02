#!/bin/bash
./ocr ../data/Propofol.mp4 ../tests/ocrPropofol.csv
./ocr ../data/Rocuronium.mp4 ../tests/ocrRocuronium.csv
./ocr ../data/Phenylephrine.mp4 ../tests/ocrPhenylephrine.csv
./ocr ../data/Lidocaine.mp4 ../tests/ocrLidocaine.csv
./ocr ../data/Dexamethasone.mp4 ../tests/ocrDexamethasone.csv
./ocr ../data/Odanestron.mp4 ../tests/ocrOdanestron.csv
./ocr ../data/2_label.mp4 ../tests/ocr2_label.csv
./ocr ../data/3_label.mp4 ../tests/ocr3_label.csv
./ocr ../data/4_label.mp4 ../tests/ocr4_label.csv
./ocr ../data/5_label.mp4 ../tests/ocr5_label.csv
./ocr ../data/6_label.mp4 ../tests/ocr6_label.csv
./ocr ../data/no_color.mp4 ../tests/ocrno_color.csv
./ocr ../data/blank.mp4 ../tests/ocrblank.csv
cd ../utils
./scorestamps ../data/Propofol.csv ../tests/ocrPropofol.csv
./scorestamps ../data/Rocuronium.csv ../tests/ocrRocuronium.csv
./scorestamps ../data/Phenylephrine.csv ../tests/ocrPhenylephrine.csv
./scorestamps ../data/Lidocaine.csv ../tests/ocrLidocaine.csv
./scorestamps ../data/Dexamethasone.csv ../tests/ocrDexamethasone.csv
./scorestamps ../data/Odanestron.csv ../tests/ocrOdanestron.csv
./scorestamps ../data/2_label.csv ../tests/ocr2_label.csv
./scorestamps ../data/3_label.csv ../tests/ocr3_label.csv
./scorestamps ../data/4_label.csv ../tests/ocr4_label.csv
./scorestamps ../data/5_label.csv ../tests/ocr5_label.csv
./scorestamps ../data/6_label.csv ../tests/ocr6_label.csv
./scorestamps ../data/no_color.csv ../tests/ocrno_color.csv
./scorestamps ../data/blank.csv ../tests/ocrblank.csv
