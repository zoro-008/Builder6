//---------------------------------------------------------------------------
#ifndef aBlobH
#define aBlobH
//---------------------------------------------------------------------------
#include "UnitImage.h"
#include "UnitArea.h"
#include "BaseDefine.h"

class CBlob
{
    public :
        struct TBlob : public TRect {
            int    iCenterX     ; //�ֿܰ����� ����
            int    iCenterY     ;
            int    iWidth       ;
            int    iHeight      ;
            int    iArea        ;
            double dBoundary    ; //�׵θ� ����.
            double dCompact     ; //������/����. �����ϼ��� ����.  ���� ����Ʈ�׽��� ����/������.
            double dCentroidX   ; //������ �����߽�.
            double dCentroidY   ;

            CLinkedList<TPoint> plBoundary ; //�ܰ� ������.

            TBlob() {
                Clear();
            };
            ~TBlob() {
                Clear();
            };
            void Clear() {

                left       = 0 ;
                top        = 0 ;
                right      = 0 ;
                bottom     = 0 ;

                iCenterX   = 0 ;
                iCenterY   = 0 ;
                iWidth     = 0 ;
                iHeight    = 0 ;
                iArea      = 0 ;

                dBoundary  = 0.0;
                dCompact   = 0.0;
                dCentroidX = 0.0;
                dCentroidY = 0.0;

                plBoundary.DeleteAll() ;
            }
            void Copy(TBlob & rhs) {
                left       = rhs.left       ;
                top        = rhs.top        ;
                right      = rhs.right      ;
                bottom     = rhs.bottom     ;

                iCenterX   = rhs.iCenterX   ;
                iCenterY   = rhs.iCenterY   ;
                iWidth     = rhs.iWidth     ;
                iHeight    = rhs.iHeight    ;
                iArea      = rhs.iArea      ;

                dBoundary  = rhs.dBoundary  ;
                dCompact   = rhs.dCompact   ;
                dCentroidX = rhs.dCentroidX ;
                dCentroidY = rhs.dCentroidY ;

                for(int i = 0 ; i < plBoundary.GetDataCnt() ; i++){
                    plBoundary.PushBack(rhs.plBoundary.GetCrntData(!i)) ;

                }


            }
            //Overriding the Operator
            void operator = (TBlob & rhs){
                Copy(rhs);
            }
        };

        struct TPara {
            double dPitchX     ; //��� �˻� X ��ġ
            double dPitchY     ; //��� �˻� Y ��ġ
            int    iThreshold  ; //�˻� Ʈ�����.
            bool   bFindLtOnDk ; //����������ü���� ����� ������ü����.
            bool   bIgnrInside ; //��� ���� ��� ����.
            bool   bIgnrSide   ; //�簢�� ������ ��迡 �ִ� ��� ����.
            int    iStartXOfs  ;
            int    iStartYOfs  ;
        } ;

        struct TFltr {
            //Filtering.
            int    iMinArea      ;
            int    iMaxArea      ; //������ �θƽ���
            int    iMinWidth     ;
            int    iMaxWidth     ; //����
            int    iMinHeight    ;
            int    iMaxHeight    ; //����
            double dMinBoundary  ;
            double dMaxBoundary  ; //�׵θ�
            double dMinCompact   ;
            double dMaxCompact   ; //������/�׵θ�
        } ;

        struct TRslt {
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                iBlobCnt = 0 ;

                dInspTime=0.0;
                sErrMsg  = "";

                TBlob* Blob ;
                for(int i=0 ; i < plBlob.GetDataCnt() ; i++) {
                    Blob = plBlob.GetCrntData(!i);
                    Blob->Clear() ;
                    delete Blob ;
                }
                plBlob.DeleteAll();

            }

            int     iBlobCnt  ;
            CLinkedList<TBlob*> plBlob ;

            double  dInspTime ;
            String  sErrMsg   ;
        } ;

        CBlob();
        ~CBlob();

    protected :
        static bool Inspect(unsigned char *_pPxl , int _iPxlWidth , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt);

    public :
        static bool Inspect(CImage *_pImg , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt);
        static bool Inspect(CArea  *_pAra , TRect _tRect , TPara _tPara , TFltr _tFltr , TRslt *_pRslt);

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
(����� ANSI C++ ǥ�� ���̺귯���� std::set���� ���� �ٸ� �̴ϴ�. �����ϼ���.) 
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
