#pragma once
#include "SingleTest.h"
#include "BatchTest.h"
#include "Setting.h"
// TestEasyPr �Ի���

class CTestEasyPr : public CDialogEx
{
	DECLARE_DYNAMIC(CTestEasyPr)

public:
	CTestEasyPr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestEasyPr();
	CTabCtrl* m_easyprTab;
	CBatchTest* m_batchTestDlg;
	CSingleTest* m_singleTestDlg;
	CSetting* m_SettingDlg;
	static CTestEasyPr* m_testDlg;
	void static OnSize(UINT nType, int cx, int cy);

	int m_detect_Type = 0;

	bool m_bLifeMode = false;

// �Ի�������
	enum { IDD = IDD_TESTPR };
	DECLARE_MESSAGE_MAP()
protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();


public:
	afx_msg void OnTcnSelchangeEasyprtab(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnSettingPatametersetting();
	afx_msg void OnAboutVersion();
};
