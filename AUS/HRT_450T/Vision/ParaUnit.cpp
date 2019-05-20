//---------------------------------------------------------------------------


#pragma hdrstop

#include "ParaUnit.h"
#include "UserIni.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void MakeComboBox(TValueListEditor * _vlList , AnsiString _sKey , int _iItemCnt , AnsiString * _sItem , int _iActivItem)
{
    int iKeyRow = 0 ;
    for(int i = 0 ; i < _vlList -> RowCount ; i++) {
        if(_vlList -> Cells[1][i] == _sKey) {
            iKeyRow = i ;
            break ;
        }
    }
    TStringList * slItemList = new TStringList ();
    AnsiString sTemp ;
    for(int i = 0 ; i < _iItemCnt ; i++) {
        sTemp = _sItem[i] ;
        slItemList -> Add(sTemp);
    }

    _vlList -> ItemProps[iKeyRow]->EditStyle = esPickList ;
    _vlList -> ItemProps[iKeyRow]->ReadOnly = true ;
    _vlList -> ItemProps[iKeyRow]->PickList = slItemList ;
    _vlList -> Cells    [1][iKeyRow] = _sItem[_iActivItem] ;

    delete slItemList ;
}

void MakeBoolComboBox(TValueListEditor * _vlList , AnsiString _sKey , bool _bActivStat)
{
    int iKeyRow = 0 ;
    for(int i = 0 ; i < _vlList -> RowCount ; i++) {
        if(_vlList -> Cells[1][i] == _sKey) {
            iKeyRow = i ;
            break ;
        }
    }
    TStringList * slItemList = new TStringList ();
    slItemList -> Add("False");
    slItemList -> Add("True" );

    _vlList -> ItemProps[iKeyRow]->EditStyle = esPickList ;
    _vlList -> ItemProps[iKeyRow]->ReadOnly = true ;
    _vlList -> ItemProps[iKeyRow]->PickList = slItemList ;
    if(_bActivStat) _vlList -> Cells [1][iKeyRow] = "True"  ;
    else            _vlList -> Cells [1][iKeyRow] = "False" ;

    delete slItemList ;
}

int GetItemIndex(TValueListEditor * _vlList , AnsiString _sKey )
{
    int iKeyRow = 0 ;
    for(int i = 0 ; i < _vlList -> RowCount ; i++) {
        if(_vlList -> Cells[1][i] == _sKey) {
            iKeyRow = i ;
            break ;
        }
    }
    return _vlList -> ItemProps[iKeyRow]->PickList-> IndexOf(_vlList -> Cells [1][iKeyRow]);
}

void InputSpliter(TValueListEditor * _vlList , AnsiString _sCaption)
{
    _vlList -> Values[_sCaption] = "";
    _vlList -> ItemProps[_vlList -> RowCount-1]->ReadOnly = true ;
}

/******************************************************************************/
/* Edge                                                                       */
/******************************************************************************/
void LoadParaEdg(bool _bLoad , EDG_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId + "_Edge" ;

    AnsiString sFilePath = _sFilePath ;

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iAverage ", _tPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iCompare ", _tPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iChkPxLow", _tPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iChkPxHgh", _tPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iOfsGap  ", _tPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iOfsPxLow", _tPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iOfsPxHgh", _tPara.iOfsPxHgh);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iAverage ", _tPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iCompare ", _tPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iChkPxLow", _tPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iChkPxHgh", _tPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iOfsGap  ", _tPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iOfsPxLow", _tPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iOfsPxHgh", _tPara.iOfsPxHgh);
    }
}
void UpdateParaEdg(bool _bToTable , TValueListEditor * _vlList , EDG_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["평균범위        "] = _tPara.iAverage ;
        _vlList -> Values["비교범위        "] = _tPara.iCompare ;
        _vlList -> Values["엦지밝기Low     "] = _tPara.iChkPxLow;
        _vlList -> Values["엦지밝기High    "] = _tPara.iChkPxHgh;
        _vlList -> Values["확정옵셑거리    "] = _tPara.iOfsGap  ;
        _vlList -> Values["확정옵셑밝기Low "] = _tPara.iOfsPxLow;
        _vlList -> Values["확정옵셑밝기High"] = _tPara.iOfsPxHgh;
    }
    else {
        _tPara.iAverage  = StrToIntDef(_vlList -> Values["평균범위        "],_tPara.iAverage ) ;
        _tPara.iCompare  = StrToIntDef(_vlList -> Values["비교범위        "],_tPara.iCompare ) ;
        _tPara.iChkPxLow = StrToIntDef(_vlList -> Values["엦지밝기Low     "],_tPara.iChkPxLow) ;
        _tPara.iChkPxHgh = StrToIntDef(_vlList -> Values["엦지밝기High    "],_tPara.iChkPxHgh) ;
        _tPara.iOfsGap   = StrToIntDef(_vlList -> Values["확정옵셑거리    "],_tPara.iOfsGap  ) ;
        _tPara.iOfsPxLow = StrToIntDef(_vlList -> Values["확정옵셑밝기Low "],_tPara.iOfsPxLow) ;
        _tPara.iOfsPxHgh = StrToIntDef(_vlList -> Values["확정옵셑밝기High"],_tPara.iOfsPxHgh) ;
        UpdateParaEdg(true , _vlList , _tPara);
    }
}

/******************************************************************************/
/* Line                                                                       */
/******************************************************************************/
void LoadParaLin (bool _bLoad , LIN_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Line" ;

    AnsiString sFilePath = _sFilePath ;

    int iTemp ;

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iScanDirect", iTemp             );_tPara.iScanDirect=(EN_LINE_SCAN_DIRECT)iTemp ;
        UserINI.Load(sFilePath, sTemp , "bLtToDk    ", _tPara.bLtToDk    );
        UserINI.Load(sFilePath, sTemp , "iSampleGap ", _tPara.iSampleGap );
        UserINI.Load(sFilePath, sTemp , "iLineGap   ", _tPara.iLineGap   );
        UserINI.Load(sFilePath, sTemp , "iEndOffset ", _tPara.iEndOffset );
        UserINI.Load(sFilePath, sTemp , "iCntrOffset", _tPara.iCntrOffset);

        UserINI.Load(sFilePath, sTemp , "iEdgAverage   ", _tPara.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iEdgCompare   ", _tPara.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxLow  ", _tPara.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxHgh  ", _tPara.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsGap    ", _tPara.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxLow  ", _tPara.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxHgh  ", _tPara.EdgPara.iOfsPxHgh);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iScanDirect", _tPara.iScanDirect);
        UserINI.Save(sFilePath, sTemp , "bLtToDk    ", _tPara.bLtToDk    );
        UserINI.Save(sFilePath, sTemp , "iSampleGap ", _tPara.iSampleGap );
        UserINI.Save(sFilePath, sTemp , "iLineGap   ", _tPara.iLineGap   );
        UserINI.Save(sFilePath, sTemp , "iEndOffset ", _tPara.iEndOffset );
        UserINI.Save(sFilePath, sTemp , "iCntrOffset", _tPara.iCntrOffset);

        UserINI.Save(sFilePath, sTemp , "iEdgAverage ", _tPara.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iEdgCompare ", _tPara.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxLow", _tPara.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxHgh", _tPara.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsGap  ", _tPara.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow", _tPara.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh", _tPara.EdgPara.iOfsPxHgh);
    }
}
void UpdateParaLin(bool _bToTable , TValueListEditor * _vlList , LIN_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["검색방향          "] = _tPara.iScanDirect;
        _vlList -> Values["밝은곳에서어두운곳"] = _tPara.bLtToDk?1:0;
        _vlList -> Values["샘플링 간격       "] = _tPara.iSampleGap ;
        _vlList -> Values["라인인정범위      "] = _tPara.iLineGap   ;
        _vlList -> Values["검색 시작끝 오프셑"] = _tPara.iEndOffset ;
        _vlList -> Values["검색 중간 오프셑  "] = _tPara.iCntrOffset;

        _vlList -> Values["Eg평균범위        "] = _tPara.EdgPara.iAverage ;
        _vlList -> Values["Eg비교범위        "] = _tPara.EdgPara.iCompare ;
        _vlList -> Values["Eg엦지밝기Low     "] = _tPara.EdgPara.iChkPxLow;
        _vlList -> Values["Eg엦지밝기High    "] = _tPara.EdgPara.iChkPxHgh;
        _vlList -> Values["Eg확정옵셑거리    "] = _tPara.EdgPara.iOfsGap  ;
        _vlList -> Values["Eg확정옵셑밝기Low "] = _tPara.EdgPara.iOfsPxLow;
        _vlList -> Values["Eg확정옵셑밝기High"] = _tPara.EdgPara.iOfsPxHgh;

        AnsiString sItems[4] = {"Down","Up","Right","Left"};
        MakeComboBox(_vlList , "검색방향" , 4 , sItems  , _tPara.iScanDirect);
        MakeBoolComboBox(_vlList , "밝은곳에서어두운곳" , _tPara.bLtToDk);
    }
    else {
        _tPara.iScanDirect       = (EN_LINE_SCAN_DIRECT)GetItemIndex(_vlList          ,"검색방향          ");
        _tPara.bLtToDk           = GetItemIndex(_vlList          ,"밝은곳에서어두운곳");
        _tPara.iSampleGap        = StrToIntDef (_vlList -> Values["샘플링 간격       "],_tPara.iSampleGap ) ;
        _tPara.iLineGap          = StrToIntDef (_vlList -> Values["라인인정범위      "],_tPara.iLineGap   ) ;
        _tPara.iEndOffset        = StrToIntDef (_vlList -> Values["검색 시작끝 오프셑"],_tPara.iEndOffset ) ;
        _tPara.iCntrOffset       = StrToIntDef (_vlList -> Values["검색 중간 오프셑  "],_tPara.iCntrOffset) ;

        _tPara.EdgPara.iAverage  = StrToIntDef (_vlList -> Values["Eg평균범위        "],_tPara.EdgPara.iAverage ) ;
        _tPara.EdgPara.iCompare  = StrToIntDef (_vlList -> Values["Eg비교범위        "],_tPara.EdgPara.iCompare ) ;
        _tPara.EdgPara.iChkPxLow = StrToIntDef (_vlList -> Values["Eg엦지밝기Low     "],_tPara.EdgPara.iChkPxLow) ;
        _tPara.EdgPara.iChkPxHgh = StrToIntDef (_vlList -> Values["Eg엦지밝기High    "],_tPara.EdgPara.iChkPxHgh) ;
        _tPara.EdgPara.iOfsGap   = StrToIntDef (_vlList -> Values["Eg확정옵셑거리    "],_tPara.EdgPara.iOfsGap  ) ;
        _tPara.EdgPara.iOfsPxLow = StrToIntDef (_vlList -> Values["Eg확정옵셑밝기Low "],_tPara.EdgPara.iOfsPxLow) ;
        _tPara.EdgPara.iOfsPxHgh = StrToIntDef (_vlList -> Values["Eg확정옵셑밝기High"],_tPara.EdgPara.iOfsPxHgh) ;

        UpdateParaLin(true , _vlList , _tPara);
    }
}

