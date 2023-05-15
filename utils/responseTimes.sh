#!/bin/bash
echo "Centroid Average Response Time Difference (ms)"
var=0
var=$(($var+$(./timedstamps ../data/Propofol.csv ../tests/centroids-Propofol.csv)))
var=$(($var+$(./timedstamps ../data/Rocuronium.csv ../tests/centroids-Rocuronium.csv)))
var=$(($var+$(./timedstamps ../data/Phenylephrine.csv ../tests/centroids-Phenylephrine.csv)))
var=$(($var+$(./timedstamps ../data/Lidocaine.csv ../tests/centroids-Lidocaine.csv)))
var=$(($var+$(./timedstamps ../data/Dexamethasone.csv ../tests/centroids-Dexamethasone.csv)))
var=$(($var+$(./timedstamps ../data/Odanestron.csv ../tests/centroids-Odanestron.csv)))
var=$(($var+$(./timedstamps ../data/2_label.csv ../tests/centroids-2_label.csv)))
var=$(($var+$(./timedstamps ../data/3_label.csv ../tests/centroids-3_label.csv)))
var=$(($var+$(./timedstamps ../data/4_label.csv ../tests/centroids-4_label.csv)))
var=$(($var+$(./timedstamps ../data/5_label.csv ../tests/centroids-5_label.csv)))
var=$(($var+$(./timedstamps ../data/6_label.csv ../tests/centroids-6_label.csv)))
var=$(($var+$(./timedstamps ../data/no_color.csv ../tests/centroids-no_color.csv)))
var=$(($var/12))
echo $var
echo "HOGSVM Average Response Time Difference (ms)"
var2=0
var2=$(($var2+$(./timedstamps ../data/Propofol.csv ../tests/hogsvmNoDisplayPropofol.csv)))
var2=$(($var2+$(./timedstamps ../data/Rocuronium.csv ../tests/hogsvmNoDisplayRocuronium.csv)))
var2=$(($var2+$(./timedstamps ../data/Phenylephrine.csv ../tests/hogsvmNoDisplayPhenylephrine.csv)))
var2=$(($var2+$(./timedstamps ../data/Lidocaine.csv ../tests/hogsvmNoDisplayLidocaine.csv)))
var2=$(($var2+$(./timedstamps ../data/Dexamethasone.csv ../tests/hogsvmNoDisplayDexamethasone.csv)))
var2=$(($var2+$(./timedstamps ../data/Odanestron.csv ../tests/hogsvmNoDisplayOdanestron.csv)))
var2=$(($var2+$(./timedstamps ../data/2_label.csv ../tests/hogsvmNoDisplay2_label.csv)))
var2=$(($var2+$(./timedstamps ../data/3_label.csv ../tests/hogsvmNoDisplay3_label.csv)))
var2=$(($var2+$(./timedstamps ../data/4_label.csv ../tests/hogsvmNoDisplay4_label.csv)))
var2=$(($var2+$(./timedstamps ../data/5_label.csv ../tests/hogsvmNoDisplay5_label.csv)))
var2=$(($var2+$(./timedstamps ../data/6_label.csv ../tests/hogsvmNoDisplay6_label.csv)))
var2=$(($var2+$(./timedstamps ../data/no_color.csv ../tests/hogsvmNoDisplayno_color.csv)))
var2=$(($var2/12))
echo $var2
echo "Lines Average Response Time Difference (ms)"
var3=0
var3=$(($var3+$(./timedstamps ../data/Propofol.csv ../tests/linesNoDisplayPropofol.csv)))
var3=$(($var3+$(./timedstamps ../data/Rocuronium.csv ../tests/linesNoDisplayRocuronium.csv)))
var3=$(($var3+$(./timedstamps ../data/Phenylephrine.csv ../tests/linesNoDisplayPhenylephrine.csv)))
var3=$(($var3+$(./timedstamps ../data/Lidocaine.csv ../tests/linesNoDisplayLidocaine.csv)))
var3=$(($var3+$(./timedstamps ../data/Dexamethasone.csv ../tests/linesNoDisplayDexamethasone.csv)))
var3=$(($var3+$(./timedstamps ../data/Odanestron.csv ../tests/linesNoDisplayOdanestron.csv)))
var3=$(($var3+$(./timedstamps ../data/2_label.csv ../tests/linesNoDisplay2_label.csv)))
var3=$(($var3+$(./timedstamps ../data/3_label.csv ../tests/linesNoDisplay3_label.csv)))
var3=$(($var3+$(./timedstamps ../data/4_label.csv ../tests/linesNoDisplay4_label.csv)))
var3=$(($var3+$(./timedstamps ../data/5_label.csv ../tests/linesNoDisplay5_label.csv)))
var3=$(($var3+$(./timedstamps ../data/6_label.csv ../tests/linesNoDisplay6_label.csv)))
var3=$(($var3+$(./timedstamps ../data/no_color.csv ../tests/linesNoDisplayno_color.csv)))
var3=$(($var3/12))
echo $var3
echo "OCR Average Response Time Difference (ms)"
var4=0
var4=$(($var4+$(./timedstamps ../data/Propofol.csv ../tests/ocrNoDisplayPropofol.csv)))
var4=$(($var4+$(./timedstamps ../data/Rocuronium.csv ../tests/ocrNoDisplayRocuronium.csv)))
var4=$(($var4+$(./timedstamps ../data/Phenylephrine.csv ../tests/ocrNoDisplayPhenylephrine.csv)))
var4=$(($var4+$(./timedstamps ../data/Lidocaine.csv ../tests/ocrNoDisplayLidocaine.csv)))
var4=$(($var4+$(./timedstamps ../data/Dexamethasone.csv ../tests/ocrNoDisplayDexamethasone.csv)))
var4=$(($var4+$(./timedstamps ../data/Odanestron.csv ../tests/ocrNoDisplayOdanestron.csv)))
var4=$(($var4+$(./timedstamps ../data/2_label.csv ../tests/ocrNoDisplay2_label.csv)))
var4=$(($var4+$(./timedstamps ../data/3_label.csv ../tests/ocrNoDisplay3_label.csv)))
var4=$(($var4+$(./timedstamps ../data/4_label.csv ../tests/ocrNoDisplay4_label.csv)))
var4=$(($var4+$(./timedstamps ../data/5_label.csv ../tests/ocrNoDisplay5_label.csv)))
var4=$(($var4+$(./timedstamps ../data/6_label.csv ../tests/ocrNoDisplay6_label.csv)))
var4=$(($var4+$(./timedstamps ../data/no_color.csv ../tests/ocrNoDisplayno_color.csv)))
var4=$(($var4/12))
echo $var4
var5=$(($var+$var2+$var3+$var4))
var5=$(($var5/4))
echo "Total Average Response Time Difference (ms)"
echo $var5
