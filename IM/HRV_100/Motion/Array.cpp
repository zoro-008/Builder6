//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

//---------------------------------------------------------------------------
#include "Array.h"
#include "SLogUnit.h"
#include "UserINI.h"
//#include "OptionMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall CArrayData::CArrayData(void)
{
    CHPS = NULL ;
    SetMaxColRow(1,1);

}

//---------------------------------------------------------------------------
__fastcall CArrayData::~CArrayData(void)
{
    if(CHPS != NULL) {
        for(int r = 0 ; r < m_iMaxRow ; r++) {
            delete [] CHPS[r];
        }
        delete [] CHPS ;
        CHPS = NULL;
    }
}

void __fastcall CArrayData::SetName(AnsiString _sName)
{
    m_sName = _sName ;
}

void __fastcall CArrayData::SetID(AnsiString _sID)
{
    m_sID = _sID ;
}

AnsiString __fastcall CArrayData::GetID  (void)
{
    return m_sID ;
}

AnsiString __fastcall CArrayData::GetName (void)
{
    return m_sName ;
}

void __fastcall CArrayData::SetLotNo(AnsiString _sLotNo)
{
    m_sLotNo = _sLotNo ;
}

AnsiString __fastcall CArrayData::GetLotNo  (void)
{
    return m_sLotNo ;
}

void __fastcall CArrayData::SetStep(int _iStep)
{
    m_iStep = _iStep ;
}

int __fastcall CArrayData::GetStep(void)
{
    return m_iStep ;
}

void __fastcall CArrayData::SetSubStep(int _iSubStep)
{
    m_iSubStep = _iSubStep ;
}

int __fastcall CArrayData::GetSubStep(void)
{
    return m_iSubStep ;
}

void __fastcall CArrayData::ClearMap(void)
{
    SetStat(csNone);
    SetLotNo("");
    SetID("");
    SetSubStep(0);

}

void __fastcall CArrayData::FlipX()
{

    CChip ** chps = new CChip*[m_iMaxRow];
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        CHPS[r] = new CChip [m_iMaxCol];
    }

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[i][m_iMaxCol - 1 - j] ;
        }
    }
    memcpy(CHPS , chps , sizeof(CChip) * m_iMaxRow * m_iMaxCol);

    for(int r = 0 ; r < m_iMaxRow ; r++) {
        delete [] chps[r];
    }
    delete [] chps ;

    OnChange();

}

void __fastcall CArrayData::FlipY()
{
    CChip ** chps = new CChip*[m_iMaxRow];
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        CHPS[r] = new CChip [m_iMaxCol];
    }

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - i][j] ;
        }
    }
    memcpy(CHPS , chps , sizeof(CChip) * m_iMaxRow * m_iMaxCol);

    for(int r = 0 ; r < m_iMaxRow ; r++) {
        delete [] chps[r];
    }
    delete [] chps ;

    OnChange();
}

void __fastcall CArrayData::FlipXY()
{
    CChip ** chps = new CChip*[m_iMaxRow];
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        CHPS[r] = new CChip [m_iMaxCol];
    }

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - i][m_iMaxCol - 1 - j] ;
        }
    }
    memcpy(CHPS , chps , sizeof(CChip) *  m_iMaxRow * m_iMaxCol);

    for(int r = 0 ; r < m_iMaxRow ; r++) {
        delete [] chps[r];
    }
    delete [] chps ;

    OnChange();
}


/*
void __fastcall CArrayData::SetMaxColRow (int c,int r)
{
    if(m_iMaxCol == c && m_iMaxRow == r) return ;
    if(CHPS != NULL) {
        for(int r = 0 ; r < m_iMaxRow ; r++) {
            delete [] CHPS[r];
        }
        delete [] CHPS ;
    }

    m_iMaxCol = c;
    m_iMaxRow = r;              ///r,c

    CHPS = new CChip*[m_iMaxRow];
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        CHPS[r] = new CChip [m_iMaxCol];
    }

    OnChange();
}
*/
void __fastcall CArrayData::TurnCw90()
{
    //버퍼 생성.
    CChip ** chps = new CChip*[m_iMaxCol];
    for(int c = 0 ; c < m_iMaxCol ; c++) {
        chps[c] = new CChip [m_iMaxRow];
    }

    //데이터 버퍼로 옮김.
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - j][i] ;
        }
    }

    //맥스 세팅.
    SetMaxColRow(m_iMaxRow , m_iMaxCol);

    //데이터 메모리로 복원.
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        memcpy(CHPS[r] , chps[r] , sizeof(CChip) * m_iMaxCol);
    }
    //버퍼 삭제.
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        delete [] chps[r];
    }
    delete [] chps ;

    OnChange();
}

void __fastcall CArrayData::TurnCw180()
{
    CChip ** chps = new CChip*[m_iMaxRow];
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        chps[r] = new CChip [m_iMaxCol];
    }

    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL];
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            chps[i][j]=CHPS[m_iMaxRow - 1 - i][m_iMaxCol - 1 - j] ;
        }
    }

    //데이터 메모리로 복원.
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        memcpy(CHPS[r] , chps[r] , sizeof(CChip) * m_iMaxCol);
    }

    //버퍼 삭제.
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        delete [] chps[r];
    }
    delete [] chps ;

    OnChange();
}

void __fastcall CArrayData::TurnCw270()
{
    //버퍼 생성.
    CChip ** chps = new CChip*[m_iMaxCol];
    for(int c = 0 ; c < m_iMaxCol ; c++) {
        chps[c] = new CChip [m_iMaxRow];
    }

    //CChip chps[MAX_ARRAY_ROW][MAX_ARRAY_COL];
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
                           //r,c
            chps[i][j]=CHPS[j][m_iMaxCol - 1 - i] ;
        }
    }

    SetMaxColRow(m_iMaxRow , m_iMaxCol);

    //데이터 메모리로 복원.
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        memcpy(CHPS[r] , chps[r] , sizeof(CChip) * m_iMaxCol);
    }

    //버퍼 삭제.
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        delete [] chps[r];
    }
    delete [] chps ;

    OnChange();


}



