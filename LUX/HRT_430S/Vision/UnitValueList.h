//---------------------------------------------------------------------------

#ifndef UnitValueListH
#define UnitValueListH
#include <ExtCtrls.hpp>
#include "BaseDefine.h"
#include "BaseClass.h"
#include "UserINI.h"


//공용 테이블. 클래스간 플렉서블 통신용.
class CValueList : public CBase
{
    public :
        CValueList();
        virtual ~CValueList();

    protected :
        CLinkedList<CValue2 *> m_lValue ;
        String m_sVisnName ;

    public :
        void Init();
        void Close();


        CValue2 * Add(String _sName    ,
                      String _sComment ,
                      String _sValType ,
                      double _dMinVal  ,
                      double _dMaxVal   );


        CValue2 * Insert(int    _iIdx     ,
                         String _sName    ,
                         String _sComment ,
                         String _sValType ,
                         double _dMinVal  ,
                         double _dMaxVal   );

        void Delete(int _iIdx );
        void DeleteAll();

        CLinkedList<CValue2 *> * GetList();

        CValue2 * GetValueByName(String _sName);

        void UpdateList(bool _bToTable , TStringGrid * _sgGrid);

        void LoadSave(bool _bLoad , String _sVisnName);
        void LoadSave(bool _bLoad);

};



#endif
