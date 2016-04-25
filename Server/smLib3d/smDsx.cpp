#include <math.h>
#include <stdio.h>
#include "smd3d.h"
#include "Log.h"

#define WINMODE			01

int WindowMode = WINMODE;
int	smTextureBPP = 16;

int smScreenWidth, smScreenHeight;
int smFlipCount = 0;

D3DRECT                 D3DRect;
LPDEVICEDESC			lpD3DDeviceDesc = NULL;
    
LPDIRECTDRAW4           lpDD = NULL;
LPDIRECTDRAWSURFACE4    lpDDSPrimary = NULL;
LPDIRECTDRAWSURFACE4    lpDDSBack = NULL;
LPDIRECTDRAWCLIPPER     lpDDClipper = NULL;

LPDIRECT3D3             lpD3D = NULL;
LPDIRECT3DDEVICE3       lpD3DDevice = NULL;
LPDIRECT3DVIEWPORT3     lpD3DViewport = NULL;
LPDIRECTDRAWSURFACE4    lpDDSZBuffer = NULL;

static LPDEVICEDESC lpDeviceDesc = NULL;

int MESSAGE(char *t)
{
	FILE *fp;
	fopen_s(&fp, "error.log", "wb");
	if (fp != NULL)
	{
		fwrite(t, lstrlen(t), 1, fp);
		fclose(fp);
	}

	return NULL;
}

LPDEVICEDESC FindDevice(LPDEVICEDESC lpDesc, LPGUID lpGuid)
{
	Log::Debug("FindDevice : Requisitado por�m foi removido.");
	return nullptr;
}

LPDEVICEDESC FindBestDevice(LPDEVICEDESC lpDesc)
{
	Log::Debug("FindBestDevice : Requisitado por�m foi removido.");
	return nullptr;
}

LPDEVICEDESC CreateDevice()
{
	Log::Debug("CreateDevice : Requisitado por�m foi removido.");
	return nullptr;
}

LPDEVICEDESC CreateDevice(LPDEVICEDESC lpDesc)
{
	Log::Debug("CreateDevice : Requisitado por�m foi removido.");
	return nullptr;
}

void DestroyDevice(LPDEVICEDESC lpDesc)
{
	Log::Debug("DestroyDevice : Requisitado por�m foi removido.");
	return;
}

static DWORD FlagsToBitDepth(DWORD dwFlags)
{
	Log::Debug("FlagsToBitDepth : Requisitado por�m foi removido.");
	return NULL;
}

HRESULT WINAPI DeviceEnumCallback(LPGUID          lpGuid,
								  LPSTR           lpDeviceDescription,
								  LPSTR           lpDeviceName,
								  LPD3DDEVICEDESC lpD3DHWDeviceDesc,
								  LPD3DDEVICEDESC lpD3DHELDeviceDesc,
								  LPVOID          lpUserArg)
{
	Log::Debug("DeviceEnumCallback : Requisitado por�m foi removido.");
	return NULL;
}

BOOL CreateD3D()
{
	Log::Debug("CreateD3D : Requisitado por�m foi removido.");
	return NULL;
}

void ReleaseD3D()
{
	Log::Debug("ReleaseD3D : Requisitado por�m foi removido.");
	return;
}

BOOL CreateViewport()
{
	Log::Debug("CreateViewport : Requisitado por�m foi removido.");
	return NULL;
}

static HRESULT WINAPI EnumZBufferCallback(DDPIXELFORMAT *DDP_Format, VOID *DDP_Desired)
{
	Log::Debug("EnumZBufferCallback : Requisitado por�m foi removido.");
	return NULL;
}

BOOL SetDisplayMode(HWND hWnd, DWORD Width, DWORD Height, DWORD BPP)
{
	Log::Debug("SetDisplayMode : Requisitado por�m foi removido.");
	return NULL;
}

BOOL SetDisplayModeWin(HWND hWnd, DWORD Width, DWORD Height, DWORD BPP)
{
	Log::Debug("SetDisplayModeWin : Requisitado por�m foi removido.");
	return NULL;
}

void updateFrameWin_Video()
{
	Log::Debug("updateFrameWin_Video : Requisitado por�m foi removido.");
	return;
}

void updateFrameWin()
{
	Log::Debug("updateFrameWin : Requisitado por�m foi removido.");
	return;
}

int Flip()
{
	Log::Debug("Flip : Requisitado por�m foi removido.");
	return NULL;
}

void InitRender()
{
	Log::Debug("InitRender : Requisitado por�m foi removido.");
	return;
}

void BeginRender()
{
	Log::Debug("BeginRender : Requisitado por�m foi removido.");
	return;
}

void EndRender()
{
	Log::Debug("EndRender : Requisitado por�m foi removido.");
	return;
}