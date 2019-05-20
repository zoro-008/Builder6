// Chain.cpp: implementation of the CChain class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <stdlib.h>
#include "Chain.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


float L[9][9]={ {0,0,0,0,0,0,0,0,0},{0,4,5,6,7,8,0,2,3},{0,3,4,5,6,7,8,0,0},
			   {0,2,3,4,5,6,7,8,0},{0,0,0,3,4,5,6,7,8},{0,8,0,2,3,4,5,6,7},
			   {0,7,8,0,0,3,4,5,6},{0,6,7,8,0,2,3,4,5},{0,5,6,7,8,0,0,3,4}};

//BYTE  OBJECT;         // ��ü�� ��Ⱚ  LOW  �ƴϸ� HIGH
//BYTE  BACKGROUND;     // ����� ��Ⱚ  HIGT �ƴϸ� LOW
float  OBJECT;         // ��ü�� ��Ⱚ  LOW  �ƴϸ� HIGH
float  BACKGROUND;     // ����� ��Ⱚ  HIGT �ƴϸ� LOW
int  MAXCHAIN;       // ���Ҽ� �ִ� Chain�� �� ( ��ü�� �� LIMIT)
long MAXCHAINDATA;   // ��ü Chain�� ���� (��ü�� ��ü ������ ����Ÿ LIMIT)
int  m_chainCount;   // ���� Chain�� �� (��ü�� ��)
int  *m_chain;       // �� Chain�� ����( [1]=N1, [2]=N2, [3]=N3....) 
int  *m_chaindata;   // Chain Data ( x0��ǥ, y0��ǥ, x1��ǥ, y1��ǥ, ...) 
long *m_chainstart;  // ���κ��� (�迭�� Index������ ��� Chain �� total���̸� ����)
unsigned char* m_fm;          // ������ ���۹���
int  m_skipx;        // ��ĵ�Ҷ� Skip�ϴ� �ȼ� �� (x����)
int  m_skipy;        // ��ĵ�Ҷ� Skip�ϴ� �ȼ� �� (y����)
int  m_minboundary;  // Chain���̰� �̰� �̻��϶��� ���Ѵ�.
long m_maxboundary;  // Chain���̰� �̰� �����϶��� ���Ѵ�.
int  m_Pitch;      // ������ x�� ��
int  BIMAGE_HEIGHT;   // ������ y�� ��
int  FLAG_SetData;   // ���������� ��� ( ���� Data�� ���õǾ������� TRUE)
//BOOL  FLAG_SetData;   // ���������� ��� ( ���� Data�� ���õǾ������� TRUE)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChain::CChain(){
}

CChain::~CChain(){

	EndChain();
}

void CChain::InitChain(int nMaxChain, long nMaxChainData)
{
    m_chain     = (int*)malloc(nMaxChain*sizeof(int)); // �ִ�� ���� Chain �� 
	m_chainstart= (long*)malloc(nMaxChain*sizeof(long));  // 
	m_chaindata = (int*)malloc(2*nMaxChainData*sizeof(int));  // ���� Chain�� ������ ���� MAXCHAINDATA�� ������ ����.
    MAXCHAIN     = nMaxChain;
	MAXCHAINDATA = nMaxChainData;
//	FLAG_SetData = FALSE;
	FLAG_SetData = 0;
}

void CChain::EndChain()
{
    if(m_chain)			free(m_chain);
	if(m_chainstart)	free(m_chainstart);
	if(m_chaindata)		free(m_chaindata);
}

void CChain::SetChainData(int Object, unsigned char *fm, int skipx, int skipy, int minboundary, long maxboundary, int width, int height)
{
	 if(Object==0) { OBJECT=LOW  ; BACKGROUND = HIGH; }
	 else          { OBJECT=HIGH ; BACKGROUND = LOW ; }
	 m_fm          = fm;          // �޸𸮵���Ÿ�� ���۹��� 
     m_skipx	   = skipx;       // x���� skip �ȼ���  ??
     m_skipy	   = skipy;       // y���� skip �ȼ���  ??
	 m_minboundary = minboundary; // ������ chain�� ���� (�̰������� chain �� ����)
     m_maxboundary = maxboundary; // ������ chain�� ���� (�̰��̻��� chain �� ����)
	 m_Pitch	   = width;       // ������ x ���� ��
	 BIMAGE_HEIGHT  = height;      // ������ y ���� ��
//	 FLAG_SetData  = TRUE;
	 FLAG_SetData  = 1;

}

