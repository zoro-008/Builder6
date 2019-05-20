//---------------------------------------------------------------------------

#ifndef PkgResultH
#define PkgResultH

#include "BasePkg.h"

#include "UnitArea.h"
#include "UnitImage.h"

#include "UnitGdi.h"
//---------------------------------------------------------------------------
//20150227 ����� ����.
class CResult_V01 : public CPkgBase
{
    REGIST_STATICLLINK_HEADER(CResult_V01)
    public  :

        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;
            TMasterPara(){
                SetDefault();
            }
            void SetDefault(){
            }
        };
        struct TCommonPara {
            CLinkedList<CValue2*> List ;
            DECLARATION(String,sResultMsg );
            TCommonPara(){
                PROPERTY(List , String,sResultMsg   ,"Result Msg" , "��� �޼��� ValueList�׸��� <>�� ǥ���Ѵ�.");
                SetDefault();
            }
            void SetDefault(){
                sResultMsg="";
            }
        };
        struct TLocalPara {
        };

        struct TRslt {
            bool   bInspEnd ;
            bool   bRsltOk  ;

            String sResultMsg ;


            TRslt(){
                Clear();
            }
            ~TRslt(){
                Clear();
            }
            void Clear(){
                bInspEnd   = false ;
                bRsltOk    = false ;
                sResultMsg = ""    ;
            }
        };

        //Functions
        CResult_V01();
        ~CResult_V01();



    //��PKG�� Ưȭ�� ���ο����� ���� ���� �Լ���.===============================
    private :
        TMasterPara  MPara ;
        TCommonPara  CPara ;

        TRslt  Rslt ;

        CLinkedList<CTracker*>   m_lTrckInsp ;
        CLinkedList<TLocalPara*> m_lLPara    ;   //������ Ʈ��Ŀ�� ������ ���� �þ.

    private :
        void OnTrackerChanged();

    public : //Result PKG������ �ִ� �ۺ�.
        String GetRsltMsg();

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


