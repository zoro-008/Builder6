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

/*
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
}; */

//템플릿으로 제작 하면 공용으로 쓰질 못해서..
class CValue2 : public CBase
{
    public :
        CValue2(){
            m_sValType = "" ;
            m_dMinVal  = 0 ;
            m_dMaxVal  = 0 ;

            m_pValAdd  = NULL;

            m_sBinName = "";


        }
        virtual ~CValue2(){
        }

    protected :
        //Property
        String        m_sBinName ; //바이너리 네임.
        String        m_sValType ;
        double        m_dMinVal  ; //
        double        m_dMaxVal  ;
        String        m_sSelList ; //enum , bool ,일경우 콤보박스에 보여줄 리스트. ','가 구분자.

        //Value.
        void *        m_pValAdd  ; //Value Address.

    public :
        //Property.
        void *      GetValAdd   (){return m_pValAdd ; } void SetValAdd (void *      _pAdd    ){m_pValAdd  = _pAdd    ;}
        String      GetBinName  (){return m_sBinName; } void SetBinName(String      _sBinName){m_sBinName = _sBinName;}
        String      GetValType  (){return m_sValType; } void SetValType(String      _sValType){m_sValType = _sValType;}
        double      GetMinVal   (){return m_dMinVal ; } void SetMinVal (double      _dMinVal ){m_dMinVal  = _dMinVal ;}
        double      GetMaxVal   (){return m_dMaxVal ; } void SetMaxVal (double      _dMaxVal ){m_dMaxVal  = _dMaxVal ;}
        String      GetSelList  (){return m_sSelList; }
        void SetSelList(String _sSelList){
            if(_sSelList == "") return ;
            m_sSelList = _sSelList;
            int iMaxCnt = 1;
            while(_sSelList.Pos(",")){
                _sSelList.Delete(_sSelList.Pos(","),1);
                iMaxCnt++;
            }
            SetMinMax(0,iMaxCnt); //리스트를 등록하면 이넘화 된다.

        }
        void        SetMinMax   (double      _dMinVal=0,
                                 double      _dMaxVal=0){
                                     m_dMinVal  = _dMinVal  ;
                                     m_dMaxVal  = _dMaxVal  ;

                                 }

        void        SetProperty (String      _sBinName ,
                                 String      _sName    ,
                                 String      _sComment ){
                                     m_sBinName = _sBinName.Trim() ;
                                     m_sName    = _sName.Trim()    ;
                                     m_sComment = _sComment.Trim() ;
                                 }




        //Value Handleing.=================================================
        //ValueHandling.
        //"String" 이런 특정적인것이 들어가서 않좋음.
        String GetString(){
            if(m_pValAdd&&m_sValType=="String")return *((String *)m_pValAdd) ;
            return "";
        }
        double GetDouble(){
            if(m_pValAdd&&m_sValType=="double")return *((double *)m_pValAdd) ;
            return 0.0;
        }
        int GetInt(){
            if(m_pValAdd&&m_sValType=="int")return *((int *)m_pValAdd) ;
            return 0;
        }
        String GetIntString(){
            if(!m_pValAdd||m_sValType!="int" ||m_sSelList == "")return "";
            int iMaxCnt = 0;
            String sSelList = m_sSelList ;
            String sItem ;
            do{
                sItem = sSelList.SubString(1,sSelList.Pos(",")-1);
                if(iMaxCnt == GetInt()) {
                    return  sItem ;
                }
                sSelList.Delete(1,sSelList.Pos(","));
                iMaxCnt++;
            }while(sSelList.Pos(","));
            return  sSelList ;
        }
        bool GetBool(){
            if(m_pValAdd&&m_sValType=="bool")return *((bool *)m_pValAdd) ;
            return false;
        }
        String GetBoolStr(){
            if(m_pValAdd&&m_sValType=="bool")return *((bool *)m_pValAdd) ? "True" : "False";
            return "False";
        }

        void Set(String _Val){
            if(!m_pValAdd||m_sValType!="String") return ;
            *((String *)m_pValAdd) = _Val ;
        }

