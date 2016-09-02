
// testPRDlg.h : ͷ�ļ�
#include <string>
#pragma once


// CtestPRDlg �Ի���
class CtestPRDlg : public CDialogEx
{
// ����
public:
	CtestPRDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTPR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CRect m_dlgRect;
	std::string m_srcPath;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//CString m_srcPath;
	afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedProcessButton();

private:
	int test_plate_locate();
	int test_plate_judge();
	int test_plate_detect();
	int test_plate_recognize();
	int test_chars_segment();

	void ChangeSize(UINT nID, int x, int y);
};
