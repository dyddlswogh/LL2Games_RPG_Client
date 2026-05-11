// CWorld.cpp: 구현 파일
//

//#include "pch.h"
#include "afxdialogex.h"
#include "CWorld.h"
#include "resource.h"
#include <vector>
#include "..\\WinAPITest_Source\\Packet.h"
#include "..\\WinAPITest_Source\\PacketParser.h"
//#include "..\\WinAPITest_Source\\MySocket.h"
#include "MySocket.h"


// CWorld 대화 상자

IMPLEMENT_DYNAMIC(CWorld, CDialogEx)

CWorld::CWorld(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORLD, pParent)
{
	m_strHost = _T("");
	m_strPort = _T("");
	m_bConnect = FALSE;
	m_pSock = new CMySocket(this, E_WORLD);
	m_pRegDlg = new CRegister(m_pSock);
}

CWorld::CWorld(CMySocket* sock, CWnd* pParent /*=nullptr*/) : m_pSock(sock), CDialogEx(IDD_WORLD, pParent), m_bConnect(FALSE)
{
	m_strHost = _T("");
	m_strPort = _T("");
	m_pRegDlg = new CRegister(m_pSock);
}

CWorld::~CWorld()
{
}

void CWorld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CWorld::connect()
{
	BOOL bRet;
	CString strHost = _T("5000");
	CString strPort = _T("5000");
	
	return m_pSock->connect(strHost, atoi(CT2A(strPort)));
}


BEGIN_MESSAGE_MAP(CWorld, CDialogEx)
END_MESSAGE_MAP()


// CWorld 메시지 처리기

BOOL CWorld::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	/*m_editHost.SetWindowTextW(_T("100.114.42.54"));
	m_editPort.SetWindowTextW(_T("5000"));

	m_editID.SetWindowTextW(_T("admin1"));
	m_editPasswd.SetWindowTextW(_T("1111"));*/

	if (m_bConnect == FALSE)
		connect();
	else
		OnSocketConnect(m_bConnect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CWorld::Login()
{
	int rc = EXIT_FAILURE;
	char* buff = NULL;
	int nBuffLen = 0;
	int nSendLen = 0;
	CString strID, strPasswd;
	std::vector<std::string> datas;
	std::string body, pkt;

	m_editID.GetWindowText(strID);
	m_editPasswd.GetWindowText(strPasswd);

	nBuffLen = strID.GetLength() + strPasswd.GetLength() + 10;
	buff = (char*)calloc(nBuffLen, sizeof(char));
	if (buff == NULL)
	{
		AfxMessageBox(_T("메모리 할당 실패"));
		goto err;
	}
	//nSendLen = sprintf_s(buff, nBuffLen, "%s$%s$", CStringA(strID), CStringA(strPasswd));
	//PacketUtility::SendPacket(m_pSock, PKT_LOGIN, std::string(buff));

	datas.push_back(std::string((CStringA(strID))));
	datas.push_back(std::string((CStringA(strPasswd))));

	body = PacketParser::MakeBody(datas);
	pkt = PacketParser::MakePacket(PKT_LOGIN, body);

	m_pSock->SendPacket(pkt);
	
	m_editID.GetWindowText(m_strID);
	rc = EXIT_SUCCESS;
err:
	free(buff);
	buff = NULL;

	return rc;
}

int CWorld::OnLogin(const char * recvBuff, const int recvLen)
{
	int i;
	char* context = NULL;
	char* pLine = NULL;
	int rc = EXIT_FAILURE;

	
	//m_pSock->Receive(recvBuff, 2048);
	{ char szTmp[2058]; sprintf_s(szTmp, sizeof(szTmp), "gunoo22_TEST recvBuff[%s]", recvBuff); OutputDebugStringA(szTmp); }

	if (recvLen == 0)
		goto err;

	for (pLine = strtok_s((char *)recvBuff, "$", &context), i = 0; pLine; pLine = strtok_s(NULL, "$", &context), i++)
	{
		switch (i)
		{
		case 0:
			if (!strcmp(pLine, "NOK"))
			{
				CString strTmp;
				pLine = strtok_s(NULL, "$", &context);
				strTmp.Format(_T("로그인 실패: %s"), CString(pLine));
				AfxMessageBox(strTmp);
				goto err;
			}
			break;
		}
	}

	rc = EXIT_SUCCESS;
err:

	if (rc != EXIT_SUCCESS)
	{
		AfxMessageBox(_T("로그인 실패: 회원가입을 하세요"));
	}
	else
	{
		AfxMessageBox(_T("로그인 성공"));
		m_pSock->m_bLoginPhase = FALSE; //로그인 끝
		EndDialog(IDOK);
	}

	return rc;
}

void CWorld::OnSocketConnect(BOOL bConnect)
{
	if (bConnect)
	{
		m_bConnect = TRUE;
		if (m_pSock->m_bRegister)
			OnBnClickedButtonRegister();
		else
			Login();
	}
}

//로그인 버튼 클릭
void CWorld::OnBnClickedButtonLogin()
{
	m_editHost.GetWindowTextW(m_strHost);
	m_editPort.GetWindowTextW(m_strPort);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//connect();
	//Login();

	//AfxMessageBox(_T("로그인 성공"));
	////		m_pSock->m_bLoginPhase = FALSE; //로그인 끝
	//EndDialog(IDOK);

	if (m_bConnect == FALSE)
		connect();
	else
		OnSocketConnect(m_bConnect);
}

//회원가입 버튼 클릭
void CWorld::OnBnClickedButtonRegister()
{
	m_pSock->m_bRegister = TRUE;

	if (m_bConnect == FALSE)
		connect();
	else
	{
		m_pRegDlg->DoModal();
	}
	m_pSock->m_bRegister = FALSE;
}