/******************************************************************************/
/* Line Cross Point                                                           */
/******************************************************************************/
void LoadParaLcp(bool _bLoad , LCP_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_LineCrossPnt" ;

    AnsiString sFilePath = _sFilePath ;

    int iTemp ;

    if(_bLoad) {
        //Line1
        UserINI.Load(sFilePath, sTemp , "iScanDirect1", iTemp                      );_tPara.LinPara1.iScanDirect=(EN_LINE_SCAN_DIRECT)iTemp ;
        UserINI.Load(sFilePath, sTemp , "bLtToDk1    ", _tPara.LinPara1.bLtToDk    );
        UserINI.Load(sFilePath, sTemp , "iSampleGap1 ", _tPara.LinPara1.iSampleGap );
        UserINI.Load(sFilePath, sTemp , "iLineGap1   ", _tPara.LinPara1.iLineGap   );
        UserINI.Load(sFilePath, sTemp , "iEndOffset1 ", _tPara.LinPara1.iEndOffset );
        UserINI.Load(sFilePath, sTemp , "iCntrOffset1", _tPara.LinPara1.iCntrOffset);

        //Edge.
        UserINI.Load(sFilePath, sTemp , "iEdgAverage1 ", _tPara.LinPara1.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iEdgCompare1 ", _tPara.LinPara1.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxLow1", _tPara.LinPara1.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxHgh1", _tPara.LinPara1.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsGap1  ", _tPara.LinPara1.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxLow1", _tPara.LinPara1.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxHgh1", _tPara.LinPara1.EdgPara.iOfsPxHgh);

        //Line2
        UserINI.Load(sFilePath, sTemp , "iScanDirect2", iTemp                      );_tPara.LinPara2.iScanDirect=(EN_LINE_SCAN_DIRECT)iTemp ;
        UserINI.Load(sFilePath, sTemp , "bLtToDk2    ", _tPara.LinPara2.bLtToDk    );
        UserINI.Load(sFilePath, sTemp , "iSampleGap2 ", _tPara.LinPara2.iSampleGap );
        UserINI.Load(sFilePath, sTemp , "iLineGap2   ", _tPara.LinPara2.iLineGap   );
        UserINI.Load(sFilePath, sTemp , "iEndOffset2 ", _tPara.LinPara2.iEndOffset );
        UserINI.Load(sFilePath, sTemp , "iCntrOffset2", _tPara.LinPara2.iCntrOffset);

        //Edge.
        UserINI.Load(sFilePath, sTemp , "iEdgAverage2 ", _tPara.LinPara2.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "iEdgCompare2 ", _tPara.LinPara2.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxLow2", _tPara.LinPara2.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgChkPxHgh2", _tPara.LinPara2.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsGap2  ", _tPara.LinPara2.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxLow2", _tPara.LinPara2.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "iEdgOfsPxHgh2", _tPara.LinPara2.EdgPara.iOfsPxHgh);

    }
    else {
        //Line1
        UserINI.Save(sFilePath, sTemp , "iScanDirect1", _tPara.LinPara1.iScanDirect);
        UserINI.Save(sFilePath, sTemp , "bLtToDk1    ", _tPara.LinPara1.bLtToDk    );
        UserINI.Save(sFilePath, sTemp , "iSampleGap1 ", _tPara.LinPara1.iSampleGap );
        UserINI.Save(sFilePath, sTemp , "iLineGap1   ", _tPara.LinPara1.iLineGap   );
        UserINI.Save(sFilePath, sTemp , "iEndOffset1 ", _tPara.LinPara1.iEndOffset );
        UserINI.Save(sFilePath, sTemp , "iCntrOffset1", _tPara.LinPara1.iCntrOffset);

        //Edge.
        UserINI.Save(sFilePath, sTemp , "iEdgAverage1 ", _tPara.LinPara1.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iEdgCompare1 ", _tPara.LinPara1.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxLow1", _tPara.LinPara1.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxHgh1", _tPara.LinPara1.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsGap1  ", _tPara.LinPara1.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow1", _tPara.LinPara1.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh1", _tPara.LinPara1.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow1", _tPara.LinPara1.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh1", _tPara.LinPara1.EdgPara.iOfsPxHgh);

        //Line2
        UserINI.Save(sFilePath, sTemp , "iScanDirect2", _tPara.LinPara2.iScanDirect);
        UserINI.Save(sFilePath, sTemp , "bLtToDk2    ", _tPara.LinPara2.bLtToDk    );
        UserINI.Save(sFilePath, sTemp , "iSampleGap2 ", _tPara.LinPara2.iSampleGap );
        UserINI.Save(sFilePath, sTemp , "iLineGap2   ", _tPara.LinPara2.iLineGap   );
        UserINI.Save(sFilePath, sTemp , "iEndOffset2 ", _tPara.LinPara2.iEndOffset );
        UserINI.Save(sFilePath, sTemp , "iCntrOffset2", _tPara.LinPara2.iCntrOffset);

        //Edge.
        UserINI.Save(sFilePath, sTemp , "iEdgAverage2 ", _tPara.LinPara2.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "iEdgCompare2 ", _tPara.LinPara2.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxLow2", _tPara.LinPara2.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgChkPxHgh2", _tPara.LinPara2.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsGap2  ", _tPara.LinPara2.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxLow2", _tPara.LinPara2.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "iEdgOfsPxHgh2", _tPara.LinPara2.EdgPara.iOfsPxHgh);

    }

}

void UpdateParaLcp(bool _bToTable , TValueListEditor * _vlList , LCP_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        //Line1
        _vlList -> Values["1검색방향          "] = _tPara.LinPara1.iScanDirect;
        _vlList -> Values["1밝은곳에서어두운곳"] = _tPara.LinPara1.bLtToDk?1:0;
        _vlList -> Values["1샘플링 간격       "] = _tPara.LinPara1.iSampleGap ;
        _vlList -> Values["1라인인정범위      "] = _tPara.LinPara1.iLineGap   ;
        _vlList -> Values["1검색 시작끝 오프셑"] = _tPara.LinPara1.iEndOffset ;
        _vlList -> Values["1검색 중간 오프셑  "] = _tPara.LinPara1.iCntrOffset;

        _vlList -> Values["1Eg평균범위        "] = _tPara.LinPara1.EdgPara.iAverage ;
        _vlList -> Values["1Eg비교범위        "] = _tPara.LinPara1.EdgPara.iCompare ;
        _vlList -> Values["1Eg엦지밝기Low     "] = _tPara.LinPara1.EdgPara.iChkPxLow;
        _vlList -> Values["1Eg엦지밝기High    "] = _tPara.LinPara1.EdgPara.iChkPxHgh;
        _vlList -> Values["1Eg확정옵셑거리    "] = _tPara.LinPara1.EdgPara.iOfsGap  ;
        _vlList -> Values["1Eg확정옵셑밝기Low "] = _tPara.LinPara1.EdgPara.iOfsPxLow;
        _vlList -> Values["1Eg확정옵셑밝기High"] = _tPara.LinPara1.EdgPara.iOfsPxHgh;

        AnsiString sItems[2] = {/*"Down","Up",*/"Right","Left"};
        MakeComboBox    (_vlList , "1검색방향"           , 2 , sItems  , _tPara.LinPara1.iScanDirect-2);
        MakeBoolComboBox(_vlList , "1밝은곳에서어두운곳" ,_tPara.LinPara1.bLtToDk);


        //Line2
        _vlList -> Values["2검색방향          "] = _tPara.LinPara2.iScanDirect;
        _vlList -> Values["2밝은곳에서어두운곳"] = _tPara.LinPara2.bLtToDk?1:0;
        _vlList -> Values["2샘플링 간격       "] = _tPara.LinPara2.iSampleGap ;
        _vlList -> Values["2라인인정범위      "] = _tPara.LinPara2.iLineGap   ;
        _vlList -> Values["2검색 시작끝 오프셑"] = _tPara.LinPara2.iEndOffset ;
        _vlList -> Values["2검색 중간 오프셑  "] = _tPara.LinPara2.iCntrOffset;

        _vlList -> Values["2Eg평균범위        "] = _tPara.LinPara2.EdgPara.iAverage ;
        _vlList -> Values["2Eg비교범위        "] = _tPara.LinPara2.EdgPara.iCompare ;
        _vlList -> Values["2Eg엦지밝기Low     "] = _tPara.LinPara2.EdgPara.iChkPxLow;
        _vlList -> Values["2Eg엦지밝기High    "] = _tPara.LinPara2.EdgPara.iChkPxHgh;
        _vlList -> Values["2Eg확정옵셑거리    "] = _tPara.LinPara2.EdgPara.iOfsGap  ;
        _vlList -> Values["2Eg확정옵셑밝기Low "] = _tPara.LinPara2.EdgPara.iOfsPxLow;
        _vlList -> Values["2Eg확정옵셑밝기High"] = _tPara.LinPara2.EdgPara.iOfsPxHgh;

        AnsiString sItems3[2] = {"Down","Up"/*,"Right","Left"*/};
        MakeComboBox    (_vlList , "2검색방향"           , 2 , sItems3  , _tPara.LinPara2.iScanDirect);
        MakeBoolComboBox(_vlList , "2밝은곳에서어두운곳" ,_tPara.LinPara2.bLtToDk);
    }
    else {
        //Line1
        _tPara.LinPara1.iScanDirect       = (EN_LINE_SCAN_DIRECT)(GetItemIndex(_vlList          ,"1검색방향          ") + 2);
        _tPara.LinPara1.bLtToDk           = GetItemIndex(_vlList          ,"1밝은곳에서어두운곳");
        _tPara.LinPara1.iSampleGap        = StrToIntDef (_vlList -> Values["1샘플링 간격       "],_tPara.LinPara1.iSampleGap ) ;
        _tPara.LinPara1.iLineGap          = StrToIntDef (_vlList -> Values["1라인인정범위      "],_tPara.LinPara1.iLineGap   ) ;
        _tPara.LinPara1.iEndOffset        = StrToIntDef (_vlList -> Values["1검색 시작끝 오프셑"],_tPara.LinPara1.iEndOffset ) ;
        _tPara.LinPara1.iCntrOffset       = StrToIntDef (_vlList -> Values["1검색 중간 오프셑  "],_tPara.LinPara1.iCntrOffset) ;

        _tPara.LinPara1.EdgPara.iAverage  = StrToIntDef (_vlList -> Values["1Eg평균범위        "],_tPara.LinPara1.EdgPara.iAverage ) ;
        _tPara.LinPara1.EdgPara.iCompare  = StrToIntDef (_vlList -> Values["1Eg비교범위        "],_tPara.LinPara1.EdgPara.iCompare ) ;
        _tPara.LinPara1.EdgPara.iChkPxLow = StrToIntDef (_vlList -> Values["1Eg엦지밝기Low     "],_tPara.LinPara1.EdgPara.iChkPxLow) ;
        _tPara.LinPara1.EdgPara.iChkPxHgh = StrToIntDef (_vlList -> Values["1Eg엦지밝기High    "],_tPara.LinPara1.EdgPara.iChkPxHgh) ;
        _tPara.LinPara1.EdgPara.iOfsGap   = StrToIntDef (_vlList -> Values["1Eg확정옵셑거리    "],_tPara.LinPara1.EdgPara.iOfsGap  ) ;
        _tPara.LinPara1.EdgPara.iOfsPxLow = StrToIntDef (_vlList -> Values["1Eg확정옵셑밝기Low "],_tPara.LinPara1.EdgPara.iOfsPxLow) ;
        _tPara.LinPara1.EdgPara.iOfsPxHgh = StrToIntDef (_vlList -> Values["1Eg확정옵셑밝기High"],_tPara.LinPara1.EdgPara.iOfsPxHgh) ;

        //Line2
        _tPara.LinPara2.iScanDirect       = (EN_LINE_SCAN_DIRECT)GetItemIndex(_vlList          ,"2검색방향          ");
        _tPara.LinPara2.bLtToDk           = GetItemIndex(_vlList          ,"2밝은곳에서어두운곳");
        _tPara.LinPara2.iSampleGap        = StrToIntDef (_vlList -> Values["2샘플링 간격       "],_tPara.LinPara2.iSampleGap ) ;
        _tPara.LinPara2.iLineGap          = StrToIntDef (_vlList -> Values["2라인인정범위      "],_tPara.LinPara2.iLineGap   ) ;
        _tPara.LinPara2.iEndOffset        = StrToIntDef (_vlList -> Values["2검색 시작끝 오프셑"],_tPara.LinPara2.iEndOffset ) ;
        _tPara.LinPara2.iCntrOffset       = StrToIntDef (_vlList -> Values["2검색 중간 오프셑  "],_tPara.LinPara2.iCntrOffset) ;

        _tPara.LinPara2.EdgPara.iAverage  = StrToIntDef (_vlList -> Values["2Eg평균범위        "],_tPara.LinPara2.EdgPara.iAverage ) ;
        _tPara.LinPara2.EdgPara.iCompare  = StrToIntDef (_vlList -> Values["2Eg비교범위        "],_tPara.LinPara2.EdgPara.iCompare ) ;
        _tPara.LinPara2.EdgPara.iChkPxLow = StrToIntDef (_vlList -> Values["2Eg엦지밝기Low     "],_tPara.LinPara2.EdgPara.iChkPxLow) ;
        _tPara.LinPara2.EdgPara.iChkPxHgh = StrToIntDef (_vlList -> Values["2Eg엦지밝기High    "],_tPara.LinPara2.EdgPara.iChkPxHgh) ;
        _tPara.LinPara2.EdgPara.iOfsGap   = StrToIntDef (_vlList -> Values["2Eg확정옵셑거리    "],_tPara.LinPara2.EdgPara.iOfsGap  ) ;
        _tPara.LinPara2.EdgPara.iOfsPxLow = StrToIntDef (_vlList -> Values["2Eg확정옵셑밝기Low "],_tPara.LinPara2.EdgPara.iOfsPxLow) ;
        _tPara.LinPara2.EdgPara.iOfsPxHgh = StrToIntDef (_vlList -> Values["2Eg확정옵셑밝기High"],_tPara.LinPara2.EdgPara.iOfsPxHgh) ;

        UpdateParaLcp(true, _vlList , _tPara);
    }
}

