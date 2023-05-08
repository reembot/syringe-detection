import cv2
import mediapipe as mp
import time
import numpy as np
import sys
from HandDetector import HandDetector
import cython
import pyximport
pyximport.install()
import labelColorsModule as lcm

cap = cv2.VideoCapture(sys.argv[1])
frame_width = int(cap.get(3))
frame_height = int(cap.get(4))
size = (frame_width,frame_height)
vout = cv2.VideoWriter(sys.argv[3],cv2.VideoWriter_fourcc(*'H264'),10,size)

hand_detector = HandDetector()
startPinch=0
pinch=0
thumbx=0
thumby=0
thumbx2=0
thumbx3=0
thumby2=0
thumby3=0
stepflag = 0
mednames = ["None","Propofol", "Rocuronium", "Phenylephrine", "Lidocaine", "Odanestron", "Dexamethasone"]
hasmeds = [0,0,0,0,0,0,0]
oldmedtime=0
f = open(sys.argv[2],'w')
cnt = 0

while cap.isOpened():
    success, image = cap.read()
    if not success:
        break
    cnt+=1
    medtime = int(cap.get(cv2.CAP_PROP_POS_MSEC))
    image_with_hands, coordinates = hand_detector.detect(image)
    if coordinates is not None and medtime-oldmedtime > 2000:
        if (startPinch==0):
            startPinch = hand_detector.get_distance_between_fingers()
        else:
            pinch = hand_detector.get_distance_between_fingers()
            totalChange = abs(pinch-startPinch)/startPinch
            thumbx3=thumbx2
            thumby3=thumby2
            thumbx2=thumbx
            thumby2=thumby
            thumbx = int(hand_detector.hand_landmarks[4][0] * image.shape[1])
            thumby = int(hand_detector.hand_landmarks[4][1] * image.shape[0])
            if(totalChange > 0.35 and pinch < 0.4):
                medLabel=0
                trial=0
                checkx=0
                checky=0
                while(medLabel==0 and trial!=3):
                    trial+=1
                    if(trial==1):
                        checkx = thumbx
                        checky = thumby
                    elif(trial==2):
                        checkx = thumbx2
                        checky = thumby2
                    else:
                        checkx = thumbx3
                        checky = thumby3
                    if(checky+100 < image.shape[0]):
                        medLabel = lcm.labelColors(image_with_hands[checky:checky+100,checkx-400:checkx],1,1,1,1,1,1)
                    else:
                        medLabel = lcm.labelColors(image_with_hands[checky:image.shape[0]-1,checkx-400:checkx],1,1,1,1,1,1)
                    if(medLabel!=0):
                        if(hasmeds[medLabel]):
                            hasmeds[medLabel]=0
                        else:
                            hasmeds[medLabel]=1
                        f.write(mednames[medLabel])
                        f.write(',')
                        f.write(str(medtime))
                        f.write('\n')
                        oldmedtime = medtime
                if(checkx!=0):
                    cv2.rectangle(image_with_hands,(checkx-400,checky),(checkx,checky+100),(0,0,255),1)   
    else:
        startPinch=0                                        

    for i in range(1,7):
        if(hasmeds[i]):
            cv2.putText(image_with_hands, mednames[i], (30,50+(i*50)), cv2.FONT_HERSHEY_COMPLEX_SMALL, 0.8, (0,0,255), 1, cv2.LINE_AA);    

    vout.write(image_with_hands)


#    cv2.imshow("MediaPipe Hands", image_with_hands)

    if(stepflag==1):
        winInput = cv2.waitKey(0)
    else:
        winInput = cv2.waitKey(1)

    if(winInput == 27):
        break
    elif(winInput == 32):
        if(stepflag==1):
            stepflag = 0
        else:
            stepflag = 1
    
cap.release()
vout.release()
cv2.destroyAllWindows()