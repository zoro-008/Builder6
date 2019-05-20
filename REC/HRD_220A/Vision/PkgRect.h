//---------------------------------------------------------------------------

#ifndef PkgRectH
#define PkgRectH

#include "BasePkg.h"
#include "aRect.h"
#include "UnitImage.h"
#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CRect_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CRect_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd   );
            DECLARATION(String,sRefOfsYAdd   );
            DECLARATION(String,sRefOfsTAdd   );
            DECLARATION(String,sRefAbsXAdd   );
            DECLARATION(String,sRefAbsYAdd   );

            DECLARATION(String,sOutOfsXAdd     );
            DECLARATION(String,sOutOfsYAdd     );
            DECLARATION(String,sOutAngleAdd    );

            DECLARATION(bool  ,bOutOfsXInverse );
            DECLARATION(bool  ,bOutOfsYInverse );
            DECLARATION(bool  ,bOutAngleInverse);

            DECLARATION(int   ,iLnSampleGap    );
            DECLARATION(int   ,iLnLineGap      );
            DECLARATION(int   ,iLnEndOffset    );
            DECLARATION(int   ,iLnCntrOffset   );

            DECLARATION(int   ,iEgAverage      );
            DECLARATION(int   ,iEgCompare      );
            DECLARATION(int   ,iEgChkPxLow     );
            DECLARATION(int   ,iEgChkPxHgh     );
            DECLARATION(int   ,iEgOfsGap       );
            DECLARATION(int   ,iEgOfsPxLow     );
            DECLARATION(int   ,iEgOfsPxHgh     );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd    ,"Ref OffsetX Add              " , "���ذ� offsetX �ּ�         ");
                PROPERTY(List , String,sRefOfsYAdd    ,"Ref OffsetY Add              " , "���ذ� offsetY �ּ�         ");
                PROPERTY(List , String,sRefOfsTAdd    ,"Ref Offset T Add             " , "���ذ� offsetT �ּ�         ");
                PROPERTY(List , String,sRefAbsXAdd    ,"Ref Absolute X Add           " , "���ذ� AbsoluteX �ּ�       ");
                PROPERTY(List , String,sRefAbsYAdd    ,"Ref Absolute Y Add           " , "���ذ� AbsoluteY �ּ�       ");

                PROPERTY(List , String,sOutOfsXAdd     ,"Out OffsetX Add             " , "����� offsetX �ּ�         ");
                PROPERTY(List , String,sOutOfsYAdd     ,"Out OffsetY Add             " , "����� offsetY �ּ�         ");
                PROPERTY(List , String,sOutAngleAdd    ,"Out Angle Add               " , "����� Angle �ּ�           ");

                PROPERTY(List , bool  ,bOutOfsXInverse ,"Out OffsetX Inverse         " , "����� offsetX ����         ");
                PROPERTY(List , bool  ,bOutOfsYInverse ,"Out OffsetY Inverse         " , "����� offsetY ����         ");
                PROPERTY(List , bool  ,bOutAngleInverse,"Out Angle Inverse           " , "����� Angle ����           ");

                PROPERTY(List , int   ,iLnSampleGap    ,"Line-Sample Gap             " , "���� Edge ���ø� ����       ");
                PROPERTY(List , int   ,iLnLineGap      ,"Line-Line In Range          " , "���� ���� ����              ");
                PROPERTY(List , int   ,iLnEndOffset    ,"Line-Insp Offset Of End     " , "���� �˻� ���� ����         ");
                PROPERTY(List , int   ,iLnCntrOffset   ,"Line-Insp Offset Of Center  " , "�߰� �˻� ���� ����         ");

                PROPERTY(List , int   ,iEgAverage      ,"Edge-Average Range          " , "���� �����źȭ ����        ");
                PROPERTY(List , int   ,iEgCompare      ,"Edge-Diff Compare           " , "���� �� ����              ");
                PROPERTY(List , int   ,iEgChkPxLow     ,"Edge-Check Pixel Low        " , "���� ��ȿ �ּ� �Ӱ谪       ");
                PROPERTY(List , int   ,iEgChkPxHgh     ,"Edge-Check Pixel High       " , "���� ��ȿ �ִ� �Ӱ谪       ");
                PROPERTY(List , int   ,iEgOfsGap       ,"Edge-Check Offset Pixel Pos " , "���� ��ȿ �����V �Ÿ�       ");
                PROPERTY(List , int   ,iEgOfsPxLow     ,"Edge-Check Offset Pixel Low " , "���� ��ȿ �����V �ּ� �Ӱ谪");
                PROPERTY(List , int   ,iEgOfsPxHgh     ,"Edge-Check Offset Pixel High" , "���� ��ȿ �����V �ִ� �Ӱ谪");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";
                sRefOfsTAdd="";
                sRefAbsXAdd="";
                sRefAbsYAdd="";

                sOutOfsXAdd     = "";
                sOutOfsYAdd     = "";
                sOutAngleAdd    = "";

                bOutOfsXInverse = false ;
                bOutOfsYInverse = false ;
                bOutAngleInverse= false ; 

                iLnSampleGap    = 5 ;
                iLnLineGap      = 1 ;
                iLnEndOffset    = 0 ;
                iLnCntrOffset   = 0 ;

                iEgAverage      = 1 ;
                iEgCompare      = 1 ;
                iEgChkPxLow     = 0 ;
                iEgChkPxHgh     = 0 ;
                iEgOfsGap       = 0 ;
                iEgOfsPxLow     = 0 ;
                iEgOfsPxHgh     = 0 ;
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool  ,bLtOnDk        );
            DECLARATION(double,dWidth         );
            DECLARATION(double,dHeight        );
            DECLARATION(double,dTolerance     );

            DECLARATION(double,dPosMax        );
            DECLARATION(double,dAngleMax      );

            TCommonPara(){
                PROPERTY(List , bool  ,bLtOnDk      ,"Find Light On Dark  " , "���� �簢�� ã��");
                PROPERTY(List , double,dWidth       ,"Rect Width          " , "�簢�� ����     ");
                PROPERTY(List , double,dHeight      ,"Rect Height         " , "�簢�� ����     ");
                PROPERTY(List , double,dTolerance   ,"Rect Size Tolerance " , "������ ������ ");

                PROPERTY(List , double,dPosMax      ,"Position Tolerance  " , "�̹��� ���Ϳ��� ������ ������ ");
                PROPERTY(List , double,dAngleMax    ,"Angle Tolerance     " , "������ ������ ");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk    = false ;
                dWidth     = 0.0   ;
                dHeight    = 0.0   ;
                dTolerance = 0.0   ;

                dPosMax    = 0.0   ; //
                dAngleMax  = 0.0   ; //

            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool    bInspEnd   ;
            bool    bRsltOk    ;

            TDPoint CrossPntLT ; //������.
            TDPoint CrossPntRT ;
            TDPoint CrossPntLB ;
            TDPoint CrossPntRB ;
            TDPoint CenterPnt  ;

            double  dLengthL   ;
            double  dLengthT   ;
            double  dLengthR   ;
            double  dLengthB   ;

            double  dAngleL    ;
            double  dAngleT    ;
            double  dAngleR    ;
            double  dAngleB    ;
            double  dAngle     ;

            double  dOffsetX   ;
            double  dOffsetY   ;

            CLinkedList<TDPoint> lEdgeList ;

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                CrossPntLT.Clear();
                CrossPntRT.Clear();
                CrossPntLB.Clear();
                CrossPntRB.Clear();
                CenterPnt .Clear();

                dLengthL  = 0.0 ;
                dLengthT  = 0.0 ;
                dLengthR  = 0.0 ;
                dLengthB  = 0.0 ;

                dAngleL   = 0.0 ;
                dAngleT   = 0.0 ;
                dAngleR   = 0.0 ;
                dAngleB   = 0.0 ;
                dAngle    = 0.0 ;

                dOffsetX = 0.0 ;
                dOffsetY = 0.0 ;

                lEdgeList.DeleteAll();

            }
        };

        //Functions
        CRect_V01();
        ~CRect_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker   *> m_lTrckInsp ;
        CLinkedList<TLocalPara *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.


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


