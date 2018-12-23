
// touch_ballDlg.cpp : implementation file
//

#include "stdafx.h"
#include "touch_ball.h"
#include "touch_ballDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ctouch_ballDlg dialog



Ctouch_ballDlg::Ctouch_ballDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOUCH_BALL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctouch_ballDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT, m_RESULT);
}

BEGIN_MESSAGE_MAP(Ctouch_ballDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INITIALI, &Ctouch_ballDlg::OnBnClickedInitiali)
	ON_BN_CLICKED(IDC_Cam_Sw, &Ctouch_ballDlg::OnBnClickedCamSw)
	ON_BN_CLICKED(IDC_SocketSw, &Ctouch_ballDlg::OnBnClickedSocketsw)
	ON_BN_CLICKED(IDC_CalibCam, &Ctouch_ballDlg::OnBnClickedCalibcam)
	ON_BN_CLICKED(IDC_RestRobot, &Ctouch_ballDlg::OnBnClickedRestrobot)
	ON_BN_CLICKED(IDC_TouchTar, &Ctouch_ballDlg::OnBnClickedTouchtar)
	ON_BN_CLICKED(IDC_FindTar, &Ctouch_ballDlg::OnBnClickedFindtar)
END_MESSAGE_MAP()


// Ctouch_ballDlg message handlers

BOOL Ctouch_ballDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//chy_s@outlook.com
	GetDlgItem(IDC_INITIALI)->SetWindowTextW(_T("initialize"));
	GetDlgItem(IDC_Cam_Sw)->SetWindowTextW(_T("open camera"));
	GetDlgItem(IDC_SocketSw)->SetWindowTextW(_T("socket init")); 
	GetDlgItem(IDC_CalibCam)->SetWindowTextW(_T("calibrate camera")); 
	GetDlgItem(IDC_RestRobot)->SetWindowTextW(_T("rest robot"));
	GetDlgItem(IDC_TouchTar)->SetWindowTextW(_T("touch target"));
	GetDlgItem(IDC_FindTar)->SetWindowTextW(_T("find target"));
	cam_flg = false;
	find_flg = false;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ctouch_ballDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

//chy_s@outlook.com
void Ctouch_ballDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ctouch_ballDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ctouch_ballDlg::OnBnClickedInitiali()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_INITIALI)->EnableWindow(false);
	GetDlgItem(IDC_INITIALI)->SetWindowTextW(_T("initi..."));
	AfxBeginThread(Initiali, this, THREAD_PRIORITY_HIGHEST);
	return;
}

UINT Ctouch_ballDlg::Initiali(void* param)
{
	Ctouch_ballDlg* dlg = (Ctouch_ballDlg*)param;
	dlg->target_detecte.inti(0.035f, 0);
	dlg->cout2cedit(dlg->m_RESULT, "set the ArUco marker success.");
	cv::Mat CameraMatrix, Distorsion;
	dlg->target_detecte.get_campara(CameraMatrix, Distorsion);
	dlg->rest_position_input = { 23.527,22.469,40.474,-0.021,-0.03,-0.01,0.98 };//cm
	dlg->move_rob.inti(CameraMatrix, Distorsion, dlg->rest_position_input);
	dlg->cout2cedit(dlg->m_RESULT, "set the robot controler success");
	dlg->GetDlgItem(IDC_INITIALI)->SetWindowTextW(_T("initialize"));
	dlg->GetDlgItem(IDC_INITIALI)->EnableWindow(true);
	return 0;
}

void Ctouch_ballDlg::OnBnClickedCamSw()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_Cam_Sw)->GetWindowTextW(temp);
	if (temp == "open camera")
	{
		cam_flg = true;
		GetDlgItem(IDC_Cam_Sw)->EnableWindow(false);
		GetDlgItem(IDC_Cam_Sw)->SetWindowTextW(_T("opening..."));
		AfxBeginThread(cam_dis, this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		GetDlgItem(IDC_Cam_Sw)->EnableWindow(false);
		GetDlgItem(IDC_Cam_Sw)->SetWindowTextW(_T("closing"));
		cam_flg = false;
	}
	return;
}

