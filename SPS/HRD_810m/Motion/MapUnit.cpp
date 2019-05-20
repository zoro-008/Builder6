//---------------------------------------------------------------------------
#pragma hdrstop
#include "MapUnit.h"

//---------------------------------------------------------------------------
#include "OptionMan.h"
#include "UtilDefine.h"
#include "FormDeviceSet.h"
#include "FormOperation.h"
#include "DataMan.h"
#include "stdio.h"
#include "Array.h"
#include <math.h>
#include "SLogUnit.h"

#pragma package(smart_init)

#define MAX_WAFER_ROW 100
#define MAX_WAFER_COL 100

//AnsiString g_sMapList[MAX_MAP_CNT] ;

CMapUnit MAPU;
__fastcall CMapUnit::CMapUnit(void)
{
    m_slRowList = new TStringList();

}

__fastcall CMapUnit::~CMapUnit(void)
{
    m_slRowList->Clear();
    delete m_slRowList ;


}

bool CMapUnit::OpenRawMap(String _sSrcFilePath )
{
    if(!FileExists(_sSrcFilePath)) return false ;

    //m_mmRowMap -> Text = "";
    m_slRowList -> Clear();

    bool bUseHex = OM.DevInfo.bUseHex;

    //String sTemp = "";

    if(OM.DevInfo.iMapType == 0 && bUseHex){  //������ ���̵� 1�� ���� Ÿ���Ѵ�...��ǥ ������ �Ƚ��� ������ ���� ������ ���� ������ �����ؾ� ������..
                                              //�翬�� ��� ����Ѵٴ� �ɼ��� üũ �� �־�� �ϰ�...
        TMemoryStream *ms = new TMemoryStream;

        ms -> LoadFromFile(_sSrcFilePath);    // ��� ������ �ε�
        ms -> Position = 0;

        AnsiString str = "";
        BYTE bOneChar ;                       // ��� �� ���� �� �о� ���� ���� ����Ʈ ������ ����...

        //Memo1 -> ScrollBy()

        for(int i = 0; i < ms->Size ; i++){
            ms->Read(&bOneChar , 1);
            //str += bOneChar ;//"0x" + IntToHex(bOneChar,2) + " ";
            str += IntToHex(bOneChar,2) ;     //����Ʈ ������ ��縦  IntToHex �� �̿��� �� ĳ���;� ��� �о� �´�.
            if(str.Pos("0D0A")) {             //�ٹٲ�� ���� �������� �̵�
                m_slRowList -> Add(str.Trim());
                //sTemp += str ;
                str = "";

            }
        }
        delete ms;
    }
    else{
        m_slRowList -> LoadFromFile(_sSrcFilePath); //���� ��� ��� �ε� ���� �ϱ�...
    }

    return true ;

}

bool CMapUnit::Copyfile(String _sSrcFilePath , String _sDstFolderPath)
{
    AnsiString Dir;
    Dir = "Hanra";

    if(!FileExists     (_sSrcFilePath  )) return false ;
    if(!DirectoryExists(_sDstFolderPath)) {
        if (!ForceDirectories(_sDstFolderPath)) return false;

    }

    CopyFile(_sSrcFilePath.c_str(), _sDstFolderPath.c_str(), false);

    return true ;
}

