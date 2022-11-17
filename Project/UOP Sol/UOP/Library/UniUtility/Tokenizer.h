#pragma once
#include "UniUtilBase.h"

#define NORMAL_SEP	0x0000
#define GROUP_SEP	0x0001
#define MERGE_SEP	0x0002
#define SKIP_TOKEN	0x0004

struct TOKEN_INFO
{
	CString delimiter;
	CString finder;
	int type;
};

// Deprecate
class DLLDECL CTokenizer
{
	CString original;
	CString tokenStr;
	CString trimChar;
	bool	moreTokenExist;

	//! 하나의 토큰 표현식으로부터 TOKEN_INFO에 필요한 정보를 얻어 온다.
	/*!
	토큰 표현식은 토큰을 구분을 보다 유연하게하기 위해 정의한 문자열이다. 
	토큰 표현식은 다음과 같은 형태를 가진다. 

		<i> [wild card string] ; [parsing function]</i>
		[wild card string] := literal include * and/or ?(*)
		[parsing function] := p([delimiter]) | g([delimiter]) 
		*;p(x)	N*;g(y)

		wild card string내에 wild card 문자(*,?)이외의 문자가 존재하면
		파싱시 토큰의 형태를 구분하여 토큰의 유효성 여부를 검사하고,
		반환되는 토큰은 해당 문자를 제거한 뒤 반환된다.

		p - normal seperator. 괄호내의 문자중 일부만 일치하면 구분함.
		g - group seperator. 괄호내의 문자가 모두 일치해야 함.
		s - skip token. 설정된 구분자로 토큰을 얻지만 반환하지 않고 무시한다.

		delimiter로 /를 사용할 수 없음. /를 사용해야 할 경우 \\를 사용.
		특수 기호 : \\ = \t , \\ = \\
	\param tokInfo [out] 파싱 정보를 토큰 표현식으로 부터 얻어 온다.
	\param tokInfoStr 토큰 표현식 
	\return tokInfoStr이 정상적이면 TRUE를 반환한다. 단 공백은 정상 표현으로 간주한다.
	*/
	BOOL GetTokenInfo(TOKEN_INFO& tokInfo, CString tokInfoStr);

	//! TOKEN_INFO를 가지고 토큰을 하나 얻어 온다.
	BOOL GetNextToken(CString& token, TOKEN_INFO& tokInfo);

public:
	//! 기본 생성자
	CTokenizer();
	//! 전달된 문자열을 가지고 토큰생성기를 초기화 한다. 
	CTokenizer(LPCTSTR str);
	
	//! 토큰 갯수를 반환한다.
	int GetTokenCount(LPCTSTR delimiter);

	//! 토큰을 반환한다.
	/*!
	이 함수는 사용은 간편하나 Token이 존재하지 않을 경우 빈 문자열이 반환되므로 
	Token의 존재 유무를 확인하는데 유연하지 못하다. Token유무를 확인하기 위해서는 
	반드시 IsTokenExist를 먼저 사용해야 한다. 따라서 다른 형태의 GetNextToken을 
	사용하는 것이 바람직하다.
	\deprecated 상술되었음.
	\param delimiter 구분 문자
	\param offset nth token. 반드시 0보다 커야 한다.
	*/
	CString GetNextToken(LPCTSTR, int offset = 1);
	//! 토큰 존재 여부 결정
	/*!
	\deprecated GetNextToken참조
	*/
	BOOL IsTokenExist ( void );

	//! 토큰을 반환한다.
	/*!
	<code>
		CToken tokenizer = "1;2;3;4";
		CString tokenStr;
		while(tokenizer.GetNextToken(tokenStr,";")) {
			cout << tokenStr << "\n";
		}
	</code>
	\param tokenStr [out] 토큰을 받을 변수.
	\param delimiter 구분 문자
	\param offset nth token. 반드시 0보다 커야 한다. 기본값은 1이다.
	\param type 토큰 생성 방법을 설정한다. GROUP_SEP 또는 MERGE_SEP을 설정할 수 있다. 기본값은 NORMAL_SEP이다.
	\return 토큰 생성이 정상적으로 수행되었으면 TRUE를 반환한다. 
	*/
	BOOL GetNextToken(CString& tokenStr, LPCTSTR delimiter, int offset = 1, int type = NORMAL_SEP);

	//! 토큰배열을 반환한다.
	/*!
	전달된 구분 문자를 이용하여 추출된 토큰을 배열에 저장한다. 
	\param tokenList [out] 토큰을 받을 배열 변수.
	\param delimiter 구분 문자
	\return 토큰 생성이 정상적으로 수행되었으면 TRUE를 반환한다. 항상, TRUE를 반환.
	*/
	BOOL GetTokenList(CStringArray& tokenList, LPCTSTR delimiter);

	//! 토큰배열을 반환한다.
	/*!
	전달된 토큰 표현식을 이용하여 토큰 생성기내의 문자열을 일괄적으로 배열에 저장한다. 
	\param tokenList [out] 토큰을 받을 배열 변수.
	\param tokExp 토큰 표현식 
	\return 토큰 생성이 정상적으로 수행되었으면 TRUE를 반환한다. 
	*/
	BOOL GetTokenListEx(CStringArray& tokenList, LPCTSTR tokExp);

	LPCTSTR PeekNextToken(TCHAR * delimiter);
	CString GetStringToken(TCHAR * delimiter);

	CString operator= ( LPCTSTR str );
	operator LPCTSTR() const;

	CString GetString ( void );
	
	//! 잘라내기 문자열 설정
	/*!
	\deprecated 이름 변경 SetTrimChar를 사용.
	*/
	void TrimChar ( LPCTSTR trimChar );
	//! 잘라내기 문자열 설정
	void SetTrimChar(LPCTSTR trimChar);

	void RemoveRemark( LPCTSTR startRemark );
};

class CStringSupport
{
public:
	static CString MakeShortString(CDC* pDC, LPCTSTR lpszLong, int width);
	static int WildCmp(LPCTSTR wild, LPCTSTR string);
	static CString N2C(long n);
	static CString N2C(int n);
	static CString N2C(double f, LPCTSTR format = _T("%.3f"));
	static BOOL GetGuid(CString& rString);
	static CString GetResString(UINT resID, HINSTANCE hModule = 0);
};

// New Name
#define CToken CTokenizer
