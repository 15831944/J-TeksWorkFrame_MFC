#include "stdafx.h"
#include "Tokenizer.h"

typedef CArray<TOKEN_INFO, TOKEN_INFO&> TTokenInfoArray;

CString Trim(CString& str, LPCTSTR trimChar) 
{
	str.TrimLeft(trimChar);
	str.TrimRight(trimChar);

	return str;
}

////////////////////////////////////////////////////////////////////////////////////
// Tokenizer 클래스

CTokenizer::CTokenizer ( void )
{
	tokenStr = _T("");
	original = _T("");
	trimChar = _T("\n\t\r ");
	moreTokenExist = false;
}

CTokenizer::CTokenizer ( LPCTSTR str )
{
	tokenStr = _T("");
	trimChar = _T("\n\t ");
	original = str;
	Trim(original, trimChar);

	if ( original != _T("") ) 
		moreTokenExist = true;
	else 
		moreTokenExist = false;
}

void CTokenizer::TrimChar ( LPCTSTR pTrimChar )
{
	trimChar = pTrimChar;
}

void CTokenizer::SetTrimChar(LPCTSTR pTrimChar)
{
	trimChar = pTrimChar;
}

int CTokenizer::GetTokenCount(LPCTSTR delimiter)
{
	int count = 0;
	CTokenizer t(original);
	while (t.IsTokenExist()) {
		t.GetNextToken(_T(" "));
		count ++;
	}

	return count;
}

CString CTokenizer::GetNextToken(LPCTSTR delimiter, int offset)
{
	ASSERT(offset > 0);

	int pos;

	// 더 이상의 토큰이 없을 경우 
	if ( !moreTokenExist )
		return _T("");

	original = Trim(original, trimChar);

	do { 
		pos = original.FindOneOf(delimiter);

		if ( pos == -1 ) { 
			tokenStr = Trim(original, trimChar);
			
			moreTokenExist = false;

			return tokenStr;
		} else {
			tokenStr = Trim(original.Left(pos), trimChar);
			original = Trim(original.Mid(pos + 1), trimChar);
		}
		offset--;
	} while ( offset > 0 );

	return tokenStr;
}

BOOL CTokenizer::GetNextToken(CString& tokenStr, LPCTSTR delimiter, int offset, int type)
{
	if (original == _T(""))
		return FALSE;

	int pos, skipLen;

	original = Trim(original, trimChar);

	do { 
		if (type & GROUP_SEP) {
			pos = original.Find(delimiter);
			skipLen = (int)wcslen(delimiter);
		} else {
			pos = original.FindOneOf(delimiter);
			skipLen = 1;
		}

		if ( pos == -1 ) { 
			tokenStr = Trim(original, trimChar);
			original = _T("");
			
			moreTokenExist = false;

			return TRUE;
		} else {
			tokenStr = Trim(original.Left(pos), trimChar);
			original = Trim(original.Mid(pos + skipLen), trimChar);
			if ( type & MERGE_SEP) {
				// Group Seperation은 Merge 기능을 제공할 수 없다.
				ASSERT (!(type & GROUP_SEP));

				CString temp = delimiter;
				TCHAR ch[1];
				ch[0] = original[0];
				while(temp.FindOneOf(ch) > -1) {
					original = original.Mid(1);
				}
			}
		}
		offset--;
	} while ( offset > 0 );

	return TRUE;
}

BOOL CTokenizer::GetTokenList(CStringArray& tokenList, LPCTSTR delimiter)
{
	CString token;
	while (GetNextToken(token, delimiter)) {
		tokenList.Add(token);
	}

	return TRUE;
}

