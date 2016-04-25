/*----------------------------------------------------------------------------*
*	���ϸ� :  sinInterFace.cpp
*	�ϴ��� :  �������̽��� ���� ���� �����Ѵ�
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ�
*-----------------------------------------------------------------------------*/

#include "sinLinkHeader.h"
#include "..\\tjboy\\clanmenu\\tjclan.h"
#include "..\\field.h"
/*----------------------------------------------------------------------------*
*								��������
*-----------------------------------------------------------------------------*/
cINTERFACE	cInterFace;
sSHADOWSTATE sShadowState[3];

int sinBmpLength[5] = { 0,0,0,0,0 }; //������ ���� ���׹̳� ����ġ 

////////////////ä�ð��� Flag
int sinChatEnter = 0;         //ä�� �÷� (����Ű�� �Է��ϸ� ä�� â�� ���´� )
int SubChatHeight = 0;	      //ä��â ���� ��ġ ��ǥ �� 	
int sinChatDisplayMode = 0;   //���� ä���� ��� 
int sinChatInputMode = 0;     //ä�� �Է� ��� 
////////////////

/////////////Test Key Msg
int TestKeyAniFlag = 0;

////////////////��ư���� Flag
int ExitButtonClick = 0;
int HelpButtonClick = 0;
int MouseButtonPosi = 0;
int sinMenuBottonPosi = 0;
int sinRunCameraMapPosi = 0; //�ȱ� ī�޶� �� 
int SunandMoonFlag[2] = { 0,0 }; //0 �� 1 �� 
int InterButtonDown = 0;  //�������̽� ��ư �ٿ� �÷� 
int sinFireShow = 1;      //���� �����ش� 

////////////////��ų ������ (����� ���ǰ� ���� �ʴ� )
int ReSizelpGage = 0;     //������ �׸��� �����Ѵ� 


////////////////
int InitStateFlag = 0;    //�ʱ�ȭ �÷� 


//////////////// ��� ������ 
char szInfoMsgBuff5[512];  //����� ��� �ٷ� ����ġ �� ��ġ ��� ���� 
char szInfoMsgBuff6[128];  //����� ��� �ٷ� ����ġ �� ��ġ ��� ���� 
POINT InfoMessageBoxPos;  //�ؽ��� ǥ���� ��ǥ 


//////////////// ���� (�ٽý��� )
int ReStartFlag = 0;  //���� ������ �ٽý��� �������̽��� �����ش� 
POINT ReStartMainXY = { 0,0 }; //�ٽý��� ��ǥ 
int ReStartIndex = 0;
POINT ReStartTextXY = { 0,0 };
char szReStartMsg[128];
char szReStartMsg2[128];

int RestartCheckExp = 0;
int ReStartOptionIndex = 0;
int ShowExpPercentFlag = 0;

//////////////// ��ų ���� ����  (�ٽý��� )

int SkillNpcFlag = 0;  //���� ������ �ٽý��� �������̽��� �����ش� 
POINT SkillMasterMainXY = { 0,0 }; //�ٽý��� ��ǥ 
int SkillMasterIndex = 0;
POINT SkillMasterTextXY = { 0,0 };
char szSkillMasterMsg[128];
char szSkillMasterMsg2[128];
int SkillMasterOptionIndex = 0;

////////��ų ��� ������ ����Ʈ 
int NotUseSkillEffect[3] = { 0,0,0 };

int SkillMouseButtonPosi[2] = { 0,0 };

/////////// �� ��
int DayOrNightFlag = 0;
int sinGageTime2 = 0;


int ReStartButtonRect[3][4] = {
	{21,22,21 + 44,22 + 44},			//�ʵ忡�� ���� 
	{72,22,72 + 44,22 + 44},			//�������� ���� 
	{123,22,123 + 44,22 + 44},			//���� ���� 

};

int ButtonRect[6][5] = {   //�ڽ� ���� ��ư 
	{648,560,648 + 25,560 + 27},
	{673,560,673 + 25,560 + 27},
	{698,560,698 + 25,560 + 27},
	{723,560,723 + 25,560 + 27},
	{748,560,748 + 25,560 + 27},
	{772,560,772 + 25,560 + 27},

};
/*
int ButtonRect[6][5]= {   //�ڽ� ���� ��ư
	{655,566,678,598},
	{678,566,701,598},
	{701,566,724,598},
	{724,566,747,598},
	{747,566,770,598},
	{770,566,792,598},

};
*/
int sinMenuRect[3][4] = {  //���� ���� ....
	{692,492,784,511},
	{692,517,784,536},
	{692,542,784,561}, //���� 


};