/******************************************************************************/
/* Rect                                                                       */
/******************************************************************************/
void LoadParaRct(bool _bLoad , RCT_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Rect" ;
    AnsiString sFilePath = _sFilePath ;

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "bLtOnDk                   ", _tPara.bLtOnDk                   );

        UserINI.Load(sFilePath, sTemp , "iChkOfs                   ", _tPara.iChkOfs                   );
        UserINI.Load(sFilePath, sTemp , "iLowThsd                  ", _tPara.iLowThsd                  );
        UserINI.Load(sFilePath, sTemp , "iHighThsd                 ", _tPara.iHighThsd                 );

        UserINI.Load(sFilePath, sTemp , "LinParaL.iSampleGap       ", _tPara.LinParaL.iSampleGap       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iLineGap         ", _tPara.LinParaL.iLineGap         );

        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", _tPara.LinParaL.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", _tPara.LinParaL.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", _tPara.LinParaL.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", _tPara.LinParaL.EdgPara.iChkPxHgh);


        UserINI.Load(sFilePath, sTemp , "LinParaL.iEndOffset       ", _tPara.LinParaL.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iCntrOffset      ", _tPara.LinParaL.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", _tPara.LinParaL.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", _tPara.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", _tPara.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaT.iEndOffset       ", _tPara.LinParaT.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaT.iCntrOffset      ", _tPara.LinParaT.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", _tPara.LinParaT.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", _tPara.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", _tPara.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaR.iEndOffset       ", _tPara.LinParaR.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaR.iCntrOffset      ", _tPara.LinParaR.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", _tPara.LinParaR.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", _tPara.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", _tPara.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaB.iEndOffset       ", _tPara.LinParaB.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaB.iCntrOffset      ", _tPara.LinParaB.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", _tPara.LinParaB.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", _tPara.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", _tPara.LinParaB.EdgPara.iOfsPxHgh);

    }
    else {
        UserINI.Save(sFilePath, sTemp , "bLtOnDk                   ", _tPara.bLtOnDk                   );

        UserINI.Save(sFilePath, sTemp , "iChkOfs                   ", _tPara.iChkOfs                   );
        UserINI.Save(sFilePath, sTemp , "iLowThsd                  ", _tPara.iLowThsd                  );
        UserINI.Save(sFilePath, sTemp , "iHighThsd                 ", _tPara.iHighThsd                 );

        UserINI.Save(sFilePath, sTemp , "LinParaL.iSampleGap       ", _tPara.LinParaL.iSampleGap       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iLineGap         ", _tPara.LinParaL.iLineGap         );

        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", _tPara.LinParaL.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", _tPara.LinParaL.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", _tPara.LinParaL.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", _tPara.LinParaL.EdgPara.iChkPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaL.iEndOffset       ", _tPara.LinParaL.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iCntrOffset      ", _tPara.LinParaL.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", _tPara.LinParaL.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", _tPara.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", _tPara.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaT.iEndOffset       ", _tPara.LinParaT.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaT.iCntrOffset      ", _tPara.LinParaT.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", _tPara.LinParaT.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", _tPara.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", _tPara.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaR.iEndOffset       ", _tPara.LinParaR.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaR.iCntrOffset      ", _tPara.LinParaR.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", _tPara.LinParaR.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", _tPara.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", _tPara.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaB.iEndOffset       ", _tPara.LinParaB.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaB.iCntrOffset      ", _tPara.LinParaB.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", _tPara.LinParaB.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", _tPara.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", _tPara.LinParaB.EdgPara.iOfsPxHgh);

    }
}
void UpdateParaRct(bool _bToTable , TValueListEditor * _vlList , RCT_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["밝은물체찾기       "] = _tPara.bLtOnDk?1:0                ;
        _vlList -> Values["크랙확인위치       "] = _tPara.iChkOfs                    ;
        _vlList -> Values["자제밝기Low        "] = _tPara.iLowThsd                   ;
        _vlList -> Values["자제밝기High       "] = _tPara.iHighThsd                  ;

        _vlList -> Values["검색간격           "] = _tPara.LinParaL.iSampleGap        ;
        _vlList -> Values["라인인정범위       "] = _tPara.LinParaL.iLineGap          ;
        _vlList -> Values["평균범위           "] = _tPara.LinParaL.EdgPara.iAverage  ;
        _vlList -> Values["비교범위           "] = _tPara.LinParaL.EdgPara.iCompare  ;
        _vlList -> Values["확정픽셀Low        "] = _tPara.LinParaL.EdgPara.iChkPxLow ;
        _vlList -> Values["확정픽셀High       "] = _tPara.LinParaL.EdgPara.iChkPxHgh ;

        _vlList -> Values["L검색 시작끝 오프셑"] = _tPara.LinParaL.iEndOffset        ;
        _vlList -> Values["L검색 중간 오프셑  "] = _tPara.LinParaL.iCntrOffset       ;
        _vlList -> Values["L확정옵셑거리      "] = _tPara.LinParaL.EdgPara.iOfsGap   ;
        _vlList -> Values["L확정옵셑거리Low   "] = _tPara.LinParaL.EdgPara.iOfsPxLow ;
        _vlList -> Values["L확정옵셑거리High  "] = _tPara.LinParaL.EdgPara.iOfsPxHgh ;

        _vlList -> Values["T검색 시작끝 오프셑"] = _tPara.LinParaT.iEndOffset        ;
        _vlList -> Values["T검색 중간 오프셑  "] = _tPara.LinParaT.iCntrOffset       ;
        _vlList -> Values["T확정옵셑거리      "] = _tPara.LinParaT.EdgPara.iOfsGap   ;
        _vlList -> Values["T확정옵셑거리Low   "] = _tPara.LinParaT.EdgPara.iOfsPxLow ;
        _vlList -> Values["T확정옵셑거리High  "] = _tPara.LinParaT.EdgPara.iOfsPxHgh ;

        _vlList -> Values["R검색 시작끝 오프셑"] = _tPara.LinParaR.iEndOffset        ;
        _vlList -> Values["R검색 중간 오프셑  "] = _tPara.LinParaR.iCntrOffset       ;
        _vlList -> Values["R확정옵셑거리      "] = _tPara.LinParaR.EdgPara.iOfsGap   ;
        _vlList -> Values["R확정옵셑거리Low   "] = _tPara.LinParaR.EdgPara.iOfsPxLow ;
        _vlList -> Values["R확정옵셑거리High  "] = _tPara.LinParaR.EdgPara.iOfsPxHgh ;

        _vlList -> Values["B검색 시작끝 오프셑"] = _tPara.LinParaB.iEndOffset        ;
        _vlList -> Values["B검색 중간 오프셑  "] = _tPara.LinParaB.iCntrOffset       ;
        _vlList -> Values["B확정옵셑거리      "] = _tPara.LinParaB.EdgPara.iOfsGap   ;
        _vlList -> Values["B확정옵셑거리Low   "] = _tPara.LinParaB.EdgPara.iOfsPxLow ;
        _vlList -> Values["B확정옵셑거리High  "] = _tPara.LinParaB.EdgPara.iOfsPxHgh ;

        MakeBoolComboBox(_vlList , "밝은물체찾기" ,_tPara.bLtOnDk);

    }
    else {
        _tPara.bLtOnDk                      = GetItemIndex(_vlList          ,"밝은물체찾기       ");
        _tPara.iChkOfs                      = StrToIntDef (_vlList -> Values["크랙확인위치       "],_tPara.iChkOfs                   ) ;
        _tPara.iLowThsd                     = StrToIntDef (_vlList -> Values["자제밝기Low        "],_tPara.iLowThsd                  ) ;
        _tPara.iHighThsd                    = StrToIntDef (_vlList -> Values["자제밝기High       "],_tPara.iHighThsd                 ) ;

        _tPara.LinParaL.iSampleGap          = StrToIntDef (_vlList -> Values["검색간격           "],_tPara.LinParaL.iSampleGap       ) ;
        _tPara.LinParaL.iLineGap            = StrToIntDef (_vlList -> Values["라인인정범위       "],_tPara.LinParaL.iLineGap         ) ;
        _tPara.LinParaL.EdgPara.iAverage    = StrToIntDef (_vlList -> Values["평균범위           "],_tPara.LinParaL.EdgPara.iAverage ) ;
        _tPara.LinParaL.EdgPara.iCompare    = StrToIntDef (_vlList -> Values["비교범위           "],_tPara.LinParaL.EdgPara.iCompare ) ;
        _tPara.LinParaL.EdgPara.iChkPxLow   = StrToIntDef (_vlList -> Values["확정픽셀Low        "],_tPara.LinParaL.EdgPara.iChkPxLow) ;
        _tPara.LinParaL.EdgPara.iChkPxHgh   = StrToIntDef (_vlList -> Values["확정픽셀High       "],_tPara.LinParaL.EdgPara.iChkPxHgh) ;

        _tPara.LinParaL.iEndOffset          = StrToIntDef (_vlList -> Values["L검색 시작끝 오프셑"],_tPara.LinParaL.iEndOffset       ) ;
        _tPara.LinParaL.iCntrOffset         = StrToIntDef (_vlList -> Values["L검색 중간 오프셑  "],_tPara.LinParaL.iCntrOffset      ) ;
        _tPara.LinParaL.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["L확정옵셑거리      "],_tPara.LinParaL.EdgPara.iOfsGap  ) ;
        _tPara.LinParaL.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["L확정옵셑거리Low   "],_tPara.LinParaL.EdgPara.iOfsPxLow) ;
        _tPara.LinParaL.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["L확정옵셑거리High  "],_tPara.LinParaL.EdgPara.iOfsPxHgh) ;

        _tPara.LinParaT.iEndOffset          = StrToIntDef (_vlList -> Values["T검색 시작끝 오프셑"],_tPara.LinParaT.iEndOffset       ) ;
        _tPara.LinParaT.iCntrOffset         = StrToIntDef (_vlList -> Values["T검색 중간 오프셑  "],_tPara.LinParaT.iCntrOffset      ) ;
        _tPara.LinParaT.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["T확정옵셑거리      "],_tPara.LinParaT.EdgPara.iOfsGap  ) ;
        _tPara.LinParaT.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["T확정옵셑거리Low   "],_tPara.LinParaT.EdgPara.iOfsPxLow) ;
        _tPara.LinParaT.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["T확정옵셑거리High  "],_tPara.LinParaT.EdgPara.iOfsPxHgh) ;

        _tPara.LinParaR.iEndOffset          = StrToIntDef (_vlList -> Values["R검색 시작끝 오프셑"],_tPara.LinParaR.iEndOffset       ) ;
        _tPara.LinParaR.iCntrOffset         = StrToIntDef (_vlList -> Values["R검색 중간 오프셑  "],_tPara.LinParaR.iCntrOffset      ) ;
        _tPara.LinParaR.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["R확정옵셑거리      "],_tPara.LinParaR.EdgPara.iOfsGap  ) ;
        _tPara.LinParaR.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["R확정옵셑거리Low   "],_tPara.LinParaR.EdgPara.iOfsPxLow) ;
        _tPara.LinParaR.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["R확정옵셑거리High  "],_tPara.LinParaR.EdgPara.iOfsPxHgh) ;

        _tPara.LinParaB.iEndOffset          = StrToIntDef (_vlList -> Values["B검색 시작끝 오프셑"],_tPara.LinParaB.iEndOffset       ) ;
        _tPara.LinParaB.iCntrOffset         = StrToIntDef (_vlList -> Values["B검색 중간 오프셑  "],_tPara.LinParaB.iCntrOffset      ) ;
        _tPara.LinParaB.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["B확정옵셑거리      "],_tPara.LinParaB.EdgPara.iOfsGap  ) ;
        _tPara.LinParaB.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["B확정옵셑거리Low   "],_tPara.LinParaB.EdgPara.iOfsPxLow) ;
        _tPara.LinParaB.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["B확정옵셑거리High  "],_tPara.LinParaB.EdgPara.iOfsPxHgh) ;

        UpdateParaRct(true , _vlList , _tPara);
    }
}

