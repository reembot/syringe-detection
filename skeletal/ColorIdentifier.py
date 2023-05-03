import cv2
import numpy as np
from sklearn.cluster import KMeans
from matplotlib import pyplot as plt

class ColorIdentifier:
    def __init__(self):
        # Define the colors to recognize
        self.colors = {
            'orange': [255, 128, 0],
            'white': [255, 255, 255],
            'pink': [220, 200, 200],
            'yellow': [255, 255, 0],
        }
        self.colors_hsv = {
            'orange': [120, 165, 225],
            'white': [50, 0, 235],
            'pink': [150, 20, 225],
            'yellow': [85, 155, 240]
        }
        self.syringe_colors = None


    def get_color(self, img):
        # Convert input image from BGR to HSV color space
        no_syringe = self.get_syringe_color(img)
        cv2.imshow("no_syringe", no_syringe)
        bgr = cv2.cvtColor(no_syringe, cv2.COLOR_HSV2BGR)
        
        unique_colors = np.unique(bgr.reshape(-1, bgr.shape[-1]), axis=0)
        
        avg_color = np.mean(unique_colors, axis=0)
        
        return avg_color

        

    def get_color_name(self, color):
            # Calculate the distance to each color
        distances = {}
        for name, value in self.colors.items():
            value_float = np.array(value, dtype=np.float32)
            dist = np.sqrt(np.sum((value_float - color)**2))
            distances[name] = dist
        
        # Return the name of the closest color
        return min(distances, key=distances.get)
    
    def get_color_name_hsv(self, color):
        distances = {}
        for name, value in self.colors_hsv.items():
            value_float = np.array(value, dtype=np.float32)
            dist = np.sqrt(np.sum((value_float - color)**2))
            distances[name] = dist
        return min(distances, key=distances.get)
    
    
    def get_syringe_color(self, img):
        # Convert BGR to HSV color space
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        # Calculate the histogram of the Hue channel
        hist = cv2.calcHist([hsv], [0], None, [180], [0, 180])

        # Find the bin with the highest frequency
        max_bin = np.argmax(hist)

        # Define a lower and upper threshold based on the most common color
        lower = np.array([max_bin - 150, 10, 10])
        upper = np.array([max_bin + 150, 255, 255])

        # Create a binary mask using the thresholded Hue channel
        mask = cv2.inRange(hsv, lower, upper)

        # Apply the mask to the original image to extract the region of interest
        result = cv2.bitwise_and(img, img, mask=mask)

        return result
    def get_syringe_colorChatGPT(self, img, k=3):
        # Flatten the image array and convert to a n x 3 matrix
        pixel_matrix = img.reshape(-1, 3)

        # Cluster the pixels using KMeans
        kmeans = KMeans(n_clusters=k, n_init=10, random_state=42)
        kmeans.fit(pixel_matrix)

        # Get the cluster centers and select the first center as the syringe color
        centers = kmeans.cluster_centers_
        syringe_color = centers[0]

        # Define a lower and upper bound for each HSV channel based on the syringe color
        hsv_min = np.array([syringe_color[0] - 100, 50, 50])
        hsv_max = np.array([syringe_color[0] + 100, 255, 255])

        # Create a binary mask using the acceptable color range
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, hsv_min, hsv_max)

        # Apply the mask to the original image to extract the region of interest
        result = cv2.bitwise_and(img, img, mask=mask)

        return result
    
    def adaptive_gaussian_threshold(self, image):
        # Convert image to grayscale
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        # Apply adaptive Gaussian thresholding
        threshold_image = cv2.adaptiveThreshold(gray_image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                                cv2.THRESH_BINARY_INV, 31, 2)
        
        cv2.imshow("threshold_image", threshold_image)
    
    #https://github.com/mrakelinggar/data-stuffs/tree/master/frequent_color
    def get_k_means_cluster(self, img, k=2):
        # self.adaptive_gaussian_threshold(img)
        # no_syringe = self.get_syringe_color(img)
        # cv2.imshow("no_syringe", no_syringe)
        no_syringe = img.copy()
        
        contours, hierarchy = cv2.findContours(cv2.cvtColor(no_syringe, cv2.COLOR_BGR2GRAY), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        if len(contours) == 0:
            return None
        max_contour = max(contours, key=cv2.contourArea)
        color_coordinates = cv2.boundingRect(max_contour)
        syringe_colors = no_syringe[color_coordinates[1]:color_coordinates[1]+color_coordinates[3], color_coordinates[0]:color_coordinates[0]+color_coordinates[2]]
        
        try:
            clt = KMeans(n_clusters=k,n_init =10, random_state=42)
            clt.fit(syringe_colors.reshape(-1, 3))
            width=300
            palette = np.zeros((50, width, 3), np.uint8)
            #remove black and white
            for idx, centers in enumerate(clt.cluster_centers_): 
                #if none of the values are above 50, then it's black
                if np.all(centers < 80) or np.all(centers > 200) and len(clt.cluster_centers_) > 1:
                    clt.cluster_centers_ = np.delete(clt.cluster_centers_, idx, 0)
                    clt.labels_ = np.delete(clt.labels_, idx, 0)
            steps = width/clt.cluster_centers_.shape[0]
            for idx, centers in enumerate(clt.cluster_centers_): 
                palette[:, int(idx*steps):(int((idx+1)*steps)), :] = centers.astype("uint8")
                print(self.get_color_name(centers) + "= " + str(centers))
                
                
            cv2.imshow("palette", palette)
            cv2.imshow("syringe", img)
            return palette
        except:
            print("There was ann error in get_k_means_cluster")
            
            
    # Define a function to check if a color is gray
    def is_gray(self, color):
        # A color is gray if the difference between its RGB values is less than a threshold
        diff_threshold = 5
        avg_threshold = 200
        return abs(color[0] - color[1]) < diff_threshold and abs(color[1] - color[2]) < diff_threshold and np.mean(color) < avg_threshold

    
    def kmeans_ignore_gray(self, img):
        
        pixels = cv2.cvtColor(img, cv2.COLOR_BGR2RGB).reshape(-1, 3)
        
        # Apply KMeans clustering to group the pixels by color
        kmeans = KMeans(n_clusters=5, n_init=10, random_state=42)
        kmeans.fit(pixels)

        # Get the cluster centers and labels
        centers = kmeans.cluster_centers_.astype(int)
        labels = kmeans.labels_

        # Count the frequency of each cluster
        counts = np.bincount(labels)

        # Find the cluster with the highest frequency, excluding gray colors
        max_count = 0
        dominant_color = None
        white_color = None
        for i in range(len(centers)):
            # Check if the color is not gray
            if not self.is_gray(centers[i]):
                # Check if the count is higher than the current maximum
                if counts[i] > max_count:
                    # Update the maximum count and the dominant color
                    if self.get_color_name_hsv(cv2.cvtColor(np.uint8([[centers[i]]]), cv2.COLOR_BGR2HSV)) == "white":
                        white_color = centers[i]
                        continue
                    max_count = counts[i]
                    dominant_color = centers[i]

        # Print the dominant color
        #create a square image of the dominant color
        if dominant_color is None and white_color is None:
            print("dominant color is none")
            return None
        if dominant_color is None and white_color is not None:
            dominant_color = white_color
        color_image = np.zeros((50, 50, 3), np.uint8)
        color_image[:] = dominant_color
        color_image = cv2.cvtColor(color_image, cv2.COLOR_RGB2BGR)
        cv2.imshow("dominant_color", color_image)
        # print(str(dominant_color) + " = " + self.get_color_name(dominant_color))
        #HSV seems to be more accurate
        hsv = cv2.cvtColor(np.uint8([[dominant_color]]), cv2.COLOR_BGR2HSV)
        hsv_color_name = self.get_color_name_hsv(hsv)
        print(str(hsv) + " = " + hsv_color_name)
        return hsv_color_name
        
                    
    
    
    