void __fastcall CArrayData::SetMaxColRow (int c,int r)
{
    if(m_iMaxCol == c && m_iMaxRow == r) return ;
    if(CHPS != NULL) {
        for(int r = 0 ; r < m_iMaxRow ; r++) {
            delete [] CHPS[r];
        }
        delete [] CHPS ;
    }

    m_iMaxCol = c;
    m_iMaxRow = r;              ///r,c

    CHPS = new CChip*[m_iMaxRow];
    for(int r = 0 ; r < m_iMaxRow ; r++) {
        CHPS[r] = new CChip [m_iMaxCol];
    }

    OnChange();
}

/***************************************************************************/
/* Direct Accessable Functions.                                            */
/***************************************************************************/
//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindChip(EN_CHIP_STAT _iStat , int r , int c)
{
    bool bRet = CheckStat(r,c,_iStat);

    return bRet ;
}

//---------------------------------------------------------------------------
int __fastcall CArrayData::FindFrstRow(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindFrstRowCol(_iStat , iRowNum , iColNum);

    //return Find Row No.
    return iRowNum;
}

//---------------------------------------------------------------------------
int __fastcall CArrayData::FindFrstCol(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindFrstColRow(_iStat , iRowNum , iColNum);

    //return Find Col No.
    return iColNum;
}
//---------------------------------------------------------------------------
int __fastcall CArrayData::FindLastRow(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindLastRowCol(_iStat , iRowNum , iColNum);

    //return Find Row No.
    return iRowNum;
}

//---------------------------------------------------------------------------
int __fastcall CArrayData::FindLastCol(EN_CHIP_STAT _iStat)
{
    //Local Var.
    int iRowNum;
    int iColNum;

    //Find First Row and Col.
    FindLastColRow(_iStat , iRowNum , iColNum);

    //return Find Col No.
    return iColNum;
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstRowCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstColRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
            if (FindChip(_iStat , j , i)) {
                r = j;
                c = i;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindLastRowCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    for (int i = m_iMaxRow - 1 ; i >= 0 ; i--) {
        for (int j = m_iMaxCol - 1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstRowLastCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = m_iMaxCol - 1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , i , j)) {                                           
                r = i;
                c = j;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindLastRowFrstCol(EN_CHIP_STAT _iStat , int &r , int &c)
{
    for (int i = m_iMaxRow - 1 ; i >= 0 ; i--) {
        for (int j = 0 ; j < m_iMaxCol ; j++) {
            if (FindChip(_iStat , i , j)) {
                r = i;
                c = j;
                return true;
            }
        }
    }


    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindLastColFrstRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    for (int i = m_iMaxCol - 1 ; i >= 0 ; i--) {
        for (int j = 0 ; j < m_iMaxRow ; j++) {
            if (FindChip(_iStat , j , i)) {
                c = i;
                r = j;
                return true;
            }
        }
    }


    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstColLastRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = 0 ; i < m_iMaxCol ; i++) {
        for (int j = m_iMaxRow - 1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , j , i)) {
                r = j;
                c = i;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindLastColRow(EN_CHIP_STAT _iStat , int &r , int &c)
{
    //Local Var.
    for (int i = m_iMaxCol-1 ; i >= 0 ; i--) {
        for (int j = m_iMaxRow-1 ; j >= 0 ; j--) {
            if (FindChip(_iStat , j , i)) {
                r = j;
                c = i;
                return true;
            }
        }
    }

    //No Find.
    r = -1;
    c = -1;
    return false;
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstRowCol_Indx (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = iStrCol ; j <= iEndCol ; j++) {
            if (FindChip(_iStat , i , j)) {
                R = i;
                C = j;
                return true;
            }
        }
    }
    //No Find.
    R = -1;
    C = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstRowLastCol_Indx  (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    for (int i = 0 ; i < m_iMaxRow ; i++) {
        for (int j = iEndCol ; j >= iStrCol ; j--) {
            if (FindChip(_iStat , i , j)) {
                R = i;
                C = j;
                return true;
            }
        }
    }
    //No Find.
    R = -1;
    C = -1;
    return false;    
}

//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstColRow_Indx (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    for (int c = iStrCol ; c <= iEndCol ; c++) {
        for (int r = 0 ; r < m_iMaxRow ; r++) {
            if (FindChip(_iStat , r , c)) {
                R = r;
                C = c;
                return true;
            }
        }
    }

    //No Find.
    R = -1;
    C = -1;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall CArrayData::FindFrstColLastRow_Indx  (EN_CHIP_STAT _iStat , int iStrCol , int iEndCol , int &R , int &C)
{
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iStrCol >= m_iMaxCol) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}
    if (iEndCol <  0        ) {R = -1; C = -1; return false ;}

    //Local Var.
    for (int c = iStrCol ; c <= iEndCol ; c++) {
        for (int r = m_iMaxRow - 1 ; r >= 0 ; r--) {
            if (FindChip(_iStat , r , c)) {
                R = r;
                C = c;
                return true;
            }
        }
    }

    //No Find.
    R = -1;
    C = -1;
    return false;
}

//---------------------------------------------------------------------------
void __fastcall CArrayData::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    sPath= EXE_FOLDER + "SeqData\\ArrayData.INI" ;

    //Load
    AnsiString sRowData ;
    AnsiString sIndex   ;
    AnsiString sTemp    ;
    int iMaxCol , iMaxRow ;
    if(IsLoad) {
        UserINI.Load(sPath, m_sName , "m_iMaxRow" , iMaxRow             );
        UserINI.Load(sPath, m_sName , "m_iMaxCol" , iMaxCol             );
        UserINI.Load(sPath, m_sName , "m_sID"     , m_sID               );
        UserINI.Load(sPath, m_sName , "m_sLotNo"  , m_sLotNo            );
        UserINI.Load(sPath, m_sName , "m_iStep"   , m_iStep             );
        UserINI.Load(sPath, m_sName , "m_iSubStep", m_iSubStep          );
        if(!iMaxRow) iMaxRow = 1 ;
        if(!iMaxCol) iMaxCol = 1 ;

        //메모리 재할당.
        SetMaxColRow(iMaxCol , iMaxRow);

        for (int r = 0 ; r < m_iMaxRow ; r++) {
            sIndex.sprintf("Row%03d", r ) ;
            UserINI.Load(sPath, m_sName , sIndex, sRowData );
            for (int c = 0 ; c < m_iMaxCol ; c++) {
                sTemp = sRowData.SubString(c*3+1,3);
                CHPS[r][c].SetStat((EN_CHIP_STAT)sTemp.ToIntDef(0));
            }
        }

        for(int i = 0 ; i < MAX_SUBSTAT ; i++) {
            for (int r = 0 ; r < m_iMaxRow ; r++) {
                sIndex.sprintf("S%01dRow%03d",i, r ) ;
                UserINI.Load(sPath, m_sName , sIndex, sRowData );
                for (int c = 0 ; c < m_iMaxCol ; c++) {
                    sTemp = sRowData.SubString(c*3+1,3);
                    CHPS[r][c].SetSubStat(i,(EN_CHIP_STAT)sTemp.ToIntDef(0));
                }
            }
        }
        OnChange();
    }
    else {
        UserINI.Save(sPath, m_sName , "m_iMaxRow" , m_iMaxRow           );
        UserINI.Save(sPath, m_sName , "m_iMaxCol" , m_iMaxCol           );
        UserINI.Save(sPath, m_sName , "m_sID"     , m_sID.ToIntDef(0)   );
        UserINI.Save(sPath, m_sName , "m_sLotNo"  , m_sLotNo            );
        UserINI.Load(sPath, m_sName , "m_iStep"   , m_iStep             );
        UserINI.Save(sPath, m_sName , "m_iSubStep", m_iSubStep          ); //원래 주석이였다.
        for (int r = 0 ; r < m_iMaxRow ; r++) {
            sRowData = "" ;
            sIndex.sprintf("Row%03d", r ) ;
            for (int c = 0 ; c < m_iMaxCol ; c++) {
                sTemp.sprintf("%03d",(int)CHPS[r][c].GetStat()) ;
                sRowData += sTemp ;
            }
            UserINI.Save(sPath, m_sName , sIndex, sRowData );
        }
        for(int i = 0 ; i < MAX_SUBSTAT ; i++) {
            for (int r = 0 ; r < m_iMaxRow ; r++) {
                sRowData = "" ;
                sIndex.sprintf("S%01dRow%03d",i, r ) ;
                for (int c = 0 ; c < m_iMaxCol ; c++) {
                    sTemp.sprintf("%03d",(int)CHPS[r][c].GetSubStat(i)) ;
                    sRowData += sTemp ;
                }
                UserINI.Save(sPath, m_sName , sIndex, sRowData );
            }
        }
    }                                                   
}