/******************************************************************************/
/* Wafer Rect                                                                 */
/******************************************************************************/
void LoadParaWft(bool _bLoad , WRT_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_WaferRect" ;
    AnsiString sFilePath = _sFilePath ;

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "bLtOnDk                   ", _tPara.bLtOnDk                   );

        UserINI.Load(sFilePath, sTemp , "iChkOfs                   ", _tPara.iChkOfs                   );
        UserINI.Load(sFilePath, sTemp , "iLowThsd                  ", _tPara.iLowThsd                  );
        UserINI.Load(sFilePath, sTemp , "iHighThsd                 ", _tPara.iHighThsd                 );

        UserINI.Load(sFilePath, sTemp , "LinParaL.iSampleGap       ", _tPara.LinParaL.iSampleGap       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iLineGap         ", _tPara.LinParaL.iLineGap         );

        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", _tPara.LinParaL.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", _tPara.LinParaL.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", _tPara.LinParaL.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", _tPara.LinParaL.EdgPara.iChkPxHgh);


        UserINI.Load(sFilePath, sTemp , "LinParaL.iEndOffset       ", _tPara.LinParaL.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaL.iCntrOffset      ", _tPara.LinParaL.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", _tPara.LinParaL.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", _tPara.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", _tPara.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaT.iEndOffset       ", _tPara.LinParaT.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaT.iCntrOffset      ", _tPara.LinParaT.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", _tPara.LinParaT.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", _tPara.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", _tPara.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaR.iEndOffset       ", _tPara.LinParaR.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaR.iCntrOffset      ", _tPara.LinParaR.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", _tPara.LinParaR.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", _tPara.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", _tPara.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Load(sFilePath, sTemp , "LinParaB.iEndOffset       ", _tPara.LinParaB.iEndOffset       );
        UserINI.Load(sFilePath, sTemp , "LinParaB.iCntrOffset      ", _tPara.LinParaB.iCntrOffset      );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", _tPara.LinParaB.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", _tPara.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", _tPara.LinParaB.EdgPara.iOfsPxHgh);

    }
    else {
        //sTemp = _sId+"_WaferRect" ;
        //sFilePath = _sFilePath ;


        UserINI.Save(sFilePath, sTemp , "bLtOnDk                   ", _tPara.bLtOnDk                   );

        UserINI.Save(sFilePath, sTemp , "iChkOfs                   ", _tPara.iChkOfs                   );
        UserINI.Save(sFilePath, sTemp , "iLowThsd                  ", _tPara.iLowThsd                  );
        UserINI.Save(sFilePath, sTemp , "iHighThsd                 ", _tPara.iHighThsd                 );

        UserINI.Save(sFilePath, sTemp , "LinParaL.iSampleGap       ", _tPara.LinParaL.iSampleGap       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iLineGap         ", _tPara.LinParaL.iLineGap         );

        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iAverage ", _tPara.LinParaL.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iCompare ", _tPara.LinParaL.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxLow", _tPara.LinParaL.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iChkPxHgh", _tPara.LinParaL.EdgPara.iChkPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaL.iEndOffset       ", _tPara.LinParaL.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaL.iCntrOffset      ", _tPara.LinParaL.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsGap  ", _tPara.LinParaL.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxLow", _tPara.LinParaL.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaL.EdgPara.iOfsPxHgh", _tPara.LinParaL.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaT.iEndOffset       ", _tPara.LinParaT.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaT.iCntrOffset      ", _tPara.LinParaT.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsGap  ", _tPara.LinParaT.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxLow", _tPara.LinParaT.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaT.EdgPara.iOfsPxHgh", _tPara.LinParaT.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaR.iEndOffset       ", _tPara.LinParaR.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaR.iCntrOffset      ", _tPara.LinParaR.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsGap  ", _tPara.LinParaR.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxLow", _tPara.LinParaR.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaR.EdgPara.iOfsPxHgh", _tPara.LinParaR.EdgPara.iOfsPxHgh);

        UserINI.Save(sFilePath, sTemp , "LinParaB.iEndOffset       ", _tPara.LinParaB.iEndOffset       );
        UserINI.Save(sFilePath, sTemp , "LinParaB.iCntrOffset      ", _tPara.LinParaB.iCntrOffset      );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsGap  ", _tPara.LinParaB.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxLow", _tPara.LinParaB.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "LinParaB.EdgPara.iOfsPxHgh", _tPara.LinParaB.EdgPara.iOfsPxHgh);

    }
}
void UpdateParaWrt(bool _bToTable , TValueListEditor * _vlList , WRT_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["밝은물체찾기       "] = _tPara.bLtOnDk?1:0                ;
        _vlList -> Values["크랙확인위치       "] = _tPara.iChkOfs                    ;
        _vlList -> Values["자제밝기Low        "] = _tPara.iLowThsd                   ;
        _vlList -> Values["자제밝기High       "] = _tPara.iHighThsd                  ;

        _vlList -> Values["검색간격           "] = _tPara.LinParaL.iSampleGap        ;
        _vlList -> Values["라인인정범위       "] = _tPara.LinParaL.iLineGap          ;
        _vlList -> Values["평균범위           "] = _tPara.LinParaL.EdgPara.iAverage  ;
        _vlList -> Values["비교범위           "] = _tPara.LinParaL.EdgPara.iCompare  ;
        _vlList -> Values["확정픽셀Low        "] = _tPara.LinParaL.EdgPara.iChkPxLow ;
        _vlList -> Values["확정픽셀High       "] = _tPara.LinParaL.EdgPara.iChkPxHgh ;

        _vlList -> Values["L검색 시작끝 오프셑"] = _tPara.LinParaL.iEndOffset        ;
        _vlList -> Values["L검색 중간 오프셑  "] = _tPara.LinParaL.iCntrOffset       ;
        _vlList -> Values["L확정옵셑거리      "] = _tPara.LinParaL.EdgPara.iOfsGap   ;
        _vlList -> Values["L확정옵셑거리Low   "] = _tPara.LinParaL.EdgPara.iOfsPxLow ;
        _vlList -> Values["L확정옵셑거리High  "] = _tPara.LinParaL.EdgPara.iOfsPxHgh ;

        _vlList -> Values["T검색 시작끝 오프셑"] = _tPara.LinParaT.iEndOffset        ;
        _vlList -> Values["T검색 중간 오프셑  "] = _tPara.LinParaT.iCntrOffset       ;
        _vlList -> Values["T확정옵셑거리      "] = _tPara.LinParaT.EdgPara.iOfsGap   ;
        _vlList -> Values["T확정옵셑거리Low   "] = _tPara.LinParaT.EdgPara.iOfsPxLow ;
        _vlList -> Values["T확정옵셑거리High  "] = _tPara.LinParaT.EdgPara.iOfsPxHgh ;

        _vlList -> Values["R검색 시작끝 오프셑"] = _tPara.LinParaR.iEndOffset        ;
        _vlList -> Values["R검색 중간 오프셑  "] = _tPara.LinParaR.iCntrOffset       ;
        _vlList -> Values["R확정옵셑거리      "] = _tPara.LinParaR.EdgPara.iOfsGap   ;
        _vlList -> Values["R확정옵셑거리Low   "] = _tPara.LinParaR.EdgPara.iOfsPxLow ;
        _vlList -> Values["R확정옵셑거리High  "] = _tPara.LinParaR.EdgPara.iOfsPxHgh ;

        _vlList -> Values["B검색 시작끝 오프셑"] = _tPara.LinParaB.iEndOffset        ;
        _vlList -> Values["B검색 중간 오프셑  "] = _tPara.LinParaB.iCntrOffset       ;
        _vlList -> Values["B확정옵셑거리      "] = _tPara.LinParaB.EdgPara.iOfsGap   ;
        _vlList -> Values["B확정옵셑거리Low   "] = _tPara.LinParaB.EdgPara.iOfsPxLow ;
        _vlList -> Values["B확정옵셑거리High  "] = _tPara.LinParaB.EdgPara.iOfsPxHgh ;

        MakeBoolComboBox(_vlList , "밝은물체찾기" ,_tPara.bLtOnDk);


    }
    else {
        _tPara.bLtOnDk                      = GetItemIndex(_vlList          ,"밝은물체찾기       ");
        _tPara.iChkOfs                      = StrToIntDef (_vlList -> Values["크랙확인위치       "],_tPara.iChkOfs                   ) ;
        _tPara.iLowThsd                     = StrToIntDef (_vlList -> Values["자제밝기Low        "],_tPara.iLowThsd                  ) ;
        _tPara.iHighThsd                    = StrToIntDef (_vlList -> Values["자제밝기High       "],_tPara.iHighThsd                 ) ;

        _tPara.LinParaL.iSampleGap          = StrToIntDef (_vlList -> Values["검색간격           "],_tPara.LinParaL.iSampleGap       ) ;
        _tPara.LinParaL.iLineGap            = StrToIntDef (_vlList -> Values["라인인정범위       "],_tPara.LinParaL.iLineGap         ) ;
        _tPara.LinParaL.EdgPara.iAverage    = StrToIntDef (_vlList -> Values["평균범위           "],_tPara.LinParaL.EdgPara.iAverage ) ;
        _tPara.LinParaL.EdgPara.iCompare    = StrToIntDef (_vlList -> Values["비교범위           "],_tPara.LinParaL.EdgPara.iCompare ) ;
        _tPara.LinParaL.EdgPara.iChkPxLow   = StrToIntDef (_vlList -> Values["확정픽셀Low        "],_tPara.LinParaL.EdgPara.iChkPxLow) ;
        _tPara.LinParaL.EdgPara.iChkPxHgh   = StrToIntDef (_vlList -> Values["확정픽셀High       "],_tPara.LinParaL.EdgPara.iChkPxHgh) ;

        _tPara.LinParaL.iEndOffset          = StrToIntDef (_vlList -> Values["L검색 시작끝 오프셑"],_tPara.LinParaL.iEndOffset       ) ;
        _tPara.LinParaL.iCntrOffset         = StrToIntDef (_vlList -> Values["L검색 중간 오프셑  "],_tPara.LinParaL.iCntrOffset      ) ;
        _tPara.LinParaL.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["L확정옵셑거리      "],_tPara.LinParaL.EdgPara.iOfsGap  ) ;
        _tPara.LinParaL.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["L확정옵셑거리Low   "],_tPara.LinParaL.EdgPara.iOfsPxLow) ;
        _tPara.LinParaL.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["L확정옵셑거리High  "],_tPara.LinParaL.EdgPara.iOfsPxHgh) ;

        _tPara.LinParaT.iEndOffset          = StrToIntDef (_vlList -> Values["T검색 시작끝 오프셑"],_tPara.LinParaT.iEndOffset       ) ;
        _tPara.LinParaT.iCntrOffset         = StrToIntDef (_vlList -> Values["T검색 중간 오프셑  "],_tPara.LinParaT.iCntrOffset      ) ;
        _tPara.LinParaT.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["T확정옵셑거리      "],_tPara.LinParaT.EdgPara.iOfsGap  ) ;
        _tPara.LinParaT.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["T확정옵셑거리Low   "],_tPara.LinParaT.EdgPara.iOfsPxLow) ;
        _tPara.LinParaT.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["T확정옵셑거리High  "],_tPara.LinParaT.EdgPara.iOfsPxHgh) ;

        _tPara.LinParaR.iEndOffset          = StrToIntDef (_vlList -> Values["R검색 시작끝 오프셑"],_tPara.LinParaR.iEndOffset       ) ;
        _tPara.LinParaR.iCntrOffset         = StrToIntDef (_vlList -> Values["R검색 중간 오프셑  "],_tPara.LinParaR.iCntrOffset      ) ;
        _tPara.LinParaR.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["R확정옵셑거리      "],_tPara.LinParaR.EdgPara.iOfsGap  ) ;
        _tPara.LinParaR.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["R확정옵셑거리Low   "],_tPara.LinParaR.EdgPara.iOfsPxLow) ;
        _tPara.LinParaR.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["R확정옵셑거리High  "],_tPara.LinParaR.EdgPara.iOfsPxHgh) ;

        _tPara.LinParaB.iEndOffset          = StrToIntDef (_vlList -> Values["B검색 시작끝 오프셑"],_tPara.LinParaB.iEndOffset       ) ;
        _tPara.LinParaB.iCntrOffset         = StrToIntDef (_vlList -> Values["B검색 중간 오프셑  "],_tPara.LinParaB.iCntrOffset      ) ;
        _tPara.LinParaB.EdgPara.iOfsGap     = StrToIntDef (_vlList -> Values["B확정옵셑거리      "],_tPara.LinParaB.EdgPara.iOfsGap  ) ;
        _tPara.LinParaB.EdgPara.iOfsPxLow   = StrToIntDef (_vlList -> Values["B확정옵셑거리Low   "],_tPara.LinParaB.EdgPara.iOfsPxLow) ;
        _tPara.LinParaB.EdgPara.iOfsPxHgh   = StrToIntDef (_vlList -> Values["B확정옵셑거리High  "],_tPara.LinParaB.EdgPara.iOfsPxHgh) ;

        UpdateParaWrt(true , _vlList , _tPara);
    }
}

