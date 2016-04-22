#include <math.h>
#include <stdio.h>

#include "smd3d.h"


extern HWND hwnd;

#define WINMODE			01



int WindowMode = WINMODE;
int	smTextureBPP = 16;

int smScreenWidth, smScreenHeight;
int smFlipCount =0;					//�ø��� ī����

/***************************************
* ���� ����
***************************************/

D3DRECT                 D3DRect;
LPDEVICEDESC			lpD3DDeviceDesc = NULL;

// DirectDraw
//LPDIRECTDRAW            lpdd		 = NULL;           
LPDIRECTDRAW4           lpDD		 = NULL;
LPDIRECTDRAWSURFACE4    lpDDSPrimary = NULL;
LPDIRECTDRAWSURFACE4    lpDDSBack    = NULL;
LPDIRECTDRAWCLIPPER     lpDDClipper	   = NULL;      

// Direct3D
LPDIRECT3D3             lpD3D          = NULL;
LPDIRECT3DDEVICE3       lpD3DDevice    = NULL;
LPDIRECT3DVIEWPORT3     lpD3DViewport  = NULL;
LPDIRECTDRAWSURFACE4    lpDDSZBuffer   = NULL;


static LPDEVICEDESC lpDeviceDesc = NULL;

int MESSAGE( char *t)
{
	FILE *fp;
	fopen_s( &fp,  "error.log" , "wb" );
	if ( fp!=NULL ) {
		fwrite( t , lstrlen( t ) , 1, fp );
		fclose(fp);
	}

	return NULL;
}


LPDEVICEDESC FindDevice( LPDEVICEDESC lpDesc, LPGUID lpGuid )
{
	LPDEVICEDESC desc = lpDesc;

	while ( desc )
	{
		if ( !memcmp( lpGuid, &desc->guid, sizeof(GUID) ) )
			break;

		desc = desc->lpNext;
	}

	return desc;
}


LPDEVICEDESC FindBestDevice( LPDEVICEDESC lpDesc )
{
	LPDEVICEDESC desc;

	desc = FindDevice( lpDesc, (LPGUID)&IID_IDirect3DHALDevice );
	if ( !desc )
		desc = FindDevice( lpDesc, (LPGUID)&IID_IDirect3DMMXDevice );

	if ( !desc )
		desc = FindDevice( lpDesc, (LPGUID)&IID_IDirect3DRGBDevice );

	return desc;
}



LPDEVICEDESC CreateDevice()
{
    LPDEVICEDESC desc = new DEVICEDESC;
    if ( !desc )
		return NULL;

    ZeroMemory( desc, sizeof(DEVICEDESC) );

    return desc;
}

LPDEVICEDESC CreateDevice( LPDEVICEDESC lpDesc )
{
    if ( !lpDesc )
        return CreateDevice();

    LPDEVICEDESC desc = new DEVICEDESC;
    if ( !desc )
        return NULL;

	ZeroMemory( desc, sizeof(DEVICEDESC) );

    LPDEVICEDESC lastDesc = lpDesc;
    while ( lastDesc->lpNext )
        lastDesc = lastDesc->lpNext;

    lastDesc->lpNext = desc;

    return desc;
}

void DestroyDevice( LPDEVICEDESC lpDesc )
{
    LPDEVICEDESC desc;

    while ( lpDesc->lpNext )
	{
        desc = lpDesc->lpNext;
        lpDesc->lpNext = lpDesc->lpNext->lpNext;

        delete [] desc;
	}

    delete [] lpDesc;
}

static DWORD FlagsToBitDepth( DWORD dwFlags )
{
    if ( dwFlags & DDBD_1 )
        return 1L;
    else if ( dwFlags & DDBD_2 )
        return 2L;
    else if ( dwFlags & DDBD_4 )
        return 4L;
    else if ( dwFlags & DDBD_8 )
        return 8L;
    else if ( dwFlags & DDBD_16 )
        return 16L;
    else if ( dwFlags & DDBD_24 )
        return 24L;
    else if ( dwFlags & DDBD_32 )
        return 32L;
    else
        return 0L; 
}