UINT Ctouch_ballDlg::cam_dis(void* param)
{
	Ctouch_ballDlg* dlg = (Ctouch_ballDlg*)param;
	bool flg = true;
	Pylon::PylonInitialize();
	cv::VideoCapture capture(1);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 900);
	while (dlg->cam_flg)
	{
		dlg->g_num_mutex.lock();
		if (flg)
		{
			dlg->GetDlgItem(IDC_Cam_Sw)->SetWindowTextW(_T("close camera"));
			dlg->GetDlgItem(IDC_Cam_Sw)->EnableWindow(true);
			flg = false;
		}
		capture >> dlg->frame;
		if (!dlg->frame.empty())
		{
			cv::Mat tem_rvec, tem_tvec;
			if (dlg->target_detecte.get_marker_position(dlg->frame, tem_tvec, tem_rvec))
			{
				//dlg->Tvec = tem_tvec;
				cv::cv2eigen(tem_tvec, dlg->Tvec);
				dlg->Tvec = 100.0*dlg->Tvec;//m to cm
				dlg->Rvec = tem_rvec;
				//dlg->cout2cedit(dlg->m_RESULT, dlg->Tvec);
			}
			dlg->DrawMat(dlg->frame, IDC_CAMDIS);
			cv::waitKey(30);
		}
		dlg->g_num_mutex.unlock();
	}
	Pylon::PylonTerminate();
	dlg->GetDlgItem(IDC_Cam_Sw)->SetWindowTextW(_T("open camera"));
	dlg->GetDlgItem(IDC_Cam_Sw)->EnableWindow(true);
	return 0;
}

void Ctouch_ballDlg::DrawMat(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);
	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA);
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1);

	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	CDC *pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

template<typename Tc>
void Ctouch_ballDlg::cout2cedit(CEdit& control, Tc c2cout)
{
	int lastLine;
	CString temp;
	std::ostringstream cccout;
	cccout << c2cout;
	std::string str = cccout.str();
	StringReplace(str, "\n", "\r\n");
	temp = str.c_str();
	lastLine = control.LineIndex(control.GetLineCount() - 1);
	if (lastLine > 10000)
	{
		control.SetWindowTextW(_T(""));
	}
	control.SetSel(lastLine + 1, lastLine + 2, 1);
	control.ReplaceSel(temp + "\r\n");
	control.UpdateWindow();
	return;
}

void Ctouch_ballDlg::StringReplace(std::string &strBase, std::string strSrc, std::string strDes)
{
	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
	return;
}


void Ctouch_ballDlg::OnBnClickedSocketsw()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_SocketSw)->GetWindowTextW(temp);
	if (temp == "socket init")
	{
		GetDlgItem(IDC_SocketSw)->EnableWindow(false);
		GetDlgItem(IDC_SocketSw)->SetWindowTextW(_T("opening..."));
		if (move_rob.socket_init(3000))
		{
			cout2cedit(m_RESULT, "socket initialization succeeded");
			GetDlgItem(IDC_SocketSw)->SetWindowTextW(_T("socket close"));
			GetDlgItem(IDC_SocketSw)->EnableWindow(true);
		}
		else
		{
			cout2cedit(m_RESULT, "socket initialization failed");
			GetDlgItem(IDC_SocketSw)->SetWindowTextW(_T("socket init"));
			GetDlgItem(IDC_SocketSw)->EnableWindow(true);
		}
	}
	else
	{
		GetDlgItem(IDC_SocketSw)->EnableWindow(false);
		GetDlgItem(IDC_SocketSw)->SetWindowTextW(_T("closing"));
		move_rob.close_link();
		move_rob.socket_close();
		cout2cedit(m_RESULT, "socket is closed");
		GetDlgItem(IDC_SocketSw)->SetWindowTextW(_T("socket init"));
		GetDlgItem(IDC_SocketSw)->EnableWindow(true);
	}
	return;
}


