#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "playmain.h"
#include "fileread.h"

#include "particle.h"
#include "sinbaram\\sinlinkheader.h"
#include "record.h"
#include "netplay.h"
#include "playsub.h"
#include "field.h"
#include "cracker.h"
#include "language.h"
#include "srcLang\\jts.h"
#include "TextMessage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\help.h"
#include "damage.h"
#include "skillsub.h"

#include "AreaServer.h"

#include "BellatraFontEffect.h"
#include "srcserver\\onserver.h"

//######################################################################################
//�� �� �� : �� �� ��
#include "WinInt\\WinIntThread.h"
#include "WinInt\\WavIntHttp.h"
#include "WinInt\\ZipLib.h"
//######################################################################################

#include "CurseFilter.h"		//IsCurse �Լ�(�ؿ�)

#ifdef _XTRAP_GUARD_4_
#include ".\\XTrapSrcD5\\Client\\XTrap4Client.h"	//XTrapD5
//#include ".\\XTrapSrc4\\XTrap.h"
#endif

//����� - XignCode
#ifdef _XIGNCODE_CLIENT
#include "Xigncode\Client\zwave_sdk_client.h"
#endif

extern int WinSizeX;
extern int WinSizeY;
extern HWND hwnd;

//��ų��� �Լ���!!
//���� ��ų üũ
//int cSKILL::CancelContinueSkill(DWORD CODE)


//������ ����� (������ ĳ���� ���� �������� )
#define SERVER_CHAR_INFO

//�ڵ� ���� �ð� ( 5�� ���� )
#define RECORD_TIME_STEP		(5*60*1000)
//���� �ε� ��ٸ��� �ð� ( 10�� )
#define NET_LOADING_WAIT_LIMIT		10000

//ũ��Ƽ�� ���� ����
extern CRITICAL_SECTION	cSection;
extern rsRECORD_DBASE	rsRecorder;					//������ ���ӵ���Ÿ �����ġ
DWORD	dwLastRecordTime = 0;

CRITICAL_SECTION	cServSection;

int Debug_SendCount = 0;
int Debug_RecvCount1 = 0;
int Debug_RecvCount2 = 0;
int Debug_RecvCount3 = 0;

int NetWorkInitFlag = 0;
int	BellatraEffectInitFlag = 0;			//����Ʈ�� ����Ʈ �ʱ�ȭ �÷�
SBL_SetFontEffect SoD_SetFontEffect;	//SoD �� ��Ʈ ����Ʈ


DWORD	dwTime_ServerT = 0;		//���ӽ� �ð� (������ Time_T )
DWORD	dwTime_ConnectMS = 0;	//���ӽ� �ð� (Ŭ���̾�Ʈms)


//#define	CLIENT_VERSION_NUM		2235

//�߱�
#ifdef	_LANGUAGE_CHINESE
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		1031
#else
#define	CLIENT_VERSION_NUM		15031
#endif
#endif

//�Ϻ�
#ifdef	_LANGUAGE_JAPANESE

#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		4100
#else
#define	CLIENT_VERSION_NUM		15100
#endif
#endif

//�븸
#ifdef _LANGUAGE_TAIWAN
#define	CLIENT_VERSION_NUM		3003
#endif

#ifdef _LANGUAGE_ENGLISH
//#ifdef _LANGUAGE_PHILIPIN
//	#ifndef	_WINMODE_DEBUG
//	#define	CLIENT_VERSION_NUM		4060		//�ʸ���
//	#else
//	#define	CLIENT_VERSION_NUM		15060		//�ʸ���GM
//	#endif
//#else
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		3122		//����
#else
#define	CLIENT_VERSION_NUM		15122		//����GM
#endif
//#endif

#endif

//��Ʈ��
#ifdef _LANGUAGE_VEITNAM
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		10565
#else
#define	CLIENT_VERSION_NUM		15565
#endif
#endif

//�±�
#ifdef _LANGUAGE_THAI
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		3223
#else
#define	CLIENT_VERSION_NUM		15223
#endif
#endif

//�����
#ifdef _LANGUAGE_BRAZIL
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		4135
#else
#define	CLIENT_VERSION_NUM		15135
#endif
#endif

//�Ƹ���Ƽ��
#ifdef _LANGUAGE_ARGENTINA
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		4005
#else
#define	CLIENT_VERSION_NUM		15005
#endif
#endif

#ifdef _LANGUAGE_KOREAN
#define	CLIENT_VERSION_NUM		3000
#endif

#define CLIENT_VERSION_CHECK	(-CLIENT_VERSION_NUM*2)
/////////////////////////// ���� ���� ///////////////////////////////////////////
int	Client_Version = CLIENT_VERSION_NUM;		//���� ���� Ŭ���̾�Ʈ Ȯ�ο�
int	Server_LimitVersion = 167;					//���� ���� ���� Ȯ�ο�
/////////////////////////////////////////////////////////////////////////////////

int	Version_WareHouse = 1;						//â�� ���嵥��Ÿ ����
int	Version_CharInfo = 1;						//ĳ������Ÿ ���嵥��Ÿ ����


smWINSOCK	*smWsockServer = 0;					//NPC-����-������
smWINSOCK	*smWsockDataServer = 0;				//����Ÿ ���� ����
smWINSOCK	*smWsockUserServer = 0;				//���� �÷��� ����Ÿ ���� ����
smWINSOCK	*smWsockExtendServer = 0;			//�ʵ� Ȯ�� ����

//���� �翬�� Ƚ��
int	ReconnDataServer = 0;
int	ReconnServer = 0;

char	szConnServerName[16];					//������ �����̸�

DWORD	dwRecvUserServerTime = 0;
DWORD	dwRecvDataServerTime = 0;
DWORD	dwRecvServerTime = 0;
DWORD	dwRecvExtendServerTime = 0;

//�ְ� �ޱ� ����ȭ ����
int	rsRecvDataServer = TRUE;
int	rsRecvServer = TRUE;
int	rsRecvUserServer = TRUE;
int	rsRecvExtendServer = TRUE;

#define CHATBUFF_MAX		64
#define CHATBUFF_MASK		63

DWORD dwMyIP;

CHATBUFF ChatBuff[CHATBUFF_MAX];
CHATBUFF ChatBuff2[6][CHATBUFF_MAX];


int	ChatBuffCnt = 0;
int	ChatBuffCnt2[6] = { 0,0,0,0,0,0 };
int ChatDispCnt = 0;
int	ChatBuffCompCnt = 0;

char *szDefaultServIP = "211.50.44.170";

char *szNetLogFile = "debugnet.log";
FILE *fpNetLog = 0;

//�������� ���� ������
int	DisconnectFlag = 0;
int	DisconnectServerCode = 0;

//��ŷ ���
int	WarningHack = 0;

DWORD	dwQuestDelayTime = 0;		//����Ʈâ ������ Ÿ��

DWORD	dwLoadingTime = 0;			//�ε� �õ� �ð�

char szServIP[32];
DWORD dwServPort;
char szDataServIP[32];
DWORD dwDataServPort;
char szUserServIP[32];
DWORD dwUserServPort;
char szExtendServIP[32];
DWORD dwExtendServPort;



DWORD	dwConnectedClientTime = 0;			//������ ����� Ŭ���̾�Ʈ �ð�
DWORD	dwConnectedServerTime = 0;			//������ ����� ���� �ð�
DWORD	dwLastRecvGameServerTime = 0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
DWORD	dwLastRecvGameServerTime2 = 0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
DWORD	dwLastRecvGameServerTime3 = 0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
DWORD	dwLastRecvGameServerTime4 = 0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�

typedef DWORD(*LPFN_CheckMem)(TRANS_FUNC_MEMORY *TransFuncMem, smTRANS_COMMAND	*lpTransCommand);
LPFN_CheckMem	fnChkMem2;

//����ġ�� �� �ŷ� �ݾ��� ��Ż�� ����Ѵ�
static int	CompWareHouseMoney = 0;
int	WareHouseSubMoney = 0;					//â���� ���� ���� �ݾ�
int TotalSubMoney = 0;						//���� ���� �ݾ�
int TotalAddMoney = 0;						//���� ���� �ݾ�
int	TotalAddExp = 0;						//���� ����ġ
int	TotalSubExp = 0;						//�پ�� ����ġ


smTRANS_COMMAND	*lpTransVirtualPotion = 0;


LPFN_CheckMem	fnChkMem;
DWORD	dwMemFunChkCode = 0;
//�޸� �˻� ��� ����
int	RecvMemFuncData(TRANS_FUNC_MEMORY *TransFuncMem);
int	RecvMemFuncData2(TRANS_FUNC_MEMORY *TransFuncMem);
//�������� �޸� ���� ���ؼ� ��������
DWORD funcCheckMemSum(DWORD FuncPoint, int count);
//���˻� �Ͽ� ������ ������
int CheckProcessModule();


//char *szServIP =	"211.50.44.170";
smPLAYDATA	PlayData;
char	TransBuff[smSOCKBUFF_SIZE];
int		TransLen;
//smCHAR_INFO	CharInfo;

//���� ������ ����
TRANS_ITEMINFO	TransThrowItem;
//�ֱ� ���� ������
TRANS_ITEMINFO	TransRecvItem;

//������ ��ȯŰ ��õ���
TRANS_TRADE_ITEMKEY	LastTransTradeItemKey;
DWORD				dwLastTransTradeKeyTime = 0;

int					InitClanMode = 0;			//Ŭ�� �ʱ�ȭ ���

DWORD				dwYahooTime = 0;			//��ȣ �����ð�

int					ServerHideMode = 0;			//���� ��������� ���


//////////////// PK �ʵ� ���� //////////////////
PK_FIELD_STATE	PK_FieldState;		//Pk �ʵ� ���� ����ü


//Ʈ���̵� ������ Ȯ�� ����ü
struct TRANS_TRADE_CHECKITEM
{
	int size, code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;

	DWORD	dwSendItemCode[MAX_TRADE_ITEM];
	DWORD	dwSendItemSum[MAX_TRADE_ITEM];

	DWORD	dwRecvItemCode[MAX_TRADE_ITEM];
	DWORD	dwRecvItemSum[MAX_TRADE_ITEM];
};


////////////////// ������ ��ȯ ���� ///////////////
int		TradeItemSucessFlag = 0;
int		TradeRecvItemKeyFlag = 0;
int		TradeSendSucessFlag = 0;
DWORD	dwTradeMaskTime = 0;
int		TradeItemCancelCount = 0;

TRANS_TRADEITEMS	TransLastRecvTrade;
/*
//���λ��� ����
struct	TRANS_MYSHOP_ITEM {
	int		size,code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;
	DWORD	Temp[4];

	sMYSHOP	sMyShop;
};
*/
TRANS_TRADEITEMS	TransTradeItems_MyShop;			//���λ��� ���� ����

/////////////////// â�� ///////////////////////////
DWORD	dwLastWareHouseChkSum = 0;

////////////// ����¡ ������ �����ۼ��� /////////////
int		TransAgingItemFlag;

smTRANS_COMMAND	TransServerConnectInfo;

//�ٸ� ������ ���� ��Ų��
int ConnectOtherPlayer(DWORD dwIP);

int HoRecvMessage(DWORD dwCode, void *RecvBuff);

//������ ��ȯ ����Ű ����
int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY *lpTransTradeItemKey, sTRADE *lpTrade);
//������ ��ȯ ����Ű ����
int SendTradeSucessKey(sTRADE *lpTrade, DWORD dwSender);
//������ ��ȯ ���� Ȯ�� ����
int RecvTradeCheckItem(TRANS_TRADE_CHECKITEM *lpTransTradeCheckItem);
//ũ�� ������ �Ű�
int SendCrackWindow(HWND hWnd);

//�ӽ� ����� ���� �������� Ȯ���Ͽ� ��ġ�ϸ� ������ ����
int ThrowItemToServer(smTRANS_COMMAND_EX *lpTransCommand);
//������ Ȯ�� ��� ����
int RecvCheckItemFromServer(TRANS_ITEM_CODE *lpTransItemCode);
//Ŭ���̾�Ʈ ��� ��ġ�� ���� ������ ����
int	SendClientFuncPos();

//���μ��� �ð� �����ϴ� ������ ����
int OpenTimeCheckThread();
//���μ��� �ð� �������� ������ �ۺ�
int SendPlayTimerMax();


//�ӽ������ ������ ť���� ã��
TRANS_ITEMINFO	*FindRecvTransItemQue(DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
//������ ť�� �ӽ� ����
int	PushRecvTransItemQue(TRANS_ITEMINFO *lpTransItemInfo);

//��ų ���� ��Ŷ ����
int RecvProcessSkill(smTRANS_COMMAND *lpTransCommand);
//��Ƽ ��ų ��Ŷ ����
int RecvPartySkillFromServer(TRANS_PARTY_SKILL *lpTransPartySkill);

//����Ʈ ������ �Լ� ���͸�
int FiltQuestItem(TRANS_ITEMINFO	*lpTransItemInfo, DWORD dwPacketCode);
//��ũ �ھ� ��� ����
int SucessLinkCore(smTRANS_COMMAND_EX *lpTransCommandEx);
//SoD ���� ���� ����
int RecvSodGameInfomation(void *Info);

//�������� ������ �Լ�
int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2);

//����Ʈ�� ��Ŷ����
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock);

//���� ���� ĳ�� ����
rsBLESS_CASTLE	rsBlessCastleOld;

//�Ӽ����� �۽� �ð�
DWORD	dwResistance_SendingTime = 0;


//������ ���� �����ڵ� ���
#define	ATT_MONSTER_MAX	128
#define	ATT_MONSTER_MASK	(ATT_MONSTER_MAX-1)

DWORD	dwAttMonsterCodeList[ATT_MONSTER_MAX];
int		AttMonsterCodeCount = 0;

//���� �ڵ� ť�� ����
int ClearAttMonsterCode()
{
	int cnt;
	for (cnt = 0; cnt < ATT_MONSTER_MAX; cnt++)
	{
		dwAttMonsterCodeList[cnt] = 0;
	}
	return TRUE;
}

int	AddAttMonsterCode(DWORD dwCode)
{
	int mcnt;
	if (dwCode)
	{
		mcnt = (AttMonsterCodeCount - 1)&ATT_MONSTER_MASK;
		if (dwAttMonsterCodeList[mcnt] == dwCode) return TRUE;
		mcnt = AttMonsterCodeCount&ATT_MONSTER_MASK;
		dwAttMonsterCodeList[mcnt] = dwCode;
		AttMonsterCodeCount++;
		return TRUE;
	}

	return FALSE;
}

int	CheckAttMonsterCode(DWORD dwCode)
{
	int cnt;
	for (cnt = 0; cnt < ATT_MONSTER_MAX; cnt++)
	{
		if (dwAttMonsterCodeList[cnt] == dwCode) return TRUE;
	}
	return FALSE;
}


///////////////////// ������ ���� ////////////////////

	//(smpacket.h / record.h / netplay.h)

	//smTRANSCODE_ID_GETUSERINFO: -> smTRANSCODE_ID_SETUSERINFO
	//ID�� ����� ĳ���� ����Ʈ �� ������ ����

	//smTRANSCODE_INSRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_DELRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_ISRECORDDATA: -> smTRANS_CHAR_COMMAND2
	//ĳ���Ͱ� ���� �ϴ��� Ȯ��

//���� ���� ���� ��� �ڸ��
//int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )

///////////////////////////////////////////////////////

///////////////////// �ޱ� ���� ////////////////////
	//smTRANSCODE_ID_SETUSERINFO:
	//ID�� ����� ĳ���� ����Ʈ �� ������ ���� 
	//(TRANS_USERCHAR_INFO ����ü�� ��� ���� )

	//smTRANSCODE_ISRECORDDATA:
	//ĳ���Ͱ� ���� �ϴ��� Ȯ�� 
	//( smTRANS_CHAR_COMMAND2 ->wParam �� ��� ���� )
///////////////////////////////////////////////////////

//���� ����


//smWINSOCK *ConnectServer_Main();

//���� ���� Ȯ��
int CheckServerInfo(smTRANS_COMMAND *lpTransCommand)
{
	if ((-Client_Version * 2) != CLIENT_VERSION_CHECK)
	{
		//������ ���� ���� ���� �Ұ�
		SetGameError(1);
		return TRUE;
	}

	if (Client_Version < lpTransCommand->WParam)
	{
		//������ ���� ���� ���� �Ұ�
		SetGameError(1);
		return TRUE;
	}
	if (lpTransCommand->LParam>0)
	{
		//�ο��� ���Ƽ� ���� �Ұ�
		SetGameError(2);
		return TRUE;
	}
	SetGameError(0);
	return TRUE;
}

//���μ��� ���� ����
int SendProcessInfo()
{
	TRANS_CHATMESSAGE	smTransChatMessage;

	if (smWsockDataServer)
	{
		smTransChatMessage.code = smTRANSCODE_PROCESSINFO;
		smTransChatMessage.dwIP = dwExeCheckSum;
		smTransChatMessage.dwObjectSerial = dwExeCheckSum;

		lstrcpy(smTransChatMessage.szMessage, szProcessPath);
		smTransChatMessage.size = 32 + lstrlen(szProcessPath);

		return smWsockDataServer->Send2((char *)&smTransChatMessage, smTransChatMessage.size, TRUE);
	}
	return FALSE;
}


//�� �÷��� ����
smWINSOCK *ConnectServer_Main()
{
	/*
		//�׽�Ʈ ����
		smConfig.szServerIP[0] = 0;
		smConfig.szDataServerIP[0] = 0;
		smConfig.szUserServerIP[0] = 0;
	*/

	if (!smWsockServer)
	{
		//���� ���� ����
		if (smConfig.szServerIP[0])
			lstrcpy(szServIP, smConfig.szServerIP);
		else
			lstrcpy(szServIP, szDefaultServIP);

		if (smConfig.dwServerPort)
			dwServPort = smConfig.dwServerPort;
		else
			dwServPort = TCP_SERVPORT;

		smWsockServer = smConnectSock(szServIP, (WORD)dwServPort);
	}

	if (smWsockServer && !smWsockDataServer)
	{
		//����Ÿ ���� ����
		if (smConfig.szDataServerIP[0] && lstrcmpi(smConfig.szDataServerIP, smConfig.szServerIP) != 0)
		{
			lstrcpy(szDataServIP, smConfig.szDataServerIP);
			dwDataServPort = smConfig.dwDataServerPort;
			smWsockDataServer = smConnectSock(szDataServIP, (WORD)dwDataServPort);
		}
		else
		{
			//����Ÿ ������ ���Ӽ����� ���� ���
			lstrcpy(szDataServIP, szServIP);
			dwDataServPort = dwServPort;
			smWsockDataServer = smWsockServer;
		}
	}

	if (smWsockServer && smWsockDataServer && !smWsockUserServer)
	{
		//���� ���� ����
		if (smConfig.szUserServerIP[0] && lstrcmpi(smConfig.szUserServerIP, smConfig.szServerIP) != 0)
		{
			lstrcpy(szUserServIP, smConfig.szUserServerIP);
			dwUserServPort = smConfig.dwUserServerPort;
			smWsockUserServer = smConnectSock(szUserServIP, (WORD)dwUserServPort);
		}
		else
		{
			//���� ������ ���Ӽ����� ���� ���
			lstrcpy(szUserServIP, szServIP);
			dwUserServPort = dwServPort;
			smWsockUserServer = smWsockServer;
		}
	}

	if (smWsockServer && smWsockDataServer && !smWsockExtendServer)
	{
		//Ȯ�� ���� ����
		if (smConfig.szExtendServerIP[0] && lstrcmpi(smConfig.szExtendServerIP, smConfig.szServerIP) != 0)
		{
			lstrcpy(szExtendServIP, smConfig.szExtendServerIP);
			dwExtendServPort = smConfig.dwExtendServerPort;
			smWsockExtendServer = smConnectSock(szExtendServIP, (WORD)dwExtendServPort);
		}
		else
		{
			//Ȯ�� ������ ���Ӽ����� ���� ���
			lstrcpy(szExtendServIP, szServIP);
			dwExtendServPort = dwServPort;
			smWsockExtendServer = smWsockServer;
		}
	}


	if (smWsockServer && smWsockDataServer && smWsockUserServer)
	{

		if (smConfig.DebugMode)
		{
			//fpNetLog = fopen( szNetLogFile , "wb" );
		}

		return smWsockServer;
	}

	return NULL;
}

//�������� ���� ��� ����
int DisconnectServerFull()
{

	if (smWsockDataServer && smWsockDataServer != smWsockServer)
	{
		smWsockDataServer->CloseSocket();
		smWsockDataServer = 0;
	}
	if (smWsockUserServer && smWsockUserServer != smWsockServer)
	{
		smWsockUserServer->CloseSocket();
		smWsockUserServer = 0;
	}
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
	{
		smWsockExtendServer->CloseSocket();
		smWsockExtendServer = 0;
	}
	if (smWsockServer)
	{
		smWsockServer->CloseSocket();
		smWsockServer = 0;
	}

	DisconnectFlag = 0;

	return TRUE;
}
/*
//���� ���� ����
smWINSOCK *ConnectServer_InfoMain();
//���� ���� ����
smWINSOCK *ConnectServer_GameMain( char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2 );
//���� ���� ���� ����
int DisconnectServer_GameMain();

*/

//���� ���� ����
smWINSOCK *ConnectServer_InfoMain()
{
	if (!smWsockDataServer)
	{
		//����Ÿ ���� ����
		if (smConfig.szDataServerIP[0])
		{
			dwDataServPort = TCP_SERVPORT;
			smWsockDataServer = smConnectSock(smConfig.szDataServerIP, (WORD)dwDataServPort);
			ZeroMemory(&TransServerConnectInfo, sizeof(smTRANS_COMMAND));
			return smWsockDataServer;
		}
	}


	return NULL;
}

//���� ���� ����
smWINSOCK *ConnectServer_GameMain(char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2)
{
	int ServerInfoFlag = 0;

	lstrcpy(smConfig.szServerIP, szIP1);
	lstrcpy(smConfig.szUserServerIP, szIP2);

	if (smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP1) != 0)
	{
		DisconnectServer_GameMain();
	}
	if (smWsockUserServer && lstrcmp(smWsockUserServer->szIPAddr, szIP2) != 0)
	{
		DisconnectServer_GameMain();
	}

	if (!smWsockServer)
	{
		//���� ���� ����
		if (smWsockDataServer &&
			lstrcmp(smWsockDataServer->szIPAddr, szIP1) == 0 && dwDataServPort == dwPort1)
		{

			smWsockServer = smWsockDataServer;
			dwServPort = dwDataServPort;

			ServerInfoFlag = TRUE;
		}
		else
		{
			smWsockServer = smConnectSock(szIP1, (WORD)dwPort1);
			dwServPort = dwPort1;
		}
		lstrcpy(smConfig.szServerIP, szIP1);
	}

	if (!smWsockUserServer)
	{
		if (smWsockServer &&
			lstrcmp(smWsockServer->szIPAddr, szIP2) == 0 && dwServPort == dwPort2)
		{

			smWsockUserServer = smWsockServer;
			dwUserServPort = dwServPort;
		}
		else
		{
			smWsockUserServer = smConnectSock(szIP2, (WORD)dwPort2);
			dwUserServPort = dwPort2;
		}
		lstrcpy(smConfig.szUserServerIP, szIP2);
	}


	if (smWsockServer && smWsockUserServer)
	{
		if (ServerInfoFlag)
		{
			//���� ���� Ȯ��
			CheckServerInfo(&TransServerConnectInfo);
		}

		return smWsockServer;
	}
	return NULL;
}

//���� ���� ����
smWINSOCK *ConnectServer_GameMain(char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2, char *szIP3, DWORD dwPort3)
{
	int ServerInfoFlag = 0;

	lstrcpy(smConfig.szServerIP, szIP1);
	lstrcpy(smConfig.szUserServerIP, szIP2);
	lstrcpy(smConfig.szExtendServerIP, szIP3);


	if (smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP1) != 0)
	{
		DisconnectServer_GameMain();
	}
	if (smWsockUserServer && lstrcmp(smWsockUserServer->szIPAddr, szIP2) != 0)
	{
		DisconnectServer_GameMain();
	}
	if (smWsockExtendServer && lstrcmp(smWsockExtendServer->szIPAddr, szIP3) != 0)
	{
		DisconnectServer_GameMain();
	}

	if (!smWsockServer)
	{
		//���� ���� ����
		if (smWsockDataServer &&
			lstrcmp(smWsockDataServer->szIPAddr, szIP1) == 0 && dwDataServPort == dwPort1)
		{

			smWsockServer = smWsockDataServer;
			dwServPort = dwDataServPort;

			ServerInfoFlag = TRUE;
		}
		else
		{
			smWsockServer = smConnectSock(szIP1, (WORD)dwPort1);
			dwServPort = dwPort1;
		}
		lstrcpy(smConfig.szServerIP, szIP1);
	}

	if (!smWsockUserServer)
	{
		if (smWsockServer &&
			lstrcmp(smWsockServer->szIPAddr, szIP2) == 0 && dwServPort == dwPort2)
		{

			smWsockUserServer = smWsockServer;
			dwUserServPort = dwServPort;
		}
		else
		{
			smWsockUserServer = smConnectSock(szIP2, (WORD)dwPort2);
			dwUserServPort = dwPort2;
		}
		lstrcpy(smConfig.szUserServerIP, szIP2);
	}

	if (!smWsockExtendServer)
	{
		if ((smWsockServer &&
			 lstrcmp(smWsockServer->szIPAddr, szIP3) == 0 && dwServPort == dwPort3) || !szIP3[0])
		{

			smWsockExtendServer = smWsockServer;
			dwExtendServPort = dwServPort;
		}
		else
		{
			smWsockExtendServer = smConnectSock(szIP3, (WORD)dwPort3);
			dwExtendServPort = dwPort3;
		}
		lstrcpy(smConfig.szExtendServerIP, szIP3);
	}

	if (smWsockServer && smWsockUserServer && smWsockExtendServer)
	{
		if (ServerInfoFlag)
		{
			//���� ���� Ȯ��
			CheckServerInfo(&TransServerConnectInfo);
		}

		return smWsockServer;
	}
	return NULL;
}


//���� ���� ���� ����
int DisconnectServer_GameMain()
{
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
	{
		if (smWsockDataServer != smWsockExtendServer)
		{
			smWsockExtendServer->CloseSocket();
		}
	}
	smWsockExtendServer = 0;

	if (smWsockUserServer && smWsockUserServer != smWsockServer)
	{
		if (smWsockDataServer != smWsockUserServer)
		{
			smWsockUserServer->CloseSocket();
		}
	}
	smWsockUserServer = 0;


	if (smWsockServer && smWsockDataServer != smWsockServer)
	{
		if (smWsockDataServer != smWsockServer)
		{
			smWsockServer->CloseSocket();
		}
	}
	smWsockServer = 0;

	DisconnectFlag = 0;

	return TRUE;
}

//���� �ڵ�� ������ ã��
smWINSOCK *GetServerSock(int ServerCode)
{
	switch (ServerCode)
	{
		case PLAY_SERVER_CODE_EXTEND:
			if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
				return smWsockExtendServer;
			else
				return smWsockUserServer;

		case PLAY_SERVER_CODE_USER:
			return smWsockUserServer;

		case PLAY_SERVER_CODE_MAIN:
			return smWsockServer;

		case PLAY_SERVER_CODE_AREA1:
			return lpWSockServer_DispArea[0];

		case PLAY_SERVER_CODE_AREA2:
			return lpWSockServer_DispArea[1];

	}

	return NULL;
}

//�������� ������ ã��
smWINSOCK *GetAreaServerSock()
{
	int ServerCode;

	if (lpCurPlayer->OnStageField >= 0)
	{
		if (AreaServerMode)
		{
			return lpWSockServer_DispArea[lpCurPlayer->OnStageField];
		}
		else
		{
			ServerCode = StageField[lpCurPlayer->OnStageField]->ServerCode;
			return GetServerSock(ServerCode);
		}
	}

	return NULL;
}


//������ ������ ã��
scITEM *FindScItem(int x, int z)
{
	int cnt;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++)
	{
		if (scItems[cnt].Flag &&
			scItems[cnt].pX == x && scItems[cnt].pZ == z)
		{
			return &scItems[cnt];
		}
	}

	return NULL;
}

//�� ������ ����ã��
scITEM *FindEmptyScItem()
{
	int cnt;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++)
	{
		if (!scItems[cnt].Flag) return &scItems[cnt];
	}

	return NULL;
}

//��ȯ �ŷ� �Ÿ� Ȯ��
int GetTradeDistance(smCHAR *lpChar)
{
	int	x, y, z;
	int dist;

	x = (lpCurPlayer->pX - lpChar->pX) >> FLOATNS;
	y = (lpCurPlayer->pY - lpChar->pY) >> FLOATNS;
	z = (lpCurPlayer->pZ - lpChar->pZ) >> FLOATNS;

	dist = x*x + y*y + z*z;

	if (dist < (256 * 256)) return TRUE;

	return FALSE;
}

//��ȯ �ŷ� �Ÿ� Ȯ��
int GetTradeDistanceFromCode(DWORD dwObjectSerial)
{
	smCHAR *lpChar;

	lpChar = FindAutoPlayer(dwObjectSerial);

	if (lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0])
	{

		return  GetTradeDistance(lpChar);

	}
	return FALSE;
}

//���θ��� ĳ������ Ȯ��
int	CheckStartCharInfo()
{
	//������ ���� �ѹ��� Ȯ��
	smCHAR_INFO	smCharInfo;
	int cnt;

	memcpy(&smCharInfo, sinChar, sizeof(smCHAR_INFO));

	CheckCharForm();
	CodeXorExp = dwPlayTime + smCharInfo.Exp;						//����ġ ���� �ڵ�
	CodeXorExp_High = dwPlayTime | (dwPlayTime*dwPlayTime);						//����ġ ���� �ڵ�
	CodeXorLife = (dwPlayTime*smCharInfo.Life[0]) & 0xFFFF;		//����� ���� �ڵ�
	cnt = smCharInfo.Dexterity + smCharInfo.Health + smCharInfo.Spirit + smCharInfo.Strength + smCharInfo.Talent;

	if (smCharInfo.Level >= 2 || cnt > 102 || smCharInfo.Exp || smCharInfo.ChangeJob || smCharInfo.Money)
	{
		//ĳ���� �����߻� ������Ȥ
		SendSetHackUser2(6000, smCharInfo.Money);
		WarningHack = TRUE;
		return FALSE;
	}

	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;

	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	ReformCharForm();

	return TRUE;
}