// Direct3D Device EnumCallback �Լ�
HRESULT WINAPI DeviceEnumCallback( LPGUID          lpGuid,
                                   LPSTR           lpDeviceDescription,
                                   LPSTR           lpDeviceName,
                                   LPD3DDEVICEDESC lpD3DHWDeviceDesc,
                                   LPD3DDEVICEDESC lpD3DHELDeviceDesc,
                                   LPVOID          lpUserArg )
{
    if ( !lpUserArg )
        return DDENUMRET_OK;


    // Direct3D Device ����Ʈ�� �߰�
	LPDEVICEDESC desc, *lpDesc = (LPDEVICEDESC *)lpUserArg;
    if ( !*lpDesc )
		desc = *lpDesc = CreateDevice();
	else
		desc = CreateDevice( *lpDesc );

	// Guid�� ����
	memcpy( &desc->guid, lpGuid, sizeof(GUID) );

	// ����̽� ���� ����
	strcpy_s( desc->szName, lpDeviceName );
	strcpy_s( desc->szDesc, lpDeviceDescription );


	// �ϵ���� ����̺� ���� �˻� �ϰ� ����̺� ���� ����
	if ( lpD3DHWDeviceDesc->dcmColorModel )
	{
		desc->bIsHardware = TRUE;
		memcpy( &desc->Desc, lpD3DHWDeviceDesc, sizeof(D3DDEVICEDESC) );
	}
	else
	{
		desc->bIsHardware = FALSE;
		memcpy( &desc->Desc, lpD3DHELDeviceDesc, sizeof(D3DDEVICEDESC) );
	}

    return DDENUMRET_OK;
}






// Direct3D ����
BOOL CreateD3D()
{
    // DirectDraw �������̽� ����
    LPDIRECTDRAW lpdd;

    // ������ DirectDraw Device
    HRESULT hresult = DirectDrawCreate( NULL, &lpdd, NULL );
    if ( hresult != DD_OK )
	{
		MESSAGE( "DirectDrawCreate" );
        return FALSE;
	}

    // DirectDraw2 �������̽� ���
    hresult = lpdd->QueryInterface( IID_IDirectDraw4, (LPVOID*)&lpDD );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpdd->QueryInterface" );
        return FALSE;
	}

    // DirectDraw �������̽� ����
    lpdd->Release();


    // Direct3D �������̽� ���
    hresult = lpDD->QueryInterface( IID_IDirect3D3, (LPVOID*)&lpD3D );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD3->QueryInterface" );
        return FALSE;
	}

    // Direct3D Device �������̽� ���
	hresult = lpD3D->EnumDevices( DeviceEnumCallback, (LPVOID)&lpDeviceDesc );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpD3D->EnumDevices" );
        return FALSE;
	}

	lpD3DDeviceDesc = FindBestDevice( lpDeviceDesc );
	if ( !lpD3DDeviceDesc )
		return FALSE;

    return TRUE;
}

// Direct3D ����
void ReleaseD3D()
{
	//######################################################################################
	//�� �� �� : �� �� ��
	DestroyDevice( lpDeviceDesc );
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	ReleaseNewRenderTarget();
	//######################################################################################

    // Viewport ����
    if ( lpD3DViewport )
    {
        // Direct3D Device ���� Viewport ����
        lpD3DDevice->DeleteViewport( lpD3DViewport );
		
        lpD3DViewport->Release();
        lpD3DViewport = NULL;
    }

    // Direct3D Device ����
    if ( lpD3DDevice )
    {
        lpD3DDevice->Release();
        lpD3DDevice = NULL;
    }


    // Z-Buffer Surface ����
    if ( lpDDSZBuffer )
    {
        // Back Surface ���� Z-Buffer Surface ����
        if ( lpDDSBack )
            lpDDSBack->DeleteAttachedSurface( 0L, lpDDSZBuffer );

        lpDDSZBuffer->Release();
        lpDDSZBuffer = NULL;
    }

	//######################################################################################
	//�� �� �� : �� �� ��
	if ( lpDDSBack )
	{
		lpDDSBack->Release();
        lpDDSBack = NULL;
	}
	//######################################################################################

    // Direct3D Interface ����
    if ( lpD3D )
    {
        lpD3D->Release();
        lpD3D = NULL;
    }

    // Primary Surface ����
    if ( lpDDSPrimary )
    {
        lpDDSPrimary->Release();
        lpDDSPrimary = NULL;
    }

    // DirectDraw2 Interface ����
    if ( lpDD )
    {
        // ���� ��� ����
        lpDD->RestoreDisplayMode();

        lpDD->Release();
        lpDD = NULL;
    }
}


