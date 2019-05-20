#include "stdafx.h"
#include "SystemData.h"
#include "MotionDefine.h"
#include "MainFrm.h"



CSystemData::CSystemData()
{

}

CSystemData::~CSystemData()
{

}

void CSystemData::MakeFullFileName(CString &strFileName)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	int nPos;

	_splitpath(strFileName, drive, dir, fname, ext);
	if(!drive[0])
	{
		CString strModule;
		GetModuleFileName(NULL,strModule.GetBuffer(MAX_INI_BUFFER),MAX_INI_BUFFER);
		strModule.ReleaseBuffer();
		strModule.ReleaseBuffer();
		_splitpath( strModule, drive, dir, fname, ext );
		strModule = drive;
		strModule += dir;

		strModule.MakeUpper();
		nPos = strModule.Find("DEBUG");
		if (nPos >= 0)
		{
			strModule = strModule.Left(nPos);
		}

		strModule += strFileName;
		strFileName = strModule;
	}
}

int CSystemData::ExCopyFile(CString strFrom, CString strTo)
{
// 	SHFILEOPSTRUCT shfo;
// 
// 	char pszFrom[1024] = {0};
// 	char pszTo[1024] = {0};
// 
// 	WORD wFlag = 0;
// 
// 	strcpy(pszFrom, (LPCTSTR)strFrom);
// 	strcpy(pszTo, (LPCTSTR)strTo);
// 
// 	pszFrom[lstrlen(pszFrom) + 1] = 0;
// 
// 	// shfo.hwnd = m_hWnd;
// 	shfo.wFunc = FO_COPY;
// 	shfo.pFrom = pszFrom;
// 	shfo.pTo = pszTo;
// 	shfo.fFlags = 0;
// 	shfo.lpszProgressTitle = "File Copy..";
// 
// 	return SHFileOperation(&shfo);
	return 0;
}

BOOL CSystemData::CopyFolder(CString& strFrom, CString& strTo)
{
// 	CFileFind finder;
// 
// 	CString sWildCard = strFrom;// + "\\*.*";
// 
// 	BOOL bFound;
// 	BOOL bWorking = finder.FindFile(sWildCard);
// 
// 	bFound = bWorking;
// 
// 	::CreateDirectory(strTo, NULL);
// 
// 	while(bWorking)
// 	{
// 		bWorking = finder.FindNextFile();
// 
// 		// skip . and .. files; otherwise, we"d
// 		// recur infinitely!
// 		if(finder.IsDots()) continue;
// 
// 		// 디렉토리이면 재귀 호출
// 		if(finder.IsDirectory())
// 		{
// 			CString sFrom2 = finder.GetFilePath();
// 			CString sTo2 = strTo + "\\" + finder.GetFileName();
// 			::CreateDirectory(sTo2, NULL);
// 
// 			CopyFolder(sFrom2, sTo2);
// 			continue;
// 		}
// 		ExCopyFile(finder.GetFilePath(), strTo);
// 	}
// 	return bFound;
	return false;
}

void CSystemData::FileDelete(CString csGroup ,CString csFolder ,CString csFileName)
{
	CString csFile;

// 	MakeFullFileName(csFile);	
// 	csFile = csFile + FOLDER_RECIPE + csGroup + "\\"  + csFolder + "\\" + csFileName + DATFILE;	
// 	remove(csFile);
}


void CSystemData::GetPara(CString csFoldername , CString  csFileName, CString csSection,CString csEntry , int nType, BOOL *bPara, int *nPara, double *dPara)
{
	CString	csFilenameRead;
	CString	csFile;	
	MakeFullFileName(csFilenameRead);

	csFilenameRead = csFilenameRead + "Setting\\" + csFoldername + "\\"   ;
	csFile += csFilenameRead + csFileName;

	if(nType == PARA_TYPE_BOOL)
	{
		m_Ini.SetFileName(csFile);
		m_Ini.SetSection(csSection);
		*bPara = m_Ini.GetBool(csEntry);
	}
	else if(nType == PARA_TYPE_INT)
	{
		m_Ini.SetFileName(csFile);
		m_Ini.SetSection(csSection);
		*nPara = m_Ini.GetInt(csEntry);
	}
	else if(nType == PARA_TYPE_DOUBLE)
	{
		m_Ini.SetFileName(csFile);
		m_Ini.SetSection(csSection);
		*dPara = m_Ini.GetDouble(csEntry);
	} 
}	

void CSystemData::SetPara(CString csFoldername , CString  csFileName, CString csSection,CString csEntry , int nType, BOOL bPara, int nPara, double dPara)
{
	CString	csFilenameRead;
	CString	csFile;	
	MakeFullFileName(csFilenameRead);

	csFilenameRead = csFilenameRead + "Setting\\" + csFoldername + "\\"   ;
	csFile += csFilenameRead + csFileName;

	if(nType == PARA_TYPE_BOOL)
	{
		m_Ini.SetFileName(csFile);
		m_Ini.SetSection(csSection);
		m_Ini.WriteBool(csEntry, bPara);
	}
	else if(nType == PARA_TYPE_INT)
	{
		m_Ini.SetFileName(csFile);
		m_Ini.SetSection(csSection);
		m_Ini.WriteInt(csEntry, nPara);
	}
	else if(nType == PARA_TYPE_DOUBLE)
	{
		m_Ini.SetFileName(csFile);
		m_Ini.SetSection(csSection);
		m_Ini.WriteDouble(csEntry, dPara);
	}		
}
void CSystemData::GetStringPara(CString csFoldername , CString  csFileName, CString csSection,CString csEntry , CString *csIO)
{
	CString	csFilenameRead;
	CString	csFile;	
	MakeFullFileName(csFilenameRead);

	csFilenameRead = csFilenameRead + "Setting\\" + csFoldername + "\\"   ;
	csFile += csFilenameRead + csFileName;

	m_Ini.SetFileName(csFile);
	m_Ini.SetSection(csSection);
	*csIO = m_Ini.GetString(csEntry, "");	
}	