int CChain::FastChain(int x1, int y1, int x2, int y2)
{
	 int i,j,k;
	 int dum;
	 int blackpoint, totaldatanumber=0;
          
     if(!FLAG_SetData) return -1; // Data Setting�� �ȵǾ����� -1�� return �Ѵ�. 

	 if(x1>x2) { dum=x1; x1=x2; x2=dum ; }
	 if(y1>y2) { dum=y1; y1=y2; y2=dum ; }
	 if(x1<0 || y1 <0 ) return -2;
	 if(x2>=m_Pitch)    x2=m_Pitch-1;
	 if(y2>=BIMAGE_HEIGHT) y2=BIMAGE_HEIGHT-1;

	 // initialize 

	 if(m_skipx<1) m_skipx=1;              // Skip�ϴ� �ȼ��� 2���� ������ ����.
	 if(m_skipy<1) m_skipy=1;              //
	 if(m_minboundary<1) m_minboundary=1;  // ���� chain�� ���̰� 5���� ������ ����.
	 
	 m_chainCount=0;

     //BEGIN-0 : ������ Line�� ���� ������� ä���. >>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 for(i=x1;i<=x2;i++)                   
	 {    
		  *(m_fm+m_Pitch*y1+i)=(unsigned char)BACKGROUND;
		  *(m_fm+m_Pitch*y2+i)=(unsigned char)BACKGROUND;  
	 }
	 for(i=y1;i<=y2;i++)
	 {  
		  *(m_fm+m_Pitch*i+x1)=(unsigned char)BACKGROUND;
		  *(m_fm+m_Pitch*i+x2)=(unsigned char)BACKGROUND; 
	 }
	 //END-0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	 //scan and find boundary pixel 
	 for (i = y1; i <= y2; i += m_skipy)
	 {
		 for (j = x1; j <= x2; j += m_skipx)
		 {
			 if (*(m_fm + i*m_Pitch + j) == OBJECT )/*&& *(m_fm + i*m_Pitch + j + 1) == OBJECT) // because of noise*/ 
			 {
				 blackpoint = 0;
				 for (k = j; k >= (j - m_skipx); k--)
				 {
					 if (*(m_fm + i*m_Pitch + k) == BACKGROUND)
					 {
						 blackpoint = k + 1;
						 break;
					 } // (blackpoint,i) is boundary pixel 
				 }
				 // if this object didnt be checked yet
				 if (*(m_fm + i*m_Pitch + blackpoint) != BOUNDARY && blackpoint != 0)
				 {
					 m_chain[m_chainCount] = FindBoundaryPixel(blackpoint, i, &m_chaindata[2 * totaldatanumber], MAXCHAINDATA - totaldatanumber);
					 if (m_chain[m_chainCount] >= m_minboundary)
					 {
						 totaldatanumber += m_chain[m_chainCount];
						 m_chainCount++;
					 }

					 //  if totaldatanumber is greater than MAXCHAINDATA or   
					 //  if m_chainCount is greater than MAXCHAIN--> STOP finding 
					 else if (m_chain[m_chainCount] == -1) goto END;
					 if (m_chainCount >= MAXCHAIN) goto END;
				 }
			 }
		 }
	 }
	 
END: totaldatanumber=0;
	 for(i=0;i<m_chainCount;i++)
     {
		 m_chainstart[i]=2*totaldatanumber;
         totaldatanumber+=m_chain[i];
	 }

	 return m_chainCount;


}