// Viewport ����
BOOL CreateViewport()
{

    D3DVIEWPORT2 viewport;

    // Viewport ����
    HRESULT hresult = lpD3D->CreateViewport( &lpD3DViewport, NULL );
    if ( hresult != D3D_OK )
        return FALSE;

    // Direct3D Device �� Viewport �߰�
    hresult = lpD3DDevice->AddViewport( lpD3DViewport );
    if ( hresult != D3D_OK )
        return FALSE;


    ZeroMemory( &viewport, sizeof(D3DVIEWPORT2) );

    viewport.dwSize         = sizeof( viewport );
    viewport.dwX            = D3DRect.x1;
    viewport.dwY            = D3DRect.y1;
	
    viewport.dwWidth        = D3DRect.x2;
    viewport.dwHeight       = D3DRect.y2;	
    viewport.dvClipWidth    = 2.0f;
    viewport.dvClipHeight   = (D3DVALUE)(D3DRect.y2 * 2.0 / D3DRect.x2);
    viewport.dvClipX        = -1.0f;
    viewport.dvClipY        = viewport.dvClipHeight / 2.0f;
    viewport.dvMinZ         = 0.0f;
    viewport.dvMaxZ         = 1.0f;

    //viewport.dvMinZ         = 0.0f;
    //viewport.dvMaxZ         = 1.0f;

    // lpD3DViewport2 ����
    hresult = lpD3DViewport->SetViewport2( &viewport );
    if ( hresult != D3D_OK )
        return FALSE;

    // ���� Viewport�� lpD3DViewport2�� ����
    hresult = lpD3DDevice->SetCurrentViewport( lpD3DViewport );
    if ( hresult != D3D_OK )
        return FALSE;

	//######################################################################################
	//�� �� �� : �� �� ��
	if( g_IsCreateFilterEffect )
        CreateNewRenderTarget();
	//######################################################################################

    return TRUE;
}

static HRESULT WINAPI EnumZBufferCallback( DDPIXELFORMAT *DDP_Format , VOID *DDP_Desired )
{
	if ( DDP_Format->dwFlags == DDPF_ZBUFFER ) {
		memcpy ( DDP_Desired , DDP_Format , sizeof( DDPIXELFORMAT ) );
		return D3DENUMRET_CANCEL;
	}
	return D3DENUMRET_OK;
}



BOOL SetDisplayModeWin( HWND hWnd, DWORD Width, DWORD Height, DWORD BPP );


