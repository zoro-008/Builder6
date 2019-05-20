//---------------------------------------------------------------------------

#ifndef PkgMatchH
#define PkgMatchH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CMatch_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CMatch_V01)
    public  :
        enum ETrainArea {
            otUnknown = 0 ,
            otNoInsp  = 1 ,
            otDkInsp  = 2 ,
            otLtInsp  = 3 ,
            otTemp    = 4 ,
            otInsp    = 5 ,
            MAX_TRAIN_AREA
        };

        enum ERsltArea {
            orNone    = 0 ,
            orInsp    = 1 ,
            MAX_RSLT_AREA
        };

        struct TPxPoint
        {
            int x,y;
            unsigned char px ;
            TPxPoint() {x=0;y=0;px=0;}
            TPxPoint(int _x, int _y , unsigned char _px) { x=_x; y=_y; px=_px;}
        };



        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(int   ,iTrThreshold);
            DECLARATION(int   ,iTrInspOft  );
            DECLARATION(int   ,iTrNoInspOft);
            DECLARATION(int   ,iTrInspFreq );
            DECLARATION(String,sRefOfsXAdd );
            DECLARATION(String,sRefOfsYAdd );
            DECLARATION(String,sOutOfsXAdd );
            DECLARATION(String,sOutOfsYAdd );

            TMasterPara(){
                PROPERTY(List , int   ,iTrThreshold,"Threashold          " , "�˻��Ӱ谪         ");
                PROPERTY(List , int   ,iTrInspOft  ,"Inspection Offset   " , "�˻翵���� �β�    ");
                PROPERTY(List , int   ,iTrNoInspOft,"NonInspection Offset" , "��˻翵���� �β�  ");
                PROPERTY(List , int   ,iTrInspFreq ,"Inspection Frequency" , "�˻��           ");
                PROPERTY(List , String,sRefOfsXAdd ,"Ref OffsetX Add     " , "���ذ� offsetX �ּ�");
                PROPERTY(List , String,sRefOfsYAdd ,"Ref OffsetY Add     " , "���ذ� offsetY �ּ�");
                PROPERTY(List , String,sOutOfsXAdd ,"Out OffsetX Add     " , "��� OffsetX �ּ�  ");
                PROPERTY(List , String,sOutOfsYAdd ,"Out OffsetY Add     " , "��� OffsetY �ּ�  ");
                SetDefault();
            }
            void SetDefault(){
                iTrThreshold = 0 ;
                iTrInspOft   = 2 ;
                iTrNoInspOft = 1 ;
                iTrInspFreq  = 1 ;
                sRefOfsXAdd  = "";
                sRefOfsYAdd  = "";
                sOutOfsXAdd  = "";
                sOutOfsYAdd  = "";
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(bool   ,bFindNg);
            DECLARATION(double ,dSinc  );
            TCommonPara(){
                PROPERTY(List , bool   ,bFindNg,"NG On Find     " , "ã������ NGó��");
                PROPERTY(List , double ,dSinc  ,"Find Match Rate" , "��ġ ��ũ����  ");
                SetDefault();
            }
            void SetDefault(){
                bFindNg=false;
                dSinc  =85.0;
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bRsltOk  ;
            double dCntPosX ;
            double dCntPosY ;
            int    iDkPxCnt ;
            int    iLtPxCnt ;
            double dSinc    ;
            TRect  tRect    ;
            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;
                dCntPosX = 0   ;
                dCntPosY = 0   ;
                iDkPxCnt = 0   ;
                iLtPxCnt = 0   ;
                dSinc    = 0.0 ;
                tRect.left   = 0 ;
                tRect.right  = 0 ;
                tRect.top    = 0 ;
                tRect.bottom = 0 ;
            }
        };

        struct TOrig {
            double dCntPosX ;
            double dCntPosY ;
            TOrig(){
               dCntPosX = 0 ;
               dCntPosY = 0 ;

            }
        };

        //Functions
        CMatch_V01();
        ~CMatch_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;
        TOrig  Orig ; //Ʈ���νÿ� ������� ���� �ؼ� ������ġ�� ��´�.

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        CArea  * m_pTrainArea ;
        CImage * m_pTrainImg  ;

    private :
        void OnTrackerChanged();
        //int  GetAutoThreshold(CImage * _pImg);
    //==========================================================================




    public : //���� ���� �Լ� ��================================================
        //Ŭ���� ���� ������ �Լ�.
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