float CChain::Chain_Area(int N)
{
	 int FirstValue=0,PrevValue=0, CurrValue;
	 float  Area=0, ExceptedArea=0;
	 int number;
     int *BPixel;      
	 int i;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 // 1�� �ȼ��� ������ �ȼ��� ���ؼ� 1�� �ȼ��� �˻��� �� ���� ����
	 if(BPixel[0] > BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=5; Area+=BPixel[1]-0.5;}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=7; Area+=BPixel[1]+0.5;}
		 else                                 { PrevValue=6; Area+=BPixel[1];}
	 }
	 else if(BPixel[0] < BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=3; Area-=BPixel[1]-0.5;}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=1; Area-=BPixel[1]+0.5;}
		 else                                 { PrevValue=2; Area-=BPixel[1];}
	 }
	 else
	 {
		 if(BPixel[1]>BPixel[2*number-1])      PrevValue=4;
		 else if(BPixel[1]<BPixel[2*number-1]) PrevValue=8;
		 else   exit(0);
	 }

	 FirstValue=PrevValue; // 1�� �ȼ��� �˻����� FirstValue�� �̸� ����

	 // 2�� �ȼ����� ������ �ȼ����� �˻����� ���� ����
	 for(i=1;i<number;i++)
	 {
		if(BPixel[2*i] > BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=5; Area+=BPixel[2*i+1]-0.5;}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=7; Area+=BPixel[2*i+1]+0.5;}
		  else                                 { CurrValue=6; Area+=BPixel[2*i+1];}
		}
		else if(BPixel[2*i] < BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=3; Area-=BPixel[2*i+1]-0.5;}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=1; Area-=BPixel[2*i+1]+0.5;}
		  else                                 { CurrValue=2; Area-=BPixel[2*i+1];}
		}
		else
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      CurrValue=4;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) CurrValue=8;
		  else   exit(0);
		}

		// ©���� ������ ���⼭ ��������
		ExceptedArea+=L[PrevValue][CurrValue];   // 1�� ���� number-1 ����.

		PrevValue=CurrValue;		

	 }

	 ExceptedArea+=L[PrevValue][FirstValue]; // number ° ���� ����    //
	 ExceptedArea = ExceptedArea/8.0;								   //

	 Area+= ExceptedArea;											   //

	 return Area;

}

float CChain::Chain_LogicalArea(int N)
{
	 float  Area=0;
	 int number;
     int *BPixel;      
	 int i;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 // 1�� �ȼ��� ������ �ȼ��� ���ؼ� 1�� �ȼ��� �˻��� �� ���� ����
	 if(BPixel[0] > BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     Area+=BPixel[1]-0.5;
		 else if(BPixel[1]<BPixel[2*number-1])Area+=BPixel[1]+0.5;
		 else                                 Area+=BPixel[1];
	 }
	 else if(BPixel[0] < BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     Area-=BPixel[1]-0.5;
		 else if(BPixel[1]<BPixel[2*number-1])Area-=BPixel[1]+0.5;
		 else                                 Area-=BPixel[1];
	 }
	 
	 // 2�� �ȼ����� ������ �ȼ����� �˻����� ���� ����
	 for(i=1;i<number;i++)
	 {
		if(BPixel[2*i] > BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      Area+=BPixel[2*i+1]-0.5;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) Area+=BPixel[2*i+1]+0.5;
		  else                                 Area+=BPixel[2*i+1];
		}
		else if(BPixel[2*i] < BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      Area-=BPixel[2*i+1]-0.5;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) Area-=BPixel[2*i+1]+0.5;
		  else                                 Area-=BPixel[2*i+1];
		}
	
	 }

	 return Area;

}

float CChain::Chain_Length(int N)
{
	 float dLength=0;
	 int number;
     int *BPixel;      
	 int i;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

     for(i=0 ;i<number-1;i++)
       if(BPixel[2*i]!=BPixel[2*i+2] && BPixel[2*i+1]!=BPixel[2*i+3])
	         dLength+=ROOT2;
	   else  dLength+=1;
     
     if(BPixel[0]!=BPixel[2*number-2] && BPixel[1]!=BPixel[2*number-1])
	       dLength+=ROOT2;
	 else  dLength+=1;

	 return dLength;

}

int CChain::FindCenterWithBoundary(int N, float *cx, float *cy)
{
	 int i;
	 float sumx,sumy;
	 int Number;
     int *BPixel;      

	 Number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 sumx=sumy=0;
	 for(i=0;i<Number;i++)
	 {
			  sumx+= BPixel[2*i];;
			  sumy+= BPixel[2*i+1];
	 }

	 *cx = sumx / Number;
	 *cy = sumy / Number;

	 return 0;

}

