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
//#include "Tracker.h"
#include "UnitTracker.h"
//---------------------------------------------------------------------------
//void GetCalPos(double _dImgX , double _dImgY , double * _dWrdX , double * _dWrdY);
typedef void   (__closure * FGetCalPos )(double  , double  , double *  , double * ); //Calibration Pos callback
typedef double (__closure * FGetPxResol)(); //Calibration Pos callback
class CPkgBase : public CBase
{
    public :
        CPkgBase(){
            //m_dPxResol = 1.0 ;
            m_bRunning = false ;
            m_sErrMsg  = "" ;
            m_bSkip    = false ;

            m_fpGetPxResol = NULL;
            m_fpGetCalPos  = NULL;
            m_fpOnPaint    = NULL;

        }
        virtual ~CPkgBase(){} //소멸자 virtual.

        struct TProp { //페키지별 특이사항 프로퍼티.
            bool bUseTrain ;
            bool bCamera   ;
            bool bResult   ;
            TProp()
            {
                bUseTrain = false ;
                bCamera   = false ;
                bResult   = false ;
            }
        };

    protected :
        //변수.
        bool   m_bRunning    ; //Running 중인지 확인 하는 플레그.
        String m_sErrMsg     ; //에러시 확인하는 메세지.
        //double m_dPxResol    ; //픽셀 레졸루션...
        bool   m_bSkip       ; //스킵여부..

        CValueList * m_pValueList ; //비젼 벨류 테이블 리스트..

        TProp  Prop ; //PKG의 종류.

        FGetPxResol m_fpGetPxResol; //현제 심플켈이나 렌즈켈이나 PxResol을 가져오는 함수.
        FGetCalPos  m_fpGetCalPos ; //켈리브레이션 포지션 가져오는 콜백.
        FOnPaint    m_fpOnPaint   ; //화면 그리는 콜백.

    public :
        bool   GetRunEnd       (                        ){return m_bRunning;          } //다른 쓰레드에서 작업 종료 확인 할때 쓰는 것.
        void   SetValueList    (CValueList * _pValueList){m_pValueList = _pValueList ;} //PKG간 공용으로 사용되는 리스트.
        TProp  GetProperty     (                        ){return Prop; }
        void   SetSkip         (bool   _bSkip           ){m_bSkip = _bSkip;}
        bool   GetSkip         (                        ){return m_bSkip ;}

        void   SetGetPxResol   (FGetPxResol _fpGetPxResol) {m_fpGetPxResol = _fpGetPxResol ;}
        void   SetGetCalPos    (FGetCalPos  _fpGetCalPos ) {m_fpGetCalPos  = _fpGetCalPos  ;}
        void   SetOnPaint      (FOnPaint    _fpOnPaint   ) {m_fpOnPaint    = _fpOnPaint    ;}

        //ComboBoxList        ,
        void  SetList(TValueListEditor * _vePara , int _iRow , String _sList){
            TStringList * slList = new TStringList ;
            slList->CommaText = _sList ;

            _vePara ->ItemProps[_iRow]-> EditStyle = esPickList ;
            _vePara ->ItemProps[_iRow]-> PickList  = slList     ;

            delete slList ;
        }

