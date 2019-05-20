//---------------------------------------------------------------------------

#ifndef PkgLineCrossPntH
#define PkgLineCrossPntH

#include "BasePkg.h"
#include "UnitImage.h"
#include "aLine.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
#define LINE_CROSSPNT_INSP_CNT 2
class CLineCrossPnt_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CLineCrossPnt_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(String,sRefOfsXAdd     );
            DECLARATION(String,sRefOfsYAdd     );

            DECLARATION(String,sOutOfsXAdd     );
            DECLARATION(String,sOutOfsYAdd     );
            DECLARATION(String,sOutOfsTAdd     );
            DECLARATION(String,sOutAbsXAdd     );
            DECLARATION(String,sOutAbsYAdd     );


            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd     ,"Ref OffsetX Add             " , "���ذ� offsetX �ּ�         ");
                PROPERTY(List , String,sRefOfsYAdd     ,"Ref OffsetY Add             " , "���ذ� offsetY �ּ�         ");

                PROPERTY(List , String,sOutOfsXAdd     ,"Out OffsetX Add             " , "����� offsetX �ּ�         ");
                PROPERTY(List , String,sOutOfsYAdd     ,"Out OffsetY Add             " , "����� offsetY �ּ�         ");
                PROPERTY(List , String,sOutOfsTAdd     ,"Out OffsetT Add             " , "����� Offset Angle �ּ�    ");

                PROPERTY(List , String,sOutAbsXAdd     ,"Out Absolute X Add          " , "����� Offset Angle �ּ�    ");
                PROPERTY(List , String,sOutAbsYAdd     ,"Out Absolute Y Add          " , "����� Offset Angle �ּ�    ");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd     = "";
                sRefOfsYAdd     = "";

                sOutOfsXAdd     = "";
                sOutOfsYAdd     = "";
                sOutOfsTAdd     = "";

                sOutAbsXAdd     = "";
                sOutAbsYAdd     = "";
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(double,dPosMax    );
            DECLARATION(double,dAngleMax  );
            DECLARATION(double,dOffsetX   );
            DECLARATION(double,dOffsetY   );


            TCommonPara(){
                PROPERTY(List , double,dPosMax      ,"Position Tolerance  " , "�̹��� ���Ϳ��� ������ ������ ");
                PROPERTY(List , double,dAngleMax    ,"Angle Tolerance     " , "������ ������  ");
                PROPERTY(List , double,dOffsetX     ,"Offset X            " , "������� �����V X");
                PROPERTY(List , double,dOffsetY     ,"Offset Y            " , "������� �����V Y");

                SetDefault();
            }
            void SetDefault(){
                dPosMax    = 0.0 ; //
                dAngleMax  = 0.0 ; //
                dOffsetX   = 0.0 ;
                dOffsetY   = 0.0 ;
            }
        };
        struct TLocalPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(bool  ,bLnLtToDk       );
            DECLARATION(int   ,iLnScanDirection);
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




            TLocalPara(){
                PROPERTY(List , bool  ,bLnLtToDk         ,"Line-Find Light To Dark     " , "��Ҵٰ� ��ο����� �� ã��" );

                PROPERTY(List , int   ,iLnScanDirection  ,"Line-Scan Direction         " , "���� �˻� ����              "); ViLnScanDirection.SetSelList("UpToDn,DnToUp,LeftToRight,RightToLeft");

                PROPERTY(List , int   ,iLnSampleGap      ,"Line-Sample Gap             " , "���� Edge ���ø� ����       ");
                PROPERTY(List , int   ,iLnLineGap        ,"Line-Line In Range          " , "���� ���� ����              ");
                PROPERTY(List , int   ,iLnEndOffset      ,"Line-Insp Offset Of End     " , "���� �˻� ���� ����         ");
                PROPERTY(List , int   ,iLnCntrOffset     ,"Line-Insp Offset Of Center  " , "�߰� �˻� ���� ����         ");

                PROPERTY(List , int   ,iEgAverage        ,"Edge-Average Range          " , "���� �����źȭ ����        ");
                PROPERTY(List , int   ,iEgCompare        ,"Edge-Diff Compare           " , "���� �� ����              ");
                PROPERTY(List , int   ,iEgChkPxLow       ,"Edge-Check Pixel Low        " , "���� ��ȿ �ּ� �Ӱ谪       ");
                PROPERTY(List , int   ,iEgChkPxHgh       ,"Edge-Check Pixel High       " , "���� ��ȿ �ִ� �Ӱ谪       ");
                PROPERTY(List , int   ,iEgOfsGap         ,"Edge-Check Offset Pixel Pos " , "���� ��ȿ �����V �Ÿ�       ");
                PROPERTY(List , int   ,iEgOfsPxLow       ,"Edge-Check Offset Pixel Low " , "���� ��ȿ �����V �ּ� �Ӱ谪");
                PROPERTY(List , int   ,iEgOfsPxHgh       ,"Edge-Check Offset Pixel High" , "���� ��ȿ �����V �ִ� �Ӱ谪");
                SetDefault();
            }

            void SetDefault(){
                bLnLtToDk    = false ;

                iLnScanDirection = CLine::lsUpToDn ;

                iLnSampleGap     = 5 ;
                iLnLineGap       = 1 ;
                iLnEndOffset     = 0 ;
                iLnCntrOffset    = 0 ;

                iEgAverage       = 1 ;
                iEgCompare       = 1 ;
                iEgChkPxLow      = 0 ;
                iEgChkPxHgh      = 0 ;
                iEgOfsGap        = 0 ;
                iEgOfsPxLow      = 0 ;
                iEgOfsPxHgh      = 0 ;

            }



        };

        struct TRslt {
            bool    bInspEnd ;
            bool    bRsltOk  ;

            double  dOffsetX ;
            double  dOffsetY ;
            double  dOffsetT ;

            double  dX ;
            double  dY ;
            double  dT ;

            CLine::TRslt tMainRslt ;
            CLine::TRslt tSubRslt  ;

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                dOffsetX = dX = 0.0 ;
                dOffsetY = dY = 0.0 ;
                dOffsetT = dT = 0.0 ;

                tMainRslt.Clear();
                tSubRslt .Clear();

            }
        };

        struct TOrig {
            double dPosX ;
            double dPosY ;
            double dAngle;
            TOrig(){
               dPosX  = 0 ;
               dPosY  = 0 ;
               dAngle = 0 ;

            }
        };

        //Functions
        CLineCrossPnt_V01();
        ~CLineCrossPnt_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TOrig  Orig ;
        TRslt  Rslt ;

        CLinkedList<CTracker   *> m_lTrckInsp ;
        CLinkedList<TLocalPara *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        TLocalPara * m_pActiveLPara ;


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


