#ifndef _BEAM_DISTRIBUTION_H
#define _BEAM_DISTRIBUTION_H

class CBeamDistribution 
{
private:
	int m_nWidth;
	int m_nHeight;
	double m_nCx;
	double m_nCy;
	double m_nDCx;
	double m_nDCy;
	RECT m_cR;

	unsigned char *m_pImg;
	unsigned char *m_pOut;
	unsigned char *m_pTmp;
public:
	CBeamDistribution();
	~CBeamDistribution();
	void InitApp(int nW,int nH);
	void SetBuffer(unsigned char *pImg);
};

#endif


