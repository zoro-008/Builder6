//---------------------------------------------------------------------------
#ifndef aBlobH
#define aBlobH
//---------------------------------------------------------------------------
#include "Image.h"
//#include "SVInspStruct.h"
#include "SVDefine.h"
//Blob..........................................................................
struct BLB_Para {
    int  iPitchX     ; //���� �˻� X ��ġ
    int  iPitchY     ; //���� �˻� Y ��ġ
    int  iStartXOfs  ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    int  iStartYOfs  ; //ó�� ��ŸƮ �ϴ� ��ġ �����V.
    int  iThreshold  ; //�˻� Ʈ�����.
    bool bFindLtOnDk ; //����������ü���� ����� ������ü����.
    bool bIgnrInside ; //���� ���� ���� ����.
    bool bIgnrSide   ; //�簢�� ������ ��迡 �ִ� ��� ����.

    //Filtering.
    int   iMinArea        ; int   iMaxArea        ; //������ �θƽ���
    int   iMinWidth       ; int   iMaxWidth       ; //����
    int   iMinHeight      ; int   iMaxHeight      ; //����
    int   iMinBoundary    ; int   iMaxBoundary    ; //�׵θ�
    float fMinCompactness ; float fMaxCompactness ; //������/�׵θ�
    float fMinAngle       ; float fMaxAngle       ; //���� �������� �ʾ���.
    float fMinAspectRatio ; float fMaxAspectRatio ; //
} ;

struct BLB_Rslt {
    BLB_Rslt(){
        iBlobCnt = 0 ;
        pBlobs   = NULL ;
    }
    ~BLB_Rslt(){ //�̷�����.....
        iBlobCnt = 0 ;
        if(pBlobs) {
            delete [] pBlobs ;
            pBlobs = NULL ;
        }
    }
    Clear(){
        iBlobCnt = 0 ;
        if(pBlobs) {
            delete [] pBlobs ;
            pBlobs = NULL ;
        }
    }



    int     iBlobCnt  ; //���߿� �ð� ���� �ɸ��� CLinkedList<SBlob> ���� �����غ��� �ȵɼ��� �ִ�.
    SBlob * pBlobs    ;

    float   fInspTime ;
} ;

struct BLB_Disp {
    BLB_Disp() {
        clRect     = clNone ;
        clCross    = clNone ;
        clBoundary = clNone ;
        clText     = clNone ;
    }
    COLORREF clRect     ;
    COLORREF clCross    ;
    COLORREF clBoundary ;
    COLORREF clText     ;
} ;


class CBlob
{
    protected :

    public :

        CBlob();
        ~CBlob();

        bool Inspect  (CImage *_pImg , TRect     _tRect , BLB_Para _tPara , BLB_Rslt *_pRslt        );
        void PaintRslt(HDC     _hHdc , BLB_Rslt *_pRslt , BLB_Disp _tDisp , float _fScaleX = 1.0 , float _fScaleY = 1.0);

    //todo
    // Area�� ���� ó�� �ɼ� �ְ� ..

};


#endif



/*
�̰��� C++�� ������ ������(operator overloading)�� ����� ���Դϴ�.
C++������ �ǿ�����(operand)�� ���� ���Ƿ� �����ڸ� ������ �� �ֽ��ϴ�. 

���� ���� ���ǰ� ������ <<, >> �������� �������� iostream���� ã�� �� �ֽ��ϴ�. 
�ٷ� istream::operator>>(), ostream::operator<<() ����. 
(��, cin >> ... , cout << ... �� ���ϴ� �̴ϴ�.) 
�̰��� ����Ʈ ����� ���� ������� ��Ʈ�� ������� ��Ÿ���ϴ�. 

C++ �ڵ忡�� � �����ڰ� ���� ���� �׻� �������� ������������ Ȯ���ؾ� �մϴ�.
���� ���򸻿��� TOpenDialog::Options�� �˻��ϸ� ������ ���� ǥ���� ���ɴϴ�. 

typedef Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> TOpenOptions;

�̰��� TOpenDialog::Options�� Ÿ���� TOpenOptions�̰�, 
TOpenOptions�� Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> ���� ���ǵ� Ÿ������ �� �� �ֽ��ϴ�.

���⼭ �ٽ� 'Set'�κ��� ��ũ�� Ŭ���ϸ�, �̰��� VCL/CLX���� �����ϴ� �ڷ����� Set(����)�̶�� ���ø� Ŭ������ �� �� �ֽ��ϴ�. 
(������ ANSI C++ ǥ�� ���̺귯���� std::set���� ���� �ٸ� �̴ϴ�. �����ϼ���.) 
��, Set::operator<<() �� ���տ� ���Ҹ� �����ϴ� �������̰�,
Set::operator>>()�� ���տ��� ���Ҹ� �����ϴ� �������Դϴ�. 

�׷��Ƿ�, �Ʒ��� �ڵ�� 

:   OpenDialog1->Options >> ofAllowMultiSelect; 
:   OpenDialog1->Options >> ofNoChangeDir; 
:   OpenDialog1->Options << ofExtensionDifferent; 

OpenDialog1->Options ���տ��� ofAllowMultiSelect, ofNoChangeDir��� ���Ҹ� �����ϰ�, 
ofExtensionDifferent��� ���Ҹ� �����Ѵٴ� �ǹ�����.

: �� <<, >> �� ������ ������ ����ŭ ��Ʈ���� �Űܰ��� ���� �˾Ҵµ�..
: �ƿ﷯ �̷��͵� �ִ���.. 
: OpenDialog1->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;.. 

���� Set::operator<<()�� ostream::operator<<()ó�� �������� �����ؼ� ����� �� �ֽ��ϴ�. 
Set::operator<<()�� ���ϰ��� Set& �� �Ǳ� ��������.

�̷��� ���� Ÿ���� VCL/CLX���� (�ΰ� �̻�) ������ �ɼ��� �����ϴ� ��� ��쿡
�������ϰ� ���˴ϴ�. ������ �ݵ�� �����صνñ� �ٶ��ϴ�.

���� ��� ������ �޽��� ���̾�α׸� ǥ���ϴ� �� ���� ����ϴ�
MessageDlg �Լ��� �� ���� �׷���. ���� ���... 

if (MessageDlg("������ ������ �����Ͻðڽ��ϱ�?", mtWarning, 
           TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
{          // TMsgDlgButtons ���� Ÿ���� �ӽ� ��ü�� �����ϰ� mbYes�� mbNo ���Ҹ� ���� 
    DeleteFile(OpenDialog1->FileName);
}

C++������ C++ �������� ��ŭ C++�� ���� ����� ���ذ� �ʿ��մϴ�.
C ���ĸ����δ� ���� ����� �ٷ� �� ���� ���Դϴ�.
�� ��쿡�� ������ �������� ���ø�, �ӽ� ��ü ���� ����� ���� ���ذ� �ʿ�������.
C++�� ���� Ȯ���� �����α⸦ �ٶ��ϴ�.
*/