//�޸� �˻� ��� ����2
int	RecvMemFuncData2(TRANS_FUNC_MEMORY *TransFuncMem)
{
	smTRANS_COMMAND	smTransCommand;


	smTransCommand.size = -1;
	fnChkMem2 = (LPFN_CheckMem)((void *)TransFuncMem->szData);
	fnChkMem2(TransFuncMem, &smTransCommand);

	ZeroMemory(TransFuncMem, 1500);

	if (smTransCommand.size > 0 && smWsockDataServer)
	{
		smTransCommand.code = smTRANSCODE_FUNCTION_MEM2;
		smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
	}

	return TRUE;
}



sSERVER_MONEY	sServerMoney[3] = { {0,0,0,0,0},{0,0,0,0,0} };
sSERVER_EXP		sServerExp[3] = { {0,0,0,0},{0,0,0,0} };
DWORD			dwLastSendTotalExpMoenyTime = 0;

//���� ����ġ ������Ÿ ��ġ�� ������ ������ Ȯ��
int	SendTotalExpMoney()
{
	TRANS_TOTAL_EXPMONEY	TransTotalExpMoney;

	if ((dwLastSendTotalExpMoenyTime + 30000) > dwPlayTime) return FALSE;

	TransTotalExpMoney.code = smTRANSCODE_CHECK_EXPMONEY;
	TransTotalExpMoney.size = sizeof(TRANS_TOTAL_EXPMONEY);

	if (smWsockServer)
	{
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[0], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[0], sizeof(sSERVER_MONEY));
		smWsockServer->Send2((char *)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}
	if (smWsockUserServer && smWsockUserServer != smWsockServer)
	{
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[1], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[1], sizeof(sSERVER_MONEY));
		smWsockUserServer->Send2((char *)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
	{
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[2], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[2], sizeof(sSERVER_MONEY));
		smWsockExtendServer->Send2((char *)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}

	dwLastSendTotalExpMoenyTime = dwPlayTime;

	return TRUE;
}

//�������� ���� ���� �� Ȯ��
int CheckServerMoney(smWINSOCK *lpsmSock, TRANS_ITEMINFO	*lpTransItemInfo)
{
	sSERVER_MONEY	*lpServerMoney;

	lpServerMoney = 0;

	if (lpsmSock == smWsockServer)
	{
		lpServerMoney = &sServerMoney[0];
	}
	else
	{
		if (lpsmSock == smWsockUserServer)
		{
			lpServerMoney = &sServerMoney[1];
		}
		else if (lpsmSock == smWsockExtendServer)
		{
			lpServerMoney = &sServerMoney[2];
		}
	}

	if (lpServerMoney)
	{
		lpServerMoney->InputMoney += lpTransItemInfo->Item.Money;
		lpServerMoney->Counter++;
		lpServerMoney->TotalX = lpTransItemInfo->x;
		lpServerMoney->TotalY = lpTransItemInfo->y;
		lpServerMoney->TotalZ = lpTransItemInfo->z;

		/*
							TransItemInfo.x = dwPlayServTime;											//�ص� Ű
							TransItemInfo.y = TransItemInfo.Item.ItemHeader.dwChkSum;					//�ص� Ű2
							TransItemInfo.z = rsPlayInfo[cnt].spMoney_Out^(TransItemInfo.x+TransItemInfo.y);	//��Ż ��� ����
		*/

		int cnt;
		cnt = lpServerMoney->TotalZ ^ (lpServerMoney->TotalX + lpServerMoney->TotalY);
		cnt += 200;
		if (cnt < lpServerMoney->InputMoney)
		{
			//SendSetHackUser( 82 );
		}
	}

	return TRUE;
}


//�������� ���� ����ġ�� �� Ȯ��
int CheckServerExp(smWINSOCK *lpsmSock, smTRANS_COMMAND	*lpTransCommand)
{
	sSERVER_EXP	*lpServerExp;

	lpServerExp = 0;

	if (lpsmSock == smWsockServer)
	{
		lpServerExp = &sServerExp[0];
	}
	else
	{
		if (lpsmSock == smWsockUserServer)
		{
			lpServerExp = &sServerExp[1];
		}
		else if (lpsmSock == smWsockExtendServer)
		{
			lpServerExp = &sServerExp[2];
		}
	}

	if (lpServerExp)
	{
		lpServerExp->InputExp += lpTransCommand->WParam;
		lpServerExp->Counter++;
		lpServerExp->Total1 = lpTransCommand->SParam;
		lpServerExp->Total2 = lpTransCommand->EParam;

		int cnt;
		// �庰 - ����ġ ���� ���� (800000 - > 1600000 = ����ġ 2�� �̺�Ʈ)
		cnt = (lpServerExp->Total1^lpServerExp->Total2) + 1600000;		//������ ����
		if (cnt < lpServerExp->InputExp)
		{
			SendSetHackUser(83);
		}

	}

	return TRUE;
}



//�ٸ� �÷��̾���� ���� �����带 ����
int CreateConnectThread(DWORD dwIP);
//�ٸ� �÷��̾���� �������� ������ ����
int CreateDisconnectThread(smWINSOCK *lpsmSock);

//�ٸ� �ٸ� �÷��̾��� �� ������ ã��
int FindNewOtherPlayer()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (!chrOtherPlayer[cnt].Flag)
			return cnt;
	}

	return -1;
}