void Ctouch_ballDlg::OnBnClickedCalibcam()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CalibCam)->EnableWindow(false);
	AfxBeginThread(CalibCam, this, THREAD_PRIORITY_HIGHEST);
	return;
}

UINT Ctouch_ballDlg::CalibCam(void* param)
{
	Ctouch_ballDlg* dlg = (Ctouch_ballDlg*)param;
	cv::Size board_size = cv::Size(9, 6);//corner size
	cv::Size2f realsize(2.0f, 2.0f);//every grid real size
	Eigen::MatrixXd oxy(3, 3);
	oxy << 50.377, 50.348, 46.327,
		9.241, 13.368, 9.357,
		25.971, 25.986, 26.018;//////////////3 point of chessboard Usage:o(3,1)|x(3,1)|y(3,1)(cm)
	cv::Mat frame;
	//dlg->g_num_mutex.lock();
	dlg->frame.copyTo(frame);
	//dlg->g_num_mutex.unlock();
	Eigen::MatrixXd world2cam;
	if (dlg->move_rob.calib_cam(frame, oxy, board_size, realsize, world2cam))
	{
		dlg->cout2cedit(dlg->m_RESULT, "camera calibration succeeded world2cam:\r\n");
		dlg->cout2cedit(dlg->m_RESULT, world2cam);
	}
	else dlg->cout2cedit(dlg->m_RESULT, "camera calibration failed");
	dlg->GetDlgItem(IDC_CalibCam)->EnableWindow(true);
	return 0;
}

void Ctouch_ballDlg::OnBnClickedRestrobot()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_RestRobot)->EnableWindow(false);
	AfxBeginThread(Restrobot, this, THREAD_PRIORITY_HIGHEST);
	return;
}

UINT Ctouch_ballDlg::Restrobot(void* param)
{
	Ctouch_ballDlg* dlg = (Ctouch_ballDlg*)param;
	std::string temp;
	dlg->g_num_mutex.unlock();
	dlg->cout2cedit(dlg->m_RESULT, "waiting link......");
	temp = dlg->move_rob.wait_link();
	dlg->cout2cedit(dlg->m_RESULT, temp);
	temp = dlg->move_rob.receive_string();
	dlg->cout2cedit(dlg->m_RESULT, temp);
	dlg->cout2cedit(dlg->m_RESULT, "sending rest message");
	dlg->move_rob.reset_send();
	temp = dlg->move_rob.receive_string();
	dlg->cout2cedit(dlg->m_RESULT, temp);
	dlg->cout2cedit(dlg->m_RESULT, "link close");
	dlg->move_rob.close_link();
	dlg->GetDlgItem(IDC_RestRobot)->EnableWindow(true);
	return 0;
}

void Ctouch_ballDlg::OnBnClickedTouchtar()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_TouchTar)->EnableWindow(false);
	AfxBeginThread(touch_target, this, THREAD_PRIORITY_HIGHEST);
	return;
}

UINT Ctouch_ballDlg::touch_target(void* param)
{
	Ctouch_ballDlg* dlg = (Ctouch_ballDlg*)param;
	Eigen::Vector3d target_position;
	std::array<double, 7> target_move;
	std::string temp;
	//dlg->g_num_mutex.lock();
	//cv::cv2eigen(dlg->Tvec, target_position);
	target_position = dlg->Tvec;
	dlg->cout2cedit(dlg->m_RESULT, "target in camera:");
	dlg->cout2cedit(dlg->m_RESULT, target_position);

	////dlg->g_num_mutex.unlock();
	target_move = dlg->move_rob.calculate_target_point(target_position);
	dlg->cout2cedit(dlg->m_RESULT, "wait link......");
	temp = dlg->move_rob.wait_link();
	dlg->cout2cedit(dlg->m_RESULT, temp);
	temp = dlg->move_rob.receive_string();
	dlg->cout2cedit(dlg->m_RESULT, temp);
	dlg->cout2cedit(dlg->m_RESULT, "sending move message");
	dlg->move_rob.move_send(target_move);
	temp = dlg->move_rob.receive_position();
	dlg->cout2cedit(dlg->m_RESULT, temp);
	dlg->cout2cedit(dlg->m_RESULT, "link close");
	dlg->move_rob.close_link();

	dlg->GetDlgItem(IDC_TouchTar)->EnableWindow(true);
	return 0;
}


