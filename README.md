# hand-eye-rokae
  

This is a robotic hand-eye system.  
The project was programmed based on the [rokae industrial robot](http://www.rokae.com/) and [basler industrial camera](https://www.basler.com/).  
The [aruco](https://www.uco.es/investiga/grupos/ava/node/26) and [opencv](https://opencv.org/) are used for target detection.The connection between the robot and the computer based on the socket.  
![system](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/robot_systerm.jpg)    

## UI
User interface  was developed based on MFC.Click the button "touch target" to move the end-effector to the target aruco code.Click the button "find target" to make the manipulator pretend to find the target aruco code.Click the button "calibrate camera" to calibrate the relationship between the camera and the base coordinate system(the parameters of the camera should be edited at [here](https://github.com/foxchys/hand_eye_rokae/blob/master/touch_ball0.2/touch_ball/camera_params.yml) and 3-points(o,x,y) for the chessborad coordinate should be edited at [here](https://github.com/foxchys/hand_eye_rokae/blob/master/touch_ball0.2/touch_ball/touch_ballDlg.cpp#L390)).  
![UI](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/dispaly_ui.PNG)  

## Robot teach pendant  
Robot teach pendant device are programmed with robot language.The system version is 3.2.0.
![teach pendant](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/rokae_tercher.jpg)  

  





Foxchys  
Email:chy_s@outlook.com
