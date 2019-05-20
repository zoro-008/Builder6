/******************************************************
(주) 한라정밀 엔지니어링 
만든이  : 김양주 
날짜    : 2002.2.26
Version : v1.00
기능    : Align data managing routines.
		  Linked-list를 이용하여 관리한다. 
*******************************************************/
#ifndef __ALIGNDATA_H_
#define __ALIGNDATA_H_

#include <afxtempl.h>
#include <string.h>
#include <memory.h>

class CAlignData : public CObject
{
private:
	char   m_pstrTime[20];//yy-mm-dd-hh-mm-ss-hh
    POINT  m_CamOneMark;  // 카메라 1의 기준위치 값
	POINT  m_CamOneGlass;  // 카메라 1의 측정위치 값 
	POINT  m_CamTwoMark;  // 카메라 2의 기준위치 값 
	POINT  m_CamTwoGlass;  // 카메라 2의 측정위치 값 
	double m_nAngleMark;  // 기준 각도 
	double m_nAngleGlass;
	double m_nAngleDiff;  // 각도 차이 
public:
	CAlignData();
	~CAlignData() {}
	void SetStrTime();
	int SaveData(char *fname);

	inline void CAlignData::SetStrTime(char *pStr)      { strcpy(m_pstrTime,pStr);                 }
	inline void CAlignData::SetCamOneMark(POINT Pt)     { memcpy(&m_CamOneMark,&Pt,sizeof(POINT)); }
	inline void CAlignData::SetCamOneGlass(POINT Pt)     { memcpy(&m_CamOneGlass,&Pt,sizeof(POINT)); }
	inline void CAlignData::SetCamTwoMark(POINT Pt)     { memcpy(&m_CamTwoMark,&Pt,sizeof(POINT)); }
	inline void CAlignData::SetCamTwoGlass(POINT Pt)     { memcpy(&m_CamTwoGlass,&Pt,sizeof(POINT)); }
	inline void CAlignData::SetAngleMark(double nValue) { m_nAngleMark=nValue;                     }
	inline void CAlignData::SetAngleGlass(double nValue){ m_nAngleGlass=nValue;}
	inline void CAlignData::SetAngleDiff(double nADiff) { m_nAngleDiff=nADiff;                     }

	inline char   *CAlignData::GetStrTime()   { return m_pstrTime;   }
	inline POINT  CAlignData::GetCamOneMark() { return m_CamOneMark; } 
	inline POINT  CAlignData::GetCamOneGlass() { return m_CamOneGlass; }
	inline POINT  CAlignData::GetCamTwoMark() { return m_CamTwoMark; } 
	inline POINT  CAlignData::GetCamTwoGlass() { return m_CamTwoGlass; }
	inline double CAlignData::GetAngleMark()  { return m_nAngleMark; }
	inline double CAlignData::GetAngleGlass() { return m_nAngleGlass; }
	inline double CAlignData::GetAngleDiff()  { return m_nAngleDiff; }

};
#endif