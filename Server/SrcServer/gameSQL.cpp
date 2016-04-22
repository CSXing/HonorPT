#define SQLSOR_CPP


#include <windows.h>
#include <windowsx.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>


#include "gamesql.h"


#ifdef _W_SERVER

//��Ÿ �ӽ� ��� ���Ϸ� ����
int Record_TempLogFile( char *szMessage );


SQLDATA::SQLDATA()
{
}
SQLDATA::~SQLDATA()
{
}

/*///////////////////////////////////////////////////////////////////////////
: �Լ�  �� :  Start_ODBC(), End_ODBC()
: �ϴ�  �� :  �����Լ�
: ��    �� :
: ��ȯ  �� :
///////////////////////////////////////////////////////////////////////////*/
int SQLDATA::Start_ODBC()
{
    char *svr_name;
    char *user_name;
    char *user_pswd;

    int r;

	if ( Odbc_Config.Dsn[0] ) {
		svr_name = Odbc_Config.Dsn;
		user_name = Odbc_Config.Name;
		user_pswd = Odbc_Config.Password;
	}
	else {
	    svr_name = "c8master";
	    user_name = "c8master";
	    user_pswd = "joddo";
	}


    r = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_hdl);
    //fd("start 00 rrrrrrrr %d ", r);
    if( r == SQL_ERROR) return 0;
    SQLSetEnvAttr(env_hdl, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3,  SQL_IS_INTEGER);

    r = SQLAllocHandle(SQL_HANDLE_DBC, env_hdl, &conn_hdl);
    //fd("start 11 rrrrrrrr %d ", r);
    if(r== SQL_ERROR) return 0;

    //r = SQLConnect(conn_hdl,(SQLTCHAR*)svr_name, SQL_NTS,
    //                        (SQLTCHAR*)user_name, SQL_NTS,
    //                        (SQLTCHAR*)user_pswd, SQL_NTS);
    r = SQLConnect(conn_hdl,(unsigned char*)svr_name, SQL_NTS,
                    (unsigned char*)user_name, SQL_NTS,
                    (unsigned char*)user_pswd, SQL_NTS);

    //fd("start 22 rrrrrrrr %d ", r);
    if(r== SQL_ERROR) return 0;

    r = SQLAllocHandle(SQL_HANDLE_STMT, conn_hdl, &stmt_hdl);
    //fd("start 33 rrrrrrrr %d ", r);
    return 1;
}

int SQLDATA::End_ODBC()
{
    int r;

    if(stmt_hdl != NULL) {
        r = SQLFreeHandle(SQL_HANDLE_STMT, stmt_hdl);
        //fd("End 00 rrrrrrrr %d ", r);
        if(r== SQL_ERROR) return 0;
    }

    if(conn_hdl != NULL) {
        r = SQLDisconnect(conn_hdl);
        //fd("End 11 rrrrrrrr %d ", r);
        if(r== SQL_ERROR) return 0;
        r = SQLFreeHandle(SQL_HANDLE_DBC, conn_hdl);
        //fd("End 22 rrrrrrrr %d ", r);
        if(r== SQL_ERROR) return 0;
    }

    if(env_hdl != NULL) {
        r = SQLFreeHandle(SQL_HANDLE_ENV, env_hdl);
        //fd("End 33 rrrrrrrr %d ", r);
        if(r== SQL_ERROR) return 0;
    }
    return 1;
}

