// TestEasyPr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testPR.h"
#include "TestEasyPr.h"
#include "afxdialogex.h"
#include "easypr.h"
using namespace easypr;
using namespace cv;
using namespace std;
using namespace api;

CTestEasyPr *CTestEasyPr::m_testDlg = NULL;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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

// TestEasyPr �Ի���

IMPLEMENT_DYNAMIC(CTestEasyPr, CDialogEx)

CTestEasyPr::CTestEasyPr(CWnd* pParent /*=NULL*/)
: CDialogEx(CTestEasyPr::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CTestEasyPr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_testDlg = this;
	m_SettingDlg = new CSetting();
	m_batchTestDlg = new CBatchTest();
	m_singleTestDlg = new CSingleTest();
	m_batchTestDlg->Create(IDD_TESTPR_BATCH, GetDlgItem(IDC_EASYPRTAB));
	m_singleTestDlg->Create(IDD_TESTPR_SINGLE, GetDlgItem(IDC_EASYPRTAB));

	m_easyprTab = ((CTabCtrl*)GetDlgItem(IDC_EASYPRTAB));
	m_easyprTab->InsertItem(0,_T("Single Test"));
	m_easyprTab->InsertItem(1, _T("Batch Test"));

	CRect rs;
	m_easyprTab->GetClientRect(&rs);

	rs.top += 25;
	rs.bottom -= 5;
	rs.left += 5;
	rs.right -= 5;

	m_batchTestDlg->MoveWindow(&rs);
	m_singleTestDlg->MoveWindow(&rs);

	m_batchTestDlg->ShowWindow(false);
	m_singleTestDlg->ShowWindow(true);

	m_easyprTab->SetCurSel(0);

    m_singleTestDlg->m_single_pd.setDetectType(PR_DETECT_CMSER);

	m_singleTestDlg->m_single_pd.setPDLifemode(false);

	m_singleTestDlg->m_single_pr.setDetectType(PR_DETECT_CMSER);

	m_singleTestDlg->m_single_pr.setPDLifemode(false);

	m_batchTestDlg->m_batch_pd.setDetectType(PR_DETECT_CMSER);

	m_batchTestDlg->m_batch_pd.setPDLifemode(false);

	return TRUE;
}

CTestEasyPr::~CTestEasyPr()
{
}

void CTestEasyPr::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestEasyPr::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestEasyPr::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestEasyPr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestEasyPr, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_EASYPRTAB, &CTestEasyPr::OnTcnSelchangeEasyprtab)
	ON_COMMAND(ID_SETTING_PATAMETERSETTING, &CTestEasyPr::OnSettingPatametersetting)
	ON_COMMAND(ID_ABOUT_VERSION, &CTestEasyPr::OnAboutVersion)
END_MESSAGE_MAP()

void CTestEasyPr::OnSize(UINT nType, int cx, int cy)
{/*
	CWnd *pWnd1[4];
	pWnd1[0] = GetDlgItem(IDD_TESTPR_SINGLE);
	pWnd1[1] = GetDlgItem(IDC_EASYPRTAB);
	pWnd1[2] = GetDlgItem(IDD_TESTPR);
	pWnd1[3] = GetDlgItem(SOURCE_IMAGE);
	*/
}
// TestEasyPr ��Ϣ�������

void CTestEasyPr::OnTcnSelchangeEasyprtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int curSel = m_easyprTab->GetCurSel();
	switch (curSel)
	{
	case 0:
		m_batchTestDlg->ShowWindow(false);
		m_singleTestDlg->ShowWindow(true);
		break;

	case 1:
		m_batchTestDlg->ShowWindow(true);
		m_singleTestDlg->ShowWindow(false);
		break;

	default:
		break;
	}

	*pResult = 0;
}


void CTestEasyPr::OnSettingPatametersetting()
{
	//m_SettingDlg->ShowWindow(SW_SHOW);
	if (m_SettingDlg->DoModal() == IDOK)
	{
		m_singleTestDlg->m_single_pd.setDetectType(m_SettingDlg->m_detect_Type);
		m_singleTestDlg->m_single_pd.setPDLifemode(m_SettingDlg->m_bLifeMode);
		m_singleTestDlg->m_single_pr.setDetectType(m_SettingDlg->m_detect_Type);
		m_singleTestDlg->m_single_pr.setPDLifemode(m_bLifeMode);

		m_batchTestDlg->m_batch_pd.setDetectType(m_SettingDlg->m_detect_Type);
		m_batchTestDlg->m_batch_pd.setPDLifemode(m_SettingDlg->m_bLifeMode);
	}
}


void CTestEasyPr::OnAboutVersion()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