int CChain::Chain_Center(int N, float *cx, float *cy)
{
	 int i,FirstValue=0,PrevValue=0, CurrValue;
	 float XSum,YSum,LineSum,Area;
     float x,y, xExcept, yExcept;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

//BEGIN-0 : x�� ���Ϳ� Area�� ���Ѵ�.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	 LineSum=0;
	 XSum=0;
	 Area =0;
	 x=(BPixel[0] + BPixel[2*number-2])/2.0;
	 if(BPixel[0] > BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=5; LineSum=(BPixel[1]-0.5);}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=7; LineSum=(BPixel[1]+0.5);}
		 else                                 { PrevValue=6; LineSum=(BPixel[1]);}
	 }
	 else if(BPixel[0] < BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=3; LineSum=-(BPixel[1]-0.5);}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=1; LineSum=-(BPixel[1]+0.5);}
		 else                                 { PrevValue=2; LineSum=-(BPixel[1]);}
	 }
	 else
	 {
		 if(BPixel[1]>BPixel[2*number-1])      PrevValue=4;
		 else if(BPixel[1]<BPixel[2*number-1]) PrevValue=8;
		 else   exit(0);
	 }

	 Area=LineSum;
	 FirstValue=PrevValue; // 1�� �ȼ��� �˻����� FirstValue�� �̸� ����
	 XSum=x*LineSum;
	 
	 for(i=1;i<number;i++)
	 {
        x=(BPixel[2*i] + BPixel[2*i-2])/2.0;
		LineSum=0;
		if(BPixel[2*i] > BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=5; LineSum=(BPixel[2*i+1]-0.5);}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=7; LineSum=(BPixel[2*i+1]+0.5);}
		  else                                 { CurrValue=6; LineSum=(BPixel[2*i+1]);}
		}
		else if(BPixel[2*i] < BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=3; LineSum=-(BPixel[2*i+1]-0.5);}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=1; LineSum=-(BPixel[2*i+1]+0.5);}
		  else                                 { CurrValue=2; LineSum=-(BPixel[2*i+1]);}
		}
		else
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      CurrValue=4;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) CurrValue=8;
		  else   exit(0);
		}
 
		Area+=LineSum;
		XSum+=x*LineSum;

		// ©���� ������ ���⼭ ��������
		Area+=L[PrevValue][CurrValue]/8.0;
		xExcept=BPixel[2*i-2];
		XSum+=xExcept*L[PrevValue][CurrValue]/8.0;   // 1�� ���� number-1 ����.

		PrevValue=CurrValue;		

	 }

     Area+=L[PrevValue][FirstValue]/8.0;  // number ° ���� ����  
	 xExcept=BPixel[2*number-2];						  
	 XSum+=xExcept*L[PrevValue][FirstValue]/8.0; // number ° ���� ����     

     *cx=XSum/Area;
//END-0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//BEGIN-0 : Y�� ���͸� ���Ѵ�. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 
     YSum=0;
	 y=(BPixel[1] + BPixel[2*number-1])/2.0;
	 if(BPixel[1] > BPixel[2*number-1])
	 {
		 if(BPixel[0]>BPixel[2*number-2])     { PrevValue=5; YSum-=y*(BPixel[0]-0.5);}
		 else if(BPixel[0]<BPixel[2*number-2]){ PrevValue=3; YSum-=y*(BPixel[0]+0.5);}
		 else                                 { PrevValue=4; YSum-=y*(BPixel[0]);}
	 }
	 else if(BPixel[1] < BPixel[2*number-1])
	 {
		 if(BPixel[0]>BPixel[2*number-2])     { PrevValue=7; YSum+=y*(BPixel[0]-0.5);}
		 else if(BPixel[0]<BPixel[2*number-2]){ PrevValue=1; YSum+=y*(BPixel[0]+0.5);}
		 else                                 { PrevValue=8; YSum+=y*(BPixel[0]);}
	 }
	 else
	 {
		 if(BPixel[0]>BPixel[2*number-2])      PrevValue=6;
		 else if(BPixel[0]<BPixel[2*number-2]) PrevValue=2;
		 else   exit(0);
	 }

	 FirstValue=PrevValue; // 1�� �ȼ��� �˻����� FirstValue�� �̸� ����

	 
	 for(i=1;i<number;i++)
	 {
        y=(BPixel[2*i+1] + BPixel[2*i-1])/2.0;
		if(BPixel[2*i+1] > BPixel[2*i-1])
		{
		  if(BPixel[2*i]>BPixel[2*i-2])      { CurrValue=5; YSum-=y*(BPixel[2*i]-0.5);}
		  else if(BPixel[2*i]<BPixel[2*i-2]) { CurrValue=3; YSum-=y*(BPixel[2*i]+0.5);}
		  else                               { CurrValue=4; YSum-=y*(BPixel[2*i]);}
		  
		}
		else if(BPixel[2*i+1] < BPixel[2*i-1])
		{
		  if(BPixel[2*i]>BPixel[2*i-2])      { CurrValue=7; YSum+=y*(BPixel[2*i]-0.5);}
		  else if(BPixel[2*i]<BPixel[2*i-2]) { CurrValue=1; YSum+=y*(BPixel[2*i]+0.5);}
		  else                               { CurrValue=8; YSum+=y*(BPixel[2*i]);}
		}
		else
		{
		  if(BPixel[2*i]>BPixel[2*i-2])      CurrValue=6;
		  else if(BPixel[2*i]<BPixel[2*i-2]) CurrValue=2;
		  else   exit(0);
		}

		// ©���� ������ ���⼭ ��������
		yExcept=BPixel[2*i-1];
		YSum+=yExcept*L[PrevValue][CurrValue]/8.0;   // 1�� ���� number-1 ����.

		PrevValue=CurrValue;		

	 }

	 yExcept=BPixel[2*number-1];									   
	 YSum+=yExcept*L[PrevValue][FirstValue]/8.0; // number ° ���� ����      //

     *cy=YSum/Area;


	 return 0;

}