        void Set(double _Val){
            if(!m_pValAdd||m_sValType!="double") return ;
            if(m_dMinVal || m_dMaxVal) {
                if(m_dMinVal > _Val) _Val = m_dMinVal ;
                if(m_dMaxVal < _Val) _Val = m_dMaxVal ;
            }
            *((double *)m_pValAdd) = _Val ;
        }

        void Set(int _Val){
            if(!m_pValAdd||m_sValType!="int") return ;
            if(m_dMinVal || m_dMaxVal) {
                if(m_dMinVal > _Val) _Val = m_dMinVal ;
                if(m_dMaxVal < _Val) _Val = m_dMaxVal ;
            }
            *((int *)m_pValAdd) = _Val ;
        }
        void Set(bool _Val){
            if(!m_pValAdd||m_sValType!="bool") return ;
            *((bool *)m_pValAdd) = _Val ;
        }

        //스트링 변환 함수들.
        bool SetIntFromStr(String _sVal){
            if(!m_pValAdd||m_sValType!="int") return false;
            if(m_sSelList != ""){
                int iMaxCnt = 0;
                String sSelList = m_sSelList ;
                String sItem ;
                do{
                    sItem = sSelList.SubString(1,sSelList.Pos(",")-1);
                    if(sItem == _sVal) {
                        Set(iMaxCnt);
                        return true ;
                    }
                    sSelList.Delete(1,sSelList.Pos(","));
                    iMaxCnt++;
                }while(sSelList.Pos(","));
                Set(iMaxCnt);
            }
            else {
                Set(_sVal.ToIntDef(*((int *)m_pValAdd)));
            }
            return true ;
        }

        bool SetDoubleFromStr(String _sVal){
            if(!m_pValAdd||m_sValType!="double") return false;
            Set((double)StrToFloatDef(_sVal,*((double *)m_pValAdd)));
            return true ;
        }

        bool SetBoolFromStr(String _sVal){
            if(!m_pValAdd||m_sValType!="bool") return false;
                 if(_sVal.Trim() == "False")Set(false);
            else if(_sVal.Trim() == "FALSE")Set(false);
            else if(_sVal.Trim() == "false")Set(false);
            else                            Set(StrToIntDef(_sVal,1)!=0);
            return true ;
        }

};

/*
        //Parameter.
        struct TMasterPara {
            CLinkedList<CValue2*> List ;

            DECLARATION(String,sRefOfsXAdd     );

            String sRefOfsXAdd ; CValue2 VsRefOfsXAdd ;


            DECLARATION(String,sRefOfsYAdd     );

            TMasterPara(){
                PROPERTY(List , String,sRefOfsXAdd     ,"Ref OffsetX Add             " , "기준값 offsetX 주소         ");
                PROPERTY(List , String,sRefOfsYAdd     ,"Ref OffsetY Add             " , "기준값 offsetY 주소         ");
                SetDefault();
            }

            void SetDefault(){
                sRefOfsXAdd     = "";
                sRefOfsYAdd     = "";
            }
        };
*/

//PKG 클래스에서 파라미터 디파인 용.============================================
#define DECLARATION(_Type,_BinName)\
_Type _BinName ; CValue2 V##_BinName

//PKG 클래스에서 이니셜 용.
#define PROPERTY(_List , _Type,_BinName,_Name , _Comment)\
V##_BinName.SetProperty(#_BinName,_Name,_Comment);\
V##_BinName.SetValType(#_Type);\
V##_BinName.SetValAdd(&_BinName);\
if(!strcmp(#_Type , "bool"))V##_BinName.SetSelList("False,True");\
_List.PushBack(&V##_BinName)
//==============================================================================

/*
#define PROPERTY(_List , _Type,_BinName,_Name , _Comment)\
V##_BinName.SetProperty(_BinName,_Name,_Comment);\
V##_BinName.SetValType(#_Type);\
V##_BinName.SetValAdd(&_BinName);\
if(#_Type=="bool")V##_BinName.SetSelList("False,True");\
_List.PushBack(&V##_BinName)
*/






//iCamId .SetProperty("iCamId","Camera Id" , "카메라 Index" ); iCamId.SetValType("int");





#endif
