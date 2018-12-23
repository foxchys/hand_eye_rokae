#include"stdafx.h"
#include"robot_control.h"


aruco_detect::aruco_detect()
{
	return;
}

aruco_detect::~aruco_detect()
{
	return;
}

void aruco_detect::inti(float marker_real_size_in, int ref_id_input, std::string campara_path, std::string aruco_dic, bool enclosed_marker)
{
	marker_real_size = marker_real_size_in;
	ref_id = ref_id_input;
	MDetector.setDictionary(aruco_dic);
	MDetector.getParameters().detectEnclosedMarkers(enclosed_marker);
	TheCameraParameters.readFromXMLFile(campara_path);
	return;
}

bool aruco_detect::get_marker_position(cv::Mat &input_image, cv::Mat &Tvec, cv::Mat &Rvec)
{
	bool flg = false;
	std::map<uint32_t, aruco::MarkerPoseTracker> MTracker;  // use a map so that for each id, we use a different pose tracker
	TheMarkers = MDetector.detect(input_image, TheCameraParameters, marker_real_size);
	for (auto& marker : TheMarkers) // for each marker
	{
		if (!MTracker[marker.id].estimatePose(marker, TheCameraParameters, marker_real_size))  // call its tracker and estimate the pose
			return flg;
	}
	for (auto&temp : TheMarkers)
	{
		if (ref_id == temp.id)
		{
			temp.Tvec.copyTo(Tvec);
			temp.Rvec.copyTo(Rvec);
			aruco::CvDrawingUtils::draw3dAxis(input_image, temp, TheCameraParameters);
			flg = true;
		}
		temp.draw(input_image, cv::Scalar(0, 0, 255), 2, true);
	}
	return flg;
}

void aruco_detect::get_campara(cv::Mat &CameraMatrix, cv::Mat &Distorsion)
{
	CameraMatrix = TheCameraParameters.CameraMatrix;
	Distorsion = TheCameraParameters.Distorsion;
	return;
}

//chy_s@outlook.com
move_calculate::move_calculate()
{
	srand((unsigned)time(NULL));
	return;
}

move_calculate::~move_calculate()
{
	close_link();
	socket_close();
	return;
}

void move_calculate::inti(cv::Mat in_InstrinsicMatrix, cv::Mat in_Distorsion, std::array<double, 7> robot_rest_position)
{
	InstrinsicMatrix = in_InstrinsicMatrix;
	Distorsion = in_Distorsion;
	rest_position = robot_rest_position;
	return;
}

bool move_calculate::get_chessboard_rt(Eigen::MatrixXd &chessboard_rt, cv::Mat frame, cv::Size board_size, cv::Size2f board_realsize)
{
	cv::Mat axis, tran, R;
	Eigen::MatrixXd r_t = Eigen::MatrixXd::Zero(4, 4);
	r_t(3, 3) = 1.0;
	std::vector<cv::Point2f> image_piont_buf, world_coordinate;//world corner on pictur//world coordinate
	world_coordinate = chessb_chessb_corner(board_size, board_realsize);
	if (find_point(frame, image_piont_buf, board_size))
	{
		if (get_r_t(world_coordinate, image_piont_buf, InstrinsicMatrix, Distorsion, axis, tran))
		{
			cv::Rodrigues(axis, R);
			Eigen::Matrix3d temp;
			cv::cv2eigen(R, temp);
			r_t.topLeftCorner(3, 3) = temp;
			for (int i = 0; i < 3; i++)
			{
				r_t(i, 3) = tran.at<double>(i, 0);
				chessboard_rt = r_t;
			}
		}
	}
	else return false;
	return true;
}

bool move_calculate::get_r_t(std::vector<cv::Point2f> world_coordinate2, std::vector<cv::Point2f> image_piont,
	cv::Mat InstrinsicMatrix, cv::Mat distortion, cv::Mat &axis, cv::Mat &tran)//get the r_t matrix from camera to chessboard 
{
	std::vector<cv::Point3f> world_coordinate3;
	for (unsigned int i = 0; i < world_coordinate2.size(); i++)
	{
		cv::Point3f temp;
		temp.z = 0; temp.x = world_coordinate2[i].y;
		temp.y = world_coordinate2[i].x;
		world_coordinate3.push_back(temp);
	}
	if (cv::solvePnPRansac(world_coordinate3, image_piont, InstrinsicMatrix, distortion, axis, tran))
		return true;
	return false;
}

