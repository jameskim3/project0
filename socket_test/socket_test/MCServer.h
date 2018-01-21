#pragma once
#include <afxsock.h>
// CMCServer 명령 대상입니다.

class CMCServer : public CAsyncSocket
{
private:
	//CMachine_Interface* pMcInterface;
public:
	CMCServer();
	virtual ~CMCServer();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnConnect(int nErrorCode);
};