/******************************************************************************/
/* Threshold                                                                  */
/******************************************************************************/
void LoadParaTsd(bool _bLoad , TSD_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Threshold" ;
    AnsiString sFilePath = _sFilePath ;

    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iThresholdLow ", _tPara.iThresholdLow );
        UserINI.Load(sFilePath, sTemp , "iThresholdHigh", _tPara.iThresholdHigh);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "iThresholdLow ", _tPara.iThresholdLow );
        UserINI.Save(sFilePath, sTemp , "iThresholdHigh", _tPara.iThresholdHigh);
    }
}

void UpdateParaTsd(bool _bToTable , TValueListEditor * _vlList , TSD_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["검색밝기Low "] = _tPara.iThresholdLow  ;
        _vlList -> Values["검색밝기High"] = _tPara.iThresholdHigh ;
    }
    else {
        _tPara.iThresholdLow  = StrToIntDef (_vlList -> Values["검색밝기Low "],_tPara.iThresholdLow ) ;
        _tPara.iThresholdHigh = StrToIntDef (_vlList -> Values["검색밝기High"],_tPara.iThresholdHigh) ;

        UpdateParaTsd(true , _vlList , _tPara);
    }
}

/******************************************************************************/
/* Blob                                                                       */
/******************************************************************************/
void LoadParaBlb(bool _bLoad , BLB_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Blob" ;
    AnsiString sFilePath = _sFilePath ;


    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "_tPara.iPitchX    ", _tPara.iPitchX    );
        UserINI.Load(sFilePath, sTemp , "_tPara.iPitchY    ", _tPara.iPitchY    );
        UserINI.Load(sFilePath, sTemp , "_tPara.iStartXOfs ", _tPara.iStartXOfs );
        UserINI.Load(sFilePath, sTemp , "_tPara.iStartYOfs ", _tPara.iStartYOfs );
        UserINI.Load(sFilePath, sTemp , "_tPara.iThreshold ", _tPara.iThreshold );
        UserINI.Load(sFilePath, sTemp , "_tPara.bFindLtOnDk", _tPara.bFindLtOnDk);
        UserINI.Load(sFilePath, sTemp , "_tPara.bIgnrInside", _tPara.bIgnrInside);
        UserINI.Load(sFilePath, sTemp , "_tPara.bIgnrSide  ", _tPara.bIgnrSide  );

        UserINI.Load(sFilePath, sTemp , "_tPara.iMinArea       ", _tPara.iMinArea       );UserINI.Load(sFilePath, sTemp , "_tPara.iMaxArea       ", _tPara.iMaxArea       );
        UserINI.Load(sFilePath, sTemp , "_tPara.iMinWidth      ", _tPara.iMinWidth      );UserINI.Load(sFilePath, sTemp , "_tPara.iMaxWidth      ", _tPara.iMaxWidth      );
        UserINI.Load(sFilePath, sTemp , "_tPara.iMinHeight     ", _tPara.iMinHeight     );UserINI.Load(sFilePath, sTemp , "_tPara.iMaxHeight     ", _tPara.iMaxHeight     );
        UserINI.Load(sFilePath, sTemp , "_tPara.iMinBoundary   ", _tPara.iMinBoundary   );UserINI.Load(sFilePath, sTemp , "_tPara.iMaxBoundary   ", _tPara.iMaxBoundary   );
        UserINI.Load(sFilePath, sTemp , "_tPara.fMinCompactness", _tPara.fMinCompactness);UserINI.Load(sFilePath, sTemp , "_tPara.fMaxCompactness", _tPara.fMaxCompactness);
        UserINI.Load(sFilePath, sTemp , "_tPara.fMinAngle      ", _tPara.fMinAngle      );UserINI.Load(sFilePath, sTemp , "_tPara.fMaxAngle      ", _tPara.fMaxAngle      );
        UserINI.Load(sFilePath, sTemp , "_tPara.fMinAspectRatio", _tPara.fMinAspectRatio);UserINI.Load(sFilePath, sTemp , "_tPara.fMaxAspectRatio", _tPara.fMaxAspectRatio);
    }
    else {
        UserINI.Save(sFilePath, sTemp , "_tPara.iPitchX    ", _tPara.iPitchX    );
        UserINI.Save(sFilePath, sTemp , "_tPara.iPitchY    ", _tPara.iPitchY    );
        UserINI.Save(sFilePath, sTemp , "_tPara.iStartXOfs ", _tPara.iStartXOfs );
        UserINI.Save(sFilePath, sTemp , "_tPara.iStartYOfs ", _tPara.iStartYOfs );
        UserINI.Save(sFilePath, sTemp , "_tPara.iThreshold ", _tPara.iThreshold );
        UserINI.Save(sFilePath, sTemp , "_tPara.bFindLtOnDk", _tPara.bFindLtOnDk);
        UserINI.Save(sFilePath, sTemp , "_tPara.bIgnrInside", _tPara.bIgnrInside);
        UserINI.Save(sFilePath, sTemp , "_tPara.bIgnrSide  ", _tPara.bIgnrSide  );

        UserINI.Save(sFilePath, sTemp , "_tPara.iMinArea       ", _tPara.iMinArea       );UserINI.Save(sFilePath, sTemp , "_tPara.iMaxArea       ", _tPara.iMaxArea       );
        UserINI.Save(sFilePath, sTemp , "_tPara.iMinWidth      ", _tPara.iMinWidth      );UserINI.Save(sFilePath, sTemp , "_tPara.iMaxWidth      ", _tPara.iMaxWidth      );
        UserINI.Save(sFilePath, sTemp , "_tPara.iMinHeight     ", _tPara.iMinHeight     );UserINI.Save(sFilePath, sTemp , "_tPara.iMaxHeight     ", _tPara.iMaxHeight     );
        UserINI.Save(sFilePath, sTemp , "_tPara.iMinBoundary   ", _tPara.iMinBoundary   );UserINI.Save(sFilePath, sTemp , "_tPara.iMaxBoundary   ", _tPara.iMaxBoundary   );
        UserINI.Save(sFilePath, sTemp , "_tPara.fMinCompactness", _tPara.fMinCompactness);UserINI.Save(sFilePath, sTemp , "_tPara.fMaxCompactness", _tPara.fMaxCompactness);
        UserINI.Save(sFilePath, sTemp , "_tPara.fMinAngle      ", _tPara.fMinAngle      );UserINI.Save(sFilePath, sTemp , "_tPara.fMaxAngle      ", _tPara.fMaxAngle      );
        UserINI.Save(sFilePath, sTemp , "_tPara.fMinAspectRatio", _tPara.fMinAspectRatio);UserINI.Save(sFilePath, sTemp , "_tPara.fMaxAspectRatio", _tPara.fMaxAspectRatio);
    }
}
void UpdateParaBlb(bool _bToTable , TValueListEditor * _vlList , BLB_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["X축 검색간격          "] = _tPara.iPitchX         ;
        _vlList -> Values["Y축 검색간격          "] = _tPara.iPitchY         ;
        _vlList -> Values["X축 검색시작옵셑      "] = _tPara.iStartXOfs      ;
        _vlList -> Values["Y축 검색시작옵셑      "] = _tPara.iStartYOfs      ;
        _vlList -> Values["검색 밝기값           "] = _tPara.iThreshold      ;
        _vlList -> Values["밝은물체찾기          "] = _tPara.bFindLtOnDk?1:0 ;
        _vlList -> Values["블랍내부무시          "] = _tPara.bIgnrInside?1:0 ;
        _vlList -> Values["ROI외곽에 걸친물체무시"] = _tPara.bIgnrSide  ?1:0 ;

        _vlList -> Values["영역개수Min           "] = _tPara.iMinArea        ; _vlList -> Values["영역개수Max           "] = _tPara.iMaxArea        ;
        _vlList -> Values["넓이Min               "] = _tPara.iMinWidth       ; _vlList -> Values["넓이Max               "] = _tPara.iMaxWidth       ;
        _vlList -> Values["높이Min               "] = _tPara.iMinHeight      ; _vlList -> Values["높이Max               "] = _tPara.iMaxHeight      ;
        _vlList -> Values["테두리길이Min         "] = _tPara.iMinBoundary    ; _vlList -> Values["테두리길이Max         "] = _tPara.iMaxBoundary    ;
        _vlList -> Values["원형율Min             "] = _tPara.fMinCompactness ; _vlList -> Values["원형율Max             "] = _tPara.fMaxCompactness ;
        _vlList -> Values["각도Min               "] = _tPara.fMinAngle       ; _vlList -> Values["각도Max               "] = _tPara.fMaxAngle       ;
        _vlList -> Values["넓이높이비Min         "] = _tPara.fMinAspectRatio ; _vlList -> Values["넓이높이비Max         "] = _tPara.fMaxAspectRatio ;

        MakeBoolComboBox(_vlList , "밝은물체찾기" , _tPara.bFindLtOnDk);
        MakeBoolComboBox(_vlList , "블랍내부무시" , _tPara.bIgnrInside);
        MakeBoolComboBox(_vlList , "ROI외곽에 걸친물체무시" , _tPara.bIgnrSide);

    }
    else {
        _tPara.iPitchX         = StrToIntDef  (_vlList -> Values["X축 검색간격          "],_tPara.iPitchX         ) ;
        _tPara.iPitchY         = StrToIntDef  (_vlList -> Values["Y축 검색간격          "],_tPara.iPitchY         ) ;
        _tPara.iStartXOfs      = StrToIntDef  (_vlList -> Values["X축 검색시작옵셑      "],_tPara.iStartXOfs      ) ;
        _tPara.iStartYOfs      = StrToIntDef  (_vlList -> Values["Y축 검색시작옵셑      "],_tPara.iStartYOfs      ) ;
        _tPara.iThreshold      = StrToIntDef  (_vlList -> Values["검색 밝기값           "],_tPara.iThreshold      ) ;
        _tPara.bFindLtOnDk     = GetItemIndex (_vlList         , "밝은물체찾기          ");
        _tPara.bIgnrInside     = GetItemIndex (_vlList         , "블랍내부무시          ");
        _tPara.bIgnrSide       = GetItemIndex (_vlList         , "ROI외곽에 걸친물체무시");

        _tPara.iMinArea        = StrToIntDef  (_vlList -> Values["영역개수Min           "],_tPara.iMinArea       ) ; _tPara.iMaxArea        = StrToIntDef  (_vlList -> Values["영역개수Max           "],_tPara.iMaxArea       ) ;
        _tPara.iMinWidth       = StrToIntDef  (_vlList -> Values["넓이Min               "],_tPara.iMinWidth      ) ; _tPara.iMaxWidth       = StrToIntDef  (_vlList -> Values["넓이Max               "],_tPara.iMaxWidth      ) ;
        _tPara.iMinHeight      = StrToIntDef  (_vlList -> Values["높이Min               "],_tPara.iMinHeight     ) ; _tPara.iMaxHeight      = StrToIntDef  (_vlList -> Values["높이Max               "],_tPara.iMaxHeight     ) ;
        _tPara.iMinBoundary    = StrToIntDef  (_vlList -> Values["테두리길이Min         "],_tPara.iMinBoundary   ) ; _tPara.iMaxBoundary    = StrToIntDef  (_vlList -> Values["테두리길이Max         "],_tPara.iMaxBoundary   ) ;
        _tPara.fMinCompactness = StrToFloatDef(_vlList -> Values["원형율Min             "],_tPara.fMinCompactness) ; _tPara.fMaxCompactness = StrToFloatDef(_vlList -> Values["원형율Max             "],_tPara.fMaxCompactness) ;
        _tPara.fMinAngle       = StrToFloatDef(_vlList -> Values["각도Min               "],_tPara.fMinAngle      ) ; _tPara.fMaxAngle       = StrToFloatDef(_vlList -> Values["각도Max               "],_tPara.fMaxAngle      ) ;
        _tPara.fMinAspectRatio = StrToFloatDef(_vlList -> Values["넓이높이비Min         "],_tPara.fMinAspectRatio) ; _tPara.fMaxAspectRatio = StrToFloatDef(_vlList -> Values["넓이높이비Max         "],_tPara.fMaxAspectRatio) ;
        UpdateParaBlb(true , _vlList , _tPara );
    }
}