int sinRunCameraMap[3][4] = { //�ȱ� , ī�޶� , �� 
	{569,555,569 + 26,555 + 26},
	{569 + 26,555,569 + 26 + 26,555 + 26},
	{569 + 26 + 26,555,569 + 26 + 26 + 26,555 + 26},

};


// ���� ���� 
int ChangeJobButtonclick = 0;


//PK����
int sinPKWarningExpFlag = 0;
int sinPKWarnigExpTime = 0;
LPDIRECTDRAWSURFACE4 lpGrowInterLeft;
LPDIRECTDRAWSURFACE4 lpGrowInterMiddle;
LPDIRECTDRAWSURFACE4 lpGrowInterRight;


//���� ����Ʈ���� ===================================================================
struct HAEFFECT
{
	int   Flag;
	int   Time;
	POINT Posi;
	POINT Size;
	int   Mat;
	int   Alpha;
};
char *HaEffectFilePath[] = {
	0,
	"Image\\SinImage\\Inter\\defense.tga", //1
	"Image\\SinImage\\Inter\\block.tga",   //2
	"Image\\SinImage\\Inter\\Evade.tga",   //3
	0,
};
#define  MAX_HAEFFECTNUM 10 //�Ѱ���

int HA_EffectIndex[4] = { 0, }; //

HAEFFECT HaEffect[MAX_HAEFFECTNUM];

int DefaultLength[4] = { 94,94,76,86 };
float sinOldNowState[4] = { 0,0,0,0 };
float sinOldMaxState[4] = { 0,0,0,0 };
char InputSecretCode[256];
char sinBuff[256];
int  ResetQuset3Flag = 0;
DWORD dwCheckTime; //�ð��� üũ�Ѵ� 
int   CheckNotUseSkillTime[3] = { 0,0,0 };
int   CheckNotUseSkillCount[3] = { 0,0,0 };
int   CheckTime2[3] = { 0,0,0 };
int   CheckExpFlag = 0;
int   CheckExpFlag2 = 0;
int   CheckExpTime = 0;
DWORD dwTestKeyTime = 0;
int   TeskKeyMessageCnt = 0;
int sinWinSizeX[] = { 1024,1280,1600 };
int sinWinSizeXindex[] = { 2,6,11 };
int nCheckSkillDraw = 0;		//�ؿ�
char *szTestKeyMsg[6] = { "��!  &(^_^)&    ",
"��!  /(^_^)/   ",
"��!  *(^_^)*  ",
"",
"",
"", };

cINTERFACE::cINTERFACE()
{

}
cINTERFACE::~cINTERFACE()
{

}
void cINTERFACE::Init()
{
	Log::Debug("Init : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::Load()
{
	Log::Debug("Load : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::Release()
{
	Log::Debug("Release : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::Draw()
{
	Log::Debug("Draw : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::Main()
{
	Log::Debug("Main : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::Close()
{
	Log::Debug("Close : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::LButtonDown(int x, int y)
{
	Log::Debug("LButtonDown : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::LButtonUp(int x, int y)
{
	Log::Debug("LButtonUp : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::RButtonDown(int x, int y)
{
	Log::Debug("RButtonDown : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::RButtonUp(int x, int y)
{

}
void cINTERFACE::KeyDown()
{
	Log::Debug("KeyDown : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::InitState()
{
	Log::Debug("InitState : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::CheckingState()
{
	Log::Debug("CheckingState : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::ShowParaState()
{
	Log::Debug("ShowParaState : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::DrawInterText()
{
	Log::Debug("DrawInterText : Requisitado por�m foi removido.");
	return;
}
void cINTERFACE::CheckAllBox(int ButtonFlag)
{
	Log::Debug("CheckAllBox : Requisitado por�m foi removido.");
	return;
}
int cINTERFACE::DrawClockArrow(int x, int y, int Angle)
{
	Log::Debug("DrawClockArrow : Requisitado por�m foi removido.");
	return NULL;
}
int cINTERFACE::DrawCompass(int x, int y, int Angle)
{
	Log::Debug("DrawCompass : Requisitado por�m foi removido.");
	return NULL;
}
int cINTERFACE::CheckExpPercentControl()
{
	Log::Debug("CheckExpPercentControl : Requisitado por�m foi removido.");
	return NULL;
}
int cINTERFACE::GetWordTimeDisplay()
{
	Log::Debug("GetWordTimeDisplay : Requisitado por�m foi removido.");
	return NULL;
}
void cINTERFACE::ShowExpPercent()
{
	Log::Debug("ShowExpPercent : Requisitado por�m foi removido.");
	return;
}
int cINTERFACE::SetStringEffect(int Index)
{
	Log::Debug("SetStringEffect : Requisitado por�m foi removido.");
	return NULL;
}