//==============================================================================
//ARRAY UI
//==============================================================================
__fastcall CArray::CArray(void)
{
    //Default Set.
    m_iClStat[csRslt0  ] = clInactiveCaption ;//csInactiveCaption = 0  ,
    m_iClStat[csRslt1  ] = clMaroon          ;//csMaroon          = 1  ,
    m_iClStat[csRslt2  ] = clPurple          ;//csPurple          = 2  ,
    m_iClStat[csRslt3  ] = clTeal            ;//csTeal            = 3  ,
    m_iClStat[csRslt4  ] = clOlive           ;//csOlive           = 4  ,
    m_iClStat[csRslt5  ] = clNavy            ;//csNavy            = 5  ,
    m_iClStat[csRslt6  ] = clMoneyGreen      ;//csMoneyGreen      = 6  ,
    m_iClStat[csRslt7  ] = clGreen           ;//csGreen           = 7  ,
    m_iClStat[csRslt8  ] = clFuchsia         ;//clHighlight       = 8  ,
    m_iClStat[csRslt9  ] = clBlack           ;//csBlack           = 9  ,
    m_iClStat[csRslt10 ] = clBlack           ;//csBlack           = 10 ,
    m_iClStat[csRslt11 ] = clBlack           ;//csBlack           = 11 ,
    m_iClStat[csRslt12 ] = clBlack           ;//csBlack           = 12 ,
    m_iClStat[csRslt13 ] = clBlack           ;//csBlack           = 13 ,
    m_iClStat[csRslt14 ] = clBlack           ;//csBlack           = 14 ,
    m_iClStat[csRslt15 ] = clBlack           ;//csBlack           = 15 ,
    m_iClStat[csRslt16 ] = clBlack           ;//csBlack           = 16 ,
    m_iClStat[csNone   ] = clWhite           ;//csWhite           = 17 ,//스트립이나 카세트가 없는 상태.                                            >
    m_iClStat[csMask   ] = clSkyBlue         ;//csSkyBlue         = 18 ,//PKG가 없지만 채크를 해놓은 상태.                                          >  요거 세개가 PKG 없는거.
    m_iClStat[csEmpty  ] = clSilver          ;//csSilver          = 19 ,//카세트에 스트립이 없는 상태 혹은 더미패드에 토출자국이 없는경우. 프로브끝 >
    m_iClStat[csUnkwn  ] = clAqua            ;//csAqua            = 20 ,//작업전 있는지 없는지 모르는 경우.
    m_iClStat[csWork   ] = clBlue            ;//csBlue            = 21 ,// 작업 끝.
    m_iClStat[csWait   ] = clYellow          ;//csYellow          = 22 ,
    m_iClStat[csFail   ] = clFuchsia         ;//csFuchsia         = 23 ,
    m_iClStat[csEror   ] = clRed             ;//csRed             = 24 ,
    m_iClStat[csGood   ] = clLime            ;//csLime            = 25 ,
    m_iClStat[csEtc    ] = clNone            ;//etc               = 26 ,
    m_iClStat[csToBuf  ] = clBlack           ;//clBlack           = 27,
    m_iClStat[csFromBuf] = clBlack           ;//clBlack           = 28,
    m_iClStat[csDrop   ] = clGray            ;//clGray            = 29,

    m_sMenuName[csRslt0  ] = "RSLT0"     ;
    m_sMenuName[csRslt1  ] = "RSLT1"     ;
    m_sMenuName[csRslt2  ] = "RSLT2"     ;
    m_sMenuName[csRslt3  ] = "RSLT3"     ;
    m_sMenuName[csRslt4  ] = "RSLT4"     ;
    m_sMenuName[csRslt5  ] = "RSLT5"     ;
    m_sMenuName[csRslt6  ] = "RSLT6"     ;
    m_sMenuName[csRslt7  ] = "RSLT7"     ;
    m_sMenuName[csRslt8  ] = "RSLT8"     ;
    m_sMenuName[csRslt9  ] = "RSLT9"     ;
    m_sMenuName[csRslt10 ] = "RSLT10"    ;
    m_sMenuName[csRslt11 ] = "RSLT11"    ;
    m_sMenuName[csRslt12 ] = "RSLT12"    ;
    m_sMenuName[csRslt13 ] = "RSLT13"    ;
    m_sMenuName[csRslt14 ] = "RSLT14"    ;
    m_sMenuName[csRslt15 ] = "RSLT15"    ;
    m_sMenuName[csRslt16 ] = "RSLT16"    ;
    m_sMenuName[csNone   ] = "NONE"      ;
    m_sMenuName[csMask   ] = "MASK"      ;
    m_sMenuName[csEmpty  ] = "EMPTY"     ;
    m_sMenuName[csUnkwn  ] = "UNKNOWN"   ;
    m_sMenuName[csWork   ] = "WORK"      ;
    m_sMenuName[csWait   ] = "WAIT"      ;
    m_sMenuName[csFail   ] = "FAIL"      ;
    m_sMenuName[csEror   ] = "ERROR"     ;
    m_sMenuName[csGood   ] = "GOOD"      ;
    m_sMenuName[csEtc    ] = "ETC"       ;
    m_sMenuName[csToBuf  ] = "ToBuf"     ;
    m_sMenuName[csFromBuf] = "FromBuf"   ;
    m_sMenuName[csDrop   ] = "DROP"      ;

    imArray = new TImage (Application);
    for( int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++) {
        pmStatus[i] = new TPopupMenu(Application);
        pmStatus[i] -> OwnerDraw = true ;
    }

    for( int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++) {
        for( int j = 0 ; j < MAX_CHIP_STAT ; j++) {
            miStatus[i][j] = new TMenuItem(Application);
            miStatus[i][j] -> OnDrawItem = MenuDrawItem ;

            switch(i) {
                default     : miStatus[i][j]-> Caption = "ERR_" + m_sMenuName[j] ;                                                                      break ;
                case spOne  : miStatus[i][j]-> Caption = "ONE_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spOne ; miStatus[i][j]-> OnClick = OneClick ; break ;
                case spCol  : miStatus[i][j]-> Caption = "COL_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spCol ; miStatus[i][j]-> OnClick = ColClick ; break ;
                case spRow  : miStatus[i][j]-> Caption = "ROW_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spRow ; miStatus[i][j]-> OnClick = RowClick ; break ;
                case spAll  : miStatus[i][j]-> Caption = "ALL_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spAll ; miStatus[i][j]-> OnClick = AllClick ; break ;
                case spDrag : miStatus[i][j]-> Caption = "DRG_" + m_sMenuName[j] ; miStatus[i][j]-> Tag = spDrag; miStatus[i][j]-> OnClick = DragClick; break ;
            }

            miStatus[i][j]-> Tag  = j;
            pmStatus[i]   -> Items -> Insert(j  , miStatus[i][j]);
            miStatus[i][j]-> AutoHotkeys = maManual ;
        }
    }

    imArray -> OnMouseDown = ImageMouseDown ;
    imArray -> OnMouseMove = ImageMouseMove ;
    imArray -> OnMouseUp   = ImageMouseUp   ;
}
__fastcall CArray::~CArray(void)
{

}
//---------------------------------------------------------------------------
void __fastcall CArray::MenuDrawItem(TObject *Sender,TCanvas *ACanvas,const TRect &ARect, bool Selected)
{
    if(Selected) {
        ACanvas->Brush->Color = (TColor)0x339900;
        ACanvas->Font ->Color = clWhite;
    }
    else {
        ACanvas->Brush->Color = (TColor)0x99FFCC;
        ACanvas->Font ->Color = clBlack;
    }

    AnsiString sCaption = ((TMenuItem *)Sender)->Caption ;
    AnsiString sTemp , sTemp1   ;

    //for(int i = 0 ; i < ((TMenuItem *)Sender)->Caption.

    ACanvas->TextRect(ARect, ARect.Left + 6, ARect.Top + 3, "   "+sCaption);


    //TColor     m_iClStat   [MAX_CHIP_STAT]
    //AnsiString m_sMenuName[MAX_CHIP_STAT]
    sTemp  = sCaption.Delete(1,4) ;
    for(int i = 0 ; i < MAX_CHIP_STAT ; i++ ) { ///잠시만...
        sTemp1 = m_sMenuName[i] ;
        if(sTemp1 == sTemp && sTemp != "") {
            ACanvas -> Brush -> Color = m_iClStat[i] ;
            break ;
        }
    }
    ACanvas->Rectangle(1,ARect.top + 1,15,ARect.top + 15) ;

    //Memo1->Lines->Add(((TMenuItem *)Sender)->Caption);
}
/*
        void __fastcall OneClick (TObject *Sender);
        void __fastcall AllClick (TObject *Sender);
        void __fastcall RowClick (TObject *Sender);
        void __fastcall ColClick (TObject *Sender);
        void __fastcall DragClick(TObject *Sender);

        void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //TObject *Sender,
        void __fastcall ImageMouseMove(TObject *Sender,                      TShiftState Shift, int X, int Y); //TObject *Sender,
        void __fastcall ImageMouseUp  (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //TObject *Sender,

        void __fastcall ImageArayRC   (int X , int Y , int &R , int &C);
        void __fastcall ImageArayRCFix(int X , int Y , int &R , int &C);

        void __fastcall UpdateArayNrm(); //셀크기 상관 없이 그냥 일율적으로 나누는 그림.
        void __fastcall UpdateArayFix(); //셀크기를 픽스 해서 그리는 그림.

        struct SClickStat {
            bool bMouseDown;
            int  iSelX1,iSelY1,iSelX2,iSelY2;
            int  iSelR1,iSelR2,iSelC1,iSelC2;
        }ClickStat;

    public:
        void __fastcall UpdateAray    ();
        void __fastcall SetParent     (TPanel * _pnBase);
        void __fastcall ClearDispItem ();
        void __fastcall SetDispItem   (EN_CHIP_STAT _iStat , AnsiString _sSubCaption ) ;
        void __fastcall SetDispColor  (EN_CHIP_STAT _iStat , TColor     _cColor      ) ;



        //PopupMenu
        void __fastcall PopupMenuEnable(bool bState);
*/
void __fastcall CArray::SetParent(TPanel * _pnBase)
{

    imArray -> Parent = _pnBase  ;
    //imArray -> Align  = alClient ;

    imArray -> Picture -> Bitmap -> Width  = _pnBase -> Width  ;
    imArray -> Picture -> Bitmap -> Height = _pnBase -> Height ;

    imArray -> Width  = _pnBase -> Width  ;
    imArray -> Height = _pnBase -> Height ;

    _pnBase -> DoubleBuffered = true ;

    m_bSameCellSize = false ;

    OnChange();


}

