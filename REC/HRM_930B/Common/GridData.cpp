//---------------------------------------------------------------------------


#pragma hdrstop

#include "GridData.h"

#include <Classes.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

CGridData::CGridData()
{
    m_iMaxCol = 0 ;
    m_iMaxRow = 0 ;

    m_iTitleCol = 0 ;
    m_iTitleRow = 0 ;

    sCells = NULL ;

}

CGridData::~CGridData()
{
    SetMaxColRow(0,0);
}


void CGridData::ClearCells()
{
    for(int c = 0 ; c < m_iMaxCol ; c++){
        for(int r = 0 ; r < m_iMaxRow ; r++){
            sCells[c][r]="";
        }
    }

}

void CGridData::SetMaxColRow(int _iMaxCol , int _iMaxRow)
{
    if(_iMaxCol == 0 && _iMaxRow == 0) { //0,0 이면 메모리 모두 해제.
        if(sCells) {
            for(int c = 0 ; c < m_iMaxCol ; c++) {
                delete [] sCells[c] ;
            }
            delete [] sCells ;
            sCells = NULL ;
        }
        m_iMaxCol = _iMaxCol ;
        m_iMaxRow = _iMaxRow ;
    }

    if(m_iMaxCol == _iMaxCol && m_iMaxRow == _iMaxRow) return ;

    String ** sTempCell = new String *[_iMaxCol];
    for(int c = 0 ; c < _iMaxCol ; c++) {
        sTempCell[c] = new String [_iMaxRow];
        if(c >= m_iMaxCol) continue ;
        for(int r = 0 ; r < _iMaxRow ; r++) {
            if(r >= m_iMaxRow) break ;
            sTempCell[c][r] = sCells[c][r];
        }
    }

    //new
    if(sCells != NULL) {
        if(sCells) {
            for(int c = 0 ; c < m_iMaxCol ; c++) {
                delete [] sCells[c] ;
            }
            delete [] sCells ;
            sCells = NULL ;
        }
    }

    m_iMaxCol = _iMaxCol;
    m_iMaxRow = _iMaxRow;
    sCells = sTempCell ;
}


int CGridData::GetMaxCol()
{
    return m_iMaxCol ;
}

int CGridData::GetMaxRow()
{
    return m_iMaxRow ;
}

String CGridData::GetCell(int _iCol , int _iRow)
{
    if(_iCol < 0 || _iCol >= m_iMaxCol) return "" ;
    if(_iRow < 0 || _iRow >= m_iMaxRow) return "" ;

    return sCells[_iCol][_iRow] ;
}

bool CGridData::SetCell(int _iCol , int _iRow , String _sCell)
{
    if(_iCol < 0 || _iCol >= m_iMaxCol) return false ;
    if(_iRow < 0 || _iRow >= m_iMaxRow) return false ;

    sCells[_iCol][_iRow] = _sCell ;
    return true ;
}

bool CGridData::SetCellByColTitle (String _sColTitle , int _iRow , String _sCell)
{
    if(_iRow < 0 || _iRow >= m_iMaxRow) return false ;

    for(int c = 0 ; c < m_iMaxCol ; c++){
        if(sCells[c][m_iTitleRow] == _sColTitle){
            sCells[c][_iRow] = _sCell ;
            return true ;
        }
    }
    return false ;
}

String CGridData::GetCellByColTitle (String _sColTitle , int _iRow )
{
    if(_iRow < 0 || _iRow >= m_iMaxRow) return "" ;

    for(int c = 0 ; c < m_iMaxCol ; c++){
        if(sCells[c][m_iTitleRow] == _sColTitle){
            return sCells[c][_iRow] ;
        }
    }
    return "";

}

bool CGridData::SetCellByRowTitle (String _sRowTitle , int _iCol , String _sCell)
{
    if(_iCol < 0 || _iCol >= m_iMaxCol) return false ;

    for(int r = 0 ; r < m_iMaxRow ; r++){
        if(sCells[m_iTitleCol][r] == _sRowTitle){
            sCells[_iCol][r] = _sCell ;
            return true ;
        }
    }
    return false ;
}

String CGridData::GetCellByRowTitle (String _sRowTitle , int _iCol )
{
    if(_iCol < 0 || _iCol >= m_iMaxCol) return "" ;

    for(int r = 0 ; r < m_iMaxRow ; r++){
        if(sCells[m_iTitleCol][r] == _sRowTitle){
            return sCells[_iCol][r] ;
        }
    }

    return "" ;

}

bool CGridData::SetCellByColRowTitle (String _sColTitle , String _sRowTitle , String _sCell)
{
    int iCol = -1;
    for(int c = 0 ; c < m_iMaxCol ; c++){
        if(sCells[c][m_iTitleRow] == _sColTitle){
            iCol = c ;
        }
    }
    if(iCol == -1) return false ;

    int iRow = -1;
    for(int r = 0 ; r < m_iMaxRow ; r++){
        if(sCells[m_iTitleCol][r] == _sRowTitle){
            iRow = r ;
        }
    }
    if(iRow == -1) return false ;

    sCells[iCol][iRow] = _sCell ;
    return true ;

}