BOOL CTokenizer::GetTokenInfo(TOKEN_INFO& tokInfo, CString tokInfoStr)
{
	int sepPos = tokInfoStr.FindOneOf(_T(";"));
	CString finderStr;
	CString delimiter;

	if ( sepPos == -1 ) { 
		tokInfo.finder = Trim(tokInfoStr, trimChar);
		tokInfo.delimiter = "";
		tokInfo.type = NORMAL_SEP;
	} else {
		tokInfo.finder = Trim(tokInfoStr.Left(sepPos), trimChar);
		tokInfo.type = NORMAL_SEP;
		CString temp = _T("");
		if (sepPos+1 < tokInfoStr.GetLength()) 
			temp = Trim(tokInfoStr.Mid(sepPos + 1), trimChar);

		if (temp.Right(1) != ")") {
			return TRUE;
		}

		// Check Type
		int index = 0;
		TCHAR ch = temp[index];
		while (ch != '(') {

			switch(ch) {
			case 'p':
				tokInfo.type |= NORMAL_SEP;
				break;
			case 'g':
				tokInfo.type |= GROUP_SEP;
				break;
			case 's':
				tokInfo.type |= SKIP_TOKEN;
				break;
			}

			index++;
			if (index > temp.GetLength())
				return FALSE;

			ch = temp[index];
		}

		CString delimiter = temp.Mid(index+1, temp.GetLength()-(index+2));
		int pos;
		char replaceCh;
		while ((pos=delimiter.Find(_T("\\")))>-1) {
			if (delimiter.GetLength() > pos+1) {
				switch (delimiter[pos+1]) {
				case 't':
					replaceCh = '\t';
					break;
				case '\\':
					replaceCh = '\\';
					break;
				case 's':
					replaceCh = '/';
				}
				
				delimiter = delimiter.Left(pos) + replaceCh + delimiter.Mid(pos+2);
			}
		}
		tokInfo.delimiter = delimiter;
	}

	return TRUE;
}

BOOL CTokenizer::GetTokenListEx(CStringArray& tokenList, LPCTSTR tokExp)
{
	TOKEN_INFO tokInfo;
	TTokenInfoArray tokInfoArray;

	CTokenizer tokenizer(tokExp);
	CString oneTokExp;
	while (tokenizer.GetNextToken(oneTokExp, _T("/"))) {
		if (GetTokenInfo(tokInfo, oneTokExp)) {
			tokInfoArray.Add(tokInfo);
		} else {
			// 토큰 표현식을 분석하는 중 오류가 발생하면 더이상의 작업을 수행하지 않고 
			// 작업을 중단한다.
			break;
		}
	}

	if (tokInfoArray.GetSize() == 0) {
		// TokenInfo가 하나도 생성되지 않았으면 
		// 기본 토큰 정보를 하나 생성한다. 
		GetTokenInfo(tokInfo,_T("*"));
		tokInfoArray.Add(tokInfo);
	}

	int index = 0;
	CString token;
	tokInfo = tokInfoArray[index];
	while (GetNextToken(token, tokInfo)) {
		if (!(tokInfo.type & SKIP_TOKEN))
			tokenList.Add(token);
		if (index < tokInfoArray.GetUpperBound()) {
			index++;
		}
		tokInfo = tokInfoArray[index];
	}

	return TRUE;
}

BOOL CTokenizer::GetNextToken(CString& token, TOKEN_INFO& tokInfo)
{
	token = "";
	CString srcStr;
	if (GetNextToken(srcStr, tokInfo.delimiter,1,tokInfo.type)) {
		if (CStringSupport::WildCmp(tokInfo.finder, srcStr)) {
			CString destStr;
			int pos = 0;
			for (int i=0; i<srcStr.GetLength(); i++) {
				if (tokInfo.finder[pos] == '*') {
					if (tokInfo.finder.GetLength() != (pos+1)) {
						if (tokInfo.finder[pos+1] == srcStr[i+1]) {
							pos++;
						}
					}
					token += srcStr[i];
				} else if (tokInfo.finder[pos] == '?') {
					pos++;
					token += srcStr[i];
				} else if (tokInfo.finder[pos] == srcStr[i]) {
					pos++;
				} else {
					return FALSE;
				}
			}
		} else {
			return FALSE;
		}
	} else {
		return FALSE;
	}

	return TRUE;
}

LPCTSTR CTokenizer::PeekNextToken ( TCHAR * delimiter )
{
	int pos;

	// 더 이상의 토큰이 없을 경우 
	if ( !moreTokenExist )
		return _T("");

	Trim(original, trimChar);

	pos = original.FindOneOf ( delimiter );

	if ( pos == -1 ) { 
		tokenStr = original;
		Trim(tokenStr, trimChar);
		
		moreTokenExist = false;

		return tokenStr;
	} else {
		tokenStr = original.Left ( pos );
		Trim(tokenStr, trimChar);
	}

	return tokenStr;
}

