#include "jts.h"

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
 �Լ��� : GetTXT
 ����   : txt���Ͽ��� ���ڿ��� �о��
�Ķ���� : SaveFile�� �ؽ�Ʈ ������ �о� �鿩�� ������ ����
           FileName�� �о�� �����̸�
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int GetTXT(char* FileName,char* SaveFile)
{
	FILE* rFile = NULL;	
	int nStrlen = 0;

	rFile = fopen(FileName,"r");

	if(!rFile)
	{
		return -1;
	}
	if(SaveFile[0] !=0)
	{
		memset(SaveFile,0,sizeof(SaveFile));
	}	
	fread(SaveFile,MAXTXTFILE,1,rFile);
	fclose(rFile);
	nStrlen = strlen(SaveFile);
	if(nStrlen > MAXTXTFILE)
	{
		return 0;
	}

	return 1;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
 �Լ��� : CheckJTS
 ����   : �Ϻ���� 2byte�Ǵ� 1byte�� ó���ϴ� 2���� ��찡 �ִ�.
          TXT���Ͽ��� ���Ͽ��� �о� ���ڿ����� �� ������ ���� ���Ͽ��� 2byte,1byte�� ó����ġ �˻��Ѵ�.
		  10���� 129~159,224~239 16����0x81~0x9f,0xe0~0xef������ ���� ��� 2byte������ 1byt�� ó���ϰ�
		  �̾ 10���� 64~126, 128~252 16����0x40~0x7e,0x80~0xfc�� ��� ������ 1byte�� ó���鼭 ���ϰ��� 2��ó���Ѵ�.
		  ����,����(ASCII, 0x21-0x7E)�� �ݰ�,������ 0xA1-0xDF�� ��� 1byte�� ó���ϸ鼭 ���ϰ� 1��ó���Ѵ�.
		  ������ �ϰ�� ���ϰ� -1�� ó���Ѵ�.
�Ķ���� : SaveFile�� �ؽ�Ʈ ������ �о� �鿩�� ������ ������ ��ġ
		   num ���ϴ� ��ġ
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int CheckJTS(char* SaveFile,int num)
{	
	if(((*(SaveFile + num) >= 129) && (*(SaveFile + num) <= 159)) || ((*(SaveFile + num) >= 224) && (*(SaveFile + num) <= 239)) || ((*(SaveFile + num) >= 0xFFFFFF81) && (*(SaveFile + num) <= 0xFFFFFF9F)) || ((*(SaveFile + num) >= 0XFFFFFFE0) && (*(SaveFile + num) <= 0XFFFFFFEF)))
	{	
		if(((*(SaveFile + num + 1) >= 0xFFFFFF40) && (*(SaveFile + num + 1) <= 0xFFFFFF7E)) || ((*(SaveFile + num + 1) >= 0xFFFFFF80)&& (*(SaveFile + num + 1) <= 0xFFFFFFFC)) || ((*(SaveFile + num + 1) >= 64)&& (*(SaveFile + num + 1) <= 126)) || ((*(SaveFile + num + 1) >= 128)&& (*(SaveFile + num + 1) <= 252)))
		{				
			return 2;
		}		
	}
	if(((*(SaveFile + num) >= 0xFFFFFF21) && (*(SaveFile + num) <= 0xFFFFFF7E)) || ((*(SaveFile + num) >= 0xFFFFFFA1) && (*(SaveFile + num) <= 0xFFFFFFDF)))
	{		
		return 1;
	}

	return -1;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
  �Լ��� : GetCharacterJTS
  ����   : txt���Ͽ��� �о�� ���ڿ��� ���ڷ� ����ϴ� �Լ�.
           CheckJTS�Լ��� ����Ͽ� �Ϻ����� ����ó���� �Ľ��ߴ�.
 �Ķ���� : SaveFile�� �ؽ�Ʈ ������ �о� �鿩�� ������ ������ ��ġ
            SaveCh �о�� ���ڸ� ������ ������ ��ġ
			 num ���ϴ� ��ġ
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

char* GetCharacterJTS(char* SaveFile, char* SaveCh, int num)
{
	int nCheck = 0;
	int nNum = 0;
	int nCount = 0;

	while(1)
	{	
		if(nCount+1 == num)
		{
			break;
		}		
		nCheck = CheckJTS(SaveFile,nNum);
		if(nCheck == 2)
		{
			nNum+=2;
		}
		else
		{
			nNum+=1;
		}		
		nCount++;
	}
    
	nCheck = CheckJTS(SaveFile,nNum);
	if(SaveCh[0] !=0)
	{
		memset(SaveCh,0,sizeof(SaveCh));
	}	

	if(nCheck == 2)
	{
		SaveCh[0] = SaveFile[nNum];
		SaveCh[1] = SaveFile[nNum+1];		
	}
	else 
	{
		SaveCh[0] = SaveFile[nNum];
	}	
	
	return SaveCh;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
  �Լ��� : CheckJTS_ptr
  ����   : num�� ��ġ�� ���� �޸� �ּҸ� ó���ϱ� ���� �Լ�
           ���ϰ� 1 : 1byte�� ó���� ���
		   ���ϰ� 2 : 2byte�� ó���ǹǷ� �� �� �޸𸮸� ��ġ��Ŵ
		   ���ϰ� 3 : 2byte�� ó���ǳ� ������ 1byte�� ó���Ǳ� ������ �� �޸� �ּҸ� ����
		   ���ϰ� 4 : 0x0D �ϰ��
		   ���ϰ� 5 : 0x0A �ϰ��
�Ķ���� : pos ���ϴ� ��ġ
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int CheckJTS_ptr(char* SaveFile,int pos)
{	
	if(((*(SaveFile + pos) >= 0xFFFFFF21) && (*(SaveFile + pos) <= 0xFFFFFF7E)) || ((*(SaveFile + pos) >= 0xFFFFFFA1) && (*(SaveFile + pos) <= 0xFFFFFFDF)))
	{		
		return 1;
	}	

	if(((*(SaveFile + pos) >= 129) && (*(SaveFile + pos) <= 159)) || ((*(SaveFile + pos) >= 224) && (*(SaveFile + pos) <= 239)) || ((*(SaveFile + pos) >= 0xFFFFFF81) && (*(SaveFile + pos) <= 0xFFFFFF9F)) || ((*(SaveFile + pos) >= 0XFFFFFFE0) && (*(SaveFile + pos) <= 0XFFFFFFEF)))
	{
		return 2;
	}

	if(((*(SaveFile + pos -1) >= 129) && (*(SaveFile + pos -1) <= 159)) || ((*(SaveFile + pos -1) >= 224) && (*(SaveFile + pos -1) <= 239)) || ((*(SaveFile + pos -1) >= 0xFFFFFF81) && (*(SaveFile + pos -1) <= 0xFFFFFF9F)) || ((*(SaveFile + pos -1) >= 0XFFFFFFE0) && (*(SaveFile + pos -1) <= 0XFFFFFFEF)))
	{		
		return 3;		
	}

	if(*(SaveFile + pos) == 0x0D)
	{
		return 4;
	}

	if(*(SaveFile + pos) == 0x0A)
	{
		return 5;
	}

	return -1;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
  �Լ��� : GetCharacter_ptr
  ����   : pos�� ��ġ�� ���� �޸� �ּҸ� ó���ϱ� ���� �Լ�
			case 1 : 1byte�� ó���� ���
		    case 2 : 2byte�� ó���ǹǷ� �� �� �޸𸮸� ��ġ��Ŵ
		    case 3 : 2byte�� ó���ǳ� ������ 1byte�� ó���Ǳ� ������ �� �޸� �ּҸ� ����
		    case 4 : 0x0D�� ��� ���� �޸� �ּҸ� ����
			csse 5 : 0x0A�� ��� �� ���� �޸� �ؼҸ� ����
 �Ķ���� : pos ���ϴ� ��ġ
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

char* GetCharacter_ptr(char* SaveFile,int pos)
{
	int nReturnValue = 0;
	nReturnValue = CheckJTS_ptr(SaveFile,pos);
	switch(nReturnValue)
	{
	case 1:
		return (SaveFile + pos);
	case 2:
		return (SaveFile + pos -1);
	case 3:
		return (SaveFile + pos);
	case 4:
		return (SaveFile + pos - 1);
	case 5:
		return (SaveFile + pos - 2);
	case -1:
		return 0;
	}
	return 0;
}
/*////////////////////////////////////////////////////////////////////////////////////////////////////////
  �Լ��� : GetPointerPos
  ����   : pos�� ��ġ�� ��ġ ó�� �Լ�
			case 1 : 1byte�� ó���� ���
		    case 2 : 2byte�� ó���ǹǷ� �� �� ��ġ�� üũ
		    case 3 : 2byte�� ó���ǳ� ������ 1byte�� ó���Ǳ� ������ �� �� ��ġ�� ����
		    case 4 : 0x0D�� ��� �� �� ��ġ�� ����
			csse 5 : 0x0A�� ��� �� ���� ��ġ�� ����
�Ķ���� : pos ���ϴ� ��ġ
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int GetPointerPos(char* SaveFile,int pos)
{
	int nPos;
	int nReturnValue = 0;

	nReturnValue = CheckJTS_ptr(SaveFile,pos);

	switch(nReturnValue)
	{
	case 1:
		nPos = pos;
		break;
	case 2:
		nPos = pos -1;
		break;
	case 3:
		nPos = pos;
		break;
	case 4:
		nPos = pos -1;
		break;
	case 5:
		nPos = pos -2;
	case -1:
		return 0;
	}	

	return nPos;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
 �Լ���: IsJTS
 ����  : �Ϻ�������üũ�Ѵ�
�Ķ���� : SaveFile�� �ؽ�Ʈ ������ �о�鿩�� ������ ����
���ϰ�  : �Ϻ��� �̸� TRUE, �Ϻ�� �ƴϸ� FALSE		
���� �߻� : ���� 16�������� ffffffa1�̹Ƿ� 1byte����(�ݰ� ���� ���� 0xA1~0xDF) ���� �����ϴ�.
            ���� �Ϻ���� �ѱ� ������ ������ �� ������ �Ϻ����(TRUE) ó�����ȴ�.
			�׷��� �ѱ����տ�(�� : ������)�Ϻ�� �� ������ �Ϻ�� �ƴ϶��(FALSE)  ����� ó���� �ȴ�.
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int IsJTS(char* SaveFile)
{
	int nCount = 0;
	int nStrlen = 0;
	int nReturnValue = -1;
    BOOL bJTS = FALSE;
	nStrlen = strlen(SaveFile);

	while(1)
	{		
		if(SaveFile[nCount] == 0)
		{
			if(nStrlen == nCount)
			{
				bJTS = TRUE;
				nReturnValue = -1;
				break;
			}
			
		}
		if((((*(SaveFile + nCount)&0xff) >= 0x21) && ((*(SaveFile + nCount)&0xff) <= 0x7E)) //ascii ����E ����
			|| (((*(SaveFile + nCount)&0xff) >= 0xA1) && ((*(SaveFile + nCount)&0xff) <= 0xDF))) //1BYTE ���� (�ݰ� ����)
		{
			nCount+=1;
		}
		else if((((*(SaveFile + nCount)&0xff) >= 0x81) && ((*(SaveFile + nCount)&0xff) <= 0x9F))
			||  (((*(SaveFile + nCount)&0xff) >= 0xE0) && ((*(SaveFile + nCount)&0xff) <= 0xEF)))
		{
			if((((*(SaveFile + nCount)&0xff) >= 0x40) && ((*(SaveFile + nCount)&0xff) <= 0x7E))
			|| (((*(SaveFile + nCount)&0xff) >= 0x80) && ((*(SaveFile + nCount)&0xff) <= 0xFC)))
			{
				nCount+=2;
			}
			else
			{
				bJTS = FALSE;
				nReturnValue = nCount;
				break;
			}
		}
		else
		{
			bJTS = FALSE;
			nReturnValue = nCount;
			break;
		}
	}
	if(!bJTS)
	{
		return nReturnValue;
	}

	return nReturnValue;
}
