/*----------------------------------------------------------------------------*
*	���ϸ� :  sinInterFace.h	
*	�ϴ��� :  �������̽��� ���� ���� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

//////////����� ��ȯ 
#define		RESTART_FEILD		1
#define		RESTART_TOWN		2
#define		RESTART_EXIT		3

/////////��ų ���� �������̽� ǥ��
#define     HA_DEFANCE			1
#define     HA_BLOCK			2
#define     HA_EVASION			3

/*----------------------------------------------------------------------------*
*						(�������̽�  )class
*-----------------------------------------------------------------------------*/	
struct sRUNCAMERAMAP{ //��ư ��� �÷� 

	int RunFlag;			 //�ȱ� �޸��� �÷� 
	int CameraAutoFlag;		 //ī�޶� �ڵ� �÷� 
	int MapOnFlag;			 //�ʺ��� �÷� 

	//�̰��� �������̽����� �Ͼ�� ��� �̺�Ʈ ��ư�� �߰���ų ���� 
	
};

////// ������ ���� ���׹̳� �ܻ�ȿ�� 
struct sSHADOWSTATE{
	int Flag;
	int Time;
	POINT Posi;
	POINT Size;
	DWORD Color;

};


class cINTERFACE{

public:

	sRUNCAMERAMAP	sInterFlags;


	int MatMain;					    //��� �������̽� 
	int MatMenu[2];						//�޴�  
	int MatChatDefault[2];				//ä�ñ⺻ 
	int MatChatSmall[2];				//���� ä��â
	int MatChatDoc[2];					//���� â 
	int MatExitMenu;					//������ �޴� 
	int MatComPass;						//��ħ�� ���� 	

	int MatLife;						//������   �ܻ�ȿ��
	int MatMana;						//����     �ܻ�ȿ��
	int MatStamina;						//���׹̳� �ܻ�ȿ�� 

	LPDIRECTDRAWSURFACE4    lpEXP;      //����ġ  
	LPDIRECTDRAWSURFACE4    lpMP;		//����  	
 	LPDIRECTDRAWSURFACE4    lpLIFE;     //������ 
 	LPDIRECTDRAWSURFACE4    lpSTM;      //���׹̳� 
	LPDIRECTDRAWSURFACE4    lpANGER;    //�г������ 

 	LPDIRECTDRAWSURFACE4    lpLIFESpot;     //������ 
 	LPDIRECTDRAWSURFACE4    lpSTMSpot;      //���׹̳� 
	LPDIRECTDRAWSURFACE4    lpMANASpot;     //����

	LPDIRECTDRAWSURFACE4    lpButton[6];//��ư ������ 
	LPDIRECTDRAWSURFACE4    lpButton2[6];//��ư ������2 

	LPDIRECTDRAWSURFACE4    lpMenuButton[3];//�޴� ��ư 

	LPDIRECTDRAWSURFACE4    lpInfoBox[6]; //ǳ�� ���� 
	LPDIRECTDRAWSURFACE4    lpWalk;		  //�ȱ� 

	LPDIRECTDRAWSURFACE4    lpRunInfo;    //�ٱ�ǳ�� ���� 
	LPDIRECTDRAWSURFACE4    lpWalkInfo;   //�ȱ�ǳ�� ���� 
	
	LPDIRECTDRAWSURFACE4    lpCameraAutoInfo;   //�ڵ� ī�޶� ǳ�� ���� 
	LPDIRECTDRAWSURFACE4    lpCameraHandInfo;   //���� ī�޶� ǳ�� ���� 
	LPDIRECTDRAWSURFACE4    lpCameraFixInfo;   //���� ī�޶� ǳ�� ���� 

	LPDIRECTDRAWSURFACE4    lpMapOnInfo;   //�ڵ� ī�޶� ǳ�� ���� 
	LPDIRECTDRAWSURFACE4    lpMapOffInfo;   //���� ī�޶� ǳ�� ���� 

	LPDIRECTDRAWSURFACE4    lpChatShowButton;	//ūä��â

	LPDIRECTDRAWSURFACE4    lpSun;	//��
	LPDIRECTDRAWSURFACE4    lpMoon;	//��

	LPDIRECTDRAWSURFACE4    lpGage[2]; //0��ü  1������ 

