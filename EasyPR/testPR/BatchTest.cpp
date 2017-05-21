// BatchTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testPR.h"
#include "BatchTest.h"
#include "afxdialogex.h"

// CBatchTest �Ի���

IMPLEMENT_DYNAMIC(CBatchTest, CDialogEx)

CBatchTest::CBatchTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchTest::IDD, pParent)
{

}

CBatchTest::~CBatchTest()
{
}

void CBatchTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBatchTest, CDialogEx)
	ON_BN_CLICKED(BATCH_CHOOSE_BTN, &CBatchTest::OnBnClickedChooseBtn)
	ON_BN_CLICKED(BATCH_START_BTN, &CBatchTest::OnBnClickedStartBtn)
END_MESSAGE_MAP()

BOOL CBatchTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_BatchList = ((CListCtrl*)GetDlgItem(BTACH_SHOW_LIST));

	//���ԭ�з��
	DWORD dwStyle = ::GetWindowLong(m_BatchList->m_hWnd, GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//�����·��
	SetWindowLong(m_BatchList->m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS);
	//������չ���
	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_BatchList->m_hWnd, styles, styles);
	/*
	����LVS_EX_FULLROWSELECT������ǰ��˵������ѡ��
		LVS_EX_GRIDLINES�������ߣ�ֻ������report����listctrl��
		LVS_EX_CHECKBOXES��ǰ��Ӹ�checkbox
		*/
	m_BatchList->SetExtendedStyle(m_BatchList->GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);

	m_BatchList->DeleteAllItems();//���
	m_BatchList->InsertColumn(0, _T("file name"));//�����
	m_BatchList->InsertColumn(1, _T("recognise result"));//�����
	m_BatchList->InsertColumn(2, _T("plate Detect"));
	m_BatchList->InsertColumn(3, _T("get Plate Color"));
	m_BatchList->InsertColumn(4, _T("chars Recognise"));
	m_BatchList->SetColumnWidth(0, 108);//�����п�
	m_BatchList->SetColumnWidth(1, 108);
	m_BatchList->SetColumnWidth(2, 108);//�����п�
	m_BatchList->SetColumnWidth(3, 108);
	m_BatchList->SetColumnWidth(4, 108);
	//BATCH_PATH_EDIT

	return TRUE;
}
// CBatchTest ��Ϣ�������


void CBatchTest::OnBnClickedChooseBtn()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("Please choose a folder:");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// ȡ���ļ�����  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			m_folderPath = szFolderPath;
			GetDlgItem(BATCH_PATH_EDIT)->SetWindowTextW(m_folderPath);
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
}