bool CMapUnit::ConvertCmn(String _sSrcFilePath , EN_ARAY_ID _riDst)
{
    //Open �ÿ�... ��... Ȯ�� JS..
    DM.ARAY[_riDst].SetStat(csNone);

    if(!OpenRawMap(_sSrcFilePath                         )) {Trace("OpenRawMapfail",_sSrcFilePath.c_str());return false ;}


    if(OM.CmnOptn.sWorkedPath == "") {
        if(!Copyfile  (_sSrcFilePath , "D:\\WorkedWafer")) {Trace("Copyfile","fail");return false ;}
    }
    else {
        if(!Copyfile  (_sSrcFilePath , OM.CmnOptn.sWorkedPath)) {Trace("Copyfile","fail");return false ;}
    }

    //const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
    const int    iWFrstCol     = OM.DevInfo.iWFrstCol - 1;
    const int    iWFrstRow     = OM.DevInfo.iWFrstRow - 1;
    const int    iColCnt       = OM.DevInfo.iWfColCnt    ;
    const int    iRowCnt       = OM.DevInfo.iWfRowCnt    ;

    //const double dDevicePitchX = OM.DevInfo.dWfPitchX    ;
    //const double dDevicePitchY = OM.DevInfo.dWfPitchY    ;

    String sGoodStr      = OM.DevInfo.sGoodStr1    ;
    String sEmptStr1     = OM.DevInfo.sEmptStr1    ;
    String sMAskStr1     = OM.DevInfo.sMaskStr1    ;
    String sEmptStr2     ;//= OM.DevInfo.sEmptStr1    ;
    //20181221 ������Ʈ.���� 2���� ���´ٰ� ��.
    String sGoodStr1     = "";
    String sGoodStr2     = "";

    if(sGoodStr.Pos(","))
    {
        sGoodStr1 = sGoodStr.SubString(1,sGoodStr.Pos(",")-1);
        sGoodStr2 = sGoodStr.Delete   (1,sGoodStr.Pos(",")  );
    }
    else
    {
        sGoodStr1 = sGoodStr ;
        sGoodStr2 = "";
    }




    bool bMskExist = OM.DevInfo.bMskExist    ;

    //�����̽��ٰ� �Ⱥ����� ~���� ġ���ؼ� �Է��ϱ� ������ ���⼭ �ٽ� ġ���Ѵ�.
    sGoodStr1 = StringReplace(sGoodStr1 , "[]" , " " , TReplaceFlags()<<rfReplaceAll);
    sEmptStr1 = StringReplace(sEmptStr1 , "[]" , " " , TReplaceFlags()<<rfReplaceAll);
    sEmptStr2 = sEmptStr1 ;

    //20161011 Ư���� ��� ������..
    //RowData:___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ 000 000 000 000 000 000 000 000 000 000 ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
    //�̷��� ������ "___ "�ε� �Ǹ������� ������ �Ⱥ���ä "___" ������ ������ ��.
    //�׷��� sEmptyStr2���� ������ ������ ����� "___"�� ������ �Ǹ����� ��¥�� ��� ��.
    if(sEmptStr2.SubString(sEmptStr2.Length(),1) == " ") sEmptStr2 = sEmptStr2.SubString(1,sEmptStr2.Length()-1);



    //sMAskStr1 = StringReplace(sMAskStr1 , "[]" , " " , TReplaceFlags()<<rfReplaceAll);



    DM.ARAY[_riDst].SetMaxColRow(iColCnt , iRowCnt);

    if(iRowCnt != DM.ARAY[_riDst].GetMaxRow() ||
       iColCnt != DM.ARAY[_riDst].GetMaxCol() ) return false ;
    if(sGoodStr1  == ""                       ) return false ;


    //���� �����V �ǳʶ��.
    AnsiString sTemp , sTemp1 ;
    int iDataRowCnt = m_slRowList -> Count ;
    for(int r=0; r< iRowCnt; r++){

        //���� ���� �߸� ������ ����....20150213
        if(r+iWFrstRow>= iDataRowCnt ) break ;


                                                      //  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
        sTemp = m_slRowList->Strings[r+iWFrstRow] ;   //4"___ ___ ___ ___ ___ ___ 005 005 000 002 000 000 000 000 005 005 005 005 005 ___ ___ ___ ___ ___ ___ "

        //���� �����V ����.
        sTemp.Delete(1,iWFrstCol);

        for(int c=0; c< iColCnt ; c++) {
            sTemp1 = sTemp.SubString(c * sGoodStr1.Length() + 1, sGoodStr1.Length()) ;  //20181221 ������ 2�����ε� �Ѵ� ���̰� ���ƾ���.

            if(sTemp1 == sMAskStr1 && bMskExist ) {
                DM.ARAY[_riDst].SetStat(r,c,csEror);
            }
            else if(sTemp1 == sEmptStr1  ){
                DM.ARAY[_riDst].SetStat(r,c,csNone);
            }
            else if(sTemp1 == "" ) {
                DM.ARAY[_riDst].SetStat(r,c,csNone);
            }
            else if(sTemp1 == sGoodStr1              ) {
                DM.ARAY[_riDst].SetStat(r,c,csGood);
            }
            else if(sTemp1 == sGoodStr2){//sGoodStr2 != "" �����ǵ� �־�� �ϴµ� ������ �ɷ��±� ������ �Ⱥ�.
                DM.ARAY[_riDst].SetStat(r,c,csGood);
            }
            else if(sTemp1 == sEmptStr2              ) {
                DM.ARAY[_riDst].SetStat(r,c,csNone);
            }
            else {
                DM.ARAY[_riDst].SetStat(r,c,csFail);
            }
        }
    }

    //�������� ����ó�� �ִ� csNone csFail�� �ٲ�.
    bool bPreExist , bPstExist ;
    for(int r=1; r< iRowCnt-1; r++){
        for(int c=1; c< iColCnt-1 ; c++) {
            if(DM.ARAY[_riDst].GetStat(r,c) == csNone) {
                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < c-1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=c+1; n < iColCnt - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }
    
                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < r-1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=r+1; n < iRowCnt - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }
            }
        }
    }



    return true ;
}

