// MCServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MCServer.h"

#include "Align_Interface.h"

// CMCServer

CMCServer::CMCServer()
{
	WSADATA    wsaData;
	int nRet = WSAStartup(MAKEWORD(2, 0), &wsaData);
}

CMCServer::~CMCServer()
{
}


// CMCServer 멤버 함수


void CMCServer::OnAccept(int nErrorCode)
{
	pAlignInterface->OnServerAccept();


	CAsyncSocket::OnAccept(nErrorCode);
}


void CMCServer::OnClose(int nErrorCode)
{
	pAlignInterface->OnServerClose();

	CAsyncSocket::OnClose(nErrorCode);
}


void CMCServer::OnReceive(int nErrorCode)
{
	pAlignInterface->OnServerReceive();

	CAsyncSocket::OnReceive(nErrorCode);
}


int CMCServer::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}

void CMCServer::OnConnect(int nErrorCode)
{
	if(nErrorCode == 0)
	{
		//연결성공
		pAlignInterface->nConnectFlag = TRUE;
	}
	else
	{
		//연결 실패
		pAlignInterface->OnConnect();
		pAlignInterface->nConnectFlag = FALSE;
	}

	CAsyncSocket::OnConnect(nErrorCode);
}
