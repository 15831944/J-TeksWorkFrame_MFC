#include "StdAfx.h"
#include "MilBuffer.h"
#include <math.h>
#include "atlimage.h"
int CMilBuffer::m_milIDBuffCounter=0;

CMilBuffer::CMilBuffer(void):m_iFileFormat(M_BMP), m_iDepth(8+M_UNSIGNED), m_BufAttr(M_IMAGE+M_DISP)
, m_pHWndSelected(NULL)
{
	m_pMilDisplay=NULL;
	m_milIDBuffCounter++;
	m_milIDSystem = CMilSystem::Instance()->GetSystemID();
}

CMilBuffer::CMilBuffer(CSize ImageSize):m_iFileFormat(M_BMP), m_iDepth(8+M_UNSIGNED), m_BufAttr(M_DISP)
{
	m_pMilDisplay=NULL;
	m_milIDSystem=CMilSystem::Instance()->GetSystemID();
	MbufAlloc2d(m_milIDSystem, ImageSize.cx, ImageSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
}


CMilBuffer::~CMilBuffer(void)
{
	Close();	
}

/*! @fn int CMilBuffer::GetMilCounter(void)
 *  @brief 할당된 Mil버퍼카운트를 한다.(Child포함)
 *  @todo 현재 사용할 수 없는 상태이며 수정하여 완료하여야 한다.
 *  @return 할당된 카운트갯수를 반환한다.
 */
int CMilBuffer::GetMilCounter(void)
{
	return m_milIDBuffCounter;
}


/*! @fn int CMilBuffer::Restore(WCHAR * strFileName)
 *  @brief 파일을 읽어 MilBuffer에 넣는다.
 @n   Buffer Alloc이 자동으로 이루어지고 File포멧도 자동으로 확인하여 읽어온다
 *  @param strFileName 읽어올 파일의 이름
 *  @return 0이면 에러 다른 값은 정상이다
 */
int CMilBuffer::Restore(CString strFileName)
{
	tImageProperties ImageProperties;
	int rv=MbufRestore(strFileName.GetBuffer(), m_milIDSystem, &m_milIDBuffer);
	if(rv==0)return -1;
	GetImageProperties(&ImageProperties);
	m_BufAttr=ImageProperties.Attr;
	m_iDepth=ImageProperties.Depth;
	return 0;
}

/*! @fn void CMilBuffer::Select(CMilDisplay * pMilDisplay)
 *  @brief Mil버퍼를 보일 Display를 연결한다.
 *  @param pMilDisplay 연결시킬 MilDisplay(할당되어 있어야함)
 */
void CMilBuffer::Select(CMilDisplay * pMilDisplay)
{
	MdispSelect(pMilDisplay->GetDisplayID(), m_milIDBuffer);
}

/*! @fn void CMilBuffer::SelectWindow(HWND ClientWindowHandle)
 *  @brief Mil버퍼를 보일 Dialog와같은 원도우를 설정하는 함수
 *  @param ClientWindowHandle 연결시킬 원도우의 핸들
 */
void CMilBuffer::SelectWindow(HWND ClientWindowHandle)
{
	MdispSelectWindow(m_pMilDisplay->GetDisplayID(), m_milIDBuffer, ClientWindowHandle);
}

/*! @fn void CMilBuffer::UnSelectDisp(void)
 *  @brief Mil Buffer와 Display와 연결된것을 해지한다.
 */
void CMilBuffer::UnSelectDisp(void)
{
	MdispSelect(m_pMilDisplay->GetDisplayID(), NULL);
}

/*! @fn void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, HWND ClientWindowHandle)
 *  @brief 파라메터로 들어오는 Display로 윈도우 핸들로 MilBuffer를 연결한다.
 *	@param pMilDisplay 연결할 Display buffer
 *  @param ClientWindowHandle 표시할 Dialog와 같은 
 */
void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, HWND ClientWindowHandle)
{
	MdispSelectWindow(pMilDisplay->GetDisplayID(), m_milIDBuffer, ClientWindowHandle);
}

/*! @fn void CMilBuffer::SelectWindow(CWnd *pHWindow)
 *  @brief Mil Buffer와 윈도우 핸들을 연결시킨다(화면에 보이기위해)
 *  @param pHWindow Mil Buffer를 보일 윈도우 핸들 Class
 */
