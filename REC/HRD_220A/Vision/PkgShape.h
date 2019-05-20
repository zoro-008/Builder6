//---------------------------------------------------------------------------

#ifndef PkgShapeH
#define PkgShapeH

#include "BasePkg.h"
//#include "aRect.h"
#include "UnitImage.h"
#include "UnitArea.h"
//
#include "BaseMaths.h"

#include "aBlob.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CShape_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CShape_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd   );
            DECLARATION(String,sRefOfsYAdd   );
            DECLARATION(String,sRefOfsTAdd   );
            DECLARATION(String,sRefAbsXAdd   );
            DECLARATION(String,sRefAbsYAdd   );

            DECLARATION(String,sOutCntOfsXAdd);
            DECLARATION(String,sOutCntOfsYAdd);
            DECLARATION(String,sOutAngleAdd  );

            DECLARATION(bool  ,bUseRoiRslt   );



            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd    ,"Ref OffsetX Add            " , "���ذ� offsetX �ּ�         ");
                PROPERTY(List , String,sRefOfsYAdd    ,"Ref OffsetY Add            " , "���ذ� offsetY �ּ�         ");
                PROPERTY(List , String,sRefOfsTAdd    ,"Ref Offset T Add           " , "���ذ� offsetT �ּ�         ");
                PROPERTY(List , String,sRefAbsXAdd    ,"Ref Absolute X Add         " , "���ذ� AbsoluteX �ּ�       ");
                PROPERTY(List , String,sRefAbsYAdd    ,"Ref Absolute Y Add         " , "���ذ� AbsoluteY �ּ�       ");

                PROPERTY(List , String,sOutCntOfsXAdd ,"Out Center OffsetX Add     " , "����� Center offsetX �ּ�  ");
                PROPERTY(List , String,sOutCntOfsYAdd ,"Out Center OffsetY Add     " , "����� Center offsetY �ּ�  ");
                PROPERTY(List , String,sOutAngleAdd   ,"Out Angle Add              " , "����� Angle �ּ�           ");

                PROPERTY(List , bool  ,bUseRoiRslt    ,"Result From ROI With NoInsp" , "�˻���� ROI�� ��ġ������� ");



                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";
                sRefOfsTAdd="";
                sRefAbsXAdd="";
                sRefAbsYAdd="";

                sOutCntOfsXAdd ="";
                sOutCntOfsYAdd ="";
                sOutAngleAdd   ="";

                bUseRoiRslt    =false ;
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
            DECLARATION(bool,bUseLstSqure  );

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
                PROPERTY(List , bool ,bUseLstSqure  ,"Use Least Squre             " , "�ּ��ڽ¹� ���            ");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk       = false ;
                iLnSampleGap  = 5 ;
                iLnLineGap    = 1 ;
                iLnEndOffset  = 0 ;
                iLnCntrOffset = 0 ;
                iCcSampleCnt  = 20;
                iEgAverage    = 1 ;
                iEgCompare    = 1 ;
                iEgChkPxLow   = 0 ;
                iEgChkPxHgh   = 0 ;
                iEgOfsGap     = 0 ;
                iEgOfsPxLow   = 0 ;
                iEgOfsPxHgh   = 0 ;
                bUseLstSqure  = true;
            }
            void operator =(const TMasterParaShape& rc)
            {
                bLtOnDk      = rc.bLtOnDk       ;
                iLnSampleGap = rc.iLnSampleGap  ;
                iLnLineGap   = rc.iLnLineGap    ;
                iLnEndOffset = rc.iLnEndOffset  ;
                iLnCntrOffset= rc.iLnCntrOffset ;
                iCcSampleCnt = rc.iCcSampleCnt  ;
                iEgAverage   = rc.iEgAverage    ;
                iEgCompare   = rc.iEgCompare    ;
                iEgChkPxLow  = rc.iEgChkPxLow   ;
                iEgChkPxHgh  = rc.iEgChkPxHgh   ;
                iEgOfsGap    = rc.iEgOfsGap     ;
                iEgOfsPxLow  = rc.iEgOfsPxLow   ;
                iEgOfsPxHgh  = rc.iEgOfsPxHgh   ;
                bUseLstSqure = rc.bUseLstSqure  ;
            }
        };

        struct TLocalParaPoly {
            CLinkedList<CValue2*> List ;
            DECLARATION(String , sLengths       );
            DECLARATION(String , sAngles        );
            DECLARATION(double , dLengTolerance );
            DECLARATION(double , dAngleTolerance);

            TLocalParaPoly(){
                PROPERTY(List , String , sLengths       ,"Lengths          " , "���� ���̵� ����  ");
                PROPERTY(List , String , sAngles        ,"sAngles          " , "���� ������ ����  ");
                PROPERTY(List , double , dLengTolerance ,"Length Tolerance " , "���� ���� ������"); VdLengTolerance  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , double , dAngleTolerance,"Angle Tolerance  " , "���� ���� ������"); VdAngleTolerance .SetMinMax(0,360);

                SetDefault();
            }
            void SetDefault(){
                sLengths        = "100,100,100,100";
                sAngles         = "90,90,90,90";
                dLengTolerance  = 0.0 ;
                dAngleTolerance = 0.0 ;
            }
            void operator =(const TLocalParaPoly& rc)
            {
                sLengths         = rc.sLengths       ;
                sAngles          = rc.sAngles        ;
                dLengTolerance   = rc.dLengTolerance ;
                dAngleTolerance  = rc.dAngleTolerance;
            }
        };


        //Parameter.
        struct TLocalParaCircle {
            CLinkedList<CValue2*> List ;
            DECLARATION(double , dRadian       );
            DECLARATION(double , dRadTolerance );
            DECLARATION(double , dStartAng     );
            DECLARATION(double , dEndAng       );


            TLocalParaCircle(){
                PROPERTY(List , double , dRadian      ,"Radian           " , "������          "); VdRadian  .SetMinMax(0,MAX_INT_VALUE);
                PROPERTY(List , double , dRadTolerance,"Radian Tolerance " , "������ ������ ");
                PROPERTY(List , double , dStartAng    ,"Start Angle      " , "ȣ �˻� ���۰���"); VdStartAng.SetMinMax(0,360);
                PROPERTY(List , double , dEndAng      ,"End Angle        " , "ȣ �˻� ������  "); VdEndAng  .SetMinMax(0,360);
                SetDefault();
            }
            void SetDefault(){
                dRadian       = 0.0 ;
                dRadTolerance = 0.0 ;
            }
            void operator =(const TLocalParaCircle& rc)
            {
                dRadian       = rc.dRadian      ;
                dRadTolerance = rc.dRadTolerance;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;

            TCommonPara(){

                SetDefault();
            }
            void SetDefault(){

            }
        };

        struct TTrackerRslt {
            bool      bRsltOk  ;

            CTracker::ETrackerType eTrackerType ;
            //����.
            int       iPointCnt ;
            TDPoint * pPoints   ; // �ٰ��� �ڳ� ����Ʈ �迭.
            TDPoint   tCntPnt   ; // ����Ʈ���� �߽�.
            double    dCntOfsX  ; //�߽ɿ����� �Ÿ�.
            double    dCntOfsY  ;
            //Circle��.
            double    dRadius   ;

            //�ٰ�����.
            double  * pLengths   ;
            double  * pAngles    ;

            CLinkedList<TDPoint> lEdgeList ;

            TTrackerRslt(){
                eTrackerType = CTracker::ttNone ;
                iPointCnt    = 0 ;
                pPoints      = NULL ;

                pLengths     = NULL ;
                pAngles      = NULL ;
            }
            ~TTrackerRslt(){
                Clear();
            }
            void Clear(){
                bRsltOk = false ;
                eTrackerType = CTracker::ttNone ;
                iPointCnt = 0 ;
                tCntPnt.Clear();
                dCntOfsX = 0;
                dCntOfsY = 0;
                if(pPoints) {
                    delete [] pPoints ;
                    pPoints = NULL;
                }
                if(pLengths) {
                    delete [] pLengths ;
                    pLengths = NULL;
                }
                if(pAngles) {
                    delete [] pAngles ;
                    pAngles = NULL;
                }

                lEdgeList.DeleteAll();
            }
        };

        struct TRslt {
            bool        bInspEnd ;
            bool        bRsltOk  ;

            int            iTrackerRsltCnt ;
            TTrackerRslt * pTrackerRslts   ;


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
            }
        };

        //Functions
        CShape_V01();
        ~CShape_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara      MPara      ;
        TMasterParaShape MParaShape ;
        TCommonPara      CPara      ;

        TRslt  Rslt ;

        CLinkedList<CPopupTracker   *> m_lTrckInsp   ;
        CLinkedList<TLocalParaCircle*> m_lLParaCircle;   //������ Ʈ��Ŀ�� ������ ���� �þ.
        CLinkedList<TLocalParaPoly  *> m_lLParaPoly  ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        //�˻� ���ÿ��� �̰��� �����Ķ� �������� �ʴ´�.
        CLinkedList<CPopupTracker *> m_lTrckIgnr ;

        TLocalParaCircle * m_pActiveLParaCircle;
        TLocalParaPoly   * m_pActiveLParaPoly  ;

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