String CGridData::GetCellByColRowTitle (String _sColTitle , String _sRowTitle )
{
    int iCol = -1;
    for(int c = 0 ; c < m_iMaxCol ; c++){
        if(sCells[c][m_iTitleRow] == _sColTitle){
            iCol = c ;
        }
    }
    if(iCol == -1) return "" ;

    int iRow = -1;
    for(int r = 0 ; r < m_iMaxRow ; r++){
        if(sCells[m_iTitleCol][r] == _sRowTitle){
            iRow = r ;
        }
    }
    if(iRow == -1) return "" ;

    return sCells[iCol][iRow] ;

}

void CGridData::SetTitleCol(int _iCol)
{
    m_iTitleCol = _iCol ;
}

int  CGridData::GetTitleCol()
{
    return m_iTitleCol ;
}

void CGridData::SetTitleRow(int _iRow)
{
    m_iTitleRow = _iRow ;
}

int CGridData::GetTitleRow()
{
    return m_iTitleRow ;

}






bool CGridData::LoadFromCsv(String _sFilePath)
{
    TStringList* slFile = new TStringList;
    TStringList* slData = new TStringList;
    //try {
        if(FileExists(_sFilePath)){
            slFile->LoadFromFile(_sFilePath);
        }
    //}
    //__finally
    //{
    //    delete slData;
    //    delete slFile;
    //    return false;
    //}
    if(slFile -> Count == 0 ) {
        delete slData;
        delete slFile;
        return false;
    }

    slData->CommaText = slFile->Strings[0];
    if(slData -> Count == 0 ) {
        delete slData;
        delete slFile;
        return false;
    }

    SetMaxColRow(slData->Count , slFile->Count);
    for(int r=0; r<slFile->Count; r++){
         //,로 구분된 문자열을 자동으로 분리시켜주는 CommaText
        //slData->CommaText = slFile->Strings[r];
        slFile->Strings[r] = StringReplace(slFile->Strings[r] , " " , "~" , TReplaceFlags()<<rfReplaceAll);
        slData->Delimiter = ',' ;
        slData->DelimitedText = slFile->Strings[r];
        for(int c=0; c<slData->Count; c++){
            slData->Strings[c] = StringReplace(slData->Strings[c],"~"," " ,TReplaceFlags()<<rfReplaceAll);
            SetCell(c,r,slData->Strings[c]);
        }
    }

    delete slData;
    delete slFile;

    return true ;

}

bool CGridData::SaveToCsv(String _sFilePath)
{
    String sContents;
    for(int iRow=0;iRow<m_iMaxRow;iRow++){
        for(int iCol=0;iCol<m_iMaxCol;iCol++){
            sContents+=sCells[iCol][iRow];
            if(iCol==m_iMaxCol-1)break ;
            sContents+=",";
        }
        sContents+="\r\n";
    }

    if(FileExists(_sFilePath)) {
        DeleteFile(_sFilePath);
    }
    //else {
    //    
    //    
    //
    //}


    int iFileHandle=FileCreate(_sFilePath);
    FileWrite(iFileHandle,sContents.c_str(),sContents.Length());
    FileClose(iFileHandle);

    return true ;

}

bool CGridData::SaveToXls(String _sFilePath)
{
    if(FileExists(_sFilePath)) {
        DeleteFile(_sFilePath);
    }
    TFileStream *FStream = new TFileStream(_sFilePath, fmCreate);


    WORD CXlsBof   [6] = {0x809, 8, 0, 0x10, 0, 0};
    WORD CXlsEof   [2] = {0x0A, 0};
    WORD CXlsLabel [6] = {0x204, 0, 0, 0, 0, 0};
    WORD CXlsNumber[5] = {0x203, 14, 0, 0, 0};
    WORD CXlsRk    [5] = {0x27E, 10, 0, 0, 0};


    try {
        CXlsBof[4] = 0;
        FStream->WriteBuffer(CXlsBof, sizeof(CXlsBof));
        String sVal ;
        WORD L ;
        double dVal ;
        int    iVal ;
        for(int r = 0; r < m_iMaxRow; r++) {
            for(int c = 0; c < m_iMaxCol;c++){
                sVal = sCells[c][r] ;
                L = sVal.Length();

                //숫자 값일때.
                if(StrToFloatDef(sVal , -1) == StrToFloatDef(sVal , 1)){
                    dVal = sVal.ToDouble() ;
                    iVal = dVal ;
                    if(dVal - iVal) { //double
                        CXlsNumber[2] = r;
                        CXlsNumber[3] = c;
                        FStream->WriteBuffer(CXlsNumber, sizeof(CXlsNumber));
                        FStream->WriteBuffer(&dVal, 8);
                    }
                    else { //int
                        CXlsRk[2] = r;
                        CXlsRk[3] = c;
                        FStream->WriteBuffer(CXlsRk, sizeof(CXlsRk));
                        //int V = ((iVal << 2) | 2);
                        FStream->WriteBuffer(&iVal, 4);
                    }

                }
                else {//String 일때.

                    CXlsLabel[1] = 8 + L;
                    CXlsLabel[2] = r;
                    CXlsLabel[3] = c;
                    CXlsLabel[5] = L;
                    FStream->WriteBuffer(CXlsLabel, sizeof(CXlsLabel));
                    FStream->WriteBuffer(sVal.c_str(), L);
                }

            }

        }
        FStream->WriteBuffer(CXlsEof, sizeof(CXlsEof));
    }
    __finally
    {
        delete FStream;
    }
    return true ;
}