// ���� ��� ��ȯ
BOOL SetDisplayMode( HWND hWnd, DWORD Width, DWORD Height, DWORD BPP )
{
    // Set Cooperative Level

	smTextureBPP = BPP;

	if ( WindowMode ) return SetDisplayModeWin( hWnd , Width , Height , BPP );
	
    HRESULT hresult = lpDD->SetCooperativeLevel( hWnd,
                                                  DDSCL_EXCLUSIVE |
                                                  DDSCL_FULLSCREEN |
												  DDSCL_ALLOWMODEX );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD->SetCooperativeLevel" );
        return FALSE;
	}


    // Ǯȭ�� ���� ��ȯ
    hresult = lpDD->SetDisplayMode( Width, Height, BPP, 0, 0 );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD3->SetDisplayMode" );
        return FALSE;
	}


    // Primary Surface ����
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof(ddsd) );

    ddsd.dwSize             = sizeof(ddsd);
    ddsd.dwBackBufferCount  = 1;
    ddsd.dwFlags            = DDSD_CAPS |
                              DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps     = DDSCAPS_PRIMARYSURFACE |
                              DDSCAPS_FLIP |
                              DDSCAPS_COMPLEX |
                              DDSCAPS_VIDEOMEMORY |
                              DDSCAPS_3DDEVICE;

    // Primary surface ����
    hresult = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD->CreateSurface(lpDDSPrimary)" );
        return FALSE;
	}

    // Back Surface ����(?)
    DDSCAPS2 ddscaps;
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    hresult = lpDDSPrimary->GetAttachedSurface( &ddscaps, &lpDDSBack );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDDSPrimary->GetAttachedSurface" );
        return FALSE;
	}


	//////////// Ŭ���� ���� ////////////////////////
	lpDD->CreateClipper( 0, &lpDDClipper , NULL );
	lpDDClipper->SetHWnd( 0, hWnd );
	lpDDSPrimary->SetClipper( lpDDClipper );
	lpDDClipper->Release();



    // z-buffer Surface ����
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize            = sizeof(ddsd);
    ddsd.dwFlags           = DDSD_CAPS |
                             DDSD_WIDTH |
                             DDSD_HEIGHT |
                             DDSD_PIXELFORMAT;
    ddsd.dwWidth           = Width;
    ddsd.dwHeight          = Height;

	lpD3D->EnumZBufferFormats( IID_IDirect3DHALDevice , EnumZBufferCallback , (VOID *)&ddsd.ddpfPixelFormat );

	//######################################################################################
	//�� �� �� : �� �� ��
	::CopyMemory( &g_ddpfPixelFormatZ, &ddsd.ddpfPixelFormat, sizeof(g_ddpfPixelFormatZ) );
	//######################################################################################

    // �ϵ���� �̸� z-buffer�� ���� �޸𸮿� �����.
    if ( lpD3DDeviceDesc->bIsHardware )
        ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
    else
        ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_SYSTEMMEMORY;

    // Create the ZBuffer surface.
    hresult = lpDD->CreateSurface( &ddsd, &lpDDSZBuffer, NULL );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD2->CreateSurface(lpDDSZBuffer)" );
        return FALSE;
	}

    // Back Surface�� Z-buffer�� ���δ�.
    hresult = lpDDSBack->AddAttachedSurface( lpDDSZBuffer );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDDSBack->AddAttachedSurface" );
        return FALSE;
	}

    // Direct3D Device ����
    hresult = lpD3D->CreateDevice( lpD3DDeviceDesc->guid,
                                    lpDDSBack,
                                    &lpD3DDevice,
									NULL );
    if ( hresult != D3D_OK )
	{
		MESSAGE( "lpD3D->CreateDevice" );
        return FALSE;
	}

    // Viewport ũ�� ����
    D3DRect.x1 = 0;
    D3DRect.y1 = 0;
    D3DRect.x2 = Width;
    D3DRect.y2 = Height;

	smScreenWidth = Width;
	smScreenHeight = Height;

    return TRUE;
}




// ���� ��� ��ȯ
BOOL SetDisplayModeWin( HWND hWnd, DWORD Width, DWORD Height, DWORD BPP )
{
    // Set Cooperative Level

	
    HRESULT hresult = lpDD->SetCooperativeLevel( hWnd,DDSCL_NORMAL );

    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD->SetCooperativeLevel" );
        return FALSE;
	}


    // Primary Surface ����
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );

    ddsd.dwSize             = sizeof(ddsd);
    ddsd.ddsCaps.dwCaps     = DDSCAPS_PRIMARYSURFACE |
                              DDSCAPS_3DDEVICE;

    // Primary surface ����
    hresult = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD2->CreateSurface(lpDDSPrimary)" );
        return FALSE;
	}


	int w,h;
	RECT lpRect;
	GetWindowRect( GetDesktopWindow() , &lpRect );
	w = lpRect.right - lpRect.left;
	h = lpRect.bottom - lpRect.top;

	// �� ���� 1 ����
    ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize=sizeof(ddsd);
    ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_3DDEVICE| DDSCAPS_VIDEOMEMORY;
    ddsd.dwWidth  = w;
    ddsd.dwHeight = h;
    lpDD->CreateSurface(&ddsd,&lpDDSBack,NULL);

	lpDD->CreateClipper( 0, &lpDDClipper , NULL );
	lpDDClipper->SetHWnd( 0, hWnd );
	lpDDSPrimary->SetClipper( lpDDClipper );
	lpDDClipper->Release();


	DDPIXELFORMAT	ddpx;
    // z-buffer Surface ����
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize            = sizeof(ddsd);
    ddsd.dwFlags           = DDSD_CAPS |
                             DDSD_WIDTH |
                             DDSD_HEIGHT|
                             DDSD_PIXELFORMAT;

    ddsd.dwWidth           = w;
    ddsd.dwHeight          = h;

	lpD3D->EnumZBufferFormats( lpD3DDeviceDesc->guid , EnumZBufferCallback , (VOID *)&ddpx );

	memcpy( &ddsd.ddpfPixelFormat , &ddpx , sizeof( DDPIXELFORMAT ) );

	//######################################################################################
	//�� �� �� : �� �� ��
	::CopyMemory( &g_ddpfPixelFormatZ, &ddsd.ddpfPixelFormat, sizeof(g_ddpfPixelFormatZ) );
	//######################################################################################

    // �ϵ���� �̸� z-buffer�� ���� �޸𸮿� �����.
    if ( lpD3DDeviceDesc->bIsHardware )
        ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
    else
        ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_SYSTEMMEMORY;

    // Create the ZBuffer surface.
    hresult = lpDD->CreateSurface( &ddsd, &lpDDSZBuffer, NULL );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDD2->CreateSurface(lpDDSZBuffer)" );
        return FALSE;
	}

    // Back Surface�� Z-buffer�� ���δ�.
    hresult = lpDDSBack->AddAttachedSurface( lpDDSZBuffer );
    if ( hresult != DD_OK )
	{
		MESSAGE( "lpDDSBack->AddAttachedSurface" );
        return FALSE;
	}

    // Direct3D Device ����
    hresult = lpD3D->CreateDevice( lpD3DDeviceDesc->guid,
									lpDDSBack,
									&lpD3DDevice,
									NULL );
    if ( hresult != D3D_OK )
	{
		MESSAGE( "lpD3D->CreateDevice" );
        return FALSE;
	}

    // Viewport ũ�� ����
    D3DRect.x1 = 0;
    D3DRect.y1 = 0;
    D3DRect.x2 = w;
    D3DRect.y2 = h;

	smScreenWidth = Width;
	smScreenHeight = Height;

	return TRUE;
}