bool CMapUnit::ConvertCoordi1 (String _sSrcFilePath , EN_ARAY_ID _riDst)
{
    //Open �ÿ�... ��... Ȯ�� JS..
    DM.ARAY[_riDst].SetStat(csNone);

    if(!OpenRawMap(_sSrcFilePath                         )) return false ;
    if(!Copyfile  (_sSrcFilePath , OM.CmnOptn.sWorkedPath)) return false ;

    //m_sRowMap.LoadStr()

    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
    const int    iColCnt       = OM.DevInfo.iWfColCnt    ;
    const int    iRowCnt       = OM.DevInfo.iWfRowCnt    ;



    DM.ARAY[_riDst].SetMaxColRow(iColCnt,iRowCnt);

    TStringList *sList = new TStringList;

    sList -> Delimiter = ',' ;

    AnsiString sRow , sCol , sChip , sMask ;

    int iMaxLine = 3721;
    int iMinRow , iMinCol;
    int r , c;

    // row , col �ְ� �� ���ϱ�
    AnsiString sTemp , sTemp1 ;
    for(int iLine = 0 ; iLine < iMaxLine ; iLine ++){
        sTemp = m_slRowList->Strings[iLine+133] ;

        sTemp.Delete(1,0);

        sList -> DelimitedText = sTemp;

        sCol  = sList -> Strings[0] ; sRow = sList -> Strings[1] ;

        if(sRow.ToInt() < iMinRow) iMinRow = sRow.ToInt();
        if(sCol.ToInt() < iMinCol) iMinCol = sCol.ToInt();
    }

    for(int a = 0 ; a < iMaxLine ; a++){
        sTemp = m_slRowList->Strings[a+133] ;

        sTemp.Delete(1,0)     ;
        sTemp  = sTemp + "/r/n"       ;
        sList -> DelimitedText = sTemp;

        sCol  = sList -> Strings[0] ; sRow = sList -> Strings[1] ;
        // �ְ��� sRow,sCol�� ���ؼ� ����� ��ȯ�� ��ǥ ��ġ ���ϱ�
        r  = sRow.ToInt() - iMinRow;
        c  = sCol.ToInt() - iMinCol;

        sChip = sList -> Strings[3];

        //TODO :: sMark�� File���� �ҷ� �ö� X, Y, A, B �߿� A �ҷ���.. Ȯ�� �� ����������..JS
        sMask = sList -> Strings[2];

             if(sMask == "128") DM.ARAY[_riDst].SetStat(r,c,csEror);
        else if(sChip == "0"  ) DM.ARAY[_riDst].SetStat(r,c,csNone);
        else if(sChip == "1"  ) DM.ARAY[_riDst].SetStat(r,c,csGood);
        else                    DM.ARAY[_riDst].SetStat(r,c,csFail);
    }

    //�������� ����ó�� �ִ� csNone csFail�� �ٲ�.
    bool bPreExist , bPstExist ;
    for(int r=1; r< 61-1; r++){
        for(int c=1; c< 61-1 ; c++) {
            if(DM.ARAY[_riDst].GetStat(r,c) == csNone) {
                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < c-1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=c+1; n < 61 - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }

                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < r-1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=r+1; n < 61 - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }
            }
        }
    }



    delete sList;

    return true ;
}