//�ڵ� �÷��̾ ã�´�
smCHAR *FindAutoPlayer(DWORD dwObjectSerial)
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial == dwObjectSerial)
		{
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

// ������ȣ�� ã�´�
smCHAR *FindChrPlayer(DWORD dwObjectSerial)
{
	int cnt;

	if (lpCurPlayer->dwObjectSerial == dwObjectSerial)
		return lpCurPlayer;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial == dwObjectSerial)
		{
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

//������Ƽ������ ã�´�
smCHAR *FindDeadPartyUser()
{
	int cnt;
	smCHAR *lpChar;
	int dist, x, y, z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300 * 300;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].PartyFlag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER)
		{
			if (chrOtherPlayer[cnt].MotionInfo->State == CHRMOTION_STATE_DEAD)
			{

				x = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpCurPlayer->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x*x + z*z + y*y;

				if (dist < NearDist)
				{
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}


//�ֱ��� ���͸� ã�´�
smCHAR *FindNearMonster(smCHAR *lpCurChar)
{
	int cnt;
	smCHAR *lpChar;
	int dist, x, y, z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300 * 300;

	if (!lpCurChar) return NULL;

	if (lpCurChar != lpCurPlayer)
	{
		x = (lpCurChar->pX - lpCurPlayer->pX) >> FLOATNS;
		y = (lpCurChar->pY - lpCurPlayer->pY) >> FLOATNS;
		z = (lpCurChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

		dist = x*x + z*z + y*y;
		if (dist < NearDist)
		{
			if (!lpCurChar->smCharInfo.ClassClan || lpCurChar->smCharInfo.ClassClan != lpCurPlayer->smCharInfo.ClassClan)
			{
				lpChar = lpCurPlayer;
				NearDist = dist;
			}
		}
	}

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY && lpCurChar != &chrOtherPlayer[cnt])
		{
			if (chrOtherPlayer[cnt].smCharInfo.Brood != smCHAR_MONSTER_USER && chrOtherPlayer[cnt].smCharInfo.Life[0]>0 &&
				(!chrOtherPlayer[cnt].smCharInfo.ClassClan || chrOtherPlayer[cnt].smCharInfo.ClassClan != lpCurChar->smCharInfo.ClassClan))
			{
				x = (lpCurChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpCurChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpCurChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x*x + z*z + y*y;

				if (dist < NearDist)
				{
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}



//�� �÷��̾ ã�´�
smCHAR *FindEmptyChar()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (!chrOtherPlayer[cnt].Flag)
			return &chrOtherPlayer[cnt];
	}

	return NULL;
}

//ä��â�� ���� ����
int AddChatBuff(char *szMsg, DWORD dwIP)
{
	Log::Debug("AddChatBuff : Requisitado por�m foi removido.");
	return NULL;
}
int AddChatBuff(char *szMsg)
{
	Log::Debug("AddChatBuff : Requisitado por�m foi removido.");
	return NULL;
}
int	ChatBuffFilter(char *szMsg, DWORD dwIP)
{
	Log::Debug("ChatBuffFilter : Requisitado por�m foi removido.");
	return NULL;
}


//�ý��� �ð��� ���ӽð����� ��ȯ
DWORD	ConvSysTimeToGameTime(DWORD dwTime)
{
	return dwTime / GAME_WORLDTIME_MIN;
}

//DispLoading

#define RECV_DATA_QUE_COUNT		64
#define RECV_DATA_QUE_MASK		63


class	rsTRANS_SERVER
{
	char	TransBuff[smSOCKBUFF_SIZE];

	rsRECORD_DBASE	*lpRecorder;


	smTHREADSOCK	*RecvDataQue[RECV_DATA_QUE_COUNT];
	int				RecvDataQuePush;
	int				RecvDataQuePop;

	TRANS_PLAYPOS	TransPlayPosLast;
	int				TransPlayPosCount;

public:
	smWINSOCK	*lpsmSock;


	//Ŭ���� �ʱ�ȭ
	int	Init(smWINSOCK *lpsmsock);
	//������ �޼��� ����
	int SendChat(char *szMessage);
	//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
	int SendPlayData(smCHAR *lpChar);

	//������ ���� ���� �޼��� ó��
	int RecvMessage(smTHREADSOCK *SockInfo);

	//�޼��� ť�� �޼����� ó��
	int RecvMessageQue();

};

/*
		case smTRANSCODE_ATTACKDATA:
		case smTRANSCODE_PLAYDATA1:
		case smTRANSCODE_PLAYDATA2:
		case smTRANSCODE_PLAYDATA3:
		case smTRANSCODE_TRANSPLAYDATA:
		case smTRANSCODE_PLAYDATAGROUP:
		case smTRANSCODE_PLAYITEM:
*/

//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_STAND
//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_WALK
//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_RUN


//�޼��� ť�� �޼����� ó��
int rsTRANS_SERVER::RecvMessageQue()
{
	int cnt;

	if (RecvDataQuePop >= RecvDataQuePush) return TRUE;
	if (MessageLoading) return FALSE;

	//ũ��ƼĮ ���� ����
	EnterCriticalSection(&cSection);

	while (1)
	{
		if (RecvDataQuePop >= RecvDataQuePush) break;

		cnt = RecvDataQuePop & RECV_DATA_QUE_MASK;

		if (RecvDataQue[cnt])
		{
			RecvMessage(RecvDataQue[cnt]);

			delete RecvDataQue[cnt];
			RecvDataQue[cnt] = 0;
		}

		RecvDataQuePop++;
	}

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection(&cSection);

	return TRUE;
}



DWORD	dwGetCharInfoTime = 0;

//������ ���� ���� �޼��� ó��
int rsTRANS_SERVER::RecvMessage(smTHREADSOCK *pData)
{
	Log::Debug("RecvMessage : Requisitado por�m foi removido.");
	return NULL;
}



int rsTRANS_SERVER::Init(smWINSOCK *lpsmsock)
{

	lpsmSock = lpsmsock;
	lpRecorder = 0;

	//ũ��ƼĮ ���� �ʱ�ȭ
	InitializeCriticalSection(&cServSection);

	RecvDataQuePush = 0;
	RecvDataQuePop = 0;

	ZeroMemory(RecvDataQue, RECV_DATA_QUE_COUNT * 4);

	return TRUE;
}


// ������ ���ڿ��� �߶󳽴�
extern int szSpaceSorting(char *lpString);

//ä�� ���ڸ� ������ ����
int rsTRANS_SERVER::SendChat(char *szMessage)
{
	int len;

	if (!szMessage || !szMessage[0]) return FALSE;

	if (szMessage[0] == '/' && (szMessage[1] == ';' || szMessage[1] == ':'))
	{
		// ������ ���ڿ��� �߶󳽴�
		szSpaceSorting(szMessage);
	}

	if (szMessage[0] == '`' && szMessage[1] == '/')
	{
		len = lstrlen(szMessage + 1) + 9;
		lstrcpy(TransBuff + 8, szMessage + 1);
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_CHATMESSAGE;
		if (smWsockDataServer) smWsockDataServer->Send(TransBuff, len, TRUE);
		return TRUE;
	}

	if (szMessage[0] == '~' && szMessage[1] == '/')
	{
		//�缭���� ���ÿ� �޼��� ���� '~'����
		len = lstrlen(szMessage + 1) + 9;
		lstrcpy(TransBuff + 8, szMessage + 1);
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_CHATMESSAGE;
		if (smWsockServer) smWsockServer->Send(TransBuff, len, TRUE);
		if (smWsockServer != smWsockUserServer && smWsockUserServer) smWsockUserServer->Send(TransBuff, len, TRUE);
		if (smWsockServer != smWsockExtendServer && smWsockExtendServer) smWsockExtendServer->Send(TransBuff, len, TRUE);
		if (AreaServerMode)
		{
			if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send(TransBuff, len, TRUE);
			if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send(TransBuff, len, TRUE);
		}

	}
	else
	{

		if (szMessage[0] == '@')
		{
			//���� ä��

			wsprintf(TransBuff + 8, "%s: %s", PlayerName, szMessage + 1);
			len = lstrlen(TransBuff + 8) + 9;
			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_PARTY_CHATMSG;

			if (smWsockServer) smWsockServer->Send(TransBuff, len, TRUE);
			AddChatBuff(TransBuff + 8, 4);
			return TRUE;
		}
		else
		{
			lstrcpy(TransBuff + 8, szMessage);
			len = lstrlen(szMessage) + 9;

			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_CHATMESSAGE;
		}

		if (szMessage[0] == '/')
		{

			if (szMessage[1] == 'c' && szMessage[2] == 'o' && szMessage[3] == 'u' && szMessage[4] == 'p' &&
				szMessage[5] == 'l' && szMessage[6] == 'e')
			{
				if (smWsockUserServer && cInvenTory.SearchItemCode(sinSP1 | sin15)) smWsockUserServer->Send(TransBuff, len, TRUE);
				return TRUE;
			}

			if (szMessage[1] == '/')
			{
				if (smWsockUserServer) smWsockUserServer->Send(TransBuff, len, TRUE);
			}
			else
			{
				if (smWsockServer) smWsockServer->Send(TransBuff, len, TRUE);
			}

			return TRUE;
		}

		if (AreaServerMode)
		{
			if (lpCurPlayer->OnStageField >= 0 && lpWSockServer_DispArea[lpCurPlayer->OnStageField])
			{
				lpWSockServer_DispArea[lpCurPlayer->OnStageField]->Send(TransBuff, len, TRUE);
			}
		}
		else
			if (smWsockUserServer) smWsockUserServer->Send(TransBuff, len, TRUE);

	}
	return TRUE;
}
//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
int rsTRANS_SERVER::SendPlayData(smCHAR *lpChar)
{
	//	int cnt;
	//	int	*lpTransBuff;
	TRANS_PLAYPOS *lpTransPlayPos;
	int	dist, x, y, z;

	TransPlayPosCount++;

	if ((TransPlayPosCount & 1) == 0)
	{
		x = (TransPlayPosLast.rsPlayPos.x - lpChar->pX) >> FLOATNS;
		y = (TransPlayPosLast.rsPlayPos.y - lpChar->pY) >> FLOATNS;
		z = (TransPlayPosLast.rsPlayPos.z - lpChar->pZ) >> FLOATNS;
		dist = x*x + y*y + z*z;

		if (sinGetLife() > 0 && dist < (16 * 16 * 16))
		{
			//��ġ�� ���Ѱ� ���� ��� ( 1���� �ɷ��� ���� )
			return TRUE;
		}
	}

	lpTransPlayPos = (TRANS_PLAYPOS *)TransBuff;
	lpTransPlayPos->code = smTRANSCODE_POSITION;
	lpTransPlayPos->size = sizeof(TRANS_PLAYPOS);

	if (lpCurPlayer->OnStageField >= 0)
		lpTransPlayPos->rsPlayPos.Area = StageField[lpCurPlayer->OnStageField]->FieldCode;
	else
		lpTransPlayPos->rsPlayPos.Area = -1;

	lpTransPlayPos->rsPlayPos.x = lpChar->pX;
	lpTransPlayPos->rsPlayPos.y = lpChar->pY;
	lpTransPlayPos->rsPlayPos.z = lpChar->pZ;

	if (lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD)
		lpTransPlayPos->Hp[0] = 0;
	else
		lpTransPlayPos->Hp[0] = sinGetLife();

	lpTransPlayPos->Hp[1] = lpChar->smCharInfo.Life[1];

	if (smWsockServer)
		smWsockServer->Send2((char *)lpTransPlayPos, lpTransPlayPos->size, TRUE);

	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
		smWsockExtendServer->Send2((char *)lpTransPlayPos, lpTransPlayPos->size, TRUE);


	int	MainArea, SubArea;

	if (AreaServerMode)
	{
		if (lpCurPlayer->OnStageField >= 0)
		{
			MainArea = (lpCurPlayer->OnStageField) & 1;
			SubArea = (lpCurPlayer->OnStageField + 1) & 1;

			if (lpWSockServer_DispArea[SubArea] && lpWSockServer_DispArea[MainArea] != lpWSockServer_DispArea[SubArea])
			{
				lpWSockServer_DispArea[SubArea]->Send2((char *)lpTransPlayPos, lpTransPlayPos->size, TRUE);
			}
		}
	}


	return TRUE;
}




//���� ��� Ŭ���� ����
rsTRANS_SERVER	rsMainServer;







/*
//�÷��̾� ������ ���濡 ����
int SendPlayerInfo( smWINSOCK *lpsmSock )
{

	smTRNAS_PLAYERINFO	*lpTransPlayerInfo;

	lpTransPlayerInfo = (smTRNAS_PLAYERINFO *)TransBuff;

	lstrcpy( lpTransPlayerInfo->smCharInfo.szName , PlayerName );
	lstrcpy( lpTransPlayerInfo->smCharInfo.szModelName , lpCurPlayer->lpDinaPattern->szPatName );

	lpTransPlayerInfo->size = sizeof( smTRNAS_PLAYERINFO );
	lpTransPlayerInfo->code = smTRANSCODE_PLAYERINFO;
	lpsmSock->Send( (char *)lpTransPlayerInfo , lpTransPlayerInfo->size );

	return TRUE;
}
*/

#define SAVE_CHAT_COMMAND_MAX		16
#define SAVE_CHAT_COMMAND_MASK		15

char szSaveChatCommands[SAVE_CHAT_COMMAND_MAX][256];
int ChatCommandSaveCnt = 0;
int ChatSaveRecallCnt = 0;

//ä�� �ڸǵ� ����
int SaveChatCommand(char *szMessage)
{
	int len, cnt;
	int mcnt;
	char szBuff[256];

	len = lstrlen(szMessage);
	lstrcpy(szBuff, szMessage);

	for (cnt = 0; cnt < len; cnt++)
	{
		if (szBuff[cnt] == ' ')
		{
			szBuff[cnt] = 0;
			break;
		}
	}
	lstrcat(szBuff, " ");

	//���Ϲ��� Ȯ��
	for (cnt = 0; cnt < ChatCommandSaveCnt; cnt++)
	{
		if (cnt >= SAVE_CHAT_COMMAND_MAX) break;
		if (lstrcmp(szSaveChatCommands[cnt], szBuff) == 0)
			return TRUE;
	}

	mcnt = ChatCommandSaveCnt & SAVE_CHAT_COMMAND_MASK;
	lstrcpy(szSaveChatCommands[mcnt], szBuff);

	ChatCommandSaveCnt++;
	ChatSaveRecallCnt = ChatCommandSaveCnt;

	return TRUE;
}

//ä�� ���� �ڸǵ� ���� 
int RecallSavedChatCommand(HWND hChatWnd, int Arrow)
{
	int mcnt;
	int len;

	if (ChatCommandSaveCnt <= 0) return FALSE;

	if (Arrow)
	{
		ChatSaveRecallCnt--;
		if (ChatSaveRecallCnt < 0)
		{
			ChatSaveRecallCnt = ChatCommandSaveCnt - 1;
		}
	}
	else
	{
		ChatSaveRecallCnt++;
		if (ChatSaveRecallCnt >= ChatCommandSaveCnt)
		{
			ChatSaveRecallCnt = ChatCommandSaveCnt - 1;
			SetWindowText(hChatWnd, "");
			return TRUE;
		}
	}

	mcnt = ChatSaveRecallCnt & SAVE_CHAT_COMMAND_MASK;
	SetWindowText(hChatWnd, szSaveChatCommands[mcnt]);

	len = lstrlen(szSaveChatCommands[mcnt]);
	SendMessage(hChatWnd, EM_SETSEL, len, len);

	return TRUE;
}

// ���ڿ� �� ª�� ������ ���̸�ŭ ���Ͽ� �� ���� ������ ��ȯ
int rsCompString(char *src1, char *src2)
{
	int cnt;
	int len1, len2;
	int len;

	len1 = lstrlen(src1);
	len2 = lstrlen(src2);

	if (len1 < len2) len = len1;
	else len = len2;

	if (!len) return NULL;

	for (cnt = 0; cnt < len1; cnt++)
	{
		if (src1[cnt] == 0) break;
		if (src1[cnt] != src2[cnt]) return NULL;
	}

	return cnt;
}



char szLastChatMessage[256];
DWORD	dwLastChatTime;
DWORD	dwContiueChatCount = 0;
#include "CurseFilter.h"		//�弳����

char *rsGetWord(char *q, char *p);		//������ ����� ��

extern int WaveCameraMode;

//ä�� ���ڸ� ������ ����
int SendChatMessageToServer(char *szChatMessage)
{
	Log::Debug("SendChatMessageToServer : Requisitado por�m foi removido.");
	return NULL;
}

//ä�� ��ũ�� ����Ʈ
int ChatScrollPoint = 0;

//ä�� ���� ȭ�鿡 ǥ��
int DisplayChatMessage(HDC hdc, int x, int y, int MaxLine)
{
	int cnt;
	int chatCnt;
	char *lpString;
	int start, end;

	if (ChatDispCnt == 0) return FALSE;

	if (ChatScrollPoint > CHATBUFF_MASK)
		ChatScrollPoint = CHATBUFF_MASK;
	if (ChatScrollPoint < 0)
		ChatScrollPoint = 0;

	start = ChatBuffCnt - MaxLine - ChatScrollPoint;
	end = ChatBuffCnt - ChatScrollPoint;

	for (cnt = start; cnt < end; cnt++)
	{
		if (cnt >= 0)
		{
			chatCnt = cnt & CHATBUFF_MASK;
			lpString = ChatBuff[chatCnt].szMessage;

			SetTextColor(hdc, RGB(0, 0, 0));
			dsTextLineOut(hdc, x + 1, y + 1, lpString, lstrlen(lpString));
			if (ChatBuff[chatCnt].dwIP == dwMyIP)
				SetTextColor(hdc, RGB(255, 128, 96));
			else
			{
				SetTextColor(hdc, RGB(255, 255, 128));

				switch (ChatBuff[chatCnt].dwIP)
				{
					case 1:
						SetTextColor(hdc, RGB(255, 128, 96));
						break;
					case 2:
						SetTextColor(hdc, RGB(96, 256, 255));
						break;
				}
			}
			dsTextLineOut(hdc, x, y, lpString, lstrlen(lpString));
		}
		y += 16;
	}

	return TRUE;
}







//250 , 60 ,342 , 70

//��Ʈ�� ���� ī����
int GetNextLineCount(char *szString, int LineMax)
{
	int cnt;
	int len;
	char *lp;

	len = lstrlen(szString);
	if (len <= LineMax) return 0;

	lp = szString;

	cnt = 0;

	while (1)
	{
		//2����Ʈ �ڵ� Ȯ��
		if (CheckCode_2Byte(lp) == 2)
		{
			cnt += 2;
			lp += 2;
		}
		else
		{
			cnt++;
			lp++;
		}

		if (cnt > (LineMax - 2)) break;
	}


	return cnt;
}

int SetIpColor(HDC hdc, DWORD dwIP)
{

	if (dwIP == dwMyIP)
	{
		SetTextColor(hdc, RGB(255, 128, 96));
		return TRUE;
	}

	switch (dwIP)
	{
		case 0:
			SetTextColor(hdc, RGB(255, 128, 96));				//�����޼��� (�����۾�)
			break;
		case 1:
			//		SetTextColor( hdc, RGB(255, 190, 150) );
			SetTextColor(hdc, RGB(150, 190, 255));			//�Ӹ�
			break;
		case 2:
			//		SetTextColor( hdc, RGB(255, 140, 120) );
			SetTextColor(hdc, RGB(150, 190, 255));			//
			break;
		case 3:
			SetTextColor(hdc, RGB(230, 160, 255));			//����
			break;
		case 4:
			SetTextColor(hdc, RGB(200, 255, 0));				//����ä��
			break;
		case 5:
			SetTextColor(hdc, RGB(255, 255, 80));			//Ŭ��ä��
			break;
		case 6:
			SetTextColor(hdc, RGB(200, 255, 255));			//�ŷ�ä��
			break;

		case 9:
			SetTextColor(hdc, RGB(200, 128, 128));			//���� ����â
			break;

		default:
			//		SetTextColor( hdc, RGB(180, 160, 255) );
			SetTextColor(hdc, RGB(255, 255, 192));
	}
	/*
		if ( dwIP>=0x010101 ) {
			//Custom Color
			SetTextColor( hdc, dwIP );
		}
	*/

	return TRUE;
}

static int	DisplayChatFilter = -1;
int	DispChatMsgHeight = 6;			//�ֱ� ǥ�õ� ä��â �� ����
int	DispChatMode = 0;

//ä��â ���͸�
int	SetChatMsgFilter(int mode)
{

	DisplayChatFilter = -1;
	ChatScrollPoint = 0;

	switch (mode)
	{
		case 0:			//��ü
			DisplayChatFilter = -1;
			break;

		case 1:			//Ŭ��
			DisplayChatFilter = 5;
			break;

		case 2:			//����
			DisplayChatFilter = 4;
			break;

		case 3:			//�ŷ�
			DisplayChatFilter = 6;
			break;

		case 4:			//�Ӹ�
			DisplayChatFilter = 1;
			break;
	}

	DispChatMode = mode;

	return TRUE;
}

int ChatBuffSort()
{
	int cnt, cnt2;
	int mCnt, nCnt;
	int	Col;

	if (ChatBuffCnt <= ChatBuffCompCnt) return FALSE;

	for (cnt = ChatBuffCompCnt; cnt < ChatBuffCnt; cnt++)
	{
		mCnt = cnt&CHATBUFF_MASK;
		Col = 0;
		switch (ChatBuff[mCnt].dwIP)
		{
			case 5:
				Col = 1;
				break;
			case 4:
				Col = 2;
				break;

			case 6:
				Col = 3;
				break;
			case 1:
				Col = 4;
				break;

			case 9:
				Col = 5;
				break;
		}
		nCnt = ChatBuffCnt2[Col] & CHATBUFF_MASK;
		memcpy(&ChatBuff2[Col][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
		ChatBuffCnt2[Col]++;

		if (Col > 0 && Col < 5)
		{
			nCnt = ChatBuffCnt2[0] & CHATBUFF_MASK;
			memcpy(&ChatBuff2[0][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
			ChatBuffCnt2[0]++;
		}

		if (Col == 0 && ChatBuff[mCnt].dwIP < 10)
		{
			for (cnt2 = 1; cnt2 < 5; cnt2++)
			{
				nCnt = ChatBuffCnt2[cnt2] & CHATBUFF_MASK;
				memcpy(&ChatBuff2[cnt2][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
				ChatBuffCnt2[cnt2]++;
			}
		}
	}

	ChatBuffCompCnt = ChatBuffCnt;

	return TRUE;
}
//ä�� ���� ȭ�鿡 ǥ�� ( ���� )
int DisplayChatMessage3(HDC hdc, int x, int y, int StrMax, int LineMax, int Mode)
{
	int result;
	int ModeBackup = DispChatMode;
	int	ScrollBackup = ChatScrollPoint;

	DispChatMode = Mode;
	if (LineMax <= 6)
		ChatScrollPoint = 0;

	result = DisplayChatMessage2(hdc, x, y, StrMax, LineMax);

	DispChatMode = ModeBackup;
	ChatScrollPoint = ScrollBackup;
	return result;
}

//ä�� ���� ȭ�鿡 ǥ��
int DisplayChatMessage2(HDC hdc, int x, int y, int StrMax, int LineMax)
{
	int cnt;
	int chatCnt;
	char *lpString;
	int LineCnt;
	int CutPos;
	int len;
	int start, end;
	int fl_Count;

	DWORD shColor = RGB(74, 74, 74);


	//if ( ChatDispCnt==0 ) return FALSE;
	ChatBuffSort();

	LineCnt = 0;

	if (ChatScrollPoint > (CHATBUFF_MAX - 32))
		ChatScrollPoint = (CHATBUFF_MAX - 32);
	if (ChatScrollPoint < 0)
		ChatScrollPoint = 0;

	fl_Count = DispChatMode;
	if (fl_Count < 0) fl_Count = 0;

	start = ChatBuffCnt2[fl_Count] - 1 - ChatScrollPoint;
	end = ChatBuffCnt2[fl_Count] - 32 - ChatScrollPoint;


	//	for( cnt=ChatBuffCnt-1;cnt>=ChatBuffCnt-32;cnt--) {

	DispChatMsgHeight = 0;

	for (cnt = start; cnt >= end; cnt--)
	{
		if (cnt >= 0)
		{
			chatCnt = cnt & CHATBUFF_MASK;

			//#ifdef _LANGUAGE_CHINESE //�߱� ä��â Ư����� ������
			//			lpString = ChatBuff2[fl_Count][chatCnt].szMessage;
			//			ConvertStringTHAI(lpString, lstrlen(lpString)+16);
			//#else
			lpString = ChatBuff2[fl_Count][chatCnt].szMessage;
			//#endif

						//if ( DisplayChatFilter<0 || DisplayChatFilter==ChatBuff[chatCnt].dwIP || ChatBuff[chatCnt].dwIP==0 || ChatBuff[chatCnt].dwIP==3 ) {


			DispChatMsgHeight++;			//���� ������

			len = lstrlen(lpString);
			if (len > StrMax)
			{
				//��Ʈ�� ���� ī����
				CutPos = GetNextLineCount(lpString, StrMax);

				SetTextColor(hdc, shColor);
				dsTextLineOut(hdc, x - 1, y - 1, lpString + CutPos, len - CutPos);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
				dsTextLineOut(hdc, x, y, lpString + CutPos, len - CutPos);

				y -= 12;
				LineCnt++;
				if (LineCnt >= LineMax) break;

#ifdef	_LANGUAGE_ENGLISH
				//���� �ٹٲ޽� '-' �߰�
				char szStrBuff[256];

				memcpy(szStrBuff, lpString, CutPos);
				szStrBuff[CutPos] = 0;


				if (lpString[CutPos] > ' ' && lpString[CutPos + 1] > ' ')
				{
					szStrBuff[CutPos] = '-';
					szStrBuff[CutPos + 1] = 0;
				}

				len = lstrlen(szStrBuff);

				SetTextColor(hdc, shColor);
				dsTextLineOut(hdc, x - 1, y - 1, szStrBuff, len);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
				dsTextLineOut(hdc, x, y, szStrBuff, len);
#else
#ifdef	_LANGUAGE_ARGENTINA
				// �Ƹ���Ƽ�� �ٹٲ� kyle
				char szStrBuff[256];

				memcpy(szStrBuff, lpString, CutPos);
				szStrBuff[CutPos] = 0;


				if (lpString[CutPos] > ' ' && lpString[CutPos + 1] > ' ')
				{
					szStrBuff[CutPos] = '-';
					szStrBuff[CutPos + 1] = 0;
				}

				len = lstrlen(szStrBuff);

				SetTextColor(hdc, shColor);
				dsTextLineOut(hdc, x - 1, y - 1, szStrBuff, len);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
				dsTextLineOut(hdc, x, y, szStrBuff, len);
#else
				SetTextColor(hdc, shColor);
				dsTextLineOut(hdc, x - 1, y - 1, lpString, CutPos);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
#ifdef _LANGUAGE_BRAZIL
				char semi[MAX_PATH], presemi[MAX_PATH], backsemi[MAX_PATH];
				ZeroMemory(presemi, MAX_PATH);
				ZeroMemory(backsemi, MAX_PATH);
				bool bsemi = 0;
				strcpy_s(semi, lpString);
				int k = 0, prelen = 0, lenth = 0;
				lenth = strlen(semi);
				//���ڿ� ���ʴ�� �˻�
				for (int i = 0; i < lenth; i++)
				{
					backsemi[k] = semi[i];
					k++;
					if ((bsemi == 0) && (semi[i] == ':'))
					{
						strcpy_s(presemi, backsemi);
						ZeroMemory(backsemi, MAX_PATH);
						bsemi++;
						k = 0;
					}
				}
				//:�־����� ������ �ٲ���
				if (bsemi)
				{
					SIZE size;
					//:�չ��ڿ�
					prelen = strlen(presemi);
					SetTextColor(hdc, RGB(100, 255, 50));
					dsTextLineOut(hdc, x, y, presemi, prelen);
					GetTextExtentPoint(hdc, presemi, prelen, &size);
					//�ڹ��ڿ�
					int j = 0;
					j = x + size.cx;
					SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
					dsTextLineOut(hdc, j, y, backsemi, CutPos - prelen);
				}
				else dsTextLineOut(hdc, x, y, lpString, CutPos);
#else
				dsTextLineOut(hdc, x, y, lpString, CutPos);
#endif
#endif
#endif

				y -= 12;
				LineCnt++;
				if (LineCnt >= LineMax) break;

			}
			else
			{

				SetTextColor(hdc, shColor);
				dsTextLineOut(hdc, x - 1, y - 1, lpString, len);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
#ifdef _LANGUAGE_BRAZIL
				char semi[MAX_PATH], presemi[MAX_PATH], backsemi[MAX_PATH];
				ZeroMemory(presemi, MAX_PATH);
				ZeroMemory(backsemi, MAX_PATH);
				bool bsemi = 0;
				strcpy_s(semi, lpString);
				int k = 0, prelen = 0, lenth = 0;
				lenth = strlen(semi);
				//���ڿ� ���ʴ�� �˻�
				for (int i = 0; i < lenth; i++)
				{
					backsemi[k] = semi[i];
					k++;
					if ((bsemi == 0) && (semi[i] == ':'))
					{
						strcpy_s(presemi, backsemi);
						ZeroMemory(backsemi, MAX_PATH);
						bsemi++;
						k = 0;
					}
				}
				//:�־����� ������ �ٲ���
				if (bsemi)
				{
					SIZE size;
					//:�չ��ڿ�
					prelen = strlen(presemi);
					SetTextColor(hdc, RGB(100, 255, 50));
					dsTextLineOut(hdc, x, y, presemi, prelen);
					GetTextExtentPoint(hdc, presemi, prelen, &size);
					//�ڹ��ڿ�
					int j = 0;
					j = x + size.cx;
					SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
					dsTextLineOut(hdc, j, y, backsemi, strlen(backsemi));
				}
				else dsTextLineOut(hdc, x, y, lpString, len);
#else
				dsTextLineOut(hdc, x, y, lpString, len);
#endif

				y -= 12;
				LineCnt++;
				if (LineCnt >= LineMax) break;
			}
			//}
		}
	}

	return TRUE;
}


//�� �÷��̾�� ����
int ConnectPlayer(smWINSOCK *lpsmSock)
{
	int OthPlayerCnt;

	if (ServerMode)
		return Serv_ConnectPlayer(lpsmSock);


	//ũ��ƼĮ ���� ����
	EnterCriticalSection(&cSection);

	OthPlayerCnt = FindNewOtherPlayer();
	if (OthPlayerCnt >= 0)
	{
		lpsmSock->ExtData1 = (void *)&chrOtherPlayer[OthPlayerCnt];
		chrOtherPlayer[OthPlayerCnt].Init();
		chrOtherPlayer[OthPlayerCnt].TransSendWait = 1;
		chrOtherPlayer[OthPlayerCnt].TransSock = lpsmSock;
		chrOtherPlayer[OthPlayerCnt].TransLastSendCnt = PlayCounter - 70;
		chrOtherPlayer[OthPlayerCnt].Flag = 1;
		chrOtherPlayer[OthPlayerCnt].smCharInfo.szName[0] = 0;
		chrOtherPlayer[OthPlayerCnt].ActionPattern = 99;
		chrOtherPlayer[OthPlayerCnt].Pattern = 0;
		chrOtherPlayer[OthPlayerCnt].AutoPlayer = 0;

		lpCurPlayer->SendCharInfo(lpsmSock);
		//		SendPlayerInfo( lpsmSock );

	}

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection(&cSection);

	return TRUE;
}

//�÷��̾� ���� ������
int DisconnectPlayer(smWINSOCK *lpsmSock)
{
	//	int cnt;
	smCHAR *lpPlayer;
	DWORD	dwTime;

	dwTime = GetCurrentTime();

	if (ServerMode)
		return Serv_DisconnectPlayer(lpsmSock);

	if (smWsockServer && lpsmSock->sock == smWsockServer->sock)
	{
		AddChatBuff("�������� ������ ���� �����ϴ�");
		if (smWsockServer == smWsockDataServer)
			smWsockDataServer = 0;
		if (smWsockServer == smWsockUserServer)
			smWsockUserServer = 0;
		if (smWsockServer == smWsockExtendServer)
			smWsockExtendServer = 0;

		smWsockServer = 0;
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 1;
		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if (cTrade.OpenFlag)
		{
			SendRequestTrade(cTrade.TradeCharCode, 3);			//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();								//�ŷ�â �ݱ�
		}
		return TRUE;
	}

	if (smWsockDataServer && lpsmSock->sock == smWsockDataServer->sock)
	{
		AddChatBuff("����Ÿ �������� ������ ���� �����ϴ�");
		smWsockDataServer = 0;
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 2;

		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if (cTrade.OpenFlag)
		{
			SendRequestTrade(cTrade.TradeCharCode, 3);			//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();								//�ŷ�â �ݱ�
		}
		return TRUE;
	}

	if (smWsockUserServer && lpsmSock->sock == smWsockUserServer->sock)
	{
		AddChatBuff("�������� ������ ���� �����ϴ�");
		smWsockUserServer = 0;
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if (cTrade.OpenFlag)
		{
			SendRequestTrade(cTrade.TradeCharCode, 3);			//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();					//�ŷ�â �ݱ�
		}
		return TRUE;
	}

	if (smWsockExtendServer && lpsmSock->sock == smWsockExtendServer->sock)
	{
		AddChatBuff("�������� ������ ���� �����ϴ�");
		smWsockExtendServer = 0;
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if (cTrade.OpenFlag)
		{
			SendRequestTrade(cTrade.TradeCharCode, 3);		//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();							//�ŷ�â �ݱ�
		}
		return TRUE;

	}

	char szBuff[256];

	//���� ���� ���� ������
	if (AreaServerMode)
	{
		if (lpWSockServer_Area[0] && lpWSockServer_Area[0]->sock == lpsmSock->sock)
		{
			if (smConfig.DebugMode)
			{
				wsprintf(szBuff, "Disconnect Area Server (%s)", lpWSockServer_Area[0]->szIPAddr);
				AddChatBuff(szBuff, 0);
			}
			if (lpWSockServer_Area[0] == lpWSockServer_DispArea[0]) lpWSockServer_DispArea[0] = 0;
			if (lpWSockServer_Area[0] == lpWSockServer_DispArea[1]) lpWSockServer_DispArea[1] = 0;

			lpWSockServer_Area[0] = 0;
			return TRUE;
		}
		if (lpWSockServer_Area[1] && lpWSockServer_Area[1]->sock == lpsmSock->sock)
		{
			if (smConfig.DebugMode)
			{
				wsprintf(szBuff, "Disconnect Area Server (%s)", lpWSockServer_Area[1]->szIPAddr);
				AddChatBuff(szBuff, 0);
			}

			if (lpWSockServer_Area[1] == lpWSockServer_DispArea[0]) lpWSockServer_DispArea[0] = 0;
			if (lpWSockServer_Area[1] == lpWSockServer_DispArea[1]) lpWSockServer_DispArea[1] = 0;

			lpWSockServer_Area[1] = 0;
			return TRUE;
		}
	}



	//ũ��ƼĮ ���� ����
	EnterCriticalSection(&cSection);

	lpPlayer = (smCHAR *)lpsmSock->ExtData1;
	if (lpPlayer)
	{
		lpPlayer->Close();
	}

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection(&cSection);

	return TRUE;
}



//���������� ������ ����Ÿ�� ���� ī����
int Ts_LastSendCounter;
DWORD	dwLastSendPosiTime = 0;		//������ ���� �ð� (��ġ)
DWORD	dwLastSendPlayTime = 0;		//������ ���� �ð� (��ǵ���Ÿ)

//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
int SendPlayDataToServer()
{
	Log::Debug("SendPlayDataToServer : Requisitado por�m foi removido.");
	return NULL;
}

//�ٸ� ������ ���� ��Ų��
int ConnectOtherPlayer(DWORD dwIP)
{
	smWINSOCK *lpsmSock;
	int OthPlayerCnt;

	OthPlayerCnt = FindNewOtherPlayer();
	if (OthPlayerCnt >= 0)
	{
		lpsmSock = smConnectSock2(ConvStrIP(dwIP), TCP_GAMEPORT);
		if (lpsmSock)
		{
			ConnectPlayer(lpsmSock);
			return TRUE;
		}
	}
	return FALSE;
}
//�ٸ� �÷��̾���� ���� ������
DWORD WINAPI ConnectPlayThreadProc(void *pInfo)
{

	ConnectOtherPlayer((DWORD)pInfo);

	ExitThread(TRUE);
	return TRUE;
}
//�ٸ� �÷��̾���� ���� ���� ������
DWORD WINAPI DisconnectPlayThreadProc(void *pInfo)
{

	((smWINSOCK *)pInfo)->CloseSocket();

	ExitThread(TRUE);
	return TRUE;
}



//�ٸ� �÷��̾���� ���� �����带 ����
int CreateConnectThread(DWORD dwIP)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, ConnectPlayThreadProc, (void *)dwIP, 0, &dwMsgId);

	return TRUE;
}

//�ٸ� �÷��̾���� �������� ������ ����
int CreateDisconnectThread(smWINSOCK *lpsmSock)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, DisconnectPlayThreadProc, (void *)lpsmSock, 0, &dwMsgId);

	return TRUE;
}


//�Ÿ��� �� �ٸ� �������� ������ ���´�
int DisconnectFarPlayer()
{
	int mx, mz, dDist;
	int cnt;
	int timeover;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		//�Ÿ��� Ȯ���Ͽ� �� ��쿡�� ������ ���������
		if (chrOtherPlayer[cnt].Flag)
		{

			if ((chrOtherPlayer[cnt].dwLastTransTime + DIS_TIME_OVER) < dwPlayTime)
				timeover = TRUE;
			else
				timeover = FALSE;

			//�Ÿ����
			mx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
			mz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
			dDist = mx*mx + mz*mz;

			if ((chrOtherPlayer[cnt].DisplayFlag && dDist > DIST_TRANSLEVEL_DISCONNECT) || timeover)
			{
				//���� ����/ ���� ����
				chrOtherPlayer[cnt].Flag = 0;

				if (chrOtherPlayer[cnt].TransSock)
				{
					chrOtherPlayer[cnt].TransSock->CloseSocket();
				}
				else
				{
					//ũ��ƼĮ ���� ����
					EnterCriticalSection(&cSection);
					chrOtherPlayer[cnt].Close();
					//ũ��ƼĮ ���� ����
					LeaveCriticalSection(&cSection);
				}
			}
		}
	}

	return TRUE;
}

DWORD	dwExtendServ_RecvTime = 0;
DWORD	dwUserServ_RecvTime = 0;
DWORD	dwMainServ_RecvTime = 0;
DWORD	dwDataServ_RecvTime = 0;

//����Ÿ �Լ�
int RecvPlayData(smTHREADSOCK *pData)
{

	DWORD *IPData;
	//smCHAR *lpPlayer;

	DWORD	dwTime;

	//ũ��ƼĮ ���� ����
	EnterCriticalSection(&cSection);


	IPData = (DWORD *)(pData->Buff);


	/*
		if ( pData->smMySock==smWsockServer ||
			 pData->smMySock==smWsockUserServer ||
			 pData->smMySock==smWsockDataServer ||
			  pData->smMySock==smWsockExtendServer ) {
	*/
	dwTime = GetCurrentTime();

	if (pData->smMySock == smWsockDataServer)
	{
		dwRecvDataServerTime = dwTime;
		dwDataServ_RecvTime = dwTime;
	}
	if (pData->smMySock == smWsockUserServer)
	{
		dwRecvUserServerTime = dwTime;
		dwUserServ_RecvTime = dwTime;
		Debug_RecvCount2++;
	}
	if (pData->smMySock == smWsockExtendServer)
	{
		dwRecvExtendServerTime = dwTime;
		dwExtendServ_RecvTime = dwTime;
		Debug_RecvCount3++;
	}
	if (pData->smMySock == smWsockServer)
	{
		Debug_RecvCount1++;
		dwRecvServerTime = dwTime;
		dwMainServ_RecvTime = dwTime;
	}

	//������ ����Ÿ ó��
	rsMainServer.RecvMessage(pData);
	/*
		}
		else {
			//Ŭ���̾�Ʈ���� ����..

			lpPlayer = (smCHAR *)pData->smMySock->ExtData1;
			if ( lpPlayer ) lpPlayer->RecvPlayData( pData );
		}
	*/
	//ũ��ƼĮ ���� ����
	LeaveCriticalSection(&cSection);

	return TRUE;
}

//�޼��� ť�� �޼����� ó��
int PlayRecvMessageQue()
{
	return rsMainServer.RecvMessageQue();
}


int LastSendCnt;
/*
int SendPlayData( smCHAR *player )
{
	int cnt;

	for(cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].Pattern
			&& chrOtherPlayer[cnt].AutoPlayer==0 ) {
			chrOtherPlayer[cnt].SendPlayData( player );
		}
	}

	return TRUE;
}
*/
#define	NET_SENDING_WAIT_TIME		400
#define	NET_SENDING_WAIT_TIME2		1000

int NetStandCnt = 0;
extern int GameMode;

DWORD	NetScoopTime = 2000;

//�� �÷��� ����
int NetWorkPlay()
{
	//	char *szIP;
	int cnt;
	DWORD	dwTime;
	DWORD	dwTime2;
	smWINSOCK	*lpsmSock;

	//����� - XignCode
#ifdef _XIGNCODE_CLIENT
	ZCWAVE_Init();
#endif

	if (NetWorkInitFlag == 0)
	{
		if (!smWsockServer || !smWsockDataServer || !smWsockUserServer || !smWsockExtendServer)
		{
			if (ConnectServer_Main() == NULL)
				return FALSE;
			//smWsockServer = smConnectSock( szServIP , dwServPort );
		}

		//Sleep( 60*1000*3 );		//3�� ���

		NetWorkInitFlag = TRUE;

		rsMainServer.Init(smWsockServer);

		//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
		SendPlayDataToServer();

		//������ ���� ����
		if (smConfig.DebugMode) SendAdminMode(TRUE);


		//		if ( smWsockServer )
		//			lpCurPlayer->SendCharInfo( smWsockServer );			//�÷��̾� ���� ����

				//����� ĳ���� ����Ÿ �ҷ����� �䱸
		if (smWsockDataServer)
		{

			SendGetRecordData(lpCurPlayer->smCharInfo.szName);

			//���� IP ýũ ( �������� ������ IP�뺸 )
			if (smWsockServer && smWsockDataServer != smWsockServer)
			{
				SendCheckIP(1, smWsockServer);
			}
			if (smWsockUserServer && smWsockDataServer != smWsockUserServer)
			{
				SendCheckIP(2, smWsockUserServer);
			}
			if (smWsockExtendServer && smWsockExtendServer != smWsockServer && smWsockDataServer != smWsockExtendServer)
			{
				SendCheckIP(3, smWsockExtendServer);
			}
		}

		ChatBuffCnt = 0;
		ChatDispCnt = 0;

		Ts_LastSendCounter = PlayCounter;
		TransThrowItem.code = 0;			//������ ������ �ʱ�ȭ

		RecordFailCount = 0;			//���� ���� ī����
		dwLastWareHouseChkSum = 0;		//â�� üũ�� �ڵ�

		TransAgingItemFlag = 0;			//����¡ �Ϸ� ������ ���� �÷�


		ZeroMemory(&TransRecvItem, sizeof(TRANS_ITEMINFO));

		Trans_SplashCharList.code = 0;	//���÷��� ���� ���� �ʱ�ȭ

		ZeroMemory(&PK_FieldState, sizeof(PK_FIELD_STATE));	//PK ���� �ʱ�ȭ
		PK_FieldState.FieldCode = -1;


		//���� �ڵ� ť�� ����
		ClearAttMonsterCode();

		Init_RecordDamage();			//���ݷ� ��� �ʱ�ȭ

		switch (smConfig.NetworkQuality)
		{
			case 0:
				NetScoopTime = 3000;
				break;
			case 1:
				NetScoopTime = 2000;
				break;
			case 2:
				NetScoopTime = 1000;
				break;
			case 3:
				NetScoopTime = 5000;
				smTransTurbRcvMode = 50;
				break;
		}

	}
	else
	{

		if (cTrade.OpenFlag)
		{
			//������ ��ȯ�� �Ϸ� Ȯ��
			if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag)
			{
				SendTradeCheckItem(cTrade.TradeCharCode);
				TradeSendSucessFlag = TRUE;
			}
		}
		if (dwTradeMaskTime)
		{
			//Ʈ���̵� ����ũ Ÿ�̸� �ʱ�ȭ
			if (dwTradeMaskTime<dwPlayTime) dwTradeMaskTime = 0;
		}

		dwTime = dwPlayTime - dwLastSendPosiTime;
		dwTime2 = dwPlayTime - dwRecvServerTime;

		//�������� ����Ÿ ����
		if ((PlayCounter - Ts_LastSendCounter)>PLAYSERVER_SENDCOUNTER)
		{//&& dwTime>NET_SENDING_WAIT_TIME &&
//			dwTime2<NET_SENDING_WAIT_TIME ) {
			//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
			SendPlayDataToServer();
			//�Ÿ��� �� �ٸ� �������� ������ ���´�
			DisconnectFarPlayer();

			//ī���� ����
			Ts_LastSendCounter = PlayCounter;
			dwLastSendPosiTime = dwPlayTime;
		}

		//		if ( (xxcnt&0x3)==3 ) SendPlayData( lpCurPlayer );

				//�ٸ� ��������� ����Ÿ ��ȯ
				//SendPlayData( lpCurPlayer );

		if (smWsockUserServer)
		{// && rsRecvUserServer ) {
//�÷��̾� ����Ÿ�� ������ ���� (40/70) �ʴ��� ���� 
//if ( (lpCurPlayer->PlayBuffCnt&0x3F)==0 ) {

			dwTime = dwPlayTime - dwLastSendPlayTime;
			dwTime2 = dwPlayTime - dwRecvUserServerTime;


			if ((lpCurPlayer->PlayBuffCnt & 0x3F) == 0 && lpCurPlayer->dwObjectSerial &&
				dwTime > NET_SENDING_WAIT_TIME)
			{//&& dwTime2<NET_SENDING_WAIT_TIME2 ) {

				if (smConfig.DebugMode && AdminCharDisable)
				{
					if (smWsockServer == smWsockUserServer)
					{
						rsMainServer.SendPlayData(lpCurPlayer);
					}
				}
				else
				{
					if (AreaServerMode)
					{
						if (lpCurPlayer->OnStageField >= 0)
						{
							lpsmSock = lpWSockServer_DispArea[lpCurPlayer->OnStageField];
						}

					}
					else
					{
						lpsmSock = smWsockUserServer;
					}

					if (lpsmSock)
					{
						lpCurPlayer->MakeTransPlayData(lpCurPlayer->srTransBuff, 0x40);
						if (((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->PlayBuffCnt > 1)
						{
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send(lpCurPlayer->srTransBuff, ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->size, TRUE);
							NetStandCnt = 0;
						}
						else
						{
							//���ִ� ���� ��Ʈ�� ���� �ּ� ( �ѹ� ������ ��Ʈ�p ���� )
							//if ( NetStandCnt==0 )
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send(lpCurPlayer->srTransBuff, ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->size, TRUE);
							NetStandCnt++;
						}
					}
				}

				Debug_SendCount++;

				rsRecvUserServer = FALSE;
				dwLastSendPlayTime = dwPlayTime;
			}
		}

		//���� ���� �ε� ��
		if (dwLoadingTime)
		{
			dwTime = GetCurrentTime();
			if (dwTime > (dwLoadingTime + NET_LOADING_WAIT_LIMIT))
			{
				//�ð� �ʰ� ����
				DisconnectFlag = GetCurrentTime();
				dwLoadingTime = 0;
			}
		}


		cnt = PlayCounter & 0x1FF;
		if (cnt == 0)
		{
			//����� ýũ ( �޼����� ���� ���� ������ ��Ʈ�� �ٿ� )
			TransCheckNetwork();
		}
		else if (cnt == 0x80)
		{
			//���μ��� �ð� �������� ������ �ۺ�
			SendPlayTimerMax();
		}



		if ((PlayCounter & 0x1F) == 0)
		{
			dwTime = GetCurrentTime();
			if (GameMode == 2 && dwMainServ_RecvTime && smWsockServer && (dwTime - dwMainServ_RecvTime) > 3000)
			{

				if (smWsockServer->WaitReadMessage >= 0)
				{
					smWsockServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						AddChatBuff("Scoop Recv Route(Main)");
				}
				else
				{
					if (smConfig.DebugMode)
						AddChatBuff("Error Server(Main)");
				}
			}
			if (GameMode == 2 && smWsockUserServer && dwUserServ_RecvTime &&
				smWsockServer != smWsockUserServer && (dwTime - dwUserServ_RecvTime) > 3000)
			{
				//dwUserServ_RecvTime = dwTime;
				if (smWsockUserServer->WaitReadMessage >= 0)
				{
					smWsockUserServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						AddChatBuff("Scoop Recv Route(User)");
				}
				else
				{
					if (smConfig.DebugMode)
						AddChatBuff("Error Server(User)");
				}
			}
			if (GameMode == 2 && smWsockExtendServer && dwExtendServ_RecvTime &&
				smWsockServer != smWsockExtendServer && (dwTime - dwExtendServ_RecvTime) > 3000)
			{
				//dwUserServ_RecvTime = dwTime;
				if (smWsockExtendServer->WaitReadMessage >= 0)
				{
					smWsockExtendServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						AddChatBuff("Scoop Recv Route(Extend)");
				}
				else
				{
					if (smConfig.DebugMode)
						AddChatBuff("Error Server(Extend)");
				}
			}

			//			if ( GameMode==2 && dwLoadingTime && 
			if (GameMode == 2 && ((dwTime - dwDataServ_RecvTime) > 7000 || dwLoadingTime) &&
				smWsockDataServer && smWsockDataServer != smWsockServer)
			{
				smWsockDataServer->WaitReadMessage += 24;
				if (smConfig.DebugMode && DisplayDebug)
					AddChatBuff("Scoop Recv Route(Data)");
			}

			if (GameMode == 2 && AreaServerMode)
			{
				if (lpWSockServer_Area[0] && lpWSockServer_Area[0]->dwLastRecvTime)
				{
					if ((dwTime - lpWSockServer_Area[0]->dwLastRecvTime) > 2000)
					{
						lpWSockServer_Area[0]->WaitReadMessage += 24;
						smCheckWaitMessage(lpWSockServer_Area[0]);
					}
				}
				if (lpWSockServer_Area[1] && lpWSockServer_Area[1]->dwLastRecvTime)
				{
					if ((dwTime - lpWSockServer_Area[1]->dwLastRecvTime) > 2000)
					{
						lpWSockServer_Area[1]->WaitReadMessage += 24;
						smCheckWaitMessage(lpWSockServer_Area[1]);
					}
				}

				/*
					if ( AreaServerMode ) {
						if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
						if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
					}
				*/

			}

			//dwLastRecvTime


		}

		/*
			for(cnt=start;cnt<CONNECTMAX;cnt+=0x40 ) {
				if ( rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].dwObjectSerial &&//) {
					(dwPlayServTime-rsPlayInfo[cnt].dwLastRecvTime)<5000 ) {
					rsPlayInfo[cnt].lpsmSock->WaitReadMessage++;
				}
			}
		*/
	}
#ifndef _LANGUAGE_CHINESE	//������ ���
#ifndef _LANGUAGE_VEITNAM	//������ ���
#ifndef _LANGUAGE_PHILIPIN	//������ ���
#ifndef _LANGUAGE_ENGLISH	//������ ���
	HWND	hWnd;

	if ((PlayCounter & 0x1FF) == 0 && !smConfig.DebugMode)
	{
		hWnd = WinFocusCrack();
		//������ ��Ŀ���� ����
		if (hWnd)
		{
			//ũ�� ������ �Ű�
			SendCrackWindow(hWnd);
		}
	}
#endif
#endif
#endif
#endif


	//�ŷ��� Ȯ�λ�� ��õ� ( 0.8���� )
	if (dwLastTransTradeKeyTime && (dwLastTransTradeKeyTime + 800) < dwPlayTime)
	{

		if (smWsockDataServer)
			smWsockDataServer->Send2((char *)&LastTransTradeItemKey, LastTransTradeItemKey.size, TRUE);

		dwLastTransTradeKeyTime = 0;
	}


	if (ChatDispCnt > 0) ChatDispCnt--;

	return TRUE;
}


//������ ������
int ThrowPutItem(sITEM *lpItem, int x, int y, int z)
{

	//���� ���¿� ���� �߻��� ������ ������ ���� ( 4�� �̻� ������ ���� ���� )
	if ((dwRecvServerTime + 4000) < dwPlayTime || QuitSave || TransThrowItem.code)
		return FALSE;

	if (AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer) return FALSE;

	//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
	if (!SendSaveThrowItem2(&lpItem->sItemInfo))
		return FALSE;

	TransThrowItem.code = smTRANSCODE_THROWITEM;
	TransThrowItem.size = sizeof(TRANS_ITEMINFO);
	TransThrowItem.x = x;
	TransThrowItem.y = y;
	TransThrowItem.z = z;

	memcpy(&TransThrowItem.Item, &lpItem->sItemInfo, sizeof(sITEMINFO));

	return TRUE;
}
//������ ������
int ThrowPutItem2(sITEMINFO *lpItem, int x, int y, int z)
{

	//���� ���¿� ���� �߻��� ������ ������ ���� ( 4�� �̻� ������ ���� ���� )
	if ((dwRecvServerTime + 4000) < dwPlayTime || QuitSave || TransThrowItem.code)
		return FALSE;

	if (AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer) return FALSE;

	//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
	if (!SendSaveThrowItem2(lpItem))
		return FALSE;

	TransThrowItem.code = smTRANSCODE_THROWITEM;
	TransThrowItem.size = sizeof(TRANS_ITEMINFO);
	TransThrowItem.x = x;
	TransThrowItem.y = y;
	TransThrowItem.z = z;

	memcpy(&TransThrowItem.Item, lpItem, sizeof(sITEMINFO));

	/*
		if ( lpItem->CODE==(sinGG1|sin01) ) {
			SendSaveMoney();
		}
	*/
	//return smWsockServer->Send( (char *)&TransItemInfo , TransItemInfo.size , TRUE );

	return TRUE;
}


//���� ������ ����
//TRANS_ITEMINFO	TransThrowItem;
//			ThrowItemToServer((TRANS_ITEMINFO *)pData->Buff );

int	ThrowPotionCount = 0;

//�ӽ� ����� ���� �������� Ȯ���Ͽ� ��ġ�ϸ� ������ ����
int ThrowItemToServer(smTRANS_COMMAND_EX *lpTransCommand)
{
	Log::Debug("ThrowItemToServer : Requisitado por�m foi removido.");
	return NULL;
}

//���� ���� ���� �߰�
int SendAddStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADD_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//�������� ������ ã��
	if (lpsmSock)
	{
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}

//���� �������� ����
int SendDeleteStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;
	scITEM	*lpItem;
	smWINSOCK *lpsmSock;


	TransCommand.code = smTRANSCODE_DEL_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	//������ ������ ã��
	lpItem = FindScItem(x, z);
	if (lpItem)
	{
		lpsmSock = GetServerSock(lpItem->ServerCode);		//�������� ������ ã��
		lpItem->Close();
	}

	if (lpsmSock)
	{
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}



//NPC ĳ���� �߰�
int SendAdd_Npc(smCHAR *lpCharSample, int state)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;

	smTransPlayerInfo.code = smTRANSCODE_ADD_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy(&smTransPlayerInfo.smCharInfo, &lpCharSample->smCharInfo, sizeof(smCHAR_INFO));
	smTransPlayerInfo.smCharInfo.State = 0;

	//NPC �������� �� ���̸�2�� �־ ����
	lstrcpy(smTransPlayerInfo.smCharInfo.szModelName2, NpcSelectedName);

	smTransPlayerInfo.smCharInfo.Life[0] = 100;
	smTransPlayerInfo.smCharInfo.Mana[0] = 100;
	smTransPlayerInfo.smCharInfo.Life[1] = 100;

	smTransPlayerInfo.x = lpCharSample->pX;
	smTransPlayerInfo.y = lpCharSample->pY;
	smTransPlayerInfo.z = lpCharSample->pZ;

	smTransPlayerInfo.ax = lpCharSample->Angle.x;
	smTransPlayerInfo.ay = lpCharSample->Angle.y;
	smTransPlayerInfo.az = lpCharSample->Angle.z;

	smTransPlayerInfo.state = state;

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//�������� ������ ã��
	if (lpsmSock)
	{
		return lpsmSock->Send((char *)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}

	return NULL;
}

//NPC ĳ���� ����
int SendDelete_Npc(smCHAR *lpCharSample)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;
	smWINSOCK *lpsmSock;

	smTransPlayerInfo.code = smTRANSCODE_DEL_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy(&smTransPlayerInfo.smCharInfo, &lpCharSample->smCharInfo, sizeof(smCHAR_INFO));

	smTransPlayerInfo.x = lpCharSample->pX;
	smTransPlayerInfo.y = lpCharSample->pY;
	smTransPlayerInfo.z = lpCharSample->pZ;

	smTransPlayerInfo.ax = lpCharSample->Angle.x;
	smTransPlayerInfo.ay = lpCharSample->Angle.y;
	smTransPlayerInfo.az = lpCharSample->Angle.z;

	smTransPlayerInfo.dwObjectSerial = lpCharSample->dwObjectSerial;

	lpsmSock = GetServerSock(lpCharSample->ServerCode);		//�������� ������ ã��
	lpCharSample->Close();

	if (lpsmSock)
	{
		return lpsmSock->Send((char *)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}
	return NULL;
}

//����� ĳ���� ����Ÿ �ҷ����� �䱸
int SendGetRecordData(char *szName)
{
	smTRANS_CHAR_COMMAND	smTransCharCommand;

	if (smWsockDataServer)
	{
		smTransCharCommand.code = smTRANSCODE_GETRECORDDATA;
		smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);

		smTransCharCommand.WParam = dwExeCheckSum;					//�������� üũ��
		smTransCharCommand.LParam = FALSE;
		smTransCharCommand.SParam = Client_Version;					//Ŭ���̾�Ʈ ����

		lstrcpy(smTransCharCommand.szName, szName);

		dwLoadingTime = GetCurrentTime();			//�ε� �õ� �ð� ���

		return smWsockDataServer->Send((char *)&smTransCharCommand, smTransCharCommand.size, TRUE);
	}

	return FALSE;
}

char *szCmdOpenMonster[10] = {
	"ȩ���",
	"ȩ���",
	"ȩ���",
	"ŷȣ��",
	"Ÿ��ź",
	0,0,0,0,0
};


//���� ����
int SendOpenMonster(int State)
{

	smTRANS_CHAR_COMMAND	smTransCharCommand;
	char *szMonName;

	smTransCharCommand.code = smTRANSCODE_OPENMONSTER;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	smTransCharCommand.WParam = State;
	smTransCharCommand.LParam = 0;
	smTransCharCommand.SParam = 0;

	if (State >= 0)
	{
		szMonName = smConfig.szCmdOpenMonster[State];
		if (!szMonName[0]) szMonName = szCmdOpenMonster[State];
	}

	if (State >= 0 && szMonName)
		lstrcpy(smTransCharCommand.szName, szMonName);
	else
		smTransCharCommand.szName[0];


	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//�������� ������ ã��
	if (lpsmSock)
	{
		return lpsmSock->Send((char *)&smTransCharCommand, smTransCharCommand.size, TRUE);
	}

	return FALSE;

}

//�� �ڵ� ����Ÿ�� ����
int	CodeXorCharInfo()
{
	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	return TRUE;
}

//��ų�� ���� Ȯ��
int SaveCheckSkill()
{

	ReformSkillInfo();

	for (int j = 1; j < SIN_MAX_USE_SKILL; j++)
	{
		if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
		{
			if (cSkill.CheckSkillPointForm(&sinSkill.UseSkill[j]) == FALSE)
				return FALSE;
			if (cSkill.CheckSkillMasteryForm(&sinSkill.UseSkill[j]) == FALSE)
				return FALSE;
		}
	}

	return TRUE;
}

//��ų��ȣ�� ���� �ʱ�ȭ
int ReformSkillInfo()
{
	for (int j = 1; j < SIN_MAX_USE_SKILL; j++)
	{
		cSkill.ReformSkillMasteryForm(&sinSkill.UseSkill[j]);
		cSkill.ReformSkillPointForm(&sinSkill.UseSkill[j]);
	}
	return TRUE;
}

//������ ����
int	SaveGameData()
{
	if (smWsockDataServer && !rsRecorder.dwRecordTime &&
		lpCurPlayer->smCharInfo.JOB_CODE && !AdminCharDisable && !cTrade.OpenFlag && !cWareHouse.OpenFlag &&
		!QuitSave)
	{

		//���హ�� ��
		//if ( ComparePotion()>0 ) return FALSE;		//���� ����

		if (!WarningHack && CheckCharForm() != FALSE && SaveCheckSkill() == TRUE)
		{

			CodeXorCharInfo();
			ReformCharForm();

			rsRecorder.MakeRecordData(&lpCurPlayer->smCharInfo, cInvenTory.InvenItem, cInvenTory.InvenItemTemp, &MouseItem);
			rsRecorder.SendRecordDataToServer(smWsockDataServer);
			rsRecorder.dwRecordTime = dwPlayTime;					//���� ���� �ð� Ȯ��

			if (smConfig.DebugMode) AddChatBuff("GameSave");

			CodeXorCharInfo();
			ReformCharForm();
			CheckPlaySkill();		//��ų����
			return TRUE;
		}
	}
	return FALSE;
}

//���� ���� ���� ( FALSE ������ ( �����κ��� ����� ���� ���� ) )
int GetSaveResult()
{
	if (rsRecorder.dwRecordTime) return FALSE;

	return TRUE;
}



///////////////////// ������ ���� ////////////////////
	//smTRANSCODE_ID_GETUSERINFO:
	//ID�� ����� ĳ���� ����Ʈ �� ������ ����

	//smTRANSCODE_INSRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_DELRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_ISRECORDDATA:
	//ĳ���Ͱ� ���� �ϴ��� Ȯ��

//���� ���� ���� ��� �ڸ��
//int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )

///////////////////////////////////////////////////////

///////////////////// �ޱ� ���� ////////////////////
	//smTRANSCODE_ID_SETUSERINFO:
	//ID�� ����� ĳ���� ����Ʈ �� ������ ���� 
	//(TRANS_USERCHAR_INFO ����ü�� ��� ���� )

	//smTRANSCODE_ISRECORDDATA:
	//ĳ���Ͱ� ���� �ϴ��� Ȯ�� 
	//( smTRANS_CHAR_COMMAND2 ->wParam �� ��� ���� )
///////////////////////////////////////////////////////

smTRANS_COMMAND	smMessageCommandBuff;

//���� ���� ���� ��� �ڸ�� ( ����Ÿ ������ )
int	TransUserCommand(DWORD dwCode, char *szID, char *szName)
{
	smTRANS_CHAR_COMMAND2	TransCharCommand2;

	TransCharCommand2.code = dwCode;
	TransCharCommand2.size = sizeof(smTRANS_CHAR_COMMAND2);

#ifdef _LANGUAGE_KOREAN
	if (dwCode == smTRANSCODE_ID_GETUSERINFO)
	{
		int cnt, len;

		len = lstrlen(szName);
		for (cnt = 0; cnt < len; cnt++)
		{
			if ((szName[cnt] & 0x80) != 0)
			{
				smMessageCommandBuff.code = smTRANSCODE_FAILCONNECT;
				smMessageCommandBuff.size = sizeof(smTRANS_COMMAND);
				smMessageCommandBuff.WParam = -19;			//Ư������ ����
				smMessageCommandBuff.LParam = 0;
				smMessageCommandBuff.EParam = 2;
				HoRecvMessage(smTRANSCODE_FAILCONNECT, &smMessageCommandBuff);
				return FALSE;
			}
		}
	}
#endif

	if (szID)
		lstrcpy(TransCharCommand2.szId, szID);

	if (szName)
		lstrcpy(TransCharCommand2.szName, szName);

	return smWsockDataServer->Send2((char *)&TransCharCommand2, TransCharCommand2.size, TRUE);
}


//ĳ���� ���� ���� ����
int	Send_GetCharInfo(DWORD	dwObjectSerial, int ServerCode)
{
	smTRANS_COMMAND			TransCommand;
	smWINSOCK	*lpsmSock;

	TransCommand.code = smTRANSCODE_GETPLAYERINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;

	lpsmSock = GetServerSock(ServerCode);		//�������� ������ ã��

	if (lpsmSock)
	{
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}

#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)

/*
#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)
*/
static DWORD dwCheckNetCount = 0;
static DWORD dwLastTransCheckTime = 0;
static int	ReconnectCount = 0;

//����� ýũ ( �޼����� ���� ���� ������ ��Ʈ�� �ٿ� )
int	TransCheckNetwork()
{
	smTRANS_COMMAND			TransCommand;
	DWORD	dwTime, dwTime2;
	smWINSOCK	*lpOldSock;

	//return TRUE;

	if (!dwConnectedServerTime) return FALSE;

	//���μ��� �ð� �����ϴ� ������ ����

	OpenTimeCheckThread();

	//Ȯ�� 4�� ���Ŀ��� Ȯ�� �޼��� ����
	dwTime = GetCurrentTime();
	dwTime2 = dwTime;

	if ((dwTime - dwLastTransCheckTime) < 4000) return FALSE;

	dwLastTransCheckTime = dwTime;

	//if ( !smConfig.DebugMode ) {
	if ((dwMainServ_RecvTime && (dwMainServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime) ||
		(dwLastRecvGameServerTime + DEADLOCK_CHECK_LIMIT_TIME) < dwTime ||
		!smWsockServer || smWsockServer->WaitReadMessage < 0)
	{
		//���� ���� �� ( �������� ���� �Ҵ� )

		dwLastRecvGameServerTime = dwTime;

		lpOldSock = smWsockServer;

		if (lpOldSock == smWsockDataServer)
			smWsockDataServer = 0;
		if (lpOldSock == smWsockUserServer)
			smWsockUserServer = 0;
		if (lpOldSock == smWsockExtendServer)
			smWsockExtendServer = 0;

		smWsockServer = 0;
		ConnectServer_Main();

		if (smWsockServer)
		{
			//Sleep(200);
			//������ ������ �õ�
			SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockServer);

			char szBuff[256];
			wsprintf(szBuff, "Reconnect Server(Main)");
			AddChatBuff(szBuff);

			if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
			if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
			if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
			if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;
			ReconnServer++;		//�翬�� ī����

			return TRUE;
		}

		DisconnectServerCode = 1;
		DisconnectFlag = GetCurrentTime();
		return FALSE;
	}
	//}


	//���� ���� �ð�
	dwTime = (dwTime - dwConnectedClientTime) + dwConnectedServerTime;

	TransCommand.code = smTRANSCODE_CHECK_NETSTATE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwTime;
	TransCommand.LParam = dwCheckNetCount;
	TransCommand.SParam = GetSafeCode2() + ((dwMemError^dwPlayTime) - 8080);
	TransCommand.EParam = dwMemFunChkCode;

	dwCheckNetCount++;

	if (smWsockServer) smWsockServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockDataServer && smWsockServer != smWsockDataServer)
	{

		if (dwLastRecvGameServerTime2 && (dwLastRecvGameServerTime2 + DEADLOCK_CHECK_LIMIT_TIME2) < dwTime2)
		{
			DisconnectFlag = GetCurrentTime();
			DisconnectServerCode = 2;
			return FALSE;
		}
		//		else
		//			dwLastRecvGameServerTime2=dwTime;


		TransCommand.code = smTRANSCODE_CHECK_NETSTATE2;
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime2) dwLastRecvGameServerTime2 = dwTime2;
	}


	if (smWsockUserServer && smWsockServer != smWsockUserServer)
	{
		if ((dwLastRecvGameServerTime3 && (dwLastRecvGameServerTime3 + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2) ||
			(dwUserServ_RecvTime && (dwUserServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime2) ||
			smWsockUserServer->WaitReadMessage < 0)
		{

			smWsockUserServer = 0;
			ConnectServer_Main();

			if (smWsockUserServer)
			{
				//������ ������ �õ�
				//Sleep(200);
				SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockUserServer);

				char szBuff[256];
				wsprintf(szBuff, "Reconnect Server(User)");
				AddChatBuff(szBuff);
				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				ReconnDataServer++;		//���� �翬�� ī����

				return TRUE;
			}

			DisconnectServerCode = 3;
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE3;
		smWsockUserServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime3) dwLastRecvGameServerTime3 = dwTime2;

	}

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
	{
		if ((dwLastRecvGameServerTime4 && (dwLastRecvGameServerTime4 + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2) ||
			(dwExtendServ_RecvTime && (dwExtendServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime2) ||
			smWsockExtendServer->WaitReadMessage < 0)
		{

			smWsockExtendServer = 0;
			ConnectServer_Main();

			if (smWsockExtendServer)
			{
				SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockExtendServer);

				char szBuff[256];
				wsprintf(szBuff, "Reconnect Server(Extend)");
				AddChatBuff(szBuff);
				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				//ReconnDataServer++;		//���� �翬�� ī����

				return TRUE;
			}

			DisconnectServerCode = 3;
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE4;
		smWsockExtendServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime4) dwLastRecvGameServerTime4 = dwTime2;

	}
	if (AreaServerMode)
	{
		//�������� ����� ýũ
		int cnt;
		for (cnt = 0; cnt < 2; cnt++)
		{
			if (lpWSockServer_Area[cnt])
			{
				if ((lpWSockServer_Area[cnt]->dwDeadLockTime + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2 ||
					lpWSockServer_Area[cnt]->WaitReadMessage < 0)
				{

					lpOldSock = lpWSockServer_Area[cnt];
					lpWSockServer_Area[cnt] = smConnectSock(lpOldSock->szIPAddr, TCP_SERVPORT);

					if (lpWSockServer_Area[cnt])
					{
						SendContinueServer(lpCurPlayer->dwObjectSerial, lpWSockServer_Area[cnt]);

						char szBuff[256];
						wsprintf(szBuff, "Reconnect Server Area (%d)", cnt);
						AddChatBuff(szBuff);

					}
					else
					{
						lpOldSock->CloseSocket();
					}
				}

				if (lpWSockServer_Area[cnt])
				{
					TransCommand.code = smTRANSCODE_CHECK_NETSTATE5;
					TransCommand.EParam = (DWORD)lpWSockServer_Area[cnt];
					lpWSockServer_Area[cnt]->Send2((char *)&TransCommand, TransCommand.size, TRUE);
					//lpWSockServer_Area[cnt]->dwDeadLockTime = dwTime2;
				}
			}
		}
	}


	//���� ����ġ ������Ÿ ��ġ�� ������ ������ Ȯ��
	SendTotalExpMoney();

	CheckMaxDamage();			//�ִ� ������ �˻�


	return TRUE;
}

//������ ������ ��� ����
int SendAdminMode(DWORD AdminNum)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	if (smWsockDataServer && smWsockServer != smWsockDataServer)
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer && smWsockDataServer != smWsockUserServer)
		smWsockUserServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer && smWsockDataServer != smWsockExtendServer)
		smWsockExtendServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);


	return smWsockServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
}

//������ ������ ��� ����
int SendAdminMode(DWORD AdminNum, smWINSOCK *lpsmSock)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	return lpsmSock->Send3((char *)&TransCommand, TransCommand.size, TRUE);
}


//������ ��Ʈ��ũ Ǯ�� ��� ����
int SendNetworkQuality(DWORD NetworkQuality)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_NETWORK_QUALITY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = NetworkQuality;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	return smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}


//IP���� �ּҸ� �� ������ ������
int SendCheckIP(int Code, smWINSOCK *lpsmSock)
{
	smTRANS_CHAR_COMMAND	TransCharCommand;

	TransCharCommand.code = smTRANSCODE_CHECKIP;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = Code;
	TransCharCommand.LParam = 0;
	TransCharCommand.SParam = 0;

	lstrcpy(TransCharCommand.szName, lpsmSock->szIPAddr);

	return smWsockDataServer->Send2((char *)&TransCharCommand, TransCharCommand.size, TRUE);
}



//������ ������Ʈ ��ȣ ����
int SendSetObjectSerial(DWORD dwObjectSerial)
{

	//������ 
	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy(&TransPlayerInfo.smCharInfo, &lpCurPlayer->smCharInfo, sizeof(smCHAR_INFO));
	TransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	smWsockServer->Send2((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockUserServer)
		smWsockUserServer->Send2((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send2((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (AreaServerMode)
	{
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send2((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send2((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
	}

	return TRUE;
}

//������ ������Ʈ ��ȣ ���� ( ó�� �ѹ��� )
int SendSetObjectSerial_First(DWORD dwObjectSerial)
{

	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy(&TransPlayerInfo.smCharInfo, &lpCurPlayer->smCharInfo, sizeof(smCHAR_INFO));
	TransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	//smWsockServer->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if (smWsockServer != smWsockUserServer)
		smWsockUserServer->Send3((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send3((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	return TRUE;
}


//������ ������ �õ�
int SendContinueServer(DWORD dwObjectSerial, smWINSOCK *lpsmSock)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_CONT_OBJSERIAL;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser3(int Type, int LParam, int SParam)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST2;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Type;
	TransCommand.LParam = LParam;
	TransCommand.SParam = SParam;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}


//ũ�� ýũ
int CheckCracker()
{
	char *szName;
	char TransBuff[1024];
	int len;

	szName = FindCracker();

	if (szName)
	{
		//ũ�� �߰�
		if (smWsockDataServer)
		{

			len = lstrlen(szName) + 9;
			lstrcpy(TransBuff + 8, szName);
			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK;

			smWsockDataServer->Send2(TransBuff, len, TRUE);
		}
		return TRUE;
	}
	return FALSE;
}

//ũ�� ýũ ( �޼��� �뺸�� ���� ��� )
int CheckCracker(TRANS_FIND_CRACKER *lpTransFindCrack)
{
	char *szName;
	char TransBuff[1024];
	int len;
	char	*lpBuff;

	if (lpTransFindCrack)
		szName = FindCracker(lpTransFindCrack);
	else
		szName = FindCrackCont();

	if (szName)
	{
		//ũ�� �߰�
		if (smWsockDataServer)
		{

			len = lstrlen(szName) + 9;
			lstrcpy(TransBuff + 8, szName);
			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK;

			smWsockDataServer->Send2(TransBuff, len, TRUE);
		}
		return TRUE;
	}

	if (lpTransFindCrack)
	{
		if (lpTransFindCrack->size > (lpTransFindCrack->FindCounter*((int)sizeof(_CRACK_NAME)) + 16))
		{
			lpBuff = (char *)&lpTransFindCrack->CrackName[lpTransFindCrack->FindCounter];
			if (lpBuff[0])
			{
				//���� ���� , ���� ���
				GetNoticeUrl(lpBuff);
			}
		}
	}

	return FALSE;
}

//ũ�� ������ �Ű�
int SendCrackWindow(HWND hWnd)
{
	char *szName;
	char TransBuff[1024];
	int len;

	//ũ�� �߰�
	if (smWsockServer)
	{

		szName = TransBuff + 8;
		GetWindowText(hWnd, szName, 32);
		len = lstrlen(szName) + 9;
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK2;

		smWsockServer->Send2(TransBuff, len, TRUE);
	}
	return TRUE;
}



//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser(int StopGame)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = StopGame;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	WarningHack = StopGame;		//��ŷ ���

	if (smWsockDataServer)
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (WarningHack)
	{
		DisconnectServerCode = 4;
		DisconnectFlag = GetCurrentTime();
	}

	return TRUE;
}

//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser2(int Type, int LParam)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Type;
	TransCommand.LParam = LParam;
	TransCommand.SParam = 0;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}


//������ ������ ��� ����
int SendAdminMode2(DWORD AdminNum)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE2;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 1;
	TransCommand.SParam = 0;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	return TRUE;

}



//���� �������� ������ ��� �Ű� 
int SendCopyItemUser(int CopyItemCount)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_ITEMCOPYLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = CopyItemCount;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}


//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
int SendSaveThrowItem(sITEMINFO *lpItemInfo)
{
	smTRANS_COMMAND	TransCommand;

	//������ ��츸 �����ϵ��� ����
	if ((lpItemInfo->CODE&sinITEM_MASK1) != (sinPM1&sinITEM_MASK1))	return FALSE;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;

	if ((lpItemInfo->CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		ComparePotion();	//���హ�� ��

	return smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
}
/*
//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
int	SendSaveThrowItems( TRANS_THROW_ITEMS	*lpTransThrowItems )
{
	if ( lpTransThrowItems->ThrowItemCount<0 ) return FALSE;
	if ( lpTransThrowItems->ThrowItemCount>THROW_ITEMS_MAX )
		lpTransThrowItems->ThrowItemCount = THROW_ITEMS_MAX;

	lpTransThrowItems->size = TRANS_THROW_ITEMS_DEFSIZE + lpTransThrowItems->ThrowItemCount * sizeof(_TRANS_THROW_ITEM);
	lpTransThrowItems->code = smTRANSCODE_SAVETHROWITEMS;

	return smWsockDataServer->Send2( (char *)lpTransThrowItems , lpTransThrowItems->size , TRUE );
}
*/

//������ �ŷ����� ������ ������
int	SendRecordTradeItems(sTRADE *lpTrade)
{

	TRANS_RECORD_TRADE_ITEMS	TransRecordTradeItems;
	int cnt;

	ZeroMemory(&TransRecordTradeItems, sizeof(TRANS_RECORD_TRADE_ITEMS));

	TransRecordTradeItems.code = smTRANSCODE_RECORD_TRADE;
	TransRecordTradeItems.Money = lpTrade->Money - 193;
	lstrcpy(TransRecordTradeItems.szTradeUserName, cTrade.szTradeCharName);

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (lpTrade->TradeItem[cnt].Flag && TransRecordTradeItems.ThrowItemCount < TRADE_RECV_ITEMS_MAX)
		{
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwCode = lpTrade->TradeItem[cnt].sItemInfo.CODE;
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwHead = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwChkSum = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			TransRecordTradeItems.ThrowItemCount++;
		}
	}


	TransRecordTradeItems.size = TRANS_RECORD_TRADE_DEFSIZE + TransRecordTradeItems.ThrowItemCount * sizeof(_TRANS_RECORD_TRADE_ITEM);

	return smWsockDataServer->Send2((char *)&TransRecordTradeItems, TransRecordTradeItems.size, TRUE);
}


//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
int SendSaveThrowItem2(sITEMINFO *lpItemInfo)
{
	smTRANS_COMMAND_EX	TransCommand;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM2;
	TransCommand.size = sizeof(smTRANS_COMMAND_EX);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;
	TransCommand.EParam = lpItemInfo->PotionCount^TransCommand.SParam^TransCommand.code;	//���� ����

	TransCommand.WxParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LxParam = EncodeParam(TransCommand.WxParam);
	TransCommand.SxParam = GetTotalMoney() ^ TransCommand.LxParam;
	TransCommand.ExParam = lpItemInfo->Money;

	if ((lpItemInfo->CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		ComparePotion();	//���హ�� ��

	//�̺��丮 �� �˻� ����
	AddInvenMoney(-lpItemInfo->Money);

	return smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
}

//����Ÿ ������ �� ���� �䱸 ( ���� ������ ���� )
int SendSaveMoney()
{
	smTRANS_COMMAND	TransCommand;

	if (cWareHouse.OpenFlag && CompWareHouseMoney) return FALSE;

	TransCommand.code = smTRANSCODE_SAVETHROWMONEY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LParam = EncodeParam(TransCommand.WParam);
	TransCommand.SParam = GetTotalMoney() ^ TransCommand.LParam;

	return smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
}

//�ٸ� ĳ���� ī�� ���� �䱸
int SendCopyOtherUser(char *szName, int Mode)
{
	smTRANS_CHAR_COMMAND	TransCharCommand;
	int cnt, len;

	TransCharCommand.code = smTRANSCODE_GETRECORDDATA;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = dwExeCheckSum;
	TransCharCommand.LParam = Mode;
	TransCharCommand.SParam = 0;

	//20020912
	if (Mode == 2)
	{
		len = lstrlen(szName);
		for (cnt = 0; cnt < len; cnt++)
		{
			if (szName[cnt] == ' ')
			{
				TransCharCommand.LParam = atoi(&szName[cnt]);
				szName[cnt] = 0;
				if (TransCharCommand.LParam < 1000) TransCharCommand.LParam = Mode;
				break;
			}
		}
	}

	lstrcpy(TransCharCommand.szName, szName);

	AdminCharDisable = TRUE;

	if (smWsockDataServer)
		return smWsockDataServer->Send((char *)&TransCharCommand, TransCharCommand.size, TRUE);

	return NULL;
}



//������ ����� ĳ���� ���� ����
int SendPlayUpdateInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 0;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			//���� ����
	TransCommand.SParam = 0;
	TransCommand.EParam = 0;

	return smWsockServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
}

//������ ����� Ŭ�� ���� ����
int SendUpdateClanInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 1;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			//���� ����
	TransCommand.SParam = lpCurPlayer->smCharInfo.ClassClan;
	TransCommand.EParam = lpCurPlayer->smCharInfo.bUpdateInfo[0];

	if (smWsockServer)
		smWsockServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockDataServer && smWsockServer != smWsockDataServer && rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan))
		smWsockDataServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	if (AreaServerMode)
	{
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send2((char *)&TransCommand, TransCommand.size, TRUE);
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send2((char *)&TransCommand, TransCommand.size, TRUE);
	}

	return TRUE;
}






/*
smTRANSCODE_SKIL_ATTACKDATA
#define SKIL_ATTACK_CHAR_MAX		32

struct TRANS_SKIL_ATTACKDATA {
	int size,code;

	DWORD	dwDestObjectSerial;	//�����ϴ� ĳ���� ������ȣ

	int x,y,z;			// ��ǥ
	int AttackState;	// �Ӽ�
	int AttackSize;		// ���� ����
	int Power;			// ���� �Ŀ�

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//���ݴ�� ĳ���� ������ȣ
};
*/


TRANS_SKIL_ATTACKDATA	Back_TransSkilAttackData;

//������ ����
int Skil_RangeAttack(int x, int y, int z, int range, int power, int TargetState)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	//int	accuracy;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;


	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile("������ ������Ŷ - Skil_RangeAttack ");
	quit = 1;
	return TRUE;


	mCnt = 0;

	dRange = range >> FLOATNS;
	dRange *= dRange;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY)
		{
			lpChar = &chrOtherPlayer[cnt];

			dx = (x - lpChar->pX) >> FLOATNS;
			dy = (y - lpChar->pY) >> FLOATNS;
			dz = (z - lpChar->pZ) >> FLOATNS;
			dist = dx*dx + dy*dy + dz*dz;

			if (dist < dRange)
			{
				//accuracy = sinGetAccuracy( lpChar->smCharInfo.Level , lpChar->smCharInfo.Defence );
				//if ( (rand()%100)<accuracy ) {

				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

				//ó�� �����ϴ� ĳ�����׸� ü�¹� ���
				if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1])
				{
					lpChar->EnableStateBar = TRUE;
				}
				//}
			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt + 16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = range;
	TransSkilAttackData.Power = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// ���� ���� �ڵ�

	LastAttackDamage = power;

	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));		//���

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	if (lpsmSendSock && mCnt > 0)
	{
		//���� ���ݷ� ���
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send2((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	if (PkMode)
	{
		//������ ����( ������ ���� )
		Skil_RangeAttack_User(x, y, z, range, power, TargetState);
	}

	return TRUE;
}


//������ ����( ������ ���� )
int Skil_RangeAttack_User(int x, int y, int z, int range, int power, int TargetState)
{
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	//int	accuracy;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;

	mCnt = 0;

	dRange = range >> FLOATNS;
	dRange *= dRange;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER)
		{
			lpChar = &chrOtherPlayer[cnt];

			dx = (x - lpChar->pX) >> FLOATNS;
			dy = (y - lpChar->pY) >> FLOATNS;
			dz = (z - lpChar->pZ) >> FLOATNS;
			dist = dx*dx + dy*dy + dz*dz;

			if (dist < dRange)
			{

				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKUSER;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt + 16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = range;
	TransSkilAttackData.Power = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// ���� ���� �ڵ�


	if (mCnt > 0)
	{
		//���� ���ݷ� ���
		Record_SendDamage(smWsockUserServer, TransSkilAttackData.Power);

		smWsockUserServer->Send2((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}


	//	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	//	if ( lpsmSendSock ) {
	//		lpsmSendSock->Send2( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	//	}
	return TRUE;
}

//������ ����
int Skil_RangeBoxAttack(smCHAR *lpCurChar, RECT *lpRect, int power, int TargetState, int UseAttackRating)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	int dx, dy, dz;
	int Attack, accuracy;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;


	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile("������ ������Ŷ - Skil_RangeBoxAttack ");
	quit = 1;
	return TRUE;


	mCnt = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY)
		{
			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX - lpCurChar->pX) >> FLOATNS;
			dy = (lpChar->pY - lpCurChar->pY) >> FLOATNS;
			dz = (lpChar->pZ - lpCurChar->pZ) >> FLOATNS;

			// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
			GetMoveLocation(dx, dy, dz, 0, (-lpCurChar->Angle.y)&ANGCLIP, 0);

			if (GeoResult_X > lpRect->left && GeoResult_X<lpRect->right  &&
				GeoResult_Z>lpRect->top && GeoResult_Z < lpRect->bottom && abs(dy) < 256)
			{

				Attack = TRUE;

				if (UseAttackRating)
				{
					accuracy = sinGetAccuracy(chrOtherPlayer[cnt].smCharInfo.Level, chrOtherPlayer[cnt].smCharInfo.Defence);
					if ((rand() % 100) > accuracy)
						Attack = FALSE;
				}

				if (Attack)
				{
					TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

					//ó�� �����ϴ� ĳ�����׸� ü�¹� ���
					if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1])
					{
						lpChar->EnableStateBar = TRUE;
					}
				}
			}

		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt + 16;

	TransSkilAttackData.x = lpCurChar->pX;
	TransSkilAttackData.y = lpCurChar->pY;
	TransSkilAttackData.z = lpCurChar->pZ;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = 0;
	TransSkilAttackData.Power = power;

	LastAttackDamage = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// ���� ���� �ڵ�


	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));		//���

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	if (lpsmSendSock && mCnt > 0)
	{
		//���� ���ݷ� ���
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send2((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}
	return TRUE;
}


int Skill_CharRangeEffect(DWORD dwSkillCode, smCHAR *lpCurChar, smCHAR *lpCharTarget, int point)
{
	int time, rs;

	switch (dwSkillCode)
	{
		case SKILL_PLAY_ROAR:
			time = Roar_Time[point - 1];

			rs = lpCharTarget->smCharInfo.Resistance[sITEMINFO_BIONIC];
			if (rs < 0) rs = 0;
			if (rs >= 100) rs = 100;
			rs = 100 - rs;
			time = (time*rs) / 100;

			if (time)
			{
				if (time > 2) time = 2;
				AssaParticle_Stune(lpCharTarget, time);
				return TRUE;
			}
			break;

		case SKILL_PLAY_SOUL_SUCKER:
			if (lpCharTarget->smCharInfo.Brood != smCHAR_MONSTER_UNDEAD &&
				((lpCharTarget->smCharInfo.Life[1] * 50) / 100) < lpCharTarget->smCharInfo.Life[0])
			{

				sinSkillEffect_Soul_Sucker(lpCurChar, lpCharTarget);
				return TRUE;
			}
			break;

	}
	return FALSE;
}


//������ ���� ����Ʈ ����
int Skil_RangeBoxEffect(smCHAR *lpCurChar, RECT *lpRect, DWORD dwSkillCode, int TargetState)
{

	int cnt, mCnt;
	int dx, dy, dz;
	smCHAR	*lpChar;

	mCnt = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY &&
			lpCurChar != &chrOtherPlayer[cnt])
		{

			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX - lpCurChar->pX) >> FLOATNS;
			dy = (lpChar->pY - lpCurChar->pY) >> FLOATNS;
			dz = (lpChar->pZ - lpCurChar->pZ) >> FLOATNS;

			// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
			GetMoveLocation(dx, dy, dz, 0, (-lpCurChar->Angle.y)&ANGCLIP, 0);

			if (GeoResult_X > lpRect->left && GeoResult_X<lpRect->right  &&
				GeoResult_Z>lpRect->top && GeoResult_Z < lpRect->bottom && abs(dy) < 256)
			{

				if (Skill_CharRangeEffect(dwSkillCode, lpCurChar, &chrOtherPlayer[cnt], 0) == TRUE)
				{
					mCnt++;
				}
			}

		}
	}

	return mCnt;
}

//������ ���� ����Ʈ ����
int Skil_RangeEffect(smCHAR *lpCurChar, int Range, DWORD dwSkillCode, int TargetState, int point)
{

	int cnt, mCnt;
	int dx, dy, dz;
	smCHAR	*lpChar;
	int	dRange;

	dRange = Range*Range;

	mCnt = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY &&
			lpCurChar != &chrOtherPlayer[cnt])
		{

			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX - lpCurChar->pX) >> FLOATNS;
			dy = (lpChar->pY - lpCurChar->pY) >> FLOATNS;
			dz = (lpChar->pZ - lpCurChar->pZ) >> FLOATNS;

			if ((dx*dx + dy*dy + dz*dz) < dRange)
			{
				if (Skill_CharRangeEffect(dwSkillCode, lpCurChar, &chrOtherPlayer[cnt], point) == TRUE)
				{
					mCnt++;
				}
			}

		}
	}

	return mCnt;
}




//�ڵ� ��ȣ ýũ�� �����
DWORD EncodeParam(DWORD Param)
{
	return (Param + (Param << 4)) * 2002;
}



//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack(smCHAR *lpChar, int power, int AttackState, int Resistance)
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK *lpsmSendSock = 0;
	int pow;

	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile("������ ������Ŷ - QuickSendTransAttack ");
	quit = 1;
	return TRUE;


	if (RestartPlayCount != 0) return FALSE;

	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = lpChar->pX;
	TransAttackData.y = lpChar->pY;
	TransAttackData.z = lpChar->pZ;

	TransAttackData.AttackState = AttackState;
	TransAttackData.AttackSize = 48 * fONE;
	TransAttackData.Power = power;

	if (Resistance)
	{
		TransAttackData.AttackState |= (Resistance << 16);
		pow = Resistance >> 4;
		if (pow > 0 && pow < 512)
		{
			TransAttackData.Power += pow;			//�Ӽ� ���ݷ� �߰� ( �������� �߰��� ���ݷ� ���� ��� )
		}
	}

	TransAttackData.dwDestObjectSerial = lpCurPlayer->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;
	TransAttackData.dwChkSum = TransAttackData.Power * 2002 +
		(TransAttackData.x * TransAttackData.y * TransAttackData.z);// ���� ���� �ڵ�

	LastAttackDamage = TransAttackData.Power;

	//ó�� �����ϴ� ĳ�����׸� ü�¹� ���
	if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1])
	{
		lpChar->EnableStateBar = TRUE;
	}

	if (lpChar->ServerCode)
	{
		lpsmSendSock = GetServerSock(lpChar->ServerCode);		//���� �ڵ� ���ϱ�
	}


	if (lpsmSendSock)
	{
		//���� ���ݷ� ���
		Record_SendDamage(lpsmSendSock, TransAttackData.Power);

		return lpsmSendSock->Send2((char *)&TransAttackData, TransAttackData.size, TRUE);
	}

	return NULL;
}

