# syringe-detection
## Final project for CSCI612's Applied Computer Vision

[Project Proposal](https://docs.google.com/document/d/1sehzGn4JcLvFzAshE9lnEjXlV90Zkglq8QPydF9xlk8/edit)

[Google Drive](https://drive.google.com/drive/folders/1tQvCCrOVV-lOu0Dz0uJPwqgfOd_VtzBX)

To build:
chmod +x makeAll.sh
./makeAll.sh

To run examples:
chmod +x runExamples.sh
./runExamples.sh

Note: Unless noted otherwise, all code runs on Jetson Nano 2G.

The following scripts run all programs with no display, create results tables and output script times:
../centroids/centroidsMetrics.sh
../hogsvm/hogsvmMetrics.sh
../lines/linesMetrics.sh
../ocr/ocrMetrics.sh
../ocrCUDA/ocrCUDAMetrics.sh

Output from scripts include raw csv files of timestamps in tests folder and final classifier metrics csv in results folder.

Raw data and test csv file format:  
medication name,
msecs into video

Final Classifier Metrics csv file format:  
manual review timestamp filename, 
test result timestamp filename, 
medication label or all total labels in file, 
true positive frames (TP), 
false positive frames (FP), 
true negative frames (TN),
false negative frames (FN),
total frames (T),
true positive rate (TPR),
false positive rate (FPR),
precision (PPR),
accuracy (ACC),
balanced accuracy (F1)

Please review these scripts for insight to individual video analysis.

The following scripts output video results from each method:
../centroids/centroidsVout.sh
../hogsvm/hogsvmVout.sh
../lines/linesVout.sh
../ocr/ocrVout.sh

The follow utilities are in the utils folder:
colorhist
use: ./color imageFileName.imgageformatextension
description:  Produces imageFileName.colormmperhist.csv multi-modal BGR histogram useful for finding InRange values and patterns between values.

createVideo
use: ./createVideo videoFileName.videoformatextension
description:  Produces challenge videos from camera.  Can also create stills from videos or camera.

labelColors.h
description:  Include file for use in hogsvm, lines and ocr

responseTimes.sh
use: ./responseTimes.sh
description:  Produces average response differences between data timestamps and test timestamps.  Must run all metrics scripts first (see above).

scorestamps
use: ./scorestamps datafile.csv testfile.csv
description:  Produces Classifier Metrics (see above) csv file for a single video data file and one application test set.

showstamps
use: ./showstamps inputvideofilename.videofileformat inputdatafilename.csv
description:  Displays video file with medication label names printed in frame as they appear at the times given in the data file.  Used for verification of data timestamps.

timedstamps
use: ./timedstamps datafile.csv testefile.csv
description: Produces single response differences for a single video data file and one application test set.  Used by responseTimes.sh (see above)

vidstamps
use: ./vidstamps inputvideofilename.videofileformat outputdatafilename.csv
description:  Produces data csv file (see format above).  Displays Input Video and creates csv row entry when user selects 1 to 6 entry corresponding to medications (Propofol, Rocuronium, Phenylephrine, Lidocaine, Ondanestron, Dexamethasone).  Users can press 'esc' to cancel, pause video by pressing 'p' and unpause with 'u' or step through to next frame with any other keystroke.

Folder Structure
centroids:  centroid application
data: videos and manually created timestamp csvs
explore: can be ignored (was used for experimentation and collaboration)
hogsvm:  hogsvm application
lines:  lines application
ocr:  ocr application
ocrCUDA:  ocr application using CUDA, in separate folder for ease of Makefile differences
results:  final classifier metric csv files (see format above) for each application
skeletal:  skeletal python code for use on Mac or Raspberry Pi
stills:  images used for training hogsvm and CNN (CNN code not included in repo, proprietary to Riverview Scientific)
syringes:  images and histograms of syringes for analysis
tests:  timestamp csv file output from each application with name format applicationnameVideoname.csv
utils:  utitilities for preparing videos for use in applications, see individual descriptions above
vidout:  location of output of videos from video scripts (see above)
