// BatchTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testPR.h"
#include "BatchTest.h"
#include "afxdialogex.h"
#include "easypr.h"

using namespace easypr;
using namespace cv;
using namespace std;
using namespace api;
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
	CFileFind fileFinder;//��CFileFindִ�б����ļ�����  
	CString strPicFile = TEXT("");//  
	CString strFilePath;//�����ļ�·��  
	CString strFileName;//�����ļ���  
	int nIndex = 0;
	do
	{
		if (m_folderPath.Right(1) == TEXT("\\"))
		{
			int nPos = m_folderPath.ReverseFind(TEXT('\\'));
			m_folderPath = m_folderPath.Left(nPos);
		}
		strPicFile.Format(TEXT("%s\\%s"), m_folderPath, TEXT("*.jpg"));//ֻѡ��jpg��ʽ��ͼƬ  
		BOOL bWorking = fileFinder.FindFile(strPicFile);
		while (bWorking)
		{
			bWorking = fileFinder.FindNextFile();
			if (fileFinder.IsDots())//IsDots�ж��Ƿ�Ϊ��,��CFileFind��������IsDots����˼�ǣ�����һ��Ŀ¼���Ҳ������Ŀ¼��������ϲ�Ŀ¼  
			{
				continue;
			}
			strFilePath = fileFinder.GetFilePath();//ͼƬ������·��  
			strFileName = fileFinder.GetFileName();//ͼƬ�ļ�������  
			if (fileFinder.IsDirectory())//����Ƿ����ļ��У��Ƿ���true�����Ƿ���false  
			{
				//��������Ŀ¼  
				continue;
			}
			else
			{
				int nPos = strFilePath.ReverseFind(TEXT('.'));
				CString strExt = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
				if (strExt.CompareNoCase(TEXT("jpg")) == 0 ||
					strExt.CompareNoCase(TEXT("jpeg")) == 0 ||
					strExt.CompareNoCase(TEXT("bmp")) == 0)
				{
					//Ҫ���е���������  
					processPlate(strFilePath);

					m_BatchList->InsertItem(nIndex, strFileName);//������
					m_BatchList->SetItemText(nIndex, 1, m_plateResult);//���ø��еĲ�ͬ�е���ʾ�ַ�
					m_BatchList->SetItemText(nIndex, 2, m_pdTime);//���ø��еĲ�ͬ�е���ʾ�ַ�
					m_BatchList->SetItemText(nIndex, 3, m_colorTime);//���ø��еĲ�ͬ�е���ʾ�ַ�
					m_BatchList->SetItemText(nIndex, 4, m_crTime);//���ø��еĲ�ͬ�е���ʾ�ַ�
					nIndex++;
				}
			}
		}
	} while (fileFinder.FindNextFile());

	fileFinder.Close();
}


void CBatchTest::processPlate(CString platePath)
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER start, end;
	double eslapsPD;
	double eslapsColor;
	double eslapsCR;


	std::vector<CPlate> plateVec;
	USES_CONVERSION;
	string tmps_srcPath(W2A(platePath));
	cv::Mat src = imread(tmps_srcPath);
	CPlateDetect pd;
	CCharsRecognise cr;

	QueryPerformanceFrequency(&nFreq);//����ÿ���������ĸ���,��Ƶ��
	QueryPerformanceCounter(&start);  //��ȡ��ʼʱ����������ֵ 

	int resultPD = pd.plateDetect(src, plateVec);


	QueryPerformanceCounter(&end);    //��ȡ����ʱ����������ֵ
	eslapsPD = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;


	if (resultPD == 0)
	{
		size_t num = plateVec.size();
		int index = 0;

		for (size_t j = 0; j < num; j++)
		{
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


			std::string plateIdentify = "";

			QueryPerformanceFrequency(&nFreq);//����ÿ���������ĸ���,��Ƶ��
			QueryPerformanceCounter(&start);  //��ȡ��ʼʱ����������ֵ 

			int resultCR = cr.charsRecognise(item, plateIdentify);

			QueryPerformanceCounter(&end);    //��ȡ����ʱ����������ֵ
			eslapsCR = (double)(end.QuadPart - start.QuadPart) / (double)nFreq.QuadPart;

			plateIdentify = plateColor + ": " + plateIdentify;
			m_plateResult = plateIdentify.c_str();

			m_pdTime.Format(_T("%.3f"), eslapsPD*1000);
			m_colorTime.Format(_T("%.3f"), eslapsColor*1000);
			m_crTime.Format(_T("%.3f"), eslapsCR*1000);
		}
	}
}