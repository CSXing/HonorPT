#include <ddraw.h>
#include <d3d.h>
#include <d3dtypes.h>

//######################################################################################
//�� �� �� : �� �� ��
#include <Shlwapi.h>
//######################################################################################


/***************************************
* Direct3D �ʱ�ȭ�� ����ü
***************************************/

// Direct3D Device ����
// �������� �̿��Ͽ� IDirect3DDevice2�� �����մϴ�.
typedef struct _DEVICEDESC
{
    // Device Info
    GUID  guid;                 // GUID
    char  szName[256];          // �̸�
    char  szDesc[256];          // ����
    BOOL  bIsHardware;          // �ϵ���� ����̺� �ΰ�?

    // Device Caps
    D3DDEVICEDESC   Desc;
	_DEVICEDESC		*lpNext;
} DEVICEDESC, *LPDEVICEDESC;



extern int WindowMode;
extern int smTextureBPP;
extern int smFlipCount;

extern int smScreenWidth, smScreenHeight;

extern D3DRECT                 D3DRect;
extern LPDEVICEDESC            lpD3DDeviceDesc;

extern LPDIRECTDRAW4           lpDD;
extern LPDIRECTDRAWSURFACE4    lpDDSPrimary;
extern LPDIRECTDRAWSURFACE4    lpDDSBack;

extern LPDIRECT3D3	           lpD3D;
extern LPDIRECT3DDEVICE3       lpD3DDevice;
extern LPDIRECT3DVIEWPORT3     lpD3DViewport;
extern LPDIRECTDRAWSURFACE4    lpDDSZBuffer;





/***************************************
* Direct3D ������� �ʱ�ȭ �ϱ�
***************************************/


// Direct3D ����
BOOL CreateD3D();

// Direct3D ����
void ReleaseD3D();


// ���� ��� ��ȯ
BOOL SetDisplayMode( HWND hWnd, DWORD Width, DWORD Height, DWORD BPP );

// Back Surface �� DC���
HDC GetDC();

// DC ����
void ReleaseDC( HDC hDC );



// Primary Surface Lost �˻�
void IsLost();

// ������ �ø���
int Flip();

// Viewport ����
BOOL CreateViewport();

// Back Surface �����
void Clear( DWORD dwColor );


// ���帵 �ʱ�ȭ
void InitRender();

// ���帵 ����
void BeginRender();

// ���帵 ��
void EndRender();


//######################################################################################
//�� �� �� : �� �� ��
extern int MESSAGE( char *t);
//######################################################################################


typedef struct {
	float r, g, b, a;
} COLORVALUE, *LPCOLORVALUE;

typedef struct {
	float	x, y, z;
} VECTOR, *LPVECTOR;

typedef struct {
    float   tu, tv;
} TVERTEX, *LPTVERTEX;



#pragma comment( lib, "ddraw.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )


//######################################################################################
//�� �� �� : �� �� ��
#pragma comment(lib, "shlwapi.lib")
//######################################################################################


