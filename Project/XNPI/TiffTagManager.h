#pragma once
#include "tiff.h"
#include "tiffconf.h"
#include "tiffio.h"
#include "tiffvers.h"

class TiffTagManager
{
private:
	int m_exifOffset;
public:
	TiffTagManager();
	~TiffTagManager();

	void AttachTagFromFile(const char* filePath, const char* tag);
	void AttachTagFromFile(const char* filePath, const char* tag, double XPixelSize_mm, double YPixelSize_mm); //181016 - X,Y 픽셀 사이즈 설정
	void AttachTagFromFile(const char* filePath, int fps, int kv, int ma);
};

