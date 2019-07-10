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
    public  :

        //Parameter.
        struct TMasterPara {
            CValue aRefOfsXAdd  ;
            CValue aRefOfsYAdd  ;

            TMasterPara(){
                aRefOfsXAdd  .SetProperty("RefOffsetXAdd      " , "���ذ� offsetX �ּ�" , CValue::vtAddress, 0 , 0  );
                aRefOfsYAdd  .SetProperty("RefOffsetYAdd      " , "���ذ� offsetY �ּ�" , CValue::vtAddress, 0 , 0  );
            }
        };
        struct TCommonPara {
            CValue bSkip         ; //�˻� ���� ����.
            CValue dPercent      ; //���� percent
            CValue iThresholdHigh; //���� ������.
            CValue iThresholdLow ; //���� ������.
            TCommonPara(){
                bSkip         .SetProperty("Skip         " , "�˻� ����         " , CValue::vtBool   , 0 , 0   );
                dPercent      .SetProperty("Percent      " , "���ù����˻�OK����" , CValue::vtDouble , 0 , 100 );
                iThresholdLow .SetProperty("ThresholdLow " , "�˻� ������� Low " , CValue::vtInt    , 0 , 255 );
                iThresholdHigh.SetProperty("ThresholdHigh" , "�˻� ������� High" , CValue::vtInt    , 0 , 255 );
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

            TRslt(){
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

        CLinkedList<TRectTracker*> m_lTrckInsp ;
        CLinkedList<TLocalPara  *> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

        CLinkedList<TPoint> m_lFailPoint  ;  //�˻� ����.

        String m_sCrntJobFilePath ;


    private :
        void OnTrackerChanged();

    public : //���� ���� �Լ� ��================================================
        //Ŭ���� ���� ������ �Լ�.
        virtual String GetPkgName(){return "CThreshold_V01";/*typeid(this).name();*/} //���߿� ¯����������... �ν��Ͻ� ���� ���¿��� Ŭ���� �̸� ����.

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