bool CMapUnit::ConvertCoordi2 (String _sSrcFilePath , EN_ARAY_ID _riDst)
{
    //Open �ÿ�... ��... Ȯ�� JS..
    DM.ARAY[_riDst].SetStat(csNone);

    if(!OpenRawMap(_sSrcFilePath                         )) return false ;
    if(!Copyfile  (_sSrcFilePath , OM.CmnOptn.sWorkedPath)) return false ;

    //m_sRowMap.LoadStr()


    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
    const int    iColCnt       = OM.DevInfo.iWfColCnt    ;
    const int    iRowCnt       = OM.DevInfo.iWfRowCnt    ;
    //�����̽��ٰ� �Ⱥ����� ~���� ġ���ؼ� �Է��ϱ� ������ ���⼭ �ٽ� ġ���Ѵ�.

    DM.ARAY[_riDst].SetMaxColRow(iColCnt,iRowCnt);

    DM.ARAY[_riDst].SetStat(csNone);

    TStringList *sList = new TStringList;

    sList -> Delimiter = ',' ;

    AnsiString sRow , sCol , sChip ;

    AnsiString sTemp , sTemp1 ;
    AnsiString sTemp2 ;
    int iMaxLine = 2352;
    int iMinRow , iMinCol;
    int r , c;

    // row , col �ְ� �� ���ϱ�
    for(int iLine = 0 ; iLine < iMaxLine ; iLine ++){
        sTemp = m_slRowList->Strings[iLine+43] ;

        sTemp.Delete(1,6);

        sList -> DelimitedText = sTemp;

        sCol  = sList -> Strings[0] ; sRow = sList -> Strings[1] ;

        if(sRow.ToInt() < iMinRow) iMinRow = sRow.ToInt();
        if(sCol.ToInt() < iMinCol) iMinCol = sCol.ToInt();
    }

    for(int a = 0 ; a < iMaxLine ; a++){
        sTemp = m_slRowList->Strings[a+43] ;

        sTemp.Delete(1,6)     ;
        sTemp  = sTemp + "/r/n"       ;
        sList -> DelimitedText = sTemp;

        sCol  = sList -> Strings[0] ; sRow = sList -> Strings[1] ;

        // �ְ��� sRow,sCol�� ���ؼ� ����� ��ȯ�� ��ǥ ��ġ ���ϱ�
        r  = sRow.ToInt() - iMinRow;
        c  = sCol.ToInt() - iMinCol;

        sChip = sList -> Strings[3].Delete(0,2) ;

             if (sChip == "1") DM.ARAY[_riDst].SetStat(r,c,csGood);
      //else if (sChip == "3") DM.ARAY[_riDst].SetStat(r,c,csEror);   // TODO :: �ܰ��� Fail�� ����... SP�ݵ�ü���� ��� �ҷ����� �𸣱� ������ �ּ�..JS 
        else                   DM.ARAY[_riDst].SetStat(r,c,csFail);
    }

    //�������� ����ó�� �ִ� csNone csFail�� �ٲ�.
    bool bPreExist , bPstExist ;
    for(int r=1; r< 53-1; r++){
        for(int c=1; c< 54-1 ; c++) {
            if(DM.ARAY[_riDst].GetStat(r,c) == csNone) {
                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < c-1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=c+1; n < 54 - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }

                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < r-1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=r+1; n < 53 - 1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }
            }
        }
    }



    delete sList;

    return true ;
}

