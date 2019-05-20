// LogUnit.h: interface for the CLogUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGUNIT_H__D7903C35_728F_4982_A8A1_285CB59E668A__INCLUDED_)
#define AFX_LOGUNIT_H__D7903C35_728F_4982_A8A1_285CB59E668A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogUnit  
{
public:
	CLogUnit();
	virtual ~CLogUnit();

	void Trace(CString sMsg);
	void Trace(char * sMsg);

};

extern CLogUnit Log ;

#endif // !defined(AFX_LOGUNIT_H__D7903C35_728F_4982_A8A1_285CB59E668A__INCLUDED_)