/******************************************************************************/
/* Calibration                                                                */
/******************************************************************************/
void LoadParaCal(bool _bLoad , CAL_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Calibration" ;
    AnsiString sFilePath = _sFilePath ;

    if(_bLoad) {

        UserINI.Load(sFilePath, sTemp , "fDotSize   ", _tPara.fDotSize   );
        UserINI.Load(sFilePath, sTemp , "fDotPitch  ", _tPara.fDotPitch  );

        UserINI.Load(sFilePath, sTemp , "BlbPara.iPitchX    ", _tPara.BlbPara.iPitchX    );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iPitchY    ", _tPara.BlbPara.iPitchY    );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iStartXOfs ", _tPara.BlbPara.iStartXOfs );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iStartYOfs ", _tPara.BlbPara.iStartYOfs );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iThreshold ", _tPara.BlbPara.iThreshold );
        UserINI.Load(sFilePath, sTemp , "BlbPara.bFindLtOnDk", _tPara.BlbPara.bFindLtOnDk);
        UserINI.Load(sFilePath, sTemp , "BlbPara.bIgnrInside", _tPara.BlbPara.bIgnrInside);
        UserINI.Load(sFilePath, sTemp , "BlbPara.bIgnrSide  ", _tPara.BlbPara.bIgnrSide  );

        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinArea       ", _tPara.BlbPara.iMinArea       );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxArea       ", _tPara.BlbPara.iMaxArea       );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinWidth      ", _tPara.BlbPara.iMinWidth      );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxWidth      ", _tPara.BlbPara.iMaxWidth      );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinHeight     ", _tPara.BlbPara.iMinHeight     );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxHeight     ", _tPara.BlbPara.iMaxHeight     );
        UserINI.Load(sFilePath, sTemp , "BlbPara.iMinBoundary   ", _tPara.BlbPara.iMinBoundary   );UserINI.Load(sFilePath, sTemp , "BlbPara.iMaxBoundary   ", _tPara.BlbPara.iMaxBoundary   );
        UserINI.Load(sFilePath, sTemp , "BlbPara.fMinCompactness", _tPara.BlbPara.fMinCompactness);UserINI.Load(sFilePath, sTemp , "BlbPara.fMaxCompactness", _tPara.BlbPara.fMaxCompactness);
        UserINI.Load(sFilePath, sTemp , "BlbPara.fMinAngle      ", _tPara.BlbPara.fMinAngle      );UserINI.Load(sFilePath, sTemp , "BlbPara.fMaxAngle      ", _tPara.BlbPara.fMaxAngle      );
        UserINI.Load(sFilePath, sTemp , "BlbPara.fMinAspectRatio", _tPara.BlbPara.fMinAspectRatio);UserINI.Load(sFilePath, sTemp , "BlbPara.fMaxAspectRatio", _tPara.BlbPara.fMaxAspectRatio);

    }
    else {
        UserINI.Save(sFilePath, sTemp , "fDotSize           ", _tPara.fDotSize           );
        UserINI.Save(sFilePath, sTemp , "fDotPitch          ", _tPara.fDotPitch          );

        UserINI.Save(sFilePath, sTemp , "BlbPara.iPitchX    ", _tPara.BlbPara.iPitchX    );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iPitchY    ", _tPara.BlbPara.iPitchY    );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iStartXOfs ", _tPara.BlbPara.iStartXOfs );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iStartYOfs ", _tPara.BlbPara.iStartYOfs );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iThreshold ", _tPara.BlbPara.iThreshold );
        UserINI.Save(sFilePath, sTemp , "BlbPara.bFindLtOnDk", _tPara.BlbPara.bFindLtOnDk);
        UserINI.Save(sFilePath, sTemp , "BlbPara.bIgnrInside", _tPara.BlbPara.bIgnrInside);
        UserINI.Save(sFilePath, sTemp , "BlbPara.bIgnrSide  ", _tPara.BlbPara.bIgnrSide  );

        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinArea       ", _tPara.BlbPara.iMinArea       );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxArea       ", _tPara.BlbPara.iMaxArea       );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinWidth      ", _tPara.BlbPara.iMinWidth      );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxWidth      ", _tPara.BlbPara.iMaxWidth      );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinHeight     ", _tPara.BlbPara.iMinHeight     );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxHeight     ", _tPara.BlbPara.iMaxHeight     );
        UserINI.Save(sFilePath, sTemp , "BlbPara.iMinBoundary   ", _tPara.BlbPara.iMinBoundary   );UserINI.Save(sFilePath, sTemp , "BlbPara.iMaxBoundary   ", _tPara.BlbPara.iMaxBoundary   );
        UserINI.Save(sFilePath, sTemp , "BlbPara.fMinCompactness", _tPara.BlbPara.fMinCompactness);UserINI.Save(sFilePath, sTemp , "BlbPara.fMaxCompactness", _tPara.BlbPara.fMaxCompactness);
        UserINI.Save(sFilePath, sTemp , "BlbPara.fMinAngle      ", _tPara.BlbPara.fMinAngle      );UserINI.Save(sFilePath, sTemp , "BlbPara.fMaxAngle      ", _tPara.BlbPara.fMaxAngle      );
        UserINI.Save(sFilePath, sTemp , "BlbPara.fMinAspectRatio", _tPara.BlbPara.fMinAspectRatio);UserINI.Save(sFilePath, sTemp , "BlbPara.fMaxAspectRatio", _tPara.BlbPara.fMaxAspectRatio);

    }
}

