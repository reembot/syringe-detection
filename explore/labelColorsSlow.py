def labelColors(frame, checkR, checkPr, checkPh, checkL, checkO, checkD):
    countR=0
    countPr=0
    countPh=0
    countL=0
    countW=0     
    #loop through every pixel or until a new medication label is detected
    for j in range(frame.shape[0]):
        for i in range(frame.shape[1]):
#    for j in range(0,2):
#        for i in range(0,2):
            #get bgr values
            bval, gval, rval = frame[j,i]
#            print(bval, gval, rval)
            #check for yellow colors where red is highest followed closely by green with almost no blue
            if ((rval > 96) and (rval-bval > rval/3) and (bval < 32)  and (gval < rval * 9 / 10) and (gval > 3 * rval / 5) ):
                countPr+=1  
                if(checkPr and countPr > 1000):
                    return 1
            #check for mostly red color
            elif( (rval > 224) and (bval < 64) and (gval < 96) ):
                countR+=1
                if(checkR and countR > 1000):
                    return 2
            #check for light purple where most values are high and green is 1/2-3/4 of red
            elif( (rval > 128) and (bval > 128) and (gval < 3 * rval / 4) and (gval > rval / 2) ):
                countPh+=1
                if(checkPh and countPh > 2000):
                    return 3
            #check for non-color values (gray and white)
            elif ( (abs(rval-gval) < 20) and (abs(rval-bval) < 20) and (abs(gval-bval) < 20) ):
                #check for gray
                if ( (rval > 48) and (gval > 48) and (bval > 48) and (rval < 80) and (gval < 80) and (bval < 80) ):
                    countL+=1
                    if(checkL and countL > 3000): 
                                 return 4
            #check for white
            elif ( (rval > 175) and (gval > 175) and (bval > 175) ):
                countW+=1 
                #first text white label threshold
                if((checkO or checkD) and countW > 500):
                    frame = cv.pyrUp(frame,cv.Size(frame.shape[0]*2,frame.shape[1]*2))
                    frame = cv.pyrUp(frame,cv.Size(frame.shape[1]*2,frame.shape[1]*2))
                    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
                    gray = cv.GaussianBlur(gray, cv.Size(7, 7), 0)
                    circles = HoughCircles(gray, cv.HOUGH_GRADIENT, 1, gray.shape[1]/32, 70, 20, 20, 35)
                    if(circles.size() > 0):
                        return 5
                    else:
                        return 6
    return 0