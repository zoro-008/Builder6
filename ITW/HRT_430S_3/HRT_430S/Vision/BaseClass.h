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
        virtual ~CValue(){}

    protected :
        //Property
        EN_VAL_TYPE m_eValType ;
        double      m_dMinVal  ;
        double      m_dMaxVal  ;

        //Value.
        String      m_sVal     ;




    public : //순수가상함수.

    public :
        //Property.
        EN_VAL_TYPE GetValType  (){return m_eValType;         }   void SetValType(EN_VAL_TYPE _eValType){m_eValType = _eValType;}
        double      GetMinVal   (){return m_dMinVal ;         }   void SetMinVal (double      _dMinVal ){m_dMinVal  = _dMinVal ;}
        double      GetMaxVal   (){return m_dMaxVal ;         }   void SetMaxVal (double      _dMaxVal ){m_dMaxVal  = _dMaxVal ;}

        void        SetProperty(String      _sName    ,
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
        int         GetEnum   (){return StrToIntDef  (m_sVal , 0  );}
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
            if(m_eValType != vtInt) return false;
            if(_sVal.ToIntDef(0) != _sVal.ToIntDef(1)) return false ;
            SetInt(_sVal.ToIntDef(0));
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
            SetBool(StrToFloatDef(_sVal,1)!=0);
            return true ;
        }
};


//===================================================
/*아.. 안됀다. 다음에 고민하자...
typedef CBase * (__closure * FCreatePkg)(); //PKG 생성하는 함수
class CPkgStatic
{
    public:
        char m_pName[32];

        FCreatePkg m_fCreatePkg ;   //CBase* (*m_FpCreateObject)();
        CPkgStatic *m_pPrev;

        bool MatchName(char *cpName);

        CPkgStatic(char *cpName, FCreatePkg fCreatePkg);
        CBase *CreatePkg();
};
// Dynamic Class Macro
#define DECLARE_PKG_STATIC(Pkg_Name) \
  public: \
    static CPkgStatic Static_##Pkg_Name; \
    static CBase * CreatePkg(); \
    virtual CPkgStatic * GetStaticClass();

#define IMPLEMENT_PKG_STATIC(Pkg_Name) \
  CBase * Pkg_Name::CreatePkg() \
  { return (CBase*)new Pkg_Name; } \
  CPkgStatic Pkg_Name::Static_##Pkg_Name(#Pkg_Name, Pkg_Name::CreatePkg); \
  CPkgStatic * Pkg_Name::GetStaticClass() \
  { return &Static_##Pkg_Name; } \

// XGetDynamicClass(char *cpClassName)을 호출해서 해당 DynamicClss를 넘겨준다.
// DLL 함수이므로 EXE에서도 DLL Class를 사용할 수 있다.
//#define DYNAMICCLASS(class_name) XGetDynamicClass(#class_name)
*/




//Pop함수때 T형을 리턴 하고 싶으나 포인터 일경우 초기화가 되지 않은값이 날라가게 되어..
//그냥 Bool 리던으로 한다. 나중에 방법을 찾아보자.
//CLinkedList<int> LinkedList ;

