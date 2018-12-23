
// touch_ballDlg.h : header file
//

#pragma once

#include <sstream>
#include <string>
#include <mutex>

#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<pylon/PylonIncludes.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "afxwin.h"

#include"robot_control.h"

// Ctouch_ballDlg dialog
class Ctouch_ballDlg : public CDialogEx
{
// Construction
public:
	Ctouch_ballDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOUCH_BALL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void DrawMat(cv::Mat& img, UINT nID);
	afx_msg void OnBnClickedInitiali();
	static UINT Initiali(void* param);
	afx_msg void OnBnClickedCamSw();
	static UINT cam_dis(void* param);
	volatile bool cam_flg;
	std::mutex g_num_mutex;//thread mutex
	aruco_detect target_detecte;
	template<typename Tc>
	void cout2cedit(CEdit& control, Tc);//output to the edit control
	void StringReplace(std::string &strBase, std::string strSrc, std::string strDes);
	CEdit m_RESULT;
	cv::Mat /*Tvec,*/ Rvec, frame;
	Eigen::MatrixXd Tvec;
	std::array<double, 7> rest_position_input;
	move_calculate move_rob;
	afx_msg void OnBnClickedSocketsw();
	afx_msg void OnBnClickedCalibcam();
	static UINT CalibCam(void* param);
	afx_msg void OnBnClickedRestrobot();
	static UINT Restrobot(void* param);
	afx_msg void OnBnClickedTouchtar();
	static UINT touch_target(void* param);
	afx_msg void OnBnClickedFindtar();
	static UINT FindTarget(void* param);
	volatile bool find_flg;
};
