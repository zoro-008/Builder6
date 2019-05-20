#include "stdafx.h"
#include "hist.h"

int CHist::HistogramEqualize(int nW,int nH,unsigned char *pImg,unsigned char *pOut)
{
	int nFrequency[256]; // �󵵼�
	int nCDF[256]; // ���� Ȯ���е� �Լ� .. ����� ���Ǹ� ���� �� ����
	int nEqualize[256]; // ���� pixel�� ���� �����ϴ� LUT Ȥ�� index table;
	int i;
    int nTotal = nW*nH; // ���� ���� ũ�� , nW : width , nH: height
	int sum=0;
	
	// ���� �� �������� �ʱ�ȭ �Ѵ�.
	memset(nFrequency,0x00,sizeof(int)*256);
	memset(nCDF,0x00,sizeof(int)*256);
	memset(nEqualize,0x00,sizeof(int)*256);
	
	// �󵵼��� ���Ѵ�. ������ Ȯ���� ���Ѵ�. PDF(probability density function)
	for(i=0; i<nTotal; i++)		nFrequency[pImg[i]]++;
	
	// CDF�� ���Ѵ�. 
	for(i=0; i<256; i++)
	{
		sum+=nFrequency[i];
		nCDF[i]=sum;
	}
	
	// index table�� ����Ѵ�.
	if(nCDF[0]==nCDF[255]) return 0;

	for(i=0; i<256; i++)
	{
		nEqualize[i]=((nCDF[i]-nCDF[0])*255)/(nCDF[255]-nCDF[0]);
	}

	// �̹����� ��ȯ�Ѵ�. 
	for(i=0; i<nTotal; i++)
		pOut[i]=(unsigned char)nEqualize[pImg[i]];
    
	return 1;
}

void CHist::HsitogramEqualizeLog(int nW,int nH, unsigned char *pImg,unsigned char *pOut)
{
   // q(i,j) = 255/log(1+high) * ( 1+ p(i,j));
	int high=0;
	double coff;
	int nTotal=nW*nH;
	int i;
    
	for(i=0; i<nTotal; i++)
		if(pImg[i]>high) high=pImg[i];

	coff=255.0/log(1.0+(double)high);

	for(i=0; i<nTotal; i++)
		pOut[i]=(unsigned char)( coff*log(1.0+(double)pImg[i]) );

	return;
}