extern int ParkPlaying;			//������ �÷�����
extern RECT ParkPlayRect;		//

void updateFrameWin_Video()
{
	RECT                rcRect;
    RECT                rcRect1;
	RECT                rcRect2;
	RECT                rcRect3;
	RECT                rcRect4;

    RECT                destRect;
	RECT                destRect1;
	RECT                destRect2;
	RECT                destRect3;
	RECT                destRect4;
    HRESULT             ddrval;
	POINT				pt;

    GetClientRect(hwnd,&destRect);

    pt.x=pt.y=0;
    ClientToScreen(hwnd,&pt);
    OffsetRect(&destRect,pt.x,pt.y);


   //////////////////////////////////////////////////////////////
   // �ΰ��� ������ ��ȯ�� �̷�� ���� �� 
   // Blt() �Լ��� ������ ��Ʈ���� �׸��� �Լ�

	rcRect.left   = 0;
	rcRect.right  = smScreenWidth;
	rcRect.top    = 0;
	rcRect.bottom = smScreenHeight;

	destRect1.left	= 0;
	destRect1.right	= smScreenWidth;
	destRect1.top	= 0;
	destRect1.bottom = ParkPlayRect.top;

	destRect2.left	= 0;
	destRect2.right	= ParkPlayRect.left;
	destRect2.top	= ParkPlayRect.top;
	destRect2.bottom = ParkPlayRect.bottom;

	destRect3.left	= ParkPlayRect.right;
	destRect3.right	= smScreenWidth;
	destRect3.top	= ParkPlayRect.top;
	destRect3.bottom = ParkPlayRect.bottom;

	destRect4.left	= 0;
	destRect4.right	= smScreenWidth;
	destRect4.top	= ParkPlayRect.bottom;
	destRect4.bottom = smScreenHeight;


	rcRect1.left   = 0;
	rcRect1.right  = smScreenWidth;
	rcRect1.top    = 0;
	rcRect1.bottom = ParkPlayRect.top;


	rcRect2.left   = 0;
	rcRect2.right  = ParkPlayRect.left;
	rcRect2.top    = ParkPlayRect.top;
	rcRect2.bottom = ParkPlayRect.bottom;


	rcRect3.left   = ParkPlayRect.right;
	rcRect3.right  = smScreenWidth;
	rcRect3.top    = ParkPlayRect.top;
	rcRect3.bottom = ParkPlayRect.bottom;

	rcRect4.left   = 0;
	rcRect4.right  = smScreenWidth;
	rcRect4.top    = ParkPlayRect.bottom;
	rcRect4.bottom = smScreenHeight;


	while(1)
	{
		//ddrval=lpDDSPrimary->Blt(&destRect,lpDDSBack,&rcRect,NULL,NULL);
		ddrval=lpDDSPrimary->Blt(&destRect1,lpDDSBack,&rcRect1,NULL,NULL);
		ddrval=lpDDSPrimary->Blt(&destRect2,lpDDSBack,&rcRect2,NULL,NULL);
		ddrval=lpDDSPrimary->Blt(&destRect3,lpDDSBack,&rcRect3,NULL,NULL);
		ddrval=lpDDSPrimary->Blt(&destRect4,lpDDSBack,&rcRect4,NULL,NULL);
		if(ddrval == DD_OK) break;
		if(ddrval == DDERR_SURFACELOST)
		{
	//		ddrval = restoreAll();
			if(ddrval != DD_OK)	return;
		}
		if(ddrval != DDERR_WASSTILLDRAWING) break;

	}

} 