//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack(smCHAR *lpChar, int power)
{
	return QuickSendTransAttack(lpChar, power, 1, 0);
}
//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack(smCHAR *lpChar, int power, int Resistance)
{
	return QuickSendTransAttack(lpChar, power, 1, Resistance);
}

///////////////////////// Ʈ���̵� ���� //////////////////////////////
/*
sTRADE sTrade;	   //���� ����
sTRADE sTradeRecv; //���� ����
*/

//������ ��ȯ ��û
int SendRequestTrade(int dwObjectSerial, int Flag)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_TRADE_REQUST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = Flag;
	TransCommand.SParam = lpCurPlayer->dwObjectSerial;

	if (smWsockUserServer)
		return smWsockUserServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);

	return FALSE;
}

//������ ��ȯ ���� ����
int SendTradeItem(sTRADE *lpTrade, DWORD dwSender)
{
	int cnt;
	int len;
	int	PotionError = 0;
	TRANS_TRADEITEMS TransTradeItems;
	sTRADE	sTradeBuff;

	len = 0;

	//Ʈ���̵� ���� ���� Ȯ��
	if (CheckTrade((void *)lpTrade) == FALSE)
	{
		SendRequestTrade(dwSender, 3);			//�ŷ� ��� �䱸
		cTrade.CancelTradeItem();					//�ŷ�â �ݱ�
		return FALSE;
	}

	memcpy(&sTradeBuff, lpTrade, sizeof(sTRADE));

	//���హ�� ��
	PotionError = ComparePotion();

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (!sTradeBuff.TradeItem[cnt].Flag)
		{
			ZeroMemory(&sTradeBuff.TradeItem[cnt], sizeof(sITEM));
		}
		else
		{
			sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head = 0;		//Ű�� ����
			if (PotionError)
			{
				sTradeBuff.TradeItem[cnt].sItemInfo.PotionCount = sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head + 1;	//���హ�� ������ ��� ����ī��Ʈ�� 1�� �ٲ۴�
			}
			len++;
		}
	}

	//����Ÿ ���� ( Z/NZ ��� )
	len = EecodeCompress((BYTE *)&sTradeBuff, TransTradeItems.TradeBuff, sizeof(sTRADE));
	if (len > TRANS_TRADE_BUFF_SIZE) return FALSE;

	TransTradeItems.code = smTRANSCODE_TRADEITEMS;
	TransTradeItems.size = len + 48;
	TransTradeItems.dwSum = 0;
	TransTradeItems.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems.dwRecver = dwSender;
	TransTradeItems.Temp[0] = 0;
	TransTradeItems.Temp[1] = 0;
	TransTradeItems.Temp[2] = 0;
	TransTradeItems.Temp[3] = 0;

	if (smWsockUserServer)
		return smWsockServer->Send2((char *)&TransTradeItems, TransTradeItems.size, TRUE);

	return FALSE;
}

