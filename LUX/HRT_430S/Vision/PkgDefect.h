//---------------------------------------------------------------------------

#ifndef PkgDefectH
#define PkgDefectH

#include "BasePkg.h"
//#include "aRect.h"
#include "UnitImage.h"
#include "UnitArea.h"
//
#include "BaseMaths.h"

#include "aBlob.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CDefect_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CDefect_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd      );
            DECLARATION(String,sRefOfsYAdd      );
            DECLARATION(String,sRefOfsTAdd      );
            DECLARATION(String,sRefAbsXAdd      );
            DECLARATION(String,sRefAbsYAdd      );
            DECLARATION(String,sOutDefectCntAdd );

            DECLARATION(bool  ,bUsePreEdge   );

            DECLARATION(int   ,iFrstMorpMeth );
            DECLARATION(int   ,iFrstMorpSize );
            DECLARATION(int   ,iScndMorpMeth );
            DECLARATION(int   ,iScndMorpSize );
            DECLARATION(int   ,iThrdMorpMeth );
            DECLARATION(int   ,iThrdMorpSize );

            DECLARATION(int   ,iInspMathod   );


            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd     ,"Ref OffsetX Add         " , "���ذ� offsetX �ּ�         ");
                PROPERTY(List , String,sRefOfsYAdd     ,"Ref OffsetY Add         " , "���ذ� offsetY �ּ�         ");
                PROPERTY(List , String,sRefOfsTAdd     ,"Ref Offset T Add        " , "���ذ� offsetT �ּ�         ");
                PROPERTY(List , String,sRefAbsXAdd     ,"Ref Absolute X Add      " , "���ذ� AbsoluteX �ּ�       ");
                PROPERTY(List , String,sRefAbsYAdd     ,"Ref Absolute Y Add      " , "���ذ� AbsoluteY �ּ�       ");
                PROPERTY(List , String,sOutDefectCntAdd,"Out DefectCnt Add       " , "Defect�� ���� �ּ�          ");

                PROPERTY(List , bool  ,bUsePreEdge     ,"Use Edge before Insp    " , "�y���� �������� �Ͽ� �˻�   ");

                PROPERTY(List , int   ,iFrstMorpMeth   ,"First Morphology Method " , "ù��° Morphology ���      "); ViFrstMorpMeth.SetSelList("None,Erosion,Dilation");
                PROPERTY(List , int   ,iFrstMorpSize   ,"First Morphology Size   " , "ù��° Morphology ũ��      "); ViFrstMorpSize.SetMinMax(1,100);
                PROPERTY(List , int   ,iScndMorpMeth   ,"Second Morphology Method" , "�ι�° Morphology ���      "); ViScndMorpMeth.SetSelList("None,Erosion,Dilation");
                PROPERTY(List , int   ,iScndMorpSize   ,"Second Morphology Size  " , "�ι�° Morphology ũ��      "); ViScndMorpSize.SetMinMax(1,100);
                PROPERTY(List , int   ,iThrdMorpMeth   ,"Third Morphology Method " , "����° Morphology ���      "); ViThrdMorpMeth.SetSelList("None,Erosion,Dilation");
                PROPERTY(List , int   ,iThrdMorpSize   ,"Third Morphology Size   " , "����° Morphology ũ��      "); ViThrdMorpSize.SetMinMax(1,100);

                PROPERTY(List , int   ,iInspMathod     ,"Inspection Mathod       " , "�˻� ���                   "); ViInspMathod  .SetSelList("Threshold,Edge");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";
                sRefOfsTAdd="";
                sRefAbsXAdd="";
                sRefAbsYAdd="";
                sOutDefectCntAdd = "";

                bUsePreEdge   = false ;

                iFrstMorpMeth = 0 ;
                iFrstMorpSize = 0 ;
                iScndMorpMeth = 0 ;
                iScndMorpSize = 0 ;
                iThrdMorpMeth = 0 ;
                iThrdMorpSize = 0 ;

                iInspMathod   = 0 ;

            }
        };

        //Parameter.
        struct TMasterParaShape {
            CLinkedList<CValue2*> List ;

            DECLARATION(bool,bLtOnDk       );
            DECLARATION(int ,iLnSampleGap  );
            DECLARATION(int ,iLnLineGap    );
            DECLARATION(int ,iLnEndOffset  );
            DECLARATION(int ,iLnCntrOffset );
            DECLARATION(int ,iCcSampleCnt  );
            DECLARATION(int ,iEgAverage    );
            DECLARATION(int ,iEgCompare    );
            DECLARATION(int ,iEgChkPxLow   );
            DECLARATION(int ,iEgChkPxHgh   );
            DECLARATION(int ,iEgOfsGap     );
            DECLARATION(int ,iEgOfsPxLow   );
            DECLARATION(int ,iEgOfsPxHgh   );

            TMasterParaShape(){
                PROPERTY(List , bool ,bLtOnDk       ,"Find Light On Dark          " , "���� �簢�� ã��            ");
                PROPERTY(List , int  ,iLnSampleGap  ,"Line-Sample Gap             " , "���� Edge ���ø� ����       ");
                PROPERTY(List , int  ,iLnLineGap    ,"Line-Line In Range          " , "���� ���� ����              ");
                PROPERTY(List , int  ,iLnEndOffset  ,"Line-Insp Offset Of End     " , "���� �˻� ���� ����         ");
                PROPERTY(List , int  ,iLnCntrOffset ,"Line-Insp Offset Of Center  " , "�߰� �˻� ���� ����         ");
                PROPERTY(List , int  ,iCcSampleCnt  ,"Circle-Insp Sample Count    " , "��Ŭ �˻��� ���ϴ� ���� ����");
                //PROPERTY(List , int  ,iEgAverage    ,"Edge-Average Range          " , "���� �����źȭ ����        ");
                //PROPERTY(List , int  ,iEgCompare    ,"Edge-Diff Compare           " , "���� �� ����              ");
                //PROPERTY(List , int  ,iEgChkPxLow   ,"Edge-Check Pixel Low        " , "���� ��ȿ �ּ� �Ӱ谪       ");
                //PROPERTY(List , int  ,iEgChkPxHgh   ,"Edge-Check Pixel High       " , "���� ��ȿ �ִ� �Ӱ谪       ");
                //PROPERTY(List , int  ,iEgOfsGap     ,"Edge-Check Offset Pixel Pos " , "���� ��ȿ �����V �Ÿ�       ");
                //PROPERTY(List , int  ,iEgOfsPxLow   ,"Edge-Check Offset Pixel Low " , "���� ��ȿ �����V �ּ� �Ӱ谪");
                //PROPERTY(List , int  ,iEgOfsPxHgh   ,"Edge-Check Offset Pixel High" , "���� ��ȿ �����V �ִ� �Ӱ谪");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk       = false ;
                iLnSampleGap  = 5 ;
                iLnLineGap    = 1 ;
                iLnEndOffset  = 0 ;
                iLnCntrOffset = 0 ;
                iEgAverage    = 1 ;
                iEgCompare    = 1 ;
                iEgChkPxLow   = 0 ;
                iEgChkPxHgh   = 0 ;
                iEgOfsGap     = 0 ;
                iEgOfsPxLow   = 0 ;
                iEgOfsPxHgh   = 0 ;
            }
            void operator =(const TMasterParaShape& rc)
            {
                bLtOnDk      = rc.bLtOnDk       ;
                iLnSampleGap = rc.iLnSampleGap  ;
                iLnLineGap   = rc.iLnLineGap    ;
                iLnEndOffset = rc.iLnEndOffset  ;
                iLnCntrOffset= rc.iLnCntrOffset ;
                iEgAverage   = rc.iEgAverage    ;
                iEgCompare   = rc.iEgCompare    ;
                iEgChkPxLow  = rc.iEgChkPxLow   ;
                iEgChkPxHgh  = rc.iEgChkPxHgh   ;
                iEgOfsGap    = rc.iEgOfsGap     ;
                iEgOfsPxLow  = rc.iEgOfsPxLow   ;
                iEgOfsPxHgh  = rc.iEgOfsPxHgh   ;
            }
        };

        struct TLocalPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(int   ,iInspOffset   );
            TLocalPara(){
                PROPERTY(List , int   ,iInspOffset    ,"Inspection Area Offset  " , "�˻� ���� �����V            ");

                SetDefault();
            }
            void SetDefault(){
                iInspOffset   = 0 ;

            }
            void operator =(const TLocalPara& rc)
            {
                iInspOffset = rc.iInspOffset ;

            }
        };


        //Parameter.
        struct TLocalParaPoly {
            CLinkedList<CValue2*> List ;

            TLocalParaPoly(){
                SetDefault();
            }
            void SetDefault(){
            }
            void operator =(const TLocalParaPoly& rc)
            {
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(int , iMinHeight     );
            DECLARATION(int , iMaxHeight     );
            DECLARATION(int , iMinWidth      );
            DECLARATION(int , iMaxWidth      );

            DECLARATION(int , dMinBoundary   );
            DECLARATION(int , dMaxBoundary   );
            DECLARATION(int , dMinCompact    );
            DECLARATION(int , dMaxCompact    );

            DECLARATION(int , iMinArea       );
            DECLARATION(int , iMaxArea       );
            DECLARATION(int , iMinTotalCnt   );

            TCommonPara(){
                PROPERTY(List , int ,iMinHeight    ,"Min Height     " , "�ּ� ����       "); ViMinHeight  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMaxHeight    ,"Max Height     " , "�ִ� ����       "); ViMaxHeight  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMinWidth     ,"Min Width      " , "�ּ� ����       "); ViMinWidth   .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMaxWidth     ,"Max Width      " , "�ִ� ����       "); ViMaxWidth   .SetMinMax(0,MAX_INT_VALUE);

                PROPERTY(List , int ,dMinBoundary  ,"Min Boundary   " , "�ּ� �׵θ� ����"); VdMinBoundary.SetMinMax(0,FLT_MAX);
                PROPERTY(List , int ,dMaxBoundary  ,"Max Boundary   " , "�ִ� �׵θ� ����"); VdMaxBoundary.SetMinMax(0,FLT_MAX);
                PROPERTY(List , int ,dMinCompact   ,"Min Compact    " , "�ּ� �е�       "); VdMinCompact .SetMinMax(0,FLT_MAX);
                PROPERTY(List , int ,dMaxCompact   ,"Max Compact    " , "�ִ� �е�       "); VdMaxCompact .SetMinMax(0,FLT_MAX);

                PROPERTY(List , int ,iMinArea      ,"Min Area       " , "�ּ� ����       "); ViMinArea    .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMaxArea      ,"Max Area       " , "�ִ� ����       "); ViMaxArea    .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , int ,iMinTotalCnt  ,"Min TotalCnt   " , "�ּ� �Ǵ� ����  "); ViMinTotalCnt.SetMinMax(0,MAX_INT_VALUE);

                SetDefault();
            }
            void SetDefault(){
                iMinHeight   = 0 ;
                iMaxHeight   = 0 ;
                iMinWidth    = 0 ;
                iMaxWidth    = 0 ;

                dMinBoundary = 0 ;
                dMaxBoundary = 0 ;
                dMinCompact  = 0 ;
                dMaxCompact  = 0 ;

                iMinArea     = 0 ;
                iMaxArea     = 0 ;
                iMinTotalCnt = 0 ;

            }
        };
        struct TCommonParaThres { //������� ��� �Ķ�.
            CLinkedList<CValue2*> List ;

            DECLARATION(int , iLowThreshold  );
            DECLARATION(int , iHighThreshold );

            TCommonParaThres(){
                PROPERTY(List , int ,iLowThreshold     ,"Low Threshold      " , "Threshold �ּҰ� "); ViLowThreshold .SetMinMax(0,256);
                PROPERTY(List , int ,iHighThreshold    ,"High Threshold     " , "Threshold ������ "); ViHighThreshold.SetMinMax(0,256);

                SetDefault();
            }
            void SetDefault(){
                iLowThreshold  = 0 ;
                iHighThreshold = 0 ;

            }
        };
        struct TCommonParaEdge {// ���� ��� �Ķ�.
            CLinkedList<CValue2*> List ;

            TCommonParaEdge(){
                SetDefault();
            }
            void SetDefault(){
            }
        };



        struct TTrackerRslt {
            CTracker::ETrackerType eTrackerType ;
            int       iRadius   ;
            int       iPointCnt ;
            TDPoint * pPoints   ; // �ٰ��� �ڳ� ����Ʈ �迭.
            CLinkedList<TDPoint> lLineEdgeList ;

            TTrackerRslt(){
                eTrackerType = CTracker::ttNone ;
                iPointCnt    = 0 ;
                pPoints      = NULL ;
            }
            ~TTrackerRslt(){
                Clear();
            }
            void Clear(){
                eTrackerType = CTracker::ttNone ;
                iPointCnt = 0 ;
                if(pPoints) {
                    delete [] pPoints ;
                    pPoints = NULL;
                }
                lLineEdgeList.DeleteAll();
            }
        };

        struct TRslt {
            bool        bInspEnd ;
            bool        bRsltOk  ;

            int         iDefectCnt ;

            int            iTrackerRsltCnt ;
            TTrackerRslt * pTrackerRslts   ;

            CArea       InspArea       ;
            CArea       DefectArea     ;

            CBlob::TRslt BlobRslt ;

            TRslt(){
                iTrackerRsltCnt = 0    ;
                pTrackerRslts   = NULL ;
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd     = false ;
                bRsltOk      = false ;

                iTrackerRsltCnt = 0 ;
                if(pTrackerRslts) {
                    delete [] pTrackerRslts ;
                    pTrackerRslts = NULL;
                }

                InspArea      .ClrArea();
                DefectArea    .ClrArea();
                BlobRslt      .Clear  ();
            }
        };

        //Functions
        CDefect_V01();
        ~CDefect_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara      MPara      ;
        TMasterParaShape MParaShape ;
        TCommonPara      CPara      ;
        TCommonParaThres CParaThres ;
        TCommonParaEdge  CParaEdge  ;

        TRslt  Rslt ;

        CLinkedList<CPopupTracker *> m_lTrckInsp ;
        CLinkedList<TLocalPara    *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.
        CLinkedList<TLocalParaPoly*> m_lLParaPoly;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        //�˻� ���ÿ��� �̰��� �����Ķ� �������� �ʴ´�.
        CLinkedList<CPopupTracker *> m_lTrckIgnr ;

        TLocalPara     * m_pActiveLPara     ;
        TLocalParaPoly * m_pActiveLParaPoly ;

        void OnTrackerMenuItemClick(EMenuItem _iItemNo , CPopupTracker* _pTracker) ;


    private :
        void OnTrackerChanged();

    public : //���� ���� �Լ� ��================================================
        virtual bool Init();
        virtual bool Close();

        //�˻� �� ���� ����.
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sVisnName);
};
#endif


