import cv2
import mediapipe as mp
import time
import numpy as np
from mat_detector import mat_detector
from HandDetector import HandDetector
from SyringeDetector import SyringeDetector
from ColorIdentifier import ColorIdentifier

cap = cv2.VideoCapture('IMG_2893.mov')
matDetector = mat_detector()
hand_detector = HandDetector()
syringe_detector = SyringeDetector()
color_identifier = ColorIdentifier()

#to deal with syringe detection and background segmentation
calibrated_image = None
update_syringe_area_roi = False
update_count = 0

# Initialize variables for measuring frame rate
prev_time = 0
curr_time = 0

start_time = time.time()
curr_time = time.time()

#to deal with hand detection and movement
hand_coordinates_over_time = []
predictions_of_hand_movement = []
init_hand_location = None
was_hand_detected =  0 #number of frames hand was detected
init_real_hand_location = None
real_predictions_of_hand_movement = []

#debugging variables
min_pinch_distance = 1
avg_pinch_distance = 0
total_pinches = 1


screen_size = np.array([0, 0])
while cap.isOpened():
    success, image = cap.read()
    if not success:
        print("Ignoring empty camera frame.")
        # If loading a video, use 'break' instead of 'continue'.
        break
    
    #detecting screen size
    if screen_size[0] == 0 or screen_size[1] == 0:
        screen_size[0] = image.shape[1]
        screen_size[1] = image.shape[0]
    
    
    #detect the hand
    image_with_hands, coordinates = hand_detector.detect(image)
        
    
    if not matDetector.mat_detected:
        matDetector.detect_mat(image)
        
    else:
         #lets set a baseline for the "truth"
        if calibrated_image is None:
            calibrated_image = image
            syringe_detector.calibrate(matDetector.get_mat_ROI(image))
            
        
        #detect the syringe only in the area of the mat
        contours = syringe_detector.detect(matDetector.get_mat_ROI(image))
        max_contour = syringe_detector.getLargestContour(contours)
        
         #if hand is detected
        if coordinates is not None:
            was_hand_detected += 1
            
            #lets wait a few frames before we are sure the hand is there
            if was_hand_detected > 10:
                update_syringe_area_roi = True
                
             #if we have not initialized the hand location
            if init_hand_location is None:
                init_hand_location = hand_detector.get_hand_side(hand_detector.wrist_landmarks, screen_size)
                init_real_hand_location = hand_detector.get_hand_sideALL(hand_detector.wrist_landmarks, screen_size)
    
            #lets get the distance between the fingers (FOR DEBUGGING)
            min_pinch_distance = min(hand_detector.get_distance_between_fingers(), min_pinch_distance)
            #if we are pinching
            if hand_detector.isPinching(0.075):
                
                #debugging
                avg_pinch_distance += hand_detector.get_distance_between_fingers()
                total_pinches += 1
                
                
                #we add the wrist coordinates to the to a list to keep track of the path
                hand_coordinates_over_time.append(hand_detector.wrist_landmarks * screen_size)
                
                #after we have a certain number of points we can make a prediction
                if len(hand_coordinates_over_time) > 3:
                    predictions_of_hand_movement.append(hand_detector.get_hand_movement(hand_coordinates_over_time))
                    real_predictions_of_hand_movement.append(hand_detector.get_hand_movementALL(hand_coordinates_over_time))
                    # print("prediction of movement : ",real_predictions_of_hand_movement[-1])
                    hand_coordinates_over_time = []
            
        #if the hand is not detected
        else:
            if max_contour is not None:
                cv2.drawContours(image_with_hands, [max_contour + matDetector.mat_coordinates[:2]], -1, (0, 255, 0), 2)
            #we can make a prediction based on the the previous predictions
            if len(predictions_of_hand_movement) > 0:
                # print("basic prediction : ", hand_detector.drop_or_pickup(predictions_of_hand_movement, init_hand_location))
                final_prediction = hand_detector.drop_or_pickupALL(real_predictions_of_hand_movement, init_real_hand_location)
                print("prediction : ", final_prediction)
                
                #if we detected a change
                if max_contour is not None:
                    
                    #lets get a bounding box around the syringe
                    syringe_coordinates = cv2.boundingRect(max_contour)
                    
                    
                    #the syringe detector to detect the syringe
                    #use the current image for dropping the syringe or the calibrated image for picking up the syringe
                    image_to_use = None
                    if final_prediction == "pickup":
                        image_to_use = calibrated_image
                    else:
                        image_to_use = image
                    
            
                    # Calculate the width and height of the original syringe ROI
                    syringe_width = syringe_coordinates[2]
                    syringe_height = syringe_coordinates[3]
                    
                    # Define the scaling factor to make the area smaller
                    #we need to flip the x and y because if syringe is vertical the width is the height
                    scale_factorX = 0.7
                    scale_factorY = 0.5
                    if syringe_width > syringe_height:
                        scale_factorX = 0.5
                        scale_factorY = 0.7
                    

                    # Calculate the new width and height of the smaller ROI
                    new_width = int(syringe_width * scale_factorX)
                    new_height = int(syringe_height * scale_factorY)

                    # Calculate the new x and y coordinates for the top-left corner of the smaller ROI
                    new_x = int(syringe_coordinates[0] + matDetector.mat_coordinates[0] + (syringe_width - new_width) / 2)
                    new_y = int(syringe_coordinates[1] + matDetector.mat_coordinates[1] + (syringe_height - new_height) / 2)

                    # Extract the smaller ROI from the image we must include the ROI's width and height
                    syringe_to_analyze_ROI = image_to_use[new_y:new_y + new_height, new_x:new_x + new_width]
                    cv2.imshow("syringe", syringe_to_analyze_ROI)
                    
                    #lets get the color of the syringe
                    color_identifier.kmeans_ignore_gray(syringe_to_analyze_ROI.copy())
                        
                        
                    print("")
            elif was_hand_detected > 3:
                print("hand was detected but no predictions were made\n")
                if total_pinches <= 1:
                    print("avg pinch distance : ", 0, "min pinch distance : ", min_pinch_distance)
                else:
                    print("avg pinch distance : ", avg_pinch_distance / total_pinches, "min pinch distance : ", min_pinch_distance)
                    
            
            hand_coordinates_over_time = []
            predictions_of_hand_movement = []
            init_hand_location = None
            real_predictions_of_hand_movement = []
            
            # if was_hand_detected:
                # print("avg pinch distance : ", avg_pinch_distance / total_pinches, "min pinch distance : ", min_pinch_distance)
            was_hand_detected = 0
            min_pinch_distance = 1
            avg_pinch_distance = 0
            total_pinches = 0
            
            #lets update the background
            if update_syringe_area_roi:
                update_count += 1
                
                #wait until we have 10 frames to update the background(we need to wait for the hand to move out of the way)
                if update_count == 10:
                    update_count = 0
                    update_syringe_area_roi = False
                    syringe_detector.calibrate(matDetector.get_mat_ROI(image))
                    calibrated_image = image
                    
            
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
print(f"Total time: {time.time() - start_time}")