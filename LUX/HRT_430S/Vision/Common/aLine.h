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
            lsUpToDn = 0 ,   //위에서 아래로
            lsDnToUp = 1 ,   //아래에서 위로
            lsLtToRt = 2 ,   //왼쪽에서 오른쪽으로
            lsRtToLt = 3 ,   //오른쪽에서 왼쪽으로
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

            ELineScanDirect iScanDirect  ; //렉트안의 라인을 스캔하는 방향
            bool            bLtToDk      ; //밝은색에서 검은색으로 가는 라인 검색.
            int             iSampleGap   ; //엣지 검출 간격
            int             iLineGap     ; //선을 구성원으로 인정하는 간격.
            int             iEndOffset   ; //검색하는 양끝단 범위 오프셑
            int             iCntrOffset  ; //검색하는 가운데 범위 오프셑
            bool            bUseLstSqure ; //최소 자승법 사용여부.

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
                sErrMsg   = ""  ; //이놈ㄸ문에 memset못씀.
            }

            double  dA         ; //직선의 기울기
            double  dB         ; //직선의 y절편.
            double  dAngle     ; //직선의 각도. 0~180
            TDPoint StartPnt   ; //검사시작 포인트.
            TDPoint EndPnt     ; //검사끝나는 포인트.

            CLinkedList<TDPoint> EdgList ;
            //int       iEdgeCnt ;
            //TDPoint * pEdges   ;

            double  dInspTime  ; //검사시간.
            String  sErrMsg    ;
        } ;
    private :
    protected :



    public :
        //Functions
        CLine();
        ~CLine();

    public  :
        //수직 수평 직사각형에서 검사.. 소수점 연산 안해서 빠르다.
        static bool Inspect  (CImage * _pImg  , TRect _tRect , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos = NULL );
        //대각 직사각형에서 검사.
        static bool Inspect  (CImage * _pImg  , TDPoint _tFrstStt , TDPoint _tFrstEnd , TDPoint _tLastStt , TDPoint _tLastEnd , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos = NULL );
};
//---------------------------------------------------------------------------
#endif