	LPDIRECTDRAWSURFACE4	lpBar_Anger;
	LPDIRECTDRAWSURFACE4	lpBar_Time;
	LPDIRECTDRAWSURFACE4	lpBar_Exp;
	LPDIRECTDRAWSURFACE4	lpBar_ExpWon;

	LPDIRECTDRAWSURFACE4	lpButton_Anger;

	LPDIRECTDRAWSURFACE4	lpMapOnImage;
	LPDIRECTDRAWSURFACE4	lpCameraImage[2]; //0���� 1 ���� 

	LPDIRECTDRAWSURFACE4	lpReStartMain; //�ٽý��� 
	LPDIRECTDRAWSURFACE4	lpReStartButton[3]; //�ٽý��� 

	LPDIRECTDRAWSURFACE4	lpSelectSkill_Main[2];		//��ų ���� 0 �׷��� 1 Ȱ�� 
	LPDIRECTDRAWSURFACE4	lpSelectChangeJob[2];   //����        0 �׷��� 2 Ȱ�� 

	LPDIRECTDRAWSURFACE4	lpTime7;


	int MatClock; //�ð� 
	int ChatFlag; //ä�� �÷� 

public:
	cINTERFACE();
	~cINTERFACE();

	void Init();
	void Load();
	void Release();
	void Close();
	void Draw();
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 


	void InitState();		//����� , ��� , �ٷ� �� �ʱ�ȭ�Ѵ� 
	void CheckingState();	//����� , ��� , �ٷ� �� ��ȭ�� ���� Bmp�� ũ�⸦ ��ȭ ��Ų��
	void ShowParaState();   //����� ��� �ٷ� ����ġ ��ġ�� ǥ���Ѵ� 

	void DrawInterText();   //�������̽��� �� �ؽ�Ʈ�� ǥ���Ѵ� 

	void CheckAllBox(int ButtonFlag); //�ڽ��� �ϳ��� �����ش� 

	int DrawClockArrow( int x, int y, int Angle ); //�ð踦 �׷��ش� 
	int DrawCompass(int x , int y ,int Angle);     //��ħ���� �׸��� 


	int CheckExpPercentControl(); //��Ʈ���� ������ ����ġ�� 4%�̸��̸� ����ġ �ٰ� ���ڰŸ��� 

	//////////////////����� �̹��� �ε� & ������ 
//	void LoadReStartImage();
//	void ReleaseReStartImage();
	int GetWordTimeDisplay();

	void ShowExpPercent(); //����ġ�� �����ش�

	int  SetStringEffect(int); //����Ʈ���ڸ� �����ش� 

};

extern cINTERFACE	cInterFace;

extern POINT InfoMessageBoxPos;  //�ؽ��� ǥ���� ��ǥ 

/*ä�ð���*/
extern int sinChatEnter;          //ä�� �÷� (����Ű�� �Է��ϸ� ä�� â�� ���´� )
extern int SubChatHeight;	      //ä��â ���� ��ġ ��ǥ �� 	
extern int sinChatDisplayMode ;   //���� ä���� ��� 
extern int sinChatInputMode;      //ä�� �Է� ��� 
////////////

extern int sinFireShow;           //���� �����ش� 
extern int ExitButtonClick;

extern int ReStartFlag;			  //���� ������ �ٽý��� �������̽��� �����ش� 
extern int ReStartOptionIndex;	  //�ٽý��� ���� �ε��� 	

extern int SkillNpcFlag;			  //���� ��ų ���� �������̽��� �����ش� 
extern int SkillMasterOptionIndex;

extern int RestartCheckExp;		  //�ٽý����Ҽ��ִ� ����ġ�� �ֳ� ���� 


extern int psDrawTexImage2( smTEXTUREHANDLE *hTexHandle , float *x, float *y, float fx, float fy, float fw, float fh, int Transp , int specular);

extern int NotUseSkillEffect[3];

extern int ChangeJobButtonclick;

extern int  ResetQuset3Flag;

extern int ReStartFlag;
extern POINT ReStartMainXY;
extern int ReStartIndex;
extern POINT ReStartTextXY;
extern char szReStartMsg[128];
extern char szReStartMsg2[128];