void UpdateParaCal(bool _bToTable , TValueListEditor * _vlList , CAL_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["도트 지름             "] = _tPara.fDotSize                ;
        _vlList -> Values["도트 피치             "] = _tPara.fDotPitch               ;

        _vlList -> Values["X축 검색간격          "] = _tPara.BlbPara.iPitchX         ;
        _vlList -> Values["Y축 검색간격          "] = _tPara.BlbPara.iPitchY         ;
        _vlList -> Values["X축 검색시작옵셑      "] = _tPara.BlbPara.iStartXOfs      ;
        _vlList -> Values["Y축 검색시작옵셑      "] = _tPara.BlbPara.iStartYOfs      ;
        _vlList -> Values["검색 밝기값           "] = _tPara.BlbPara.iThreshold      ;
        _vlList -> Values["밝은물체찾기          "] = _tPara.BlbPara.bFindLtOnDk?1:0 ;
        _vlList -> Values["블랍내부무시          "] = _tPara.BlbPara.bIgnrInside?1:0 ;
        _vlList -> Values["ROI외곽에 걸친물체무시"] = _tPara.BlbPara.bIgnrSide  ?1:0 ;

        _vlList -> Values["영역개수Min           "] = _tPara.BlbPara.iMinArea        ; _vlList -> Values["영역개수Max           "] = _tPara.BlbPara.iMaxArea        ;
        _vlList -> Values["넓이Min               "] = _tPara.BlbPara.iMinWidth       ; _vlList -> Values["넓이Max               "] = _tPara.BlbPara.iMaxWidth       ;
        _vlList -> Values["높이Min               "] = _tPara.BlbPara.iMinHeight      ; _vlList -> Values["높이Max               "] = _tPara.BlbPara.iMaxHeight      ;
        _vlList -> Values["테두리길이Min         "] = _tPara.BlbPara.iMinBoundary    ; _vlList -> Values["테두리길이Max         "] = _tPara.BlbPara.iMaxBoundary    ;
        _vlList -> Values["원형율Min             "] = _tPara.BlbPara.fMinCompactness ; _vlList -> Values["원형율Max             "] = _tPara.BlbPara.fMaxCompactness ;
        _vlList -> Values["각도Min               "] = _tPara.BlbPara.fMinAngle       ; _vlList -> Values["각도Max               "] = _tPara.BlbPara.fMaxAngle       ;
        _vlList -> Values["넓이높이비Min         "] = _tPara.BlbPara.fMinAspectRatio ; _vlList -> Values["넓이높이비Max         "] = _tPara.BlbPara.fMaxAspectRatio ;

        MakeBoolComboBox(_vlList , "밝은물체찾기"           , _tPara.BlbPara.bFindLtOnDk);
        MakeBoolComboBox(_vlList , "블랍내부무시"           , _tPara.BlbPara.bIgnrInside);
        MakeBoolComboBox(_vlList , "ROI외곽에 걸친물체무시" , _tPara.BlbPara.bIgnrSide  );

    }
    else {
        _tPara.fDotSize        = StrToFloatDef(_vlList -> Values["도트 지름             "],_tPara.fDotSize       ) ;
        _tPara.fDotPitch       = StrToFloatDef(_vlList -> Values["도트 피치             "],_tPara.fDotPitch      ) ;

        _tPara.BlbPara.iPitchX         = StrToIntDef  (_vlList -> Values["X축 검색간격          "],_tPara.BlbPara.iPitchX        ) ;
        _tPara.BlbPara.iPitchY         = StrToIntDef  (_vlList -> Values["Y축 검색간격          "],_tPara.BlbPara.iPitchY        ) ;
        _tPara.BlbPara.iStartXOfs      = StrToIntDef  (_vlList -> Values["X축 검색시작옵셑      "],_tPara.BlbPara.iStartXOfs     ) ;
        _tPara.BlbPara.iStartYOfs      = StrToIntDef  (_vlList -> Values["Y축 검색시작옵셑      "],_tPara.BlbPara.iStartYOfs     ) ;
        _tPara.BlbPara.iThreshold      = StrToIntDef  (_vlList -> Values["검색 밝기값           "],_tPara.BlbPara.iThreshold     ) ;
        _tPara.BlbPara.bFindLtOnDk     = GetItemIndex (_vlList         , "밝은물체찾기          ");       
        _tPara.BlbPara.bIgnrInside     = GetItemIndex (_vlList         , "블랍내부무시          ");       
        _tPara.BlbPara.bIgnrSide       = GetItemIndex (_vlList         , "ROI외곽에 걸친물체무시");       

        _tPara.BlbPara.iMinArea        = StrToIntDef  (_vlList -> Values["영역개수Min           "],_tPara.BlbPara.iMinArea       ) ; _tPara.BlbPara.iMaxArea        = StrToIntDef  (_vlList -> Values["영역개수Max           "],_tPara.BlbPara.iMaxArea       ) ;
        _tPara.BlbPara.iMinWidth       = StrToIntDef  (_vlList -> Values["넓이Min               "],_tPara.BlbPara.iMinWidth      ) ; _tPara.BlbPara.iMaxWidth       = StrToIntDef  (_vlList -> Values["넓이Max               "],_tPara.BlbPara.iMaxWidth      ) ;
        _tPara.BlbPara.iMinHeight      = StrToIntDef  (_vlList -> Values["높이Min               "],_tPara.BlbPara.iMinHeight     ) ; _tPara.BlbPara.iMaxHeight      = StrToIntDef  (_vlList -> Values["높이Max               "],_tPara.BlbPara.iMaxHeight     ) ;
        _tPara.BlbPara.iMinBoundary    = StrToIntDef  (_vlList -> Values["테두리길이Min         "],_tPara.BlbPara.iMinBoundary   ) ; _tPara.BlbPara.iMaxBoundary    = StrToIntDef  (_vlList -> Values["테두리길이Max         "],_tPara.BlbPara.iMaxBoundary   ) ;
        _tPara.BlbPara.fMinCompactness = StrToFloatDef(_vlList -> Values["원형율Min             "],_tPara.BlbPara.fMinCompactness) ; _tPara.BlbPara.fMaxCompactness = StrToFloatDef(_vlList -> Values["원형율Max             "],_tPara.BlbPara.fMaxCompactness) ;
        _tPara.BlbPara.fMinAngle       = StrToFloatDef(_vlList -> Values["각도Min               "],_tPara.BlbPara.fMinAngle      ) ; _tPara.BlbPara.fMaxAngle       = StrToFloatDef(_vlList -> Values["각도Max               "],_tPara.BlbPara.fMaxAngle      ) ;
        _tPara.BlbPara.fMinAspectRatio = StrToFloatDef(_vlList -> Values["넓이높이비Min         "],_tPara.BlbPara.fMinAspectRatio) ; _tPara.BlbPara.fMaxAspectRatio = StrToFloatDef(_vlList -> Values["넓이높이비Max         "],_tPara.BlbPara.fMaxAspectRatio) ;
        UpdateParaCal(true , _vlList , _tPara );
    }

}

