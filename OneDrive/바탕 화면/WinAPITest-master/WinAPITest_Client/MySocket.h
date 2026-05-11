#pragma once

#include <afxsock.h>
#include <vector>
#include "CLogin.h"
#include "CWorld.h"
//class CchatClientDlg;

enum e_Status
{
    E_LOGIN = 0,
    E_WORLD,
    E_CHAT,
};

class CMySocket : public CAsyncSocket {

public:
    CMySocket(CDialogEx* pDlg, e_Status eStatus = E_LOGIN);
    ~CMySocket();

public:
    virtual void OnReceive(int nErrorCode) override ;
    virtual void OnConnect(int nErrorCode) override ;
    BOOL connect(const CString& strHost, const int nPort);

    std::string RecevieBuff();
    //void Parse(CchatClientDlg* pDlg);
    BOOL SendSync(const char* buf, const int len);
    bool SendPacket(const std::string& packet);

public:
    CDialogEx* m_dlg;

    std::vector<char> m_recvBuff;

    BOOL m_bConnect;
	BOOL m_bChatSocket;
    BOOL m_bLoginPhase;
    BOOL m_bRegister;
};