//������ ��ȯ ���� ���� 
int RecvTradeItems(TRANS_TRADEITEMS *lpTransTradeItems, sTRADE *lpTrade)
{
	//���� ����Ÿ �ص� ( Z/NZ ��� )
	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE *)lpTrade);
	memcpy(&TransLastRecvTrade, lpTransTradeItems, lpTransTradeItems->size);		//���� Ʈ���̵� ����Ÿ �ӽ� ����
	return TRUE;
}

//��ȯâ ������ �۽�
int SendTradeItem(DWORD dwSender)
{
	return SendTradeItem(&sTrade, dwSender);
}
//��ȯâ ������ ����
int RecvTradeItems(TRANS_TRADEITEMS *lpTransTradeItems)
{
	int res;

	res = RecvTradeItems(lpTransTradeItems, &sTradeRecv);
	cTrade.LoadTradeItemIamge();
	cTrade.CheckTradeButtonOk();

	ItemPickUpFlag = 1;

	//Ʈ���̵� ���� ���� Ȯ��
	if (CheckTrade(&sTradeRecv) == FALSE)
	{
		SendRequestTrade(lpTransTradeItems->dwSender, 3);	//��ȯ ���
		cTrade.CancelTradeItem();
		return FALSE;
	}

	if (sTrade.CheckFlag && !sTradeRecv.CheckFlag)
	{
		sTrade.CheckFlag = 0;
		TradeItemCancelCount++;
		if (TradeItemCancelCount >= 3)
		{
			//3�� ��� ����
			SendRequestTrade(lpTransTradeItems->dwSender, 3);	//��ȯ ���
			cTrade.CancelTradeItem();
			return FALSE;
		}
		else
		{
			cMessageBox.ShowMessage(MESSAGE_MOVE_TRADE_ITEM);
			SendTradeItem(lpTransTradeItems->dwSender);
		}
	}

	return res;
}

//������ ��ȯ ����Ű ����
int SendTradeSucessKey(DWORD dwSender)
{
	//������ ��ȯ ����Ű ����
	return SendTradeSucessKey(&sTrade, dwSender);
}



//-------> ���Լ��� ���� ��ä�� ���� �ٲ����� ������ ����� Ȯ�� //////////////

