/************************************************
 �ۼ���    : ����� 
 ���      : �̹��� ��Ȱȭ 
 
 ����      : �����̰� �ϵ� ���ż�..... 
             �� �� ���Ŀ� �� ������ .....
 
 ��� ��� :
		1) header file include 
         #include "hist.h"
		2) �ν��Ͻ� �߻� 
		 CHist h;
        3) �̹����� �޸𸮿� �Ҵ�.   ���α׷� �����Ҷ� 
		   unsigned char *pImg= new [nW*nH];
		   unsigned char *pOut= new [nW* nH];
		4) ���� �̹��� �ϳ��� ���� 
		    
			  pImg�� �����̹����� ����

        5) ��ƾ ���� 

			h.HistogramEqualize(nW,nH,pImg,pOut);
            
			  ; pOut�� ����� ���� �� 
	    
		6) pOut�̹����� ȭ�鿡 �Ѹ� 

        7)  �̹��� �Ҵ��Ѱ� ����  , ���α׷� ���� �Ҷ� 
		   delete[] pImg;
		   delete[] pOut;

 * ��� ��Ƽ� Log �Լ��� ����� ���Ҵµ�.. ���� ������....
 * ���� ���� �ִ°� �� ������ ����.. �ϵ� �������� �ؼ� �����.....������ 
 * ���� ���� �͸� ���� .. �׷��� �I������ ����.....
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