void CMilBuffer::SelectWindow(CWnd *pHWindow)
{
	m_pHWndSelected = pHWindow;
	MdispSelectWindow(m_pMilDisplay->GetDisplayID(), m_milIDBuffer, pHWindow->GetSafeHwnd());
}

/*! @fn void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, CWnd *pHWindow)
 *  @brief Mil Buffer와 윈도우 핸들을 연결시킨다(화면에 보이기위해)
 *  @param pHWindow Mil Buffer를 보일 윈도우 핸들 Class
 */
void CMilBuffer::SelectWindow(CMilDisplay * pMilDisplay, CWnd *pHWindow)
{
	MdispSelectWindow(m_pMilDisplay->GetDisplayID(), m_milIDBuffer, pHWindow->GetSafeHwnd());
}

/*! @fn BOOL CMilBuffer::IsDispAllocated(void)
 *  @brief Mil Buffer와 Display buffer가 연결되어 있는지 확인할 수 있는 함수
 *  @return TRUE:할당되어 있음 FALSE:할당된 Display가 없음
 */
BOOL CMilBuffer::IsDispAllocated(void)
{
	if(m_pMilDisplay) return TRUE;
	else return FALSE;
}

/*! @fn int CMilBuffer::Close(void)
 *  @brief Mil Buffer를 해지함
 *  @return 현재 무조건 0으로 나옴
 */
int CMilBuffer::Close(void)
{
	if(m_milIDBuffer!=M_NULL)
	{
		m_milIDBuffCounter--;
		MbufFree(m_milIDBuffer);
		m_milIDBuffer=M_NULL;
	}
	return 0;
}

/*! @fn BOOL CMilBuffer::IsAlloced(void)
 *  @brief Mil Buffer가 할당되어 있는지 확인
 *  @return 1:할당되어 있음 0:할당되어 있지 않음
 */
BOOL CMilBuffer::IsAlloced(void)
{
	if(m_milIDBuffer)
		return 1;
	else 
		return 0;
}

/*! @fn void CMilBuffer::ConvertToBMPFmt(CMilBuffer *pBMPBuffer, MIL_INT64 BufferAttribute)
 *  @brief Mil Buffer을 BMP에서 사용할 수 있는 형태의 버퍼로 만들어줌
 *  @n 16bit->8bit로 Gray value를 leveling함
 *	@n buffer할당까지 이루어짐
 *  @param pBMPBuffer 변환된 버퍼가 저장될 Mil buffer(Alloc을 하지 않은 버퍼)
 *  @param BufferAttribute 변환될 버퍼의 Attribute
 */
void CMilBuffer::ConvertToBMPFmt(CMilBuffer *pBMPBuffer, MIL_INT64 BufferAttribute)
{
	tImageProperties imgProperty;
	long GrayMax, GrayMin;
	GetImageProperties(&imgProperty);
	CMilIm::GetMinMax(this, &GrayMin, &GrayMax);
	pBMPBuffer->SetImageProperties(&imgProperty);
	pBMPBuffer->SetDepth(8+M_UNSIGNED);
	pBMPBuffer->SetAttribute(BufferAttribute);
	pBMPBuffer->Alloc2D(imgProperty.ImageSize);
	WndLvlTranform(pBMPBuffer, GrayMin, GrayMax, 0, 255);
}

/*! @fn void CMilBuffer::ExportBMP(WCHAR * strFileName)
 *  @brief Mil Buffer의 내용을 BMP파일로 저장
 *  @n 16bit->8bit로 Gray value를 leveling함
 *  @param strFileName 저장할 파일이름
 */
void CMilBuffer::ExportBMP(CString strFileName)
{
	CMilBuffer newImage;
	ConvertToBMPFmt(&newImage);
	MbufExport(strFileName.GetBuffer(), M_BMP, newImage.GetIDBuffer());
	newImage.Close();
}

/*! @fn void CMilBuffer::ExportJPG(WCHAR * strFileName)
*  @brief Mil Buffer의 내용을 JPG파일로 저장->추가 키락 없이 처리하기 위함
*  @n 16bit->8bit로 Gray value를 leveling한 영상을 CImage를 이용하여 JPG로 저장
*  @param strFileName 저장할 파일이름
*/
void CMilBuffer::ExportJPG(CString strFileName)
{
	CImage tempImg;
	COLORREF color;

	CMilBuffer newImage;
	ConvertToBMPFmt(&newImage);

	tImageProperties ImageProperty;
	this->GetImageProperties(&ImageProperty);
	int cx = ImageProperty.ImageSize.cx;
	int cy = ImageProperty.ImageSize.cy;
	int x, y;
	tempImg.Create(cx, cy, 24);
	for (y = 0; y < cy; y++)
		for (x = 0; x < cx; x++)
		{
		this->GetPixelColor(x, y, &color);
		tempImg.SetPixel(x, y, color);

		}
	tempImg.Save(strFileName.GetBuffer());

	newImage.Close();
}