//������ ��ȯ ����Ű ����
int SendTradeSucessKey(sTRADE *lpTrade, DWORD dwSender)
{

	TRANS_TRADE_ITEMKEY	TransTradeItemKey;
	int cnt;
	int	TradeOpenFlag, CancelFlag;

	if (DisconnectFlag) return FALSE;

	TransTradeItemKey.code = smTRANSCODE_TRADE_ITEMKEY;
	TransTradeItemKey.size = sizeof(TRANS_TRADE_ITEMKEY);
	TransTradeItemKey.dwSender = lpCurPlayer->dwObjectSerial;;
	TransTradeItemKey.dwRecver = dwSender;
	TransTradeItemKey.Money = lpTrade->Money;


	TradeOpenFlag = cTrade.OpenFlag;
	cTrade.OpenFlag = 0;
	CancelFlag = 0;

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (lpTrade->TradeItem[cnt].Flag)
		{
			TransTradeItemKey.dwItemCode[cnt] = lpTrade->TradeItem[cnt].sItemInfo.CODE;
			TransTradeItemKey.dwItemKey[cnt] = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransTradeItemKey.dwItemSum[cnt] = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			if (FindInvenItem(lpTrade->TradeItem[cnt].sItemInfo.CODE,
							  lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head,
							  lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum) != 0)
			{
				//���� �������� �κ��� ���� ������ �߰�
				CancelFlag = TRUE;

				//�κ��丮 ������ ���� ����
				SendInvenItemError(1, lpTrade->TradeItem[cnt].sItemInfo.CODE,
								   lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head,
								   lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum);
			}

			if ((TransTradeItemKey.dwItemCode[cnt] & sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
			{
				//�����̴�! ( ������ üũ���� ���హ���� ���� )
				TransTradeItemKey.dwItemSum[cnt] = lpTrade->TradeItem[cnt].sItemInfo.PotionCount & 0xFFFF;
				TransTradeItemKey.dwItemSum[cnt] |= (lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum) << 16;
				TransTradeItemKey.dwItemSum[cnt] ^= (TransTradeItemKey.dwItemSum[cnt] >> 16) ^ 0x5cbc;
			}

		}
		else
		{
			TransTradeItemKey.dwItemCode[cnt] = 0;
			TransTradeItemKey.dwItemKey[cnt] = 0;
			TransTradeItemKey.dwItemSum[cnt] = 0;
		}
	}

	cTrade.OpenFlag = TradeOpenFlag;

	if (!CancelFlag)
	{

		/*
				TRANS_THROW_ITEMS	TransThrowItems;

				TransThrowItems.ThrowItemCount = 0;

				for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
					if ( lpTrade->TradeItem[cnt].Flag ) {
						TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = lpTrade->TradeItem[cnt].sItemInfo.CODE;
						TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
						TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
						TransThrowItems.ThrowItemCount++;
						//SendSaveThrowItem( &lpTrade->TradeItem[cnt].sItemInfo );
					}
				}
				//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
				SendSaveThrowItems( &TransThrowItems );

				if ( smWsockServer )
					smWsockServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;

				if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
					smWsockUserServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;
		*/
		if (smWsockDataServer)
			smWsockDataServer->Send2((char *)&TransTradeItemKey, TransTradeItemKey.size, TRUE);


		//������ ��ȯŰ
		memcpy(&LastTransTradeItemKey, &TransTradeItemKey, sizeof(TRANS_TRADE_ITEMKEY));
		dwLastTransTradeKeyTime = dwPlayTime;
	}

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}


//������ ��ȯ ����Ű ����
int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY *lpTransTradeItemKey, sTRADE *lpTrade)
{
	int cnt;
	sTRADE sChkTrade;
	DWORD	dwChkSum1, dwChkSum2;
	int	ItemFlag;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (TransLastRecvTrade.code)
	{
		//������ ���� ����Ÿ�� ���� ����Ÿ �ٽ� �ص� ( Z/NZ ��� )
		DecodeCompress(TransLastRecvTrade.TradeBuff, (BYTE *)&sChkTrade);
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
		{
			memcpy(&lpTrade->TradeItem[cnt].sItemInfo, &sChkTrade.TradeItem[cnt].sItemInfo, sizeof(sITEMINFO));
		}
	}
	//cTrade.LoadTradeItemIamge();

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (lpTrade->TradeItem[cnt].Flag)
		{

			dwChkSum1 = lpTransTradeItemKey->dwItemSum[cnt];
			dwChkSum2 = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			if ((lpTrade->TradeItem[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
			{
				//�����ΰ��
				ItemFlag = 1;
				dwChkSum1 = dwChkSum1 >> 16;
				dwChkSum2 = dwChkSum2 & 0xFFFF;
			}
			else
				ItemFlag = 0;

			if (lpTransTradeItemKey->dwItemCode[cnt] == lpTrade->TradeItem[cnt].sItemInfo.CODE && dwChkSum1 == dwChkSum2)
			{
				//			lpTransTradeItemKey->dwItemSum[cnt]==lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head = lpTransTradeItemKey->dwItemKey[cnt];

				//�߸��� ������ ����
				if (!lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head)
					lpTrade->TradeItem[cnt].Flag = 0;
				else
				{
					if (ItemFlag == 1)
					{
						//���� ������
						ReformItem(&lpTrade->TradeItem[cnt].sItemInfo);
					}
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			if (lpTransTradeItemKey->dwItemCode[cnt])
				return FALSE;

		}
	}

	//SendRecordTradeItems( lpTrade );			//��ȯ ���� ������ ���

	return TRUE;
}



//������ ��ȯ ���� Ȯ��
int SendTradeCheckItem(DWORD dwSender)
{

	TRANS_TRADE_CHECKITEM	TransTradeCheckItem;
	int cnt;

	if (DisconnectFlag) return FALSE;

	TransTradeCheckItem.code = smTRANSCODE_TRADE_READYITEM;
	TransTradeCheckItem.size = sizeof(TRANS_TRADE_CHECKITEM);
	TransTradeCheckItem.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeCheckItem.dwRecver = dwSender;

	if (sTrade.Money)
	{
		SendSaveMoney();			//�� ���� ( ������� )
	}

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (sTrade.TradeItem[cnt].Flag)
		{
			TransTradeCheckItem.dwSendItemCode[cnt] = sTrade.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwSendItemSum[cnt] = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			//SendSaveThrowItem( &sTrade.TradeItem[cnt].sItemInfo );		//������ ���� ����

			//������ ���� Ȯ��
			if (CheckItemForm(&sTrade.TradeItem[cnt].sItemInfo) == FALSE)
				return FALSE;		//�߸��� ������ 

		}
		else
		{
			TransTradeCheckItem.dwSendItemCode[cnt] = 0;
			TransTradeCheckItem.dwSendItemSum[cnt] = 0;
		}
	}


	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (sTradeRecv.TradeItem[cnt].Flag)
		{
			TransTradeCheckItem.dwRecvItemCode[cnt] = sTradeRecv.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwRecvItemSum[cnt] = sTradeRecv.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
		}
		else
		{
			TransTradeCheckItem.dwRecvItemCode[cnt] = 0;
			TransTradeCheckItem.dwRecvItemSum[cnt] = 0;
		}
	}
	/*
		TRANS_THROW_ITEMS	TransThrowItems;
		TransThrowItems.ThrowItemCount = 0;
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( sTrade.TradeItem[cnt].Flag ) {
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = sTrade.TradeItem[cnt].sItemInfo.CODE;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.Head;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
				TransThrowItems.ThrowItemCount++;
			}
		}
		//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
		SendSaveThrowItems( &TransThrowItems );
	*/

	if (smWsockServer)
		smWsockServer->Send2((char *)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send2((char *)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}

//������ ��ȯ ���� Ȯ�� ����
int RecvTradeCheckItem(TRANS_TRADE_CHECKITEM *lpTransTradeCheckItem)
{
	int cnt;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (DisconnectFlag) return FALSE;

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
	{
		if (sTrade.TradeItem[cnt].Flag)
		{
			if (lpTransTradeCheckItem->dwRecvItemCode[cnt] != sTrade.TradeItem[cnt].sItemInfo.CODE ||
				((lpTransTradeCheckItem->dwRecvItemCode[cnt] & sinITEM_MASK1) != (sinPM1&sinITEM_MASK1) &&
				 lpTransTradeCheckItem->dwRecvItemSum[cnt] != sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum))
			{

				return FALSE;

			}
		}
		else
		{
			if (lpTransTradeCheckItem->dwRecvItemCode[cnt])
				return FALSE;
		}

		if (sTradeRecv.TradeItem[cnt].Flag)
		{
			if (lpTransTradeCheckItem->dwSendItemCode[cnt] != sTradeRecv.TradeItem[cnt].sItemInfo.CODE ||
				((lpTransTradeCheckItem->dwSendItemCode[cnt] & sinITEM_MASK1) != (sinPM1&sinITEM_MASK1) &&
				 lpTransTradeCheckItem->dwSendItemSum[cnt] != sTradeRecv.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum))
			{

				return FALSE;

			}
		}
		else
		{
			if (lpTransTradeCheckItem->dwSendItemCode[cnt])
				return FALSE;
		}
	}

	return TRUE;
}







//��Ƽ ��� 
int SendJoinPartyUser(DWORD dwObjectCode)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_PARTY_JOIN;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectCode;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//��Ƽ �������̽� ��ư�Է� 
int	PartyButton(int Num, int Button)
{
	// Num ��Ƽ�� ��ȣ
	// Button  0-���� 1-�ػ� 2-�߹� 3-Ż��
	DWORD	dwCode;
	smTRANS_COMMAND	smTransCommand;

	if (!InterfaceParty.PartyMemberCount) return FALSE;

	dwCode = 0;

	if (Num >= 0)
		dwCode = InterfaceParty.PartyMember[Num].ChrCode;
	else
		dwCode = 0;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_PARTY_COMMAND;
	smTransCommand.WParam = dwCode;
	smTransCommand.LParam = Button;
	smTransCommand.SParam = 0;

	if (Button < 3 && InterfaceParty.PartyMember[0].ChrCode != lpCurPlayer->dwObjectSerial)
	{
		//��Ƽ�� ����
		return FALSE;
	}

	return smWsockUserServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
}





////////////////////////// ���� ����ġ �̵� ��� ///////////////

int	SetTotalSubMoney(int Money)
{
	TotalSubMoney += Money;

	return TRUE;
}

int	SetTotalAddMoney(int Money)
{
	TotalAddMoney += Money;

	return TRUE;
}

int	SetTotalAddExp(int Exp)
{
	TotalAddExp += Exp;

	return TRUE;
}

int	SetTotalSubExp(int Exp)
{
	TotalSubExp += Exp;

	return TRUE;
}

int	GetTotalMoney()
{
	int m;

	m = sServerMoney[0].InputMoney + sServerMoney[1].InputMoney + sServerMoney[2].InputMoney + WareHouseSubMoney + TotalAddMoney - TotalSubMoney;

	return m;
}

int GetTotalSubExp()
{
	int m;

	m = (sServerExp[0].InputExp + sServerExp[1].InputExp + sServerExp[2].InputExp) - (TotalAddExp - TotalSubExp);

	return m;
}

int GetTotalExp()
{
	int m;

	m = sServerExp[0].InputExp + sServerExp[1].InputExp + sServerExp[2].InputExp + TotalAddExp - TotalSubExp;

	return m;
}

//â�� ����
int	SaveWareHouse(sWAREHOUSE *lpWareHouse, TRANS_WAREHOUSE *lpTransWareHouse)
{
	TRANS_WAREHOUSE	TransWareHouse;
	//TRANS_THROW_ITEMS	TransThrowItems;
	sWAREHOUSE	WareHouseCheck;
	int	CompSize;
	int cnt;
	DWORD	dwChkSum;
	char	*szComp1, *szComp2;
	int	flag;

	if (lpTransWareHouse) flag = 1;
	else flag = 0;

	if (!flag && QuitSave)	return FALSE;

	for (cnt = 0; cnt < 100; cnt++)
	{
		if (!lpWareHouse->WareHouseItem[cnt].Flag)
		{
			ZeroMemory(&lpWareHouse->WareHouseItem[cnt], sizeof(sITEM));
		}
	}


	CompSize = EecodeCompress((BYTE *)lpWareHouse, (BYTE *)TransWareHouse.Data, sizeof(sWAREHOUSE), sizeof(sITEM) * 100);


	//�뷮 ýũ
	if (!flag && CompSize > (smSOCKBUFF_SIZE - 256))
		return FALSE;

	if (flag && CompSize > (smSOCKBUFF_SIZE - 140))
		return FALSE;


	DecodeCompress((BYTE *)TransWareHouse.Data, (BYTE *)&WareHouseCheck, sizeof(sWAREHOUSE));

	szComp1 = (char *)lpWareHouse;
	szComp2 = (char *)&WareHouseCheck;

	dwChkSum = 0;

	for (cnt = 0; cnt < sizeof(sWAREHOUSE); cnt++)
	{
		if (szComp1[cnt] != szComp2[cnt])
		{
			return FALSE;
		}
		else
		{
			dwChkSum += szComp1[cnt] * (cnt + 1);
		}
	}


	//â�� ���������� ���������� ���� ���� �˻��ϱ� ���� ����
	WareHouseSubMoney += (CompWareHouseMoney - lpWareHouse->Money);
	CompWareHouseMoney = 0;
	/*
		if ( !flag ) {
			//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
			SendSaveMoney();				//��

			TransThrowItems.ThrowItemCount = 0;
			for( cnt=0;cnt<THROW_ITEMS_MAX;cnt++) {
				if ( lpWareHouse->WareHouseItem[cnt].Flag ) {
					TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;
					TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
					TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					TransThrowItems.ThrowItemCount++;
				}
			}
			//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
			SendSaveThrowItems( &TransThrowItems );
		}
	*/

	TransWareHouse.code = smTRANSCODE_WAREHOUSE;
	TransWareHouse.size = sizeof(TRANS_WAREHOUSE) - (sizeof(sWAREHOUSE) - CompSize);
	TransWareHouse.DataSize = CompSize;
	TransWareHouse.dwChkSum = dwChkSum;
	TransWareHouse.wVersion[0] = Version_WareHouse;
	TransWareHouse.wVersion[1] = 0;

	if (!flag)
	{
		//�� ���� ( �������� ����� ���� / �ҷ��ö��� �������� �ʱ�ȭ ����� �� )
		TransWareHouse.WareHouseMoney = lpWareHouse->Money ^ (dwChkSum^smTRANSCODE_WAREHOUSE);
		TransWareHouse.UserMoney = lpCurPlayer->smCharInfo.Money ^ (dwChkSum^smTRANSCODE_WAREHOUSE);
	}

	TransWareHouse.dwTemp[0] = 0;
	TransWareHouse.dwTemp[1] = 0;
	TransWareHouse.dwTemp[2] = 0;
	TransWareHouse.dwTemp[3] = 0;
	TransWareHouse.dwTemp[4] = 0;

	dwLastWareHouseChkSum = dwChkSum;				//���� ���� â�� �ڵ�

	if (flag)
	{
		memcpy(lpTransWareHouse, &TransWareHouse, sizeof(TRANS_WAREHOUSE));
		return TRUE;
	}

	if (TransWareHouse.size > smSOCKBUFF_SIZE) TransWareHouse.size = smSOCKBUFF_SIZE;

	if (smWsockDataServer)
	{
		smWsockDataServer->Send2((char *)&TransWareHouse, TransWareHouse.size, TRUE);

		SaveGameData();						//������ ����
		return TRUE;
	}

	return FALSE;
}
//â�� ����
int	SaveWareHouse(sWAREHOUSE *lpWareHouse)
{
	return SaveWareHouse(lpWareHouse, 0);
}

//â�� ��������
int	LoadWareHouse(TRANS_WAREHOUSE *lpTransWareHouse, sWAREHOUSE *lpWareHouse, int flag)
{
	sWAREHOUSE	WareHouseCheck;
	int cnt;
	DWORD	dwChkSum;
	char	*szComp;
	char	szMsgBuff[128];

	if (lpTransWareHouse->DataSize == 0)
	{
		//�ʱⰪ �ƹ��͵� ����
		ZeroMemory(lpWareHouse, sizeof(sWAREHOUSE));
		lpWareHouse->Money = 2023;
		lpWareHouse->Weight[0] = 197;
		CompWareHouseMoney = lpWareHouse->Money;	//â�� ���������� ���������� ���� ���� �˻��ϱ� ���� ����
		return TRUE;
	}

	DecodeCompress((BYTE *)lpTransWareHouse->Data, (BYTE *)&WareHouseCheck);

	dwChkSum = 0;

	szComp = (char *)&WareHouseCheck;

	for (cnt = 0; cnt < sizeof(sWAREHOUSE); cnt++)
	{
		dwChkSum += szComp[cnt] * (cnt + 1);
	}
	if (dwChkSum == lpTransWareHouse->dwChkSum)
	{

		if (!flag && dwLastWareHouseChkSum && dwLastWareHouseChkSum != lpTransWareHouse->dwChkSum && !smConfig.DebugMode)
		{
			//������ ������ â�� ����Ÿ�� �ű� �ε� ����Ÿ�� �ڵ尡 Ʋ��
			return FALSE;
		}

		memcpy(lpWareHouse, &WareHouseCheck, sizeof(sWAREHOUSE));

		CompWareHouseMoney = lpWareHouse->Money;	//â�� ���������� ���������� ���� ���� �˻��ϱ� ���� ����

		for (cnt = 0; cnt < 100; cnt++)
		{
			if (lpWareHouse->WareHouseItem[cnt].Flag)
			{

				//2�� ���� ��
				if (!lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0)
					lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0 = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum + lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;

				//CheckCopyItem( &lpWareHouse->WareHouseItem[cnt].sItemInfo );		//���� �Ǵ� �ҷ������� �˻�

				if (!lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime)
				{
					//�ڵ� ����
					lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime =
						(lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head >> 2) ^ (lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum << 2);
				}

				//���� Ȯ��
				if ((lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE&sinITEM_MASK2) == sinSP1)
				{
					//���� ( ����ī���Ͱ� ������ �����ִ� ���� )
					if (lpWareHouse->WareHouseItem[cnt].sItemInfo.PotionCount > 1)	lpWareHouse->WareHouseItem[cnt].Flag = 0;

					// �庰 - SP1 ������ â���� ������� ����
					/*
					//�����Ⱓ Ȯ��
					if ( lpWareHouse->WareHouseItem[cnt].sItemInfo.dwCreateTime<(sinItemTime-(60*60*24*12)) ||
						lpWareHouse->WareHouseItem[cnt].sItemInfo.dwCreateTime>(sinItemTime+(60*60*24*7)) ) {
							lpWareHouse->WareHouseItem[cnt].Flag =0;
						}
						*/
				}

				if (DeleteEventItem_TimeOut(&lpWareHouse->WareHouseItem[cnt].sItemInfo) == TRUE)
				{
					//�̺�Ʈ ������ ��¥���� ����
					lpWareHouse->WareHouseItem[cnt].Flag = 0;
					wsprintf(szMsgBuff, mgItemTimeOut, lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemName);
					AddChatBuff(szMsgBuff, 0);
				}
			}
		}
		return TRUE;
	}

	return FALSE;
}

//â�� ��������
int	LoadWareHouse(TRANS_WAREHOUSE *lpTransWareHouse, sWAREHOUSE *lpWareHouse)
{
	return LoadWareHouse(lpTransWareHouse, lpWareHouse, 0);
}

//�̺�Ʈ ������ ����
int SendCollectMoney(int Money)
{
	/*
		smTRANS_COMMAND	TransCommand;

		TransCommand.code =	smTRANSCODE_COLLECTMONEY;
		TransCommand.size = sizeof(smTRANS_COMMAND);
		TransCommand.WParam = Money;							//����� ��
		TransCommand.LParam = lpCurPlayer->smCharInfo.Money;		//������ ��
		TransCommand.SParam = 0;

		AddInvenMoney( -Money );				//�κ� �� Ȯ��

		if ( smWsockServer ) {
			return smWsockServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
		}
	*/

	sITEM	sItemMoney;
	int j;


	memset(&sItemMoney, 0, sizeof(sITEM));
	sItemMoney.sItemInfo.CODE = sinGG1 | sin01;
	for (j = 0; j < MAX_ITEM; j++)
	{
		if (sItem[j].CODE == sItemMoney.sItemInfo.CODE)
		{ //�����̸� 
			memcpy(&sItemMoney, &sItem[j], sizeof(sITEM));
			sItemMoney.sItemInfo.Money = Money;			//�ݾ��� �Ѱ��ش� 
			sItemMoney.sItemInfo.ItemKindCode = smTRANSCODE_COLLECTMONEY;
			ReformItem(&sItemMoney.sItemInfo);		//���� 
			if (sinThrowItemToFeild(&sItemMoney))
			{
				AddInvenMoney(-Money);				//�κ� �� Ȯ��
				return TRUE;
			}
		}
	}

	return FALSE;
}

//PK���ؼ� ����
int PlayerKilling(DWORD dwObjectSerial)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_PLAYERKILLING;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;				//���� �ڵ�
	TransCommand.LParam = lpCurPlayer->dwObjectSerial;		//�� �ڵ�
	TransCommand.SParam = 0;

	if (smWsockUserServer)
	{
		return smWsockUserServer->Send2((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}



//�ͽ��� ����Ÿ�� ������ ����
int SendCraftItemToServer(void *lpsCraftItem_Send)
{
	Log::Debug("SendCraftItemToServer : Requisitado por�m foi removido.");
	return NULL;
}
//������ �� ������ ����Ÿ�� ������ ����
int SendWingItemToServer(void *lpsCraftItem_Send)
{
	Log::Debug("SendWingItemToServer : Requisitado por�m foi removido.");
	return NULL;
}

//����¡ ����Ÿ�� ������ ����
int SendAgingItemToServer(void *lpsCraftItem_Send)
{
	Log::Debug("SendAgingItemToServer : Requisitado por�m foi removido.");
	return NULL;
}
//��ũ�ھ� ��� ����
int SendLinkCoreToServer(sITEMINFO *lpItem)
{
	Log::Debug("SendLinkCoreToServer : Requisitado por�m foi removido.");
	return NULL;
}
int SucessLinkCore(smTRANS_COMMAND_EX *lpTransCommandEx)
{
	int x, z, cnt;

	sITEMINFO	*lpItem;

	//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´� ( ��ũ �ھ� ���� )
	lpItem = FindInvenItem(lpTransCommandEx->WxParam, lpTransCommandEx->LxParam, lpTransCommandEx->SxParam);

	if (lpItem)
	{
		SendUseItemCodeToServer(lpItem);
		cInvenTory.DeleteInvenItemToServer(lpTransCommandEx->WxParam, lpTransCommandEx->LxParam, lpTransCommandEx->SxParam);
		ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ

		if (FindInvenItem(lpTransCommandEx->WxParam, lpTransCommandEx->LxParam, lpTransCommandEx->SxParam) != 0) return FALSE;

	}
	else
		return FALSE;


	CloseEachPlayer();

	WarpFieldNearPos(lpTransCommandEx->EParam, lpTransCommandEx->WParam, lpTransCommandEx->SParam, &x, &z);

	lpCurPlayer->SetPosi(lpTransCommandEx->WParam, lpTransCommandEx->LParam, lpTransCommandEx->SParam, 0, 0, 0);
	TraceCameraPosi.x = lpCurPlayer->pX;
	TraceCameraPosi.y = lpCurPlayer->pY;
	TraceCameraPosi.z = lpCurPlayer->pZ;
	TraceTargetPosi.x = lpCurPlayer->pX;
	TraceTargetPosi.y = lpCurPlayer->pY;
	TraceTargetPosi.z = lpCurPlayer->pZ;

	//�ٴ� Ȯ��
	lpCurPlayer->OnStageField = -1;
	if (smGameStage[0])
	{
		cnt = smGameStage[0]->GetFloorHeight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpCurPlayer->Pattern->SizeHeight);
		lpCurPlayer->OnStageField = 0;
	}
	if (cnt == CLIP_OUT && smGameStage[1])
	{
		lpCurPlayer->OnStageField = 1;
	}

	StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 48 * fONE, lpCurPlayer->pZ, EFFECT_RETURN1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//��ų ȿ����
	RestartPlayCount = 350;		//5�� ���� ����

	return TRUE;
}


//����¡ �Ϸ�� �������� ������ ����
int SendAgingUpgradeItemToServer(sITEMINFO *lpItem)
{
	Log::Debug("SendAgingUpgradeItemToServer : Requisitado por�m foi removido.");
	return NULL;
}
/*
//������ Ȯ��
#define smTRANSCODE_CHECKITEM			0x50320300
//������ ���� Ȯ��
#define smTRANSCODE_ERRORITEM			0x50320310

struct TRANS_ITEMINFO {
	int	size,code;

	//char	szItemName[32];
	//DWORD	ItemCode;

	sITEMINFO	Item;

	int x,y,z;

};

struct	TRANS_ITEM_CODE {
	int size,code;

	DWORD	dwFlag;

	DWORD	dwItemCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
	DWORD	dwTime;
}


*/


//�κ��丮�� ���హ���� ���� 1�� ����
int ClearInvenPotionCount()
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++)
	{
		if (cInvenTory.InvenItem[cnt].Flag && (cInvenTory.InvenItem[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		{
			cInvenTory.InvenItem[cnt].sItemInfo.PotionCount = 1;
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++)
	{
		if (cInvenTory.InvenItemTemp[cnt].Flag && (cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		{
			cInvenTory.InvenItemTemp[cnt].sItemInfo.PotionCount = 1;
		}
	}

	if (cTrade.OpenFlag)
	{
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++)
		{
			if (sTrade.TradeItem[cnt].Flag && (sTrade.TradeItem[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
			{
				sTrade.TradeItem[cnt].sItemInfo.PotionCount = 1;
			}
		}
	}

	if (MouseItem.Flag && (MouseItem.sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		MouseItem.sItemInfo.PotionCount = 1;

	cInvenTory.ReFormPotionNum();

	SaveGameData();

	return TRUE;
}

//�κ��丮�� ������ ã�� (�ڵ�)
sITEMINFO *FindItemFromCode(DWORD dwItemCode)
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++)
	{
		if (cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].sItemInfo.CODE == dwItemCode)
		{
			return &cInvenTory.InvenItem[cnt].sItemInfo;
		}
	}
	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++)
	{
		if (cInvenTory.InvenItemTemp[cnt].Flag && cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE == dwItemCode)
		{
			return &cInvenTory.InvenItemTemp[cnt].sItemInfo;
		}
	}

	return NULL;
}



DWORD	dwLastCheckItemTime = 0;

//�������� ������ ������ Ȯ��
int	SendCheckItemToServer(sITEMINFO *lpItem)
{
	Log::Debug("SendCheckItemToServer : Requisitado por�m foi removido.");
	return NULL;
}
//������ Ȯ�� ��� ����
int RecvCheckItemFromServer(TRANS_ITEM_CODE *lpTransItemCode)
{
	Log::Debug("RecvCheckItemFromServer : Requisitado por�m foi removido.");
	return NULL;
}
//�Ǹ��� �������� ������ ����
int	SellItemToServer(sITEMINFO *lpItem, int Count)
{
	Log::Debug("SellItemToServer : Requisitado por�m foi removido.");
	return NULL;
}



int Chk_InventPosCount = 1;
DWORD	dwCheckInvenItemTime = 0;

//�κ��丮�� ������ �������� ������ ������ �˻�
int CheckInvenItemToServer()
{
	Log::Debug("CheckInvenItemToServer : Requisitado por�m foi removido.");
	return NULL;
}

////////////////////////////// �޸� Ÿ�� ýũ ������ ///////////////////////////////

HANDLE	hTimeCheckThread = 0;
int		TimeCheckCounter;

DWORD WINAPI TimeCheckThread(void *pInfo)
{
	HANDLE	hThread;
	DWORD	dwTime;
	DWORD	dwLastTime;
	int		cnt;

	hThread = GetCurrentThread();
	dwTime = GetCurrentTime();
	dwLastTime = dwTime;
	TimeCheckCounter = 0;

	while (1)
	{
		dwTime = GetCurrentTime();
		cnt = dwTime - dwLastTime;
		if (cnt > PlayTimerMax) PlayTimerMax = cnt;
		dwLastTime = dwTime;
		TimeCheckCounter++;
		Sleep(800);
	}

	ExitThread(TRUE);
	return TRUE;
}


//�ε��� ȭ�� ǥ��
int OpenTimeCheckThread()
{
	DWORD	dwTimeCheckThreadID;

	if (!hTimeCheckThread)
	{
		hTimeCheckThread = CreateThread(NULL, 0, TimeCheckThread, 0, 0, &dwTimeCheckThreadID);
	}

	return TRUE;
}

int	PlayTimerMax_SendingCount = 0;

int SendPlayTimerMax()
{

	smTRANS_COMMAND	smTransCommand;

	if (PlayTimerMax >= 10000 || (PlayTimerMax_SendingCount & 0x7) == 0)
	{

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_PROCESS_TIMEMAX;
		smTransCommand.WParam = PlayTimerMax;
		smTransCommand.LParam = TimeCheckCounter;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		if (smWsockDataServer)
			smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

		PlayTimerMax = 0;
	}

	PlayTimerMax_SendingCount++;

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////




//�������� �޸� ���� ���ؼ� ��������
DWORD funcCheckMemSum(DWORD FuncPoint, int count)
{
	int cnt;
	DWORD dwSum;
	int	xcnt;
	DWORD *lpGetPrtectPoint;


	lpGetPrtectPoint = (DWORD *)FuncPoint;

	dwSum = 0;
	xcnt = 1;

	for (cnt = 0; cnt < count; cnt++)
	{
		xcnt += cnt;
		dwSum += lpGetPrtectPoint[cnt] * xcnt;
	}

	return dwSum;
}

/*
		dwResult = fnChkMem( TransFuncMem->Param[0] , TransFuncMem->Param[1] );

		smTransCommand.code = smTRANSCODE_FUNCTON_RESULT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransFuncMem->Param[0];
		smTransCommand.LParam = TransFuncMem->Param[1];
		smTransCommand.SParam = TransFuncMem->Param[2];
		smTransCommand.EParam = dwResult;

*/
//�������� �޸� ���� ���ؼ� ��������
//DWORD funcCheckMemSum2( DWORD FuncPoint , int count )

///////////////////////////////////////////////////////////////////////////////

DWORD	dwFuncList[][2] = {
	{ (DWORD)funcCheckMemSum << 1				,	(DWORD)512		},
	{ (DWORD)Check_PlaySubReleaseFunc << 1	,	(DWORD)512		},
	{ (DWORD)PlayPat3D << 1					,	(DWORD)2048	},
	{ (DWORD)Check_CodeSafe << 1				,	(DWORD)1024	},
	{ (DWORD)AddExp << 1						,	(DWORD)512		},
	{ (DWORD)sinSetLife << 1					,	(DWORD)512		},
	{ (DWORD)NetWorkPlay << 1					,	(DWORD)2800	},

	{ (DWORD)E_Shield_BlockRate << 1			,	(DWORD)((Meteo_UseMana - E_Shield_BlockRate) + 10) },		//��ų ���̺� ����

	{ (DWORD)Code_VRamBuffOpen << 1			,	(DWORD)3600	},			//������ ���� ���� ýũ �Ѵ�
	{ (DWORD)0								,	(DWORD)0 }
};



//Ŭ���̾�Ʈ ��� ��ġ�� ���� ������ ����
int	SendClientFuncPos()
{

	TRANS_CLIENT_FUNPOS	TransClientFuncPos;
	int cnt;


	ZeroMemory(&TransClientFuncPos, sizeof(TRANS_CLIENT_FUNPOS));

	TransClientFuncPos.code = smTRANSCODE_CLIENT_FUNCPOS;
	TransClientFuncPos.size = sizeof(TRANS_CLIENT_FUNPOS);
	TransClientFuncPos.ClientVersion = Client_Version;

	cnt = 0;
	TransClientFuncPos.dwFunc_CheckMemSum = dwFuncList[cnt][0] >> 1;
	TransClientFuncPos.dwFuncValue[cnt].dwFunc = dwFuncList[cnt][0] >> 1;
	TransClientFuncPos.dwFuncValue[cnt].dwLen = dwFuncList[cnt][1];
	TransClientFuncPos.dwFuncValue[cnt].dwChkSum = funcCheckMemSum(TransClientFuncPos.dwFuncValue[cnt].dwFunc, TransClientFuncPos.dwFuncValue[cnt].dwLen);
	//TransClientFuncPos.dwFuncValue[cnt].dwChkSum = fnChkMem( TransClientFuncPos.dwFuncValue[cnt].dwFunc , TransClientFuncPos.dwFuncValue[cnt].dwLen );

	while (1)
	{

		if (dwFuncList[cnt][0] == 0) break;
		if (cnt >= dwFUNC_VALUE_MAX)  break;

		TransClientFuncPos.dwFuncValue[cnt].dwFunc = dwFuncList[cnt][0] >> 1;
		TransClientFuncPos.dwFuncValue[cnt].dwLen = dwFuncList[cnt][1];
		TransClientFuncPos.dwFuncValue[cnt].dwChkSum = funcCheckMemSum(TransClientFuncPos.dwFuncValue[cnt].dwFunc, TransClientFuncPos.dwFuncValue[cnt].dwLen);

		cnt++;
	}

	TransClientFuncPos.FuncCount = cnt;

	//������ ����
	if (smWsockDataServer)
	{
		return smWsockDataServer->Send2((char *)&TransClientFuncPos, TransClientFuncPos.size, TRUE);
	}

	return FALSE;
}


//�޸� �˻� ��� ����
int	RecvMemFuncData(TRANS_FUNC_MEMORY *TransFuncMem)
{
	smTRANS_COMMAND	smTransCommand;

	fnChkMem = (LPFN_CheckMem)((void *)TransFuncMem->szData);

	smTransCommand.size = 0;

	fnChkMem(TransFuncMem, &smTransCommand);

	if (smWsockDataServer && smTransCommand.size > 0)
		smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}


////////////////////////////// ������ ���� ���� ťó�� //////////////////////////

#define	RECV_TRANSITEM_QUE_MAX			256
#define	RECV_TRANSITEM_QUE_MASK			255

TRANS_ITEMINFO	Recv_TransItemInfo_Que[RECV_TRANSITEM_QUE_MAX];

int	RecvTransItemQue_Push = 0;
int	RecvTransItemQue_Pop = 0;

//������ ť�� �ӽ� ����
int	PushRecvTransItemQue(TRANS_ITEMINFO *lpTransItemInfo)
{
	int mCnt;

	mCnt = RecvTransItemQue_Push & RECV_TRANSITEM_QUE_MASK;

	memcpy(&Recv_TransItemInfo_Que[mCnt], lpTransItemInfo, sizeof(TRANS_ITEMINFO));
	RecvTransItemQue_Push++;
	RecvTransItemQue_Pop = RecvTransItemQue_Push - RECV_TRANSITEM_QUE_MASK;
	if (RecvTransItemQue_Pop < 0) RecvTransItemQue_Pop = 0;

	return TRUE;
}

//�ӽ������ ������ ť���� ã��
TRANS_ITEMINFO	*FindRecvTransItemQue(DWORD dwCode, DWORD dwHead, DWORD dwChkSum)
{
	int cnt, mCnt;

	for (cnt = RecvTransItemQue_Pop; cnt < RecvTransItemQue_Push; cnt++)
	{
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;

		if (Recv_TransItemInfo_Que[mCnt].Item.CODE == dwCode &&
			Recv_TransItemInfo_Que[mCnt].Item.ItemHeader.Head == dwHead &&
			Recv_TransItemInfo_Que[mCnt].Item.ItemHeader.dwChkSum == dwChkSum)
		{

			return &Recv_TransItemInfo_Que[mCnt];
		}
	}

	return NULL;
}

//�ӽ������ ������ ť�� ������ ��������� �˻�
int CheckRecvTrnsItemQue()
{
	int cnt, mCnt;

	for (cnt = RecvTransItemQue_Pop; cnt < RecvTransItemQue_Push; cnt++)
	{
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;
		if (Recv_TransItemInfo_Que[mCnt].Item.CODE) return FALSE;
	}

	return TRUE;
}

//����� ������ �ڵ� ������ ����
int SendUseItemCodeToServer(sITEMINFO *lpItem)
{
	Log::Debug("SendUseItemCodeToServer : Requisitado por�m foi removido.");
	return NULL;
}
//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ�
int RecvCommandUser(smTRANS_COMMAND_EX *lpTransCommandEx)
{
	smCHAR *lpChar;
	smCHAR *lpChar2;

	switch (lpTransCommandEx->WParam)
	{
		case smCOMMNAD_USER_WARP:
			//���� ����Ʈ ǥ��
			StartEffect(lpTransCommandEx->LParam, lpTransCommandEx->SParam, lpTransCommandEx->EParam, EFFECT_RETURN1);
			SkillPlaySound(SKILL_SOUND_LEARN, lpTransCommandEx->LParam, lpTransCommandEx->SParam, lpTransCommandEx->EParam);			//��ų ȿ����
			break;


		case smCOMMNAD_USER_AGINGUP:
			//�ڵ� �÷��̾ ã�´�
			lpChar = FindChrPlayer(lpTransCommandEx->WxParam);
			if (lpChar)
			{
				StartEffect(lpChar->pX, lpChar->pY, lpChar->pZ, EFFECT_AGING);
				esPlaySound(7, GetDistVolume(lpChar->pX, lpChar->pY, lpChar->pZ), 1600);	//���� ����
			}
			break;

		case smCOMMNAD_USER_HOLY_INCANTATION:
			lpChar = FindChrPlayer(lpTransCommandEx->LParam);
			if (lpChar)
			{
				AssaParticle_HolyIncantationPart(lpChar, lpTransCommandEx->SParam);
			}
			break;

		case smCOMMNAD_USER_RESURRECTION:
			lpChar = FindChrPlayer(lpTransCommandEx->LParam);
			if (lpChar)
			{
				AssaParticle_ResurrectionPart(lpChar);
			}
			break;

		case smCOMMNAD_USER_FORCEORB:		//���� ���
			//�ڵ� �÷��̾ ã�´�
			lpChar = FindChrPlayer(lpTransCommandEx->WxParam);
			if (lpChar)
			{
				StartSkill(lpChar->pX, lpChar->pY, lpChar->pZ, 0, 0, 0, SKILL_UP1);
				SkillPlaySound(SKILL_SOUND_LEARN, lpChar->pX, lpChar->pY, lpChar->pZ);			//��ų ȿ����
			}
			break;

		case smCOMMNAD_USER_VALENTO:
			//�߷��� �ҿＭĿ ����Ʈ
			lpChar = FindChrPlayer(lpTransCommandEx->LParam);
			lpChar2 = FindChrPlayer(lpTransCommandEx->SParam);
			if (lpChar && lpChar2)
			{
				sinSkillEffect_Soul_Sucker(lpChar, lpChar2);
			}
			break;

		case smCOMMNAD_USER_FIREFLOWER:
			lpChar = FindChrPlayer(lpTransCommandEx->WxParam);
			if (lpChar && lpChar != lpCurPlayer)
			{
				DWORD	dwTable = GetTableNum(lpTransCommandEx->LParam);
				AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 1000, (dwTable >> 24) & 0xFF, 0, 0, lpChar);
				AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 1500, (dwTable >> 16) & 0xFF, 0, 0, lpChar);
				AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 2000, (dwTable >> 8) & 0xFF, 0, 0, lpChar);
				AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 2500, dwTable & 0xFF, 0, 0, lpChar);
			}
			break;

	}

	return TRUE;
}

//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
int	SendCommandUser(int wParam, int lParam, int sParam, int eParam)
{
	smTRANS_COMMAND_EX	smTransCommandEx;

	smTransCommandEx.code = smTRANSCODE_COMMAND_USER;
	smTransCommandEx.size = sizeof(smTRANS_COMMAND_EX);
	smTransCommandEx.WParam = wParam;
	smTransCommandEx.LParam = lParam;
	smTransCommandEx.SParam = sParam;
	smTransCommandEx.EParam = eParam;

	smTransCommandEx.WxParam = lpCurPlayer->dwObjectSerial;
	smTransCommandEx.LxParam = 0;
	smTransCommandEx.SxParam = 0;
	smTransCommandEx.ExParam = 0;

	if (smWsockUserServer)
	{
		return smWsockUserServer->Send((char *)&smTransCommandEx, smTransCommandEx.size, TRUE);
	}
	return FALSE;
}

//���������� ��� �ڵ� ����
int	SendCommand_AreaServer(DWORD dwCode, int wParam, int lParam, int sParam, int eParam)
{
	Log::Debug("SendCommand_AreaServer : Requisitado por�m foi removido.");
	return NULL;
}

//���������� ��� �ڵ� ����
int	SendCommand_DataServer(DWORD dwCode, int wParam, int lParam, int sParam, int eParam)
{
	Log::Debug("SendCommand_DataServer : Requisitado por�m foi removido.");
	return NULL;
}

struct	sDAMAGE_RECORD
{
	int	Damage;
	int	Count;
};

sDAMAGE_RECORD	sRecvDamage[3];
sDAMAGE_RECORD	sSendDamage[3];

////////////////////////////////////////////////////////////////////////////
//���ݷ� ��� �ʱ�ȭ
int	Init_RecordDamage()
{
	ZeroMemory(sRecvDamage, sizeof(sDAMAGE_RECORD) * 3);
	ZeroMemory(sSendDamage, sizeof(sDAMAGE_RECORD) * 3);

	return TRUE;
}

//���� ���ݷ� ���
int	Record_RecvDamage(smWINSOCK *lpsmSock, int Damage)
{
	smTRANS_COMMAND	smTransCommand;

	if (smWsockServer == lpsmSock)
	{
		sRecvDamage[0].Damage -= Damage;
		sRecvDamage[0].Count--;

		if ((sRecvDamage[0].Count & 0x1FF) == 0)
		{
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[0].Count;
			smTransCommand.LParam = sRecvDamage[0].Damage;
			smTransCommand.SParam = sSendDamage[0].Count;
			smTransCommand.EParam = sSendDamage[0].Damage;
			if (lpsmSock) lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		}
	}
	else if (smWsockUserServer == lpsmSock)
	{
		sRecvDamage[1].Damage -= Damage;
		sRecvDamage[1].Count--;

		if ((sRecvDamage[1].Count & 0x1FF) == 0)
		{
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[1].Count;
			smTransCommand.LParam = sRecvDamage[1].Damage;
			smTransCommand.SParam = sSendDamage[1].Count;
			smTransCommand.EParam = sSendDamage[1].Damage;
			if (lpsmSock) lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		}

	}
	else if (smWsockExtendServer == lpsmSock)
	{
		sRecvDamage[2].Damage -= Damage;
		sRecvDamage[2].Count--;

		if ((sRecvDamage[2].Count & 0x1FF) == 0)
		{
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[2].Count;
			smTransCommand.LParam = sRecvDamage[2].Damage;
			smTransCommand.SParam = sSendDamage[2].Count;
			smTransCommand.EParam = sSendDamage[2].Damage;
			if (lpsmSock) lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		}
	}

	return TRUE;
}

//���� ���ݷ� ���
int	Record_SendDamage(smWINSOCK *lpsmSock, int Damage)
{
	smTRANS_COMMAND	smTransCommand;

	if (smWsockServer == lpsmSock)
	{
		sSendDamage[0].Damage -= Damage;
		sSendDamage[0].Count--;

		if ((sRecvDamage[0].Count & 0x1FF) == 0)
		{
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[0].Count;
			smTransCommand.LParam = sRecvDamage[0].Damage;
			smTransCommand.SParam = sSendDamage[0].Count;
			smTransCommand.EParam = sSendDamage[0].Damage;
			if (lpsmSock) lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		}
	}
	else if (smWsockUserServer == lpsmSock)
	{
		sSendDamage[1].Damage -= Damage;
		sSendDamage[1].Count--;

		if ((sRecvDamage[1].Count & 0x1FF) == 0)
		{
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[1].Count;
			smTransCommand.LParam = sRecvDamage[1].Damage;
			smTransCommand.SParam = sSendDamage[1].Count;
			smTransCommand.EParam = sSendDamage[1].Damage;
			if (lpsmSock) lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		}

	}
	else if (smWsockExtendServer == lpsmSock)
	{
		sSendDamage[2].Damage -= Damage;
		sSendDamage[2].Count--;

		if ((sRecvDamage[2].Count & 0x1FF) == 0)
		{
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[2].Count;
			smTransCommand.LParam = sRecvDamage[2].Damage;
			smTransCommand.SParam = sSendDamage[2].Count;
			smTransCommand.EParam = sSendDamage[2].Damage;
			if (lpsmSock) lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		}
	}

	return TRUE;
}

//��Ƽ������ ��ų ��� ���� ������
int	SendPartySkillToServer(DWORD dwSkillCode, int SkillLevel, int Around, int wParam, int lParam, int sParam, int eParam)
{
	Log::Debug("SendPartySkillToServer : Requisitado por�m foi removido.");
	return NULL;
}
//��ų ���
int SendCancelSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam)
{
	Log::Debug("SendCancelSkillToServer : Requisitado por�m foi removido.");
	return NULL;
}
//��ų ����
int SendUpdateSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam)
{
	Log::Debug("SendUpdateSkillToServer : Requisitado por�m foi removido.");
	return NULL;
}
//������ ��ų���� ����
int SendProcessSKillToServer(DWORD dwSkillCode, int point, int Param1, int Param2)
{
	Log::Debug("SendProcessSKillToServer : Requisitado por�m foi removido.");
	return NULL;
}
//��Ƽ ��ų ��Ŷ ����
int RecvPartySkillFromServer(TRANS_PARTY_SKILL *lpTransPartySkill)
{
	Log::Debug("RecvPartySkillFromServer : Requisitado por�m foi removido.");
	return NULL;
}
DWORD	dwLastClanUserCode = 0;

//Ŭ�� �޼��� ������
int SendClanJoinService(DWORD dwMsgCode, smCHAR *lpChar)
{
	smTRANS_CHAR_COMMAND2	smTransCharCommand;

#ifdef  __CLANSUBCHIP__
	if (cldata.myPosition != 101 && cldata.myPosition != 104) return FALSE;
#else
	if (cldata.myPosition != 101) return FALSE;
#endif

	smTransCharCommand.code = smTRANSCODE_CLAN_SERVICE;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND2);
	smTransCharCommand.WParam = dwMsgCode;
	smTransCharCommand.LParam = lpChar->dwObjectSerial;
	smTransCharCommand.SParam = lpCurPlayer->dwObjectSerial;
	lstrcpy(smTransCharCommand.szName, cldata.name);
	lstrcpy(smTransCharCommand.szId, UserAccount);

	if (smWsockUserServer)
		smWsockUserServer->Send2((char *)&smTransCharCommand, smTransCharCommand.size, TRUE);

	return TRUE;
}

//Ŭ�� ���� ���� �޼��� ������
int SendJoinClan()
{
	smTRANS_CHAR_COMMAND2	smTransCharCommand;

	if (!dwLastClanUserCode) return FALSE;

	smTransCharCommand.code = smTRANSCODE_CLAN_SERVICE;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND2);
	smTransCharCommand.WParam = 2;
	smTransCharCommand.LParam = dwLastClanUserCode;
	smTransCharCommand.SParam = lpCurPlayer->dwObjectSerial;
	lstrcpy(smTransCharCommand.szName, lpCurPlayer->smCharInfo.szName);
	lstrcpy(smTransCharCommand.szId, UserAccount);

	if (smWsockUserServer)
		smWsockUserServer->Send2((char *)&smTransCharCommand, smTransCharCommand.size, TRUE);

	return TRUE;
}

//Ŭ�� �޼��� �ޱ�
int RecvClanJoinService(smTRANS_CHAR_COMMAND2 *lpTransCharCommand)
{
	smCHAR *lpChar;

	switch (lpTransCharCommand->WParam)
	{
		case 1:		//���� ����
			if (cldata.myPosition == 102 || cldata.myPosition == 103)
			{

				lpChar = FindAutoPlayer(lpTransCharCommand->SParam);
				if (lpChar)
				{
					ClanJoin(lpTransCharCommand->szName, lpChar->smCharInfo.szName);
					OpenEachPlayer(lpChar);
					DispEachMode = TRUE;
					dwLastClanUserCode = lpTransCharCommand->SParam;
				}
			}
			break;

		case 2:
#ifdef  __CLANSUBCHIP__
			if (cldata.myPosition == 101 || cldata.myPosition == 104)
			{
#else
			if (cldata.myPosition == 101)
			{
#endif
				lpChar = FindAutoPlayer(lpTransCharCommand->SParam);
				if (lpChar)
					menu_joinOpen_Chip(1, lpTransCharCommand->szId, lpTransCharCommand->szName, lpChar->smCharInfo.JOB_CODE, lpChar->smCharInfo.Level);
				else
					menu_joinOpen_Chip(1, lpTransCharCommand->szId, lpTransCharCommand->szName, 0, 0);
			}
			break;
	}

	return TRUE;
}


/////////// DivineLightning ��ų ���� �Լ� ///////////////////

DWORD	dwSkill_DivineLightning_Target[8];
int		DivineLightning_Target_Count = 0;
int		DivineLightning_FindCount = 0;

//����� ����Ʈ�� ��ų ���� ( �غ��Լ� - ���ΰ��� )
int	SkillPlay_DivineLightning_Select(smCHAR *lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	smWINSOCK *lpsmSendSock = 0;

	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile("������ ������Ŷ - SkillPlay_DivineLightning_Select ");
	quit = 1;
	return TRUE;

	if (lpChar != lpCurPlayer) return FALSE;

	TargetCount = 0;

	LightningCount = Divine_Lightning_Num[SkillPoint - 1];

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{

		cCount = (DivineLightning_FindCount + cnt)&OTHER_PLAYER_MASK;

		if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
			lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
			chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
			chrOtherPlayer[cCount].smCharInfo.Life[0]>0)
		{

			x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
			y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
			z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

			dist = x*x + y*y + z*z;

			if (dist < (160 * 160))
			{
				dwSkill_DivineLightning_Target[TargetCount] = chrOtherPlayer[cCount].dwObjectSerial;
				TransSkilAttackData.dwTarObjectSerial[TargetCount] = chrOtherPlayer[cCount].dwObjectSerial;
				TargetCount++;

				if (chrOtherPlayer[cCount].smCharInfo.Life[1] && chrOtherPlayer[cCount].smCharInfo.Life[0] == chrOtherPlayer[cCount].smCharInfo.Life[1])
				{
					chrOtherPlayer[cCount].EnableStateBar = TRUE;
				}
				if (TargetCount >= LightningCount)
				{
					DivineLightning_FindCount = cCount;
					break;
				}
			}
		}
	}

	TransSkilAttackData.TargetCount = TargetCount;
	DivineLightning_Target_Count = TargetCount;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*TargetCount + 16;

	TransSkilAttackData.x = lpChar->pX;
	TransSkilAttackData.y = lpChar->pY;
	TransSkilAttackData.z = lpChar->pZ;

	TransSkilAttackData.AttackState = 103;			//�𵥵� ����
	TransSkilAttackData.AttackSize = 160 * 160;
	TransSkilAttackData.Power = GetRandomPos(Divine_Lightning_Damage[SkillPoint - 1][0], Divine_Lightning_Damage[SkillPoint - 1][1]);

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	if (lpsmSendSock && TargetCount > 0)
	{
		//���� ���ݷ� ���
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send2((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	return TRUE;
}

//����� ����Ʈ�� ��ų ���� ( ����Ʈ ���� )
int SkillPlay_DivineLightning_Effect(smCHAR *lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	smCHAR *lpTarChar;

	TargetCount = 0;

	if (lpChar == lpCurPlayer)
	{
		for (cnt = 0; cnt < DivineLightning_Target_Count; cnt++)
		{
			lpTarChar = FindAutoPlayer(dwSkill_DivineLightning_Target[cnt]);
			if (lpTarChar)
			{
				AssaParticle_DivineLighting(lpTarChar);
				TargetCount++;
			}
		}
	}
	else
	{
		LightningCount = Divine_Lightning_Num[SkillPoint - 1];

		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
		{
			cCount = (DivineLightning_FindCount + cnt)&OTHER_PLAYER_MASK;
			if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0]>0)
			{

				x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

				dist = x*x + y*y + z*z;

				if (dist < (160 * 160))
				{
					AssaParticle_DivineLighting(&chrOtherPlayer[cCount]);
					TargetCount++;
					if (TargetCount >= LightningCount)
					{
						DivineLightning_FindCount = cCount;
						break;
					}
				}
			}
		}

		if (TargetCount < LightningCount && lpChar->smCharInfo.State == smCHAR_STATE_ENEMY)
		{
			x = (lpChar->pX - lpCurPlayer->pX) >> FLOATNS;
			y = (lpChar->pY - lpCurPlayer->pY) >> FLOATNS;
			z = (lpChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

			dist = x*x + y*y + z*z;

			if (dist < (160 * 160))
			{
				AssaParticle_DivineLighting(lpCurPlayer);
				TargetCount++;
			}
		}

	}


	return TargetCount;
}


//����� ����Ʈ�� ��ų ���� ( �ӹ̷ε� )
int SkillPlay_MummyLord_Effect(smCHAR *lpChar, int Range)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	dRange = Range*Range;

	TargetCount = 0;

	if (lpChar->smCharInfo.Brood != smCHAR_MONSTER_USER)
	{
		//���Ͱ� ���� ����
		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
		{
			if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER &&
				chrOtherPlayer[cnt].smCharInfo.Life[0]>0)
			{

				x = (lpChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x*x + y*y + z*z;

				if (dist < dRange)
				{
					ParkAssaParticle_DivineLighting(&chrOtherPlayer[cnt]);
					TargetCount++;
				}

			}

		}

		if (lpChar->smCharInfo.State == smCHAR_STATE_ENEMY)
		{
			x = (lpChar->pX - lpCurPlayer->pX) >> FLOATNS;
			y = (lpChar->pY - lpCurPlayer->pY) >> FLOATNS;
			z = (lpChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

			dist = x*x + y*y + z*z;

			if (dist < dRange)
			{
				ParkAssaParticle_DivineLighting(lpCurPlayer);
				TargetCount++;
			}
		}
	}
	else
	{
		//������ ���� �� ( ���� ���� )
		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
		{
			if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cnt].smCharInfo.Life[0]>0)
			{

				x = (lpChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x*x + y*y + z*z;

				if (dist < dRange)
				{
					ParkAssaParticle_DivineLighting(&chrOtherPlayer[cnt]);
					TargetCount++;
				}

			}
		}
	}


	return TargetCount;
}


//���� ���Ǿ� ��ų ���� ( ����Ʈ ���� )
int SkillPlay_VenomSpear_Effect(smCHAR *lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	smCHAR *lpTarChar;
	int range;

	range = VenomSpear_Range[SkillPoint - 1];
	range *= range;

	TargetCount = 0;

	if (lpChar == lpCurPlayer)
	{
		for (cnt = 0; cnt < DivineLightning_Target_Count; cnt++)
		{
			lpTarChar = FindAutoPlayer(dwSkill_DivineLightning_Target[cnt]);
			if (lpTarChar)
			{
				AssaParticle_VeonmSpear(lpTarChar);
				TargetCount++;
			}
		}
	}
	else
	{
		LightningCount = Divine_Lightning_Num[SkillPoint - 1];

		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
		{
			cCount = (DivineLightning_FindCount + cnt)&OTHER_PLAYER_MASK;
			if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0]>0)
			{

				x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

				dist = x*x + y*y + z*z;

				if (dist < range)
				{
					AssaParticle_VeonmSpear(&chrOtherPlayer[cCount]);
					TargetCount++;
					if (TargetCount >= LightningCount)
					{
						DivineLightning_FindCount = cCount;
						break;
					}
				}
			}
		}
	}


	return TargetCount;
}

//���� ��ų ����Ʈ ���� ( ����Ʈ ���� )
int SkillPlay_Monster_Effect(smCHAR *lpChar, int EffectKind, int Range)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int dDist = Range*Range;


	TargetCount = 0;

	x = (lpChar->pX - lpCurPlayer->pX) >> FLOATNS;
	y = (lpChar->pY - lpCurPlayer->pY) >> FLOATNS;
	z = (lpChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

	dist = x*x + y*y + z*z;

	if (dist < dDist)
	{
		ParkAssaParticle_ChaosKaraSkill_User(lpCurPlayer);	// ī����ī�� �����̾ (����)
		TargetCount++;
	}


	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
			lpChar != &chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER &&
			chrOtherPlayer[cnt].smCharInfo.Life[0]>0)
		{
			x = (lpChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
			y = (lpChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
			z = (lpChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

			dist = x*x + y*y + z*z;

			if (dist < dDist)
			{
				ParkAssaParticle_ChaosKaraSkill_User(&chrOtherPlayer[cnt]);	// ī����ī�� �����̾ (����)
				TargetCount++;
			}
		}
	}

	return TargetCount;
}




//��ų ���� ��Ŷ ����
int RecvProcessSkill(smTRANS_COMMAND *lpTransCommand)
{
	int cnt;
	int dwSkillCode, point;
	smCHAR	*lpChar, *lpChar2;
	POINT3D	Pos1, Pos2;

	dwSkillCode = lpTransCommand->WParam & 0xFF;
	point = (lpTransCommand->WParam >> 8) & 0xF;

	switch (dwSkillCode)
	{
		case SKILL_PLAY_HOLY_REFLECTION:
			//Ȧ�� ���÷��� Ÿ�� ����Ʈ
			sinSkillEffect_Holy_Reflection_Defense(lpCurPlayer);
			break;

		case SKILL_PLAY_ENCHANT_WEAPON:
			//��þƮ ����
			cnt = Enchant_Weapon_Time[point - 1];				//lpTransCommand->LParam-1];
			lpCurPlayer->EnchantEffect_Point = point;		//lpTransCommand->LParam;

			cSkill.SetEnchant_Weapon(cnt, point);

			switch (lpTransCommand->EParam)
			{
				case 0:
					AssaParticle_EnchantWeaponIceJang(lpCurPlayer, cnt);
					lpCurPlayer->WeaponEffect = sITEMINFO_ICE + 1;
					lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt * 1000;
					break;
				case 1:
					AssaParticle_EnchantWeaponLightJang(lpCurPlayer, cnt);
					lpCurPlayer->WeaponEffect = sITEMINFO_LIGHTING + 1;
					lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt * 1000;
					break;
				case 2:
					AssaParticle_EnchantWeaponFireJang(lpCurPlayer, cnt);
					lpCurPlayer->WeaponEffect = sITEMINFO_FIRE + 1;
					lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt * 1000;
					break;
			}
			break;

		case SKILL_PLAY_RESURRECTION:
			//������ �츰��
			if (lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD)
			{
				SendProcessSKillToServer(SKILL_PLAY_RESURRECTION, point, 0, 0);

				lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_RESTART);		//���۸��
				StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_GAME_START1);
				SetDynLight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 100, 100, 100, 0, 200);
				CharPlaySound(lpCurPlayer);

				sinSetLife(lpCurPlayer->smCharInfo.Life[1] / 2);
				ResetEnergyGraph(4);								//������ �׷��� ýũ �ʱ�ȭ
				lpCurPlayer->MoveFlag = FALSE;
				RestartPlayCount = 350;								//5�� ���� ����
				ReStartFlag = 0;
			}
			break;

		case SKILL_PLAY_VIRTUAL_LIFE:
			//����� ������ ����
			cSkill.SetVirtualLife(Virtual_Life_Time[point - 1], point);
			AssaParticle_VirtualLife(lpCurPlayer, Virtual_Life_Time[point - 1]);
			break;

		case SKILL_PLAY_LOVELY_LIFE:
			//����� ���ݷ�
			if (lpTransCommand->LParam == lpCurPlayer->dwObjectSerial)
			{
				cSkill.SetVirtualLife(Virtual_Life_Time[point - 1], point);
				AssaParticle_VirtualLife(lpCurPlayer, Virtual_Life_Time[point - 1]);
				SkillPlaySound(SKILL_SOUND_SKILL_VIRTUAL_LIFE, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
			}
			else
			{
				lpChar = FindChrPlayer(lpTransCommand->LParam);
				if (lpChar)
				{
					AssaParticle_VirtualLife(lpChar, Virtual_Life_Time[point - 1]);
					SkillPlaySound(SKILL_SOUND_SKILL_VIRTUAL_LIFE, lpChar->pX, lpChar->pY, lpChar->pZ);
				}
			}
			break;

		case SKILL_PLAY_VANISH:
			//��Ͻ� ��ų ����
			lpCurPlayer->PlayVanish = 0;
			cSkill.CancelContinueSkill(SKILL_VANISH);

			if (dwM_BlurTime) dwM_BlurTime = dwPlayTime;
			break;

		case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
			//Ʈ���̾���
			cSkill.SetT_Of_Valhalla(point, lpTransCommand->EParam);
			AssaParticle_TriumphOfValhalla(lpCurPlayer, T_Of_Valhalla_Time[point - 1]);
			break;

		case SKILL_PLAY_SPARK_SHIELD:
			//����ũ���� ����Ʈ

			// ������ȣ�� ã�´�
			lpChar = FindChrPlayer(lpTransCommand->LParam);
			lpChar2 = FindChrPlayer(lpTransCommand->SParam);
			if (lpChar && lpChar2)
			{
				//����ũ ����
				if (lpChar->HvLeftHand.PatTool)
				{
					if (lpChar->GetToolBipPoint(&lpChar->HvLeftHand, &Pos1.x, &Pos1.y, &Pos1.z))
					{

						Pos2.x = lpChar2->pX;
						Pos2.y = lpChar2->pY + 24 * fONE;
						Pos2.z = lpChar2->pZ;

						AssaParticle_SparkShieldDefence(&Pos1, &Pos2);
						SkillPlaySound(SKILL_SOUND_SKILL_SPARK2, lpChar->pX, lpChar->pY, lpChar->pZ);
					}
				}
			}
			break;

		case SKILL_PLAY_HALL_OF_VALHALLA:
			//Ȧ������Ҷ�
			cSkill.HellOfValhalla(Hall_Of_Valhalla_Time[point - 1], point, lpTransCommand->SParam, lpTransCommand->EParam, 1);
			SkillValhallaHallOfValhallaHandEffect(lpCurPlayer, (float)lpTransCommand->EParam);
			lpCurPlayer->dwHallOfValhallaTime = dwPlayTime + Hall_Of_Valhalla_Time[point - 1] * 1000;	//Ȧ������Ҷ� �����ð�
			break;

		case SKILL_PLAY_FORCE_OF_NATURE:
			//�������� ������
			cSkill.ForeceOfNature(Force_Of_Nature_Time[point - 1], point, 1);
			SkillSagittarionForceOfNature1(lpCurPlayer, (float)Force_Of_Nature_Time[point - 1]);
			lpCurPlayer->dwForceOfNatureTime = dwPlayTime + Force_Of_Nature_Time[point - 1] * 1000;	//�������� ������ �����ð�
			break;


		case SKILL_PLAY_SOD_ITEM:
			//SOD ��¦ ������ ����Ʈ

			switch (lpTransCommand->SParam)
			{
				case 1:	//��ź
					SetBellatraFontEffect(E_BL_QUAKE_SEAL);
					SetBellatraFontEffect(E_BL_FONT_QUAKE_SEAL);
					EffectWaveCamera(50, 3);
					SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
					break;

				case 2:	//�ð�
					SetBellatraFontEffect(E_BL_STUN_SEAL);
					SetBellatraFontEffect(E_BL_FONT_STUN_SEAL);
					EffectWaveCamera(50, 3);
					SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
					break;

				case 3:	//����
					SetBellatraFontEffect(E_BL_FREEZE_SEAL);
					SetBellatraFontEffect(E_BL_FONT_FREEZE_SEAL);
					EffectWaveCamera(50, 3);
					SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
					break;

				case 4:	//�䳢
					SetBellatraFontEffect(E_BL_RABBIE_SEAL);
					SetBellatraFontEffect(E_BL_FONT_RABBIE_SEAL);
					EffectWaveCamera(50, 3);
					SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
					break;

				case 5:	//�޼���
					SetBellatraFontEffect(E_BL_STYGIAN_SEAL);
					SetBellatraFontEffect(E_BL_FONT_STYGIAN_SEAL);
					break;

				case 6:	//�¾����
					SetBellatraFontEffect(E_BL_GUARDIAN_SEAL);
					SetBellatraFontEffect(E_BL_FONT_GUARDIAN_SEAL);
					break;

				case 7:	//õ������
					SetBellatraFontEffect(E_BL_POINT_SEAL);
					SetBellatraFontEffect(E_BL_FONT_POINT_SEAL);
					break;
			}
			esPlaySound(19, 360);		//SOD ������ �Ҹ�
			break;

		case SKILL_PLAY_HEALING:
			//���� ������ �������
			lpChar = FindChrPlayer(lpTransCommand->LParam);
			if (lpChar)
			{
				sinEffect_Healing2(lpChar);		//���� ����Ʈ
				SkillPlaySound(SKILL_SOUND_SKILL_HEALING, lpChar->pX, lpChar->pY, lpChar->pZ);		//���� ����
			}
			break;

		case SKILL_PLAY_BLESS_SIEGE_ITEM:
			//���� ĳ�� ���� ������ ���

			switch (lpTransCommand->SParam)
			{

				////////////// ������ /////////////////
				case (sinBI1 | sin05) :	//������ũ��
					lpCurPlayer->PlayInvincible = 70 * 30;		//30��
					break;
				case (sinBI1 | sin06) :	//ũ��Ƽ�� ��ũ��
					break;
				case (sinBI1 | sin07) :	//�߰� ȸ�� ��ũ��
					break;

					//////////////// ������ //////////////////

				case (sinBC1 | sin01) :	//������ũ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_INVU);
					lpCurPlayer->PlayInvincible = 70 * 30;		//30��
					break;
				case (sinBC1 | sin02) :	//ũ��Ƽ�� ��ũ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_CRITICAL);
					break;
				case (sinBC1 | sin03) :	//�߰� ȸ�� ��ũ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_EVASION);
					break;
					/*
						case (sinBC1|sin04):	//��Ƽ�� ���� ȸ��
						case (sinBC1|sin05):	//��Ȱ ��ũ��
							return FALSE;
					*/
				case (sinBC1 | sin06) :	//���̾� ũ����Ż ���׼�
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_FIRE_C);
					break;
				case (sinBC1 | sin07) :	//���̽� ũ����Ż ���׼�
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_ICE_C);
					break;
				case (sinBC1 | sin08) :	//����Ʈ�� ũ����Ż ���׼�
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_LIGHTING_C);
					break;
				case (sinBC1 | sin09) :	//�� ������ ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_FIGHTER);
					break;
				case (sinBC1 | sin10) :	//�� ��ī�ϼ� ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_MECHANICIAN);
					break;
				case (sinBC1 | sin11) :	//������ũ�� ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_PIKEMAN);
					break;
				case (sinBC1 | sin12) :	//�� ���� ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_ARCHER);
					break;
				case (sinBC1 | sin13) :	//�� ����Ʈ ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_KNIGHT);
					break;
				case (sinBC1 | sin14) :	//�� ��Ż��Ÿ ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_ATALANTA);
					break;
				case (sinBC1 | sin15) :	//�� ������ ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_MAGICIAN);
					break;
				case (sinBC1 | sin16) :	//�� ������Ƽ�� ���ݷ� ��ȭ��
					//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_PRIESTESS);
					break;
			}
			break;

	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////

