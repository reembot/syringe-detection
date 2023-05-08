import numpy as np
import cv2 as cv
import sys
import cython
import pyximport
pyximport.install()
import labelColorsModule as lcm
#from labelColorsSlow import labelColors
cap = cv.VideoCapture(sys.argv[1])
if not cap.isOpened():
    exit()
#frame_width = int(cap.get(3))
#frame_height = int(cap.get(4))
frame_width = 1280
frame_height = 480
size = (frame_width,frame_height)
#vout = cv.VideoWriter(sys.argv[3],cv.VideoWriter_fourcc(*'H264'),10,size)
mednames = ["Propofol", "Rocuronium", "Phenylephrine", "Lidocaine", "Odanestron", "Dexamethasone"]
hadmeds = [0,0,0,0,0,0]
oldmedtime=0
f = open(sys.argv[2],'w')
while True:
    # Capture frame-by-frame
    ret, src = cap.read()
    # if frame is read correctly ret is True
    if not ret:
#        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    medtime = int(cap.get(cv.CAP_PROP_POS_MSEC))
    img = cv.cvtColor(src, cv.COLOR_BGR2GRAY)
    size = np.size(img)
    skel = np.zeros(img.shape,np.uint8)
    ret,img = cv.threshold(img,20,255,0)
    element = cv.getStructuringElement(cv.MORPH_CROSS,(120,120))
    eroded = cv.erode(img,element)
    edges = cv.Canny(eroded,127,254)
    if(medtime-oldmedtime > 2000):
#    if(1):
        circles = cv.HoughCircles(eroded,cv.HOUGH_GRADIENT,10,1000,param1=50,param2=254,minRadius=75,maxRadius=125) 
        medLabel = 0
        if circles is not None:
            circles = np.uint16(np.around(circles))
            i = circles[0,0]
#           cv.circle(src,(i[0],i[1]),i[2],(0,255,0),2)
            rx = i[0]
            ry = i[1]
            if(rx-400 > 0 and ry-100 > 0 and ry+100 < src.shape[0] and rx < 1000):
                cv.rectangle(src,(rx-400,ry-100),(rx,ry+100),(0,0,255),1) 
                medLabel = lcm.labelColors(src[rx-600:rx,ry-100:ry+100],1,1,1,1,1,1)
#                medLabel = labelColors(src[rx-600:rx,ry-100:ry+100],1,1,1,1,1,1)
#                print(medLabel)
                if(medLabel!=0):
                    if(hadmeds[medLabel-1]):
                        hadmeds[medLabel-1]=0
                    else:
                        hadmeds[medLabel-1]=1
                    f.write(mednames[medLabel-1])
                    f.write(',')
                    f.write(str(medtime))
                    f.write('\n')
                    oldmedtime = medtime
    for i in range(0,6):
        if(hadmeds[i]):
            cv.putText(src, mednames[i], (30,50+(i*50)), cv.FONT_HERSHEY_COMPLEX_SMALL, 0.8, (0,0,255), 1, cv.LINE_AA);    

#    cv.imshow("skel",eroded)
#    cv.imshow("orig",src)
#    cv.imshow("canned",edges)


    edges = cv.resize(edges,(640,480))
    edges = cv.cvtColor(edges, cv.COLOR_GRAY2BGR)
    src = cv.resize(src,(640,480))
    sidebyside = np.concatenate((src,edges),axis=1)
#    cv.imshow("orig",sidebyside)
#    vout.write(sidebyside)
    
    waitVal = cv.waitKey(1)
    if (waitVal == ord('q')):
        break
    elif (waitVal == ord(' ')):
      cv.waitKey(0)

cap.release()
#vout.release()
#cv.destroyAllWindows()