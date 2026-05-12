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
//#include "UTIL.h"

//로그인 아이디
extern std::string g_account_id;

// CWorld 대화 상자

IMPLEMENT_DYNAMIC(CWorld, CDialogEx)

namespace UTIL
{
	static CString Utf8ToCString(const std::string& utf8)
	{
		int len = MultiByteToWideChar(
			CP_UTF8, 0,
			utf8.c_str(), (int)utf8.size(),
			nullptr, 0
		);

		CString result;
		wchar_t* buf = result.GetBuffer(len);
		MultiByteToWideChar(
			CP_UTF8, 0,
			utf8.c_str(), (int)utf8.size(),
			buf, len
		);
		result.ReleaseBuffer(len);

		return result;
	}
};

CWorld::CWorld(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORLD, pParent)
{
	m_bConnect = FALSE;
	m_pSock = new CMySocket(this, E_WORLD_INIT);
	m_pRegDlg = new CRegister(m_pSock);
}

CWorld::CWorld(CMySocket* sock, CWnd* pParent /*=nullptr*/) : m_pSock(sock), CDialogEx(IDD_WORLD, pParent), m_bConnect(FALSE)
{
	m_pRegDlg = new CRegister(m_pSock);
}

CWorld::~CWorld()
{
}

void CWorld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHARLIST, m_editCharList);
}

BOOL CWorld::connect()
{
	BOOL bRet;
	CString strHost = _T("100.114.42.54");
	CString strPort = _T("5500");
	
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

void CWorld::OnSocketConnect(BOOL bConnect)
{
	if (bConnect)
	{
		m_bConnect = TRUE;
		this->InitWorld();
	}
}


int CWorld::InitWorld()
{
	int rc = EXIT_FAILURE;

	m_pSock->m_status = E_WORLD_INIT;

	std::vector<std::string> datas;
	std::string body, pkt;

	datas.push_back(g_account_id);
	body = PacketParser::MakeBody(datas);
	pkt = PacketParser::MakePacket(PKT_INIT_WORLD, body);

	m_pSock->SendPacket(pkt);

	rc = EXIT_SUCCESS;
err:

	return rc;
}

int CWorld::OnInitWorld(const char* recvBuff, const int recvLen)
{
	int rc = EXIT_FAILURE;

	size_t offset = 0;
	std::string status, errMsg;
	std::string sBuff;
	std::vector<char> vBuff;

	sBuff.append(recvBuff, recvLen);
	vBuff.insert(vBuff.end(), sBuff.begin(), sBuff.end());
	auto pkt = PacketParser::Parse(vBuff);
	if (!pkt.has_value())
	{
		return -1;
	}

	PacketParser::ParseLengthPrefixedString(pkt->payload.c_str(), pkt->payload.size(), offset, status, errMsg);

	if (status == "nok")
	{
		CString strTmp;
		PacketParser::ParseLengthPrefixedString(recvBuff, recvLen, offset, status, errMsg);
		strTmp.Format(_T("World 초기화 실패: %s"), CString(status.c_str()));
		AfxMessageBox(strTmp);
		goto err;
	}

	rc = EXIT_SUCCESS;
err:

	if (rc != EXIT_SUCCESS)
	{
		//AfxMessageBox(_T("로그인 실패: 회원가입을 하세요"));
	}
	else
	{
		AfxMessageBox(_T("World 성공"));
		this->CharacterList(); //캐릭터 선택
		//m_pSock->m_bWorldPhase = FALSE; //로그인 끝
		//EndDialog(IDOK);
	}

	return rc;
}

int CWorld::CharacterList()
{
	int rc = EXIT_FAILURE;

	m_pSock->m_status = E_WORLD_CHAR_LIST;

	std::vector<std::string> datas;
	std::string body, pkt;

	body = PacketParser::MakeBody({});
	pkt = PacketParser::MakePacket(PKT_SELECT_CHARACTER, body);

	m_pSock->SendPacket(pkt);
	
	rc = EXIT_SUCCESS;
err:

	return rc;
}

int CWorld::OnCharacterList(const char* recvBuff, const int recvLen)
{
	int i;
	char* context = NULL;
	char* pLine = NULL;
	int rc = EXIT_FAILURE;
	size_t offset = 0;
	std::string status, errMsg;
	CString strCharList;

	std::string sBuff;
	std::vector<char> vBuff;

	sBuff.append(recvBuff, recvLen);
	vBuff.insert(vBuff.end(), sBuff.begin(), sBuff.end());
	auto pkt = PacketParser::Parse(vBuff);
	if (!pkt.has_value())
	{
		return -1;
	}

	
	//반복하여 캐릭터 닉네임 추출
	while (1)
	{
		std::string char_name;

		if (!PacketParser::ParseLengthPrefixedString(
			pkt->payload.c_str(),
			pkt->payload.size(),
			offset,
			char_name,
			errMsg))
		{
			//더이상 없으면 중단
			//K_slog_trace(K_SLOG_DEBUG, "[%s][%d]gunoo22_TEST", __FUNCTION__, __LINE__);
			break;
		}

		CString wideValue = UTIL::Utf8ToCString(char_name);

		/*CString line;
		line.Format(
			L"FIELD[%d] : %s\r\n",
			fieldIndex++,
			wideValue.GetString()
		);
		m_listResponse.InsertString(nIdxResponse++, line);*/
		
		if (strCharList.GetLength() == 0)
		{
			strCharList.Format(L"%s", wideValue.GetString());
		}
		else
		{
			strCharList.Format(L"%s,%s", strCharList, wideValue.GetString());
		}
	}

	m_editCharList.SetWindowText(strCharList);


	rc = EXIT_SUCCESS;
err:

	if (rc != EXIT_SUCCESS)
	{
		//AfxMessageBox(_T("로그인 실패: 회원가입을 하세요"));
	}
	else
	{
		AfxMessageBox(_T("CharList 성공"));
		//this->CharacterList(); //캐릭터 선택
		//m_pSock->m_bWorldPhase = FALSE; //로그인 끝
		//EndDialog(IDOK);
	}

	return rc;
}




//
////로그인 버튼 클릭
//void CWorld::OnBnClickedButtonLogin()
//{
//	m_editHost.GetWindowTextW(m_strHost);
//	m_editPort.GetWindowTextW(m_strPort);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//	//connect();
//	//Login();
//
//	//AfxMessageBox(_T("로그인 성공"));
//	////		m_pSock->m_bLoginPhase = FALSE; //로그인 끝
//	//EndDialog(IDOK);
//
//	if (m_bConnect == FALSE)
//		connect();
//	else
//		OnSocketConnect(m_bConnect);
//}
//