template <typename T>
class CLinkedList
{
    private : //아직 확인 안한놈들. 나중에 확인 하고 Public으로 놓자.
        T & operator [] (int Pos) const
        {   T * RetData ;
            if (m_iDataCnt == 0  ) return * RetData;
            if (Pos     <  0     ) return * RetData;
            if (m_iDataCnt <= Pos) return * RetData;

            TNode * Crnt = m_pSttNode ;
            int i = 0 ;
            while(i<m_iDataCnt) {
               if(i == Pos ) {
                   RetData = &Crnt -> Data ;
                   return * RetData ;
               }
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return * RetData ;
        }


    private :
    //public :
        struct TNode  {
           T      Data     ;
           TNode *NextNode ;
           TNode (             )            { NextNode = NULL ; };
           TNode (const T& data): Data(data){ NextNode = NULL ; };
           ~TNode(             )            {                   };
        };

        int    m_iDataCnt ;
        TNode *m_pSttNode ;
        TNode *m_pEndNode ;
        //TNode *m_pTmpNode ;
        //TNode *pTmpNod ;

        int    m_iCrntNodeId ;
        TNode *m_pCrntNode   ;

    public :

        CLinkedList()
        {
           m_iDataCnt    = 0 ;
           m_iCrntNodeId = 0 ;
        }

        ~CLinkedList()
        {
           DeleteAll();
        }

        bool DeleteAll() //확인
        {
            if(m_iDataCnt == 0) return false ;
            TNode * Crnt = m_pSttNode ;
            TNode * pTmpNode ;
            for(int i = 0 ; i < m_iDataCnt ; i++) {
                pTmpNode = Crnt -> NextNode ;
                delete Crnt ;
                Crnt = pTmpNode ;
            }
            m_iDataCnt    = 0 ;
            m_iCrntNodeId = 0 ;

            return true ;
        }

        bool PushFrnt(T Data) //OK
        {
            TNode * pTmpNode = new TNode(Data) ;
            if (m_iDataCnt==0) {
                m_pSttNode = pTmpNode ;
                m_pEndNode = pTmpNode ;
                pTmpNode-> NextNode = NULL ;
            }
            else {
                pTmpNode -> NextNode = m_pSttNode ;
                m_pSttNode             = pTmpNode ;
            }
            m_iDataCnt++;

            return true ;
        };

        bool PushBack(T Data) //OK
        {
            TNode * pTmpNode = new TNode(Data) ;
            pTmpNode -> NextNode = NULL ;
            if (m_iDataCnt==0) {
                m_pSttNode = pTmpNode ;
                m_pEndNode = pTmpNode ;

            }
            else {
                m_pEndNode -> NextNode = pTmpNode;
                m_pEndNode             = pTmpNode;
            }
            m_iDataCnt++ ;

            return true ;
        };

        bool PopFrnt()
        {
            if (m_iDataCnt == 0) return false ;

            TNode * pTmpNode = m_pSttNode -> NextNode ;

            delete m_pSttNode ;
            m_pSttNode = pTmpNode ;
            m_iDataCnt-- ;

            return true ;
        };

        bool PopBack()
        {
            if (m_iDataCnt == 0) return false ;

            // 왜 이렇게 했지 m_pEndNode가 있는데......
            // 아.. Node가 NextNode만 있어서(SingleLinkedList) 그렇구나...나중에 PreNode도 만들자...
            TNode * Crnt = m_pSttNode ;
            TNode * pPreNode = NULL ;

            int i = 0 ;

            while(Crnt != NULL) {
               if(i + 1 == m_iDataCnt) { //맨마지막노드 찾기.
                  pPreNode -> NextNode = NULL ;
                  m_pEndNode = pPreNode ;
                  delete Crnt ;
                  m_iDataCnt-- ;
                  return true ;
               }
               pPreNode = Crnt ;
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return true ;


        };


        bool Insert(T Data, int Pos) //OK
        {
            if (Pos > m_iDataCnt ) return false;
            if (Pos == 0         ) { return PushFrnt(Data); } //이런부분 나중에 보완하자..
            if (Pos == m_iDataCnt) { return PushBack(Data); } //PushFront Back 다 없애고 인서트에서 다해결하게...

            TNode * pTmpNode = new TNode(Data) ;

            TNode *Crnt = m_pSttNode ;
            int  i      = 0          ;

            while(Crnt!=NULL){
                if (i + 1 == Pos){
                    pTmpNode -> NextNode = Crnt -> NextNode ;
                    Crnt -> NextNode = pTmpNode ;
                    m_iDataCnt++;
                    return true ;
                }
                Crnt =  Crnt -> NextNode ;
                i++;
            }
            return true;
        };



        bool Delete(int Pos)  //T를 리턴하고 싶지만 포인터의 경우 초기화가 안돼서 안됌.
        {
            //T RetData ; //여기도 문제 초기화 되지 않음.
            if (m_iDataCnt ==  0 ) return false;
            if (Pos        <   0 ) return false;
            if (m_iDataCnt <= Pos) return false;

            if (Pos == 0           ) { PopFrnt(); return false; } //PopFrnt를 의 리턴값을 리턴하고 싶지만 포인터의 경우 초기화가 안돼서 안됌.
            if (Pos == m_iDataCnt-1) { PopBack(); return false; } //포인터의 경우 일단 시간이 걸려도 GetData를 이용하여 해당포인터 반환받은다음 메모리Delete시키고
                                                               //Delete 함수 수행해야 한다.나중에 보완하자..
            TNode *Crnt = m_pSttNode ;
            TNode *_pTmpNode ;

            int i = 0 ;

            while(i < m_iDataCnt) {
               if(i + 1 == Pos) {
                  _pTmpNode = Crnt -> NextNode ;

                  Crnt -> NextNode = _pTmpNode -> NextNode ;

                  delete _pTmpNode ;    //여기서 원래 T형 받아놓고 리턴 하고 싶은데....
                  m_iDataCnt-- ;
                  return true ;
               }
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return false ;

        };

        int GetDataCnt()
        {
            return m_iDataCnt ;
        };

        T GetData(int Pos)
        {
            T RetData ;
            if (m_iDataCnt == 0  ) return RetData;
            if (Pos        <  0  ) return RetData;
            if (m_iDataCnt <= Pos) return RetData;

            TNode * Crnt = m_pSttNode ;
            int i = 0 ;
            while(i<m_iDataCnt) {
               if(i == Pos ) {
                   RetData = Crnt -> Data ;
                   return RetData ;
               }
               Crnt = Crnt -> NextNode ;
               i++ ;
            }
            return RetData ;
        };

        //왜 하다 말았지???
        //연산자 "=" 가 되는 객체만 사용가능함......  필요 없게 수정...
        bool  Move(int Pos1 , int Pos2) //Pos1 노드를 Pos2에 넣음.
        {
            if (m_iDataCnt == 0   ) return false;
            if (Pos1       <  0   ) return false;
            if (Pos2       <  0   ) return false;
            if (m_iDataCnt <= Pos1) return false;
            if (m_iDataCnt <= Pos2) return false;
            if (Pos1       == Pos2) return false;

            //Insert(T Data, int Pos) //OK

            int pos1 = (Pos1 < Pos2) ? Pos1 : Pos2 ;
            int pos2 = (Pos1 < Pos2) ? Pos2 : Pos1 ;

            Insert(GetData(pos1),pos2+1);
            Insert(GetData(pos2),pos1+1);
            Delete(pos1);
            Delete(pos1);
            return true ;
        }

        //빠른패턴을 위해 쓰면 좋다.   포문에서 GetCrntData(!i) 이렇게 붙이면 0번째에 첫번째 인덱스로 이동 하고 1부터는 이어서 진행.
        T GetCrntData(bool FromFirst)
        {
            T RetData ;
            if (m_iDataCnt == 0 ) return RetData ;   //여기 나중에 클리어 해서 리턴하는거 삽입.
            if (FromFirst) {
                RetData    = m_pSttNode -> Data  ;
                m_pCrntNode   = m_pSttNode -> NextNode ;
                m_iCrntNodeId = 0 ;
                return RetData ;
            }
            if (m_iDataCnt <  m_iCrntNodeId) return RetData ;

            RetData = m_pCrntNode -> Data ;
            m_iCrntNodeId ++ ;
            if(m_pCrntNode -> NextNode == NULL) m_pCrntNode = m_pSttNode ;
            else                                m_pCrntNode = m_pCrntNode -> NextNode ;
            return RetData ;
        };

        int GetCrntNodeId()
        {
            if (m_iDataCnt == 0         ) return -1 ;
            if (m_iDataCnt <  m_iCrntNodeId) return -1 ;
            return m_iCrntNodeId ;

        };



        //Overriding the Operator
        void operator = (CLinkedList<T>& rhs) {
            DeleteAll();

            for(int i = 0 ; i < rhs.m_iDataCnt ; i++) {
                PushBack(rhs.GetCrntData(!i));
            }
        }
};

//----------------------------------------------------------------------------
double __fastcall GetTime(void)
{
    LARGE_INTEGER liEndCounter,liFrequency  ;
    QueryPerformanceCounter  (&liEndCounter);
    QueryPerformanceFrequency(&liFrequency );

    return (((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000.0 );
}




#endif
