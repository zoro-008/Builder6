#include "stdafx.h"
#include "hist.h"

int CHist::HistogramEqualize(int nW,int nH,unsigned char *pImg,unsigned char *pOut)
{
	int nFrequency[256]; // 빈도수
	int nCDF[256]; // 누적 확률밀도 함수 .. 계산의 편의를 위해 걍 만듬
	int nEqualize[256]; // 실제 pixel의 값을 결정하는 LUT 혹은 index table;
	int i;
    int nTotal = nW*nH; // 실제 영상 크기 , nW : width , nH: height
	int sum=0;
	
	// 먼저 각 변수들을 초기화 한다.
	memset(nFrequency,0x00,sizeof(int)*256);
	memset(nCDF,0x00,sizeof(int)*256);
	memset(nEqualize,0x00,sizeof(int)*256);
	
	// 빈도수를 구한다. 원래는 확률을 구한다. PDF(probability density function)
	for(i=0; i<nTotal; i++)		nFrequency[pImg[i]]++;
	
	// CDF를 구한다. 
	for(i=0; i<256; i++)
	{
		sum+=nFrequency[i];
		nCDF[i]=sum;
	}
	
	// index table를 계산한다.
	if(nCDF[0]==nCDF[255]) return 0;

	for(i=0; i<256; i++)
	{
		nEqualize[i]=((nCDF[i]-nCDF[0])*255)/(nCDF[255]-nCDF[0]);
	}

	// 이미지를 변환한다. 
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