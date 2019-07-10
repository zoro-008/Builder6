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
        //변수.
        bool   m_bRunning    ; //Running 중인지 확인 하는 플레그.
        String m_sErrMsg     ; //에러시 확인하는 메세지.

        CValueList * m_pValueList ; //비젼 벨류 테이블 리스트..


    public :
        bool   GetRunEnd       (            ){return m_bRunning;     } //다른 쓰레드에서 작업 종료 확인 할때 쓰는 것.
        void   SetValueList    (CValueList * _pValueList){m_pValueList = _pValueList ;}

    //순수 가상 함수 단=========================================================
    public :
        //클래스 네임 얻어오는 함수.
        virtual String GetPkgName ()=0;

        virtual bool Init()=0;
        virtual bool Close()=0;

        //러닝 관련.
        virtual bool   GetTrainAvailable()=0; //Train이 필요한 함수 인지 확인 한다. ex 메치 종류.
        virtual bool   Train            (CImage * _pImg)=0; //Train이 필요한 함수만 사용.
        virtual void   RsltClear        ()=0; //검사 결과값을 검사전에 클리어 한번 하고 한다.
        virtual bool   GetRslt          ()=0; //검사결과 true : OK , false : NG ;
        virtual bool   Run              (CImage * _pImg)=0; //작업.  ex) 카메라는 사진찍기 , 검사는 검사.
        virtual String GetErrMsg        ()=0;


        //화면 출력.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;

        //이미지 화면 클릭 이벤트
        virtual TTracker * MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual TTracker * MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void       MouseUp(TMouseButton Button,TShiftState Shift )=0;

        //파라미터 디스플레이
        virtual void UpdateMasterPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateCommonPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateLocalPara  (bool _bToTable , TValueListEditor * _vePara)=0;

        //로드 세이브.
        virtual void LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName )=0;
    //==========================================================================
};



#endif
