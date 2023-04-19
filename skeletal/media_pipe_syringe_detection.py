import cv2
import mediapipe as mp
import time
import numpy as np
from HandDetector import HandDetector
from SyringeDetector import SyringeDetector
from LineDetector import LineDetector



# Initialize variables for measuring frame rate
prev_time = 0
curr_time = 0

start_time = time.time()
curr_time = time.time()

cap = cv2.VideoCapture('IMG_2893.mov')
hand_detector = HandDetector()
syringe_detector = SyringeDetector()
line_detector = LineDetector()
have_roi = False
syringe_count = 0
syringe_roi = np.array([0, 0, 0, 0])
update_syringe_roi = False
update_count = 0
times_hand_not_detected = 0
frames_since_hand_detected = 0
while cap.isOpened():
    success, image = cap.read()
    if not success:
        print("Ignoring empty camera frame.")
        # If loading a video, use 'break' instead of 'continue'.
        break
    
    if not have_roi:
        image_with_lines, parallel_lines, temp_syringe_roi = line_detector.detectMat(image)
        #lets keep the largest cooordinates
        for i in range(len(temp_syringe_roi)):
            syringe_roi[i] = max(syringe_roi[i], temp_syringe_roi[i])
        syringe_count += 1
        cv2.imshow("Lines", image_with_lines[temp_syringe_roi[1]:temp_syringe_roi[3], temp_syringe_roi[0]:temp_syringe_roi[2]])
        if syringe_count == 10:
            have_roi = True
            syringe_detector.calibrate(image[syringe_roi[1]:syringe_roi[3], syringe_roi[0]:syringe_roi[2]])
            cv2.destroyAllWindows()
        
            
        print(syringe_roi)
        
    else:
        ROI = image[syringe_roi[1]:syringe_roi[3], syringe_roi[0]:syringe_roi[2]]
        
        syringe_detector.detect(ROI)
    
    
    image_with_hands, coordinates = hand_detector.detect(image)
    
    
    #if we recognize the hand we need to update the syringe roi(after the hand is removed)
    if coordinates is not None:
        if not update_syringe_roi:
            times_hand_not_detected += 1
            update_syringe_roi = True
        update_count = 0
        frames_since_hand_detected = 0
    
    #now we need to update the syringe roi when the hand is out of the way
    if update_syringe_roi:
        print(times_hand_not_detected)
        #if the hand coordinates are not within the syringe roi
        if coordinates is None and times_hand_not_detected == 2:
            update_count += 1
            if update_count == 10:
                update_syringe_roi = False
                update_count = 0
                times_hand_not_detected = 0
                syringe_detector.calibrate(image[syringe_roi[1]:syringe_roi[3], syringe_roi[0]:syringe_roi[2]])
        elif coordinates is None: 
            frames_since_hand_detected += 1
            if frames_since_hand_detected == 20:
                update_syringe_roi = False
                frames_since_hand_detected = 0
            
    # Calculate and display frame rate
    curr_time = time.time()
    fps = 1 / (curr_time - prev_time)
    prev_time = curr_time
    cv2.putText(
        image_with_hands,
        f"FPS: {int(fps)}",
        (10, 50),
        cv2.FONT_HERSHEY_SIMPLEX,
        1,
        (0, 255, 0),
        2,
    )
    cv2.imshow("MediaPipe Hands", image_with_hands)
    if cv2.waitKey(5) & 0xFF == 27:
        break
cap.release()
cv2.destroyAllWindows()

   