bool move_calculate::find_point(cv::Mat chessboard, std::vector<cv::Point2f> &image_piont_buf, cv::Size board_size)//find corner on the chessboard picture
{
	cv::Mat grayImage;
	grayImage.create(chessboard.size(), chessboard.type());
	cv::cvtColor(chessboard, grayImage, CV_BGR2GRAY);
	if (cv::findChessboardCorners(grayImage, board_size, image_piont_buf))
	{
		cv::find4QuadCornerSubpix(grayImage, image_piont_buf, cv::Size(5, 5));
	}
	else return false;
	return true;
}

std::vector<cv::Point2f> move_calculate::chessb_chessb_corner(cv::Size board_size, cv::Size2f realsize)//calculate corner point at chessboard coordinate
{
	int width = board_size.width;
	int height = board_size.height;
	std::vector<cv::Point2f> world_corner;
	cv::Point2f tempcorner;
	int j = 0, k = 0;
	for (int i = 0; i < (width*height); i++)
	{
		if (j == width)
		{
			j = 0;
			k++;
		}
		tempcorner.x = (realsize.width)*(width - 1);
		tempcorner.y = (realsize.height)*(height - 1);
		tempcorner.x -= ((realsize.width)*(float)j);
		tempcorner.y -= ((realsize.height)*(float)k);
		j++;
		world_corner.push_back(tempcorner);
	}
	return world_corner;
}

Eigen::MatrixXd move_calculate::three_point_calib(Eigen::MatrixXd oxy)//cm
{
	Eigen::MatrixXd r_t = Eigen::MatrixXd::Zero(4, 4); r_t(3, 3) = 1.0;
	Eigen::Vector3d point_o = oxy.topLeftCorner(3, 1);
	Eigen::Vector3d point_x = oxy.middleCols(1, 1);
	Eigen::Vector3d point_y = oxy.middleCols(2, 1);
	r_t.topRightCorner(3, 1) = point_o;
	Eigen::Vector3d o_x = point_x - point_o; o_x = o_x / o_x.norm();
	Eigen::Vector3d o_y = point_y - point_o; o_y = o_y / o_y.norm();
	Eigen::Vector3d o_z = o_x.cross(o_y); o_z = o_z / o_z.norm();
	Eigen::Vector3d base_o_x = { 1.0,0.0,0.0 };
	Eigen::Vector3d base_o_y = { 0.0,1.0,0.0 };
	Eigen::Vector3d base_o_z = { 0.0,0.0,1.0 };
	Eigen::MatrixXd r_base2chess(3, 3);
	r_base2chess << o_x.dot(base_o_x), o_y.dot(base_o_x), o_z.dot(base_o_x),
		o_x.dot(base_o_y), o_y.dot(base_o_y), o_z.dot(base_o_y),
		o_x.dot(base_o_z), o_y.dot(base_o_z), o_z.dot(base_o_z);
	r_t.topLeftCorner(3, 3) = r_base2chess;
	return r_t;
}

bool  move_calculate::calib_cam(cv::Mat frame, Eigen::MatrixXd oxy, cv::Size board_size, cv::Size2f board_realsize, Eigen::MatrixXd &w2c_out)
{
	table_w_oxy = oxy;
	Eigen::MatrixXd cam2chess, base2chess;
	if (get_chessboard_rt(cam2chess, frame, board_size, board_realsize))
	{
		base2chess = three_point_calib(oxy);
		worldcoord2cam = base2chess*cam2chess.inverse();
		w2c_out = worldcoord2cam;
	}
	else return false;
	return true;
}

bool move_calculate::socket_init(int port)
{
	sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	//if(!AfxSocketInit(&wsaData))
	{
		return false;
	}
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		//"socket error !"
		return false;
	}
	sin_socket.sin_family = AF_INET;
	sin_socket.sin_port = htons(port);
	sin_socket.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin_socket, sizeof(sin_socket)) == SOCKET_ERROR)
	{
		//"bind error !"
		return false;
	}
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		//"listen error !";
		return false;
	}
	nAddrlen = sizeof(remoteAddr);
	return true;
}

