//---------------------------------------------------------------------------


#pragma hdrstop

#include "BaseClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//template<typename T>
//T    CValue::GetVal (     ){return *m_pValAdd;}//m_pValAdd==NULL�̸� ����ó���� �ȵȴ�. �������
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
//TPoint �Ⱦ��.
//float �Ⱦ��.
//PKG List ��Ͻÿ� ���� �̸� ���� �ϱ�.
//Gdi Scale �� ��� Ȯ�� ����...... ����..
//Gdi ���� Ȯ�� �ϰ� �Ű����� �̻� �Ұ� �̴�.

//���̳��� Ŭ���� �̸� ��� .....

//PkgList�� ValueList ������ �ִ°� ����.
//PKG �� �Ķ���� ����Ʈ �ڵ����� ValueEditor�� ���÷��� �ǵ���.
//value������ �޺��ڽ� �߰�.


//ī�޶� PKg �̵� ���� �ϰ� ����... �ٸ� ��Ű���� �μ�Ʈ �� Up���� �� ħ�� �ϴ°� ����. ī�޶�� ������ 0�̿��� �ȴ�.

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




//���ø����� ���� �ϸ� �������� ���� ���ؼ�..
//
class CValue : public CBase
{
    public :
        enum EN_VAL_TYPE {
            vtNone = -1 ,
            vtString    ,
            vtAddress   , //������ �̸���.
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
        String        m_sSelList ; //�޺��ڽ��� ������ ����Ʈ. ','�� ������.

        //Value.
        String      m_sVal     ;




    public : //���������Լ�.

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
