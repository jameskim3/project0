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

#define MC_MSG_CONNECT					0	//연결됨
#define MC_MSG_DISCONNECT				1	//해제됨
#define MC_MSG_RECV						2	//데이터 받음
#define MC_MSG_SEND						3	//데이터 전송

//주고받을 명령을 정의함
#define CMD_READY_STATUS				0	//준비 상태인지 확인요청
#define CMD_READY_OK					1	//준비 상태임을 알림
#define CMD_READY_NG					2	//준비 상태가 아님을 알림
#define CMD_LOAD_SCRIPT					3	//script를 읽으라고 명령
#define CMD_LOAD_OK						4	//script를 읽는데 성공 했다고 알림
#define CMD_LOAD_NG						5	//script를 읽는데 실패 했다고 알림
#define CMD_WORK_START					6	//가공을하라고 명령
#define CMD_WORK_OK						7	//가공을 완료 했다고 알림
#define CMD_WORK_NG						8	//가공을 실패 했다고 알림
#define CMD_PI_START					9	//이동을하라고 명령
#define CMD_PI_OK						10	//이동을 완료 했다고 알림
#define CMD_PI_NG						11	//이동을 실패 했다고 알림


#define ID_MC_INTERFACE					1	//타이머 사용할 index

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
	CString strRecvData;	//전송 받은 데이터를 저장
	CString strSendData;	//전송한 데이터를 저장
	CString strScriptName;	//Load해야할 script파일명
	CString strJobName;		//작업(가공)해야할 JOB이름
	CString strNewportCmd;	//광학계를 동작시킬 명령 (모터 이동위치)

	CWnd *m_cWnd;			//특정 이벤트 발생시 해당 내용을 원하는 화면으로 알리기 위함


	int nServer;			//서버인지 클라이언트인지
	int nConnectFlag;		//연결여부 확인

	int nMachineStatus;		//메세지 보낸 결과 또는 설비 상태
	int nMachineCommand;	//main에서 sub 전달한 명령

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

	void SetMachineStatus(int nVal);				//설비의 상태를 확인하기 위함
	int IsMachineStatus(int nVal);					//해당 상태가 맞는지 확인하는 함수
	int IsReplyMsgTimeOut();


	int SendCommand(int nCmd, long lTimeOut=1000);
	int DataParsingServer();
	int DataParsingClient();
};

extern CAlign_Interface* pAlignInterface;