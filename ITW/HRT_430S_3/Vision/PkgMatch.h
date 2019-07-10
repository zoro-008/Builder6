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
    public  :
        enum EN_TRAIN_AREA {
            otUnknown = 0 ,
            otNoInsp  = 1 ,
            otDkInsp  = 2 ,
            otLtInsp  = 3 ,
            otTemp    = 4 ,
            otInsp    = 5 ,
            MAX_TRAIN_AREA
        };

        enum EN_RSLT_AREA {
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
            CValue iTrThreshold ; //�˻� �Ӱ谪
            CValue iTrInspOft   ;
            CValue iTrNoInspOft ;
            CValue iTrInspFreq  ; //�˻�󵵼� ������ �������� ������.
            CValue aRefOfsXAdd  ;
            CValue aRefOfsYAdd  ;
            CValue aOutOfsXAdd  ; // OffsetValue X
            CValue aOutOfsYAdd  ; // OffsetValue Y

            TMasterPara(){
                iTrThreshold .SetProperty("Threashold         " , "�˻��Ӱ谪         " , CValue::vtInt    , 0 , 255);
                iTrInspOft   .SetProperty("InspectionOffset   " , "�˻翵���� �β�    " , CValue::vtInt    , 0 , 10 );
                iTrNoInspOft .SetProperty("NonInspectionOffset" , "��˻翵���� �β�  " , CValue::vtInt    , 0 , 10 );
                iTrInspFreq  .SetProperty("InspectionFrequency" , "�˻��           " , CValue::vtInt    , 0 , 10 );
                aRefOfsXAdd  .SetProperty("RefOffsetXAdd      " , "���ذ� offsetX �ּ�" , CValue::vtAddress, 0 , 0  );
                aRefOfsYAdd  .SetProperty("RefOffsetYAdd      " , "���ذ� offsetY �ּ�" , CValue::vtAddress, 0 , 0  );
                aOutOfsXAdd  .SetProperty("OutOffsetXAdd      " , "��� OffsetX �ּ�  " , CValue::vtAddress, 0 , 0  );
                aOutOfsYAdd  .SetProperty("OutOffsetYAdd      " , "��� OffsetY �ּ�  " , CValue::vtAddress, 0 , 0  );
            }
        };
        struct TCommonPara {
            CValue bSkip       ; //�˻� ���� ����.
            CValue bFindNg      ; //�˻� ���� ã���� NG ó��.
            CValue dSinc        ; //OK/NG��ũ
            TCommonPara(){
                bSkip       .SetProperty("Skip                " , "�˻� ����        " , CValue::vtBool   , 0 , 0   );
                bFindNg     .SetProperty("NG On Find          " , "ã������ NGó��  " , CValue::vtBool   , 0 , 0   );
                dSinc       .SetProperty("Find Match Rate     " , "��ġ ��ũ����    " , CValue::vtDouble , 0 , 100 );
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

        CLinkedList<TOutBandRectTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara         *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        CArea  * m_pTrainArea ;
        CImage * m_pTrainImg  ;

        String m_sCrntJobFilePath ;


    private :
        void OnTrackerChanged();
        int  GetAutoThreshold(CImage * _pImg);
    //==========================================================================




    public : //���� ���� �Լ� ��================================================
        //Ŭ���� ���� ������ �Լ�.
        virtual String GetPkgName(){return "CMatch_V01";/*typeid(this).name();*/} //���߿� ¯����������... �ν��Ͻ� ���� ���¿��� Ŭ���� �̸� ����.

        virtual bool Init();
        virtual bool Close();

        //�˻� �� ���� ����.
        virtual bool   GetTrainAvailable();
        virtual bool   Train            (CImage * _pImg);
        virtual void   RsltClear        (); //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ();
        virtual bool   Run              (CImage * _pImg);
        virtual String GetErrMsg        ();

        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual TTracker * MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual TTracker * MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        virtual void       MouseUp(TMouseButton Button,TShiftState Shift );

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara);
        virtual void UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara);

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName);
};
#endif


