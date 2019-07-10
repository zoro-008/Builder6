//---------------------------------------------------------------------------

#ifndef UnitValueListH
#define UnitValueListH
#include <ExtCtrls.hpp>
#include "BaseClass.h"
#include "UserINI.h"


//공용 테이블. 클래스간 플렉서블 통신용.
/*
enum EN_COMMON_VAL {
    cdOriginOffsetX  = 0 ,
    cdOriginOffsetY  = 1 ,
    cdOriginSinc     = 2 ,
    cbOriginRsltOk   = 3 ,
    cn004            = 4 ,
    cdMatchSinc      = 5 ,
    cbMatchRsltOk    = 6 ,
    cn007            = 7 ,
    cn008            = 8 ,
    cn009            = 9 ,
    cdCheckPxAvr     = 10,
    ciCheckPxCnt     = 11,
    cbCheckPxOk      = 12,
    cn013            = 13,
    cn014            = 14,

    MAX_GROBAL_VAL
};


        void InitCmmTable(){
            m_aCmmTable[cdOriginOffsetX].SetProperty("OriginOffsetX ", "Origin OffsetX " ,vtDouble );
            m_aCmmTable[cdOriginOffsetY].SetProperty("OriginOffsetY ", "Origin OffsetY " ,vtDouble );
            m_aCmmTable[cdOriginSinc   ].SetProperty("OriginSinc    ", "Origin Sinc    " ,vtDouble );
            m_aCmmTable[cbOriginRsltOk ].SetProperty("OriginRsltOk  ", "Origin RsltOk  " ,vtBool   );
            m_aCmmTable[cn004          ].SetProperty("004           ", "004            " ,vtNone   );
            m_aCmmTable[cdMatchSinc    ].SetProperty("MatchSinc     ", "Match Sinc     " ,vtDouble );
            m_aCmmTable[cbMatchRsltOk  ].SetProperty("MatchRsltOk   ", "Match RsltOk   " ,vtBool   );
            m_aCmmTable[cn007          ].SetProperty("007           ", "007            " ,vtNone   );
            m_aCmmTable[cn008          ].SetProperty("008           ", "008            " ,vtNone   );
            m_aCmmTable[cn009          ].SetProperty("009           ", "009            " ,vtNone   );
            m_aCmmTable[cdCheckPxAvr   ].SetProperty("CheckPxAvr    ", "CheckPx Average" ,vtDouble );
            m_aCmmTable[ciCheckPxCnt   ].SetProperty("CheckPxCnt    ", "CheckPx Count  " ,vtInt    );
            m_aCmmTable[cbCheckPxRsltOk].SetProperty("CheckPxOk     ", "CheckPx RsltOk " ,vtBool   );
            m_aCmmTable[cn013          ].SetProperty("013           ", "013            " ,vtNone   );
            m_aCmmTable[cn014          ].SetProperty("014           ", "014            " ,vtNone   );
        }

        CValue * GetCmmVal(String _sName){
            for(int i = 0 ; i < MAX_GROBAL_VAL ; i++) {
                if(m_aCmmTable[i].GetName() == _sName) return &m_aCmmTable[i] ;
            }
            return NULL ;
        }



*/
class CValueList : public CBase
{
    public :
        CValueList();
        virtual ~CValueList();

    protected :
        CLinkedList<CValue *> m_lValue ;
        String m_sCrntJobFilePath ;
        String m_sVisnName ;

    public :
        void Init();
        void Close();


        void Add(String              _sName    ,
                 String              _sComment ,
                 CValue::EN_VAL_TYPE _eValType );

        void Insert(int                 _iIdx     ,
                   String              _sName    ,
                   String              _sComment ,
                   CValue::EN_VAL_TYPE _eValType );

        void Delete(int _iIdx );
        void DeleteAll();

        CLinkedList<CValue *> * GetList() {return &m_lValue ;}

        CValue * GetValueByName(String _sName) {
            CValue * Value ;
            for(int i = 0 ; i < m_lValue.GetDataCnt() ; i++){
                Value = m_lValue.GetCrntData(!i);
                if(Value->GetName() == _sName) return Value ;
            }
            return NULL ;
        }



        //여기서 적당한 컨트롤 찾고 PKG베이스에 벨류리스트 포인터 및 포인터셑 함수 추가.
        void UpdateList(bool _bToTable , TStringGrid * _sgGrid);

        void LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName);
        void LoadSave(bool _bLoad);

};



#endif
