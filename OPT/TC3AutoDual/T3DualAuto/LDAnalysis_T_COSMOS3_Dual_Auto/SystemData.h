#include "MotionDefine.h"
#include "IniPara.h"  


#pragma once

class  CSystemData
{
	//Contruction
public:
	CSystemData();
	//Attributes
protected:
	CIni	m_Ini;
public:

	//Operations
public:
	void MakeFullFileName(CString& strFileName);
	BOOL CopyFolder(CString& strFrom, CString& strTo);
	int ExCopyFile(CString strFrom, CString strTo);
	void FileDelete(CString csGroup ,CString csFolder , CString csFileName);

	//Implementation
public:
	virtual ~CSystemData();

protected:

public:
	CFileFind	finder;
	
	 
private:
public:
	void GetPara(CString csFoldername , CString  csFileName, CString csSection, CString csEntry , int nType, BOOL *bPara, int *nPara, double *dPara);
	void SetPara(CString csFoldername , CString  csFileName, CString csSection, CString csEntry , int nType, BOOL bPara = FALSE, int nPara = 0, double dPara = 0.0);
	void GetStringPara(CString csFoldername , CString  csFileName, CString csSection,CString csEntry , CString *csIO);
};
