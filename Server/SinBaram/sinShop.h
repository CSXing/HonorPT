/*----------------------------------------------------------------------------*
*	���ϸ� :  sinShop.h	
*	�ϴ��� :  ������ �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#define     MAX_SHOP_CLASS_ITEM	30  //�� Ŭ������ 30��

#define     SHOP_ROW			9  //�ӽ� ���� ������ ���� 
#define     SHOP_COL			9
	
#define 	SINALLREPAIR		1 
#define 	SINREPAIR			2    
#define 	SINSELL				3      
#define 	SINBUY				4       
#define 	SINRIGHTARROW		5
#define 	SINLEFTARROW		6  
#define     SINSHOPEXIT			7

#define 	SIN_WEAPON			1 
#define 	SIN_DEFENSE			2 
#define 	SIN_ETC				3 


struct	sMYSHOP_ITEM{
	sITEMINFO	sItem;
	int			Price;
	short       Posi[2];
	DWORD		dwTemp[3];
};

struct sMYSHOP{
	DWORD			CHAR_CODE;
	DWORD			dwTemp[6];
	sMYSHOP_ITEM	sMyShopItem[30]; //������ �޴� ������
};

struct sMYSHOP_ITEM_SERVER{ //���λ������� ������ ������ 
	DWORD CODE;
	DWORD CkSum;
	DWORD Head;
	DWORD sTime;
	DWORD Price;
	DWORD SendFlag;
	DWORD Buyer;

	DWORD Temp[4];

};

class cSHOP{


public:

	int OpenFlag;	//���� �κ��丮�� �����ֳ� �ƴѰ��� ���� �÷� 
	int UseItemFlag; // ����� - �̵� ���� ������

	int PosX;
	int PosY;

	int MatShop[10];

	int SelectShop; // 1���� ,2 �� ,3 �׿� �͵� 

	int ShopIconPosi;

	
	sITEM ShowShopItem[30];	//�ѹ��� ������ ������ �ִ� 30
	sITEM ShopItem[60]; //����� ������ 
	

	int RevWeaponCnt;
	int RevDefenseCnt;
	int RevEtcCnt;
	int MatMyShop_Button;
	
	LPDIRECTDRAWSURFACE4 lpGoldEdit;

	LPDIRECTDRAWSURFACE4 lpAllRepair;
	LPDIRECTDRAWSURFACE4 lpRepair;

	LPDIRECTDRAWSURFACE4 lpSell;
	LPDIRECTDRAWSURFACE4 lpBuy;

	LPDIRECTDRAWSURFACE4 lpAllRepair_D;
	LPDIRECTDRAWSURFACE4 lpRepair_D;

	LPDIRECTDRAWSURFACE4 lpSell_D;
	LPDIRECTDRAWSURFACE4 lpBuy_D;

	LPDIRECTDRAWSURFACE4 lpRightArrow;
	LPDIRECTDRAWSURFACE4 lpLeftArrow;

	
	LPDIRECTDRAWSURFACE4 lpTabAttack;
	LPDIRECTDRAWSURFACE4 lpTabDefense;
	LPDIRECTDRAWSURFACE4 lpTabEtc;

	LPDIRECTDRAWSURFACE4 lpTabAttack_D;
	LPDIRECTDRAWSURFACE4 lpTabDefense_D;
	LPDIRECTDRAWSURFACE4 lpTabEtc_D;

	LPDIRECTDRAWSURFACE4 lpExit;
	LPDIRECTDRAWSURFACE4 lpGrid;

	LPDIRECTDRAWSURFACE4 lpRepairAllInfo;	
	LPDIRECTDRAWSURFACE4 lpRepairInfo;
	LPDIRECTDRAWSURFACE4 lpDefaultRepairAll;	
	LPDIRECTDRAWSURFACE4 lpDefaultRepair;

	LPDIRECTDRAWSURFACE4 lpSellInfo;
	LPDIRECTDRAWSURFACE4 lpBuyInfo;


	LPDIRECTDRAWSURFACE4 lpTitle_Attack;
	LPDIRECTDRAWSURFACE4 lpTitle_Etc;
	LPDIRECTDRAWSURFACE4 lpTitle_Trade;
	LPDIRECTDRAWSURFACE4 lpTitle_WareHouse;

	LPDIRECTDRAWSURFACE4 lpWeightEdit;

	LPDIRECTDRAWSURFACE4 lpNoUseBox;

	LPDIRECTDRAWSURFACE4 lpCraftItemMain;
	LPDIRECTDRAWSURFACE4 lpCraftItemButton;
	LPDIRECTDRAWSURFACE4 lpCraftItemButtonInfo;
	LPDIRECTDRAWSURFACE4 lpTitle_CraftItem;

	// pluto ����
	LPDIRECTDRAWSURFACE4 lpSmeltingItemMain;
	LPDIRECTDRAWSURFACE4 lpTitle_SmeltingItem;
	LPDIRECTDRAWSURFACE4 lpSmeltingItemButtonInfo;

	// pluto ����
	LPDIRECTDRAWSURFACE4 lpManufactureItemMain;
	LPDIRECTDRAWSURFACE4 lpTitle_ManufactureItem;
	LPDIRECTDRAWSURFACE4 lpManufactureItemButtonInfo;

	// ������ - �ͽ��� ���¿� �̹��� �ε� �߰�
	LPDIRECTDRAWSURFACE4 lpMResetTitle;
	LPDIRECTDRAWSURFACE4 lpMResetMain;
	LPDIRECTDRAWSURFACE4 lpMResetButton;
	LPDIRECTDRAWSURFACE4 lpMResetButtonInfo;
	
	LPDIRECTDRAWSURFACE4 lpTitle_Aging;
	LPDIRECTDRAWSURFACE4 lpAging_Info;
	

	LPDIRECTDRAWSURFACE4	lpShopMain;
	LPDIRECTDRAWSURFACE4	lpShopMain2;


	//���� ����
	LPDIRECTDRAWSURFACE4	lpMyShopExp;
	LPDIRECTDRAWSURFACE4	lpMyShopExp_;
	LPDIRECTDRAWSURFACE4	lpMyShopExp_T;
	LPDIRECTDRAWSURFACE4	lpMyShopNoSale;
	LPDIRECTDRAWSURFACE4	lpMyShopNoSale_;
	LPDIRECTDRAWSURFACE4	lpMyShopNoSale_T;
	LPDIRECTDRAWSURFACE4	lpMyShopSale;
	LPDIRECTDRAWSURFACE4	lpMyShopSale_;
	LPDIRECTDRAWSURFACE4	lpMyShopSale_T;
	LPDIRECTDRAWSURFACE4    lpTitle_MyShop;
	LPDIRECTDRAWSURFACE4    lpImage_MyShop;
	LPDIRECTDRAWSURFACE4    lpMyShop_T;
	

	
public:
	cSHOP();
	~cSHOP();

	void Init(); //Ŭ���� �ʱ�ȭ
	void Load();
	void Release();
	void Draw();
	void Close();//Ŭ���� ���� 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 

	void CheckShopNpcState();  //NPC�� ������� üũ�Ѵ� 

	//void CopyShopItemToShow(int Index);//�������� �޾ƿ� �������� ������ ���������� �����Ѵ� 
	void CopyShopItemToShow(int Index,int Kind=0);

	int GetShopItemXY(sITEM *pItem); //�������� ��ǥ�� ���Ѵ� 

	void DrawShopText(); //���� �ؽ�Ʈ�� ǥ���Ѵ� 

	////////////////////�ƾ��� ��� �ȱ� 
	int RecvBuyItemToServer(sITEM *pItem ,int ItemCount=1); //�������� ��� 
	int SellItemToShop(sITEM *pItem,int ItemCount=1); //�ƾ����� �Ǵ� 

	int SendBuyItemToServer(sITEM *pItem , int ItemCount=1 ); //���������������� ��´� 

	void DeleteShopItem(); //������ ������ ������������ �ʱ�ȭ�Ѵ�

	int CheckHighRankItem(sITEM *pItem); //���������� üũ

	//���� �������� ����ִ� �� üũ�Ѵ�.
	int haBuyMoneyCheck(int BuyMoney);
	//���� ������ ��������� ������ ǥ�����ش�.
	int haShopItemPrice(int Money);
};

/*----------------------------------------------------------------------------*
*								���λ��� ��� Ŭ���� 
*-----------------------------------------------------------------------------*/
class cMYSHOP{
public:
	int OpenFlag;	
	sITEM MyShopItem[30];