        void UpdateListPara(bool _bToTable , TValueListEditor * _vePara , CLinkedList<CValue2*> * _pList){
            CValue2* Value ;
            if(_bToTable) {
                //_vePara -> Strings -> Clear() ;
                //_vePara -> DisplayOptions >> doColumnTitles ;
                //int iRowCount  = _vePara -> RowCount ;
                int iRowCount = _vePara -> Strings -> Count ;
                int iTemp ;
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  )_vePara -> Values[Value->GetName()] = Value->GetBoolStr();
                    else if(Value->GetValType()=="int"   ){
                        if(Value -> GetSelList() == "")   _vePara -> Values[Value->GetName()] = Value->GetInt    ();//int이고 SelList있을때 확인해야함.. 숫자로 되는지 아니면 Text로 되는지.
                        else                              _vePara -> Values[Value->GetName()] = Value->GetIntString();//int이고 SelList있을때 확인해야함.. 숫자로 되는지 아니면 Text로 되는지.

                    }
                    else if(Value->GetValType()=="double")_vePara -> Values[Value->GetName()] = Value->GetDouble ();
                    else if(Value->GetValType()=="String")_vePara -> Values[Value->GetName()] = Value->GetString ();
                    if(Value->GetSelList()!=""){
                        iTemp = _vePara -> Strings -> Count ;
                        SetList(_vePara , iRowCount + i /*- 2*/ , Value->GetSelList()); //타이틀 등등 고려 해서 -2 함.
                    }
                }
            }
            else {
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  ) Value->SetBoolFromStr  (_vePara -> Values[Value->GetName()]) ;
                    else if(Value->GetValType()=="int"   ) Value->SetIntFromStr   (_vePara -> Values[Value->GetName()]) ;
                    else if(Value->GetValType()=="double") Value->SetDoubleFromStr(_vePara -> Values[Value->GetName()]) ;
                    else if(Value->GetValType()=="String") Value->Set             (_vePara -> Values[Value->GetName()]) ;
                }
            }
        }

        void LoadSaveList(bool _bLoad , String _sIniPath , String _sSection , CLinkedList<CValue2*> * _pList){
            TUserINI Ini ;
            if(_bLoad) {
                Ini.Load(_sIniPath, _sSection , "m_bSkip", m_bSkip);


                CValue2* Value ;
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  )Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (bool  *)Value->GetValAdd());
                    else if(Value->GetValType()=="int"   )Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (int   *)Value->GetValAdd());
                    else if(Value->GetValType()=="double")Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (double*)Value->GetValAdd());
                    else if(Value->GetValType()=="String")Ini.Load(_sIniPath, _sSection , Value->GetBinName(), (String*)Value->GetValAdd());
                }
            }
            else {
                Ini.Save(_sIniPath, _sSection , "m_bSkip", m_bSkip);


                CValue2* Value ;
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  )Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (bool  *)Value->GetValAdd());
                    else if(Value->GetValType()=="int"   )Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (int   *)Value->GetValAdd());
                    else if(Value->GetValType()=="double")Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (double*)Value->GetValAdd());
                    else if(Value->GetValType()=="String")Ini.Save(_sIniPath, _sSection , Value->GetBinName(), (String*)Value->GetValAdd());
                }
            }
        }

        /*
        String GetActiveComment(TValueListEditor * _vePara, CLinkedList<CValue2*> * _pList){
            CValue2* Value ;
            if(_bToTable) {
                //_vePara -> Strings -> Clear() ;
                //_vePara -> DisplayOptions >> doColumnTitles ;
                //int iRowCount  = _vePara -> RowCount ;
                int iRowCount = _vePara -> Row ;
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  )_vePara -> Values[Value->GetName()] = Value->GetBoolStr();
                    else if(Value->GetValType()=="int"   ){
                        if(Value -> GetSelList() == "")   _vePara -> Values[Value->GetName()] = Value->GetInt    ();//int이고 SelList있을때 확인해야함.. 숫자로 되는지 아니면 Text로 되는지.
                        else                              _vePara -> Values[Value->GetName()] = Value->GetIntString();//int이고 SelList있을때 확인해야함.. 숫자로 되는지 아니면 Text로 되는지.

                    }
                    else if(Value->GetValType()=="double")_vePara -> Values[Value->GetName()] = Value->GetDouble ();
                    else if(Value->GetValType()=="String")_vePara -> Values[Value->GetName()] = Value->GetString ();
                    if(Value->GetSelList()!="")SetList(_vePara , iRowCount + i , Value->GetSelList());
                }
            }
            else {
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  ) Value->SetBoolFromStr  (_vePara -> Values[Value->GetName()]) ;
                    else if(Value->GetValType()=="int"   ) Value->SetIntFromStr   (_vePara -> Values[Value->GetName()]) ;
                    else if(Value->GetValType()=="double") Value->SetDoubleFromStr(_vePara -> Values[Value->GetName()]) ;
                    else if(Value->GetValType()=="String") Value->Set             (_vePara -> Values[Value->GetName()]) ;
                }
            }
        }*/

    //순수 가상 함수 단=========================================================
    public :
        virtual bool Init()=0;
        virtual bool Close()=0;

        //러닝 관련.
        //virtual bool   GetTrainAvailable()=0; //Train이 필요한 함수 인지 확인 한다. ex 메치 종류.
        virtual bool   Train            (CImage * _pImg)=0; //Train이 필요한 함수만 사용.
        virtual void   RsltClear        ()=0; //검사 결과값을 검사전에 클리어 한번 하고 한다.
        virtual bool   GetRslt          ()=0; //검사결과 true : OK , false : NG ;
        virtual bool   Run              (CImage * _pImg)=0; //작업.  ex) 카메라는 사진찍기 , 검사는 검사.
        virtual String GetErrMsg        ()=0;


        //화면 출력.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;

        //이미지 화면 클릭 이벤트
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void MouseUp(TMouseButton Button,TShiftState Shift )=0;

        //파라미터 디스플레이
        virtual void UpdateMasterPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateCommonPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateLocalPara  (bool _bToTable , TValueListEditor * _vePara)=0;

        //로드 세이브.
        virtual void LoadSave(bool _bLoad , String _sVisnName )=0;
    //==========================================================================
};

