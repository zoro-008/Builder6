#ifndef __DATASEQUENCE_H
#define __DATASEQUENCE_H

#include <afxtempl.h>
#include <memory.h>
#include <string.h>


class CDataSequence: public CObject
{
public:
	long m_nNum;
	int m_nFailStatus;
	double m_nScore[5];
    BYTE *m_pImg;
	CDataSequence(){memset(m_nScore,0x00,sizeof(double)*5);}
	~CDataSequence(){}

	inline void CDataSequence::SetScore(double nScore[5]){memcpy(m_nScore,nScore,sizeof(double)*5);}
};

typedef CTypedPtrList<CObList,CDataSequence*> CDataSequenceList;
#endif

