import cv2
import mediapipe as mp
import time
import numpy as np


class HandDetector:
    def __init__(self, model_complexity=0, min_detection_confidence=0.5, min_tracking_confidence=0.5):
        self.mp_drawing = mp.solutions.drawing_utils
        self.mp_drawing_styles = mp.solutions.drawing_styles
        self.mp_hands = mp.solutions.hands
        self.wrist_landmarks = None
        self.hands = self.mp_hands.Hands(
            model_complexity=model_complexity,
            min_detection_confidence=min_detection_confidence,
            min_tracking_confidence=min_tracking_confidence,
        )

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

            # self.wrist_landmarks = handLandmarks[self.mp_hands.HandLandmark.WRIST]
            
            # skin_color = self.sample_skin_color(image, self.wrist_landmarks)
            
            # skinless = self.remove_skin(image,skin_color[0],skin_color[1])
            
            # cv2.imshow("skinless", skinless)
            # print(skin_color)
        
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