CString CTokenizer::operator= ( LPCTSTR str )
{
	original = str;
	
	Trim(original, trimChar);

	if ( original != "" ) 
		moreTokenExist = true;
	else 
		moreTokenExist = false;

	return original;
}

CTokenizer::operator LPCTSTR() const
{
	return original; 
}

CString CTokenizer::GetString ( void )
{
	return original;
}

void CTokenizer::RemoveRemark( LPCTSTR startRemark )
{
	if ( moreTokenExist ) {
		int pos;
		if ( ( pos = original.Find ( startRemark ) ) != -1 ) {
			original = original.Left ( pos );
			if ( original == "" ) 
				moreTokenExist = false;
		}
	}
}

BOOL CTokenizer::IsTokenExist ( void )
{
	return moreTokenExist;
}

CString CTokenizer::GetStringToken ( TCHAR * delimiter ) 
{
	// 더 이상의 토큰이 없을 경우 
	if ( tokenStr == original )
		return _T("");

	Trim(original, trimChar);

	if ( original[0] == '\"' ) {
		original = original.Mid(1);
		int pos = original.FindOneOf ( _T("\"") );
		if ( pos == -1 ) { 
			tokenStr = original.Mid(1);
		} else {
			tokenStr = original.Mid ( 0 , pos );
			original = original.Mid ( pos+1 );
		}
		Trim(tokenStr, trimChar);

		return tokenStr;
	} else {
		return GetNextToken ( delimiter );
	}
}

CString CStringSupport::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int width)
{
	static const _TCHAR szThreeDots[]=_T("...");
	CString convString = lpszLong;

	int nStringLen=lstrlen(lpszLong);
	int pixelWidth = pDC->GetTextExtent(lpszLong,nStringLen).cx;
	if(nStringLen==0 || pixelWidth < width+1)
		return lpszLong;

	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	CString tmpString;
	int start = 0 , end = nStringLen;
	int half = nStringLen / 2;
	while ((start != end) && half ) {
		tmpString = convString.Left(start+half);
		if(pDC->GetTextExtent(tmpString,half).cx + nAddLen < width) {
			start += half;
		} else {
			end -= half;
		}
		half /= 2;
	}

	return convString.Left(start) + szThreeDots;
}

int CStringSupport::WildCmp(LPCTSTR wild, LPCTSTR string) {
	LPCTSTR cp=NULL;
	LPCTSTR mp=NULL;
	
	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}
		
	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}
		
	while (*wild == '*') {
		wild++;
	}
	return !*wild;
}

CString CStringSupport::N2C(long n) 
{
	CString str;
	str.Format(_T("%d"), n);

	return str;
}

CString CStringSupport::N2C(int n) 
{
	CString str;
	str.Format(_T("%d"), n);

	return str;
}

CString CStringSupport::N2C(double f, LPCTSTR format) 
{
	CString str;
	str.Format(format, f);

	return str;
}

CString CStringSupport::GetResString(UINT resID, HINSTANCE hModule)
{
	CString text;
	if (hModule != 0) {

		HINSTANCE hInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle(hModule);

		text.LoadString(resID);

		AfxSetResourceHandle(hInstResource);

	} else {
		text.LoadString(resID);
	}

	return text;
}

BOOL CStringSupport::GetGuid(CString& rString)
{
	CString strFormat = _T("%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X");

	GUID guidValue = GUID_NULL;
	::CoCreateGuid(&guidValue);
	if (guidValue == GUID_NULL) {
		return FALSE;
	}
 
	// then format into destination
	rString.Format(strFormat,
		// first copy...
		guidValue.Data1, guidValue.Data2, guidValue.Data3,
		guidValue.Data4[0], guidValue.Data4[1], guidValue.Data4[2], guidValue.Data4[3],
		guidValue.Data4[4], guidValue.Data4[5], guidValue.Data4[6], guidValue.Data4[7],
		// second copy...
		guidValue.Data1, guidValue.Data2, guidValue.Data3,
		guidValue.Data4[0], guidValue.Data4[1], guidValue.Data4[2], guidValue.Data4[3],
		guidValue.Data4[4], guidValue.Data4[5], guidValue.Data4[6], guidValue.Data4[7]);

	return TRUE;
}
