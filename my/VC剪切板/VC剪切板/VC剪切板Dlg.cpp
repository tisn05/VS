
// VC剪切板Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VC剪切板.h"
#include "VC剪切板Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CVC剪切板Dlg 对话框



CVC剪切板Dlg::CVC剪切板Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVC剪切板Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC剪切板Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVC剪切板Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CLIPBOARDUPDATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CVC剪切板Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CVC剪切板Dlg 消息处理程序

BOOL CVC剪切板Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	AddClipboardFormatListener(this->m_hWnd);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVC剪切板Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVC剪切板Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVC剪切板Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVC剪切板Dlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	RemoveClipboardFormatListener(this->m_hWnd);

	CDialogEx::OnClose();
}


void CVC剪切板Dlg::OnClipboardUpdate()
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!OpenClipboard())
		return;
	UINT uFormat;
	char szFormatName[1024];
	CString lpFormatName;
	uFormat = EnumClipboardFormats(0);
	while (uFormat)
	{
		//GetPriorityClipboardFormat()
		if (InArray(uFormat)){
			CString str;
			str.Format(_T("%d"), uFormat);
			lpFormatName = str;
		}
		else{
			if (GetClipboardFormatName(uFormat, (LPWSTR)szFormatName, 1024))
				lpFormatName = (LPWSTR)szFormatName;
			else
				lpFormatName = _T("(unknown)");
		}
		HGLOBAL hData = GetClipboardData(uFormat);
		byte *bData = (byte *)GlobalLock(hData);
		if (bData != NULL){
			WFile(lpFormatName, bData, GlobalSize(hData));
			GlobalUnlock(hData);
		}
		uFormat = EnumClipboardFormats(uFormat);
	}

	CloseClipboard();

	CDialogEx::OnClipboardUpdate();
}

void CVC剪切板Dlg::WFile(LPCTSTR fileName, byte *content, UINT dLen){
	HANDLE hFile = CreateFile(CString("D:\\test\\clipboard\\") + CString(fileName), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile){
		DWORD dwSize = 0;
		WriteFile(hFile, content, dLen, &dwSize, NULL);
		CloseHandle(hFile);
	}
}


void CVC剪切板Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileFind *cff = new CFileFind();
}

BOOL CVC剪切板Dlg::InArray(UINT a){
	UINT ar[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0x0080, 0x0081, 0x0082, 0x0083, 0x008E, 0x0200, 0x02FF, 0x0300, 0x03FF };;
	for (int i = 0; i < 27; i++){
		if (ar[i] == a)
			return TRUE;
	}
	return FALSE;
}