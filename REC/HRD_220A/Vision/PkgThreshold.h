//---------------------------------------------------------------------------

#ifndef PkgThresholdH
#define PkgThresholdH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CThreshold_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CThreshold_V01)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sRefOfsXAdd );
            DECLARATION(String,sRefOfsYAdd );

            DECLARATION(String,sOutCntrdXAdd );
            DECLARATION(String,sOutCntrdYAdd );

            DECLARATION(String,sOutCntrdCntOfsXAdd );
            DECLARATION(String,sOutCntrdCntOfsYAdd );

            DECLARATION(bool  ,bOutOfsXInverse );
            DECLARATION(bool  ,bOutOfsYInverse );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd         ,"Ref OffsetX Add"     , "���ذ� offsetX �ּ�");
                PROPERTY(List , String,sRefOfsYAdd         ,"Ref OffsetY Add"     , "���ذ� offsetY �ּ�");

                PROPERTY(List , String,sOutCntrdXAdd       ,"Out CentroidX Add"   , "�����߽�X ��� �ּ�");
                PROPERTY(List , String,sOutCntrdYAdd       ,"Out CentroidY Add"   , "�����߽�Y ��� �ּ�");

                PROPERTY(List , String,sOutCntrdCntOfsXAdd ,"Out CentroidX Add"   , "�����߽�X �߽� offset ��� �ּ�");
                PROPERTY(List , String,sOutCntrdCntOfsYAdd ,"Out CentroidY Add"   , "�����߽�Y �߽� offset ��� �ּ�");

                PROPERTY(List , bool  ,bOutOfsXInverse     ,"Out OffsetX Inverse" , "����� offsetX ����");
                PROPERTY(List , bool  ,bOutOfsYInverse     ,"Out OffsetY Inverse" , "����� offsetY ����");

                SetDefault();
            }
            void SetDefault(){
                sRefOfsXAdd="";
                sRefOfsYAdd="";

                sOutCntrdXAdd="";      
                sOutCntrdYAdd="";

                sOutCntrdCntOfsXAdd="";
                sOutCntrdCntOfsYAdd="";

                bOutOfsXInverse = false ;
                bOutOfsYInverse = false ;

            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(double ,dPercent      ); //���� percent
            DECLARATION(int    ,iThresholdLow ); //���� ������.
            DECLARATION(int    ,iThresholdHigh); //���� ������.

            TCommonPara(){
                PROPERTY(List , double,dPercent       ,"Percent      " , "���ù����˻�OK����"); VdPercent      .SetMinMax(0,100);
                PROPERTY(List , int   ,iThresholdLow  ,"ThresholdLow " , "�˻� ������� Low "); ViThresholdLow .SetMinMax(0,255);
                PROPERTY(List , int   ,iThresholdHigh ,"ThresholdHigh" , "�˻� ������� High"); ViThresholdHigh.SetMinMax(0,255);
                SetDefault();
            }
            void SetDefault(){
                dPercent      =85.0 ;
                iThresholdLow =0    ;
                iThresholdHigh=255  ;
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bRsltOk  ;
            TRect  tRect    ;
            int    iPxCnt   ;
            double dAverage ;
            double dPercent ;

            double dCentroidX ;
            double dCentroidY ;

            double dCentroidOfsCntX ;
            double dCentroidOfsCntY ;

            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd = false ;
                bRsltOk  = false ;
                iPxCnt   = 0  ;
                dAverage = 0.0;
                dPercent = 0  ;
                tRect.left   = 0 ;
                tRect.right  = 0 ;
                tRect.top    = 0 ;
                tRect.bottom = 0 ;

                dCentroidX = 0.0;
                dCentroidY = 0.0;

                dCentroidOfsCntX = 0.0 ;
                dCentroidOfsCntY = 0.0 ;

            }
        };

        //Functions
        CThreshold_V01();
        ~CThreshold_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        CLinkedList<TPoint> m_lFailPoint  ;  //�˻� ����.

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


