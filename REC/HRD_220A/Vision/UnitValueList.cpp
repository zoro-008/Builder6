//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitValueList.h"
#include "UnitArea.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


CValueList::CValueList()
{


}
CValueList::~CValueList()
{

}


void CValueList::Init()
{
    m_sVisnName = "";
}

void CValueList::Close()
{
    DeleteAll();
}


CValue2 *  CValueList::Add(String _sName    ,
                           String _sComment ,
                           String _sValType ,
                           double _dMinVal  ,
                           double _dMaxVal  )
{

    CValue2 * Val = new CValue2();
    Val -> SetProperty(_sName.Trim() , _sName.Trim() , _sComment.Trim());
    Val -> SetMinMax(_dMinVal , _dMaxVal);
         if(_sValType == "bool"  ){Val -> SetValAdd(new bool  ); Val -> SetValType("bool"  );}
    else if(_sValType == "int"   ){Val -> SetValAdd(new int   ); Val -> SetValType("int"   );}
    else if(_sValType == "double"){Val -> SetValAdd(new double); Val -> SetValType("double");}
    else if(_sValType == "String"){Val -> SetValAdd(new String); Val -> SetValType("String");}
    else if(_sValType == "CArea" ){Val -> SetValAdd(new CArea ); Val -> SetValType("CArea" );}

    m_lValue.PushBack(Val);

    return Val ;
}

CValue2 *  CValueList::Insert(int    _iIdx     ,
                              String _sName    ,
                              String _sComment ,
                              String _sValType ,
                              double _dMinVal  ,
                              double _dMaxVal  )
{

    CValue2 * Val = new CValue2();
    Val -> SetProperty(_sName.Trim() , _sName.Trim() , _sComment.Trim());
    Val -> SetMinMax(_dMinVal , _dMaxVal);
         if(_sValType == "bool"  ){Val -> SetValAdd(new bool  ); Val -> SetValType("bool"  );}
    else if(_sValType == "int"   ){Val -> SetValAdd(new int   ); Val -> SetValType("int"   );}
    else if(_sValType == "double"){Val -> SetValAdd(new double); Val -> SetValType("double");}
    else if(_sValType == "String"){Val -> SetValAdd(new String); Val -> SetValType("String");}
    else if(_sValType == "CArea" ){Val -> SetValAdd(new CArea ); Val -> SetValType("CArea" );}

    m_lValue.Insert(Val , _iIdx);

    return Val ;
}

void CValueList::Delete(int _iIdx )
{
    CValue2 * Val = m_lValue.GetData(_iIdx) ;
         if(Val->GetValType() == "bool"  ){delete (bool   *)Val -> GetValAdd(); }
    else if(Val->GetValType() == "int"   ){delete (int    *)Val -> GetValAdd(); }
    else if(Val->GetValType() == "double"){delete (double *)Val -> GetValAdd(); }
    else if(Val->GetValType() == "String"){delete (String *)Val -> GetValAdd(); }
    else if(Val->GetValType() == "CArea" ){delete (CArea  *)Val -> GetValAdd(); }
    else                                  {delete           Val -> GetValAdd(); }

    delete Val ;

    m_lValue.Delete(_iIdx);
}

void CValueList::DeleteAll()
{
    int iCnt = m_lValue.GetDataCnt();
    CValue2 * Val  ;
    for(int i = 0 ; i < iCnt ; i++) {
        Val = m_lValue.GetCrntData(!i);
             if(Val->GetValType() == "bool"  ){delete (bool   *)Val -> GetValAdd(); }
        else if(Val->GetValType() == "int"   ){delete (int    *)Val -> GetValAdd(); }
        else if(Val->GetValType() == "double"){delete (double *)Val -> GetValAdd(); }
        else if(Val->GetValType() == "String"){delete (String *)Val -> GetValAdd(); }
        else if(Val->GetValType() == "CArea" ){delete (CArea  *)Val -> GetValAdd(); }
        else                                  {delete           Val -> GetValAdd(); }

        delete Val ;
    }

    m_lValue.DeleteAll();
}

CLinkedList<CValue2 *> * CValueList::GetList()
{
    return &m_lValue ;
}

CValue2 * CValueList::GetValueByName(String _sName)
{
    CValue2 * Value ;
    for(int i = 0 ; i < m_lValue.GetDataCnt() ; i++){
        Value = m_lValue.GetCrntData(!i);
        if(Value->GetName() == _sName) return Value ;
    }
    return NULL ;
}