void __fastcall CArray::SetCellSize   (bool _bSameCellSize)
{
    m_bSameCellSize = _bSameCellSize ;
}

void __fastcall CArray::ClearDispItem()
{
    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++ ) {
            miStatus[i][j] -> Visible = false ;
        }
    }
}

void __fastcall CArray::SetDispItem(EN_CHIP_STAT _iStat , AnsiString _sSubCaption )
{
    bool isAllVisible = true ;
    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++ ) {
            if(!miStatus[i][j] -> Visible) {
                isAllVisible = false ;
                break ;
            }
        }
        if(isAllVisible) break ;
    }

    if(isAllVisible) ClearDispItem();

    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        miStatus[i][_iStat] -> Visible = true ;
        if((EN_CHIP_STAT)i != spAll && (_iStat == csToBuf || _iStat == csFromBuf) ) miStatus[i][_iStat] -> Visible = false ;
        //miStatus[i][_iStat] = new TMenuItem(Application);
        m_sMenuName[_iStat] = _sSubCaption ;
        miStatus[i][_iStat]->AutoHotkeys = maManual ; //동작 안함....
        pmStatus[i]        ->AutoHotkeys = maManual ;

        switch(i) {                                       //글자수 바꾸지 말기.  MenuDrawItem에서 4글자 앞에 트림함.
            default     : miStatus[i][_iStat]-> Caption = "ERR_" + m_sMenuName[_iStat] ; break ;
            case spOne  : miStatus[i][_iStat]-> Caption = "ONE_" + m_sMenuName[_iStat] ; break ;
            case spCol  : miStatus[i][_iStat]-> Caption = "COL_" + m_sMenuName[_iStat] ; break ;
            case spRow  : miStatus[i][_iStat]-> Caption = "ROW_" + m_sMenuName[_iStat] ; break ;
            case spAll  : miStatus[i][_iStat]-> Caption = "ALL_" + m_sMenuName[_iStat] ; break ;
            case spDrag : miStatus[i][_iStat]-> Caption = "DRG_" + m_sMenuName[_iStat] ; break ;
        }
    }
}

