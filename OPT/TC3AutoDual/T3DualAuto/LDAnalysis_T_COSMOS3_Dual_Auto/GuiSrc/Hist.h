/************************************************
 작성자    : 김양주 
 기능      : 이미지 평활화 
 
 목적      : 선일이가 하도 갈궈서..... 
             제 발 이후에 안 갈구길 .....
 
 사용 방법 :
		1) header file include 
         #include "hist.h"
		2) 인스턴스 발생 
		 CHist h;
        3) 이미지를 메모리에 할당.   프로그램 시작할때 
		   unsigned char *pImg= new [nW*nH];
		   unsigned char *pOut= new [nW* nH];
		4) 원본 이미지 하나를 읽음 
		    
			  pImg가 원본이미지라 가정

        5) 루틴 실행 

			h.HistogramEqualize(nW,nH,pImg,pOut);
            
			  ; pOut에 결과가 저장 됨 
	    
		6) pOut이미지를 화면에 뿌림 

        7)  이미지 할당한거 제거  , 프로그램 종료 할때 
		   delete[] pImg;
		   delete[] pOut;

 * 재미 삼아서 Log 함수도 만들어 보았는데.. 별루 안좋음....
 * 원래 논문에 있는게 더 나을거 같음.. 하도 오래전에 해서 까먹음.....ㅋㅋㅋ 
 * 개념 적인 것만 구현 .. 그래두 괞찮은거 같음.....
*************************************************/
#ifndef __HIST_H
#define __HIST_H

class CHist:public CObject
{
public:
	CHist(){}
	~CHist(){}

	int  HistogramEqualize(int nW,int nH,unsigned char *pImg,unsigned char *pOut);
	void HsitogramEqualizeLog(int nW,int nH, unsigned char *pImg,unsigned char *pOut);

};
#endif