#pragma once
#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>    
#include <winsock2.h>    
#include<afxsock.h>
#include <string>
#include<array>
#include<time.h> 
#pragma comment(lib,"ws2_32.lib") 

#include"stdafx.h"
#include"eigen-eigen\Eigen\Core"
#include"eigen-eigen\Eigen\Geometry"
#include"eigen-eigen\Eigen\Dense"

#include <aruco/aruco.h>
#include <aruco/markerdetector.h>
#include <aruco/marker.h>
#include <aruco/cameraparameters.h>
#include <aruco/posetracker.h>
#include <aruco/cvdrawingutils.h>
#include <stdexcept>

#include<cmath>

#include "opencv2/core/core.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/calib3d/calib3d.hpp"  
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/core/eigen.hpp"

#define random(a, b) (((double)rand() / RAND_MAX)*(b - a) + a)
#define PI_RO 3.141592653589793

class aruco_detect
{
public:
	// parameters: camera device number[0,1,...],the path of camera parameters[/home/s/mycode/srvrob_arm/src/object_touch/camera_params.yml],...
	// ...real size of marker(meter[0.135]),diction of marker[ARUCO],target marker id[0,1,2,...],...
	// ...enclosed_marker[0,1]
	aruco_detect();
	void inti(float marker_real_size_in, int ref_id_input, std::string campara_path = "camera_params.yml",
		std::string aruco_dic = "ARUCO", bool enclosed_marker = true);
	virtual ~aruco_detect();
	bool get_marker_position(cv::Mat &input_image, cv::Mat &Tvec, cv::Mat &Rvec);
	void get_campara(cv::Mat &CameraMatrix, cv::Mat &Distorsion);
private:
	aruco::MarkerDetector MDetector;
	aruco::CameraParameters TheCameraParameters;
	std::vector<aruco::Marker> TheMarkers;
	float marker_real_size;
	int ref_id;
};//chy_s@outlook.com

class move_calculate
{
public:
	move_calculate();
	~move_calculate();
	void inti(cv::Mat in_InstrinsicMatrix, cv::Mat in_Distorsion, std::array<double, 7> robot_rest_position);
	bool socket_init(int port);
	std::string wait_link();
	std::string receive_string();
	void move_send(std::array<double,7> target_position);//send move message to robot.input:(x,y,z)(cm)&quarternion:q,i,j,k,
	std::string receive_position();
	void close_link();
	void socket_close();
	void reset_send();

	bool calib_cam(cv::Mat frame, Eigen::MatrixXd oxy, cv::Size board_size, cv::Size2f board_realsize, Eigen::MatrixXd &w2c_out);//calculate transformation from world to camera
	Eigen::MatrixXd get_worldcoord2cam();

	std::array<double, 7> calculate_next_point(Eigen::Vector3d target_position, Eigen::Vector3d tool_position);//input target point in camera,tool position now in world
	std::array<double, 7> calculate_target_point(Eigen::Vector3d target_position);//calculate target point to touch the target input(cm)

private:
	Eigen::MatrixXd worldcoord2cam;//transformation from world to camera
	cv::Mat InstrinsicMatrix;
	cv::Mat Distorsion;
	Eigen::MatrixXd three_point_calib(Eigen::MatrixXd oxy);//Usage:o(3,1)|x(3,1)|y(3,1)(cm)
	bool get_r_t(std::vector<cv::Point2f> world_coordinate2, std::vector<cv::Point2f> image_piont,
		cv::Mat InstrinsicMatrix, cv::Mat distortion, cv::Mat &axis, cv::Mat &tran);//get the r_t matrix from camera to chessboard 
	bool find_point(cv::Mat chessboard, std::vector<cv::Point2f> &image_piont_buf, cv::Size board_size);//find corner on the chessboard picture
	std::vector<cv::Point2f> chessb_chessb_corner(cv::Size board_size, cv::Size2f realsize);//calculate corner point at chessboard coordinate
	bool get_chessboard_rt(Eigen::MatrixXd &chessboard_rt, cv::Mat frame, cv::Size board_size, cv::Size2f board_realsize);//get r_t from camera to chessboard
	Eigen::MatrixXd table_w_oxy;//Set the plane of table to avoid crash the table.
	std::array<double, 7> rest_position;//robot_rest_position
	//------------------socket-----------------
	WORD sockVersion;//WSA   
	WSADATA wsaData;
	SOCKET slisten;
	sockaddr_in sin_socket;
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen;
	//-----------------------------------------
};
#endif // !ROBOT_CONTROL_H
//chy_s@outlook.com