/******************************************************************************/
/* Circle                                                                     */
/******************************************************************************/
void LoadParaCcl(bool _bLoad , CCL_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Circle" ;
    AnsiString sFilePath = _sFilePath ;


    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iGap              ", _tPara.iGap      );
        UserINI.Load(sFilePath, sTemp , "iSampleCnt        ", _tPara.iSampleCnt);
        UserINI.Load(sFilePath, sTemp , "iStartAng         ", _tPara.iStartAng );
        UserINI.Load(sFilePath, sTemp , "iEndAng           ", _tPara.iEndAng   );

        UserINI.Load(sFilePath, sTemp , "EdgPara.iAverage  ", _tPara.EdgPara.iAverage );
        UserINI.Load(sFilePath, sTemp , "EdgPara.iCompare  ", _tPara.EdgPara.iCompare );
        UserINI.Load(sFilePath, sTemp , "EdgPara.iChkPxLow ", _tPara.EdgPara.iChkPxLow);
        UserINI.Load(sFilePath, sTemp , "EdgPara.iChkPxHgh ", _tPara.EdgPara.iChkPxHgh);
        UserINI.Load(sFilePath, sTemp , "EdgPara.iOfsGap   ", _tPara.EdgPara.iOfsGap  );
        UserINI.Load(sFilePath, sTemp , "EdgPara.iOfsPxLow ", _tPara.EdgPara.iOfsPxLow);
        UserINI.Load(sFilePath, sTemp , "EdgPara.iOfsPxHgh ", _tPara.EdgPara.iOfsPxHgh);

    }
    else {
        UserINI.Save(sFilePath, sTemp , "iGap      ", _tPara.iGap       );
        UserINI.Save(sFilePath, sTemp , "iSampleCnt", _tPara.iSampleCnt );
        UserINI.Save(sFilePath, sTemp , "iStartAng ", _tPara.iStartAng  );
        UserINI.Save(sFilePath, sTemp , "iEndAng   ", _tPara.iEndAng    );

        UserINI.Save(sFilePath, sTemp , "EdgPara.iAverage ", _tPara.EdgPara.iAverage );
        UserINI.Save(sFilePath, sTemp , "EdgPara.iCompare ", _tPara.EdgPara.iCompare );
        UserINI.Save(sFilePath, sTemp , "EdgPara.iChkPxLow", _tPara.EdgPara.iChkPxLow);
        UserINI.Save(sFilePath, sTemp , "EdgPara.iChkPxHgh", _tPara.EdgPara.iChkPxHgh);
        UserINI.Save(sFilePath, sTemp , "EdgPara.iOfsGap  ", _tPara.EdgPara.iOfsGap  );
        UserINI.Save(sFilePath, sTemp , "EdgPara.iOfsPxLow", _tPara.EdgPara.iOfsPxLow);
        UserINI.Save(sFilePath, sTemp , "EdgPara.iOfsPxHgh", _tPara.EdgPara.iOfsPxHgh);

    }
}
void UpdateParaCcl(bool _bToTable , TValueListEditor * _vlList , CCL_Para &_tPara)
{
    if(_bToTable) {

        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["검색 간격             "] = _tPara.iGap              ;
        _vlList -> Values["검색 개수             "] = _tPara.iSampleCnt        ;
        _vlList -> Values["검색시작 각도         "] = _tPara.iStartAng         ;
        _vlList -> Values["검색끝 각도           "] = _tPara.iEndAng           ;

        _vlList -> Values["Eg평균범위            "] = _tPara.EdgPara.iAverage  ;
        _vlList -> Values["Eg비교범위            "] = _tPara.EdgPara.iCompare  ;
        _vlList -> Values["Eg엦지밝기Low         "] = _tPara.EdgPara.iChkPxLow ;
        _vlList -> Values["Eg엦지밝기High        "] = _tPara.EdgPara.iChkPxHgh ;
        _vlList -> Values["Eg확정옵셑거리        "] = _tPara.EdgPara.iOfsGap   ;
        _vlList -> Values["Eg확정옵셑밝기Low     "] = _tPara.EdgPara.iOfsPxLow ;
        _vlList -> Values["Eg확정옵셑밝기High    "] = _tPara.EdgPara.iOfsPxHgh ;

    }
    else {
        _tPara.iGap              = StrToIntDef (_vlList -> Values["검색 간격             "],_tPara.iGap             ) ;
        _tPara.iSampleCnt        = StrToIntDef (_vlList -> Values["검색 개수             "],_tPara.iSampleCnt       ) ;
        _tPara.iStartAng         = StrToIntDef (_vlList -> Values["검색시작 각도         "],_tPara.iStartAng        ) ;
        _tPara.iEndAng           = StrToIntDef (_vlList -> Values["검색끝 각도           "],_tPara.iEndAng          ) ;

        _tPara.EdgPara.iAverage  = StrToIntDef (_vlList -> Values["Eg평균범위            "],_tPara.EdgPara.iAverage ) ;
        _tPara.EdgPara.iCompare  = StrToIntDef (_vlList -> Values["Eg비교범위            "],_tPara.EdgPara.iCompare ) ;
        _tPara.EdgPara.iChkPxLow = StrToIntDef (_vlList -> Values["Eg엦지밝기Low         "],_tPara.EdgPara.iChkPxLow) ;
        _tPara.EdgPara.iChkPxHgh = StrToIntDef (_vlList -> Values["Eg엦지밝기High        "],_tPara.EdgPara.iChkPxHgh) ;
        _tPara.EdgPara.iOfsGap   = StrToIntDef (_vlList -> Values["Eg확정옵셑거리        "],_tPara.EdgPara.iOfsGap  ) ;
        _tPara.EdgPara.iOfsPxLow = StrToIntDef (_vlList -> Values["Eg확정옵셑밝기Low     "],_tPara.EdgPara.iOfsPxLow) ;
        _tPara.EdgPara.iOfsPxHgh = StrToIntDef (_vlList -> Values["Eg확정옵셑밝기High    "],_tPara.EdgPara.iOfsPxHgh) ;
        UpdateParaCcl(true , _vlList , _tPara);
    }
}

/******************************************************************************/
/* Ocv                                                                        */
/******************************************************************************/
void LoadParaOcv(bool _bLoad , OCV_Para &_tPara, AnsiString _sFilePath , AnsiString _sId)
{
    TUserINI UserINI ;

    AnsiString sTemp = _sId+"_Ocv" ;

    AnsiString sFilePath = _sFilePath ;

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sFilePath, sTemp , "iInspOft  ", _tPara.iTrInspOft  );
        UserINI.Load(sFilePath, sTemp , "iNoInspOft", _tPara.iTrNoInspOft);
        UserINI.Load(sFilePath, sTemp , "iThreshold", _tPara.iTrThreshold);

        UserINI.Load(sFilePath, sTemp , "iInspFreq ", _tPara.iInspFreq );
        UserINI.Load(sFilePath, sTemp , "fSinc     ", _tPara.fSinc     );

    }
    else {
        UserINI.Save(sFilePath, sTemp , "iInspOft  ", _tPara.iTrInspOft  );
        UserINI.Save(sFilePath, sTemp , "iNoInspOft", _tPara.iTrNoInspOft);
        UserINI.Save(sFilePath, sTemp , "iThreshold", _tPara.iTrThreshold);

        UserINI.Save(sFilePath, sTemp , "iInspFreq ", _tPara.iInspFreq );
        UserINI.Save(sFilePath, sTemp , "fSinc     ", _tPara.fSinc     );
    }
}

void UpdateParaOcv(bool _bToTable , TValueListEditor * _vlList , OCV_Para &_tPara)
{
    if(_bToTable) {
        _vlList -> Strings -> Clear() ;
        _vlList -> DisplayOptions >> doColumnTitles ;

        _vlList -> Values["트레인 인스펙션영역  "] = _tPara.iTrInspOft   ;
        _vlList -> Values["트레인 넌인스펙션영역"] = _tPara.iTrNoInspOft ;
        _vlList -> Values["트레인 임계값        "] = _tPara.iTrThreshold ;

        _vlList -> Values["검사 빈도            "] = _tPara.iInspFreq    ;
        _vlList -> Values["싱크율%              "] = _tPara.fSinc        ;
    }
    else {
        _tPara.iTrInspOft   = StrToIntDef (_vlList -> Values["트레인 인스펙션영역  "],_tPara.iTrInspOft  ) ;
        _tPara.iTrNoInspOft = StrToIntDef (_vlList -> Values["트레인 넌인스펙션영역"],_tPara.iTrNoInspOft) ;
        _tPara.iTrThreshold = StrToIntDef (_vlList -> Values["트레인 임계값        "],_tPara.iTrThreshold) ;

        _tPara.iInspFreq    = StrToIntDef (_vlList -> Values["검사 빈도            "],_tPara.iInspFreq   ) ;
        _tPara.fSinc        = StrToIntDef (_vlList -> Values["싱크율%              "],_tPara.fSinc       ) ;

        UpdateParaOcv(true , _vlList , _tPara );
    }
}