//스테틱 링크...
typedef CPkgBase * (* FCreate)(          ); //PKG 생성하는 함수
class CStaticPkgLink
{
    protected: //멤바.
        char m_caPkgName[64];

        FCreate          m_fpCreate ;   //CBase* (*m_FpCreateObject)();
        CStaticPkgLink * m_pPrev    ;

        bool CheckName(char * _pName)
        {
            return !strcmp(m_caPkgName , _pName) ;
        }

    public: //생성자 소멸자.
        CStaticPkgLink(char * _cpPkgName , FCreate _fpCreate)
        {
            strcpy(m_caPkgName, _cpPkgName);
            m_pPrev = m_pLastStaticPkg ;
            m_fpCreate = _fpCreate ;

            m_pLastStaticPkg = this ;
        }

    public:
        CPkgBase *CreatePkg()
        {
            if(m_fpCreate) {
                return  m_fpCreate();
            }
            return NULL ;
        }
        char * GetPkgName(){return m_caPkgName ;}
    public: //Class Linked List
        static int              m_iStaticPkgCnt  ;
        static CStaticPkgLink * m_pLastStaticPkg ;

        static CStaticPkgLink *GetStaticPkgLink(char *_caPkgName)
        {
            CStaticPkgLink *pStaticPkgLink = m_pLastStaticPkg;
            if(!pStaticPkgLink) return NULL ;
            do{
                if(strcmp(pStaticPkgLink->m_caPkgName, _caPkgName) == 0) return pStaticPkgLink;
                pStaticPkgLink = pStaticPkgLink->m_pPrev;
            }
            while(pStaticPkgLink != NULL);
            return NULL;
        }

        static String GetPkgList(char _cDelimiter)
        {
            CStaticPkgLink *pStaticPkgLink = m_pLastStaticPkg;
            String sList = "";

            if(pStaticPkgLink==NULL){
                return sList ;
            }

            sList =  pStaticPkgLink -> m_caPkgName ;

            while(pStaticPkgLink->m_pPrev != NULL){
                sList += _cDelimiter ;
                sList += pStaticPkgLink->m_pPrev->m_caPkgName ;
                pStaticPkgLink = pStaticPkgLink->m_pPrev;
            }
            return sList;
        }
};

//모든 페키지는 메크로 선언해야 함.
// Static Link Pkg Macro Header
#define REGIST_STATICLLINK_HEADER(_PkgName) \
    public: \
      static CStaticPkgLink StaticPkgLink_##_PkgName; \
      static CPkgBase *CreatePkg(); \
      virtual CStaticPkgLink *GetStaticPkgLink();


// Static Link Pkg Macro Cpp
#define REGIST_STATICLLINK_CPP(_PkgName) \
CPkgBase *_PkgName::CreatePkg() \
{   \
    return (CPkgBase*)new _PkgName; \
} \
CStaticPkgLink *_PkgName::GetStaticPkgLink() \
{ \
    return &StaticPkgLink_##_PkgName; \
} \
CStaticPkgLink _PkgName::StaticPkgLink_##_PkgName(#_PkgName, _PkgName::CreatePkg);

#endif
