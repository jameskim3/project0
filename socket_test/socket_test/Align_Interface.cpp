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
	
	//�����϶���..
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
		//�������
		delete m_pClientSocket;
		m_pClientSocket = NULL;
		return;
	}
	else
	{
		//���Ἲ��
		nConnectFlag = TRUE;
		CString strAddress;
		UINT uiPort;
		m_pClientSocket->GetPeerName(strAddress, uiPort); // ������ IP �ּҿ� ��Ʈ ��ȣ ����
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

	//������ ó���� ����
	if (nServer == TRUE)
		DataParsingServer();
	else
		DataParsingClient();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�ӽ÷� ���� �޼����� �ݼ��ϰ���..
	if (nServer == TRUE)
		m_pClientSocket->Send(str, strlen(str));
	//////////////////////////////////////////////////////////////////////////

	return 0;
}


int CAlign_Interface::OnConnect()
{
	BOOL bRet = FALSE;

	//�̹� ����Ǿ� �ִٸ�...
	if (nConnectFlag == TRUE)
		return TRUE;

	m_pClientSocket = new CMCServer();

	if (m_pClientSocket->Create())
	{
		nConnectFlag = 2;	//�õ���...
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
	//����Ǿ� �־�� ����
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

//> FIXME_ cyh 110114 : ����� ���� Ÿ�Ӿƿ� ������ 49.7���� ���� GetTickCount�� ���µǴ� �������� ������ �߻� ��ų �� ����.
void CAlign_Interface::SetTimeOut(double milliseconds, int nTimerID)
{
	m_nTimeValue[nTimerID] = GetTickCount() + int(milliseconds * 1000);
}

BOOL CAlign_Interface::IsTimeOut(int nTimerID)
{
	//	if (IsDryMode) return FALSE;

	return (GetTickCount() > (DWORD)m_nTimeValue[nTimerID] ? TRUE : FALSE);
}

//msg ������ �ð��� ����Ǿ����� Ȯ���ϴ� �Լ�
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

	//������ �����Ǿ�����...
	if (IsConnect() == FALSE)
	{
		return(FALSE);
	}
	//��� �ð��� ���������
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

	//sec������ �Է��ϸ� �ڵ����� millisec���� ����
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
	if (nLen >= 7)	//�ּ� ��ɾ 7�ڸ� �̻��̴ϱ�
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
	if (nLen >= 7)	//�ּ� ��ɾ 7�ڸ� �̻��̴ϱ�
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