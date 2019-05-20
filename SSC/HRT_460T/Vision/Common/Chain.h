// Chain.h: interface for the CChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAIN_H__6C4F1463_1134_11D6_89EC_00B0D0D334DD__INCLUDED_)
#define AFX_CHAIN_H__6C4F1463_1134_11D6_89EC_00B0D0D334DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HIGH         255  // 배경의 gray level값
#define LOW          0    // 물체의 gray level값 
#define BOUNDARY     1    // 경계 픽셀을 이값으로 세팅한다.
#define CHAIN_PI     3.1415926535
#define ROOT2        1.414213562

class CChain  
{
public:
	int FineDistFromPoint(int N,float cx,float cy,float *min,float *max,float *avg);
	int FindBoundaryPixel(int startx,int starty,int *data,long emptyarray);
	int FindMinY(int N);
	int FindMaxY(int N);
	int FindMinX(int N);
	int FindMaxX(int N);
// Circleness를 구해서 리턴
	float FindCircleness(int N);
// Moment를 구해서 리턴
	float FindMoment(int N,float cx,float cy);
// 각 Chain데이타의 시작번지를 return한다.
	int * GetChainData(int N);
// 각 Chain의 데이타수를 를 return한다.
	int GetChainDataNumber(int N);
// Principal Angle을 구하는 함수이다.
	float FindAngle(int N);
// Center구하는 함수 (Area의 무게중심)
// N    : Chain 번호 
// Return값 : 중심 ( *cx, *cy ) 
	int Chain_Center(int N, float *cx, float *cy);
// Center 구하는 함수 ; Chain 데이타 즉 경계데이타만으로 구하는 중심( N : chain 번호 )
	int FindCenterWithBoundary(int N,float *cx,float *cy);
// Length구하는 함수 ( N : chain 번호 )
	float Chain_Length(int N);
// Area구하는 함수 ( N : chain 번호 ) , 짤려져 나가는 부분 보상하지 않음 
	float Chain_LogicalArea(int N);
// Area구하는 함수 ( N : chain 번호 ) , 짤려져 나가는 부분 보상
	float Chain_Area(int N);
// ChainData를 구한다.
	int FastChain(int x1,int y1,int x2,int y2);
// 데이타를 세팅한다. FastChain을 부르기전에 반드시 이를 Call해줘야 한다
// argument는 아래 멤버변수 참조 
	void SetChainData(int object,unsigned char* fm,int skipx,int skipy,int minboundary,long maxboundary,int width,int height);
	void EndChain();
// Chain class의 생성자 : 구할수 있는 최대 물체의 수(nMaxChsin)와 그 전체길이(nMaxChainData)를 설정한다.
	void InitChain(int nMaxChain,long nMaxChainData);


	CChain();
	virtual ~CChain();
};

#endif // !defined(AFX_CHAIN_H__6C4F1463_1134_11D6_89EC_00B0D0D334DD__INCLUDED_)