//int ReadClanInfo(DWORD dwClanNum)
//CLANINFO ClanInfo[CLAN_MAX]; //Ŭ�� ���� ����
//
//cldata ������Ʈ��
//void Updatecldata();

//Ŭ�� ������Ʈ �䱸 �޼���
//#define	smTRANSCODE_CLAN_UPDATE			0x50320C10


//Ŭ���� ���� ���ۿ� �Լ�
int SendClanCommandUser(smWINSOCK *lpsmSock, DWORD	dwCode, int Param1, int Param2, CLANWONLIST *lpClanUserList, int ClanUserCount)
{

	TRANS_CLAN_COMMAND_USER	TransClanUser;
	int	cnt, len;
	char *lpBuff;
	DWORD	dwSpeedSum;
	int	BuffSize;

	TransClanUser.code = dwCode;

	TransClanUser.Param[0] = Param1;
	TransClanUser.Param[1] = Param2;
	TransClanUser.Param[2] = 0;
	TransClanUser.Param[3] = 0;
	TransClanUser.UserCount = ClanUserCount;

	lpBuff = TransClanUser.szUserBuff;

	for (cnt = 0; cnt < ClanUserCount; cnt++)
	{
		dwSpeedSum = GetSpeedSum(lpClanUserList->clanWon[cnt]);
		((DWORD *)lpBuff)[0] = dwSpeedSum;
		lpBuff += sizeof(DWORD);
		lstrcpy(lpBuff, lpClanUserList->clanWon[cnt]);
		len = lstrlen(lpClanUserList->clanWon[cnt]);
		lpBuff += len + 1;
	}

	BuffSize = lpBuff - TransClanUser.szUserBuff;
	BuffSize += 16;

	TransClanUser.size = 32 + BuffSize;

	if (lpsmSock && ClanUserCount > 0)
	{
		lpsmSock->Send2((char *)&TransClanUser, TransClanUser.size, TRUE);
	}

	return TRUE;
}

//Ŭ���� ���� �ص�
int	RecvClanCommand(TRANS_CLAN_COMMAND_USER *lpTransClanUser, _CLAN_USER_INFO *ClanUserInfo)// char **lpClanUserName , DWORD *dwUserSpeedSum )
{
	int cnt;
	int cnt2;
	char *lpBuff;
	char	ch;

	lpBuff = lpTransClanUser->szUserBuff;

	if (lpTransClanUser->UserCount >= CLAN_USER_MAX) return FALSE;

	for (cnt = 0; cnt < lpTransClanUser->UserCount; cnt++)
	{
		ClanUserInfo[cnt].dwSpeedSum = ((DWORD *)lpBuff)[0];
		lpBuff += sizeof(DWORD);
		for (cnt2 = 0; cnt2 < 32; cnt2++)
		{
			ch = *lpBuff;
			ClanUserInfo[cnt].szName[cnt2] = ch;
			lpBuff++;
			if (!ch) break;
		}
		ClanUserInfo[cnt].szName[31] = 0;
	}

	return TRUE;
}

//����Ʈ ������ �Լ� ���͸�
int FiltQuestItem(TRANS_ITEMINFO	*lpTransItemInfo, DWORD dwPacketCode)
{
	int cnt;

	if (chaQuest.sHaQuestElementary[6].CODE != HAQUEST_CODE_ELEMENTARY_G)
	{

		cnt = cInvenTory.SearchItemCode(sinQT1 | sin16);
		if (cnt >= 1) { DeleteQuestItem(sinQT1 | sin16); }
		if (lpTransItemInfo->Item.CODE == (sinQT1 | sin16)) { return FALSE; }		//���ټ���

	}

	switch (sinQuest_ChangeJob2.CODE)
	{
		case SIN_QUEST_CODE_CHANGEJOB2_NPC_M:
			//���� ������ (�ִ� ���� Ȯ�� )
			if (lpTransItemInfo->Item.CODE == (sinMA2 | sin01))
			{
				if (!cInvenTory.SearchItemCode(sinMA1 | sin01)) return FALSE;		//������ ���°��
				if (cInvenTory.SearchItemCode(sinQT1 | sin04)) return FALSE;		//�ξ����� �ִ� ���
				cnt = cInvenTory.SearchItemCode(sinMA2 | sin01);
				if (cnt >= 9) return FALSE;
			}
			if (lpTransItemInfo->Item.CODE == (sinMA2 | sin02)) return FALSE;
			return TRUE;

		case SIN_QUEST_CODE_CHANGEJOB2_NPC_D:
			//�⸧ ������ (�ִ� ���� Ȯ�� )
			if (lpTransItemInfo->Item.CODE == (sinMA2 | sin02))
			{
				if (!cInvenTory.SearchItemCode(sinMA1 | sin01)) return FALSE;		//������ ���°��
				if (cInvenTory.SearchItemCode(sinQT1 | sin05)) return FALSE;		//�߸��� �ִ� ���
				cnt = cInvenTory.SearchItemCode(sinMA2 | sin02);
				if (cnt >= 7) return FALSE;
			}
			if (lpTransItemInfo->Item.CODE == (sinMA2 | sin01)) return FALSE;
			return TRUE;
	}

	//3�� ���� ����Ʈ
	//����
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin06))
	{
		if (sinQuest_ChangeJob3.CODE == SIN_QUEST_CODE_CHANGEJOB3)
		{
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin06);
			if (cnt >= 3) return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	//���� ����Ʈ�� ���� 1
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin09))
	{
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2)
		{
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin09);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	//���� ����Ʈ�� ���� 2
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin10))
	{
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2)
		{
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin10);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	//���� ����Ʈ�� ���� 3
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin11))
	{
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2)
		{
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin11);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}


	//3�� ���� ����Ʈ�� 
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin13))
	{

		if (sinQuest_ChangeJob4.CODE == SIN_QUEST_CODE_CHANGEJOB4 && sinQuest_ChangeJob4.State == 5)
		{
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin13);
			if (cnt >= 1 || CheckAttMonsterCode(sinQuest_ChangeJob4.Kind) == FALSE)
				return FALSE;

			return TRUE;
		}
		return FALSE;
	}



	//������ ��ġ ����Ʈ 
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin16))
	{

		if (chaQuest.sHaQuestElementary[6].CODE == HAQUEST_CODE_ELEMENTARY_G)
		{
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin16);
			if (cnt >= 1)
				return FALSE;

			if (dwPacketCode == smTRANSCODE_GETITEM)
			{
				//������ ȹ�� �޼���
				char szBuff[256];
				//wsprintf( szBuff, mgGetQuestItem , lpTransItemInfo->Item.ItemName );//_ignore_//�ؿ�
				AddChatBuff(szBuff, 6);
			}
			return TRUE;
		}
		return FALSE;
	}


	if ((lpTransItemInfo->Item.CODE&sinITEM_MASK2) == sinMA1 ||
		(lpTransItemInfo->Item.CODE&sinITEM_MASK2) == sinMA2)
	{
		//����Ʈ�� ���°�� ����Ʈ ������ ���� ���

		return FALSE;
	}

	return TRUE;
}

POINT GetProcessModule();

//���˻� �Ͽ� ������ ������
int CheckProcessModule()
{
	POINT p;
	OSVERSIONINFO VersionInfo;
	BOOL Result;
	smTRANS_COMMAND	smTransCommand;


	return TRUE;

	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CLIENTINFO;

	Result = GetVersionEx(&VersionInfo);
	p = GetProcessModule();

	if (Result != FALSE)
	{
		smTransCommand.WParam = VersionInfo.dwPlatformId;
		smTransCommand.LParam = (VersionInfo.dwMajorVersion * 100) + VersionInfo.dwMinorVersion;
		smTransCommand.LParam = (smTransCommand.LParam << 16) | (VersionInfo.dwBuildNumber & 0xFFFF);
		smTransCommand.SParam = p.x;
		smTransCommand.EParam = p.y;

		smTransCommand.WParam ^= ((DWORD *)(UserAccount + 0))[0];
		smTransCommand.LParam ^= ((DWORD *)(UserAccount + 1))[0];
		smTransCommand.SParam ^= ((DWORD *)(UserAccount + 2))[0];
		smTransCommand.EParam ^= ((DWORD *)(UserAccount + 3))[0];

		if (smWsockDataServer) smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
	}

	return TRUE;
}

//�Ȱ� ������ ������
int SendFalconDamage(smCHAR *lpChar)
{
	DWORD point;
	int cnt;

	if (lpChar == lpCurPlayer && lpChar->chrAttackTarget)
	{
		point = ((SkillFalconPoint ^ ((DWORD)chrOtherPlayer^lpCurPlayer->dwObjectSerial)) + 1) << 8;
		point |= SKILL_PLAY_FALCON;
		cnt = dm_SendTransDamage(lpChar->chrAttackTarget, 0, 0, 0, 0, point, FALSE);
		DamageExp(lpChar->chrAttackTarget, cnt);
		return cnt;
	}
	return NULL;
}

//��̼ҵ� ������ ������
int SendDancingSwordDamage(smCHAR *lpChar)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar)
	{

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = 1 << 8;
		point |= SKILL_PLAY_DANCING_SWORD;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		if ((rand() % 2) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK1, lpChar->pX, lpChar->pY, lpChar->pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK2, lpChar->pX, lpChar->pY, lpChar->pZ);

		return cnt;
	}
	return FALSE;
}


//�ޱ׳�ƽ ���Ǿ� ������ ������
int SendMegneticSphereDamage(smCHAR *lpChar)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar)
	{

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = 1 << 8;
		point |= SKILL_PLAY_MAGNETIC_SPHERE;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		if ((rand() % 2) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK1, lpChar->pX, lpChar->pY, lpChar->pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK2, lpChar->pX, lpChar->pY, lpChar->pZ);

		return cnt;
	}
	return FALSE;
}

//�ӽ��� ������ ������
int SendMuspellDamage(smCHAR *lpChar)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar)
	{

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = 1 << 8;
		point |= SKILL_PLAY_SUMMON_MUSPELL;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		if ((rand() % 2) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK1, lpChar->pX, lpChar->pY, lpChar->pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK2, lpChar->pX, lpChar->pY, lpChar->pZ);

		return cnt;
	}
	return FALSE;
}



//������ �� ������
int SendLowLevelPetDamage(smCHAR *lpChar, int petType)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar)
	{

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = (1 + petType) << 8;
		point |= SKILL_PLAY_PET_ATTACK;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		return cnt;
	}
	return FALSE;
}


//PC��� �� ������
int SendPCBangPetDamage(smCHAR *lpChar, int petType)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar)
	{

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = (1 + petType) << 8;
		point |= SKILL_PLAY_PET_ATTACK2;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		return cnt;
	}
	return FALSE;
}



//������ �ù� ���� ���� �䱸
int	SendItemExpress(DWORD	dwItemCode, char *szPassCode)
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory((char *)&smTransCommand, sizeof(smTRANS_COMMAND));
	smTransCommand.code = smTRANSCODE_ITEM_EXPRESS;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwItemCode;

	if (szPassCode)
		smTransCommand.LParam = GetSpeedSum(szPassCode);

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}

//������ �ù� ���� ���� �䱸
int	SendItemExpress(DWORD	dwItemCode)
{
	return	SendItemExpress(dwItemCode, 0);
}


//��ȣ ��� ������ ������
int SendClanYahooMotion()
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK *lpsmSock;

	if (lpCurPlayer->smCharInfo.ClassClan)
	{
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_YAHOO_MOTION;
		smTransCommand.WParam = dwPlayTime;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		lpsmSock = GetAreaServerSock();		//�������� ������ ã��
		if (lpsmSock)
			lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

		return TRUE;

	}
	return FALSE;
}

//�� ����Ʈ ������ ������ ������
int	SendStarPointToServer(int Price, int Cash)
{
	Log::Debug("SendStarPointToServer : Requisitado por�m foi removido.");
	return NULL;
}
//����ѵ��� ������ ������
int	SendGiveMoneyToServer(int Money)
{
	Log::Debug("SendGiveMoneyToServer : Requisitado por�m foi removido.");
	return NULL;
}
//Ŭ���Ӵ� ������ ������ ������
int	SendClanMoneyToServer(int Money, int Flag, int Kind)
{
	Log::Debug("SendClanMoneyToServer : Requisitado por�m foi removido.");
	return NULL;
}
int	SendClanMoneyToServer(int Money, int Flag)
{
	Log::Debug("SendClanMoneyToServer : Requisitado por�m foi removido.");
	return NULL;
}
//�̿��� ������ ������ ������
int	SendPaymentMoneyToServer(int Money, int Flag)
{
	Log::Debug("SendPaymentMoneyToServer : Requisitado por�m foi removido.");
	return NULL;
}


