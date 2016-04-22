
#include "..\\nettype.hpp"

#ifdef _W_SERVER

#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

class SQLDATA
{
public:
	SQLHENV env_hdl;
	SQLHDBC conn_hdl;
	SQLHSTMT stmt_hdl;

	char stmt_buf[512];
	char errmsg[512];

    SQLDATA();
    ~SQLDATA();

    int Start_ODBC();
    int End_ODBC();

	int	LogOn( char *szID , char *szPassword );

};


#endif
////////////////////////////////////////////////////////////////////////
struct	ODBC_CONFIG {
	char	Dsn[32];			//���� ���񽺸�
	char	Name[32];			//������ ID
	char	Password[32];		//������ ��ȣ

	char	Table[32];			//���̺� �̸�
	char	Table_Id[32];		//���̺� ����
	int		Table_Password;		//���̺� ��ȣ ��
	int		Table_Play;			//���̺� �㰡 ��

	int		CloseAccount;		//�׽�Ʈ ������ ���

};

extern	ODBC_CONFIG	Odbc_Config;


int	InitODBC();
int	CloseODBC();
int SqlLogOn(char *szID , char *szPass );



