#include "stdafx.h"
#include "Hvsystem.h"
#include "BeamDistribution.h"

CBeamDistribution::CBeamDistribution()
{
	m_nWidth=640;
	m_nHeight=480;

	m_nCx=320.0;
	m_nCy=240.0;

	m_pImg=NULL;
	m_pOut=NULL;
	m_pTmp=NULL;
}

CBeamDistribution::~CBeamDistribution()
{
}

void CBeamDistribution::InitApp(int nW,int nH)
{
	m_nWidth=nW;
	m_nHeight=nH;
	m_nCx=(double)nW/2.0;
	m_nCy=(double)nH/2.0;

    m_pImg=new unsigned char [m_nWidth*m_nHeight];
	m_pOut=new unsigned char [m_nWidth*m_nHeight];
	m_pTmp=new unsigned char [m_nWidth*m_nHeight];
	
	return;
}

void CBeamDistribution::SetBuffer(unsigned char *pImg)
{
	memcpy(m_pImg,pImg,m_nWidth*m_nHeight);
	return;
}
