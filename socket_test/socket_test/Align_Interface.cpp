#include "stdafx.h"
#include "Align_Interface.h"

CAlign_Interface::CAlign_Interface(int nType)
{
	//return;
	m_pListenSocket = NULL;
	m_pClientSocket = NULL;
	nServer = FALSE;
	nConnectFlag = FALSE;
	m_cWnd = NULL;
	
	//서버일때만..
	if (nType == 1)
	{
		nServer = TRUE;
		m_pListenSocket = new CMCServer();
		if (m_pListenSocket->Create(MC_PORT))
		{			
			m_pListenSocket->Listen();
		}
	}
}


CAlign_Interface::~CAlign_Interface()
{
	if (m_pClientSocket){
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}

void CAlign_Interface::OnServerAccept()
{
	m_pClientSocket = new CMCServer();

	if (!m_pListenSocket->Accept(*m_pClientSocket))
	{
		//연결실패
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		return;
	}
	else
	{
		//연결성공
		nConnectFlag = TRUE;
		CString strAddress;
		UINT uiPort;
		m_pClientSocket->GetPeerName(strAddress, uiPort); // 상대방의 IP 주소와 포트 번호 얻음
		CString strInfo;
		strInfo.Format(_T("Accept : %s %d"), strAddress, uiPort);

		if (m_cWnd != NULL)
			m_cWnd->SendMessage(WM_MC_TCPIP_MSG, (WPARAM)0, LPARAM((LPCTSTR)strInfo));
	}
}

int CAlign_Interface::OnServerClose()
{
	if (m_pClientSocket)
	{
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
	nConnectFlag = FALSE;

	if (m_cWnd != NULL)
		m_cWnd->SendMessage(WM_MC_TCPIP_MSG, (WPARAM)0, LPARAM((LPCTSTR)_T("Disconnected")));

	return 0;
}

int CAlign_Interface::OnServerReceive()
{
	char *str = new char[MAX_PACKET_SIZE];
	CString tempStr; 

	int nSize = m_pClientSocket->Receive(str, strlen(str));// Receive(&nData, sizeof(nData));
	if (nSize >= MAX_PACKET_SIZE)
		nSize = MAX_PACKET_SIZE-1;
	str[nSize] = '\0';

	tempStr = str;
	strRecvData = tempStr;
	TRACE(_T("RECV : %s\n"), tempStr);

	CString strInfo = _T("Receive : ") + tempStr;
	if (m_cWnd != NULL)
		m_cWnd->SendMessage(WM_MC_TCPIP_MSG, (WPARAM)0, LPARAM((LPCTSTR)strInfo));

	//////////////////////////////////////////////////////////////////////////
	//theApp.MsgSetList(strRecvData);

	//데이터 처리를 하자
	if (nServer == TRUE)
		DataParsingServer();
	else
		DataParsingClient();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//임시로 받은 메세지를 반송하고자..
	if (nServer == TRUE)
		m_pClientSocket->Send(str, strlen(str));
	//////////////////////////////////////////////////////////////////////////

	return 0;
}


int CAlign_Interface::OnConnect()
{
	BOOL bRet = FALSE;

	//이미 연결되어 있다면...
	if (nConnectFlag == TRUE)
		return TRUE;

	m_pClientSocket = new CMCServer();

	if (m_pClientSocket->Create())
	{
		nConnectFlag = 2;	//시도중...
		//m_pClientSocket->Connect(strIP, nPort);
		m_pClientSocket->Connect(MC_IP, MC_PORT);
		//nConnectFlag = TRUE;

		//if (m_cWnd != NULL)
		//	m_cWnd->SendMessage(WM_MC_TCPIP_MSG, (WPARAM)0, LPARAM(MC_MSG_CONNECT));
	}
	return bRet;
}

int CAlign_Interface::OnSend(CString strData)
{
	int nRet = FALSE;
	int nLen = 0, nSend = 0;

	nLen = strData.GetLength();
	//연결되어 있어야 전송
	if (nConnectFlag)
	{
		nSend = m_pClientSocket->Send(strData, nLen);
		
		strSendData = strData;

		CString strInfo = _T("Send : ") + strSendData;
		if (m_cWnd != NULL)
			m_cWnd->SendMessage(WM_MC_TCPIP_MSG, (WPARAM)0, LPARAM((LPCTSTR)strInfo));
	}

	if (nLen == nSend)
		nRet = TRUE;

	return nRet;
}

int CAlign_Interface::IsConnect()
{
	return nConnectFlag;
}

void CAlign_Interface::SetHwnd(CWnd *cwnd)
{
	m_cWnd = cwnd;
}

//> FIXME_ cyh 110114 : 현재와 같은 타임아웃 구조는 49.7일이 지나 GetTickCount가 리셋되는 시점에서 문제를 발생 시킬 수 있음.
void CAlign_Interface::SetTimeOut(double milliseconds, int nTimerID)
{
	m_nTimeValue[nTimerID] = GetTickCount() + int(milliseconds * 1000);
}

BOOL CAlign_Interface::IsTimeOut(int nTimerID)
{
	//	if (IsDryMode) return FALSE;

	return (GetTickCount() > (DWORD)m_nTimeValue[nTimerID] ? TRUE : FALSE);
}

//msg 보내고 시간이 경과되었는지 확인하는 함수
int CAlign_Interface::IsReplyMsgTimeOut()
{
// 	if (getcnt(&counter) <= 0)
// 		return(FAIL);
// 
// 	return(GOOD);

	if(IsTimeOut(ID_MC_INTERFACE))
		return TRUE;

	return FALSE;
}

void CAlign_Interface::SetMachineStatus(int nVal)
{
	nMachineStatus = nVal;
	nMachineCommand = nVal;
}

int CAlign_Interface::IsMachineStatus(int nVal)
{
	if (nMachineStatus == nVal)
		return TRUE;

	//연결이 해제되었으면...
	if (IsConnect() == FALSE)
	{
		return(FALSE);
	}
	//대기 시간을 경과했으면
	if (IsReplyMsgTimeOut() == TRUE)
	{		
		return FALSE;
	}

	if (nMachineStatus == nVal)
		return TRUE;
	else if (nMachineStatus == MC_STATUS_NONE)
		return 2;
	else if (nMachineStatus == MC_STATUS_ERROR)
		return FALSE;

	return FALSE;
}

int CAlign_Interface::SendCommand(int nCmd, long lTimeOut)
{
	CString strMsg;
	int nRet = FALSE;

	switch (nCmd)
	{
	//case CMD_READY_STATUS:
	//	strMsg.Format("REDY:IS");
	//	break;
	//case CMD_READY_OK:
	//	strMsg.Format("REDY:OK");
	//	break;
	//case CMD_READY_NG:
	//	strMsg.Format("REDY:NG");
	//	break;
	//case CMD_LOAD_SCRIPT:
	//	strMsg.Format("LOAD:%s", strScriptName);
	//	break;
	//case CMD_LOAD_OK:
	//	strMsg.Format("LOAD:OK");
	//	break;
	//case CMD_LOAD_NG:
	//	strMsg.Format("LOAD:NG");
	//	break;
	//case CMD_WORK_START:
	//	strMsg.Format("WORK:%s", strJobName);
	//	break;
	//case CMD_WORK_OK:
	//	strMsg.Format("WORK:OK");
	//	break;
	//case CMD_WORK_NG:
	//	strMsg.Format("WORK:NG");
	//	break;
	//case CMD_PI_START:
	//	strMsg.Format("MOVE:%s", strNewportCmd);
	//	break;
	//case CMD_PI_OK:
	//	strMsg.Format("MOVE:OK");
	//	break;
	//case CMD_PI_NG:
	//	strMsg.Format("MOVE:NG");
	//	break;
	default:
		return FALSE;
		break;
	}

	//sec단위로 입력하면 자동으로 millisec으로 변경
	SetTimeOut(lTimeOut, ID_MC_INTERFACE);

	nRet = OnSend(strMsg);

	return nRet;
}

int CAlign_Interface::DataParsingServer()
{
	CString strTemp = strRecvData;
	CString strCmd, strRet;
	int nLen = 0;
	int nParsingRet = MC_STATUS_ERROR;

	nLen = strTemp.GetLength();
	if (nLen >= 7)	//최소 명령어가 7자리 이상이니까
	{
		strCmd = strTemp.Left(4);
		strRet = strTemp.Mid(5, 2);

		if (strCmd == "REDY")
		{
			if (strRet == "OK")
				nParsingRet = MC_STATUS_READY_OK;
			else
				nParsingRet = MC_STATUS_READY_NG;
		}
		else if (strCmd == "LOAD")
		{
			if (strRet == "OK")
				nParsingRet = MC_STATUS_LOAD_OK;
			else
				nParsingRet = MC_STATUS_LOAD_NG;
		}
		else if (strCmd == "WORK")
		{
			if (strRet == "OK")
			{
				nParsingRet = MC_STATUS_WORK_OK;
				//theApp.m_bSubScanBusy = FALSE;
			}
			else
			{
				nParsingRet = MC_STATUS_WORK_NG;
				//theApp.m_bSubScanBusy = FALSE;
			}
		}
		else if (strCmd == "MOVE")
		{
			if (strRet == "OK")
			{
				nParsingRet = MC_STATUS_PI_OK;
				//theApp.m_nSubNewportBusy = FALSE;
			}
			else
			{
				nParsingRet = MC_STATUS_PI_NG;
				//theApp.m_nSubNewportBusy = FALSE;
			}
		}
	}

	SetMachineStatus(nParsingRet);

	return FALSE;
}

int CAlign_Interface::DataParsingClient()
{
	CString strTemp = strRecvData;
	CString strCmd, strRet;
	int nLen = 0;
	int nParsingRet = MC_STATUS_ERROR;

	nLen = strTemp.GetLength();
	if (nLen >= 7)	//최소 명령어가 7자리 이상이니까
	{
		strCmd = strTemp.Left(4);
		strRet = strTemp.Mid(5, 2);

		if (strCmd == "LOAD")
		{
			strScriptName = strTemp.Mid(5);			
			nParsingRet = MC_STATUS_LOAD;
		}
		else if (strCmd == "WORK")
		{
			strJobName = strTemp.Mid(5);	
			nParsingRet = MC_STATUS_WORK;
		}
		else if (strCmd == "MOVE")
		{
			strNewportCmd = strTemp.Mid(5);	
			nParsingRet = MC_STATUS_PI;
		}
	}

	SetMachineStatus(nParsingRet);

	return FALSE;
}