// angle = atan2( 2*(M00*M11-M10*M01),(M00*M20-M10*M10)-(M00*M02-M01*M01))*90/3.141592;
// angle�� degree�̴�.
float CChain::FindAngle(int N)
{
	 int number;
     int *BPixel; 
     float M00, M11, M10, M01, M20, M02;
	 float dAngle;
	 int i;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 M00=M11=M10=M01=M20=M02=0;
	 for(i=0;i < number ;i++)
     {
		 M00++;
		 M10+=(BPixel[2*i]);
		 M01+=(BPixel[2*i+1]);
		 M11+=(BPixel[2*i])*(BPixel[2*i+1]);
		 M20+=(BPixel[2*i])*(BPixel[2*i]);
		 M02+=(BPixel[2*i+1])*(BPixel[2*i+1]);
     }
	 dAngle = atan2(2*(M00*M11-M10*M01),(M00*M20-M10*M10)-(M00*M02-M01*M01))*90/CHAIN_PI;

	 return  dAngle;

}

// �� Chain�� ����Ÿ���� �� return�Ѵ�.
int CChain::GetChainDataNumber(int N)
{
	return m_chain[N]; 
}

// �� Chain����Ÿ�� ���۹����� return�Ѵ�.
int* CChain::GetChainData(int N)
{
	return &m_chaindata[m_chainstart[N]]; 
}

float CChain::FindMoment(int N, float cx, float cy)
{
	 int number;
     int *BPixel;      
	 float dMoment;
	 int i;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

     dMoment=0;
	 for(i=0;i<number;i++)
		 dMoment+=(BPixel[2*i]-cx)*(BPixel[2*i]-cx)+(BPixel[2*i+1]-cy)*(BPixel[2*i+1]-cy);

	 return (dMoment/number);

}

float CChain::FindCircleness(int N)
{
	 float dCircleness;
	 float dLength;       
	 float dArea;

	 dLength = Chain_Length(N);
	 dArea   = Chain_Area(N);
	 dCircleness=(4*CHAIN_PI*dArea)/(dLength*dLength);

     if(dCircleness>1.0) dCircleness=1.0;

	 return dCircleness;

}

int CChain::FindMaxX(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[0];
     for(i=1;i<number;i++)
		 if(BPixel[2*i]>Data) Data=BPixel[2*i];

	 return Data;

}

int CChain::FindMinX(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[0];
     for(i=1;i<number;i++)
		 if(BPixel[2*i]<Data) Data=BPixel[2*i];

	 return Data;

}

int CChain::FindMaxY(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[1];
     for(i=1;i<number;i++)
		 if(BPixel[2*i+1]>Data) Data=BPixel[2*i+1];

	 return Data;

}

int CChain::FindMinY(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[1];
     for(i=1;i<number;i++)
		 if(BPixel[2*i+1]<Data) Data=BPixel[2*i+1];

	 return Data;

}

