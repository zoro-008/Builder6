//---------------------------------------------------------------------------

#ifndef PkgDualRectH
#define PkgDualRectH

#include "BasePkg.h"
#include "aRect.h"
#include "UnitImage.h"
#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.

#define DUAL_RECT_INSP_CNT 2
class CDualRect_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CDualRect_V01)
    public  :
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(String,sRefOfsXAdd    );
            DECLARATION(String,sRefOfsYAdd    );

            DECLARATION(int   ,iLnSampleGap   );
            DECLARATION(int   ,iLnLineGap     );
            DECLARATION(int   ,iLnEndOffset   );
            DECLARATION(int   ,iLnCntrOffset  );

            DECLARATION(int   ,iEgAverage     );
            DECLARATION(int   ,iEgCompare     );
            DECLARATION(int   ,iEgChkPxLow    );
            DECLARATION(int   ,iEgChkPxHgh    );
            DECLARATION(int   ,iEgOfsGap      );
            DECLARATION(int   ,iEgOfsPxLow    );
            DECLARATION(int   ,iEgOfsPxHgh    );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd    ,"Ref OffsetX Add             " , "���ذ� offsetX �ּ�         ");
                PROPERTY(List , String,sRefOfsYAdd    ,"Ref OffsetY Add             " , "���ذ� offsetY �ּ�         ");

                PROPERTY(List , int   ,iLnSampleGap   ,"Line-Sample Gap             " , "���� Edge ���ø� ����       ");
                PROPERTY(List , int   ,iLnLineGap     ,"Line-Line In Range          " , "���� ���� ����              ");
                PROPERTY(List , int   ,iLnEndOffset   ,"Line-Insp Offset Of End     " , "���� �˻� ���� ����         ");
                PROPERTY(List , int   ,iLnCntrOffset  ,"Line-Insp Offset Of Center  " , "�߰� �˻� ���� ����         ");

                PROPERTY(List , int   ,iEgAverage     ,"Edge-Average Range          " , "���� �����źȭ ����        ");
                PROPERTY(List , int   ,iEgCompare     ,"Edge-Diff Compare           " , "���� �� ����              ");
                PROPERTY(List , int   ,iEgChkPxLow    ,"Edge-Check Pixel Low        " , "���� ��ȿ �ּ� �Ӱ谪       ");
                PROPERTY(List , int   ,iEgChkPxHgh    ,"Edge-Check Pixel High       " , "���� ��ȿ �ִ� �Ӱ谪       ");
                PROPERTY(List , int   ,iEgOfsGap      ,"Edge-Check Offset Pixel Pos " , "���� ��ȿ �����V �Ÿ�       ");
                PROPERTY(List , int   ,iEgOfsPxLow    ,"Edge-Check Offset Pixel Low " , "���� ��ȿ �����V �ּ� �Ӱ谪");
                PROPERTY(List , int   ,iEgOfsPxHgh    ,"Edge-Check Offset Pixel High" , "���� ��ȿ �����V �ִ� �Ӱ谪");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd     = "";
                sRefOfsYAdd     = "";

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
            DECLARATION(double,dXOffset        );
            DECLARATION(double,dYOffset        );
            DECLARATION(double,dOffsetTolerance);

            DECLARATION(double,dAngleTolerance );

            TCommonPara(){
                PROPERTY(List , double,dXOffset        ,"Main-Sub OffsetX" , "Main Center���� Sub Center ������ X �Ÿ�"       );
                PROPERTY(List , double,dYOffset        ,"Main-Sub OffsetX" , "Main Center���� Sub Center ������ Y �Ÿ�"       );
                PROPERTY(List , double,dOffsetTolerance,"Offset Tolerance" , "Main Center���� Sub Center ������ �Ÿ� ������");
                PROPERTY(List , double,dAngleTolerance ,"Angle Tolerance " , "Main ���� Sub Rect ����"                        );
                SetDefault();
            }
            void SetDefault(){
                dXOffset         = 0.0 ;
                dYOffset         = 0.0 ;
                dOffsetTolerance = 0.0 ;
                dAngleTolerance  = 0.0 ;
            }

        };
        struct TLocalPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool  ,bLtOnDk        );
            DECLARATION(double,dWidth         );
            DECLARATION(double,dHeight        );
            DECLARATION(double,dTolerance     );


            TLocalPara(){
                PROPERTY(List , bool  ,bLtOnDk      ,"Find Light On Dark  " , "���� �簢�� ã��");
                PROPERTY(List , double,dWidth       ,"Rect Width          " , "�簢�� ����     ");
                PROPERTY(List , double,dHeight      ,"Rect Height         " , "�簢�� ����     ");
                PROPERTY(List , double,dTolerance   ,"Rect Size Tolerance " , "������ ������ ");

                SetDefault();
            }
            void SetDefault(){
                bLtOnDk    = false ;
                dWidth     = 0.0   ;
                dHeight    = 0.0   ;
                dTolerance = 0.0   ;

            }
        };

        struct TRslt {
            bool    bInspEnd ;
            bool    bRsltOk  ;

            double  dOffsetX ; //���� �簢�� T�� 0���� ���߰� ���� ���� �簢�� ���� X �����V.
            double  dOffsetY ; //���� �簢�� T�� 0���� ���߰� ���� ���� �簢�� ���� Y �����V.
            double  dOffsetT ; //���� �簢���� ���� �簢������ ���� ����.

            double  dLengthL[DUAL_RECT_INSP_CNT];
            double  dLengthT[DUAL_RECT_INSP_CNT];
            double  dLengthR[DUAL_RECT_INSP_CNT];
            double  dLengthB[DUAL_RECT_INSP_CNT];

            CRect::TRslt RectRslt[DUAL_RECT_INSP_CNT] ; //�����Ƽ� ��ũ�帮��Ʈ �Ⱦ�.

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;

                dOffsetX = 0.0 ;
                dOffsetY = 0.0 ;
                dOffsetT = 0.0 ;

                for(int i=0 ; i < DUAL_RECT_INSP_CNT ;i++){
                    dLengthL[i]= 0.0 ;
                    dLengthT[i]= 0.0 ;
                    dLengthR[i]= 0.0 ;
                    dLengthB[i]= 0.0 ;
                    RectRslt[i].Clear();
                }
            }
        };

        //Functions
        CDualRect_V01();
        ~CDualRect_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

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