void __fastcall CArray::SetDispColor(EN_CHIP_STAT _iStat , TColor _cColor )
{
    m_iClStat[_iStat] = _cColor ;

    OnChange();
}

void __fastcall CArray::PopupMenuEnable(bool bState)
{
    for(int i = 0 ; i < MAX_STATUS_POPUP_ID ; i++ ) {
        for(int j = 0 ; j < MAX_CHIP_STAT ; j++ ) {
            miStatus[i][j] -> Enabled = bState ;
        }
    }
}


//---------------------------------------------------------------------------
void __fastcall CArray::OneClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("(R2=%03d,C2=%03d)%s,%s",ClickStat.iSelR2,ClickStat.iSelC2 ,m_sMenuName[Tag],"OneClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;


    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    //Local Var.
    SetStat(ClickStat.iSelR2,ClickStat.iSelC2 ,(EN_CHIP_STAT)Tag);



}
//---------------------------------------------------------------------------
void __fastcall CArray::ColClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("(R2=%03d,C2=%03d)%s,%s",ClickStat.iSelR2,ClickStat.iSelC2,m_sMenuName[Tag],"ColClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    for(register int r = 0 ; r < GetMaxRow() ; r++)
        SetStat(r,ClickStat.iSelC2,(EN_CHIP_STAT)Tag);




}
//---------------------------------------------------------------------------
void __fastcall CArray::RowClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("(R2=%03d,C2=%03d)%s,%s",ClickStat.iSelR2,ClickStat.iSelC2,m_sMenuName[Tag],"RowClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    //Get RC.
    for(register int c = 0 ; c < GetMaxCol() ; c++)
        SetStat(ClickStat.iSelR2,c,(EN_CHIP_STAT)Tag);



}
//---------------------------------------------------------------------------
void __fastcall CArray::AllClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;

    AnsiString sTemp ;
    sTemp.sprintf("%s,%s",m_sMenuName[Tag],"AllClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str()) ;

    if(Tag == csToBuf) {
        //memcpy(BUFF , CHPS , sizeof(CChip) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
        return ;
    }

    if(Tag == csFromBuf) {
        //memcpy(CHPS , BUFF , sizeof(CChip) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
        return ;
    }

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    //Get RC.
    SetStat((EN_CHIP_STAT)Tag);



}
//---------------------------------------------------------------------------
void __fastcall CArray::DragClick(TObject *Sender)
{
    //Local Var.
    int Tag = ((TMenuItem *)Sender) -> Tag;
    int iRow1 , iCol1;
    int iRow2 , iCol2;

    int r1,r2,c1,c2;
    if( ClickStat.iSelR1 < ClickStat.iSelR2 ) { r1 = ClickStat.iSelR1 ; r2 = ClickStat.iSelR2; }
    else                                      { r1 = ClickStat.iSelR2 ; r2 = ClickStat.iSelR1; }
    if( ClickStat.iSelC1 < ClickStat.iSelC2 ) { c1 = ClickStat.iSelC1 ; c2 = ClickStat.iSelC2; }
    else                                      { c1 = ClickStat.iSelC2 ; c2 = ClickStat.iSelC1; }

    if(c1 < 0 ) c1 = 0;  //준선이가 함.
    if(c2 < 0 ) c2 = 0;

    ClickStat.iSelX1 = ClickStat.iSelX2 = ClickStat.iSelY1 = ClickStat.iSelY2 = 0 ;

    RangeSetStat(r1,c1,r2,c2,(EN_CHIP_STAT)Tag) ;

    AnsiString sTemp ;
    //sTemp.sprintf("(%03d,%03d)%s,%s",ClickStat.iSelR2,ClickStat.iSelC2,m_sMenuName[Tag],"RowClicked");
    sTemp.sprintf("(R1=%03d,C1=%03d)(R2=%03d,C2=%03d)%s,%s",ClickStat.iSelR1,ClickStat.iSelC1,ClickStat.iSelR2,ClickStat.iSelC2,m_sMenuName[Tag],"DragClicked");
    Trace( "Array(Chip Stats)", sTemp.c_str());



}

