#pragma once
#include <string>

// CBatchTest �Ի���

class CBatchTest : public CDialogEx
{
	DECLARE_DYNAMIC(CBatchTest)

public:
	CBatchTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBatchTest();

// �Ի�������
	enum { IDD = IDD_TESTPR_BATCH };

protected:
	CListCtrl* m_BatchList;
	CString m_folderPath;
	CString m_plateResult;
	std::string m_srcPath;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void processPlate(CString platePath);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChooseBtn();
	afx_msg void OnBnClickedStartBtn();
};
