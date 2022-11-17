#pragma once
#include "ModelBase.h"
#include "Frame.h"
#include "Broadcaster.h"
#include "Module.h"
// CModelData 명령 대상입니다.

class CModelData : public CBroadcaster
{
	DECLARE_DYNAMIC(CModelData)
public:
	CModelData();
	virtual ~CModelData();
	int AddElement(CModelBase* pElement);
	CFrame* FindElmtWindow(CdblPoint dblPosition);
	int ReturnDIR(void);
	int SaveModelData(CString strFileName = _T(""));
	int SaveModelDataTemp(void);
	int LoadModelData(CString strFileName=_T(""));
	int LoadModelDataTemp(void);
	CString m_strModelPath;
	void SetModelPath(TCHAR * szModelPath);
	CXmlBrowser m_ModelFile;
	int SaveModelInfo(void);
	int SaveModelInfoforTemp(void);
	int Create(void);
	CString GetModelPath(void);
	BOOL IsExist(CModelBase* pElement);
	BOOL IsExistKindOf(CRuntimeClass* pElementType);
	int m_nSeqNum;
	double m_dblLeftPos;
	double m_dblRightPos;
	double m_dblTopPos;
	double m_dblBottomPos;
	double m_dblXrayPIPZ;

	TCHAR *m_chUpdateTime;

	int LoadModelInfo(void);
	int LoadModelInfoTemp(void);
	static CModelBase * GetModelClass(TCHAR * szModelName);
	void RemoveAll(void);
	CPtrList * GetList(void);
	// /The name of ModelData 
	CString m_strMDName;
	void SetMDName(TCHAR * szMDName);
	CString GetMDName(void);
	int AddElementHead(CModelBase* pElement);
	char* timeToString(struct tm *t);
//private:
	CPtrList m_ListElement;

public:

	CModule m_ModuleCalss;
   
	int SaveAttr(CXmlNode * pNode);
	int GetOLTTImg(double posx, double posy, double posz);
	CdblSize GetModelScaleFactor();
	int GetOLTTImg(int iFrameNumber);
	BOOL IsEmpty(void);
	CString strClip(CString str, int nType);
	int MakeModelFolder();
};