//---------------------------------------------------------------------------
void __fastcall CArray::ImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //To remove error.
    ClickStat.bMouseDown = true;

    imArray ->Canvas->MoveTo(X, Y);
    ClickStat.iSelX1 = X ;
    ClickStat.iSelY1 = Y ;
    ClickStat.iSelX2 = X ;
    ClickStat.iSelY2 = Y ;

    UpdateAray() ; //드레그 렉트 그릴라고.
    ImageArayRC(ClickStat.iSelX1 , ClickStat.iSelY1 , ClickStat.iSelR1 , ClickStat.iSelC1 );
    ImageArayRC(ClickStat.iSelX2 , ClickStat.iSelY2 , ClickStat.iSelR2 , ClickStat.iSelC2 );

}

//---------------------------------------------------------------------------
void __fastcall CArray::ImageMouseMove(TObject *Sender,TShiftState Shift, int X, int Y)
{
    if(!ClickStat.bMouseDown) return ;
    ClickStat.iSelX2 = X ;
    ClickStat.iSelY2 = Y ;
    ImageArayRC(ClickStat.iSelX2 , ClickStat.iSelY2 , ClickStat.iSelR2 , ClickStat.iSelC2 );
    UpdateAray();
}

//---------------------------------------------------------------------------
void __fastcall CArray::ImageMouseUp(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!ClickStat.bMouseDown) return ;
    ClickStat.bMouseDown = false ;

    POINT lpPoint ;

    GetCursorPos(&lpPoint);

    AnsiString sTemp ;

    if(ClickStat.iSelR1 == ClickStat.iSelR2 && ClickStat.iSelC1 == ClickStat.iSelC2){
        if(Shift.Contains(ssCtrl)&&Shift.Contains(ssShift)) {
            pmStatus[spAll] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
        }

        else if(Shift.Contains(ssCtrl))                     {
            pmStatus[spCol] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
        }

        else if(Shift.Contains(ssShift))                    {
            pmStatus[spRow] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
        }

        else if(Shift.Contains(ssAlt)){
            sTemp = GetSubStat(ClickStat.iSelR2 , ClickStat.iSelC2,0) ;
            for(int i = 1 ; i < MAX_SUBSTAT ; i++) {
                sTemp += "," + AnsiString(GetSubStat(ClickStat.iSelR2 , ClickStat.iSelC2,i)) ;
            }

            ShowMessage(sTemp) ;
        }

        else                                                {
            pmStatus[spOne] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
        }

    }

    else{
        pmStatus[spDrag] -> Popup(lpPoint.x + 5 , lpPoint.y + 5) ;
    }

}

void __fastcall CArray::ImageArayRC(int X , int Y , int &R , int &C)
{
    const int iDire = 0 ; //나중에 쓸일있으면 만들자.
    //Local Var.
    TColor     sColor;
    AnsiString str = "";
    int        uRow , uCol;
    int        iLeft;

    //Set Disp Dir.
    if(iDire == 0 || iDire == 2) {
        uRow = GetMaxRow() ;
        uCol = GetMaxCol() ;
    }
    else {
        uRow = GetMaxCol() ;
        uCol = GetMaxRow() ;
    }
    //Get Row, Col.
    double ImageW = (double)imArray -> Width ;
    double ImageH = (double)imArray -> Height;
    double uGw    = ImageW / (double)uCol  ;
    double uGh    = ImageH / (double)uRow  ;
    int    Urow   = (int)((double)Y / uGh) ;
    int    Ucol   = (int)((double)X / uGw) ;
    int    rRow, rCol;

    //Set Row Col.
    if      (iDire == 0) { rRow = Urow;              rCol = Ucol;              } // 0
    else if (iDire == 1) { rRow = Ucol;              rCol = (uRow - 1) - Urow; } // 90
    else if (iDire == 2) { rRow = (uRow - 1) - Urow; rCol = (uCol - 1) - Ucol; } // 180
    else if (iDire == 3) { rRow = (uCol - 1) - Ucol; rCol = Urow;              } // 270
    else if (iDire == 4) { rRow = (uCol - 1) - Ucol; rCol = (uRow - 1) - Urow; } // 270 + vert. mirror.
    else if (iDire == 5) { rRow = Urow;              rCol = (uCol - 1) - Ucol; } // Horz. Mirrror.

    //return.
    R = rRow;
    C = rCol;
}

