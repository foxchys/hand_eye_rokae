# hand-eye-rokae
  

This is a robotic hand-eye system of [rokae industrial robot](http://www.rokae.com/) and [basler industrial camera](https://www.basler.com/).  
The system uses the [aruco](https://www.uco.es/investiga/grupos/ava/node/26) and [opencv](https://opencv.org/) for target detection.Communication between the robot and the computer based on the socket.
![system](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/robot_systerm.jpg)    

## UI
User interface development based on MFC.Click the button "touch target" to move the end of the arm to the target aruco code.Click the button "find target" to make the robotic arm pretend to find the target aruco code.Click the button "calibrate camera" to calibrate the relationship between the camera and the base coordinate system.  
![UI](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/dispaly_ui.PNG)  

## Robot teach pendant  
Robot underlying scripts are programmed using robot language.The system version is 3.2.0.
![teach pendant](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/rokae_tercher.jpg)  

## some method  
Some methods about this system.  
### calibrate method  
The relationship between the camera and the checkerboard can be obtained by using the method in the paper [Zhengyou Zhang. A flexible new technique for camera calibration.](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/tr98-71.pdf).Take the checkerboard coordinate system as the origin, the X direction, and the three points in the Y direction,read the coordinates of the point in the robot base coordinate system through the robot teach pendant.Based on the principle of the rotation matrix ![rotation matrix ](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/rot.jpg), the relationship between the base coordinate system and the checkerboard can be obtained.So far, the relationship between the camera and the base coordinate system can be found.  
### pretend to find the target
Each time the coordinates of the control point of the arm and the target point in the base coordinate system are read,confirming a sphere from the two points for the diameter.Each time a random point in the ball is set as the target point of the robot control point movement.  





NikofoxS  
Email:chy_s@outlook.com
