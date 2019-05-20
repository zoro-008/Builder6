//---------------------------------------------------------------------------


#pragma hdrstop

#include "BaseClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//template<typename T>
//T    CValue::GetVal (     ){return *m_pValAdd;}//m_pValAdd==NULL이면 예외처리가 안된다. 사용주의
//
////template<typename T>
//void CValue::SetVal (T Val){*m_pValAdd = Val ;}



/*
//---------------------------------------------------------------------------

#ifndef BaseClassH
#define BaseClassH
#include <ValEdit.hpp>
#include <system.hpp>
#include <SysUtils.hpp>
#include <typeinfo>


//---------------------------------------------------------------------------
//TodoList
//TPoint 걷어내기.
//float 걷어내기.
//PKG List 등록시에 같은 이름 조사 하기.
//Gdi Scale 및 뷰어 확대 관련...... 정리..
//Gdi 영상 확대 하고 옮겼을때 이상 할것 이다.

//다이나믹 클래스 이름 등록 .....

//PkgList에 ValueList 가지고 있는것 점검.
//PKG 별 파라미터 리스트 자동으로 ValueEditor에 디스플레이 되도록.
//value에디터 콤보박스 추가.


//카메라 PKg 이동 금지 하게 수정... 다른 페키지가 인서트 및 Up했을 때 침범 하는것 수정. 카메라는 무조건 0이여야 된다.

class CBase
{
    public :
        CBase(){
            m_sName    = "";
            m_sComment = "";
        }
        virtual ~CBase(){

        }

    protected :
        String m_sName    ;
        String m_sComment ;

    public :

        String GetName   (){return m_sName   ;} void SetName   (String _sName   ){m_sName    = _sName   ;}
        String GetComment(){return m_sComment;} void SetComment(String _sComment){m_sComment = _sComment;}

};




//템플릿으로 제작 하면 공용으로 쓰질 못해서..
//
class CValue : public CBase
{
    public :
        enum EN_VAL_TYPE {
            vtNone = -1 ,
            vtString    ,
            vtAddress   , //벨류의 이름값.
            vtDouble    ,
            vtInt       ,
            vtEnum      ,
            vtBool      ,
        };
        CValue(){
            m_eValType = vtNone ;
            m_dMinVal  = 0 ;
            m_dMaxVal  = 0 ;

            m_sVal     = "";


        }
        virtual ~CValue(){
        }

    protected :
        //Property
        EN_VAL_TYPE   m_eValType ;
        double        m_dMinVal  ;
        double        m_dMaxVal  ;
        String        m_sSelList ; //콤보박스에 보여줄 리스트. ','가 구분자.

        //Value.
        String      m_sVal     ;




    public : //순수가상함수.

    public :
        //Property.
        EN_VAL_TYPE GetValType  (){return m_eValType;         }   void SetValType(EN_VAL_TYPE _eValType){m_eValType = _eValType;}
        double      GetMinVal   (){return m_dMinVal ;         }   void SetMinVal (double      _dMinVal ){m_dMinVal  = _dMinVal ;}
        double      GetMaxVal   (){return m_dMaxVal ;         }   void SetMaxVal (double      _dMaxVal ){m_dMaxVal  = _dMaxVal ;}
        void        SetProperty (String      _sName    ,
                                 String      _sComment ,
                                 EN_VAL_TYPE _eValType ,
                                 double      _dMinVal=0,
                                 double      _dMaxVal=0){
                                     m_sName    = _sName.Trim()    ;
                                     m_sComment = _sComment.Trim() ;
                                     m_eValType = _eValType ;
                                     m_dMinVal  = _dMinVal  ;
                                     m_dMaxVal  = _dMaxVal  ;
                                 }


        //ValueHandling.
        String      GetString (){return m_sVal    ;}
        String      GetAddress(){return m_sVal    ;}
        double      GetDouble (){return StrToFloatDef(m_sVal , 0.0);}
        int         GetInt    (){return StrToIntDef  (m_sVal , 0  );}
        int         GetEnum   (){
            int iRet = 0 ;
            iRet = StrToIntDef  (m_sVal , 0  );
            return iRet ;
        }
        bool        GetBool   (){return m_sVal != "False";}
        String *    GetStrPnt (){return &m_sVal   ;}

        void SetString (String _sVal){
            if(m_eValType != ::vtString) return ;
            m_sVal = _sVal.Trim();
        }

        void SetAddress(String _sVal){
            if(m_eValType != vtAddress) return ;
            m_sVal = _sVal.Trim();
        }
        void SetDouble (double _dVal){
            if(m_eValType != vtDouble) return ;
            if(m_dMinVal || m_dMaxVal) {
                if(m_dMinVal > _dVal) _dVal = m_dMinVal ;
                if(m_dMaxVal < _dVal) _dVal = m_dMaxVal ;
            }
            m_sVal = _dVal ;
        }
        void SetInt    (int    _iVal){
            if(m_eValType != vtInt) return ;
            if(m_dMinVal || m_dMaxVal) {
                if(m_dMinVal > _iVal) _iVal = m_dMinVal ;
                if(m_dMaxVal < _iVal) _iVal = m_dMaxVal ;
            }

            m_sVal = _iVal ;
        }
        void SetEnum   (int    _iVal){
            if(m_eValType != vtEnum) return ;
            if(             0         > _iVal) _iVal = 0         ;
            if(m_dMaxVal && m_dMaxVal < _iVal) _iVal = m_dMaxVal ;

            m_sVal = _iVal ;
        }
        void SetBool   (bool    _bVal){
            if(m_eValType != vtBool) return ;
            m_sVal = _bVal ? "True" : "False";
        }

        bool SetIntFromStr(String _sVal){
            if(m_eValType != vtInt) return false;
            if(_sVal.ToIntDef(0) != _sVal.ToIntDef(1)) return false ;
            SetInt(_sVal.ToIntDef(0));
            return true ;
        }

        bool SetEnumFromStr(String _sVal){
            if(m_eValType != vtEnum) return false;
            if(_sVal.ToIntDef(0) != _sVal.ToIntDef(1)) return false ;
            SetEnum(_sVal.ToIntDef(0));
            return true ;
        }

        bool SetDoubleFromStr(String _sVal){
            if(m_eValType != vtDouble) return false;
            if(StrToFloatDef(_sVal,0) != StrToFloatDef(_sVal,1)) return false ;
            SetDouble(StrToFloatDef(_sVal,0));
            return true ;
        }

        bool SetBoolFromStr(String _sVal){
            if(m_eValType != vtBool) return false;
                 if(_sVal.Trim() == "False")SetBool(false);
            else if(_sVal.Trim() == "FALSE")SetBool(false);
            else if(_sVal.Trim() == "false")SetBool(false);
            else if(_sVal.Trim() == "0"    )SetBool(false);
            else                            SetBool(StrToFloatDef(_sVal,1)!=0);
            return true ;
        }
};






#endif
*/