void __fastcall CArray::ImageArayRCFix(int X , int Y , int &R , int &C)
{
    const int iDire = 0 ; //나중에 쓸일있으면 만들자.

    //Local Var.
    AnsiString str = "";
    int        uRow , uCol;

    //Set Disp Dir.
    if(iDire == 0 || iDire == 2) {
        uRow = GetMaxRow() ;
        uCol = GetMaxCol() ;
    }
    else  {
        uRow = GetMaxCol() ;
        uCol = GetMaxRow() ;
    }

    //Get Row, Col.
    int    iWidth   = imArray ->Width  / uCol  ;
    int    iHeight  = imArray ->Height / uRow  ;
    int    iXOffset = (imArray ->Width  - iWidth  * uCol)/2 ;
    int    iYOffset = (imArray ->Height - iHeight * uRow)/2 ;
    double ImageW   = (double)iWidth  * uCol ;
    double ImageH   = (double)iHeight * uRow ;
    int    Urow     = (int)((double)(Y - iYOffset) / iHeight)  ;
    int    Ucol     = (int)((double)(X - iXOffset) / iWidth )  ;

    int    rRow, rCol;

    //Set Row Col.
    if      (iDire == 0) { rRow = Urow;              rCol = Ucol;              } // 0
    else if (iDire == 1) { rRow = Ucol;              rCol = (uRow - 1) - Urow; } // 90
    else if (iDire == 2) { rRow = (uRow - 1) - Urow; rCol = (uCol - 1) - Ucol; } // 180
    else if (iDire == 3) { rRow = (uCol - 1) - Ucol; rCol = Urow;              } // 270
    else if (iDire == 4) { rRow = (uCol - 1) - Ucol; rCol = (uRow - 1) - Urow; } // 270 + vert. mirror.
    else if (iDire == 5) { rRow = Urow;              rCol = (uCol - 1) - Ucol; } // Horz. Mirrror.

    //return.
    R = rRow;
    C = rCol;
}

void __fastcall CArray::UpdateAray()
{

    if(m_bDispLock) return ;
    m_iTempDpCnt ++ ;
    if(m_bSameCellSize) UpdateArayFix() ;
    else                UpdateArayNrm() ;
}

void __fastcall CArray::UpdateArayNrm()
{
    //To remove error.
    const int iDire = 0 ;

    //Local Var.
    TColor sColor;
    char   str[10] = { 0 , };
    int    uRow , uCol;
    int    iLeft;

    //Set Disp Dir.
    uRow = GetMaxRow();
    uCol = GetMaxCol();

    if(!uRow) uRow = 1 ;
    if(!uCol) uCol = 1 ;

    int uX1, uX2, uY1, uY2;
    double uGw   = (double)imArray -> Width  / (double)uCol;
    double uGh   = (double)imArray -> Height / (double)uRow;
    int    iWOff = (int)((double)(imArray -> Width  - uGw * uCol) / 2.0);
    int    iHOff = (int)((double)(imArray -> Height - uGh * uRow) / 2.0);
    int    sRow, sCol, rRow;

    //Sleep(1);
    imArray -> Canvas -> Lock();
    imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;
    imArray -> Canvas -> Pen  -> Color = TColor(clBlack);
    imArray -> Canvas -> Pen  -> Style = psSolid        ;

    if(CheckAllStat(csNone)) {
        sColor = m_iClStat[csNone] ;

        //Drawing.
        sprintf(str , "NONE");
        imArray -> Canvas -> Brush -> Color = TColor(sColor );
        //imArray -> Canvas -> Rectangle(uX1 - 1 , uY1 - 1 , uX2 + 1 , uY2 + 1);
        imArray -> Canvas -> Rectangle(0 , 0 , imArray -> Width , imArray -> Height);
        //View Chip Info.
        imArray -> Canvas -> Font -> Name = "Times New Roman";
        imArray -> Canvas -> Font -> Size = 12;
        imArray -> Canvas -> TextOut(imArray -> Width / 2 - 12 , imArray -> Height / 2 - 12 , str);
    }
    else {
        imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;

        for (int r = 0 ; r < uRow ; r++) {
            for (int c = 0 ; c < uCol ; c++) {
                switch (iDire) {
                    case 0: sRow = r;              sCol = c;              break; // 0
                    case 1: sRow = c;              sCol = (uRow - 1) - r; break; // 90
                    case 2: sRow = (uRow - 1) - r; sCol = (uCol - 1) - c; break; // 180
                    case 3: sRow = (uCol - 1) - c; sCol = r;              break; // 270
                    case 4: sRow = (uCol - 1) - c; sCol = (uRow - 1) - r; break; // 270 + Vert. Mirror.
                    case 5: sRow = r;              sCol = (uCol - 1) - c; break; // Horz. Mirrror.
                }

                uX1 = iWOff + c * uGw       + 2;
                uX2 = iWOff + c * uGw + uGw - 2;
                uY1 = iHOff + r * uGh       + 2;
                uY2 = iHOff + r * uGh + uGh - 2;

                sColor = m_iClStat[GetStat(r,c)] ;

                //Drawing.
                sprintf(str , "%d" , sRow * uCol + sCol + 1);
                imArray -> Canvas -> Brush -> Color = TColor(sColor );
                //imArray -> Canvas -> Rectangle(uX1 - 1 , uY1 - 1 , uX2 + 1 , uY2 + 1);
                imArray -> Canvas -> Rectangle(uX1 -2 , uY1 -2 , uX2 +2 , uY2 +2);
                //View Chip Info.
                imArray -> Canvas -> Font -> Size = 6;
                //imArray -> Canvas -> TextOut(uX1 , uY1 , str);
            }
        }
    }
    //sprintf(str , GetName().c_str() );
    sprintf(str , AnsiString(m_iTempDpCnt).c_str() );
    sColor = m_iClStat[GetStat(0,0)] ;
    imArray -> Canvas -> Font -> Size = 6;
    imArray -> Canvas -> Brush -> Color = TColor(sColor );
    //imArray -> Canvas -> TextOut(1 , 1 , str);
    sprintf(str , GetName().c_str() );
    imArray -> Canvas -> TextOutA(1 , 1 , str);

    if(ClickStat.iSelX1 != ClickStat.iSelX2 || ClickStat.iSelY1 != ClickStat.iSelY2) {
        imArray->Canvas->Brush->Style = bsClear;
        imArray->Canvas->Pen->Color = clBlack;
        imArray->Canvas->Pen->Style = psDot ;
        imArray->Canvas->Pen->Width = 1;
        imArray->Canvas->Rectangle(ClickStat.iSelX1, ClickStat.iSelY1, ClickStat.iSelX2, ClickStat.iSelY2);
    }

    imArray -> Canvas -> Unlock();
}