/*! @fn void CMilBuffer::Export(WCHAR * strFileName, int m_iFileFormat)
 *  @brief Mil Buffer을 각종형태의 파일로 저장 M_BMP, M_JPEG, M_TIFF
 *  @n BMP의 경우 16bit->8bit로 Gray value를 leveling함
 *  @param strFileName 저장할 파일이름
 *	@param m_iFileFormat 저장할 파일 포멧
 *	@n M_TIFF, M_BMP
 *	@todo jpeg의 경우 라이센스가 없어 동작할 수 없음. 라이센스는 10여만원이상하기 때문에 jpeg를 저장할 수 있는 모듈을 구해 구현해야함
 */
void CMilBuffer::Export(CString strFileName, int m_iFileFormat)
{
	switch(m_iFileFormat)
	{
	case M_BMP:
		ExportBMP(strFileName);
		break;
	case M_JPEG_LOSSY:
		MbufExport(strFileName, M_JPEG_LOSSY, m_milIDBuffer);
		break;
	default:
		MbufExport(strFileName, m_iFileFormat, m_milIDBuffer); 
		break;
	}

}


/*! @fn void CMilBuffer::SetFileFormat(MIL_INT iFileFormat)
 *  @brief Mil Buffer을 각종형태의 파일로 저장 M_BMP, M_JPEG, M_TIFF
 *	@param m_iFileFormat 저장할 파일 포멧
 *	@n M_BMP,M_JPEG_LOSSLESS, M_JPEG2000_LOSSLESS, M_JPEG_LOSSLESS_INTERLACED, M_JPEG2000_LOSSLESS_JP2
 *	@n M_JPEG_LOSSY,M_JPEG2000_LOSSY,M_JPEG_LOSSY_INTERLACED,M_JPEG2000_LOSSY_JP2,M_JPEG_LOSSY_RGB
 *	@n M_MIL+, M_RAW, M_TIFF+ 
 */
void CMilBuffer::SetFileFormat(MIL_INT iFileFormat)
{
	m_iFileFormat = iFileFormat;
}

CMilBuffer* CMilBuffer::operator=(CMilBuffer &  LValue)
{

	return NULL;
}

/*! @fn MIL_INT64 CMilBuffer::SetAttribute(MIL_INT64 BufferAttribute)
 *  @brief 할당할 Mil Buffer의 Atrribute를 설정한다
 *  @n 이미할당한것을 변경하는것이 아니다.
 *  @param BufferAttribute 설정할 속성값
 *  @n M_IMAGE, M_DISP, M_GRAB, M_PROC, M_DIB
 */
MIL_INT64 CMilBuffer::SetAttribute(MIL_INT64 BufferAttribute)
{
	return m_BufAttr  = BufferAttribute;
}

/*! @fn MIL_INT64 CMilBuffer::GetAttribute(void)
 *  @brief 할당할때 사용한 Attribute를 가져옴
 *  @n 할당된것을 확인하는것은 GetImageProperties사용
 *  @return 속성 값
 *  @n M_IMAGE, M_DISP, M_GRAB, M_PROC, M_DIB
 */
MIL_INT64 CMilBuffer::GetAttribute(void)
{
	return m_BufAttr;
}

/*! @fn void CMilBuffer::BufLoad(WCHAR * strFileName)
 *  @brief 이미 할당되어있는 버퍼에 파일을 읽어 넣는다
 *  @n 이미 크기가 할당되어 있는 버퍼를 사용해야 한다.
 *  @param strFileName 읽어들일 파일명
 */
void CMilBuffer::BufLoad(CString strFileName)
{
	return MbufLoad(strFileName.GetBuffer(), m_milIDBuffer);
}

