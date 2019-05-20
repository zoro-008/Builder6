//---------------------------------------------------------------------------
#ifndef aEdgeH
#define aEdgeH
//---------------------------------------------------------------------------
#include "UnitImage.h"
#include "BaseDefine.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------
class CEdge {
    public :
        struct TPara {
            TPara(){
                iAverage=1;
                iCompare=1;

                iChkPxLow = 0 ;
                iChkPxHgh = 0 ;
                iOfsGap   = 0 ;
                iOfsPxLow = 0 ;
                iOfsPxHgh = 0 ;

            }
            int  iAverage  ; //row �����͸� smoothing�� �������� ���� ��հ�
            int  iCompare  ; //avr�� ���� dif �����͸� �̾� ���� �� �ϴ� ������.

            int  iChkPxLow ; //Low�� High ���̿� �ִ� ������ ã�´�.
            int  iChkPxHgh ; //�Ѵ� 0 �̸� ��� ����.
            int  iOfsGap   ; //Gap��ŭ �̵��Ͽ� OfsPxLow High���̿� �ִ� ������ ã�´�.
            int  iOfsPxLow ; //Gap�� 0�̸� ��� ���Ѵ�.
            int  iOfsPxHgh ;

        } ;
        struct TRslt {
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                DkToLtPnt.Clear();
                LtToDkPnt.Clear();

                iDkToLtGap = 0 ;
                iLtToDkGap = 0 ;
                iDkToLtPx  = 0 ;
                iLtToDkPx  = 0 ;
                StartPnt.x = 0 ;
                StartPnt.y = 0 ;
                EndPnt  .x = 0 ;
                EndPnt  .y = 0 ;

                dInspTime = 0.0 ;
                sErrMsg   = ""  ; //�̳𤨹��� memset����.
            }
            TDPoint DkToLtPnt  ; //������� ����.    �����ȼ�.
            TDPoint LtToDkPnt  ; //��ο����� �y��.    �����ȼ�.

            int     iDkToLtGap ; //������� ���� ��.
            int     iLtToDkGap ; //��ο����� �y�� ��.
            int     iDkToLtPx  ; //������� ���� �ȼ���.
            int     iLtToDkPx  ; //��ο����� ���� �ȼ���.
            TPoint  StartPnt   ; //�˻���� ����Ʈ.
            TPoint  EndPnt     ; //�˻糡���� ����Ʈ.

            double  dInspTime  ; //�˻�ð�.
            String  sErrMsg    ;
        } ;
        //Functions
        CEdge();
        ~CEdge();

        static bool Inspect  (CImage * _pImg, TPoint  _tSttPnt , TPoint  _tEndPnt , TPara _tPara , TRslt * _pRslt);
};
//---------------------------------------------------------------------------
#endif