bool CMapUnit::ConvertCoordi8in (String _sSrcFilePath , EN_ARAY_ID _riDst)
{
    /*
    if(!OpenRawMap(_sSrcFilePath                         )) return false ;

    if(!Copyfile  (_sSrcFilePath , OM.CmnOptn.sWorkedPath)) return false ;

    //m_sRowMap.LoadStr()

    //const int iLastLine  = OM.DevInfo.iMapLstLine ;
    const int iFlatAngle = OM.DevInfo.iFlatAngle  ;

    DM.ARAY[_riDst].SetStat(csNone);

    AnsiString sRow , sCol , sChip ,ss ;

    AnsiString sTemp , sTemp1 ;
    AnsiString sTemp2 ;
    //int iMaxLine = 10362;
    int iMinRow , iMinCol = -1;
    int iMaxRow , iMaxCol = -1;
    int iRowCnt , iColCnt ;
    int r , c , g;

    // row , col �ְ� �� ���ϱ�
    for(int iLine = 0 ; iLine < iLastLine ; iLine += 3){

        //X= 0192 Y= 0136 B= 006 D= 001 S= 0001
        sTemp = m_slRowList->Strings[iLine+26] ;

        sTemp.Delete(sTemp.Pos("X= ") , 3);
        sCol  = sTemp.SubString(1,sTemp.Pos(" ")) ;
        sTemp.Delete(1,sTemp.Pos(" ")) ;
        sTemp.Delete(sTemp.Pos("Y= ") , 3);
        sRow  = sTemp.SubString(1,sTemp.Pos(" ")) ;

        sRow  = sRow.Trim();
        sCol  = sCol.Trim();

        if(iMinRow == -1 || iMinCol == -1){
            iMinRow = sRow.ToInt();
            iMinCol = sCol.ToInt();
        }

        if(sRow.ToInt() < iMinRow) iMinRow = sRow.ToInt();
        if(sCol.ToInt() < iMinCol) iMinCol = sCol.ToInt();

        if(iMaxRow == -1 || iMaxCol == -1){
            iMaxRow = sRow.ToInt();
            iMaxCol = sCol.ToInt();
        }

        if(sRow.ToInt() > iMaxRow) iMaxRow = sRow.ToInt();
        if(sCol.ToInt() > iMaxCol) iMaxCol = sCol.ToInt();
    }

    //DM.ARAY[_riDst].SetMaxColRow(iWfColCnt ,iWfRowCnt);
    iRowCnt = iMaxRow - iMinRow + 1;
    iColCnt = iMaxCol - iMinCol + 1;
    DM.ARAY[_riDst].SetMaxColRow(iColCnt , iRowCnt);

    for(int a = 0 ; a < iLastLine ; a += 3){
        sTemp = m_slRowList->Strings[a+26] ;

        sTemp  = sTemp + "/r/n"       ;

        //sList -> DelimitedText = sTemp;
        sTemp.Delete(sTemp.Pos("X= ") , 3);
        sCol  = sTemp.SubString(1,sTemp.Pos(" ")) ;
        sTemp.Delete(1,sTemp.Pos(" ")) ;
        sTemp.Delete(sTemp.Pos("Y= ") , 3);
        sRow  = sTemp.SubString(1,sTemp.Pos(" ")) ;
        sTemp.Delete(1,sTemp.Pos(" ")) ;
        sTemp.Delete(sTemp.Pos("B= ") , 3);
        sChip = sTemp.SubString(1,sTemp.Pos(" ")) ;


        sRow  =  sRow.Trim();
        sCol  =  sCol.Trim();
        sChip = sChip.Trim();

        // �ְ��� sRow,sCol�� ���ؼ� ����� ��ȯ�� ��ǥ ��ġ ���ϱ�
        r  = sRow.ToInt() - iMinRow;
        c  = sCol.ToInt() - iMinCol;
        g = sChip.ToInt();

             if (g == 1 ) DM.ARAY[_riDst].SetStat(r,c,csGood);
        //else if (g == 6 ) DM.ARAY[_riDst].SetStat(r,c,csNone);
        else              DM.ARAY[_riDst].SetStat(r,c,csFail);
    }

    //�������� ����ó�� �ִ� csNone csFail�� �ٲ�.
    bool bPreExist , bPstExist ;
    for(int r=1; r< iRowCnt; r++){
        for(int c=1; c< iColCnt; c++) {
            if(DM.ARAY[_riDst].GetStat(r,c) == csNone) {
                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < c-1; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=c+1; n < iColCnt; n++){
                    if(DM.ARAY[_riDst].GetStat(r,n) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }

                //���� Ȯ��.
                bPreExist = false ;
                bPstExist = false ;
                for(int n=1; n < r-1; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPreExist = true ; break ;}
                }
                for(int n=r+1; n < iRowCnt; n++){
                    if(DM.ARAY[_riDst].GetStat(n,c) != csNone) {bPstExist = true ; break ;}
                }
                if(bPreExist && bPstExist) {
                    DM.ARAY[_riDst].SetStat(r, c, csFail);
                    continue ;
                }
            }
        }
    }


         if(iFlatAngle == 0) {                            }
    else if(iFlatAngle == 1) {DM.ARAY[_riDst].TurnCw90 ();}
    else if(iFlatAngle == 2) {DM.ARAY[_riDst].TurnCw180();}
    else if(iFlatAngle == 3) {DM.ARAY[_riDst].TurnCw270();}

    //delete sList;

    return true ;     */
}





