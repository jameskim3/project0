#pragma once

#include "MCServer.h"

#define MAX_PACKET_SIZE					4096

#define MC_PORT							9997
#define MC_IP							_T("127.0.0.1")

#define MC_STATUS_ERROR					0
#define MC_STATUS_NONE					1
#define MC_STATUS_READY					2
#define MC_STATUS_READY_OK				3
#define MC_STATUS_READY_NG				4
#define MC_STATUS_LOAD					5
#define MC_STATUS_LOAD_OK				6
#define MC_STATUS_LOAD_NG				7
#define MC_STATUS_WORK					8
#define MC_STATUS_WORK_OK				9
#define MC_STATUS_WORK_NG				10
#define MC_STATUS_PI					11
#define MC_STATUS_PI_OK					12
#define MC_STATUS_PI_NG					13

#define MC_MSG_CONNECT					0	//�����
#define MC_MSG_DISCONNECT				1	//������
#define MC_MSG_RECV						2	//������ ����
#define MC_MSG_SEND						3	//������ ����

//�ְ���� ����� ������
#define CMD_READY_STATUS				0	//�غ� �������� Ȯ�ο�û
#define CMD_READY_OK					1	//�غ� �������� �˸�
#define CMD_READY_NG					2	//�غ� ���°� �ƴ��� �˸�
#define CMD_LOAD_SCRIPT					3	//script�� ������� ���
#define CMD_LOAD_OK						4	//script�� �дµ� ���� �ߴٰ� �˸�
#define CMD_LOAD_NG						5	//script�� �дµ� ���� �ߴٰ� �˸�
#define CMD_WORK_START					6	//�������϶�� ���
#define CMD_WORK_OK						7	//������ �Ϸ� �ߴٰ� �˸�
#define CMD_WORK_NG						8	//������ ���� �ߴٰ� �˸�
#define CMD_PI_START					9	//�̵����϶�� ���
#define CMD_PI_OK						10	//�̵��� �Ϸ� �ߴٰ� �˸�
#define CMD_PI_NG						11	//�̵��� ���� �ߴٰ� �˸�


#define ID_MC_INTERFACE					1	//Ÿ�̸� ����� index

#define WM_MC_TCPIP_MSG					WM_USER + 7980

class CAlign_Interface
{
public:
	CAlign_Interface(int nType);
	~CAlign_Interface();

	CMCServer* m_pListenSocket;
	CMCServer* m_pClientSocket;

	//CString sClientIP;
	//int nClientPort;
	CString strRecvData;	//���� ���� �����͸� ����
	CString strSendData;	//������ �����͸� ����
	CString strScriptName;	//Load�ؾ��� script���ϸ�
	CString strJobName;		//�۾�(����)�ؾ��� JOB�̸�
	CString strNewportCmd;	//���а踦 ���۽�ų ��� (���� �̵���ġ)

	CWnd *m_cWnd;			//Ư�� �̺�Ʈ �߻��� �ش� ������ ���ϴ� ȭ������ �˸��� ����


	int nServer;			//�������� Ŭ���̾�Ʈ����
	int nConnectFlag;		//���Ῡ�� Ȯ��

	int nMachineStatus;		//�޼��� ���� ��� �Ǵ� ���� ����
	int nMachineCommand;	//main���� sub ������ ���

	long	m_nTimeValue[10];

	void OnServerAccept();
	int OnServerReceive();
	int OnServerClose();

	int OnConnect();
	int OnSend(CString strData);
	int IsConnect();


	void SetHwnd(CWnd *cwnd);

	BOOL IsTimeOut(int nTimerID = 0);
	void SetTimeOut(double milliseconds, int nTimerID = 0);

	void SetMachineStatus(int nVal);				//������ ���¸� Ȯ���ϱ� ����
	int IsMachineStatus(int nVal);					//�ش� ���°� �´��� Ȯ���ϴ� �Լ�
	int IsReplyMsgTimeOut();


	int SendCommand(int nCmd, long lTimeOut=1000);
	int DataParsingServer();
	int DataParsingClient();
};

extern CAlign_Interface* pAlignInterface;