int CChain::FindBoundaryPixel(int startx, int starty, int *data, long emptyarray)
{
	  long count=0;
	  int cx,cy;
	  int bcx,bcy;
	  unsigned char* cfm;
	  int SP;   // start point 

	  if(emptyarray<0) return -1;

	  cx=bcx=startx; cy=bcy=starty;

	  data[0]=cx; data[1]=cy; count++; SP=3;

	  cfm=m_fm+cy*m_Pitch+cx;

	  do
	  {
		  switch(SP)
		  {
				case 1: if( *(cfm-m_Pitch-1)==OBJECT ||*(cfm-m_Pitch-1)==BOUNDARY)
						  {
							  cx=cx-1; cy=cy-1;
							  SP=7;
							  break;
						  }
				case 2: if( *(cfm-1)==OBJECT || *(cfm-1)==BOUNDARY)
						  {
							  cx=cx-1; cy=cy;
							  SP=1;
							  break;
						  }
				case 3: if( *(cfm+m_Pitch-1)==OBJECT || *(cfm+m_Pitch-1)==BOUNDARY)
						  {
							  cx=cx-1; cy=cy+1;
							  SP=1;
							  break;
						  }
				case 4: if( *(cfm+m_Pitch)==OBJECT || *(cfm+m_Pitch)==BOUNDARY)
						  {
							  cx=cx; cy=cy+1;
							  SP=3;
							  break;
						  }
				case 5: if( *(cfm+m_Pitch+1)==OBJECT || *(cfm+m_Pitch+1)==BOUNDARY)
						  {
							  cx=cx+1; cy=cy+1;
							  SP=3;
							  break;
						  }
				case 6: if( *(cfm+1)==OBJECT || *(cfm+1)==BOUNDARY)
						  {
							  cx=cx+1; cy=cy;
							  SP=5;
							  break;
						  }
				case 7: if( *(cfm-m_Pitch+1)==OBJECT || *(cfm-m_Pitch+1)==BOUNDARY)
						  {
							  cx=cx+1; cy=cy-1;
							  SP=5;
							  break;
						  }
				case 8: if( *(cfm-m_Pitch)==OBJECT || *(cfm-m_Pitch)==BOUNDARY)
						  {
							  cx=cx; cy=cy-1;
							  SP=7;
							  break;
						  }

				default:if( *(cfm-m_Pitch-1)==OBJECT || *(cfm-m_Pitch-1)==BOUNDARY)
						  {
							  cx=cx-1; cy=cy-1;
							  SP=7;
							  break;
						  }
						  else if( *(cfm-1)==OBJECT || *(cfm-1)==BOUNDARY)
						  {
							  cx=cx-1; cy=cy;
							  SP=1;
							  break;
						  }
						  else if( *(cfm+m_Pitch-1)==OBJECT || *(cfm+m_Pitch-1)==BOUNDARY)
						  {
							  cx=cx-1; cy=cy+1;
							  SP=1;
							  break;
						  }
						  else if( *(cfm+m_Pitch)==OBJECT || *(cfm+m_Pitch)==BOUNDARY)
						  {
							  cx=cx; cy=cy+1;
							  SP=3;
							  break;
						  }
						  else if( *(cfm+m_Pitch+1)==OBJECT || *(cfm+m_Pitch+1)==BOUNDARY)
						  {
							  cx=cx+1; cy=cy+1;
							  SP=3;
							  break;
						  }

		  }
		  if(count >= emptyarray)
			  return -1;  // �� ������ �ִ� �޸𸮸� �ʰ��ϸ� -1�� ������
		  data[2*count]  = cx;
		  data[2*count+1]= cy;
		  count++;
		  cfm=m_fm+cy*m_Pitch+cx;
		  *(cfm)=BOUNDARY;
		}while( cx!=startx || cy!=starty);
		if(--count<m_minboundary) return 0;   //���� boundary�� m_minboundary���� �۰ų� 
		else if(count>m_maxboundary) return 0;//				m_maxboundary���� ũ�� 0�� �����ش�.
		else                         return count;

}

int CChain::FineDistFromPoint(int N, float cx, float cy, float *min, float *max, float *avg)
{
	 int i, number;
     int *BPixel;      
	 float dist,sum;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 
	
	 dist=sqrt((BPixel[0]-cx)*(BPixel[0]-cx)+(BPixel[1]-cy)*(BPixel[1]-cy));
	 *min=*max=sum=dist;
	 for(i=1;i<number;i++)
	 {
		 dist=sqrt((BPixel[2*i]-cx)*(BPixel[2*i]-cx)+(BPixel[2*i+1]-cy)*(BPixel[2*i+1]-cy));
		 if(*min<dist) *min=dist;
		 if(*max>dist) *max=dist;
		 sum+=dist;
	 }
	 *avg=sum/number;

	return 0;

}