void updateFrameWin()
{
    RECT                rcRect;
    RECT                destRect;
    HRESULT             ddrval;
	POINT				pt;

    GetClientRect(hwnd,&destRect);

    pt.x=pt.y=0;
    ClientToScreen(hwnd,&pt);
    OffsetRect(&destRect,pt.x,pt.y);
   //////////////////////////////////////////////////////////////
   // �ΰ��� ������ ��ȯ�� �̷�� ���� �� 
   // Blt() �Լ��� ������ ��Ʈ���� �׸��� �Լ�

	rcRect.left   = 0;
	rcRect.right  = smScreenWidth;
	rcRect.top    = 0;
	rcRect.bottom = smScreenHeight;



	while(1)
	{
		ddrval=lpDDSPrimary->Blt(&destRect,lpDDSBack,&rcRect,NULL,NULL);
		if(ddrval == DD_OK) break;
		if(ddrval == DDERR_SURFACELOST)
		{
	//		ddrval = restoreAll();
			if(ddrval != DD_OK)	return;
		}
		if(ddrval != DDERR_WASSTILLDRAWING) break;

	}

} 

extern HWND hFocusWnd;

// ������ �ø���
int Flip()
{
	//������ ���
	if ( ParkPlaying && (smFlipCount&1)==0 )  {
		updateFrameWin_Video();
		return TRUE;
	}

	if ( WindowMode || ( hFocusWnd && (smFlipCount&1)==0) ) {
		updateFrameWin();
		return TRUE;
	}


    HRESULT hresult = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT );
    if ( hresult == DDERR_SURFACELOST ) {
        lpDDSPrimary->Restore();
		return FALSE;
	}
	smFlipCount++;			//�ø��� ī����

	return TRUE;
}


// ���帵 �ʱ�ȭ
void InitRender()
{

    // Turn on Z-buffering

    lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
    lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
    lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL );
//	lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_USEW );//TRUE );


    // null out the texture handle
    lpD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREHANDLE, 0 );

    // turn on dithering
    lpD3DDevice->SetRenderState( D3DRENDERSTATE_DITHERENABLE, TRUE );

	lpD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP );
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE ); 

	// D3DFILTER_LINEAR
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR );
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR );
 
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_SPECULARENABLE, TRUE );
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, FALSE );

	lpD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE );

	lpD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpD3DDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );

	smRender.AlphaTestDepth = 60;
	smRender.dwMatDispMask = sMATS_SCRIPT_NOTVIEW;
	smRender.DeviceRendMode = TRUE;
	smRender.ZWriteAuto = FALSE;
}

extern DWORD smBackColor;

// ���帵 ����
void BeginRender()
{
    // Primary Surface�� Lost�Ǿ����� �˻�
    //if ( lpDDSPrimary->IsLost() == DDERR_SURFACELOST )
    //   lpDDSPrimary->Restore();

    // Viewport �����
    lpD3DViewport->Clear2( 1UL, &D3DRect, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET  , 
		smBackColor, 1, 0L );
//    lpD3DViewport->Clear2( 1UL, &D3DRect, D3DCLEAR_ZBUFFER , 
//		smBackColor, 1, 0L );

/*
    // Back Surface �����
    DDBLTFX bltFx;
    ZeroMemory( &bltFx, sizeof(DDBLTFX) );
    bltFx.dwSize        = sizeof(DDBLTFX);
    bltFx.dwFillColor   = 65535;
    bltFx.dwFillColor   = 0;
    bltFx.dwFillColor   = (1<<15)+(15<<10)+(15<<5)+31;
    lpDDSBack->Blt( NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &bltFx );
*/

    //lpD3DDevice->BeginScene();

	//smRender.SetMaterialGroup( smMaterialGroup );			//�⺻ ��Ʈ���� �׷�
}

// ���帵 ��
void EndRender()
{
    //lpD3DDevice->EndScene();
}


