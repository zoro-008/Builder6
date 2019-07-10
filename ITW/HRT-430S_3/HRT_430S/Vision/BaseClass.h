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
        virtual ~CValue(){}

    protected :
        //Property
        EN_VAL_TYPE m_eValType ;
        double      m_dMinVal  ;
        double      m_dMaxVal  ;

        //Value.
        String      m_sVal     ;




    public : //���������Լ�.

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
/*��.. �ȉ´�. ������ �������...
typedef CBase * (__closure * FCreatePkg)(); //PKG �����ϴ� �Լ�
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

// XGetDynamicClass(char *cpClassName)�� ȣ���ؼ� �ش� DynamicClss�� �Ѱ��ش�.
// DLL �Լ��̹Ƿ� EXE������ DLL Class�� ����� �� �ִ�.
//#define DYNAMICCLASS(class_name) XGetDynamicClass(#class_name)
*/




//Pop�Լ��� T���� ���� �ϰ� ������ ������ �ϰ�� �ʱ�ȭ�� ���� �������� ���󰡰� �Ǿ�..
//�׳� Bool �������� �Ѵ�. ���߿� ����� ã�ƺ���.
//CLinkedList<int> LinkedList ;

template <typename T>
class CLinkedList
{
    private : //���� Ȯ�� ���ѳ��. ���߿� Ȯ�� �ϰ� Public���� ����.
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

        bool DeleteAll() //Ȯ��
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

            // �� �̷��� ���� m_pEndNode�� �ִµ�......
            // ��.. Node�� NextNode�� �־(SingleLinkedList) �׷�����...���߿� PreNode�� ������...
            TNode * Crnt = m_pSttNode ;
            TNode * pPreNode = NULL ;

            int i = 0 ;

            while(Crnt != NULL) {
               if(i + 1 == m_iDataCnt) { //�Ǹ�������� ã��.
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
            if (Pos == 0         ) { return PushFrnt(Data); } //�̷��κ� ���߿� ��������..
            if (Pos == m_iDataCnt) { return PushBack(Data); } //PushFront Back �� ���ְ� �μ�Ʈ���� ���ذ��ϰ�...

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



        bool Delete(int Pos)  //T�� �����ϰ� ������ �������� ��� �ʱ�ȭ�� �ȵż� �ȉ�.
        {
            //T RetData ; //���⵵ ���� �ʱ�ȭ ���� ����.
            if (m_iDataCnt ==  0 ) return false;
            if (Pos        <   0 ) return false;
            if (m_iDataCnt <= Pos) return false;

            if (Pos == 0           ) { PopFrnt(); return false; } //PopFrnt�� �� ���ϰ��� �����ϰ� ������ �������� ��� �ʱ�ȭ�� �ȵż� �ȉ�.
            if (Pos == m_iDataCnt-1) { PopBack(); return false; } //�������� ��� �ϴ� �ð��� �ɷ��� GetData�� �̿��Ͽ� �ش������� ��ȯ�������� �޸�Delete��Ű��
                                                               //Delete �Լ� �����ؾ� �Ѵ�.���߿� ��������..
            TNode *Crnt = m_pSttNode ;
            TNode *_pTmpNode ;

            int i = 0 ;

            while(i < m_iDataCnt) {
               if(i + 1 == Pos) {
                  _pTmpNode = Crnt -> NextNode ;

                  Crnt -> NextNode = _pTmpNode -> NextNode ;

                  delete _pTmpNode ;    //���⼭ ���� T�� �޾Ƴ��� ���� �ϰ� ������....
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

        //�� �ϴ� ������???
        //������ "=" �� �Ǵ� ��ü�� ��밡����......  �ʿ� ���� ����...
        bool  Move(int Pos1 , int Pos2) //Pos1 ��带 Pos2�� ����.
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

        //���������� ���� ���� ����.   �������� GetCrntData(!i) �̷��� ���̸� 0��°�� ù��° �ε����� �̵� �ϰ� 1���ʹ� �̾ ����.
        T GetCrntData(bool FromFirst)
        {
            T RetData ;
            if (m_iDataCnt == 0 ) return RetData ;   //���� ���߿� Ŭ���� �ؼ� �����ϴ°� ����.
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
