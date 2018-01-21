
// socket_testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "socket_test.h"
#include "socket_testDlg.h"
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
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Csocket_testDlg dialog



Csocket_testDlg::Csocket_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Csocket_testDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csocket_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}

BEGIN_MESSAGE_MAP(Csocket_testDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Csocket_testDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_MESSAGE(WM_MC_TCPIP_MSG, OnReceiveMessage)
END_MESSAGE_MAP()


// Csocket_testDlg message handlers

BOOL Csocket_testDlg::OnInitDialog()
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
	SetTimer(10, 100, NULL);
	pAlignInterface = new CAlign_Interface(1);
	pAlignInterface->SetHwnd(this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Csocket_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Csocket_testDlg::OnPaint()
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
HCURSOR Csocket_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Csocket_testDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString str;

	time_t t2 = time(0);   // get time now
	struct tm * now2 = localtime(&t2);

	CString s;
	s.Format(_T("[%d/%d %d:%d:%d] [send] "), now2->tm_mon+1, now2->tm_mday, now2->tm_hour, now2->tm_min, now2->tm_sec );

	GetDlgItemText(IDC_EDIT1, str);
	m_list1.AddString(s + str);
}


void Csocket_testDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT Csocket_testDlg::OnReceiveMessage(WPARAM wParam, LPARAM lParam)
{
	CString str;
	LPCTSTR lpszString = (LPCTSTR)lParam;

	time_t t2 = time(0);   // get time now
	struct tm * now2 = localtime(&t2);

	CString s;
	s.Format(_T("[%d/%d %d:%d:%d] "), now2->tm_mon + 1, now2->tm_mday, now2->tm_hour, now2->tm_min, now2->tm_sec);
	m_list1.InsertString(-1, s + lpszString);

	return 0;
}