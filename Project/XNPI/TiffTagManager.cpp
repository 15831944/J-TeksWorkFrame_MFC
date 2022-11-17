#include "stdafx.h"
#include "TiffTagManager.h"


TiffTagManager::TiffTagManager()
	:m_exifOffset(8)
{
}

TiffTagManager::~TiffTagManager()
{
}

void TiffTagManager::AttachTagFromFile(const char* filePath, const char* tag)
{
	TIFF* tif = TIFFOpen(filePath, "a");

	TIFFSetDirectory(tif, 0);

	TIFFSetField(tif, TIFFTAG_EXIFIFD, m_exifOffset);
	TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, tag);

	TIFFCheckpointDirectory(tif);

	TIFFClose(tif);
}

void TiffTagManager::AttachTagFromFile(const char * filePath, const char * tag, double XPixelSize_mm, double YPixelSize_mm)
{
	TIFF* tif = TIFFOpen(filePath, "a");

	TIFFSetDirectory(tif, 0);

	TIFFSetField(tif, TIFFTAG_EXIFIFD, m_exifOffset);
	TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, tag);

	//181016 
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, 3); //centemeter ¥‹¿ß
	TIFFSetField(tif, TIFFTAG_XRESOLUTION, XPixelSize_mm);	//DPI
	TIFFSetField(tif, TIFFTAG_YRESOLUTION, YPixelSize_mm);	//DPI

	TIFFCheckpointDirectory(tif);

	TIFFClose(tif);
}

void TiffTagManager::AttachTagFromFile(const char* filePath, int fps, int kv, int ma)
{
	CString str = NULL;
	str.Format(_T("FPS:%d, KV:%d, MA:%d"), fps, kv, ma);

	AttachTagFromFile(filePath, (CStringA)str);
}
