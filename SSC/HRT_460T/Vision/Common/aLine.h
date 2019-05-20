//---------------------------------------------------------------------------
#ifndef aLineH
#define aLineH
//---------------------------------------------------------------------------
#include "UnitImage.h"
#include "aEdge.h"
#include "BaseDefine.h"

//
//

//---------------------------------------------------------------------------
// 2009.01.02
// SUN Kye Won
//---------------------------------------------------------------------------

class CLine {
    typedef void (__closure * FGetCalPos)(double  , double  , double *  , double * ); //Calibration Pos callback
    public :
        enum ELineScanDirect {
            lsUpToDn = 0 ,   //������ �Ʒ���
            lsDnToUp = 1 ,   //�Ʒ����� ����
            lsLtToRt = 2 ,   //���ʿ��� ����������
            lsRtToLt = 3 ,   //�����ʿ��� ��������
        };

        struct TPara {
            TPara(){
                iScanDirect =lsUpToDn;
                bLtToDk     =false;
                iSampleGap  =1;
                iLineGap    =1;
                iEndOffset  =0;
                iCntrOffset =0;
                bUseLstSqure=false ;
            }

            ELineScanDirect iScanDirect  ; //��Ʈ���� ������ ��ĵ�ϴ� ����
            bool            bLtToDk      ; //���������� ���������� ���� ���� �˻�.
            int             iSampleGap   ; //���� ���� ����
            int             iLineGap     ; //���� ���������� �����ϴ� ����.
            int             iEndOffset   ; //�˻��ϴ� �糡�� ���� �����V
            int             iCntrOffset  ; //�˻��ϴ� ��� ���� �����V
            bool            bUseLstSqure ; //�ּ� �ڽ¹� ��뿩��.

            CEdge::TPara EdgPara ;

        } ;
        struct TRslt {
            TRslt(){
                //pEdges = NULL ;
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){

                dA         = 0.0 ;
                dB         = 0.0 ;
                dAngle     = 0.0 ;
                StartPnt.x = 0   ;
                StartPnt.y = 0   ;
                EndPnt  .x = 0   ;
                EndPnt  .y = 0   ;

                EdgList.DeleteAll();
                //iEdgeCnt = 0 ;
                //if(pEdges) {
                //    delete [] pEdges ;
                //}


                dInspTime = 0.0 ;
                sErrMsg   = ""  ; //�̳𤨹��� memset����.
            }

            double  dA         ; //������ ����
            double  dB         ; //������ y����.
            double  dAngle     ; //������ ����. 0~180
            TDPoint StartPnt   ; //�˻���� ����Ʈ.
            TDPoint EndPnt     ; //�˻糡���� ����Ʈ.

            CLinkedList<TDPoint> EdgList ;
            //int       iEdgeCnt ;
            //TDPoint * pEdges   ;

            double  dInspTime  ; //�˻�ð�.
            String  sErrMsg    ;
        } ;
    private :
    protected :



    public :
        //Functions
        CLine();
        ~CLine();

    public  :
        //���� ���� ���簢������ �˻�.. �Ҽ��� ���� ���ؼ� ������.
        static bool Inspect  (CImage * _pImg  , TRect _tRect , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos = NULL );
        //�밢 ���簢������ �˻�.
        static bool Inspect  (CImage * _pImg  , TDPoint _tFrstStt , TDPoint _tFrstEnd , TDPoint _tLastStt , TDPoint _tLastEnd , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos = NULL );
};
//---------------------------------------------------------------------------
#endif