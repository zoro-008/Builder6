/******************************************************
(��) �Ѷ����� �����Ͼ 
������  : ����� 
��¥    : 2002.2.26
Version : v1.00
���    : Align data managing routines.
		  Linked-list�� �̿��Ͽ� �����Ѵ�. 
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
    POINT  m_CamOneMark;  // ī�޶� 1�� ������ġ ��
	POINT  m_CamOneGlass;  // ī�޶� 1�� ������ġ �� 
	POINT  m_CamTwoMark;  // ī�޶� 2�� ������ġ �� 
	POINT  m_CamTwoGlass;  // ī�޶� 2�� ������ġ �� 
	double m_nAngleMark;  // ���� ���� 
	double m_nAngleGlass;
	double m_nAngleDiff;  // ���� ���� 
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