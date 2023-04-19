import cv2
import mediapipe as mp
import time
import numpy as np


class LineDetector:
    def __init__(self, min_line_length=50, max_line_gap=10):
        self.min_line_length = min_line_length
        self.max_line_gap = max_line_gap
        
    def detectMat(self, image):
        # Convert the image to grayscale
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        # Apply a Gaussian blur to the image
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        
        # Detect edges using the Canny edge detection algorithm
        edges = cv2.Canny(blurred, 50, 150, apertureSize=3)
        
        # Apply a Hough transform to detect lines
        lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 100, self.min_line_length, self.max_line_gap)
        
        # Filter the lines to only keep the ones that are parallel to each other
        parallel_lines = []
        for i in range(len(lines)):
            for j in range(i+1, len(lines)):
                line1 = lines[i][0]
                line2 = lines[j][0]
                angle1 = np.arctan2(line1[3] - line1[1], line1[2] - line1[0]) * 180 / np.pi
                angle2 = np.arctan2(line2[3] - line2[1], line2[2] - line2[0]) * 180 / np.pi
                if np.abs(angle1 - angle2) < 10:
                    parallel_lines.append((line1, line2))
        
        image_copy = image.copy()
        # Draw the lines on the original image
        for line in parallel_lines:
            cv2.line(image_copy, (line[0][0], line[0][1]), (line[0][2], line[0][3]), (0, 0, 255), 2)
            cv2.line(image_copy, (line[1][0], line[1][1]), (line[1][2], line[1][3]), (0, 0, 255), 2)
        
        # cv2.imshow("Lines", image_copy)
        
        #lets find the region of interest(where most parallel lines are)
        largest_cluster = []
        max_cluster_size = 0
        
        for i, line in enumerate(parallel_lines):
            cluster = [line]
            for j in range(i+1, len(parallel_lines)):
                if abs(line[0][0] - parallel_lines[j][0][0]) <= 100 and abs(line[0][1] - parallel_lines[j][0][1]) <= 100:
                    cluster.append(parallel_lines[j])
            if len(cluster) > max_cluster_size:
                largest_cluster = cluster
                max_cluster_size = len(cluster)
        
        print("Largest cluster size: ", max_cluster_size)
        
        #if we have a cluster of lines, we can find the region of interest
        if len(largest_cluster) > 0:
            x_min = min([min(line[0][0], line[1][0]) for line in largest_cluster])
            x_max = max([max(line[0][0], line[1][0]) for line in largest_cluster])
            y_min = min([min(line[0][1], line[1][1]) for line in largest_cluster])
            y_max = max([max(line[0][1], line[1][1]) for line in largest_cluster])
            
        # Draw the lines on the original image
        ROI = image[y_min:y_max, x_min:x_max]
        # cv2.imshow("ROI", ROI)
        
        return image_copy, parallel_lines, np.array([x_min, y_min, x_max, y_max])