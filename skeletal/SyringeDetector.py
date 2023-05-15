import cv2
import mediapipe as mp
import time
import numpy as np


class SyringeDetector:
    def __init__(self):
        self.image = None
        self.isCalibrated = False
    def calibrate(self, image):
        self.image = image
        self.isCalibrated = True
    def detect(self, image):
        if not self.isCalibrated:
            raise Exception("SyringeDetector is not calibrated")
        # Convert the image to grayscale
        self_gray = cv2.cvtColor(self.image, cv2.COLOR_BGR2GRAY)
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        # Apply a Gaussian blur to the image
        # Apply a Gaussian blur to the image
        self_gray_blur = cv2.GaussianBlur(self_gray, (21, 21), 0)
        gray_blur = cv2.GaussianBlur(gray, (21, 21), 0)
        
        # Compute the absolute difference between the current image and the calibrated image
        diff = cv2.absdiff(self_gray_blur, gray_blur)
        
        # Apply a threshold to the difference image
        threshold = cv2.threshold(diff, 100, 255, cv2.THRESH_BINARY)[1]
        
        # Find the contours in the thresholded image
        contours, hierarchy = cv2.findContours(threshold, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        
        # Draw the contours on the original image
        cv2.drawContours(image, contours, -1, (0, 255, 0), 2)
        
        # Show the difference image
        cv2.imshow("Difference Image", diff)
        
        # Show the thresholded image
        cv2.imshow("Thresholded Image", threshold)
        
        # Return the coordinates of the contours
        return contours