bool CMapUnit::SetMask(EN_ARAY_ID _riDst)
{


    DM.ARAY[_riDst].ClearMap();
    //DM.ARAY[_riDst].SetStat(csNone);

    const int iMskLine = OM.DevInfo.iMskLine;
    const int iRowCnt  = (iMskLine*2) + OM.DevInfo.iWfRowCnt;
    const int iColCnt  = (iMskLine*2) + OM.DevInfo.iWfColCnt;

    AnsiString sPath = FrmDeviceSet -> odMap -> FileName;

    if(OM.DevInfo.iMapType == 0){
        ConvertCmn(sPath , riSTG); //���������� �� �ӽ÷� �ѷ��ְ�
    }

    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
    DM.ARAY[_riDst].SetMaxColRow(iColCnt , iRowCnt);



    // ����ũ�� �����V �ְ� ���������� �ӽ÷� �ѷ��� ������ ��������
    for(int r = 0 ; r <OM.DevInfo.iWfRowCnt /*+ iMskLine*/ ; r++){
        for(int c = 0 ; c <OM.DevInfo.iWfColCnt /*+ iMskLine*/ ; c++){
            DM.ARAY[_riDst].SetStat(r+iMskLine,c+iMskLine,DM.ARAY[riSTG].GetStat(r,c));
        }
    }

    //���� ����ũ ���� �ѷ��ֱ�...����ũ ������ 2ĭ ��
    for(int r = 0 ; r < iRowCnt ; r++){
        for(int c = 0 ; c < iColCnt ; c++){
            if(DM.ARAY[_riDst].GetStat(r,c-iMskLine) == csNone && DM.ARAY[_riDst].GetStat(r,c) != csNone && DM.ARAY[_riDst].GetStat(r,c) != csEror){
                   DM.ARAY[_riDst].SetStat(r,c-iMskLine,csEror);
            }
            if(DM.ARAY[_riDst].GetStat(r,c+iMskLine) == csNone && DM.ARAY[_riDst].GetStat(r,c) != csNone && DM.ARAY[_riDst].GetStat(r,c) != csEror){
                   DM.ARAY[_riDst].SetStat(r,c+iMskLine,csEror);
            }
        }
    }
    //���� ����ũ ���� �ѷ��ֱ�...����ũ ������ 2ĭ ��
    for(int c = 0 ; c < iColCnt ; c++){
        for(int r = 0 ; r < iRowCnt ; r++){
            if(DM.ARAY[_riDst].GetStat(r-iMskLine,c) == csNone && DM.ARAY[_riDst].GetStat(r,c) != csNone && DM.ARAY[_riDst].GetStat(r,c) != csEror){
                   DM.ARAY[_riDst].SetStat(r-iMskLine,c,csEror);
            }
            if(DM.ARAY[_riDst].GetStat(r+iMskLine,c) == csNone && DM.ARAY[_riDst].GetStat(r,c) != csNone && DM.ARAY[_riDst].GetStat(r,c) != csEror){
                   DM.ARAY[_riDst].SetStat(r+iMskLine,c,csEror);
            }
        }
    }
    //     if(iFlatAngle == 0) {                            }/*0  */
    //else if(iFlatAngle == 1) {DM.ARAY[_riDst].TurnCw90 ();}/*90 */
    //else if(iFlatAngle == 2) {DM.ARAY[_riDst].TurnCw180();}/*180*/
    //else if(iFlatAngle == 3) {DM.ARAY[_riDst].TurnCw270();}/*270*/

   // DM.ARAY[riSTG].SetStat(csNone);




    return true;



}


