// Chain.h: interface for the CChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAIN_H__6C4F1463_1134_11D6_89EC_00B0D0D334DD__INCLUDED_)
#define AFX_CHAIN_H__6C4F1463_1134_11D6_89EC_00B0D0D334DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HIGH         255  // ����� gray level��
#define LOW          0    // ��ü�� gray level�� 
#define BOUNDARY     1    // ��� �ȼ��� �̰����� �����Ѵ�.
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
// Circleness�� ���ؼ� ����
	float FindCircleness(int N);
// Moment�� ���ؼ� ����
	float FindMoment(int N,float cx,float cy);
// �� Chain����Ÿ�� ���۹����� return�Ѵ�.
	int * GetChainData(int N);
// �� Chain�� ����Ÿ���� �� return�Ѵ�.
	int GetChainDataNumber(int N);
// Principal Angle�� ���ϴ� �Լ��̴�.
	float FindAngle(int N);
// Center���ϴ� �Լ� (Area�� �����߽�)
// N    : Chain ��ȣ 
// Return�� : �߽� ( *cx, *cy ) 
	int Chain_Center(int N, float *cx, float *cy);
// Center ���ϴ� �Լ� ; Chain ����Ÿ �� ��赥��Ÿ������ ���ϴ� �߽�( N : chain ��ȣ )
	int FindCenterWithBoundary(int N,float *cx,float *cy);
// Length���ϴ� �Լ� ( N : chain ��ȣ )
	float Chain_Length(int N);
// Area���ϴ� �Լ� ( N : chain ��ȣ ) , ©���� ������ �κ� �������� ���� 
	float Chain_LogicalArea(int N);
// Area���ϴ� �Լ� ( N : chain ��ȣ ) , ©���� ������ �κ� ����
	float Chain_Area(int N);
// ChainData�� ���Ѵ�.
	int FastChain(int x1,int y1,int x2,int y2);
// ����Ÿ�� �����Ѵ�. FastChain�� �θ������� �ݵ�� �̸� Call����� �Ѵ�
// argument�� �Ʒ� ������� ���� 
	void SetChainData(int object,unsigned char* fm,int skipx,int skipy,int minboundary,long maxboundary,int width,int height);
	void EndChain();
// Chain class�� ������ : ���Ҽ� �ִ� �ִ� ��ü�� ��(nMaxChsin)�� �� ��ü����(nMaxChainData)�� �����Ѵ�.
	void InitChain(int nMaxChain,long nMaxChainData);


	CChain();
	virtual ~CChain();
};

#endif // !defined(AFX_CHAIN_H__6C4F1463_1134_11D6_89EC_00B0D0D334DD__INCLUDED_)
