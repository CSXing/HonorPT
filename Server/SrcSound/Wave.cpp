#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include "wave.h"
#include "Log.h"

BOOL LSEnable = FALSE;

int WaveOpenFile(char *pszFileName, HMMIO *phmmioIn, WAVEFORMATEX **ppwfxInfo, MMCKINFO *pckInRIFF)
{
	Log::Debug("WaveOpenFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveStartDataRead(HMMIO *phmmioIn, MMCKINFO *pckIn, MMCKINFO *pckInRIFF)
{
	Log::Debug("WaveStartDataRead : Requisitado por�m foi removido.");
	return NULL;
}

int WaveReadFile(HMMIO hmmioIn, UINT cbRead, BYTE *pbDest, MMCKINFO *pckIn, UINT *cbActualRead)
{
	Log::Debug("WaveReadFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveCloseReadFile(HMMIO *phmmio, WAVEFORMATEX **ppwfxSrc)
{
	Log::Debug("WaveCloseReadFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveCreateFile(char *pszFileName, HMMIO *phmmioOut, WAVEFORMATEX *pwfxDest, MMCKINFO *pckOut, MMCKINFO *pckOutRIFF)
{
	Log::Debug("WaveCreateFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveStartDataWrite(HMMIO *phmmioOut, MMCKINFO *pckOut, MMIOINFO *pmmioinfoOut)
{
	Log::Debug("WaveStartDataWrite : Requisitado por�m foi removido.");
	return NULL;
}

int WaveWriteFile(HMMIO hmmioOut, UINT cbWrite, BYTE *pbSrc, MMCKINFO *pckOut, UINT *cbActualWrite, MMIOINFO *pmmioinfoOut)
{
	Log::Debug("WaveWriteFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveCloseWriteFile(HMMIO *phmmioOut, MMCKINFO *pckOut, MMCKINFO *pckOutRIFF, MMIOINFO *pmmioinfoOut, DWORD cSamples)
{
	Log::Debug("WaveCloseWriteFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveCopyUselessChunks(HMMIO *phmmioIn, MMCKINFO *pckIn, MMCKINFO *pckInRiff, HMMIO *phmmioOut, MMCKINFO *pckOut, MMCKINFO *pckOutRiff)
{
	Log::Debug("WaveCopyUselessChunks : Requisitado por�m foi removido.");
	return NULL;
}

BOOL riffCopyChunk(HMMIO hmmioSrc, HMMIO hmmioDst, const LPMMCKINFO lpck)
{
	Log::Debug("riffCopyChunk : Requisitado por�m foi removido.");
	return NULL;
}

int WaveLoadFile(char *pszFileName, UINT *cbSize, DWORD *pcSamples, WAVEFORMATEX **ppwfxInfo, BYTE **ppbData)
{
	Log::Debug("WaveLoadFile : Requisitado por�m foi removido.");
	return NULL;
}

int WaveSaveFile(char *pszFileName, UINT cbSize, DWORD cSamples, WAVEFORMATEX *pwfxDest, BYTE *pbData)
{
	Log::Debug("WaveSaveFile : Requisitado por�m foi removido.");
	return NULL;
}