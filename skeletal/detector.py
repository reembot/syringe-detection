import cv2
import numpy as np

class FaceDetector:
    
    def __init__(self, image):
        self.img = image
        self.face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
        self.lower_skin = None
        self.upper_skin = None
    
    def detect_face(self):
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        
        faces = self.face_cascade.detectMultiScale(gray, 1.3, 5)
        
        if len(faces) == 0:
            return None
        
        (x, y, w, h) = faces[0]
        
        face_image = img[y:y+h, x:x+w]
        
        return (x, y, w, h)
    def skin_color_boundary(self, boundary):
        x, y, w, h = boundary
        
        # get the region of interest(the face) from the image
        roi = self.img[y:y+h, x:x+w]
        
        # convert roi to hsv color space
        hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)
        
        #find the lowset and highest hsv values in the roi
        # find the minimum and maximum HSV values in the roi
        h_mean, s_mean, v_mean = np.mean(hsv, axis=(0, 1))
        h_std, s_std, v_std = np.std(hsv, axis=(0, 1))

        # Define the HSV range for skin tone based on the mean and standard deviation
        lower_skin = np.array([h_mean - h_std, s_mean - s_std, v_mean - v_std])
        upper_skin = np.array([h_mean + h_std, s_mean + s_std, v_mean + v_std])
        
        return lower_skin, upper_skin
    def update_skin_color(self, lower_skin, upper_skin):
        if self.lower_skin is None:
            self.lower_skin = lower_skin
        if self.upper_skin is None:
            self.upper_skin = upper_skin
            
        else:
            #lets average it
            self.lower_skin = (self.lower_skin + lower_skin) / 2
            self.upper_skin = (self.upper_skin + upper_skin) / 2
    def remove_face(self, boundary):
        if boundary is None:
            return
        
        mask = np.zeros(self.img.shape[:2], dtype=np.uint8)
        cv2.rectangle(mask, (boundary[0], boundary[1]), (boundary[0]+boundary[2], boundary[1]+boundary[3]), 255, -1)
        
        masked_img = cv2.bitwise_and(self.img, self.img, mask=mask)
    
    
        face_less_image = self.img - masked_img
        
        return face_less_image

        
class HandDetector:
    def __init__(self, image, lower_skin, upper_skin):
        self.img = image
        self.lower_skin = lower_skin
        self.upper_skin = upper_skin
        self.roi = None
    def segment(self):
        if self.lower_skin is None or self.upper_skin is None or self.img is None:
            return None
        # Convert frame to HSV color space
        hsv = cv2.cvtColor(self.img, cv2.COLOR_BGR2HSV)
        
        #threshold the image based on our calculated HSV values
        mask = cv2.inRange(hsv, self.lower_skin, self.upper_skin)
        
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
        if len(contours) == 0:
            return None

        # Find the largest contour (the hand)
        contour = max(contours, key=cv2.contourArea)
        
        return contour
    def where_is_hand(self, contour):
        
        if contour is None:
            return None
        
        # Get the bounding box of the hand contour
        x, y, w, h = cv2.boundingRect(contour)
        
        # Crop the original image to the bounding box
        hand_roi = self.img[y:y+h, x:x+w]
        
        cv2.imshow('hand_roi', hand_roi)
        self.roi = hand_roi
        return hand_roi
  
    def skeletal_transform(self, roi):
        if roi is None:
            return None
        gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
        
        size = np.size(gray)
        skel = np.zeros(gray.shape,np.uint8)

        ret,gray = cv2.threshold(gray,127,255,0)
        element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
        done = False

        while not done:
            eroded = cv2.erode(gray,element)
            temp = cv2.dilate(eroded,element)
            temp = cv2.subtract(gray,temp)
            skel = cv2.bitwise_or(skel,temp)
            gray = eroded.copy()

            zeros = size - cv2.countNonZero(gray)
            if zeros == size:
                done = True
        
        cv2.imshow('skel', skel)
        return skel
        
        
        
        
    


face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')


cap = cv2.VideoCapture(0)
counter = 0
lower_skin = None
upper_skin = None
while True:
    ret, img = cap.read()
    face_detector = FaceDetector(img)
    face_cord = face_detector.detect_face()
    
    if counter < 20:
        

        if face_cord is not None:
            x, y, w, h = face_cord
            cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
            
            lower, upper = face_detector.skin_color_boundary(face_cord)
            face_detector.update_skin_color(lower, upper)
            lower_skin = face_detector.lower_skin
            upper_skin = face_detector.upper_skin
            counter += 1
            
            
    else:
        
        face_removed = face_detector.remove_face(face_cord)
        
        hand_detector = HandDetector(face_removed, lower_skin, upper_skin)
        
        contour = hand_detector.segment()
        
        hand_detector.where_is_hand(contour)
        
        hand_detector.skeletal_transform(hand_detector.roi)
        
        if contour is not None:
            cv2.drawContours(img, [contour], -1, (0, 255, 0), 2)
        
    cv2.imshow('img', img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
    
    