int	SQLDATA::LogOn( char *szID , char *szPassword )
{

    char id[128] = {0,};
    char pass[128] = {0,};
	char sell,block;
	//char	Temp[256];

	int r;

	//u_hope		->������ ( 0 - �Ϲ�  1 - ���Ӹ��ϰ� )
/*
	if ( !Odbc_Config.Dsn[0] ) {
		//�⺻ ���� 

		///////////////// ID ù���ڿ� ���� ���̺� ã�� ////////////////////// 
		BYTE	ch;
		char TableName[32];
	
		ch = (BYTE)szID[0];
		TableName[0]=0;

		if ( ch>='a' && ch<='z' ) {
			TableName[0] = ch;
			TableName[1] = 0;
		}
		else {
			if ( ch>='A' && ch<='Z' ) {
				TableName[0] = ch+0x20;
				TableName[1] = 0;
			}
			else {
				if ( ch>='0' && ch<='9' ) {
					lstrcpy( TableName , "number" );
				}
				else {
					lstrcpy( TableName , "etc" );
				}
			}
		}

		if ( !TableName[0] ) 
			return -1;
		/////////////////////////////////////////////////////////////////////////

		wsprintf( stmt_buf , "SELECT * FROM %s_member WHERE u_id='%s'" , TableName , szID );
	    r = SQLExecDirect(stmt_hdl, (unsigned char*)stmt_buf, SQL_NTS);
		if(r== SQL_ERROR) return 0;											//���� �Ұ�

		r = SQLBindCol(stmt_hdl, 3, SQL_C_CHAR, &pass, 9 , NULL);
		r = SQLBindCol(stmt_hdl, 17, SQL_C_UTINYINT , &sell, 1 , NULL);
		r = SQLBindCol(stmt_hdl, 20, SQL_C_UTINYINT , &block, 1 , NULL);

	    if(SQLFetch(stmt_hdl) == SQL_NO_DATA_FOUND ) 
			return -1;				//������ ����
	
	    SQLFreeStmt( stmt_hdl, SQL_CLOSE );

		if ( lstrcmpi( szPassword , pass )!=0 ) return -2;					//��� Ʋ��
		if ( Odbc_Config.CloseAccount && sell!=1 ) return -3;			//���Ѿ��� ( ��Ÿ�׽��� �ƴ� )
		if ( block ) return -3;											//���� ��
	}
*/

	if ( !Odbc_Config.Dsn[0] ) {
		//�⺻ ���� 

		///////////////// ID ù���ڿ� ���� ���̺� ã�� ////////////////////// 
		BYTE	ch;
		char TableName[32];
	
		ch = (BYTE)szID[0];
		TableName[0]=0;

		if ( ch>='a' && ch<='z' ) {
			// TableName[0] = ch;
			// TableName[1] = 0;
			// ����� �κ�
			TableName[0] = ch-0x20;
			TableName[1] = 0;
		}
		else {
			if ( ch>='A' && ch<='Z' ) {
				// TableName[0] = ch+0x20;
				// TableName[1] = 0;
				// ����� �κ�
				TableName[0] = ch;
				TableName[1] = 0;	
			}
			else {
				if ( ch>='0' && ch<='9' ) {
					// lstrcpy( TableName , "number" );
					// ����� �κ�
					lstrcpy( TableName , "[0GameUser]" );
				}
				else {
					// lstrcpy( TableName , "etc" );
					// ����� �κ�
					lstrcpy( TableName , "[9GameUser]" );
				}
			}
		}

		if ( !TableName[0] ) 
			return -1;
		/////////////////////////////////////////////////////////////////////////

		// wsprintf( stmt_buf , "SELECT * FROM %s_member WHERE u_id='%s'" , TableName , szID );
		// ����� �κ�
		wsprintf( stmt_buf , "SELECT * FROM %sGameUser WHERE userid='%s'" , TableName , szID );

		//��Ÿ �ӽ� ��� ���Ϸ� ����
		//wsprintf( Temp , "%s\r\n" , stmt_buf );
		//Record_TempLogFile( Temp );

	    r = SQLExecDirect(stmt_hdl, (unsigned char*)stmt_buf, SQL_NTS);
		if(r== SQL_ERROR) {
			return 0;											//���� �Ұ�
		}

		//r = SQLBindCol(stmt_hdl, 3, SQL_C_CHAR, &pass, 9 , NULL);
		//r = SQLBindCol(stmt_hdl, 17, SQL_C_UTINYINT , &sell, 1 , NULL);
		//r = SQLBindCol(stmt_hdl, 20, SQL_C_UTINYINT , &block, 1 , NULL);
		// ����� �κ�

		r = SQLBindCol(stmt_hdl, 2, SQL_C_CHAR, &pass, 9 , NULL);
		r = SQLBindCol(stmt_hdl, 11, SQL_C_UTINYINT , &sell, 1 , NULL);
		r = SQLBindCol(stmt_hdl, 12, SQL_C_UTINYINT , &block, 1 , NULL);


		if(SQLFetch(stmt_hdl) == SQL_NO_DATA_FOUND )  {

			return -1;				//������ ����
		}
	
	    SQLFreeStmt( stmt_hdl, SQL_CLOSE );

		if ( lstrcmpi( szPassword , pass )!=0 ) return -2;	//��� Ʋ��
		if ( Odbc_Config.CloseAccount && sell!=1 ) return -3;	//���Ѿ��� ( ��Ÿ�׽��� �ƴ� )
		if ( block ) return -3;					//���� ��
	}

	else {
		//���� ����  ( �Ѱ��� ��� )
		wsprintf( stmt_buf , "SELECT * FROM %s WHERE %s='%s'" , Odbc_Config.Table , Odbc_Config.Table_Id , szID );
	    r = SQLExecDirect(stmt_hdl, (unsigned char*)stmt_buf, SQL_NTS);
		if(r== SQL_ERROR) return 0;											//���� �Ұ�

		r = SQLBindCol(stmt_hdl, Odbc_Config.Table_Password , SQL_C_CHAR, &pass, 16 , NULL);
		if ( Odbc_Config.Table_Play ) {
			r = SQLBindCol(stmt_hdl, Odbc_Config.Table_Play, SQL_C_UTINYINT , &sell, 1 , NULL);
		}
		else {
			sell = 1;
		}

	    if(SQLFetch(stmt_hdl) == SQL_NO_DATA_FOUND ) return -1;				//������ ����
	
	    SQLFreeStmt( stmt_hdl, SQL_CLOSE );

		if ( lstrcmpi( szPassword , pass )!=0 ) return -2;					//��� Ʋ��
		if ( Odbc_Config.CloseAccount && sell!=1 ) return -3;											//���Ѿ��� ( ��Ÿ�׽��� �ƴ� )

	}

	return TRUE;		//���� ����
}



SQLDATA		SqlData;


int SqlLogOn(char *szID , char *szPass )
{
	int val;
	SqlData.Start_ODBC();
	val = SqlData.LogOn( szID , szPass );
	SqlData.End_ODBC();

	return val;
}

int LogTest()
{
	int val;

	SqlData.Start_ODBC();

	val=SqlData.LogOn( "penguinboy" , "pengo1" );

	SqlData.End_ODBC();

	return TRUE;
}

#endif


int	InitODBC()
{
	//return SqlData.Start_ODBC();
	return TRUE;
}
int	CloseODBC()
{
	//return SqlData.End_ODBC();
	return TRUE;

}