std::string move_calculate::wait_link()
{
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET) return "accept error !";
	std::string temp_revData, TEMPP;
	temp_revData = inet_ntoa(remoteAddr.sin_addr);
	TEMPP = "A link was received£º\r\n";
	TEMPP += temp_revData;
	return TEMPP;
}

std::string move_calculate::receive_string()
{
	char revData[1024];
	std::string temp;
	int ret = recv(sClient, revData, 1024, 0);
	if (ret > 0)
	{
		revData[ret] = 0x00;
		temp = revData;
	}
	else temp = "No data.";
	return temp;
}

void move_calculate::move_send(std::array<double, 7> target_position)
{
	double temp_posi;
	std::string temp_send;
	std::array<double, 7> temp_position = target_position;
	for (int i = 0; i < 3; i++)
		temp_position[i] = temp_position[i] * 10.0;//cm to mm
	for (auto&temp : temp_position)
	{
		temp_posi = floor(temp * 10000.000 + 0.5) / 10000.000;
		temp_send += std::to_string(temp_posi);
		temp_send += ',';
	}
	temp_send += '\r';
	send(sClient, temp_send.c_str(), temp_send.size(), 0);
	return;
}

std::string move_calculate::receive_position()
{
	char revData[1024];
	std::string temp = "No data";
	for (int k = 0; k < 2; k++)
	{
		int ret = recv(sClient, revData, 1024, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			temp = revData;
		}
	}
	return temp;
}

void move_calculate::close_link()
{
	closesocket(sClient);
	return;
}

void move_calculate::socket_close()
{
	closesocket(slisten);
	WSACleanup();
	return;
}

std::array<double, 7> move_calculate::calculate_next_point(Eigen::Vector3d target_position, Eigen::Vector3d tool_position)
{
	Eigen::MatrixXd target_cam_H(4, 1);
	target_cam_H.topLeftCorner(3, 1) = target_position; target_cam_H(3, 0) = 1.0;
	Eigen::MatrixXd tool_w_H(4, 1);
	tool_w_H.topLeftCorner(3, 1) = tool_position; tool_w_H(3, 0) = 1.0;
	Eigen::MatrixXd target_W_H = worldcoord2cam*target_cam_H;
	Eigen::Vector3d ball_center = (tool_w_H + target_W_H).topLeftCorner(3, 1) / 2.0;//ball_center in w coordinate
	double radius = ((tool_w_H - target_W_H).topLeftCorner(3, 1)).norm() / 2.0;//radius
	int i = 0;
	Eigen::Vector3d next_point;
	
	{
		//set a random Unit vectors(polar coordinates method)////////////
		double rand_r = 1.0;
		double rand_t1 = random(0.0, PI_RO * 2);
		double rand_t2 = random(-PI_RO / 2.0, PI_RO / 2.0);
		Eigen::Vector3d rand_unit_vector = { cos(rand_t1)*cos(rand_t2) *rand_r,sin(rand_t1) * cos(rand_t2) * rand_r, sin(rand_t2)* rand_r };
		//another method:x=random(-1,1),y=random(-sqrt(1-x^2) , sqrt(1-x^2)),z=+or-sqrt(1-x^2-y^2)
		/////////////////////////////////////////////////////////////////
		next_point = ball_center + random(0.0, 1.0)*radius*rand_unit_vector;
		i++;
	}while ((next_point(2, 0) < table_w_oxy(2, 0)) && i < 1000)

	if (i == 1000)return rest_position;
	return{ next_point(0,0),next_point(1,0),next_point(2,0),rest_position[3],rest_position[4],rest_position[5],rest_position[6] };
}

std::array<double, 7> move_calculate::calculate_target_point(Eigen::Vector3d target_position)
{
	Eigen::MatrixXd target_cam_H(4, 1);
	target_cam_H.topLeftCorner(3, 1) = target_position; target_cam_H(3, 0) = 1.0;
	Eigen::MatrixXd target_W_H = worldcoord2cam*target_cam_H;
	return{ target_W_H(0,0),target_W_H(1,0),target_W_H(2,0),rest_position[3],rest_position[4],rest_position[5],rest_position[6] };
}

void move_calculate::reset_send()
{
	move_send(rest_position);
	return;
}

Eigen::MatrixXd move_calculate::get_worldcoord2cam()
{
	return worldcoord2cam;
}
//chy_s@outlook.com