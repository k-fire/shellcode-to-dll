
// output_dllDlg.h: 头文件
//

#pragma once


// CoutputdllDlg 对话框
class CoutputdllDlg : public CDialogEx
{
// 构造
public:
	CoutputdllDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT_DLL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL ReleaseRes(CString strFileName, WORD wResID, CString strFileType);
//	CEdit m_edit1;
//	CEdit m_edit2;
//	char *m_edit1;
//	unsigned char m_edit1;
//	CString m_edit1;
	CEdit m_edit1;
	CEdit m_edit2;
	CString init_text1;
	CString init_text2;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnSetfocusEdit1();
};
