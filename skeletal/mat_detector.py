import cv2
import numpy as np

class mat_detector:
    def __init__(self):
        self.mat_coordinates = np.array([0, 0, 0, 0])
        self.mat_detected = False
        self.detected_count = 0
        
    def detect_mat(self, img):
        # Convert the image to grayscale
        # Convert the image to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Define the lower and upper range for black pixels
        lower_black = np.array([0])
        upper_black = np.array([50])

        # Create a mask using cv2.inRange()
        mask = cv2.inRange(gray, lower_black, upper_black)

        cv2.imshow("mask", mask)
        
        contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        
        areas = [cv2.contourArea(c) for c in contours]
        max_index = np.argmax(areas)
        cnt = contours[max_index] 
        
        x, y, w, h = cv2.boundingRect(cnt)
        
        self.detected_count += 1
        
        if self.detected_count > 10:
            self.mat_detected = True
        
        
        #we want to store the largest mat coordinates
        if w*h > self.mat_coordinates[2]*self.mat_coordinates[3]:
            self.mat_coordinates = np.array([x, y, w, h])
            
        

    def get_mat_ROI(self, img):
        # If the mat was detected, extract the region of interest
        if self.mat_detected:
            return img[self.mat_coordinates[1]:self.mat_coordinates[1]+self.mat_coordinates[3], self.mat_coordinates[0]:self.mat_coordinates[0]+self.mat_coordinates[2]]
        else:
            return None