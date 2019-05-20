// LogUnit.cpp: implementation of the CLogUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvsystem.h"
#include "LogUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogUnit::CLogUnit()
{

}

CLogUnit::~CLogUnit()
{

}

void CLogUnit::Trace(CString sMsg)
{
//20101216 lwb 참조 되지 않은 변수는 주석처리
//	int i,j;
	SYSTEMTIME nSysTime;
	int count=0; 
    char buf[256]="\0";
 
    FILE *fp;
	char fname[256];
	int index=0;

	::GetLocalTime(&nSysTime);

	::CreateDirectory("d:\\LOG",NULL);	
	sprintf(fname,"d:\\LOG\\%d-%d-%d.txt",nSysTime.wYear,nSysTime.wMonth,nSysTime.wDay);
    fp=fopen(fname,"w+t");
	if(fp==NULL) return;
	fprintf(fp,"%02d:%02d:%02d_" , nSysTime.wHour, nSysTime.wMinute,nSysTime.wSecond);
	fprintf(fp,"%s\n",sMsg.GetBuffer(sMsg.GetLength()));


	fclose(fp);

}

void CLogUnit::Trace(char * sMsg)
{
	CString _sMsg = sMsg;

	Trace(_sMsg);
/*
	char Msg[256] ;
    sprintf(Msg , "%s_%s" __FILE__,"");
    Trace(Msg);
*/
  }


