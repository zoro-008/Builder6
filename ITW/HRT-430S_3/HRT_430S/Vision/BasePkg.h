//---------------------------------------------------------------------------

#ifndef BasePkgH
#define BasePkgH
#include <ValEdit.hpp>
#include <system.hpp>
#include <SysUtils.hpp>
#include <typeinfo>

#include "BaseClass.h"
#include "UnitValueList.h"
#include "UnitImage.h"
#include "Tracker.h"
//---------------------------------------------------------------------------
class CPkgBase : public CBase
{
    public :
        CPkgBase(){
            m_bRunning    = false ;
            m_sErrMsg = "" ;
        }
        virtual ~CPkgBase(){}

    protected :
        //����.
        bool   m_bRunning    ; //Running ������ Ȯ�� �ϴ� �÷���.
        String m_sErrMsg     ; //������ Ȯ���ϴ� �޼���.

        CValueList * m_pValueList ; //���� ���� ���̺� ����Ʈ..


    public :
        bool   GetRunEnd       (            ){return m_bRunning;     } //�ٸ� �����忡�� �۾� ���� Ȯ�� �Ҷ� ���� ��.
        void   SetValueList    (CValueList * _pValueList){m_pValueList = _pValueList ;}

    //���� ���� �Լ� ��=========================================================
    public :
        //Ŭ���� ���� ������ �Լ�.
        virtual String GetPkgName ()=0;

        virtual bool Init()=0;
        virtual bool Close()=0;

        //���� ����.
        virtual bool   GetTrainAvailable()=0; //Train�� �ʿ��� �Լ� ���� Ȯ�� �Ѵ�. ex ��ġ ����.
        virtual bool   Train            (CImage * _pImg)=0; //Train�� �ʿ��� �Լ��� ���.
        virtual void   RsltClear        ()=0; //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ()=0; //�˻��� true : OK , false : NG ;
        virtual bool   Run              (CImage * _pImg)=0; //�۾�.  ex) ī�޶�� ������� , �˻�� �˻�.
        virtual String GetErrMsg        ()=0;


        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual TTracker * MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual TTracker * MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void       MouseUp(TMouseButton Button,TShiftState Shift )=0;

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateCommonPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateLocalPara  (bool _bToTable , TValueListEditor * _vePara)=0;

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName )=0;
    //==========================================================================
};



#endif
