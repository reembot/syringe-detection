import cv2
import mediapipe as mp
import time
import numpy as np
import math


class HandDetector:
    def __init__(self, model_complexity=0, min_detection_confidence=0.3, min_tracking_confidence=0.3):
        self.mp_drawing = mp.solutions.drawing_utils
        self.mp_drawing_styles = mp.solutions.drawing_styles
        self.mp_hands = mp.solutions.hands
        self.wrist_landmarks = None
        self.hand_landmarks = None
        self.hands = self.mp_hands.Hands(
            model_complexity=model_complexity,
            min_detection_confidence=min_detection_confidence,
            min_tracking_confidence=min_tracking_confidence,
        )
        self.smallest_pinch = 1
    def detect(self, image):
        # To improve performance, optionally mark the image as not writeable to
        # pass by reference.
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = self.hands.process(image)

        # Draw the hand annotations on the image.
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        # Initially set finger count to 0 for each cap
        fingerCount = 0

        if results.multi_hand_landmarks:

            for hand_landmarks in results.multi_hand_landmarks:
                # Get hand index to check label (left or right)
                handIndex = results.multi_hand_landmarks.index(hand_landmarks)
                handLabel = results.multi_handedness[handIndex].classification[0].label

                # Set variable to keep landmarks positions (x and y)
                handLandmarks = []

                # Fill list with x and y positions of each landmark
                for landmarks in hand_landmarks.landmark:
                    handLandmarks.append([landmarks.x, landmarks.y])
                # Draw hand landmarks
                self.mp_drawing.draw_landmarks(
                    image,
                    hand_landmarks,
                    self.mp_hands.HAND_CONNECTIONS,
                    self.mp_drawing_styles.get_default_hand_landmarks_style(),
                    self.mp_drawing_styles.get_default_hand_connections_style(),
                )
                # draw a square around the hand
                x_min, y_min = (
                    int(min([landmark.x for landmark in hand_landmarks.landmark]) * image.shape[1]),
                    int(min([landmark.y for landmark in hand_landmarks.landmark]) * image.shape[0]),
                )
                x_max, y_max = (
                    int(max([landmark.x for landmark in hand_landmarks.landmark]) * image.shape[1]),
                    int(max([landmark.y for landmark in hand_landmarks.landmark]) * image.shape[0]),
                )
                cv2.rectangle(image, (x_min, y_min), (x_max, y_max), (0, 255, 0), 2)

                self.wrist_landmarks = handLandmarks[self.mp_hands.HandLandmark.WRIST]
                self.hand_landmarks = handLandmarks
            
            # skin_color = self.sample_skin_color(image, self.wrist_landmarks)
            
            # skinless = self.remove_skin(image,skin_color[0],skin_color[1])
            
            # cv2.imshow("skinless", skinless)
            # print(skin_color)
                # distance = self.get_distance_between_fingers(handLandmarks)
                # self.hand_landmarks = handLandmarks
                # if distance < self.smallest_pinch:
                #     self.smallest_pinch = distance
                # if distance < 0.065:
                #     print("pinch" + str(distance))
            
        
            return image, np.array([x_min, y_min, x_max, y_max])
        
        self.wrist_landmarks = None
        return image, None
    def removeHand(self, coordinates, image):
        x_min, y_min, x_max, y_max = coordinates
        roi = image[y_min:y_max, x_min:x_max]
        return roi
    def sample_skin_color(self, frame, joint):
        # Get the x, y coordinates of the joint
        joint_x, joint_y = joint[0], joint[1]

        # Define the size of the skin color sampler rectangle
        rectangle_size = 20

        # Calculate the coordinates of the top-left corner of the skin color sampler rectangle
        rectangle_x = max(joint_x - int(rectangle_size/2), 0)
        rectangle_y = max(joint_y - int(rectangle_size/2), 0)

        # Extract the skin color sample from the frame
        skin_color_sample = frame[rectangle_y:rectangle_y+rectangle_size, rectangle_x:rectangle_x+rectangle_size]

        # Convert the skin color sample to the HSV color space
        hsv_skin_color_sample = cv2.cvtColor(skin_color_sample, cv2.COLOR_BGR2HSV)

        # Calculate the mean HSV value of the skin color sample
        hsv_mean = cv2.mean(hsv_skin_color_sample)

        # Define the offset for the lower and upper HSV values
        offset = 80

        # Calculate the lower and upper HSV values
        hsv_lower = np.array([max(hsv_mean[0] - offset, 0), max(hsv_mean[1] - offset, 0), max(hsv_mean[2] - offset, 0)])
        hsv_upper = np.array([min(hsv_mean[0] + offset, 179), min(hsv_mean[1] + offset, 255), min(hsv_mean[2] + offset, 255)])

        return hsv_lower, hsv_upper
    
    def remove_skin(self, frame, hsv_lower, hsv_upper):
        # Convert the frame to the HSV color space
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # Create a mask based on the lower and upper HSV value
        mask = cv2.inRange(hsv, hsv_lower, hsv_upper)

        # Apply the mask to the frame
        frame = cv2.bitwise_and(frame, frame, mask=mask)

        return frame
    def get_distance_between_fingers(self):
        # Get the landmarks for the index finger tip and the thumb tip
        index_finger_tip = self.hand_landmarks[8]
        thumb_tip = self.hand_landmarks[4]

        # Calculate the Euclidean distance between the two landmarks
        distance = math.sqrt((index_finger_tip[0] - thumb_tip[0]) ** 2 + (index_finger_tip[1] - thumb_tip[1]) ** 2)
        
        return distance
    def isPinching(self, threshold):
        if self.get_distance_between_fingers() < threshold:
            return True
        return False
    #takes in the coordinates of the hand over time and returns the direction of the hand movement
    def get_hand_movement(self, hand_coordinates_over_time):
        if len(hand_coordinates_over_time) < 2:
            return "none"  # not enough data to calculate movement
        total_movement = 0
        for i in range(len(hand_coordinates_over_time)-1):
            start_x, _ = hand_coordinates_over_time[i]
            end_x, _ = hand_coordinates_over_time[i+1]
            total_movement += (end_x - start_x)
        avg_movement = total_movement / (len(hand_coordinates_over_time)-1)
        if avg_movement > 0:
            return "right"
        elif avg_movement < 0:
            return "left"
        else:
            return "none"  # no movement in the x-direction
        
    #takes in the coordinates of the hand over time and returns the most extreme direction of the hand movement
    def get_hand_movementALL(self, hand_coordinates_over_time):
        if len(hand_coordinates_over_time) < 2:
            return "none"  # not enough data to calculate movement
        total_movementX = 0
        total_movementY = 0
        for i in range(len(hand_coordinates_over_time)-1):
            start_x, start_y = hand_coordinates_over_time[i]
            end_x, end_y = hand_coordinates_over_time[i+1]
            
            total_movementX += (end_x - start_x)
            total_movementY += (end_y - start_y)
            
        avg_movementX = total_movementX / (len(hand_coordinates_over_time)-1)
        avg_movementY = total_movementY / (len(hand_coordinates_over_time)-1)
        if abs(total_movementX) > abs(total_movementY):
            if avg_movementX > 0:
                return "right"
            return "left"
        elif abs(total_movementX) < abs(total_movementY):
            if avg_movementY > 0:
                return "bottom"
            return "top"
        return "none"  # no movement in the x-direction or y-direction

    #takes in the initial coordinate of the hand and the frame size
    #returns weather the hand is in the right or left side of the frame
    #ONLY WORKS FOR LEFT AND RIGHT EDGES OF THE FRAME
    def get_hand_side(self, hand_coordinate, frame_size):
        # print("hand coordinate: ", hand_coordinate * frame_size)
        # print("frame size: ", frame_size)
        if hand_coordinate[0] * frame_size[0] > frame_size[0] / 2:
            return "right"
        return "left"
    
    def get_hand_sideALL(self, hand_coordinate, frame_size):
        x, y = hand_coordinate[0] * frame_size[0], hand_coordinate[1] * frame_size[1]
        frame_width, frame_height = frame_size[0], frame_size[1]
        if x < frame_width/2 and y < frame_height/2:
            return "top", "left"
        elif x >= frame_width/2 and y < frame_height/2:
            return "top", "right"
        elif x < frame_width/2 and y >= frame_height/2:
            return "bottom", "left"
        else:
            return "bottom", "right"
    
    #takes in the predicted hand movement and the starting side of the hand
    #if we are moving in the opposite direction of the starting side, we have let go
    def drop_or_pickup(self, predicted_hand_movement, init_hand_side):
        count = 0
        for i in range(len(predicted_hand_movement)):
            if predicted_hand_movement[i] != init_hand_side:
                count += 1
        if count > len(predicted_hand_movement) / 2:
            return "drop"
        return "pickup"
    
     #takes in the predicted hand movement and the starting side of the hand
    #if we are moving in the opposite direction of the starting side, we have let go
    def drop_or_pickupALL(self, predicted_hand_movement, init_hand_side):
        count = 0
        for i in range(len(predicted_hand_movement)):
            if predicted_hand_movement[i] == init_hand_side[0] or predicted_hand_movement[i] == init_hand_side[1]:
                count += 1
                # print("prediction ", i, ": ", predicted_hand_movement[i])
        # print("count: ", count, "len: ", len(predicted_hand_movement))
        if count >= (len(predicted_hand_movement) / 2):
            return "pickup"
        return "drop"
  
  