void Ctouch_ballDlg::OnBnClickedFindtar()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_FindTar)->GetWindowTextW(temp);
	if (temp == "find target")
	{
		GetDlgItem(IDC_FindTar)->EnableWindow(false);
		GetDlgItem(IDC_FindTar)->SetWindowTextW(_T("opening..."));
		find_flg = true;
		AfxBeginThread(FindTarget, this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		OnBnClickedSocketsw();
		GetDlgItem(IDC_FindTar)->EnableWindow(false);
		GetDlgItem(IDC_FindTar)->SetWindowTextW(_T("closing"));
		find_flg = false;
	}
	return;
}

UINT Ctouch_ballDlg::FindTarget(void* param)
{
	Ctouch_ballDlg* dlg = (Ctouch_ballDlg*)param;
	Eigen::Vector3d target_position_c, target_position_w, tool_now_position_w;
	std::array<double, 7> target_move;
	std::string temp;
	bool flg = true;
	while (dlg->find_flg)
	{
		dlg->cout2cedit(dlg->m_RESULT, "waiting link......");
		temp = dlg->move_rob.wait_link();
		dlg->cout2cedit(dlg->m_RESULT, temp);
		temp = dlg->move_rob.receive_string();
		dlg->cout2cedit(dlg->m_RESULT, temp);
		dlg->cout2cedit(dlg->m_RESULT, "detecting target ......");
		//dlg->g_num_mutex.lock();
		//cv::cv2eigen(dlg->Tvec, target_position_c);
		target_position_c = dlg->Tvec;
		dlg->cout2cedit(dlg->m_RESULT, "target in world:");
		dlg->cout2cedit(dlg->m_RESULT, target_position_c);
		//dlg->g_num_mutex.unlock();
		if (flg)//the first time
		{
			tool_now_position_w = { dlg->rest_position_input[0],dlg->rest_position_input[1], dlg->rest_position_input[2] };
			flg = false;
			dlg->GetDlgItem(IDC_FindTar)->SetWindowTextW(_T("stop find"));
			dlg->GetDlgItem(IDC_FindTar)->EnableWindow(true);
		}
		dlg->cout2cedit(dlg->m_RESULT, "route planning ......");
		target_move = dlg->move_rob.calculate_next_point(target_position_c, tool_now_position_w);
		Eigen::MatrixXd target_cam_H(4, 1);
		target_cam_H.topLeftCorner(3, 1) = target_position_c; target_cam_H(3, 0) = 1.0;
		target_position_w = (dlg->move_rob.get_worldcoord2cam()*target_cam_H).topLeftCorner(3, 1);
		//if((target_position_w - tool_now_position_w).norm()>2) //1cm
		tool_now_position_w = { target_move[0], target_move[1], target_move[2] };
		dlg->cout2cedit(dlg->m_RESULT, "sending move message");
		dlg->move_rob.move_send(target_move);
		temp = dlg->move_rob.receive_position();
		dlg->cout2cedit(dlg->m_RESULT, temp);
		dlg->cout2cedit(dlg->m_RESULT, "link close");
		dlg->move_rob.close_link();
	}
	dlg->GetDlgItem(IDC_FindTar)->SetWindowTextW(_T("find target"));
	dlg->GetDlgItem(IDC_FindTar)->EnableWindow(true);
	return 0;
}