/*! @fn int CMilBuffer::BufImport(WCHAR * strFileName, MIL_INT FileFormat, MIL_INT Operation)
 *  @brief 다용도로 로딩시킬수 있는 방법을 제공해준다.
 *  @n 포멧을 지정하여 로딩하며, 
 *  @param strFileName 읽어들일 파일명
 *  @param FileFormat 읽어들일 파일의 파일종류 지정해준다 
 *  @param Operation 읽어들이는 방법을 지정한다
 *  @n M_LOAD, M_RESTORE 버퍼를 할당할지 아니면 할당된 버퍼를 사용할지.
    *return M_RESOTRE일 경우에만 할당된 ID반환 할당실패의 경우 M_NULL반환
 */
int CMilBuffer::BufImport(CString strFileName, MIL_INT FileFormat, MIL_INT Operation)
{
	return MbufImport(strFileName.GetBuffer(), FileFormat, Operation, m_milIDSystem, &m_milIDBuffer);
}

/*! @fn void CMilBuffer::GetImageProperties(tImageProperties *pImageProperty)
 *  @brief mil buffer의 속성을 알려준다.
 *  @param pImageProperty tImageProperties구조체에 속성을 반납한다.
 */
void CMilBuffer::GetImageProperties(tImageProperties *pImageProperty)
{
	MIL_INT64 attr=M_NULL;
	long cx=0, cy=0, Depth=0, BandSize=0;
    cx = MbufInquire(m_milIDBuffer, M_SIZE_X, M_NULL);
	cy = MbufInquire(m_milIDBuffer, M_SIZE_Y, M_NULL);
	pImageProperty->Type = MbufInquire(m_milIDBuffer, M_TYPE, M_NULL);
	pImageProperty->Depth = MbufInquire(m_milIDBuffer, M_SIZE_BIT, M_NULL);
	pImageProperty->BandSize = MbufInquire(m_milIDBuffer, M_SIZE_BAND, M_NULL);
	MbufInquire(m_milIDBuffer, M_EXTENDED_ATTRIBUTE, &attr);
	pImageProperty->Attr = attr; 
	pImageProperty->Sign = MbufInquire(m_milIDBuffer, M_SIGN, M_NULL);
	pImageProperty->ImageSize=CSize(cx,cy);
}

/*! @fn int CMilBuffer::Alloc2D(CSize imageSize)
 *  @brief Mil Buffer를 할당한다
 *  @n 버퍼의 속성은 이전에 이미 설정되어 있어야한다(SetAttribute, SetDepth등)
 *  @param imageSize 할당할 버퍼의 크기를 지정해준다
 */
