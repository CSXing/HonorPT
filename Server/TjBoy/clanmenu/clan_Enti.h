#pragma once
#ifndef __CLAN_ENTI_HPP__
#define __CLAN_ENTI_HPP__

//#define PARKMENU
//#define CLANHELPICON_NUM_ENIT		100

class CLANEntity
{
public:
	int				life;	
	CLANEntity      *parent;


public:
	CLANEntity();
	~CLANEntity();

	virtual int Init() ;
	virtual int Clear();

	//***************************************************************************************
	virtual void Main();
	virtual void RenderMain();

	//***************************************************************************************
	void	setParent(CLANEntity      *p){parent = p;}
	void			Kill()			{ life = 0; };
	int				IsDead()		{ return(life); };

	//virtual void main();
	virtual void Draw();
	//***************************************************************************************
	virtual void menuInit(int num);

	//***************************************************************************************
	void Draw_C_Text(char *str, int x, int y, int sw=0);
	void Draw_C_Text2(char *str, int x, int y, int sw=0); //Ŭ���� �������� ������(red)����
	void Draw_C_Text3(char *str, int x, int y, int sw=0);  //ä�ø���Ʈ���� ����Ʈ ���(green)����

//��öȣ
//#ifdef PARKMENU
	void Draw_C_TextPark(char *str, int x, int y, int RedColor, int GreenColor, int BlueColor, int sw);

	


	//***************************************************************************************
	//int  menuSu;
	//RECT menuPos[30];		//�޴�����.
	int  chkMenu(int x, int y, RECT *r);
	int  chkeckMenuN();