void CBatchTest::OnBnClickedStartBtn()
{
	if (m_folderPath.IsEmpty())
	{
		return;
	}

	CPaintDC paintDC(this);

	CFileFind fileFinder;//��CFileFindִ�б����ļ�����  
	CString strPicFile = TEXT("");//  
	CString strFilePath;//�����ļ�·��  
	CString strFileName;//�����ļ���  
	int nIndex = 0;
	m_BatchList->DeleteAllItems();//���

	char buffer[260] = { 0 };

	USES_CONVERSION;
	string tmps_srcPath(W2A(m_folderPath));
	vector<string> imgPathTrain = utils::getFiles(tmps_srcPath.c_str());
	int fileNum = imgPathTrain.size();

	//��ʾ��ǰʶ��Ľ���
	CProgressCtrl myProCtrl;
	//myProCtrl.EnableWindow(TRUE);

	CRect rect, proRect;
	GetClientRect(&rect);
	proRect.left = rect.left + rect.Width() / 2 - 100;
	proRect.top = rect.top + rect.Height() / 2 - 20;
	proRect.right = rect.right - rect.Width() / 2 + 100;
	proRect.bottom = rect.bottom - rect.Height() / 2 + 20;
	//WS_CHILD|WS_VISIBLE|PBS_SMOOTHREVERSE
	myProCtrl.Create(WS_VISIBLE, proRect, this, 99); //����λ�á���С
	myProCtrl.SetRange(0, fileNum);

	for (auto file : imgPathTrain)
	{
		auto image = cv::imread(file);
		if (!image.data) {
			fprintf(stdout, ">> Invalid image: %s  ignore.\n", file.c_str());
			continue;
		}

		strFileName = (utils::getFileName(file)).c_str();

		processPlate(image);

		m_BatchList->InsertItem(nIndex, strFileName);//������
		m_BatchList->SetItemText(nIndex, 1, m_plateResult);//���ø��еĲ�ͬ�е���ʾ�ַ�
		m_BatchList->SetItemText(nIndex, 2, m_pdTime);//���ø��еĲ�ͬ�е���ʾ�ַ�
		m_BatchList->SetItemText(nIndex, 3, m_colorTime);//���ø��еĲ�ͬ�е���ʾ�ַ�
		m_BatchList->SetItemText(nIndex, 4, m_crTime);//���ø��еĲ�ͬ�е���ʾ�ַ�
		nIndex++;

		myProCtrl.OffsetPos(1);

		//double Fraction = (double)nIndex / ((double)fileNum);
		//CString str;
		//str.Format(_T("%d%%"), (int)(Fraction*100.0));
		//CRgn rgn;
		//rgn.CreateRectRgn(proRect.left, proRect.top, proRect.right, proRect.bottom);
		//dc.SelectClipRgn(&rgn);
		//paintDC.SetTextColor(RGB(235, 235, 235));
		//paintDC.DrawText(str, proRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//str.Format(_T("%d%%"), nIndex); //�ٷֱ�
		//myProCtrl.SetWindowText(str);
		//myProCtrl.SetDlgItemTextW(str);
	}
}


void CBatchTest::processPlate(Mat& src)
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER start, end;
	double eslapsPD;
	double eslapsColor;
	double eslapsCR;


	std::vector<CPlate> plateVec;

	CCharsRecognise cr;

	QueryPerformanceFrequency(&nFreq);//����ÿ���������ĸ���,��Ƶ��
	QueryPerformanceCounter(&start);  //��ȡ��ʼʱ����������ֵ 

	//pd.setDetectType(PR_DETECT_CMSER);
	int resultPD = m_batch_pd.plateDetect(src, plateVec);


	QueryPerformanceCounter(&end);    //��ȡ����ʱ����������ֵ
	eslapsPD = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;
	size_t num = plateVec.size();

	if ((resultPD == 0) && (num > 0))
	{		
		int index = 0;
		std::string plateIdentify = "";

		for (size_t j = 0; j < num; j++)
		{
			std::string tmpIdentify = "";

		    CString plateIndex = _T("");
			plateIndex.Format(_T("%d"), j);

			CPlate item = plateVec.at(j);
			Mat plateMat = item.getPlateMat();

			Color color = item.getPlateColor();
			if (color == UNKNOWN)
			{
				color = getPlateType(plateMat, true);
				item.setPlateColor(color);
			}

			QueryPerformanceFrequency(&nFreq);//����ÿ���������ĸ���,��Ƶ��
			QueryPerformanceCounter(&start);  //��ȡ��ʼʱ����������ֵ 

			std::string plateColor = cr.getPlateColor(color);

			QueryPerformanceCounter(&end);    //��ȡ����ʱ����������ֵ
			eslapsColor = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;		

			QueryPerformanceFrequency(&nFreq);//����ÿ���������ĸ���,��Ƶ��
			QueryPerformanceCounter(&start);  //��ȡ��ʼʱ����������ֵ 

			int resultCR = cr.charsRecognise(item, tmpIdentify);

			QueryPerformanceCounter(&end);    //��ȡ����ʱ����������ֵ
			eslapsCR = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;

			plateIdentify = plateIdentify + plateColor + ": " + tmpIdentify + "\r\n";

			m_pdTime.Format(_T("%.3f"), eslapsPD*1000);
			m_colorTime.Format(_T("%.3f"), eslapsColor*1000);
			m_crTime.Format(_T("%.3f"), eslapsCR*1000);
		}
		m_plateResult = plateIdentify.c_str();
		
	}
}