void CValueList::UpdateList(bool _bToTable , TStringGrid * _sgGrid)
{
    _sgGrid -> ColCount = 7 ;

    _sgGrid -> Cells[0][0] = "NO"       ;
    _sgGrid -> Cells[1][0] = "NAME"     ;
    _sgGrid -> Cells[2][0] = "COMMENT"  ;
    _sgGrid -> Cells[3][0] = "TYPE"     ;
    _sgGrid -> Cells[4][0] = "MIN VALUE";
    _sgGrid -> Cells[5][0] = "MAX VALUE";
    _sgGrid -> Cells[6][0] = "VALUE"    ;


    if(_bToTable) {

        if(m_lValue.GetDataCnt() + 1 < 2) {
            _sgGrid -> RowCount = 2 ;
            for (int i = 0 ;i < _sgGrid -> RowCount ; i++) _sgGrid -> Rows[i] -> Clear();
        }
        else {
            _sgGrid -> RowCount = m_lValue.GetDataCnt() + 1  ;
        }

        String sValType ;
        CValue2 * Val ;

        for (int i = 0 ;i < m_lValue.GetDataCnt() ; i++) {
            Val = m_lValue.GetCrntData(!i) ;
            _sgGrid -> Cells[0][i+1] = i ;
            _sgGrid -> Cells[1][i+1] = Val->GetName()   ;
            _sgGrid -> Cells[2][i+1] = Val->GetComment();
            _sgGrid -> Cells[3][i+1] = Val->GetValType();
            _sgGrid -> Cells[4][i+1] = Val->GetMinVal() ;
            _sgGrid -> Cells[5][i+1] = Val->GetMaxVal() ;
                 if(Val->GetValType() == "bool"  )_sgGrid -> Cells[6][i+1] = *((short  *)Val->GetValAdd()) ;
            else if(Val->GetValType() == "int"   )_sgGrid -> Cells[6][i+1] = *((int    *)Val->GetValAdd()) ;
            else if(Val->GetValType() == "double")_sgGrid -> Cells[6][i+1] = *((double *)Val->GetValAdd()) ;
            else if(Val->GetValType() == "String")_sgGrid -> Cells[6][i+1] = *((String *)Val->GetValAdd()) ;
        }
    }
    else {
        //메모리에 쓰는것은 여기서 하지 않는다...
    }

}

void CValueList::LoadSave(bool _bLoad , String _sVisnName)
{

    String sIniPath  = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + ".ini" ;".ini" ;

    TUserINI Ini ;
    int iCnt ;
    CValue2 * Val ;

    String sName    ;
    String sComment ;
    String sValType ;
    double dMinVal  ;
    double dMaxVal  ;

    if(_bLoad) {
        //원래 있던놈들 메모리와 포인터  제거.
        DeleteAll();
        Ini.Load(sIniPath, "Total" , "Count" , iCnt);
        for(int i=0; i < iCnt ; i++) {
            //Val = new CValue2();
            Ini.Load(sIniPath, String(i) , "sName   ", sName    );
            Ini.Load(sIniPath, String(i) , "sComment", sComment ); 
            Ini.Load(sIniPath, String(i) , "sValType", sValType );
            Ini.Load(sIniPath, String(i) , "dMinVal ", dMinVal  ); 
            Ini.Load(sIniPath, String(i) , "dMaxVal ", dMaxVal  );

            Val = Add(sName    ,
                      sComment ,
                      sValType ,
                      dMinVal  ,
                      dMaxVal  );
            if(Val) {
                     if(Val->GetValType() == "bool"  )Ini.Load(sIniPath, String(i) , "Val", (bool   *)Val->GetValAdd() );
                else if(Val->GetValType() == "int"   )Ini.Load(sIniPath, String(i) , "Val", (int    *)Val->GetValAdd() );
                else if(Val->GetValType() == "double")Ini.Load(sIniPath, String(i) , "Val", (double *)Val->GetValAdd() );
                else if(Val->GetValType() == "String")Ini.Load(sIniPath, String(i) , "Val", (String *)Val->GetValAdd() );
                else if(Val->GetValType() == "CArea" )((CArea *)Val->GetValAdd())->LoadBmp((GM.GetJobFileFolder() + _sVisnName + "_" + sName + "_Area.bmp").c_str());
            }
        }
    }
    else {
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath ) ;

        iCnt = m_lValue.GetDataCnt() ;
        Ini.Save(sIniPath, "Total" , "Count" , iCnt);
        for(int i=0; i < iCnt ; i++) {
            Val = m_lValue.GetCrntData(!i) ;
            if(!Val)continue ;

            sName   =Val->GetName   ();Ini.Save(sIniPath, String(i) , "sName   ", sName    );
            sComment=Val->GetComment();Ini.Save(sIniPath, String(i) , "sComment", sComment );
            sValType=Val->GetValType();Ini.Save(sIniPath, String(i) , "sValType", sValType );
            dMinVal =Val->GetMinVal ();Ini.Save(sIniPath, String(i) , "dMinVal ", dMinVal  );
            dMaxVal =Val->GetMaxVal ();Ini.Save(sIniPath, String(i) , "dMaxVal ", dMaxVal  );

                 if(Val->GetValType() == "bool"  )Ini.Save(sIniPath, String(i) , "Val", (bool   *)Val->GetValAdd() );
            else if(Val->GetValType() == "int"   )Ini.Save(sIniPath, String(i) , "Val", (int    *)Val->GetValAdd() );
            else if(Val->GetValType() == "double")Ini.Save(sIniPath, String(i) , "Val", (double *)Val->GetValAdd() );
            else if(Val->GetValType() == "String")Ini.Save(sIniPath, String(i) , "Val", (String *)Val->GetValAdd() );
            else if(Val->GetValType() == "CArea" )((CArea *)Val->GetValAdd())->SaveBmp((GM.GetJobFileFolder() + _sVisnName + "_" + sName + "_Area.bmp").c_str());

        }
    }

    m_sVisnName        = _sVisnName   ;

}


void CValueList::LoadSave(bool _bLoad)
{
    LoadSave(_bLoad , m_sVisnName);

}






















