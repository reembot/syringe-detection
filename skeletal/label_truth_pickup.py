import cv2
import sys

# Hardcoded labels
labels = {"1": "pickup", "2": "dropped"}

color_labels = {"1": "orange", "2": "white", "3": "pink", "4": "yellow"}

# Paths to the input video file and output CSV file
input_file = "IMG_2918.mov"
output_file = "IMG_2918.csv"

# Open the output file
with open(output_file, "w") as outfile:
    # Open the video capture device
    cap = cv2.VideoCapture(input_file)
    if not cap.isOpened():
        print("Failed to open video capture device")
        sys.exit()

    # Create a window to display the video
    window_name = input_file
    cv2.namedWindow(window_name)

    # Initialize loop variables
    waitval = 100
    paused = False

    # Loop over the frames of the video
    while True:
        # Read a frame from the video
        ret, frame = cap.read()

        # If the read was unsuccessful, break the loop
        if not ret:
            break

        # Display the frame in the window
        cv2.imshow(window_name, frame)

        # Wait for a key press
        key = cv2.waitKey(waitval) & 0xFF

        # Handle the key press
        if key == 27:
            # Escape key was pressed, exit the loop
            break
        elif key == ord("p") or paused:
            # Pause key was pressed, pause the video
            paused = True
            waitval = 0
            print(f"Paused at {cap.get(cv2.CAP_PROP_POS_MSEC)} msecs")
            print("Enter label number (1 for pickup, 2 for dropped) or any other key to skip labeling.")
            print("Then Enter color number (1 for orange, 2 for white, 3 for pink, 4 for yellow) or any other key to skip labeling.")
            key = cv2.waitKey(0) & 0xFF
            #make key from ascii to char
            print(f"Action Key pressed: {key}")
            label = None
            timestamp = None
            color = None
            if chr(key) in labels:
                label = labels[chr(key)]
                timestamp = cap.get(cv2.CAP_PROP_POS_MSEC)
            key = cv2.waitKey(0) & 0xFF
            print("Color key pressed: ", key)
            if chr(key) in color_labels:
                color = color_labels[chr(key)]
                print(f"Writing {color} with action {label} at {timestamp} msecs")
                outfile.write(f"{color},{label},{timestamp}\n")
                
            paused = False
            waitval = 100
            print("Unpaused")
        elif key == ord("u") and waitval == 0:
            # Unpause key was pressed, unpause the video
            paused = False
            waitval = 100
            print(f"Unpaused at {cap.get(cv2.CAP_PROP_POS_MSEC)} msecs")
            print("Press p to pause.")

    # Release the video capture device and close the window
    cap.release()
    cv2.destroyAllWindows()