bool CMapUnit::MergeArray(EN_ARAY_ID _riMsk , EN_ARAY_ID _riMap , EN_ARAY_ID _riDst )
{
    int iFrstGoodR , iFrstGoodC , iFrstFailR , iFrstFailC ;
    int iOriR , iOriC ;
    int iOfsR , iOfsC ;

    bool bGood ;
    bool bMskExist = OM.DevInfo.bMskExist ;

    if(!DM.ARAY[_riMsk].FindFrstRowCol(csOrig , iOriR  , iOriC  )) return false ;  // ����Ĩ Ȯ��

    // ����ũ���� ù �����ϴ� good fail ��ġ
    if(DM.ARAY[_riMsk].FindFrstRow(csGood)) { iFrstGoodR = DM.ARAY[_riMsk].FindFrstRow(csGood);} //else { return false/*iFrstGoodR = OM.DevInfo.iWfRowCnt*/ ;}
    if(DM.ARAY[_riMsk].FindFrstCol(csGood)) { iFrstGoodC = DM.ARAY[_riMsk].FindFrstCol(csGood);} //else { return false/*iFrstGoodC = OM.DevInfo.iWfColCnt*/ ;}
    if(DM.ARAY[_riMsk].FindFrstRow(csFail)) { iFrstFailR = DM.ARAY[_riMsk].FindFrstRow(csFail);} //else { return false/*iFrstFailR = OM.DevInfo.iWfRowCnt*/ ;}
    if(DM.ARAY[_riMsk].FindFrstCol(csFail)) { iFrstFailC = DM.ARAY[_riMsk].FindFrstCol(csFail);} //else { return false/*iFrstFailC = OM.DevInfo.iWfColCnt*/ ;}

    if(iFrstGoodR <= 0) iFrstGoodR = OM.DevInfo.iWfRowCnt ;
    if(iFrstGoodC <= 0) iFrstGoodC = OM.DevInfo.iWfColCnt ;
    if(iFrstFailR <= 0) iFrstFailR = OM.DevInfo.iWfRowCnt ;
    if(iFrstFailC <= 0) iFrstFailC = OM.DevInfo.iWfColCnt ;

    DM.ARAY[_riDst] = DM.ARAY[_riMsk] ; //���������� ����ũ �� ������ ���� �����...

    if(iFrstGoodR < iFrstFailR) { iOfsR = iFrstGoodR; } else { iOfsR = iFrstFailR; }
    if(iFrstGoodC < iFrstFailC) { iOfsC = iFrstGoodC; } else { iOfsC = iFrstFailC; }  // �����V

    //DM.ARAY[_riDst].ChangeStat(csOrig , csEror);

    if(DM.ARAY[_riMap].GetStat(iOriR , iOriC) != csGood){
        DM.ARAY[_riMap].SetStat(iOriR , iOriC , csFail); //bGood = false;
    }
    else{
        DM.ARAY[_riMap].SetStat(iOriR , iOriC ,  csGood); //bGood = true;
    }

    if(!bMskExist){
    //�������� ����ũ ������ �����...
        for(int c = 0 ; c < DM.ARAY[_riMap].GetMaxCol() ; c++) {
            for(int r = 0 ; r < DM.ARAY[_riMap].GetMaxRow() ; r++) {
                     if(DM.ARAY[_riMap].GetStat(r,c) == csGood) DM.ARAY[_riDst].SetStat(r+iOfsR , c+iOfsC , csGood ) ;
                else if(DM.ARAY[_riMap].GetStat(r,c) == csFail) DM.ARAY[_riDst].SetStat(r+iOfsR , c+iOfsC , csFail ) ;
            }
        }
    }
    else {
        DM.ARAY[_riDst] = DM.ARAY[_riMap];
        if(DM.ARAY[_riDst].GetStat(iOriR , iOriC) != csGood){
            DM.ARAY[_riDst].SetStat(iOriR , iOriC , csFail); //bGood = false;
        }
        else{
            DM.ARAY[_riDst ].SetStat(iOriR , iOriC ,  csGood); //bGood = true;
        }
    }


    return true ;


}

bool CMapUnit::ChangeMaskNone (String _sSrcFilePath , int _iMapType , EN_ARAY_ID _riDst)
{

         if(_iMapType == 0) {if(!ConvertCmn      (_sSrcFilePath , _riDst)) return false;}
    else if(_iMapType == 1) {if(!ConvertCoordi1  (_sSrcFilePath , _riDst)) return false;}
    else if(_iMapType == 2) {if(!ConvertCoordi2  (_sSrcFilePath , _riDst)) return false;}
    else                    {if(!ConvertCoordi8in(_sSrcFilePath , _riDst)) return false;}

    AnsiString sPath = _sSrcFilePath; //FrmDeviceSet -> odMap -> FileName;

    ConvertCmn(sPath , _riDst);

    int iCount = 0;

    int iMaxRow = DM.ARAY[_riDst].GetMaxRow();
    int iMaxCol = DM.ARAY[_riDst].GetMaxCol();
    for(int i=0 ; i <= iMaxRow ; i++){
        for(int j =0 ; j <= iMaxCol ; j++) {
            if(DM.ARAY[_riDst].GetStat(i,j  ) == csEror) {
                if(DM.ARAY[_riDst].GetStat(i-1,j  ) == csGood ||
                   DM.ARAY[_riDst].GetStat(i-1,j  ) == csFail ){ iCount++; }
                if(DM.ARAY[_riDst].GetStat(i  ,j-1) == csGood ||
                   DM.ARAY[_riDst].GetStat(i  ,j-1) == csFail ){ iCount++; }
                if(DM.ARAY[_riDst].GetStat(i+1,j  ) == csGood ||
                   DM.ARAY[_riDst].GetStat(i+1,j  ) == csFail ){ iCount++; }
                if(DM.ARAY[_riDst].GetStat(i  ,j+1) == csGood ||
                   DM.ARAY[_riDst].GetStat(i  ,j+1) == csFail ){ iCount++; }
           }
           if(iCount >3) { DM.ARAY[_riDst].SetStat(i,j,csFail); }

           iCount = 0 ;
        }
    }

    //DM.ARAY[_riDst].UpdateAray();

    return true;
}