int CMilBuffer::Alloc2D(CSize imageSize)
{
	return MbufAlloc2d(m_milIDSystem, imageSize.cx, imageSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
}

/*! @fn ResizeBuf(CMilBuffer *pMilDest,CdblSize imageSize)
 *  @brief 이미지의 크기를 조절한다.
 *  @n 버퍼의 크기는 변경되지 않음
 *  @param pMilDest 조절된 이미지가 복사될 MilBuffer
 *  @param imageSize 이미지 크기비율
 *  @n 예)cx=1.5,cy=2이면 x축으로 1.5배 y축으로 2배로 만드는것임
 *	@return 0이 아니면 Error임
 */
void CMilBuffer::ResizeBuf(CMilBuffer *pMilDest,CdblSize imageSize)
{
	MimResize(m_milIDBuffer, pMilDest->m_milIDBuffer, imageSize.dblCX, imageSize.dblCY, M_DEFAULT);
}

/*! @fn void CMilBuffer::Put2D(CSize offsetPos, CSize sizeImage, void *pUserArrayPtr)
 *  @brief 사용자 배열인 pUserArrayPtr에 있는 데이터를 Mil Buffer로 복사한다
 *  @param offsetPos Copy시작위치
 *  @param sizeImage offsetPos위치부터 sizeImage크기지정
 *  @param pUserArrayPtr 이미지가 있는 배열의 위치
 */
void CMilBuffer::Put2D(CSize offsetPos, CSize sizeImage, void *pUserArrayPtr)
{
	MbufPut2d(m_milIDBuffer, offsetPos.cx, offsetPos.cy, sizeImage.cx, sizeImage.cy, pUserArrayPtr);
}

/*! @fn CMilBuffer * CMilBuffer::Child2D(CRect *pRect)
 *  @brief Child버퍼를 만든다.
 *  @n Child는 C의 포인터개념으로 Child에서 변경된것은 원본에도 영향을 미친다. 문론 원본을 수정하면 Child에도 영향을 미친다.
 *  @param pRect 원본에서 pRect위치를 Child로 만든다
 *  @return Child Buffer를 반환한다
 */
CMilBuffer * CMilBuffer::Child2D(CRect *pRect)
{
	return Child2D(pRect->left,pRect->top, pRect->Width(), pRect->Height());
}

/*! @fn CMilBuffer * CMilBuffer::Child2D(int offsetX, int offsetY, int childWidth, int childHeight)
 *  @brief OffsetX, OffsetY에서 ChildWidth폭, childHeight높이로 Child를 만든다
 */
CMilBuffer * CMilBuffer::Child2D(int offsetX, int offsetY, int childWidth, int childHeight)
{
	CMilBuffer *pBuffer=new CMilBuffer;
	pBuffer->m_pMilDisplay = m_pMilDisplay;
	pBuffer->m_iDepth = m_iDepth;
	pBuffer->m_BufAttr = m_BufAttr;
	pBuffer->m_milIDSystem = m_milIDSystem;
	pBuffer->m_milIDBuffer=MbufChild2d(m_milIDBuffer, offsetX, offsetY, childWidth, childHeight, M_NULL);
	return pBuffer;
}

/*! @fn int CMilBuffer::Create2D(CSize sizeImage,void * pAddr, int nDepth, int Attr)
 *  @brief sizeImage크기로 pAddr를 Host Address로 하는 버퍼를 생성한다.
 *  @param nDepth 생성할 버퍼 depth 
 *  @param Attr 생성할 버퍼의 Attribute 
 *  @return return 0이 아니면 Error임
 */
int CMilBuffer::Create2D(CSize sizeImage,void * pAddr, int nDepth, int Attr)
{
	return MbufCreate2d(m_milIDSystem, sizeImage.cx, sizeImage.cy, nDepth, Attr, M_HOST_ADDRESS, M_DEFAULT,  pAddr, &m_milIDBuffer);
}

/*! @fn void * CMilBuffer::GetAddr(void)
 *  @brief 버퍼의 메모리주소를 얻는다.
 *  @return 메모리버퍼의 주소를 받환한다 0이면 찾지 못함(Error)
 */
void * CMilBuffer::GetAddr(void)
{
	void * pAddr;
	MbufInquire(m_milIDBuffer, M_HOST_ADDRESS, &pAddr);
	return pAddr;
}

/*! @fn void CMilBuffer::SetDepth(int Depth)
 *  @brief 할당할 버퍼의 Depth설정
 *	@n 1,8,16,32 M_FLOAT, M_SIGNED, M_UNSIGNED
 */
void CMilBuffer::SetDepth(int Depth)
{
	m_iDepth=Depth;
}

/*! @fn CMilBuffer* CMilBuffer::DuplicateBuff(void)
 *  @brief 동일한 속성과 크기의 버퍼를 생성하여 반환
 *  @return 생성된 Mil buffer
 */
CMilBuffer* CMilBuffer::DuplicateBuff(void)
{
	tImageProperties ImageProperties;
	GetImageProperties(&ImageProperties);

	CMilBuffer * pMilBuffer = new CMilBuffer;
	pMilBuffer->m_pMilDisplay = m_pMilDisplay;
	pMilBuffer->m_iDepth = m_iDepth;
	pMilBuffer->m_BufAttr = ImageProperties.Attr;
	pMilBuffer->m_milIDSystem = m_milIDSystem;
	pMilBuffer->Alloc2D(ImageProperties.ImageSize);
	return pMilBuffer;
}

/*! @fn void CMilBuffer::Clear(MIL_DOUBLE ClearColor)
 *  @brief 버퍼를 Clear한다
 *  @n 초기생성때 쓰레기값등을 지울때 혹은 있는 이미지를 지울때
 *  @param ClearColor 화면을 지울때 색상(지워지고나서 모드 이색상으로 체워진다
 *  @M_RGB888(MIL_INT Red, MIL_INT Green, MIL_INT Blue), M_COLOR_BLACK, M_COLOR_RED... 
 */
void CMilBuffer::Clear(MIL_DOUBLE ClearColor)
{
	MbufClear(m_milIDBuffer, ClearColor);
}

/*! @fn int CMilBuffer::Copy(CMilBuffer* pMilSrc)
 *  @brief pMilSrc에서 이미지를 복사해온다
 */
int CMilBuffer::Copy(CMilBuffer* pMilSrc)
{
	pMilSrc->m_pMilDisplay = m_pMilDisplay;
	pMilSrc->m_iDepth = m_iDepth;
	pMilSrc->m_BufAttr = m_BufAttr;
	MbufCopy(pMilSrc->m_milIDBuffer, m_milIDBuffer);
	return 0;
}

/*! @fn int CMilBuffer::CopyClip(CMilBuffer* pMilClipBuffer, CPoint Offset)
 *  @brief pMilClipBuffer의 Offset위치에서 이미지를 읽어 복사해온다.
 */
int CMilBuffer::CopyClip(CMilBuffer* pMilClipBuffer, CPoint Offset)
{
	MbufCopyClip(pMilClipBuffer->m_milIDBuffer, m_milIDBuffer, Offset.x, Offset.y);

	return 0;
}

/*! @fn int CMilBuffer::Get2D(void * pUserArray, CRect rect)
 *  @brief Mil Buffer의 이미지를 사용자 배열인 pUserArray주소로 rect크기 이미지를 읽어온다
 *  @n Mil Buffer의 이미지를 다른 프로그램에서 사용하거나 Pixel단위로 조작하기 위해사용
 */
int CMilBuffer::Get2D(void * pUserArray, CRect rect)
{
	MbufGet2d(m_milIDBuffer, rect.left, rect.top, rect.Width(), rect.Height(), pUserArray);

	return 0;
}

/*! @fn int CMilBuffer::SetImageProperties(tImageProperties* pImageProperties)
 *  @brief Alloc을 할때 다른 ImagePorperties를 사용하여 설정함
 *  @n Alloc을 해야만 영향을 미침
 */
int CMilBuffer::SetImageProperties(tImageProperties* pImageProperties)
{
	m_iDepth = pImageProperties->Depth+pImageProperties->Sign;
	m_BufAttr = pImageProperties->Attr;
	return 0;
}

/*! @fn int CMilBuffer::SetDisplay(CMilDisplay* pMilDisplay)
 *  @brief Display버퍼를 할당함
 *  @n 연결을 하지 않고 있음
 */
int CMilBuffer::SetDisplay(CMilDisplay* pMilDisplay)
{
	m_pMilDisplay = pMilDisplay;
	return 0;
}

/*! @fn CMilDisplay* CMilBuffer::GetDisplay()
 *  @brief 할당된 Display의 주소를 얻는다.
 *  @return CMilDisplay의 주소가 반환된다.
 */
CMilDisplay* CMilBuffer::GetDisplay()
{
	return m_pMilDisplay;
}

/*! @fn int CMilBuffer::GetIDBuffer(void)
 *  @brief 버퍼의 Mil ID를 받환한다.
 *  @return Mil Buffer의 Mil ID
 */
int CMilBuffer::GetIDBuffer(void)
{
	return m_milIDBuffer;
}

void CMilBuffer::SetIDBuffer(int milID)
{
	m_milIDBuffer=milID;
}

/*! @fn void CMilBuffer::CopyColor2D(CMilBuffer *pBufDest, CRect rectCopy, CPoint ofsDest, int bandDest, int bandSrc)
 *  @brief 버퍼대 버퍼로 복사한다
 *  @param pBufDest 복사되는 버퍼의 CMilBuffer포인트
 *  @param rectCopy 원본에서의 복사할 대상위치및 크기
 *  @param ofsDest 복사되는 버퍼의 복사 시작위치
 *  @param bandDest 복사되는 버퍼의 Band위치
 *  @param bandSrc  원본의 Band 위치 Default=M_ALL_BANDS
 */
void CMilBuffer::CopyColor2D(CMilBuffer *pBufDest, CRect rectCopy, CPoint ofsDest, int bandDest, int bandSrc)
{
	MbufCopyColor2d(m_milIDBuffer, pBufDest->GetIDBuffer(),  bandSrc, rectCopy.left, rectCopy.top, bandDest, ofsDest.x, ofsDest.y, rectCopy.Width(), rectCopy.Height());
}

//void CMilBuffer::CopyColor2D(CMilBuffer *pBufDest, CPoint ofsSrc, CSize sizeImg, CPoint ofsDest, int bandDest, int bandSrc)
//{
//	MbufCopyColor2d(m_milIDBuffer, pBufDest->GetIDBuffer(),  bandSrc, ofsSrc.x, ofsSrc.y, bandDest, ofsDest.x, ofsDest.y, sizeImg.cx, sizeImg.cy);
//}

/*! @fn int CMilBuffer::GetDepth(void)
 *  @brief 할당할 때 사용한 Depth반환
 *  @n 현재값을 정확히 알고자 할때는 GetImageProperties사용
 *  @return 할당할 때 사용한 Depth
 */
int CMilBuffer::GetDepth(void)
{
	return m_iDepth;
}

/*! @fn int CMilBuffer::WndLvlTranform(CMilBuffer *pBufDest, int OrgStart, int OrgEnd, int DestStart, int DestEnd)
 *  @brief Gray Value Windowing처리를 한다.
 *	@param pBufDest 처리된 결과가 들어갈 버퍼
 *	@param OrgStart 원본의 시작 Gray value
 *	@param OrgEnd 원본의 끝 Gray value
 *	@param DestStart 처리된 결과의 시작 Gray value
 *	@param DestEnd 처리된 결과의 최대 Gray value
 */
int CMilBuffer::WndLvlTranform(CMilBuffer *pBufDest, int OrgStart, int OrgEnd, int DestStart, int DestEnd)
{
	MIL_ID ImgLUT;
	double dblOrgStart=OrgStart, dblOrgEnd=OrgEnd;
	long sizeTable=2 << (GetDepth()-1);
	MIL_DOUBLE dblDestStart=DestStart;
	int MaxValue=CMilIm::GetMax(this);
	int LimitEnd=(OrgEnd<MaxValue)?OrgEnd:MaxValue;
	MbufAlloc1d(CMilSystem::Instance()->GetSystemID(), sizeTable, GetDepth()+M_UNSIGNED, M_LUT, &ImgLUT);
	MgenLutRamp(ImgLUT, 0, 0, OrgStart,0);
	MgenLutRamp(ImgLUT, OrgStart, DestStart, OrgEnd,DestEnd);
	MgenLutRamp(ImgLUT, LimitEnd, DestEnd, MaxValue, DestEnd);
	//if(MaxValue>4095)TRACE(_T("MaxError"));
	MimLutMap(GetIDBuffer(), pBufDest->GetIDBuffer(), ImgLUT);
	MbufFree(ImgLUT);
	return 0;
}

int CMilBuffer::GetPixelGray(int x, int y, int * vGray)
{
	Get2D(vGray, CRect(x,y, x+1, y+1));
	return 0;
}

int CMilBuffer::GetPixelGray(CPoint posPixel, int * vGray)
{
	Get2D(vGray, CRect(posPixel.x,posPixel.y, posPixel.x+1, posPixel.y+1));
	return 0;
}

int CMilBuffer::GetPixelColor(int x, int y, COLORREF * vColor)
{
	MbufGetColor2d(m_milIDBuffer, M_PACKED + M_BGR24, M_ALL_BANDS, x, y, 1, 1, (void *)vColor);
	return 0;
}

int CMilBuffer::GetPixelColor(CPoint posPixel, COLORREF * vColor)
{
	MbufGetColor2d(m_milIDBuffer, M_PACKED + M_BGR24, M_ALL_BANDS, posPixel.x, posPixel.y, 1, 1, (void *)vColor);
	return 0;
}

int CMilBuffer::LoadRawFile(CString strFileName, CSize rawSize)
{
	CFile rawFile;
	CFileException e;
	int sizeFile;
	if(!rawFile.Open(strFileName, CFile::modeRead, &e))return -1;

	sizeFile=(int)rawFile.SeekToEnd();
	rawFile.SeekToBegin();
	

	BYTE *pRawData= new BYTE[rawSize.cx * rawSize.cy *(m_iDepth/8)];
	rawFile.Read(pRawData, sizeFile);

	
	//MbufAlloc2d(m_milIDSystem, rawSize.cx, rawSize.cy, m_iDepth, m_BufAttr, &m_milIDBuffer);
	//MbufPut2d(m_milIDBuffer, 0, 0, rawSize.cx, rawSize.cy, pRawData);
	//delete pRawData;

	MbufCreate2d((MIL_ID)m_milIDSystem, rawSize.cx, rawSize.cy, m_iDepth+M_UNSIGNED, m_BufAttr, M_HOST_ADDRESS, M_DEFAULT, pRawData, &m_milIDBuffer);
  
	return 0;
}