	//data �Ľ� ========================================
	void ParsingIsClanMember(char* data);
	void ParsingIsClanMember2(char *data);
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);
	void ParsingData(char* cmd, char* data, char* save);
	void ParsingMakeClanMoney(char* data,int* money);


	void ParsigClanWon(char* data);
	
	void ParsingClanWonStrLen();

	void LoadToFile();
	

	//���� ���� �Լ���================
	void LoadHelp(char *help);
	void DestroyClanHelpIcon();
	void LoadHelpImage();
	BOOL ReadClanHelpScr(char *filename);
	void ReMoveClanWon(char *src);
	//void Updatecldata();
	void WriteMyClanInfo();
	int CheckMark(BOOL bFlag);

	void CheckClanExp(char* byte); //Ŭ�� ���� ���� ����



	

	//�Ʒ� ���ҽ� �ε� Ŭ���� �Լ���
	void Load();
	void Load2();

	virtual void Close();
	void copy_LoadResource(CLANEntity *copyDest);
	void copy_LoadResource2(CLANEntity *copyDest);
	//��ũ �Ѱ��ֱ�
	inline void SetBase(CLANEntity *copyDest){parent = copyDest;}

	#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	#else
	inline HBITMAP GetClanMark(){return parent->hMark;}
	#endif

	//PartyInterface ���ҽ�..
	#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	
	int	MatPartyBackGround;		//���
	int	hBoxCorner,hBoxLine;	//�ڽ��׸���
	int	hBox1,hBox2,hBox3,hBox4,hBox6,hBox7,hBox8,hBox9; //�ڽ��׸���
	int	hScl_Bar_tj;
	int	hScl_icon_tj;
	int   hLine;
	

	inline LPDIRECTDRAWSURFACE4 GetClanMark(){return parent->hMark;}


	LPDIRECTDRAWSURFACE4	hBt_Help;
	LPDIRECTDRAWSURFACE4	hMark_box;
	LPDIRECTDRAWSURFACE4	hOk_Gray,hOk_Yellow; //Ȯ��
	LPDIRECTDRAWSURFACE4	hCancel_Gray,hCancel_Yellow; //���
	LPDIRECTDRAWSURFACE4	hYes_Gray,hYes_Yellow; // ��
	LPDIRECTDRAWSURFACE4	hNo_Gray,hNo_Yellow; //�ƴϿ�

	LPDIRECTDRAWSURFACE4	hMark,hClanName;
	LPDIRECTDRAWSURFACE4	hSecession[2];
	LPDIRECTDRAWSURFACE4	hClose[2];
	LPDIRECTDRAWSURFACE4	hIcon;
	LPDIRECTDRAWSURFACE4	hIcon_Click;
	LPDIRECTDRAWSURFACE4	hHelp1;
	LPDIRECTDRAWSURFACE4	hClinetMark;
	LPDIRECTDRAWSURFACE4	hServerMark;
	LPDIRECTDRAWSURFACE4	hWareHouse[2]; //���콺�� ���� �ö������
	LPDIRECTDRAWSURFACE4	hWHRight[2];
	LPDIRECTDRAWSURFACE4	hClanMark[2];
	LPDIRECTDRAWSURFACE4	hRemove[2];
	LPDIRECTDRAWSURFACE4	hClanRelease[2];
	LPDIRECTDRAWSURFACE4	hHelp[2];
	LPDIRECTDRAWSURFACE4	hOut[2];

	LPDIRECTDRAWSURFACE4	hButton_Box;
	LPDIRECTDRAWSURFACE4	hClanTitle;
	LPDIRECTDRAWSURFACE4	hRemoveToMenu[2];
	LPDIRECTDRAWSURFACE4	hScl_Bar;
	LPDIRECTDRAWSURFACE4	hScl_icon;
	LPDIRECTDRAWSURFACE4	hClanHelpIcon[CLANHELPICON_NUM][4];
	LPDIRECTDRAWSURFACE4    hClanNameInput;

	LPDIRECTDRAWSURFACE4   hTxt_Remove[2]; //�߹�
	LPDIRECTDRAWSURFACE4   hTxt_Release[2]; //��ü
	LPDIRECTDRAWSURFACE4   hTxt_Mark[2]; //��ũ
	LPDIRECTDRAWSURFACE4   hTxt_Help[2]; //����
	LPDIRECTDRAWSURFACE4   hTxt_Secession[2];//Ż��
	LPDIRECTDRAWSURFACE4   hTxt_LeaveClan[2];//����
	LPDIRECTDRAWSURFACE4   hTxt_SubChip[2];//��Ŭ��Ĩ
	LPDIRECTDRAWSURFACE4   hSubChip;

	

	
	char		szHelpMsg1[HELPMSG_MAX];
	char		szErrorMsg[256];

	#else

	HBITMAP	hLine;

	HBITMAP	hBox1,hBox2,hBox3,hBox4,hBox6,hBox7,hBox8,hBox9;
	HBITMAP	MatPartyBackGround;
	HBITMAP	hMark_box;
	HBITMAP	hBt_Help;
	
	HBITMAP	hOk_Gray,hOk_Yellow; //Ȯ��
	HBITMAP	hCancel_Gray,hCancel_Yellow; //���
	HBITMAP	hYes_Gray,hYes_Yellow; // ��
	HBITMAP	hNo_Gray,hNo_Yellow; //�ƴϿ�

	HBITMAP	hMark,hClanName;
	HBITMAP	hSecession[2];
	HBITMAP	hClose[2];
	HBITMAP	hIcon;
	HBITMAP   hIcon_Click;
	HBITMAP	hHelp1;
	HBITMAP	hClinetMark;
	HBITMAP	hServerMark;
	HBITMAP	hWareHouse[2]; //���콺�� ���� �ö������
	HBITMAP	hWHRight[2];
	HBITMAP	hClanMark[2];
	HBITMAP	hRemove[2];
	HBITMAP	hClanRelease[2];
	HBITMAP	hHelp[2];
	HBITMAP	hOut[2];
	HBITMAP	hButton_Box;
	HBITMAP	hClanTitle;
	HBITMAP	hRemoveToMenu[2];
	HBITMAP	hScl_Bar;
	HBITMAP	hScl_icon;
	HBITMAP   hClanHelpIcon[CLANHELPICON_NUM][4]; //������ �迭�� ó��

	HBITMAP   hTxt_Remove[2]; //�߹�
	HBITMAP   hTxt_Release[2]; //��ü
	HBITMAP   hTxt_Mark[2]; //��ũ
	HBITMAP   hTxt_Help[2]; //����
	HBITMAP   hTxt_Secession[2]; //Ż��
	HBITMAP   hTxt_LeaveClan[2];//����
	HBITMAP   hTxt_SubChip[2];//��Ŭ��Ĩ
	HBITMAP   hSubChip;

	HBITMAP    hClanNameInput;

	HWND		m_hClanName;	
	char		szErrorMsg[256];

	#endif
	BOOL bImageDown;
};

#endif //__CLAN_ENTI_HPP__





#if defined (CLAN_EITI_CPP)


	class CLANEntity  cBaseMenu;

#else

	extern class CLANEntity  cBaseMenu;


#endif