bool CMapUnit::DeleteNullColRow (EN_ARAY_ID _riMap , EN_ARAY_ID _riDst)
{
    if(OM.DevInfo.bMskExist){

        int iStartR , iStartC , iEndR , iEndC ;
        iStartC = DM.ARAY[_riMap].FindFrstCol(csEror);
        iStartR = DM.ARAY[_riMap].FindFrstRow(csEror);
        iEndC   = DM.ARAY[_riMap].FindLastCol(csEror);
        iEndR   = DM.ARAY[_riMap].FindLastRow(csEror);

        DM.ARAY[_riDst].SetMaxColRow(iEndC - iStartC+1 , iEndR - iStartR+1);

        int iRow,iCol;
        iRow = DM.ARAY[_riMap].GetMaxRow();
        iCol = DM.ARAY[_riMap].GetMaxCol();

        for(int r = 0 ; r < iRow ; r++){
            for(int c = 0 ; c < iCol ; c++){
                DM.ARAY[_riDst].SetStat(r,c,DM.ARAY[_riMap].GetStat(r+iStartR,c+iStartC ));
            }
        }
        return true;

    }
    else{
        int iGoodR , iGoodC ,iFailR , iFailC , iMaskR , iMaskC , iOfSetC , iOfSetR;
        int iLGoodR , iLGoodC ,iLFailR , iLFailC , iLMaskR , iLMaskC , iLOfSetC , iLOfSetR;

        iMaskC  = DM.ARAY[_riMap].FindFrstCol(csEror); iGoodC  = DM.ARAY[_riMap].FindFrstCol(csGood); iFailC  = DM.ARAY[_riMap].FindFrstCol(csFail);
        iMaskR  = DM.ARAY[_riMap].FindFrstRow(csEror); iGoodR  = DM.ARAY[_riMap].FindFrstRow(csGood); iFailR  = DM.ARAY[_riMap].FindFrstRow(csFail);
        iLMaskC = DM.ARAY[_riMap].FindLastCol(csEror); iLGoodC = DM.ARAY[_riMap].FindLastCol(csGood); iLFailC = DM.ARAY[_riMap].FindLastCol(csFail);
        iLMaskR = DM.ARAY[_riMap].FindLastRow(csEror); iLGoodR = DM.ARAY[_riMap].FindLastRow(csGood); iLFailR = DM.ARAY[_riMap].FindLastRow(csFail);


        if(iMaskC <= iGoodC){ if(iMaskC <= iFailC) { iOfSetC = iMaskC; }
                              else                 { iOfSetC = iFailC; }}
        else                { if(iGoodC <= iFailC) { iOfSetC = iGoodC; }
                              else                 { iOfSetC = iFailC; }}

        if(iMaskR <= iGoodR){ if(iMaskR <= iFailR) { iOfSetR = iMaskR; }
                              else                 { iOfSetR = iFailR; }}
        else                { if(iGoodR <= iFailR) { iOfSetR = iGoodR; }
                              else                 { iOfSetR = iFailR; }}

        if(iLMaskC <= iLGoodC){ if(iLMaskC <= iLFailC) { iLOfSetC = iLMaskC; }
                                else                   { iLOfSetC = iLFailC; }}
        else                  { if(iLGoodC <= iLFailC) { iLOfSetC = iLGoodC; }
                                else                   { iLOfSetC = iLFailC; }}

        if(iLMaskR <= iLGoodR){ if(iLMaskR <= iLFailR) { iLOfSetR = iLMaskR; }
                                else                   { iLOfSetR = iLFailR; }}
        else                  { if(iLGoodR <= iLFailR) { iLOfSetR = iLGoodR; }
                                else                   { iLOfSetR = iLFailR; }}

        DM.ARAY[_riDst].SetMaxColRow(iLOfSetC - iOfSetC+1 , iLOfSetR - iOfSetR+1);
        int iRow,iCol;
        iRow = DM.ARAY[_riMap].GetMaxRow();
        iCol = DM.ARAY[_riMap].GetMaxCol();

        for(int r = 0 ; r < iRow ; r++){
            for(int c = 0 ; c < iCol ; c++){
                DM.ARAY[_riDst].SetStat(r,c,DM.ARAY[_riMap].GetStat(r+iOfSetR,c+iOfSetC ));
            }
        }


        return true;
    }



    //;



}


