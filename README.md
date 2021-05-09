# hand-eye-rokae
  

This is a robotic hand-eye system.  
The project was programmed based on the [rokae industrial robot](http://www.rokae.com/) and [basler industrial camera](https://www.basler.com/).  
The [aruco](https://www.uco.es/investiga/grupos/ava/node/26) and [opencv](https://opencv.org/) are used for target detection.The connection between the robot and the computer based on the socket(tcp).  
![system](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/robot_systerm.jpg)    

## UI
User interface  was developed based on MFC.Click the button "touch target" to move the end-effector to the target aruco code.Click the button "find target" to make the manipulator pretend to find the target aruco code.Click the button "calibrate camera" to calibrate the relationship between the camera and the base coordinate system(the parameters of the camera should be edited at [here](https://github.com/foxchys/hand_eye_rokae/blob/master/touch_ball0.2/touch_ball/camera_params.yml) and 3-points(o,x,y) for the chessborad coordinate should be edited at [here](https://github.com/foxchys/hand_eye_rokae/blob/master/touch_ball0.2/touch_ball/touch_ballDlg.cpp#L390)).  
![UI](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/dispaly_ui.PNG)  

## Robot teach pendant  
Robot teach pendant device are programmed with robot language.The system version is 3.2.0.
![teach pendant](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/rokae_tercher.jpg)  

## Hand-Eye
In this program. I had used a method that is not rigorous  to get the relationship between robot-base and camera.  Just take the tool control point(TCP) to touch the three points of chessboard_coordinate_system(zero point&point on the x-axis&point alone the y-axis) and read the coordinate(from robot teach pendant) based robot-base coordinate system.Then you can solve the the relationship between chessboard and robot-base based on the data three points(e.g.[here](https://github.com/foxchys/hand_eye_rokae/blob/master/touch_ball0.2/touch_ball/robot_control.cpp#L178)).And it is easy to get the relationship between chessboard and camera with PnP methods(e.t. [cv::solvePnP](https://docs.opencv.org/4.5.0/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d) and [here](https://github.com/foxchys/hand_eye_rokae/blob/master/touch_ball0.2/touch_ball/robot_control.cpp#L78)).Finaly, we get the relationship between robot-base and camera.  
Optionally, you can take the tool control point to touch many points of chessboard_coordinate_system and read the coordinate(from robot teach pendant) based robot-base coordinate system.And solve the PnP problem(camer and robot) based on the data of the points .Then you will get the relationship between robot and camera.  
For the normal methods. Solve the equation AX=XB.  
e.g. Tsai-Lenz([A new technique for fully autonomous and efficient 3D robotics hand/eye calibration](https://ieeexplore.ieee.org/document/34770?arnumber=34770&newsearch=true&queryText=A%20New%20Technique%20for%20Fully%20Autonomous%20and%20Efficient%203D%20Robotics%20Hand%2FEye%20Calibration)).  
[calibrateRobotWorldHandEye](https://docs.opencv.org/4.5.0/d9/d0c/group__calib3d.html#ga41b1a8dd70eae371eba707d101729c36).  






Foxchys  
Email:chy_s@outlook.com
