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
        virtual ~CPkgBase(){} //�Ҹ��� virtual.

        struct TProp { //��Ű���� Ư�̻��� ������Ƽ.
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
        //����.
        bool   m_bRunning    ; //Running ������ Ȯ�� �ϴ� �÷���.
        String m_sErrMsg     ; //������ Ȯ���ϴ� �޼���.
        //double m_dPxResol    ; //�ȼ� �������...
        bool   m_bSkip       ; //��ŵ����..

        CValueList * m_pValueList ; //���� ���� ���̺� ����Ʈ..

        TProp  Prop ; //PKG�� ����.

        FGetPxResol m_fpGetPxResol; //���� �������̳� �������̳� PxResol�� �������� �Լ�.
        FGetCalPos  m_fpGetCalPos ; //�̸��극�̼� ������ �������� �ݹ�.
        FOnPaint    m_fpOnPaint   ; //ȭ�� �׸��� �ݹ�.

    public :
        bool   GetRunEnd       (                        ){return m_bRunning;          } //�ٸ� �����忡�� �۾� ���� Ȯ�� �Ҷ� ���� ��.
        void   SetValueList    (CValueList * _pValueList){m_pValueList = _pValueList ;} //PKG�� �������� ���Ǵ� ����Ʈ.
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
                for(int i = 0 ; i < _pList->GetDataCnt() ; i++){
                    Value = _pList->GetCrntData(!i);
                         if(Value->GetValType()=="bool"  )_vePara -> Values[Value->GetName()] = Value->GetBoolStr();
                    else if(Value->GetValType()=="int"   ){
                        if(Value -> GetSelList() == "")   _vePara -> Values[Value->GetName()] = Value->GetInt    ();//int�̰� SelList������ Ȯ���ؾ���.. ���ڷ� �Ǵ��� �ƴϸ� Text�� �Ǵ���.
                        else                              _vePara -> Values[Value->GetName()] = Value->GetIntString();//int�̰� SelList������ Ȯ���ؾ���.. ���ڷ� �Ǵ��� �ƴϸ� Text�� �Ǵ���.

                    }
                    else if(Value->GetValType()=="double")_vePara -> Values[Value->GetName()] = Value->GetDouble ();
                    else if(Value->GetValType()=="String")_vePara -> Values[Value->GetName()] = Value->GetString ();
                    if(Value->GetSelList()!=""){
                        SetList(_vePara , iRowCount + i /*- 2*/ , Value->GetSelList()); //Ÿ��Ʋ ��� ���� �ؼ� -2 ��.
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
                        if(Value -> GetSelList() == "")   _vePara -> Values[Value->GetName()] = Value->GetInt    ();//int�̰� SelList������ Ȯ���ؾ���.. ���ڷ� �Ǵ��� �ƴϸ� Text�� �Ǵ���.
                        else                              _vePara -> Values[Value->GetName()] = Value->GetIntString();//int�̰� SelList������ Ȯ���ؾ���.. ���ڷ� �Ǵ��� �ƴϸ� Text�� �Ǵ���.

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

    //���� ���� �Լ� ��=========================================================
    public :
        virtual bool Init()=0;
        virtual bool Close()=0;

        //���� ����.
        //virtual bool   GetTrainAvailable()=0; //Train�� �ʿ��� �Լ� ���� Ȯ�� �Ѵ�. ex ��ġ ����.
        virtual bool   Train            (CImage * _pImg)=0; //Train�� �ʿ��� �Լ��� ���.
        virtual void   RsltClear        ()=0; //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
        virtual bool   GetRslt          ()=0; //�˻��� true : OK , false : NG ;
        virtual bool   Run              (CImage * _pImg)=0; //�۾�.  ex) ī�޶�� ������� , �˻�� �˻�.
        virtual String GetErrMsg        ()=0;


        //ȭ�� ���.
        virtual void PaintRslt        (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void PaintTracker     (HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;

        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
        virtual bool MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual bool MouseMv(                    TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)=0;
        virtual void MouseUp(TMouseButton Button,TShiftState Shift )=0;

        //�Ķ���� ���÷���
        virtual void UpdateMasterPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateCommonPara (bool _bToTable , TValueListEditor * _vePara)=0;
        virtual void UpdateLocalPara  (bool _bToTable , TValueListEditor * _vePara)=0;

        //�ε� ���̺�.
        virtual void LoadSave(bool _bLoad , String _sVisnName )=0;
    //==========================================================================
};

//����ƽ ��ũ...
typedef CPkgBase * (* FCreate)(          ); //PKG �����ϴ� �Լ�
class CStaticPkgLink
{
    protected: //���.
        char m_caPkgName[64];

        FCreate          m_fpCreate ;   //CBase* (*m_FpCreateObject)();
        CStaticPkgLink * m_pPrev    ;

        bool CheckName(char * _pName)
        {
            return !strcmp(m_caPkgName , _pName) ;
        }

    public: //������ �Ҹ���.
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

//��� ��Ű���� ��ũ�� �����ؾ� ��.
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