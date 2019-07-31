//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitValueList.h"

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
    m_sCrntJobFilePath = "";
    m_sVisnName = "";
}
void CValueList::Close()
{
    DeleteAll();
}


void CValueList::Add(String              _sName    ,
                     String              _sComment ,
                     CValue::EN_VAL_TYPE _eValType )
{

    CValue * Val = new CValue();
    Val->SetProperty(_sName , _sComment , _eValType);
    m_lValue.PushBack(Val);
}

void CValueList::Insert(int                 _iIdx     ,
                        String              _sName    ,
                        String              _sComment ,
                        CValue::EN_VAL_TYPE _eValType )
{

    CValue * Val = new CValue();
    Val->SetProperty(_sName , _sComment , _eValType);
    m_lValue.Insert(Val , _iIdx);
}

void CValueList::Delete(int _iIdx )
{
    delete m_lValue.GetData(_iIdx) ;
    m_lValue.Delete(_iIdx);
}

void CValueList::DeleteAll()
{
    int iCnt = m_lValue.GetDataCnt();
    for(int i = 0 ; i < iCnt ; i++) {
        delete m_lValue.GetCrntData(!i);
    }

    m_lValue.DeleteAll();
}



//여기서 적당한 컨트롤 찾고 PKG베이스에 벨류리스트 포인터 및 포인터셑 함수 추가.
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

        if(m_lValue.GetDataCnt() + 1 < 2) _sgGrid -> RowCount = 2 ;
        else                              _sgGrid -> RowCount = m_lValue.GetDataCnt() + 1  ;

        String sValType ;
        CValue * Val ;

        for (int i = 0 ;i < m_lValue.GetDataCnt() ; i++) {
            Val = m_lValue.GetCrntData(!i) ;

                 if(Val->GetValType() == CValue::vtString ){sValType="String ";}
            else if(Val->GetValType() == CValue::vtAddress){sValType="Address";}
            else if(Val->GetValType() == CValue::vtDouble ){sValType="Double ";}
            else if(Val->GetValType() == CValue::vtInt    ){sValType="Int    ";}
            else if(Val->GetValType() == CValue::vtEnum   ){sValType="Enum   ";}
            else if(Val->GetValType() == CValue::vtBool   ){sValType="Bool   ";}
            else                                           {sValType="Nome   ";}

            _sgGrid -> Cells[0][i+1] = i ;
            _sgGrid -> Cells[1][i+1] = Val->GetName()   ;
            _sgGrid -> Cells[2][i+1] = Val->GetComment();
            _sgGrid -> Cells[3][i+1] =      sValType    ;
            _sgGrid -> Cells[4][i+1] = Val->GetMinVal() ;
            _sgGrid -> Cells[5][i+1] = Val->GetMaxVal() ;
            _sgGrid -> Cells[6][i+1] = Val->GetString() ;
        }
    }
    else {
        //메모리에 쓰는것은 여기서 하지 않는다...
    }

}

void CValueList::LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName)
{

    String sIniPath  = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + ".ini" ;

    TUserINI Ini ;
    int iCnt ;
    CValue * Val ;

    String sName    ;
    String sComment ;
    int    eValType ;
    double dMinVal  ;
    double dMaxVal  ;
    String sVal     ;

    if(_bLoad) {
        //원래 있던놈들 메모리와 포인터  제거.
        DeleteAll();
        Ini.Load(sIniPath, "Total" , "Count" , iCnt);
        for(int i=0; i < iCnt ; i++) {
            Val = new CValue();
            Ini.Load(sIniPath, String(i) , "sName   ", sName    ); Val->SetName   (sName   );
            Ini.Load(sIniPath, String(i) , "sComment", sComment ); Val->SetComment(sComment);
            Ini.Load(sIniPath, String(i) , "eValType", eValType ); Val->SetValType(eValType);
            Ini.Load(sIniPath, String(i) , "dMinVal ", dMinVal  ); Val->SetMinVal (dMinVal );
            Ini.Load(sIniPath, String(i) , "dMaxVal ", dMaxVal  ); Val->SetMaxVal (dMaxVal );
            Ini.Load(sIniPath, String(i) , "sVal    ", sVal     ); Val->SetString (sVal    );

            m_lValue.PushBack(Val) ;
        }
    }
    else {
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath ) ;

        iCnt = m_lValue.GetDataCnt() ;
        Ini.Save(sIniPath, "Total" , "Count" , iCnt);
        for(int i=0; i < iCnt ; i++) {
            Val = m_lValue.GetCrntData(!i) ;
            sName   =Val->GetName   ();Ini.Save(sIniPath, String(i) , "sName   ", sName    );
            sComment=Val->GetComment();Ini.Save(sIniPath, String(i) , "sComment", sComment );
            eValType=Val->GetValType();Ini.Save(sIniPath, String(i) , "eValType", eValType );
            dMinVal =Val->GetMinVal ();Ini.Save(sIniPath, String(i) , "dMinVal ", dMinVal  );
            dMaxVal =Val->GetMaxVal ();Ini.Save(sIniPath, String(i) , "dMaxVal ", dMaxVal  );
            sVal    =Val->GetString ();Ini.Save(sIniPath, String(i) , "sVal    ", sVal     );

        }
    }

    m_sCrntJobFilePath = _sFolderPath ;
    m_sVisnName        = _sVisnName   ;

}

void CValueList::LoadSave(bool _bLoad)
{
    LoadSave(_bLoad , m_sCrntJobFilePath , m_sVisnName);

}






