void __fastcall CArray::UpdateArayFix()
{
    //To remove error.
    if (imArray == NULL) return;

    const int iDire = 0 ;

    //init
    int  uRow    = 0 ;
    int  uCol    = 0 ;
    int  iWidth  = 0 ;
    int  iHeight = 0 ;

    int  iXOffset = 0 ;
    int  iYOffset = 0 ;

    int  sRow, sCol  ;
    TColor sColor ;

    //Set Disp Dir.
    if(iDire == 0 || iDire == 2) {
        uRow = GetMaxRow() ;
        uCol = GetMaxCol() ;
    }
    else {
        uRow = GetMaxCol() ;
        uCol = GetMaxRow() ;
    }

    if(uRow < 1) uRow = 1 ;
    if(uCol < 1) uCol = 1 ;


    int  a = imArray -> Width ;
    a = imArray ->Width  / uCol ;
    a = imArray ->Height ;


    //iWidth  = imArray ->Width  / uCol - 1;
    //iHeight = imArray ->Height / uRow - 1;

    iWidth  = imArray ->Width  / uCol ;
    iHeight = imArray ->Height / uRow ;

    iXOffset = (imArray ->Width  - iWidth  * uCol)/2 ;
    iYOffset = (imArray ->Height - iHeight * uRow)/2 ;

    imArray -> Canvas -> Lock();
    for (register int r = 0 ; r < uRow; r++) {
        for (register int c = 0 ; c < uCol ; c++) {
            imArray -> Canvas -> Pen  -> Color = clBtnFace      ;
            imArray -> Canvas -> Pen  -> Style = psSolid        ;
            switch (iDire) {
                case 0: sRow = r;              sCol = c;              break; // 0
                case 1: sRow = c;              sCol = (uRow - 1) - r; break; // 90
                case 2: sRow = (uRow - 1) - r; sCol = (uCol - 1) - c; break; // 180
                case 3: sRow = (uCol - 1) - c; sCol = r;              break; // 270
                case 4: sRow = (uCol - 1) - c; sCol = (uRow - 1) - r; break; // 270 + Vert. Mirror.
                case 5: sRow = r;              sCol = (uCol - 1) - c; break; // Horz. Mirrror.
            }

            sColor = m_iClStat[GetStat(r,c)] ;

            imArray->Canvas->Pen  ->Mode  = pmCopy ;
            imArray->Canvas->Pen  ->Style = psSolid;
            imArray->Canvas->Pen  ->Width = 1;
            imArray->Canvas->Brush->Color = sColor ;
            imArray->Canvas->Rectangle(iXOffset + (sCol * iWidth           ), iYOffset + (sRow * iHeight            ),
                                     iXOffset + (sCol * iWidth + iWidth +1 ), iYOffset + (sRow * iHeight + iHeight +1 ));
            if(r == uRow - 1 && !(c%10) && c){
                imArray->Canvas->Pen  ->Color = clBlue  ;
                imArray->Canvas->Brush->Color = clWhite ;
                imArray->Canvas->MoveTo(c*iWidth + iXOffset ,    iYOffset               ) ;
                imArray->Canvas->LineTo(c*iWidth + iXOffset , imArray ->Height - iYOffset ) ;
                imArray->Canvas->Pen  ->Width = 1;
                imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;
                imArray -> Canvas -> Font -> Size  = 7;
                imArray->Canvas->TextOutA(c*iWidth - 1 + iXOffset ,                  iYOffset - 12 , (AnsiString)c ) ;
                imArray->Canvas->TextOutA(c*iWidth - 1 + iXOffset , imArray ->Height - iYOffset + 1  , (AnsiString)c ) ;

            }

        }


        if(!(r%10)&& r) {
            imArray->Canvas->Pen  ->Color = clBlue ;
            imArray->Canvas->Brush->Color = clWhite ;
            imArray->Canvas->MoveTo(iXOffset                , r * iHeight + iYOffset) ;
            imArray->Canvas->LineTo(imArray ->Width - iXOffset, r * iHeight + iYOffset) ;
            imArray->Canvas->Pen  ->Width = 1;
            imArray -> Canvas -> Font -> Name  = "Small Fonts"  ;
            imArray -> Canvas -> Font -> Size  = 7;
            imArray -> Canvas ->TextOut(                  iXOffset - 12 , r * iHeight - 1 + iYOffset , (AnsiString)r) ;
            imArray -> Canvas ->TextOut(imArray ->Width - iXOffset + 1  , r * iHeight - 1 + iYOffset , (AnsiString)r) ;
        }
    }

    if(ClickStat.iSelX1 != ClickStat.iSelX2 || ClickStat.iSelY1 != ClickStat.iSelY2) {
        imArray->Canvas->Brush->Style = bsClear;
        imArray->Canvas->Pen->Color = clBlack;
        imArray->Canvas->Pen->Style = psDot ;
        imArray->Canvas->Pen->Width = 1;
        imArray->Canvas->Rectangle(ClickStat.iSelX1, ClickStat.iSelY1, ClickStat.iSelX2, ClickStat.iSelY2);
    }

    imArray -> Canvas -> Unlock();

}












//---------------------------------------------------------------------------