	char szDoc[128];     //ȫ������
	char szSendDoc[128]; //������ ���� ȫ������

public:
	
	//�������� ���õɰ����� ã�´�
	int SetMyShopItem(sITEM *pItem);

	//�����Ѵ�
	int LastSetMyShopItem(sITEM *pItem);

	//�κ��������� ����Ѵ�
	int SetShopItemToInven(sITEM *pItem);

	//������ ����� �������� ã�´�
	int SearchShopItemToInven(sITEM *pItem);

	//�������� ������ ������ 
	int SendMyShopItem(); 

	//�������� ���ſ�û�� ���� �������� �޴´�
	int RecvMyShopItem( DWORD dwCharCode , sMYSHOP_ITEM_SERVER *lpShopItem );

	//�ڽ��� ������ �ִ� �ݾ��� �����Ѵ�
	int GetLimitMoney(int Money);

	//������ ���ȸ��� ������ �ݴ´�
	int AutoCloseShop();

	//���λ����� ���� ���ؿ´�
	int GetTotalMyShopItemMoney();

};
/*----------------------------------------------------------------------------*
*								���λ��� Ŭ���� 
*-----------------------------------------------------------------------------*/
class cCHARSHOP{
public:
	int OpenFlag;	

public:
	sITEM CharShopItem[30];
	
	//�������� �޴´�
	int RecvShopItem(sMYSHOP *sMyShop);

	//���� �������� ã�´�
	int SearchMyShopItem(DWORD CODE ,DWORD Head , DWORD CheckSum);

	//���λ��������� �������� �κ��丮�� �����Ѵ�
	int SetCharShopItemToInven(sITEMINFO *pItem_Info);

	//���λ������� �������� ���
	int BuyItem(int Index);

};

extern cSHOP cShop;

extern int sinShopKind;  //���� ������ ����  1  ���� 2 ��� 3 ���� (defalt �� ����)

extern int TalkNpcState; //���� ���� NPC�� �Բ� ������� 

extern int ShopGoldEdit[2][4];

extern int ShopArrowPosi; //���� ȭ��ǥ ���� 

extern int BuyItemServerFlag; //�������� ������ �춧 �޼����� ���������� ������� 

extern cMYSHOP cMyShop;
extern cCHARSHOP cCharShop;

extern sMYSHOP     sMyShop;       //������ MyShop ����ü
extern sMYSHOP     sRecvMyShop;   //�޴� MyShop ����ü
extern int MyShopSendButton;

extern int MyShopExpBox;
extern int MyShopItemSellMoney2;
extern sMYSHOP_ITEM_SERVER sMyShop_Server;
extern sITEMINFO MyShopPotion;
extern int MyShopItemCancelIndex[2];