/*
//���λ��� ����
#define smTRANSCODE_OPEN_PERTRADE	0x48478A90
//���λ��� �θ���
#define smTRANSCODE_CALL_PERTRADE	0x48478AA0
//���λ��� ������
#define	smTRANSCODE_MYSHOP_ITEM		0x48478AB0

int	Send_ShowMyShopItem( smCHAR *lpChar );							//���λ��� ������ ���� �䱸
int	Send_MyShopItemList( smTRANS_COMMAND	*lpTransCommand );		//���λ��� ������ ���� ���濡 ������
int	Recv_MyShopItemList( TRANS_TRADEITEMS *lpTransTradeItems );		//���λ��� ������ ���� ����
*/


//���λ��� ������ ���� �䱸
int	Send_ShowMyShopItem(smCHAR *lpChar)
{
	smTRANS_COMMAND	smTransCommand;
	int	result;
	smWINSOCK	*lpsmSock;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CALL_MYSHOP;
	smTransCommand.WParam = 0;
	smTransCommand.LParam = lpChar->dwObjectSerial;
	smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smTransCommand.EParam = 0;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

	return result;
}

//���λ��� ������ ���� ���濡 ������
int	Send_MyShopItemList(smTRANS_COMMAND	*lpTransCommand)
{
	int	result;
	smWINSOCK	*lpsmSock;

	TransTradeItems_MyShop.dwRecver = lpTransCommand->SParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char *)&TransTradeItems_MyShop, TransTradeItems_MyShop.size, TRUE);

	return result;
}


//���λ��� ������ ���� ����
int	Recv_MyShopItemList(TRANS_TRADEITEMS *lpTransTradeItems)
{
	sMYSHOP MyShopItem;
	smCHAR	*lpChar;

	//���� ����Ÿ �ص� ( Z/NZ ��� )
	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE *)&MyShopItem);

	//��ľ� �Լ� ȣ�� ����
	cCharShop.RecvShopItem(&MyShopItem);


	lpChar = FindAutoPlayer(lpTransTradeItems->dwSender);

	if (lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0])
	{
		OpenEachPlayer(lpChar);
		DispEachMode = TRUE;
	}

	return TRUE;
}


//���� ���� ���� ����
int	Send_PersonalShopItem(DWORD dwCharCode, void *lpShopItem)
{
	smTRANS_COMMAND_BUFF	smtBuff;


	smtBuff.smTransCommand.code = smTRANSCODE_MYSHOP_TRADE;
	smtBuff.smTransCommand.size = sizeof(smTRANS_COMMAND) + sizeof(sMYSHOP_ITEM_SERVER);
	smtBuff.smTransCommand.WParam = 0;
	smtBuff.smTransCommand.LParam = dwCharCode;
	smtBuff.smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smtBuff.smTransCommand.EParam = 0;

	memcpy(smtBuff.Buff, lpShopItem, sizeof(sMYSHOP_ITEM_SERVER));

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char *)&smtBuff, smtBuff.smTransCommand.size, TRUE);

	return FALSE;
}

//���� ���� ������ ���� ������Ʈ
int	UpdateMyShopList(void *lpMyShop)
{
	sMYSHOP *lpMyShopItem = (sMYSHOP *)lpMyShop;
	int	result = 0;
	int		len;

	ZeroMemory(&TransTradeItems_MyShop, sizeof(TRANS_TRADEITEMS));

	//����Ÿ ���� ( Z/NZ ��� )
	len = EecodeCompress((BYTE *)lpMyShopItem, TransTradeItems_MyShop.TradeBuff, sizeof(sMYSHOP));
	if (len > TRANS_TRADE_BUFF_SIZE) return FALSE;

	TransTradeItems_MyShop.code = smTRANSCODE_MYSHOP_ITEM;
	TransTradeItems_MyShop.size = len + 48;
	TransTradeItems_MyShop.dwSum = 0;
	TransTradeItems_MyShop.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems_MyShop.dwRecver = 0;
	TransTradeItems_MyShop.Temp[0] = 0;
	TransTradeItems_MyShop.Temp[1] = 0;
	TransTradeItems_MyShop.Temp[2] = 0;
	TransTradeItems_MyShop.Temp[3] = 0;

	return TRUE;
}

//���� ���� ����
int SendOpenPersonalTrade(char *szTradeMsg, void *lpPersTrade)
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;
	DWORD	dwCode;
	sMYSHOP *lpMyShopItem = (sMYSHOP *)lpPersTrade;
	int		len;

	//smTRANSCODE_OPEN_PERTRADE
	//lpMyShopItem->

	ZeroMemory(&TransTradeItems_MyShop, sizeof(TRANS_TRADEITEMS));
	//memcpy( &TransMyShop.sMyShop , lpMyShopItem , sizeof(sMYSHOP) );

	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	//����Ÿ ���� ( Z/NZ ��� )
	len = EecodeCompress((BYTE *)lpMyShopItem, TransTradeItems_MyShop.TradeBuff, sizeof(sMYSHOP));
	if (len > TRANS_TRADE_BUFF_SIZE) return FALSE;

	TransTradeItems_MyShop.code = smTRANSCODE_MYSHOP_ITEM;
	TransTradeItems_MyShop.size = len + 48;
	TransTradeItems_MyShop.dwSum = 0;
	TransTradeItems_MyShop.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems_MyShop.dwRecver = 0;
	TransTradeItems_MyShop.Temp[0] = 0;
	TransTradeItems_MyShop.Temp[1] = 0;
	TransTradeItems_MyShop.Temp[2] = 0;
	TransTradeItems_MyShop.Temp[3] = 0;


	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy(TransChatMessage.szMessage, szTradeMsg);

	dwCode = GetSpeedSum(szTradeMsg);
	TransChatMessage.dwIP = dwCode;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		//�ؿ� �߱�(���λ����̸�����)
		//#ifdef _LANGUAGE_CHINESE
		//		ConvertStringTHAI((char *)&TransChatMessage, lstrlen((char *)&TransChatMessage)+16);
		//#endif
		result = lpsmSock->Send2((char *)&TransChatMessage, TransChatMessage.size, TRUE);

	if (result)
	{
		/*
		//�ؿ� �߱�(���λ����̸�����)
		#ifdef _LANGUAGE_CHINESE
				ConvertStringTHAI(szTradeMsg, lstrlen(szTradeMsg)+16);
		#endif
		*/
		wsprintf(lpCurPlayer->szTradeMessage, "%s: %s", lpCurPlayer->smCharInfo.szName, szTradeMsg);
		lpCurPlayer->dwTradeMsgCode = dwCode;
	}

	return result;
}

//���� �ݱ�
int SendClosePersonalTrade()
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;

	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char *)&TransChatMessage, TransChatMessage.size, TRUE);

	lpCurPlayer->dwTradeMsgCode = 0;
	lpCurPlayer->szTradeMessage[0] = 0;

	return result;
}

// �庰 - �Է�â
int SendsServerDoc(char *szTradeMsg)
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;


	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	TransChatMessage.code = smTRANSCODE_ITEMDOC;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy(TransChatMessage.szMessage, szTradeMsg);

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char *)&TransChatMessage, TransChatMessage.size, TRUE);

	return result;
}


//��¥ Ȯ�� ���� ������ 
int DeleteEventItem_TimeOut(sITEMINFO	*lpsItem)
{
	// ����� - ����¡ ������, ��ų ������ �߰�
	if (lpsItem->CODE == (sinBI1 | sin32) || lpsItem->CODE == (sinBI1 | sin33) || lpsItem->CODE == (sinBI1 | sin34) || lpsItem->CODE == (sinBI1 | sin51) ||// pluto ��(�ؿ�)
	//	lpsItem->CODE == (sinBI1|sin35) || lpsItem->CODE == (sinBI1|sin36) || lpsItem->CODE == (sinBI1|sin37) || lpsItem->CODE == (sinBI1|sin38) )	//��ǥ
		lpsItem->CODE == (sinBI1 | sin36) || lpsItem->CODE == (sinBI1 | sin37) ||
		lpsItem->CODE == (sinBI1 | sin38) || lpsItem->CODE == (sinBI1 | sin39) ||
		lpsItem->CODE == (sinBI1 | sin40) || lpsItem->CODE == (sinBI1 | sin41) ||
		lpsItem->CODE == (sinBI1 | sin42) || lpsItem->CODE == (sinBI1 | sin43) ||
		lpsItem->CODE == (sinBI1 | sin44) || lpsItem->CODE == (sinBI1 | sin45) ||
		lpsItem->CODE == (sinBI1 | sin46) || lpsItem->CODE == (sinBI1 | sin47) ||
		lpsItem->CODE == (sinBI1 | sin48) || lpsItem->CODE == (sinBI1 | sin49) ||
		lpsItem->CODE == (sinBI1 | sin50) || lpsItem->CODE == (sinBI1 | sin52) || // ����� - ���� ����� �� �߰�
		lpsItem->CODE == (sinBI1 | sin53) || lpsItem->CODE == (sinBI1 | sin54) ||

		lpsItem->CODE == (sinBI1 | sin65) || lpsItem->CODE == (sinBI1 | sin66) || // �庰 - ���� ����� �� (7��, 1��)
		lpsItem->CODE == (sinBI1 | sin67) || lpsItem->CODE == (sinBI1 | sin68) ||
		lpsItem->CODE == (sinBI1 | sin69) || lpsItem->CODE == (sinBI1 | sin70) ||
		lpsItem->CODE == (sinBI1 | sin71) || lpsItem->CODE == (sinBI1 | sin72) ||
		lpsItem->CODE == (sinBI1 | sin55) || lpsItem->CODE == (sinBI1 | sin77) ||	// �庰 - �Ǵн���(1�ð�)
		lpsItem->CODE == (sinBI1 | sin62) || lpsItem->CODE == (sinBI1 | sin63) ||	// ����� - ����¡ ������(2��) ������ �߰� D, E
		lpsItem->CODE == (sinBI1 | sin64) ||										// ����� - ����¡ ������(2��) ������ �߰� F
		lpsItem->CODE == (sinBI1 | sin73) || lpsItem->CODE == (sinBI1 | sin74) || // �庰 - ���� ����� �� �߰�(�׸� 1�ð� �� 4��)
		lpsItem->CODE == (sinBI1 | sin75) || lpsItem->CODE == (sinBI1 | sin76)) // �庰 - ���� ����� �� (1�ð�)
	{
		return FALSE;
	}

	struct	tm	tm;
	time_t		ttm;//,ttm2;
	DWORD		dwCreateTime;

	DWORD		dwItemCode;

	dwItemCode = lpsItem->CODE&sinITEM_MASK2;

	DWORD NowTime = GetPlayTime_T(); // ���� �ð�(second����)

	//�Ѻ� 14��
	if (lpsItem->CODE == (sinDA1 | sin48) || lpsItem->CODE == (sinDA2 | sin48))
	{
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 14) < NowTime)
			return true;
	}
	//�ڽ�Ƭ 7��
	if (lpsItem->CODE == (sinDA1 | sin31) || lpsItem->CODE == (sinDA1 | sin33) ||
		lpsItem->CODE == (sinDA1 | sin35) || lpsItem->CODE == (sinDA1 | sin37) ||
		lpsItem->CODE == (sinDA1 | sin39) || lpsItem->CODE == (sinDA1 | sin41) ||
		lpsItem->CODE == (sinDA1 | sin43) || lpsItem->CODE == (sinDA1 | sin45) ||
		lpsItem->CODE == (sinDA2 | sin31) || lpsItem->CODE == (sinDA2 | sin33) ||
		lpsItem->CODE == (sinDA2 | sin35) || lpsItem->CODE == (sinDA2 | sin37) ||
		lpsItem->CODE == (sinDA2 | sin39) || lpsItem->CODE == (sinDA2 | sin41) ||
		lpsItem->CODE == (sinDA2 | sin43) || lpsItem->CODE == (sinDA2 | sin45) ||
		lpsItem->CODE == (sinDB1 | sin31) || // ����� - ���ǵ� ����(7��) �߰�
		lpsItem->CODE == (sinOA2 | sin31) || // ����� - ���� �ϸ�(7��) �߰�
		lpsItem->CODE == (sinOR2 | sin31) || // ����� - ���� ���� �� �߰�(�ٺ�)
		lpsItem->CODE == (sinOR2 | sin32))  // ����� - ���� ���� �� �߰�(ǻ��)
	{
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 7) < NowTime)
			return true;
	}
	//�ڽ�Ƭ 30��
	if (lpsItem->CODE == (sinDA1 | sin32) || lpsItem->CODE == (sinDA1 | sin34) ||
		lpsItem->CODE == (sinDA1 | sin36) || lpsItem->CODE == (sinDA1 | sin38) ||
		lpsItem->CODE == (sinDA1 | sin40) || lpsItem->CODE == (sinDA1 | sin42) ||
		lpsItem->CODE == (sinDA1 | sin44) || lpsItem->CODE == (sinDA1 | sin46) ||
		lpsItem->CODE == (sinDA1 | sin54) || lpsItem->CODE == (sinDA1 | sin55) || // ����� - ������ ���� �߰�
		lpsItem->CODE == (sinDA2 | sin32) || lpsItem->CODE == (sinDA2 | sin34) ||
		lpsItem->CODE == (sinDA2 | sin36) || lpsItem->CODE == (sinDA2 | sin38) ||
		lpsItem->CODE == (sinDA2 | sin40) || lpsItem->CODE == (sinDA2 | sin42) ||
		lpsItem->CODE == (sinDA2 | sin44) || lpsItem->CODE == (sinDA2 | sin46) ||
		lpsItem->CODE == (sinDA2 | sin54) || lpsItem->CODE == (sinDA2 | sin55) || // ����� - ������ ���� �߰�
		lpsItem->CODE == (sinDB1 | sin32) || // ����� - ���ǵ� ����(30��) �߰�
		lpsItem->CODE == (sinOA2 | sin32) || // ����� - ���� �ϸ�(30��) �߰�
		lpsItem->CODE == (sinSP1 | sin34))  // ����� - ȣ���� ĸ��(30��) �߰�

	{
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 30) < NowTime)
			return true;
	}

	// 1��
	if (lpsItem->CODE == (sinDB1 | sin33) || lpsItem->CODE == (sinOA2 | sin33)) // �庰 - ���ǵ� ����(1��) �߰� // �庰 - ���� �ϸ�(1��)
	{
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 1) < NowTime)
			return true;
	}

	// 1�ð�
	if (lpsItem->CODE == (sinDB1 | sin34) || lpsItem->CODE == (sinOA2 | sin34)) // �庰 - ���ǵ� ����(1�ð�) �߰� // �庰 - ���� �ϸ�(1�ð�)
	{
		if (lpsItem->dwCreateTime + (60 * 60) < NowTime)
			return true;
	}

	//�ۼ��� : �����, �ۼ��� : 08.04.07
	//��  �� : Ŭ��ġ�� �� ������ 5���Ŀ� ����
	if (lpsItem->CODE == (sinOR2 | sin06) || lpsItem->CODE == (sinOR2 | sin07) ||
		lpsItem->CODE == (sinOR2 | sin08) || lpsItem->CODE == (sinOR2 | sin09) ||
		lpsItem->CODE == (sinOR2 | sin10) || lpsItem->CODE == (sinOR2 | sin11) ||
		lpsItem->CODE == (sinOR2 | sin12) || lpsItem->CODE == (sinOR2 | sin13) ||
		lpsItem->CODE == (sinOR2 | sin14) || lpsItem->CODE == (sinOR2 | sin15) ||
		lpsItem->CODE == (sinOR2 | sin16) || lpsItem->CODE == (sinOR2 | sin17) ||
		lpsItem->CODE == (sinOR2 | sin18) || lpsItem->CODE == (sinOR2 | sin19) ||
		lpsItem->CODE == (sinOR2 | sin20) || lpsItem->CODE == (sinOR2 | sin21) ||
		lpsItem->CODE == (sinOR2 | sin22) || lpsItem->CODE == (sinOR2 | sin23) ||
		lpsItem->CODE == (sinOR2 | sin24) || lpsItem->CODE == (sinOR2 | sin25))
	{
		// �����ð� + 5�� < ����ð�
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 5) < NowTime)
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.12.08
	//��  �� : ��Ÿ ��, ��Ÿ �ƹķ� ������ 3�� �Ŀ� ����
	if (lpsItem->CODE == (sinOR2 | sin27) || lpsItem->CODE == (sinOA1 | sin32))  // ��Ÿ ��, ��Ÿ �ƹķ� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� : ��Ÿ��, ��Ÿ�ƹķ��� �ͽ��ĵǸ鼭 �ð��� �ʱ�ȭ �Ǿ� ������ ������Ű�� ����.
			// �����ð� + 3�� < ����ð�
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 3) < NowTime)
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.07.31
	//��  �� : �̺�Ʈ ��, �̺�Ʈ �ƹķ� ������ 7�� �Ŀ� ����
	if (lpsItem->CODE == (sinOR2 | sin28) || lpsItem->CODE == (sinOA1 | sin33))  // �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�
	{
		// �����ð� + 7�� < ����ð�
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 7) < NowTime)
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.12.17
	//��  �� : �̺�Ʈ ��, �̺�Ʈ �ƹķ� ������ 1�ð� �Ŀ� ����
	if (lpsItem->CODE == (sinOR2 | sin29) || lpsItem->CODE == (sinOA1 | sin34))  // �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� - �̺�Ʈ��, �̺�Ʈ�ƹķ� ������ ������Ű�� ����.
		//		return TRUE;
		// �����ð� + 1�ð� < ����ð�
		if (lpsItem->dwCreateTime + (60 * 60) < NowTime)
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.12.17
	//��  �� : �̺�Ʈ ��, �̺�Ʈ �ƹķ� ������ 1�� �Ŀ� ����
	if (lpsItem->CODE == (sinOR2 | sin30) || lpsItem->CODE == (sinOA1 | sin35))  // �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� - �̺�Ʈ��, �̺�Ʈ�ƹķ� ������ ������Ű�� ����.
		//		return TRUE;
		// �����ð� + 1�� < ����ð�
		if (lpsItem->dwCreateTime + (60 * 60 * 24) < NowTime)
			return TRUE;
	}

	//�ۼ��� : �庰, �ۼ��� : 10.02.02
	//��  �� : ��Ʈ�� ������ 7�� �Ŀ� ����
	if (lpsItem->CODE == (sinOR2 | sin33))  // ��Ʈ�� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� - �̺�Ʈ��, �̺�Ʈ�ƹķ� ������ ������Ű�� ����.
		//		return TRUE;
		// �����ð� + 7�� < ����ð�
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 7) < NowTime)
			return TRUE;
	}


	// 7��
	if (lpsItem->CODE == (sinOA1 | sin36) || lpsItem->CODE == (sinOA1 | sin37) || // �庰 - ���� �ƹķ�, ��Ʈ �ƹķ�
		lpsItem->CODE == (sinOR2 | sin36) || lpsItem->CODE == (sinOR2 | sin37) || // �庰 - �ҿｺ��
		lpsItem->CODE == (sinOR2 | sin38) || lpsItem->CODE == (sinOR2 | sin39) ||
		lpsItem->CODE == (sinOR2 | sin40))
	{
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 7) < NowTime)
			return TRUE;
	}

	// �庰 - �ҿｺ�� 14�� // �庰 - ���� �̺�Ʈ �ƹķ�, ��
	if (lpsItem->CODE == (sinOA1 | sin39) ||
		lpsItem->CODE == (sinOA1 | sin40) || lpsItem->CODE == (sinOA1 | sin41) ||
		lpsItem->CODE == (sinOA1 | sin42) ||
		lpsItem->CODE == (sinOA1 | sin38) || lpsItem->CODE == (sinOR2 | sin34) || lpsItem->CODE == (sinOR2 | sin35))
	{
		if (lpsItem->dwCreateTime + (60 * 60 * 24 * 14) < NowTime)
			return TRUE;
	}



	//(�������� , ������� ��¥ Ȯ���Ͽ� ����)
	if (lpsItem->CODE != (sinOR2 | sin01) &&
		dwItemCode != sinPZ1 && dwItemCode != sinPZ2)
		return FALSE;

#ifdef _LANGUAGE_THAI_DELETE_RING
	if (lpsItem->CODE == (sinOR2 | sin01)) return TRUE;
#endif

	tm.tm_year = 2004 - 1900;	// 2004��
	tm.tm_mon = 4 - 1;			// 4��
	tm.tm_mday = 1;			// 1��
	tm.tm_hour = 15;			// 15��
	tm.tm_min = 30;				// 30��
	tm.tm_sec = 0;				// 0��
	ttm = mktime(&tm);		//���� �ð�

	dwCreateTime = (DWORD)ttm;

	if (lpsItem->dwCreateTime < dwCreateTime) return TRUE;

	return FALSE;
}

int RecvSodGameInfomation(void *Info)
{
	smTRANS_COMMAND_SOD	*lpTransCommand = (smTRANS_COMMAND_SOD *)Info;

	if (!BellatraEffectInitFlag)
	{
		CreateBellatraFontEffect();
		BellatraEffectInitFlag = TRUE;
	}

	switch (lpTransCommand->smTransCommand.WParam)
	{
		case 1:
			if (SoD_SetFontEffect.eBL_Type == E_BL_LODING)
			{
				CheckBellatraFontEffect(&SoD_SetFontEffect, TRUE);	//�ε� ����
			}

			if (lpTransCommand->smTransCommand.SParam < 0)
			{
				//SoD ���� ����Ʈ
				//Sod_SodEffect( lpTransCommand->smTransCommand.LParam+1 );
				sSodScore.EffectStartCount = 70 * 3;
				sSodScore.EffectCode = lpTransCommand->smTransCommand.LParam + 1;
			}
			else
			{
				if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) break;		//������� ����

				sSodScore.SodNextStageNum = lpTransCommand->smTransCommand.SParam;
				sSodScore.dwSoD_NextStageTime = dwPlayTime + 9000;
				sSodScore.dwSoD_CloseStageTime = dwPlayTime + 5000;
				sSodScore.NextRound = lpTransCommand->smTransCommand.LParam + 1;

				StopBGM();
				esPlayContSound(14);
				lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);

				SetBellatraFontEffect(E_BL_FONT_STAGE);
				SetBellatraFontEffect(E_BL_FONT_COMPLETE);
				SoDGateFlag = TRUE;
			}
			break;

		case 3:
			//���� ���ư���
			if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) break;		//������� ����

			sSodScore.SodNextStageNum = -1;
			sSodScore.dwSoD_NextStageTime = dwPlayTime + 8000;
			sSodScore.dwSoD_CloseStageTime = dwPlayTime + 6000;
			sSodScore.NextRound = -1;

			StopBGM();
			esPlayContSound(15);

			SetBellatraFontEffect(E_BL_FONT_STAGE);
			SetBellatraFontEffect(E_BL_FONT_FAIL);
			SoDGateFlag = TRUE;
			break;

		case 4:
			//��� Ŭ����
			if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) break;		//������� ����

			sSodScore.SodNextStageNum = lpTransCommand->smTransCommand.SParam;
			sSodScore.dwSoD_NextStageTime = dwPlayTime + 7000;
			sSodScore.NextRound = lpTransCommand->smTransCommand.LParam + 1;

			StopBGM();
			esPlayContSound(16);
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);

			//SetBellatraFontEffect( E_BL_FONT_STAGE );
			SetBellatraFontEffect(E_BL_FONT_CON);
			SoDGateFlag = FALSE;

			//ȹ���� �ݾ� ���
			char szBuff[128];
			wsprintf(szBuff, mgSOD_Clear, lpTransCommand->smTransCommand.EParam);
			cMessageBox.ShowMessageEvent(szBuff);

			break;

		case 2:
			//���� ��� ����
			if (lpTransCommand->smTransCommand.LParam)
			{
				//������ ȹ�濡 ���� ���� ����
				sSodScore.ScoreEffectCount = 255;

			}
			else
			{
				//�ֱ������� �������� ��ü ����


			}

			if (!sSodScore.dwSoD_NextStageTime)
			{
				if (sSodScore.Round != lpTransCommand->smTransCommand.SParam)
				{
					switch (lpTransCommand->smTransCommand.SParam)
					{
						case 1:		//1���� ����
							sSodScore.dwPlayTime = dwPlayTime;
							PlayBGM_Direct(BGM_CODE_SOD1);
							break;

						case 4:
							PlayBGM_Direct(BGM_CODE_SOD2);
							break;

						case 7:
							PlayBGM_Direct(BGM_CODE_SOD3);
							break;
					}
				}
				sSodScore.Round = lpTransCommand->smTransCommand.SParam;
			}

			sSodScore.dwDispTime = dwPlayTime + 10 * 1000;
			sSodScore.MyTeam = lpTransCommand->MyTeam;
			sSodScore.Score = lpTransCommand->MyScore;
			sSodScore.TeamScore[0] = lpTransCommand->TeamScore[0];
			sSodScore.TeamScore[1] = lpTransCommand->TeamScore[1];
			sSodScore.TeamScore[2] = lpTransCommand->TeamScore[2];
			sSodScore.TeamScore[3] = lpTransCommand->TeamScore[3];

			break;

		case smCODE_SOD_EFFECT:					//�ֹ� �ý��� �߰� - SOD ����Ʈ ����
			if (!BellatraEffectInitFlag)
			{
				CreateBellatraFontEffect();
				BellatraEffectInitFlag = TRUE;
			}
			SetBellatraFontEffect((EBL_FontEffectType)lpTransCommand->smTransCommand.LParam);
			break;

	}

	return TRUE;
}

//�������� ������ �Լ�
int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2)
{
	TRANS_ITEMINFO_GROUP	TransItemGroup;

	ZeroMemory(&TransItemGroup, sizeof(TRANS_ITEMINFO_GROUP));
	memcpy(&TransItemGroup, lpTransItemGroup2, TRANS_GROUPITEM_HEADER_SIZE);

	DecodeCompress((BYTE *)lpTransItemGroup2->szBuff, (BYTE *)TransItemGroup.sItemInfo, sizeof(sITEMINFO)*TRANS_GROUPITEM_MAX);

	//�ڻ�� �Լ� ȣ��
	//���� �������� �޴´�
	sinRecvForceOrb(TransItemGroup.sItemInfo, TransItemGroup.ItemCount);


	return TRUE;
}

//����Ʈ ��� ������
int SendQuestCommandToServer(DWORD dwQuestCode, int Param1, int Param2, int Param3)
{
	Log::Debug("SendQuestCommandToServer : Requisitado por�m foi removido.");
	return NULL;
}
//���� ����Ʈ ����
int Start_QuestArena(DWORD dwQuestCode, int Param1, int Param2)
{
	smTRANS_COMMAND	smTransCommand;
	int	result = 0;
	smWINSOCK *lpsmSock;

	//if ( dwQuestCode==SIN_QUEST_CODE_CHANGEJOB4 ) {

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_QUEST_COMMAND;
	smTransCommand.WParam = dwQuestCode;
	smTransCommand.LParam = QUEST_ARENA_FIELD;
	smTransCommand.SParam = Param1;
	smTransCommand.EParam = Param2;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

	if (result)
		WarpField2(QUEST_ARENA_FIELD);

	//}

	return FALSE;
}

//����ĳ�� �������� ������ ����
int	SendBlessCastleToServer(smTRANS_BLESSCASTLE *lpBlessCastleSetup, int Mode)
{
	Log::Debug("SendBlessCastleToServer : Requisitado por�m foi removido.");
	return NULL;
}
int	Send_GetBlessCastleTax()
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));

	smTransCommand.code = smTRANSCODE_BLESSCASTLE_INFO;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.SParam = cSinSiege.GetTaxRate();

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}
//����ĳ�� �������� ����
int RecvBlessCastInfo(void *lpPacket)
{
	smTRANS_BLESSCASTLE *lpBlessCastleSetup = (smTRANS_BLESSCASTLE *)lpPacket;
	rsUSER_LIST_TOP10	*lpClanListTop10;

	switch (lpBlessCastleSetup->smTransCommand.WParam)
	{
		case 1:		//�������� ����
			chaSiege.ShowSiegeMenu(lpBlessCastleSetup);
			break;

		case 2:		//���� ���� ����
			lpClanListTop10 = (rsUSER_LIST_TOP10 *)lpPacket;
			//���� ��� �Լ� ȣ��
			chaSiege.ShowSiegeScore(lpClanListTop10);
			break;
	}

	return TRUE;
}

//���� ĳ�� ������ Ŭ�� ����
int SetBlessCastleMaster(DWORD dwClanCode, int Mode)
{

	if (!Mode && lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD)
	{

	}
	else
	{
		if (rsBlessCastle.dwMasterClan != dwClanCode)
		{
			//ĳ�� ������ ������Ʈ
			UpdateCastleMasterClan(rsBlessCastle.dwMasterClan);
		}

		rsBlessCastle.dwMasterClan = dwClanCode;
	}

	return TRUE;
}

//�Ӽ����� ������ ����
int	SendResistanceToServer()
{
	Log::Debug("SendResistanceToServer : Requisitado por�m foi removido.");
	return NULL;
}
//�������� ��� ������ ������
int	SendPublicPollingToServer(int PollCode, int PollCnt, BYTE *bPollingData)
{
	Log::Debug("SendPublicPollingToServer : Requisitado por�m foi removido.");
	return NULL;
}
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock)
{
	return TRUE;
}

int	HackTrap_OpenFlagMask = 0;

int HackTrap_CheckOpenFlag()
{
	Log::Debug("HackTrap_CheckOpenFlag : Requisitado por�m foi removido.");
	return NULL;
}

int	SendHackTrapToServer(DWORD dwTrapCode, int Param)
{
	Log::Debug("SendHackTrapToServer : Requisitado por�m foi removido.");
	return NULL;
}

int SendSmeltingItemToServer(void *lpsSmeltingItem_Send)
{
	Log::Debug("SendSmeltingItemToServer : Requisitado por�m foi removido.");
	return NULL;
}

int SendManufactureItemToServer(void *lpsManufactureItem_Send)
{
	Log::Debug("SendManufactureItemToServer : Requisitado por�m foi removido.");
	return NULL;
}

int SendMixtureResetItemToServer(void *lpsMixtureResetItem_Send)
{
	Log::Debug("SendMixtureResetItemToServer : Requisitado por�m foi removido.");
	return NULL;
}