# hand-eye-rokae
  

This is a robotic hand-eye system of [rokae industrial robot](http://www.rokae.com/) and [basler industrial camera](https://www.basler.com/).  
The system uses the [aruco](https://www.uco.es/investiga/grupos/ava/node/26)and [opencv](https://opencv.org/) for target detection.Communication between the robot and the computer based on the socket.
![system](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/robot_systerm.jpg)    

## UI
User interface development based on MFC.Click the button "touch target" to move the end of the arm to the target aruco code.Click the button "find target" to make the robotic arm pretend to find the target aruco code.Click the button "calibrate camera" to calibrate the relationship between the camera and the base coordinate system.  
![UI](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/dispaly_ui.PNG)  

## Robot teach pendant  
Robot underlying scripts are programmed using robot language.The system version is 3.2.0.Confirmed with the engineer of rokae, there are still some problems with the system.Each time a socket link can only receive information once.Preliminary inference that rokae cannot clear the socket receive buffer.
![teach pendant](https://raw.githubusercontent.com/NikofoxS/hand-eye-rokae/master/pictures/rokae_tercher.jpg)  

## some method  
Some methods about this system.  
### three point method 

