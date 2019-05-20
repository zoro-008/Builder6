//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PstBuff.h"
#include "UnLoader.h"
#include "Rail.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "SPCUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#define PSB_INSERT_SPD 10
#define PSB_INSERT_DIS 10
//---------------------------------------------------------------------------

CPstBuff  PSB;

enum EN_PSB_FLIP {
    pfNone = 0 ,
    pfFlip = 1
};


CPstBuff::CPstBuff(void)
{
    Init();
}

CPstBuff::~CPstBuff (void)
{
//    Close();
}

void CPstBuff::Init()
{
    m_sPartName = "Post Buff " ;
    Reset();
    Load(true);


    m_bDnEdgePSBPkg2 = false;
//    AnsiString BinPath ;
//    AnsiString IniPath ;

//    BinPath = EXE_FOLDER + "SeqData\\DataMapPSBLot.dat" ;
//    IniPath = EXE_FOLDER + "SeqData\\DataMapPSBLot.ini" ;
//    riLot.Load(true,BinPath , IniPath , 1);
}

void CPstBuff::Close()
{
//    AnsiString BinPath ;
//    AnsiString IniPath ;

//    BinPath = EXE_FOLDER + "SeqData\\DataMapWR1Buffer.dat" ;
//    IniPath = EXE_FOLDER + "SeqData\\DataMapWR1Buffer.ini" ;
//    riLot.Load(false,BinPath , IniPath , 1);

    Load(false);

}

void CPstBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPstBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}


//리뷰랑 트림 포지션만 걸려 있군. dd
bool CPstBuff::FindChip(EN_ARAY_ID _iAray , EN_CHIP_STAT _iStat ,  int &r , int &c )
{
    if( _iAray  == riPSB ) {
        c = DM.ARAY[_iAray].FindFrstCol(_iStat);

        if(c % 2)DM.ARAY[_iAray].FindFrstColRow    (_iStat , r , c) ;
        else     DM.ARAY[_iAray].FindFrstColLastRow(_iStat , r , c) ; // 첫포지션 밑에서 부터 찍을까봐 바꾸긴 하는데 이로인한 삽질이 있을수도 있음. dd
    }

    return (r > -1 && c > -1);
}
//------------------------------------------------------------------------------
// 2012.07.30 dd
// 4014 자재가 리젝이 안되서 펜 마킹 하기로 하여 추가함.
//------------------------------------------------------------------------------
bool CPstBuff::FindChipMrk( int &r , int &c ) //
{
    int rTemp = -1 , cTemp = -1 ;
    int rRst  = -1 , cRst  = OM.DevInfo.iColCnt ;

    //Col Find
    for(int i = (int)csRslt0; i <= (int)csRsltL; i++ )
    {
        if(OM.CmnOptn.bIgnrEmptyTrm && i == csRslt1 ) continue ; //Empty == csRslt1
        cTemp = DM.ARAY[riPSB].FindFrstCol((EN_CHIP_STAT)i);
        if(cTemp != -1 && cRst > cTemp) cRst = cTemp ;
    }

    //Row Find
    if(cRst % 2){
        rRst = -1 ;
        for(int i = (int)csRslt0; i <= (int)csRsltL; i++ )
        {
            if(OM.CmnOptn.bIgnrEmptyTrm && i == csRslt1 ) continue ; //Empty == csRslt1
            DM.ARAY[riPSB].FindFrstColLastRow( (EN_CHIP_STAT)i , rTemp , cTemp) ;
            if(rTemp != -1 && cTemp == cRst && rRst<rTemp) rRst = rTemp ;
        }

    }
    else{
        rRst = OM.DevInfo.iRowCnt ;
        for(int i = (int)csRslt0; i <= (int)csRsltL; i++ )
        {
            if(OM.CmnOptn.bIgnrEmptyTrm && i == csRslt1 ) continue ; //Empty == csRslt1
            DM.ARAY[riPSB].FindFrstColRow( (EN_CHIP_STAT)i , rTemp , cTemp) ;
            if(rTemp != -1 && cTemp == cRst && rRst>rTemp) rRst = rTemp ;
        }

    }
    r= rRst ; c= cRst ;
    return (r > -1 && c > -1 );

}
//GetMotrPos 함수가 너무 길어져서 나누겟습니다. dd
/*
double CPstBuff::GetMotrMrkPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int    r  = 0 , c  = 0 ;
    int    gr = 0 , gc = 0 ;
    double dPos = 0;

    FindChipMrk(r , c);

    //X , Y Pos Setting.
    double dXPos =  PM.GetValue(miPSB_XTrm , pvPSB_XMrkWorkStart) +
                    c * OM.DevInfo.dColPitch ;

    double dYPos =  PM.GetValue(miPSB_YTrm , pvPSB_YMrkWorkStart) +
                    r * OM.DevInfo.dRowPitch ;

    if( OM.DevInfo.iColGrCnt) {
        gc = c / OM.DevInfo.iColGrCnt ; // integer
        dXPos = dXPos - (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc ;
    }

    if( OM.DevInfo.iRowGrCnt) {
        gr = r / OM.DevInfo.iRowGrCnt ; // integer
        dYPos = dYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr;
    }

    if(_iMotr == miPSB_XTrm){
        switch(_iPstnId) {
            default                    : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_XMrkWorkStart   : dPos = PM.GetValue (_iMotr , pvPSB_XMrkWorkStart); break ;
            case piPSB_XMrkWork        : dPos = dXPos ;                                     break ;
        }
    }
    else if(_iMotr == miPSB_YTrm){
        switch(_iPstnId) {
            default                    : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_YMrkWorkStart   : dPos = PM.GetValue (_iMotr , pvPSB_YMrkWorkStart); break ;
            case piPSB_YMrkWork        : dPos = dYPos ;                                     break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
*/
bool CPstBuff::GetChipFlipY(int r, int c , int &rr , int &rc)
{
    rr = OM.DevInfo.iRowCnt - r - 1;
    rc =                      c    ;
}

double CPstBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int    r  , c  ;
    int    gr , gc ;
    double dPos ;

    if(_iMotr == miPSB_XTrm){
        switch(_iPstnId) {
            case piPSB_XTrmRslt0   : FindChip(riPSB , csRslt0  , r , c); break ;
            case piPSB_XTrmRslt1   : FindChip(riPSB , csRslt1  , r , c); break ;
            case piPSB_XTrmRslt2   : FindChip(riPSB , csRslt2  , r , c); break ;
            case piPSB_XTrmRslt3   : FindChip(riPSB , csRslt3  , r , c); break ;
            case piPSB_XTrmRslt4   : FindChip(riPSB , csRslt4  , r , c); break ;
            case piPSB_XTrmRslt5   : FindChip(riPSB , csRslt5  , r , c); break ;
            case piPSB_XTrmRslt6   : FindChip(riPSB , csRslt6  , r , c); break ;
            case piPSB_XTrmRslt7   : FindChip(riPSB , csRslt7  , r , c); break ;
            case piPSB_XTrmRslt8   : FindChip(riPSB , csRslt8  , r , c); break ;
            case piPSB_XTrmRslt9   : FindChip(riPSB , csRslt9  , r , c); break ;
            case piPSB_XTrmRsltA   : FindChip(riPSB , csRsltA  , r , c); break ;
            case piPSB_XTrmRsltB   : FindChip(riPSB , csRsltB  , r , c); break ;
            case piPSB_XTrmRsltC   : FindChip(riPSB , csRsltC  , r , c); break ;
            case piPSB_XTrmRsltD   : FindChip(riPSB , csRsltD  , r , c); break ;
            case piPSB_XTrmRsltE   : FindChip(riPSB , csRsltE  , r , c); break ;
            case piPSB_XTrmRsltF   : FindChip(riPSB , csRsltF  , r , c); break ;
            case piPSB_XTrmRsltG   : FindChip(riPSB , csRsltG  , r , c); break ;
            case piPSB_XTrmRsltH   : FindChip(riPSB , csRsltH  , r , c); break ;
            case piPSB_XTrmRsltI   : FindChip(riPSB , csRsltI  , r , c); break ;
            case piPSB_XTrmRsltJ   : FindChip(riPSB , csRsltJ  , r , c); break ;
            case piPSB_XTrmRsltK   : FindChip(riPSB , csRsltK  , r , c); break ;
            case piPSB_XTrmRsltL   : FindChip(riPSB , csRsltL  , r , c); break ;
            case piPSB_XVsnRsltMM  : r = m_iReviewR; c = m_iReviewC;    break ;

        }
    }
    else if(_iMotr == miPSB_YTrm){
        switch(_iPstnId) {
            case piPSB_YTrmRslt0  : FindChip(riPSB , csRslt0  , r , c); break ;
            case piPSB_YTrmRslt1  : FindChip(riPSB , csRslt1  , r , c); break ;
            case piPSB_YTrmRslt2  : FindChip(riPSB , csRslt2  , r , c); break ;
            case piPSB_YTrmRslt3  : FindChip(riPSB , csRslt3  , r , c); break ;
            case piPSB_YTrmRslt4  : FindChip(riPSB , csRslt4  , r , c); break ;
            case piPSB_YTrmRslt5  : FindChip(riPSB , csRslt5  , r , c); break ;
            case piPSB_YTrmRslt6  : FindChip(riPSB , csRslt6  , r , c); break ;
            case piPSB_YTrmRslt7  : FindChip(riPSB , csRslt7  , r , c); break ;
            case piPSB_YTrmRslt8  : FindChip(riPSB , csRslt8  , r , c); break ;
            case piPSB_YTrmRslt9  : FindChip(riPSB , csRslt9  , r , c); break ;
            case piPSB_YTrmRsltA  : FindChip(riPSB , csRsltA  , r , c); break ;
            case piPSB_YTrmRsltB  : FindChip(riPSB , csRsltB  , r , c); break ;
            case piPSB_YTrmRsltC  : FindChip(riPSB , csRsltC  , r , c); break ;
            case piPSB_YTrmRsltD  : FindChip(riPSB , csRsltD  , r , c); break ;
            case piPSB_YTrmRsltE  : FindChip(riPSB , csRsltE  , r , c); break ;
            case piPSB_YTrmRsltF  : FindChip(riPSB , csRsltF  , r , c); break ;
            case piPSB_YTrmRsltG  : FindChip(riPSB , csRsltG  , r , c); break ;
            case piPSB_YTrmRsltH  : FindChip(riPSB , csRsltH  , r , c); break ;
            case piPSB_YTrmRsltI  : FindChip(riPSB , csRsltI  , r , c); break ;
            case piPSB_YTrmRsltJ  : FindChip(riPSB , csRsltJ  , r , c); break ;
            case piPSB_YTrmRsltK  : FindChip(riPSB , csRsltK  , r , c); break ;
            case piPSB_YTrmRsltL  : FindChip(riPSB , csRsltL  , r , c); break ;
            case piPSB_YVsnRsltMM : r = m_iReviewR; c = m_iReviewC;     break ;

        }
    }

    double dXTrmPos ;
    double dYTrmPos ;
    double dXVsnPos ;
    double dYVsnPos ;
    double dXMrkPos ;
    double dYMrkPos ;

    int    iFlipC ;
    int    iFlipR ;

    GetChipFlipY(r,c,iFlipR,iFlipC);
    //아 pi 없어서 rc , 쓰레기 값 들어가 있음. dd
    //Find Chip 합쳐서 해야 되는데 굳이 ... dd
    int mr = 0 , mc = 0 ;
    FindChipMrk(mr, mc);


    //Trim Pos
    dXTrmPos =  PM.GetValue(miPSB_XTrm , pvPSB_XTrmWorkStart) -
                iFlipC * OM.DevInfo.dColPitch ;
    dYTrmPos =  PM.GetValue(miPSB_YTrm , pvPSB_YTrmWorkStart) +
                iFlipR * OM.DevInfo.dRowPitch ;

    //여기서수정함. 2012 11 19 김동우. Y로 서브 그룹 카운트 적용함.
    //원본
/*
    if( OM.DevInfo.iColGrCnt) {
        dXTrmPos = dXTrmPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ) * (iFlipC / OM.DevInfo.iColGrCnt) ;
        dXVsnPos = dXVsnPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ) * (c      / OM.DevInfo.iColGrCnt) ;
        dXMrkPos = dXMrkPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ) * (mc     / OM.DevInfo.iColGrCnt) ;
    }

    if( OM.DevInfo.iRowGrCnt) {
        dYTrmPos = dYTrmPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch ) * (iFlipR / OM.DevInfo.iRowGrCnt) ;
        dYVsnPos = dYVsnPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch ) * (r      / OM.DevInfo.iRowGrCnt) ;
        dYMrkPos = dYMrkPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch ) * (mr     / OM.DevInfo.iRowGrCnt) ;
    }
*/
    //수정

    //여기서수정함. 2012 11 19 김동우. Y로 서브 그룹 카운트 적용함.
    //여기서 수정함. 2014 03 31 고준선. x서브 그룹 추가 함.
    double dSubRowGroupGap = 0.0 ;
    double dSubColGroupGap = 0.0 ;

    if(OM.DevInfo.iRowSubGroupCount) dSubRowGroupGap = OM.DevInfo.dRowSubGroupGap - OM.DevInfo.dRowPitch ;
    else                             dSubRowGroupGap = 0.0 ;

    if(OM.DevInfo.iColSubGroupCount) dSubColGroupGap = OM.DevInfo.dColSubGroupGap - OM.DevInfo.dColPitch ;
    else                             dSubColGroupGap = 0.0 ;


    if( OM.DevInfo.iColGrCnt) {
        dXTrmPos = dXTrmPos + ((OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (iFlipC / OM.DevInfo.iColGrCnt)) ;
        dXVsnPos = dXVsnPos + ((OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (c      / OM.DevInfo.iColGrCnt)) ;
        dXMrkPos = dXMrkPos + ((OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (mc     / OM.DevInfo.iColGrCnt)) ;
    }


    if( OM.DevInfo.iRowGrCnt           ) dYTrmPos = dYTrmPos + ((OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (iFlipR / OM.DevInfo.iRowGrCnt)) ;
    if( OM.DevInfo.iColSubGroupCount   ) dYTrmPos = dYTrmPos + ((dSubColGroupGap ) * (iFlipR / OM.DevInfo.iColSubGroupCount)) ;
    if( OM.DevInfo.iRowSubGroupCount   ) dYTrmPos = dYTrmPos + ((dSubRowGroupGap ) * (iFlipR / OM.DevInfo.iRowSubGroupCount)) ;

    if(_iMotr == miPSB_TFlp){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_TFlpWait      : dPos = PM.GetValue (_iMotr , pvPSB_TFlpWait     ); break ;
            case piPSB_TFlpWork      : dPos = PM.GetValue (_iMotr , pvPSB_TFlpWork     ); break ;
        }
    }

    else if(_iMotr == miPSB_XIns){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piPSB_XInsWait      : dPos = PM.GetValue (_iMotr , pvPSB_XInsWait     ); break ;
            case piPSB_XInsInsert    : dPos = PM.GetValue (_iMotr , pvPSB_XInsInsert   ); break ;
            case piPSB_XInsOut       : dPos = PM.GetValue (_iMotr , pvPSB_XInsOut      ); break ;
        }
    }

    else if(_iMotr == miPSB_XTrm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      );     break ;
            case piPSB_XTrmWait      : dPos = PM.GetValue (_iMotr , pvPSB_XTrmWait     );     break ;
            case piPSB_XTrmWorkStart : dPos = PM.GetValue (_iMotr , pvPSB_XTrmWorkStart);     break ;
            case piPSB_XTrmRslt0     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt1     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt2     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt3     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt4     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt5     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt6     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt7     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt8     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRslt9     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltA     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltB     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltC     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltD     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltE     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltF     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltG     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltH     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltI     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltJ     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltK     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XTrmRsltL     : dPos = dXTrmPos ;                                      break ;
            case piPSB_XVsnRslt      : dPos = dXVsnPos ;                                      break ;
            case piPSB_XVsnRsltMM    : dPos = dXVsnPos ;                                      break ;
        }
    }

    else if(_iMotr == miPSB_YTrm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                      );     break ;
            case piPSB_YTrmWait      : dPos = PM.GetValue (_iMotr , pvPSB_YTrmWait     );     break ;
            case piPSB_YTrmWorkStart : dPos = PM.GetValue (_iMotr , pvPSB_YTrmWorkStart);     break ;
            case piPSB_YTrmRslt0     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt1     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt2     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt3     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt4     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt5     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt6     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt7     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt8     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRslt9     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltA     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltB     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltC     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltD     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltE     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltF     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltG     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltH     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltI     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltJ     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltK     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YTrmRsltL     : dPos = dYTrmPos ;                                      break ;
            case piPSB_YVsnRslt      : dPos = dYVsnPos ;                                      break ;
            case piPSB_YVsnRsltMM    : dPos = dYVsnPos ;                                      break ;            
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPstBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId) )) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;
                                                        //sun 이거꼭 추가 하기......
    bool isTrimDn     = !AT_Complete(aiPSB_PnchDnUp , ccBwd);
    bool isTrimXWait  = !MT_CmprPos (miPSB_XTrm    , PM.GetValue(miPSB_XTrm ,pvPSB_XTrmWait));
    bool isTrimYWait  = !MT_CmprPos (miPSB_YTrm    , PM.GetValue(miPSB_YTrm ,pvPSB_YTrmWait));

//    if(MT_GetCmdPos(mips)
    bool isFlpMove    =(!MT_CmprPos (miPSB_TFlp    , PM.GetValue(miPSB_TFlp ,pvPSB_TFlpWait)) &&
                        !MT_CmprPos (miPSB_TFlp    , PM.GetValue(miPSB_TFlp ,pvPSB_TFlpWork))) ;

    bool isFlpWait    = !MT_CmprPos (miPSB_TFlp    , PM.GetValue(miPSB_TFlp ,pvPSB_TFlpWait));
    bool isIstUp      = !AT_Complete(aiPSB_IndxUpDn , ccBwd);
//    bool isIstWait  = !MT_CmprPos (miPSB_XIns    , PM.GetValue(miPSB_XIns ,pvPSB_XInsWait));
    bool isNotIstWait =  MT_GetCmdPos(miPSB_XIns) > PM.GetValue(miPSB_XIns,pvPSB_XInsInsert) ;//+ 1 ;

    if(_iMotr == miPSB_XIns){
        if(isFlpMove){ sMsg = MT_GetName(miPSB_TFlp )+" is Moving" ; bRet = false ;}
    }
    else if(_iMotr == miPSB_YTrm){
        if(isFlpMove   ){ sMsg = MT_GetName(miPSB_TFlp    )+" is Moving"        ; bRet = false ;}
        if(isTrimDn    ){ sMsg = AT_GetName(aiPSB_PnchDnUp)+" is Fwd"           ; bRet = false ;}
    }
    else if(_iMotr == miPSB_XTrm){
        if(isFlpMove   ){ sMsg = MT_GetName(miPSB_TFlp    )+" is Moving"        ; bRet = false ;}
        if(isTrimDn    ){ sMsg = AT_GetName(aiPSB_PnchDnUp)+" is Fwd"           ; bRet = false ;}
    }
    else if(_iMotr == miPSB_TFlp){
        if(isTrimXWait ){ sMsg = MT_GetName(miPSB_XTrm  )+" is not in wait pos" ; bRet = false ;}
        if(isTrimYWait ){ sMsg = MT_GetName(miPSB_YTrm  )+" is not in wait pos" ; bRet = false ;}
        if(isNotIstWait){ sMsg = MT_GetName(miPSB_XIns  )+" is not in wait pos" ; bRet = false ;}
//        if(isIstUp     ){ sMsg = AT_GetName(aiPSB_Insert)+" is Fwd"             ; bRet = false ;} 애 미리 포워드 시켜놔서 안됨. dd
//        if(isIstWait  ){ sMsg = MT_GetName(miPSB_XIns )+" is not in wait pos" ; bRet = false ;} 나중에 수정해라 x도 넣고 앤좀 빼고.dd
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CPstBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool isTrimDn = AT_Complete(aiPSB_Marking , ccFwd ) ;
    bool isXMoving   = MT_GetStopInpos(miPSB_XTrm);
    bool isYMoving   = MT_GetStopInpos(miPSB_YTrm);

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}


bool CPstBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPstBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPstBuff::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 15000 )) {
        EM_SetErr(eiPSB_HomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    //if(Stat.bReqStop) {
    //    Step.iHome = 0;
    //    return true ;
    //}

    switch (Step.iHome) {
        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_Reset(miPSB_XTrm);
                 MT_Reset(miPSB_YTrm);
                 MT_Reset(miPSB_XIns);
                 MT_Reset(miPSB_TFlp);

                 MT_SetServo(miPSB_XTrm,true);
                 MT_SetServo(miPSB_YTrm,true);
                 MT_SetServo(miPSB_XIns,true);
                 MT_SetServo(miPSB_TFlp,true);

                 AT_MoveCyl(aiPSB_AlgnFwBw    , ccBwd);
                 AT_MoveCyl(aiPSB_IndxUpDn    , ccBwd);
                 AT_MoveCyl(aiPSB_FlprOpCl    , ccFwd);
                 AT_MoveCyl(aiPSB_PnchDnUp    , ccBwd);

                 AT_MoveCyl(aiPSB_SortFwBw    , ccBwd);
                 AT_MoveCyl(aiPSB_SrtStprFwBw , ccBwd);

                 AT_MoveCyl(aiPSB_PshrFwBw    , ccBwd);
                 AT_MoveCyl(aiPSB_PshrDnUp    , ccBwd);
                 AT_MoveCyl(aiPSB_GripUpDn    , ccBwd);

                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_AlgnFwBw    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_IndxUpDn    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_FlprOpCl    , ccFwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PnchDnUp    , ccBwd)) return false ;

                 if(!AT_MoveCyl(aiPSB_SortFwBw    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_SrtStprFwBw , ccBwd)) return false ;

                 if(!AT_MoveCyl(aiPSB_PshrFwBw    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_PshrDnUp    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_GripUpDn    , ccBwd)) return false ;

                 m_iCrntBin = 1 ;


                 Step.iHome++ ;
                 return false ;

        case 12:
                 Step.iHome++ ;
                 return false ;

        case 13: MT_DoHome(miPSB_XTrm);
                 MT_DoHome(miPSB_YTrm);
                 MT_DoHome(miPSB_XIns);
                 Step.iHome++ ;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miPSB_XTrm)) return false ;
                 if(!MT_GetHomeEnd(miPSB_YTrm)) return false ;
                 if(!MT_GetHomeEnd(miPSB_XIns)) return false ;
                 MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm,pvPSB_XTrmWait)) ;
                 MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm,pvPSB_YTrmWait)) ;
                 MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait)) ;

                 Step.iHome++ ;
                 return false ;

        case 15: if(!MT_GoAbsMan(miPSB_XTrm , PM.GetValue(miPSB_XTrm,pvPSB_XTrmWait)))return false ;
                 if(!MT_GoAbsMan(miPSB_YTrm , PM.GetValue(miPSB_YTrm,pvPSB_YTrmWait)))return false ;
                 if(!MT_GoAbsMan(miPSB_XIns , PM.GetValue(miPSB_XIns,pvPSB_XInsWait)))return false ;
                 Step.iHome++;
                 return false ;

        case 16: MT_DoHome(miPSB_TFlp);
                 Step.iHome++;
                 return false;

        case 17: if(!MT_GetHomeEnd(miPSB_TFlp)) return false ;
                 MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait)) ;

                 Step.iHome++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait)))return false ;
                 MoveActr(aiPSB_FlprOpCl , ccBwd);
                 Step.iHome++;
                 return false ;

        case 19: if(!MoveActr(aiPSB_FlprOpCl , ccBwd)) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CPstBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;

    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPstBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPstBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPSB_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    int iTemp  = 0;
    int iRslt0 = 0;
    int iRslt1 = 0;
    int iRslt2 = 0;
    int iRslt3 = 0;
    int iRslt4 = 0;
    int iRslt5 = 0;
    int iRslt6 = 0;
    int iRslt7 = 0;
    int iRslt8 = 0;
    int iRslt9 = 0;
    int iRsltA = 0;
    int iRsltB = 0;
    int iRsltC = 0;
    int iRsltD = 0;
    int iRsltE = 0;
    int iRsltF = 0;
    int iRsltG = 0;
    int iRsltH = 0;
    int iRsltI = 0;
    int iRsltJ = 0;
    int iRsltK = 0;
    int iRsltL = 0;

    bool bTemp  = false ;

    bool bFlpFw = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: MoveActr(aiPSB_AlgnFwBw    , ccBwd);
                 MoveActr(aiPSB_IndxUpDn    , ccBwd);
                 MoveActr(aiPSB_PnchDnUp    , ccBwd);

                 MoveActr(aiPSB_SortFwBw    , ccBwd);
                 MoveActr(aiPSB_SrtStprFwBw , ccBwd);

                 MoveActr(aiPSB_PshrFwBw    , ccBwd);
                 MoveActr(aiPSB_PshrDnUp    , ccBwd);
                 MoveActr(aiPSB_GripUpDn    , ccBwd);

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_AlgnFwBw    , ccBwd))return false ;
                 if(!MoveActr(aiPSB_IndxUpDn    , ccBwd))return false ;
                 if(!MoveActr(aiPSB_PnchDnUp    , ccBwd))return false ;

                 if(!MoveActr(aiPSB_SortFwBw    , ccBwd))return false ;
                 if(!MoveActr(aiPSB_SrtStprFwBw , ccBwd))return false ;

                 if(!MoveActr(aiPSB_PshrFwBw    , ccBwd))return false ;
                 if(!MoveActr(aiPSB_PshrDnUp    , ccBwd))return false ;

                 if(!MoveActr(aiPSB_GripUpDn    , ccBwd))return false ;

                 m_iCrntBin = 1 ;

                 //if(MT_CmprPos(miPSB_TFlp , PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait)) && //TODO :: ToStart ,,, 으악ㄱㄱㄱㄱㄱㄱㄱㄱㄱ 이게 모야... dd
                 //  (DM.ARAY[riVS4].GetCntStat(csWork)+DM.ARAY[riVS4].GetCntStat(csGood)) == (OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt) &&
                 //   OM.CmnOptn.bUsePenMrk && AT_Complete(aiPSB_FlprCmp,ccFwd)) {
                 //    Step.iToStart = 0;
                 //    return true ;
                 //}
                 MoveMotr(miPSB_YTrm , piPSB_YTrmWait);
                 MoveMotr(miPSB_XIns , piPSB_XInsWait);

                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miPSB_YTrm , piPSB_YTrmWait)) return false ;
                 if(!MoveMotr(miPSB_XIns , piPSB_XInsWait)) return false ;
                 MoveMotr(miPSB_XTrm , piPSB_XTrmWait);
                 //여기 할차례 OM.CmmOptn.bUseMark 이거해서 플리퍼 뒤집어라. dd
                 //To Stop 에도 몬가 걸게 있을기...dd
                 Step.iToStart++;
                 return false;

        case 13: if(!MoveMotr(miPSB_XTrm , piPSB_XTrmWait)) return false ;
                 bFlpFw = AT_Complete(aiPSB_FlprOpCl,ccFwd);
                 if(bFlpFw) {
                     Step.iToStart++;
                     return false ;
                 }
                 else {
                     Step.iToStart = 20;
                     return false ;
                 }


        case 14: if(!MoveActr(aiPSB_FlprOpCl,ccFwd)) return false ; //펜 마킹 할때 뒤집지 않고 그냥 클램프로만 잡고 한다네...아이구야...dd
                 MoveMotr(miPSB_TFlp,piPSB_TFlpWait);
                 Step.iToStart++;
                 return false ;

        case 15: if(!MoveMotr(miPSB_TFlp,piPSB_TFlpWait)) return false ;
//                 MoveActr(aiPSB_FlprCmp,ccBwd); //아 소리 가라짓좀 하겟음. dd
//                 Step.iToStart++;
//                 return false ;
//
//        case 15: if(!MoveActr(aiPSB_FlprCmp,ccBwd)) return false ;
                 Step.iToStart = 0;
                 return true ;

        case 20: MoveActr(aiPSB_IndxUpDn  , ccBwd);
                 MoveMotr(miPSB_TFlp,piPSB_TFlpWait);
                 Step.iToStart++;
                 return false ;

        case 21: if(!MoveActr(aiPSB_IndxUpDn  , ccBwd)) return false ;
                 if(!MoveMotr(miPSB_TFlp,piPSB_TFlpWait)) return false ;
                 MoveMotr(miPSB_XIns,piPSB_XInsWait);
                 MoveActr(aiPSB_FlprOpCl , ccBwd); //mipsb_Xins 가 Insert포지션으로 이동시 Fwd상태면 안되기 떄문JS
                 Step.iToStart++;
                 return false ;

        case 22: if(!MoveMotr(miPSB_XIns,piPSB_XInsWait)) return false ;
                 if(!MoveActr(aiPSB_FlprOpCl , ccBwd)) return false;
                 MoveActr(aiPSB_IndxUpDn,ccFwd);
                 Step.iToStart++;
                 return false ;

        case 23: if(!MoveActr(aiPSB_IndxUpDn,ccFwd)) return false ;
                 MT_GoAbsRun(miPSB_XIns , GetMotrPos(miPSB_XIns , piPSB_XInsInsert) - PSB_INSERT_DIS );
                 Step.iToStart++;
                 return false ;

        case 24: if(!MT_GoAbsRun(miPSB_XIns , GetMotrPos(miPSB_XIns , piPSB_XInsInsert) - PSB_INSERT_DIS )) return false ;
                 MT_GoAbs(miPSB_XIns , GetMotrPos(miPSB_XIns , piPSB_XInsInsert) , PSB_INSERT_SPD );
                 Step.iToStart++;
                 return false ;

        case 25: if(!MT_GoAbs(miPSB_XIns , GetMotrPos(miPSB_XIns , piPSB_XInsInsert) , PSB_INSERT_SPD )) return false ;
                 MoveActr(aiPSB_AlgnFwBw,ccFwd);
                 Step.iToStart++;
                 return false ;

        case 26: if(!MoveActr(aiPSB_AlgnFwBw,ccFwd)) return false ;
                 MoveActr(aiPSB_FlprOpCl,ccBwd);
                 Step.iToStart++;
                 return false ;

        case 27: if(!MoveActr(aiPSB_FlprOpCl,ccBwd)) return false ;
                 MoveMotr(miPSB_XIns     , piPSB_XInsWait);
                 MoveActr(aiPSB_IndxUpDn , ccBwd         );
                 MoveActr(aiPSB_AlgnFwBw , ccBwd         );
                 Step.iToStart++;
                 return false ;

        case 28: if(!MoveMotr(miPSB_XIns     , piPSB_XInsWait)) return false ;
                 if(!MoveActr(aiPSB_IndxUpDn , ccBwd         )) return false ;
                 if(!MoveActr(aiPSB_AlgnFwBw , ccBwd         )) return false ;
//                 Step.iToStart++;
//                 return false ;    //더이상 싸이클이 없음...Js
                 Step.iToStart = 0;
                 return true;








/*
                 if(!DM.ARAY[riRVP].CheckAllStat(csNone)){
                     if(!DM.ARAY[riRVP].CheckAllStat(csWork)){
                         iRslt0 = DM.ARAY[riPSB].GetCntStat(csRslt0 ) ; iTemp += iRslt0 ;
                         iRslt1 = DM.ARAY[riPSB].GetCntStat(csRslt1 ) ; iTemp += iRslt1 ;
                         iRslt2 = DM.ARAY[riPSB].GetCntStat(csRslt2 ) ; iTemp += iRslt2 ;
                         iRslt3 = DM.ARAY[riPSB].GetCntStat(csRslt3 ) ; iTemp += iRslt3 ;
                         iRslt4 = DM.ARAY[riPSB].GetCntStat(csRslt4 ) ; iTemp += iRslt4 ;
                         iRslt5 = DM.ARAY[riPSB].GetCntStat(csRslt5 ) ; iTemp += iRslt5 ;
                         iRslt6 = DM.ARAY[riPSB].GetCntStat(csRslt6 ) ; iTemp += iRslt6 ;
                         iRslt7 = DM.ARAY[riPSB].GetCntStat(csRslt7 ) ; iTemp += iRslt7 ;
                         iRslt8 = DM.ARAY[riPSB].GetCntStat(csRslt8 ) ; iTemp += iRslt8 ;
                         iRslt9 = DM.ARAY[riPSB].GetCntStat(csRslt9 ) ; iTemp += iRslt9 ;
                         iRsltA = DM.ARAY[riPSB].GetCntStat(csRsltA ) ; iTemp += iRsltA ;
                         iRsltB = DM.ARAY[riPSB].GetCntStat(csRsltB ) ; iTemp += iRsltB ;
                         iRsltC = DM.ARAY[riPSB].GetCntStat(csRsltC ) ; iTemp += iRsltC ;
                         iRsltD = DM.ARAY[riPSB].GetCntStat(csRsltD ) ; iTemp += iRsltD ;
                         iRsltE = DM.ARAY[riPSB].GetCntStat(csRsltE ) ; iTemp += iRsltE ;
                         iRsltF = DM.ARAY[riPSB].GetCntStat(csRsltF ) ; iTemp += iRsltF ;
                         iRsltG = DM.ARAY[riPSB].GetCntStat(csRsltG ) ; iTemp += iRsltG ;
                         iRsltH = DM.ARAY[riPSB].GetCntStat(csRsltH ) ; iTemp += iRsltH ;
                         iRsltI = DM.ARAY[riPSB].GetCntStat(csRsltI ) ; iTemp += iRsltI ;
                         iRsltJ = DM.ARAY[riPSB].GetCntStat(csRsltJ ) ; iTemp += iRsltJ ;
                         iRsltK = DM.ARAY[riPSB].GetCntStat(csRsltK ) ; iTemp += iRsltK ;
                         iRsltL = DM.ARAY[riPSB].GetCntStat(csRsltL ) ; iTemp += iRsltL ;

                         //토탈 페일 갯수.
                         if( OM.CmnOptn.iCheckFailCnt <= iTemp) {
                         EM_SetErr(eiPSB_TotalFailCnt);
                         }

                         //같은 에러 갯수.
                         if(OM.CmnOptn.iCheckFailCnt0 &&iRslt0>= OM.CmnOptn.iCheckFailCnt0) EM_SetErr(eiSameFailCnt0);
                         if(OM.CmnOptn.iCheckFailCnt1 &&iRslt1>= OM.CmnOptn.iCheckFailCnt1) EM_SetErr(eiSameFailCnt1);
                         if(OM.CmnOptn.iCheckFailCnt2 &&iRslt2>= OM.CmnOptn.iCheckFailCnt2) EM_SetErr(eiSameFailCnt2);
                         if(OM.CmnOptn.iCheckFailCnt3 &&iRslt3>= OM.CmnOptn.iCheckFailCnt3) EM_SetErr(eiSameFailCnt3);
                         if(OM.CmnOptn.iCheckFailCnt4 &&iRslt4>= OM.CmnOptn.iCheckFailCnt4) EM_SetErr(eiSameFailCnt4);
                         if(OM.CmnOptn.iCheckFailCnt5 &&iRslt5>= OM.CmnOptn.iCheckFailCnt5) EM_SetErr(eiSameFailCnt5);
                         if(OM.CmnOptn.iCheckFailCnt6 &&iRslt6>= OM.CmnOptn.iCheckFailCnt6) EM_SetErr(eiSameFailCnt6);
                         if(OM.CmnOptn.iCheckFailCnt7 &&iRslt7>= OM.CmnOptn.iCheckFailCnt7) EM_SetErr(eiSameFailCnt7);
                         if(OM.CmnOptn.iCheckFailCnt8 &&iRslt8>= OM.CmnOptn.iCheckFailCnt8) EM_SetErr(eiSameFailCnt8);
                         if(OM.CmnOptn.iCheckFailCnt9 &&iRslt9>= OM.CmnOptn.iCheckFailCnt9) EM_SetErr(eiSameFailCnt9);
                         if(OM.CmnOptn.iCheckFailCntA &&iRsltA>= OM.CmnOptn.iCheckFailCntA) EM_SetErr(eiSameFailCntA);
                         if(OM.CmnOptn.iCheckFailCntB &&iRsltB>= OM.CmnOptn.iCheckFailCntB) EM_SetErr(eiSameFailCntB);
                         if(OM.CmnOptn.iCheckFailCntC &&iRsltC>= OM.CmnOptn.iCheckFailCntC) EM_SetErr(eiSameFailCntC);
                         if(OM.CmnOptn.iCheckFailCntD &&iRsltD>= OM.CmnOptn.iCheckFailCntD) EM_SetErr(eiSameFailCntD);
                         if(OM.CmnOptn.iCheckFailCntE &&iRsltE>= OM.CmnOptn.iCheckFailCntE) EM_SetErr(eiSameFailCntE);
                         if(OM.CmnOptn.iCheckFailCntF &&iRsltF>= OM.CmnOptn.iCheckFailCntF) EM_SetErr(eiSameFailCntF);
                         if(OM.CmnOptn.iCheckFailCntG &&iRsltG>= OM.CmnOptn.iCheckFailCntG) EM_SetErr(eiSameFailCntG);
                         if(OM.CmnOptn.iCheckFailCntH &&iRsltH>= OM.CmnOptn.iCheckFailCntH) EM_SetErr(eiSameFailCntH);
                         if(OM.CmnOptn.iCheckFailCntI &&iRsltI>= OM.CmnOptn.iCheckFailCntI) EM_SetErr(eiSameFailCntI);
                         if(OM.CmnOptn.iCheckFailCntJ &&iRsltJ>= OM.CmnOptn.iCheckFailCntJ) EM_SetErr(eiSameFailCntJ);
                         if(OM.CmnOptn.iCheckFailCntK &&iRsltK>= OM.CmnOptn.iCheckFailCntK) EM_SetErr(eiSameFailCntK);
                         if(OM.CmnOptn.iCheckFailCntL &&iRsltL>= OM.CmnOptn.iCheckFailCntL) EM_SetErr(eiSameFailCntL);
                     }
                 }


                 Step.iToStart = 0 ;
                 return true ;
*/
    }
}

bool CPstBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPSB_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //bool bWaitPos = MT_CmprPos(miPSB_XTrm,GetMotrPos(miPSB_XTrm,piPSB_XTrmWait)) && MT_CmprPos(miPSB_YTrm,GetMotrPos(miPSB_YTrm,piPSB_YTrmWait)) ;
    static bool bWaitPos ;
    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10: //IO_SetY(yPSB_CamLight,false);
//                 bWaitPos = MT_CmprPos(miPSB_TFlp,GetMotrPos(miPSB_TFlp,piPSB_TFlpWork)) ; 왜 이렇게 되있는지 잘 모르겟네. dd
                 bWaitPos = MT_CmprPos(miPSB_TFlp,PM.GetValue(miPSB_TFlp,pvPSB_TFlpWait));
                 Step.iToStop ++ ;
                 return false ;

        case 11: //IO_SetY(yPSB_CamLight,false);
                 MoveActr(aiPSB_AlgnFwBw , ccBwd);
                 MoveActr(aiPSB_PnchDnUp , ccBwd);

                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveActr(aiPSB_AlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_PnchDnUp , ccBwd)) return false ;
                 MoveMotr(miPSB_XTrm,piPSB_XTrmWait);
                 MoveMotr(miPSB_YTrm,piPSB_YTrmWait);
                 Step.iToStop ++;
                 return false;

        case 13: if(!MoveMotr(miPSB_XTrm,piPSB_XTrmWait)) return false ;
                 if(!MoveMotr(miPSB_YTrm,piPSB_YTrmWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;

    }
}


bool CPstBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

//    if(IO_GetX(xPSB_InsertOverload)) {
//        EM_SetErr( eiPSB_InsertOverload );
//        MT_Stop(miPSB_XIns);
//        return false ;
//    }
    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //우선 비전 셋팅 위해서 가라짓함..JS 나중에 수정JS.

        bool isExstFail = DM.ARAY[riPSB].GetCntStat(csRslt0) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt1) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt2) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt3) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt4) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt5) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt6) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt7) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt8) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt9) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltA) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltB) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltC) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltD) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltE) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltF) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltG) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltH) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltI) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltJ) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltK) ||
                          DM.ARAY[riPSB].GetCntStat(csRsltL) ;

        bool isULDIdle    = (DM.ARAY[riULD].GetCntStat(csEmpty) && ULD.GetSeqStep()== CUnLoader::scIdle) ;

        bool isULDWorkPos = (DM.ARAY[riULD].GetCntStat(csEmpty)&& MT_CmprPos(miULD_ZCmp , ULD.GetMotrPos(miULD_ZCmp , piULD_ZWork)));

        bool isCycleOutUL = !DM.ARAY[riPSB].CheckAllStat(csNone) && !isExstFail  && isULDIdle && isULDWorkPos ;
                            //다시 수정해야함..JS 우선 가라가라.

        //처음 들어 왔을때 데이터 정리를 위해.
        //bool isCycleIn      = RAL.GetSeqStep() != CRail::scMove && !DM.ARAY[riPSB].CheckAllStat(csNone) && IO_GetXDn(xPSB_PkgIn2);

        //bool isCycleIn      = !DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riVS4].CheckAllStat(csNone) && RAL.GetSeqStep() != CRail::scMove ;
        //bool isCycleVisn    =  false ;//오토에서 안하고 메뉴얼에서만.. DM.ARAY[riVS4].GetCntStat(csUnkwn);
                                    //Total 카운트 이상일때 In 에서 에러 띠우고 카메라 첫위치로 보냄.dd
        //bool isCycleFlip    = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  isExstFail  && !MT_CmprPos(miPSB_TFlp , GetMotrPos(miPSB_TFlp , piPSB_TFlpWork)) ;

        if(IO_GetXDn(xPSB_PkgIn2)) m_bDnEdgePSBPkg2 = true;

        bool isCycleIn      = RAL.GetSeqStep() != CRail::scMove    && !DM.ARAY[riPSB].CheckAllStat(csNone) && !IO_GetX(xPSB_PkgIn2) && DM.ARAY[riPSB].GetStep() == stWait;
        bool isCycleFlip    = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  isExstFail  && !MT_CmprPos(miPSB_TFlp , GetMotrPos(miPSB_TFlp , piPSB_TFlpWork)) && DM.ARAY[riPSB].GetStep() == stAlign;
        bool isCycleWork    = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  isExstFail  &&  MT_CmprPos(miPSB_TFlp , GetMotrPos(miPSB_TFlp , piPSB_TFlpWork)) ;
        bool isCycleOut     = !DM.ARAY[riPSB].CheckAllStat(csNone) && !isExstFail  && isULDIdle && isULDWorkPos && RAL.GetSeqStep() != CRail::scMove ;
        bool isConEnd       =  DM.ARAY[riPSB].CheckAllStat(csNone) && !IO_GetX(xPSB_PkgOut3) && !IO_GetX(xPSB_PkgOut3);

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleIn          ) {Trace(m_sPartName.c_str(),"CycleIn           Stt"); Step.iSeq = scIn    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleFlip        ) {Trace(m_sPartName.c_str(),"CycleFlip         Stt"); Step.iSeq = scFlip  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork        ) {Trace(m_sPartName.c_str(),"CycleWork         Stt"); Step.iSeq = scWork  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut         ) {Trace(m_sPartName.c_str(),"CycleOut          Stt"); Step.iSeq = scOut   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd           ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                                                                                                return false ;
        case  scIn     : if(CycleIn         ()){Trace(m_sPartName.c_str(),"CycleIn          End");Step.iSeq = scIdle ;} return false ;
        case  scFlip   : if(CycleFlip       ()){Trace(m_sPartName.c_str(),"CycleFlip        End");Step.iSeq = scIdle ;} return false ;
        case  scWork   : if(CycleWork       ()){Trace(m_sPartName.c_str(),"CycleWork        End");Step.iSeq = scIdle ;} return false ;
        case  scOut    : if(CycleOut        ()){Trace(m_sPartName.c_str(),"CycleOut         End");Step.iSeq = scIdle ;} return false ;
    }
}

bool CPstBuff::CycleIn(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //Step.iCycle = 0;
        //return true ;
    }

    int iRslt0 ;
    int iRslt1 ;
    int iRslt2 ;
    int iRslt3 ;
    int iRslt4 ;
    int iRslt5 ;
    int iRslt6 ;
    int iRslt7 ;
    int iRslt8 ;
    int iRslt9 ;
    int iRsltA ;
    int iRsltB ;
    int iRsltC ;
    int iRsltD ;
    int iRsltE ;
    int iRsltF ;
    int iRsltG ;
    int iRsltH ;
    int iRsltI ;
    int iRsltJ ;
    int iRsltK ;
    int iRsltL ;
    int iTotal ;


    static bool bTrim      = false ;
    static bool bReview    = false ;
    static bool bChipCheck = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : bTrim      = false ;

                  iTotal = 0 ;
                  iRslt0 = DM.ARAY[riPSB].GetCntStat(csRslt0 ) ; iTotal += iRslt0 ;
                  iRslt1 = DM.ARAY[riPSB].GetCntStat(csRslt1 ) ; iTotal += iRslt1 ;
                  iRslt2 = DM.ARAY[riPSB].GetCntStat(csRslt2 ) ; iTotal += iRslt2 ;
                  iRslt3 = DM.ARAY[riPSB].GetCntStat(csRslt3 ) ; iTotal += iRslt3 ;
                  iRslt4 = DM.ARAY[riPSB].GetCntStat(csRslt4 ) ; iTotal += iRslt4 ;
                  iRslt5 = DM.ARAY[riPSB].GetCntStat(csRslt5 ) ; iTotal += iRslt5 ;
                  iRslt6 = DM.ARAY[riPSB].GetCntStat(csRslt6 ) ; iTotal += iRslt6 ;
                  iRslt7 = DM.ARAY[riPSB].GetCntStat(csRslt7 ) ; iTotal += iRslt7 ;
                  iRslt8 = DM.ARAY[riPSB].GetCntStat(csRslt8 ) ; iTotal += iRslt8 ;
                  iRslt9 = DM.ARAY[riPSB].GetCntStat(csRslt9 ) ; iTotal += iRslt9 ;
                  iRsltA = DM.ARAY[riPSB].GetCntStat(csRsltA ) ; iTotal += iRsltA ;
                  iRsltB = DM.ARAY[riPSB].GetCntStat(csRsltB ) ; iTotal += iRsltB ;
                  iRsltC = DM.ARAY[riPSB].GetCntStat(csRsltC ) ; iTotal += iRsltC ;
                  iRsltD = DM.ARAY[riPSB].GetCntStat(csRsltD ) ; iTotal += iRsltD ;
                  iRsltE = DM.ARAY[riPSB].GetCntStat(csRsltE ) ; iTotal += iRsltE ;
                  iRsltF = DM.ARAY[riPSB].GetCntStat(csRsltF ) ; iTotal += iRsltF ;
                  iRsltG = DM.ARAY[riPSB].GetCntStat(csRsltG ) ; iTotal += iRsltG ;
                  iRsltH = DM.ARAY[riPSB].GetCntStat(csRsltH ) ; iTotal += iRsltH ;
                  iRsltI = DM.ARAY[riPSB].GetCntStat(csRsltI ) ; iTotal += iRsltI ;
                  iRsltJ = DM.ARAY[riPSB].GetCntStat(csRsltJ ) ; iTotal += iRsltJ ;
                  iRsltK = DM.ARAY[riPSB].GetCntStat(csRsltK ) ; iTotal += iRsltK ;
                  iRsltL = DM.ARAY[riPSB].GetCntStat(csRsltL ) ; iTotal += iRsltL ;

//2012.09.24 dd
                  bTrim  = DM.ARAY[riPSB].GetCntStat(csRslt0) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt1) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt2) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt3) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt4) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt5) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt6) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt7) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt8) ||
                           DM.ARAY[riPSB].GetCntStat(csRslt9) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltA) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltB) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltC) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltD) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltE) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltF) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltG) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltH) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltI) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltJ) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltK) ||
                           DM.ARAY[riPSB].GetCntStat(csRsltL) ;

                  //토탈 페일 갯수.
                  if( OM.CmnOptn.iCheckFailCnt <= iTotal) {
                      EM_SetErr(eiPSB_TotalFailCnt);
                      bReview = true ;
                  }

                  //같은 에러 갯수.
                  if(OM.CmnOptn.iCheckFailCnt0 && iRslt0 >= OM.CmnOptn.iCheckFailCnt0) {EM_SetErr(eiSameFailCnt0); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt1 && iRslt1 >= OM.CmnOptn.iCheckFailCnt1) {EM_SetErr(eiSameFailCnt1); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt2 && iRslt2 >= OM.CmnOptn.iCheckFailCnt2) {EM_SetErr(eiSameFailCnt2); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt3 && iRslt3 >= OM.CmnOptn.iCheckFailCnt3) {EM_SetErr(eiSameFailCnt3); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt4 && iRslt4 >= OM.CmnOptn.iCheckFailCnt4) {EM_SetErr(eiSameFailCnt4); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt5 && iRslt5 >= OM.CmnOptn.iCheckFailCnt5) {EM_SetErr(eiSameFailCnt5); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt6 && iRslt6 >= OM.CmnOptn.iCheckFailCnt6) {EM_SetErr(eiSameFailCnt6); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt7 && iRslt7 >= OM.CmnOptn.iCheckFailCnt7) {EM_SetErr(eiSameFailCnt7); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt8 && iRslt8 >= OM.CmnOptn.iCheckFailCnt8) {EM_SetErr(eiSameFailCnt8); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCnt9 && iRslt9 >= OM.CmnOptn.iCheckFailCnt9) {EM_SetErr(eiSameFailCnt9); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntA && iRsltA >= OM.CmnOptn.iCheckFailCntA) {EM_SetErr(eiSameFailCntA); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntB && iRsltB >= OM.CmnOptn.iCheckFailCntB) {EM_SetErr(eiSameFailCntB); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntC && iRsltC >= OM.CmnOptn.iCheckFailCntC) {EM_SetErr(eiSameFailCntC); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntD && iRsltD >= OM.CmnOptn.iCheckFailCntD) {EM_SetErr(eiSameFailCntD); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntE && iRsltE >= OM.CmnOptn.iCheckFailCntE) {EM_SetErr(eiSameFailCntE); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntF && iRsltF >= OM.CmnOptn.iCheckFailCntF) {EM_SetErr(eiSameFailCntF); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntG && iRsltG >= OM.CmnOptn.iCheckFailCntG) {EM_SetErr(eiSameFailCntG); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntH && iRsltH >= OM.CmnOptn.iCheckFailCntH) {EM_SetErr(eiSameFailCntH); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntI && iRsltI >= OM.CmnOptn.iCheckFailCntI) {EM_SetErr(eiSameFailCntI); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntJ && iRsltJ >= OM.CmnOptn.iCheckFailCntJ) {EM_SetErr(eiSameFailCntJ); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntK && iRsltK >= OM.CmnOptn.iCheckFailCntK) {EM_SetErr(eiSameFailCntK); bReview = true ;}
                  if(OM.CmnOptn.iCheckFailCntL && iRsltL >= OM.CmnOptn.iCheckFailCntL) {EM_SetErr(eiSameFailCntL); bReview = true ;}
                  MoveActr(aiPSB_FlprOpCl , ccFwd);
                  Step.iCycle++;
                  return false ;


        case 11 : MoveActr(aiPSB_IndxUpDn , ccBwd);
                  if(!MoveActr(aiPSB_FlprOpCl , ccFwd))return false;
                  if(bTrim){
                      Step.iCycle ++;
                      return false;
                  }
                  else {
                      DM.ARAY[riPSB].SetStep(stAlign);
                      Step.iCycle = 0;
                      return true;
                  }

        case 12 : if(!MoveActr(aiPSB_IndxUpDn , ccBwd)) return false ;
                  MoveMotr(miPSB_XIns,piPSB_XInsWait);
                  MoveActr(aiPSB_FlprOpCl , ccBwd);
                  Step.iCycle ++ ;
                  return false ;

        case 13 : if(!MoveMotr(miPSB_XIns,piPSB_XInsWait)) return false ;
                  if(!MoveActr(aiPSB_FlprOpCl , ccBwd)) return false;
                  MoveActr(aiPSB_IndxUpDn , ccFwd);
                  MoveActr(aiPSB_FlprOpCl , ccBwd);
                  Step.iCycle ++ ;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_IndxUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiPSB_FlprOpCl , ccBwd)) return false ;
                  MT_GoAbsRun(miPSB_XIns , GetMotrPos(miPSB_XIns, piPSB_XInsInsert)- PSB_INSERT_DIS );
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MT_GoAbsRun(miPSB_XIns , GetMotrPos(miPSB_XIns, piPSB_XInsInsert)- PSB_INSERT_DIS )) return false ;
                  MT_GoAbs(miPSB_XIns , GetMotrPos(miPSB_XIns, piPSB_XInsInsert) , PSB_INSERT_SPD );
                  Step.iCycle++;
                  return false ;

        case 16 : if(!MT_GoAbs(miPSB_XIns , GetMotrPos(miPSB_XIns,piPSB_XInsInsert) , PSB_INSERT_SPD )) return false ;
                  MoveActr(aiPSB_AlgnFwBw , ccFwd);
                  Step.iCycle ++ ;
                  return false ;

        case 17 : if(!MoveActr(aiPSB_AlgnFwBw , ccFwd)) return false ;
                  MoveActr(aiPSB_FlprOpCl , ccFwd);
                  //MoveActr(aiPSB_AlgnFwBw , ccBwd);
                  m_tmTemp.Clear();
                  Step.iCycle ++ ;
                  return false ;

        case 18 : if(!MoveActr(aiPSB_FlprOpCl , ccFwd)) return false ;
                  if(!m_tmTemp.OnDelay(true , 200))return false;// JS
                  MoveActr(aiPSB_AlgnFwBw , ccBwd);
                  //if(!MoveActr(aiPSB_AlgnFwBw , ccBwd)) return false ;
                  Step.iCycle ++ ;
                  return false ;

        case 19 : if(!MoveActr(aiPSB_AlgnFwBw , ccBwd)) return false ;
                  MoveActr(aiPSB_IndxUpDn , ccBwd);
                  MoveMotr(miPSB_XIns , piPSB_XInsWait); //로스를 줄여 보자. dd

                  m_bDnEdgePSBPkg2 = false;
                  DM.ARAY[riPSB].SetStep(stAlign);

                  Step.iCycle = 0 ;
                  return true ;
    }
}


//One Cycle.
bool CPstBuff::CycleFlip(void) //뒤집기.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    bool r1 , r2 , r3 , r4 ;

    EN_PSTN_ID   piFindPosX , piFindPosY ;
    int          iRjctSrt ;
    EN_CHIP_STAT csFindChip ;
    int          iCol , iRow ;                                                                                               //1,2,3 bin

    FindReject(piFindPosX , piFindPosY , iRjctSrt , csFindChip ) ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : if(!MoveActr(aiPSB_FlprOpCl , ccFwd)) return false ;
                  MoveActr(aiPSB_IndxUpDn , ccBwd         );
                  MoveMotr(miPSB_XIns     , piPSB_XInsWait);

                  MoveActr(aiPSB_AlgnFwBw , ccBwd         );
                  MoveActr(aiPSB_PnchDnUp , ccBwd         );
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_IndxUpDn , ccBwd         )) return false ;
                  if(!MoveMotr(miPSB_XIns     , piPSB_XInsWait)) return false ;

                  if(!MoveActr(aiPSB_AlgnFwBw , ccBwd         )) return false ;
                  if(!MoveActr(aiPSB_PnchDnUp , ccBwd         )) return false ;
                  MoveMotr(miPSB_XTrm , piPSB_XTrmWait);
                  MoveMotr(miPSB_YTrm , piPSB_YTrmWait);

                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSB_XTrm , piPSB_XTrmWait)) return false ;
                  if(!MoveMotr(miPSB_YTrm , piPSB_YTrmWait)) return false ;
                  MoveMotr(miPSB_TFlp , piPSB_TFlpWork);
                  MoveSort(iRjctSrt , true);
                  Step.iCycle++;
                  return false ;

        case 14 : MoveSort(iRjctSrt , false);
                  if(!MoveMotr(miPSB_TFlp     , piPSB_TFlpWork)) return false ;
                  if(!MoveMotr(miPSB_XIns     , piPSB_XInsWait)) return false ;
                  if(!MoveActr(aiPSB_IndxUpDn , ccBwd         )) return false ;
                  if(!MoveSort(iRjctSrt , false)) return false ; // case 19 :밑에서 확인함.dd

                  if(!CheckSort(iRjctSrt)){ // case 10 : 위에서 돌린거 확인.dd
                      EM_SetErr(eiPSB_SortPos);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!IO_GetX(xPSB_RejectBox1)) {return false ; }
                  if(!IO_GetX(xPSB_RejectBox2)) {return false ; }
                  if(!IO_GetX(xPSB_RejectBox3)) {return false ; }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case 15 : if(!m_tmTemp.OnDelay(true , 500)) return false; //JS

                  MoveActr(aiPSB_GripUpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case 16 : if(!MoveActr(aiPSB_GripUpDn , ccFwd))return false ;
                  Step.iCycle = 0;
                  return true ;
    }
}


bool CPstBuff::MoveSort(int _iBinNo , bool _bInit) //_iBinNo = 1,2,3
{
    static int         iSortStep = 0 ;
    static CDelayTimer tmCycle ;

    AnsiString sTemp ;

    if(_bInit ) {//&& iSortStep != 10) {
        iSortStep = 10 ;
        return false ;
    }
//    else return true ;

    if(EM_IsErr()){
        iSortStep=0;
        return true ;
    }


    //Cycle.
    switch (iSortStep) {

        default : if(iSortStep) {
                      sTemp = sTemp.sprintf("MoveSort Default Clear %s iSortStep=%02d" , __FUNC__ , iSortStep );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  iSortStep = 0 ;
                  return true ;

        case 10 :      if(AT_Complete(aiPSB_SrtStprFwBw , ccFwd)) m_iCrntBin = 2 ;
                  else if(AT_Complete(aiPSB_SortFwBw    , ccBwd)) m_iCrntBin = 3 ;
                  else if(AT_Complete(aiPSB_SortFwBw    , ccFwd)) m_iCrntBin = 1 ;

                  if(m_iCrntBin == _iBinNo) {
                      iSortStep = 0 ;
                      return true ;
                  }

                  tmCycle.Clear();
                  iSortStep++;
                  return false ;


                  //소팅 빨리 움직이면 않됌 전에 친 칩이 떨어지는 중.
        case 11 : if(!tmCycle.OnDelay(true , OM.CmnOptn.iPSBSortMoveDelay)) return false ;

                  MoveActr(aiPSB_SrtStprFwBw , ccBwd);
                  MoveActr(aiPSB_SortFwBw    , ccBwd);

                  iSortStep++;
                  return false ;

        case 12 : if(!MoveActr(aiPSB_SrtStprFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_SortFwBw    , ccBwd)) return false ;

                  if(_iBinNo == 3) {
                      return true ;
                      iSortStep = 0 ;
                  }

                  if(_iBinNo == 2) MoveActr(aiPSB_SrtStprFwBw , ccFwd) ;

                  iSortStep++;
                  return false ;

        case 13 : if(_iBinNo == 1 && !MoveActr(aiPSB_SrtStprFwBw , ccBwd)) return false ;
                  if(_iBinNo == 2 && !MoveActr(aiPSB_SrtStprFwBw , ccFwd)) return false ;
                  MoveActr(aiPSB_SortFwBw , ccFwd);
                  iSortStep++;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_SortFwBw , ccFwd)) return false ;
                  iSortStep=0;
                  return true ;
    }
}

bool CPstBuff::CheckSort(int _iBinNo) //_iBinNo = 1,2,3
{
    if(_iBinNo == 3 && !IO_GetX(xPSB_SortBw)) return false ;
    if(_iBinNo == 2 && !IO_GetX(xPSB_SortMd)) return false ;
    if(_iBinNo == 1 && !IO_GetX(xPSB_SortFw)) return false ;

    return true ;
}
bool CPstBuff::FindReject(EN_PSTN_ID &piFindPosX , EN_PSTN_ID &piFindPosY , int &iRjctSrt , EN_CHIP_STAT &csFindChip )
{

//    EN_PSTN_ID   piFindPosX , piFindPosY ;
//    int          iRjctSrt ;
//    EN_CHIP_STAT csFindChip ;
//    int          iCol , iRow ;                                                                                               //1,2,3 bin

    iRjctSrt = 4 ;

    if(OM.CmnOptn.iSort0 < 1||OM.CmnOptn.iSort0 > 3) OM.CmnOptn.iSort0 = 1 ;
    if(OM.CmnOptn.iSort1 < 1||OM.CmnOptn.iSort1 > 3) OM.CmnOptn.iSort1 = 1 ;
    if(OM.CmnOptn.iSort2 < 1||OM.CmnOptn.iSort2 > 3) OM.CmnOptn.iSort2 = 1 ;
    if(OM.CmnOptn.iSort3 < 1||OM.CmnOptn.iSort3 > 3) OM.CmnOptn.iSort3 = 1 ;
    if(OM.CmnOptn.iSort4 < 1||OM.CmnOptn.iSort4 > 3) OM.CmnOptn.iSort4 = 1 ;
    if(OM.CmnOptn.iSort5 < 1||OM.CmnOptn.iSort5 > 3) OM.CmnOptn.iSort5 = 1 ;
    if(OM.CmnOptn.iSort6 < 1||OM.CmnOptn.iSort6 > 3) OM.CmnOptn.iSort6 = 1 ;
    if(OM.CmnOptn.iSort7 < 1||OM.CmnOptn.iSort7 > 3) OM.CmnOptn.iSort7 = 1 ;
    if(OM.CmnOptn.iSort8 < 1||OM.CmnOptn.iSort8 > 3) OM.CmnOptn.iSort8 = 1 ;
    if(OM.CmnOptn.iSort9 < 1||OM.CmnOptn.iSort9 > 3) OM.CmnOptn.iSort9 = 1 ;
    if(OM.CmnOptn.iSortA < 1||OM.CmnOptn.iSortA > 3) OM.CmnOptn.iSortA = 1 ;
    if(OM.CmnOptn.iSortB < 1||OM.CmnOptn.iSortB > 3) OM.CmnOptn.iSortB = 1 ;
    if(OM.CmnOptn.iSortC < 1||OM.CmnOptn.iSortC > 3) OM.CmnOptn.iSortC = 1 ;
    if(OM.CmnOptn.iSortD < 1||OM.CmnOptn.iSortD > 3) OM.CmnOptn.iSortD = 1 ;
    if(OM.CmnOptn.iSortE < 1||OM.CmnOptn.iSortE > 3) OM.CmnOptn.iSortE = 1 ;
    if(OM.CmnOptn.iSortF < 1||OM.CmnOptn.iSortF > 3) OM.CmnOptn.iSortF = 1 ;
    if(OM.CmnOptn.iSortG < 1||OM.CmnOptn.iSortG > 3) OM.CmnOptn.iSortG = 1 ;
    if(OM.CmnOptn.iSortH < 1||OM.CmnOptn.iSortH > 3) OM.CmnOptn.iSortH = 1 ;
    if(OM.CmnOptn.iSortI < 1||OM.CmnOptn.iSortI > 3) OM.CmnOptn.iSortI = 1 ;
    if(OM.CmnOptn.iSortJ < 1||OM.CmnOptn.iSortJ > 3) OM.CmnOptn.iSortJ = 1 ;
    if(OM.CmnOptn.iSortK < 1||OM.CmnOptn.iSortK > 3) OM.CmnOptn.iSortK = 1 ;
    if(OM.CmnOptn.iSortL < 1||OM.CmnOptn.iSortL > 3) OM.CmnOptn.iSortL = 1 ;


    for(int i = 1 ; i <= 3 ; i++) {


    }


    if(!OM.CmnOptn.bIgnrEmptyTrm      &&  //밑에 순서 주의 0,1번  1번이 엠티라 서 그럼.
       iRjctSrt > OM.CmnOptn.iSort1 && DM.ARAY[riPSB].GetCntStat(csRslt1)) {piFindPosX = piPSB_XTrmRslt1 ; piFindPosY = piPSB_YTrmRslt1 ; iRjctSrt = OM.CmnOptn.iSort1; csFindChip = csRslt1 ;}
    if(iRjctSrt > OM.CmnOptn.iSort0 && DM.ARAY[riPSB].GetCntStat(csRslt0)) {piFindPosX = piPSB_XTrmRslt0 ; piFindPosY = piPSB_YTrmRslt0 ; iRjctSrt = OM.CmnOptn.iSort0; csFindChip = csRslt0 ;}
    if(iRjctSrt > OM.CmnOptn.iSort2 && DM.ARAY[riPSB].GetCntStat(csRslt2)) {piFindPosX = piPSB_XTrmRslt2 ; piFindPosY = piPSB_YTrmRslt2 ; iRjctSrt = OM.CmnOptn.iSort2; csFindChip = csRslt2 ;}
    if(iRjctSrt > OM.CmnOptn.iSort3 && DM.ARAY[riPSB].GetCntStat(csRslt3)) {piFindPosX = piPSB_XTrmRslt3 ; piFindPosY = piPSB_YTrmRslt3 ; iRjctSrt = OM.CmnOptn.iSort3; csFindChip = csRslt3 ;}
    if(iRjctSrt > OM.CmnOptn.iSort4 && DM.ARAY[riPSB].GetCntStat(csRslt4)) {piFindPosX = piPSB_XTrmRslt4 ; piFindPosY = piPSB_YTrmRslt4 ; iRjctSrt = OM.CmnOptn.iSort4; csFindChip = csRslt4 ;}
    if(iRjctSrt > OM.CmnOptn.iSort5 && DM.ARAY[riPSB].GetCntStat(csRslt5)) {piFindPosX = piPSB_XTrmRslt5 ; piFindPosY = piPSB_YTrmRslt5 ; iRjctSrt = OM.CmnOptn.iSort5; csFindChip = csRslt5 ;}
    if(iRjctSrt > OM.CmnOptn.iSort6 && DM.ARAY[riPSB].GetCntStat(csRslt6)) {piFindPosX = piPSB_XTrmRslt6 ; piFindPosY = piPSB_YTrmRslt6 ; iRjctSrt = OM.CmnOptn.iSort6; csFindChip = csRslt6 ;}
    if(iRjctSrt > OM.CmnOptn.iSort7 && DM.ARAY[riPSB].GetCntStat(csRslt7)) {piFindPosX = piPSB_XTrmRslt7 ; piFindPosY = piPSB_YTrmRslt7 ; iRjctSrt = OM.CmnOptn.iSort7; csFindChip = csRslt7 ;}
    if(iRjctSrt > OM.CmnOptn.iSort8 && DM.ARAY[riPSB].GetCntStat(csRslt8)) {piFindPosX = piPSB_XTrmRslt8 ; piFindPosY = piPSB_YTrmRslt8 ; iRjctSrt = OM.CmnOptn.iSort8; csFindChip = csRslt8 ;}
    if(iRjctSrt > OM.CmnOptn.iSort9 && DM.ARAY[riPSB].GetCntStat(csRslt9)) {piFindPosX = piPSB_XTrmRslt9 ; piFindPosY = piPSB_YTrmRslt9 ; iRjctSrt = OM.CmnOptn.iSort9; csFindChip = csRslt9 ;}
    if(iRjctSrt > OM.CmnOptn.iSortA && DM.ARAY[riPSB].GetCntStat(csRsltA)) {piFindPosX = piPSB_XTrmRsltA ; piFindPosY = piPSB_YTrmRsltA ; iRjctSrt = OM.CmnOptn.iSortA; csFindChip = csRsltA ;}
    if(iRjctSrt > OM.CmnOptn.iSortB && DM.ARAY[riPSB].GetCntStat(csRsltB)) {piFindPosX = piPSB_XTrmRsltB ; piFindPosY = piPSB_YTrmRsltB ; iRjctSrt = OM.CmnOptn.iSortB; csFindChip = csRsltB ;}
    if(iRjctSrt > OM.CmnOptn.iSortC && DM.ARAY[riPSB].GetCntStat(csRsltC)) {piFindPosX = piPSB_XTrmRsltC ; piFindPosY = piPSB_YTrmRsltC ; iRjctSrt = OM.CmnOptn.iSortC; csFindChip = csRsltC ;}
    if(iRjctSrt > OM.CmnOptn.iSortD && DM.ARAY[riPSB].GetCntStat(csRsltD)) {piFindPosX = piPSB_XTrmRsltD ; piFindPosY = piPSB_YTrmRsltD ; iRjctSrt = OM.CmnOptn.iSortD; csFindChip = csRsltD ;}
    if(iRjctSrt > OM.CmnOptn.iSortE && DM.ARAY[riPSB].GetCntStat(csRsltE)) {piFindPosX = piPSB_XTrmRsltE ; piFindPosY = piPSB_YTrmRsltE ; iRjctSrt = OM.CmnOptn.iSortE; csFindChip = csRsltE ;}
    if(iRjctSrt > OM.CmnOptn.iSortF && DM.ARAY[riPSB].GetCntStat(csRsltF)) {piFindPosX = piPSB_XTrmRsltF ; piFindPosY = piPSB_YTrmRsltF ; iRjctSrt = OM.CmnOptn.iSortF; csFindChip = csRsltF ;}
    if(iRjctSrt > OM.CmnOptn.iSortG && DM.ARAY[riPSB].GetCntStat(csRsltG)) {piFindPosX = piPSB_XTrmRsltG ; piFindPosY = piPSB_YTrmRsltG ; iRjctSrt = OM.CmnOptn.iSortG; csFindChip = csRsltG ;}
    if(iRjctSrt > OM.CmnOptn.iSortH && DM.ARAY[riPSB].GetCntStat(csRsltH)) {piFindPosX = piPSB_XTrmRsltH ; piFindPosY = piPSB_YTrmRsltH ; iRjctSrt = OM.CmnOptn.iSortH; csFindChip = csRsltH ;}
    if(iRjctSrt > OM.CmnOptn.iSortI && DM.ARAY[riPSB].GetCntStat(csRsltI)) {piFindPosX = piPSB_XTrmRsltI ; piFindPosY = piPSB_YTrmRsltI ; iRjctSrt = OM.CmnOptn.iSortI; csFindChip = csRsltI ;}
    if(iRjctSrt > OM.CmnOptn.iSortJ && DM.ARAY[riPSB].GetCntStat(csRsltJ)) {piFindPosX = piPSB_XTrmRsltJ ; piFindPosY = piPSB_YTrmRsltJ ; iRjctSrt = OM.CmnOptn.iSortJ; csFindChip = csRsltJ ;}
    if(iRjctSrt > OM.CmnOptn.iSortK && DM.ARAY[riPSB].GetCntStat(csRsltK)) {piFindPosX = piPSB_XTrmRsltK ; piFindPosY = piPSB_YTrmRsltK ; iRjctSrt = OM.CmnOptn.iSortK; csFindChip = csRsltK ;}
    if(iRjctSrt > OM.CmnOptn.iSortL && DM.ARAY[riPSB].GetCntStat(csRsltL)) {piFindPosX = piPSB_XTrmRsltL ; piFindPosY = piPSB_YTrmRsltL ; iRjctSrt = OM.CmnOptn.iSortL; csFindChip = csRsltL ;}

    if(iRjctSrt == 4) {Step.iCycle = 0; return false  ;}
    return true ;
}
//One Cycle.
bool CPstBuff::CycleWork(void) //Marking
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 15000 )) {
        if(Step.iCycle == 13 && !IO_GetX(xPSB_RejectBox1)){EM_SetErr(eiPSB_NoRjBox1); Step.iCycle = 0 ; return true ;}
        if(Step.iCycle == 13 && !IO_GetX(xPSB_RejectBox2)){EM_SetErr(eiPSB_NoRjBox2); Step.iCycle = 0 ; return true ;}
        if(Step.iCycle == 13 && !IO_GetX(xPSB_RejectBox3)){EM_SetErr(eiPSB_NoRjBox3); Step.iCycle = 0 ; return true ;}

        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //Step.iCycle = 0;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;


    EN_PSTN_ID   piFindPosX , piFindPosY ;
    int          iRjctSrt ;
    EN_CHIP_STAT csFindChip ;
    int          iCol , iRow ;                                                                                               //1,2,3 bin
    int    iFlipC ;
    int    iFlipR ;


    FindReject(piFindPosX , piFindPosY , iRjctSrt , csFindChip ) ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_PnchDnUp , ccBwd);
                  MoveActr(aiPSB_GripUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;


        case 11 : if(!MoveActr(aiPSB_PnchDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_GripUpDn , ccFwd)) return false ;
                  MoveMotr(miPSB_XTrm , piFindPosX);
                  MoveMotr(miPSB_YTrm , piFindPosY);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSB_XTrm , piFindPosX)) return false ;
                  if(!MoveMotr(miPSB_YTrm , piFindPosY)) return false ;
                  MoveSort(iRjctSrt , true); //Init dd
                  Step.iCycle++;
                  return false ;

        case 13 :
                  Step.iCycle++;
                  return false ;

        //스텝 바꿀때 주의 위에서 씀.
        case 14 : if(!MoveSort(iRjctSrt , false)) return false ; // Step go dd

                  if(!CheckSort(iRjctSrt)){ //소팅 위치 센서 에러..
                      EM_SetErr(eiPSB_SortPos);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!IO_GetX(xPSB_RejectBox1)) {return false ; }
                  if(!IO_GetX(xPSB_RejectBox2)) {return false ; }
                  if(!IO_GetX(xPSB_RejectBox3)) {return false ; }

                  MoveActr(aiPSB_PnchDnUp , ccFwd);

                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiPSB_PnchDnUp , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case 16 : if(!m_tmTemp.OnDelay(true , OM.DevOptn.iPunchingDelay)) return false;
                  MoveActr(aiPSB_PnchDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(!MoveActr(aiPSB_PnchDnUp , ccBwd)) return false ;
                  FindChip(riPSB , csFindChip , iRow , iCol) ;

                  DM.ARAY[riPSB].SetStat(iRow,iCol,csEmpty) ;
                  Step.iCycle=0;
                  return true ;
    }
}

bool CPstBuff::CycleOut(void) //자제 아웃.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //Step.iCycle = 0 ;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_IndxUpDn , ccBwd);
                  MoveActr(aiPSB_PnchDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_IndxUpDn , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_PnchDnUp , ccBwd)) return false ;
                  MoveMotr(miPSB_XTrm     , piPSB_XTrmWait );
                  MoveMotr(miPSB_YTrm     , piPSB_YTrmWait );
                  MoveMotr(miPSB_XIns     , piPSB_XInsWait );

                  MoveActr(aiPSB_AlgnFwBw , ccBwd          );
                  MoveActr(aiPSB_PshrFwBw , ccBwd          );
                  MoveActr(aiPSB_PshrDnUp , ccBwd          );

                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSB_XTrm  , piPSB_XTrmWait )) return false ;
                  if(!MoveMotr(miPSB_YTrm  , piPSB_YTrmWait )) return false ;
                  if(!MoveMotr(miPSB_XIns  , piPSB_XInsWait )) return false ;
                  if(DM.ARAY[riPSB].GetCntStat(csEmpty)) MoveActr(aiPSB_FlprOpCl , ccFwd);
                  MoveActr(aiPSB_GripUpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(DM.ARAY[riPSB].GetCntStat(csEmpty) && !MoveActr(aiPSB_FlprOpCl , ccFwd)) return false;
                  if(!MoveActr(aiPSB_AlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_PshrFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_PshrDnUp , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_GripUpDn , ccBwd)) return false ;

                  MoveActr(aiPSB_IndxUpDn , ccFwd         ); //dd
                  MoveMotr(miPSB_TFlp     , piPSB_TFlpWait);

                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_IndxUpDn , ccFwd         )) return false ;
                  if(!MoveMotr(miPSB_TFlp     , piPSB_TFlpWait)) return false ;
                  MoveActr(aiPSB_FlprOpCl , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveActr(aiPSB_FlprOpCl ,ccBwd)) return false ;
                  MoveMotr(miPSB_XIns , piPSB_XInsOut);

                  Step.iCycle++;
                  return false ;

        case 16 : if(IO_GetX(xPSB_InsertOverload)) {
                      EM_SetErr( eiPSB_InsertOverload );
                      MT_Stop(miPSB_XIns);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!MoveMotr(miPSB_XIns , piPSB_XInsOut)) return false ;
                  MoveActr(aiPSB_IndxUpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(!MoveActr(aiPSB_IndxUpDn , ccBwd)) return false ;
                  MoveMotr(miPSB_XIns         , piPSB_XInsWait) ;
                  MoveActr(aiPSB_PnchDnUp     , ccBwd         ) ;
                  Step.iCycle++;
                  return false ;

        case 18 : if(!MoveActr(aiPSB_PnchDnUp , ccBwd )) return false ;

                  MoveActr(aiPSB_PshrDnUp , ccFwd  ) ;

                  Step.iCycle++;
                  return false ;

        case 19 : if(!MoveActr(aiPSB_PshrDnUp , ccFwd  ))return false;
                  MoveActr(aiPSB_PshrFwBw , ccFwd  ) ;
                  Step.iCycle++;
                  return false ;

        case 20 : if(!MoveActr(aiPSB_PshrFwBw , ccFwd )) return false ;

                  if( IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4)) {
                      EM_SetErr(eiPSB_PusherMiss);
                  }
                  //스토퍼 다운되면 ...데이터 넘김.
/*
                  if(OM.CmnOptn.bUseTrimMachine) {
                      DM.ShiftArrayData(riPSB,riULR);
                      DM.ARAY[riULR].SetStep(asRailIn);
//                      EC.WriteArayData(riPSB); //다음장비용 데이터.
                  }
                  else {
                      //if(DM.ARAY[riULT].CheckAllStat(csEmpty)&&!DM.ARAY[riULB].GetCntStat(csEmpty))


                      if(DM.ARAY[riULB].GetCntStat(csEmpty)) {
                          DM.ARAY[riULB].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                          DM.ARAY[riULB].SetID(DM.ARAY[riPSB].GetID());
                          DM.ARAY[riULB].SetStat(DM.ARAY[riULB].FindLastRow(csEmpty),0 , csWork);
                      }
                      else if(DM.ARAY[riULT].GetCntStat(csEmpty)) {
                          DM.ARAY[riULT].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                          DM.ARAY[riULT].SetID(DM.ARAY[riPSB].GetID());
                          DM.ARAY[riULT].SetStat(DM.ARAY[riULT].FindLastRow(csEmpty),0 , csWork);
                      }
                  }
*/
                  //레일에서 PSB로 넘어올때 담아 놓은거 여기서 남김. 포스트버퍼에서 빼낸 자제는 데이터 남기지 않게.
                  //TODO :: 이거 250은 SPC로 바꿧는데 어레이 넘겨서 일단 LT 그대로 둠...몬가 안될수도...dd
                  LT.WriteArayData(&DM.ARAY[riLot]);
                  LotInfoAdd (&DM.ARAY[riLot]);

                  VC.WriteArayVisnFormat(&DM.ARAY[riLot]);

                  if(GetLastStrip()) {
                      m_bLotEnded = true ;
//                      if(OM.CmnOptn.bUseTrimMachine) EC.SendPstEqp(psLotEnd);
                  }

                  m_sWorkedId    = DM.ARAY[riPSB].GetID();
                  m_sWorkedLotNo = DM.ARAY[riPSB].GetLotNo();

                  DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riULD].SetID(DM.ARAY[riPSB].GetID());
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);
                  DM.ARAY[riPSB].SetStat(csNone) ;
                  Step.iCycle++;
                  return false ;

/*
                  if(DM.ARAY[riULT].CheckAllStat(csEmpty)&&!DM.ARAY[riULB].GetCntStat(csEmpty)) DM.ARAY[riULT].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riPSB].FlipY();
                  DM.ARAY[riPSB].SetStat(csNone);


                  //m_bLotEnded = true ;

                       if(DM.ARAY[riULB].GetCntStat(csEmpty)) DM.ARAY[riULB].SetStat(DM.ARAY[riULB].FindLastRow(csEmpty),0 , csWork);
                  else if(DM.ARAY[riULT].GetCntStat(csEmpty)) DM.ARAY[riULT].SetStat(DM.ARAY[riULT].FindLastRow(csEmpty),0 , csWork);
//
//                  LT.AddDayInfoWorkStrp(1);
//                  LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

//                  LT.AddLotInfoWorkStrp(1);
//                  LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
//
                  MoveActr(aiPSB_PusherFB, ccBwd );
                  Step.iCycle++;
                  return false ;
*/
        case 21 : if(!MoveActr(aiPSB_PshrFwBw,ccBwd)) return false ;
                  MoveActr(aiPSB_PshrDnUp, ccBwd );

                  Step.iCycle++;
                  return false ;

        case 22 : if(!MoveActr(aiPSB_PshrDnUp, ccBwd )) return false ;
                  if(!MoveMotr(miPSB_XIns, piPSB_XInsWait)) return false ;
                  Step.iCycle++;
                  return false;
        case 23 :
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CPstBuff::LotInfoAdd(CArray * Array)
{
//    Array.ChangeStat(csUnkwn,csWork);
    LT.AddEqpInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);
    LT.AddEqpInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) );//- Array->GetCntStat(csRslt1) );

    LT.AddDayInfoWorkStrp(1);
    LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddLotInfoWorkStrp(1);
    LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

//    if(OM.MstOptn.bEmptyNoTrm) {
//        LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) - Array->GetCntStat(csRslt1) );
//        LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) - Array->GetCntStat(csRslt1) );
//        OM.iTotalChip     += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;
//        OM.iTotalFailChip += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) - Array->GetCntStat(csRslt1) ;
//    }
//    else {
        LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) );//- Array->GetCntStat(csRslt1));
        LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) );//- Array->GetCntStat(csRslt1));
        OM.iTotalChip     += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;
        OM.iTotalFailChip += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - Array->GetCntStat(csWork) - Array->GetCntStat(csRslt1) ;
//    }

    if(OM.iTotalChip < 0 || OM.iTotalFailChip < 0) {//OverFlow
        OM.iTotalChip     = 0 ;
        OM.iTotalFailChip = 0 ;
    }

    LT.AddDayInfoChips(Array->GetCntStat(csRslt0 ) , //
                       Array->GetCntStat(csRslt1 ) , //
                       Array->GetCntStat(csRslt2 ) , //
                       Array->GetCntStat(csRslt3 ) , //
                       Array->GetCntStat(csRslt4 ) , //
                       Array->GetCntStat(csRslt5 ) , //
                       Array->GetCntStat(csRslt6 ) , //
                       Array->GetCntStat(csRslt7 ) , //
                       Array->GetCntStat(csRslt8 ) , //
                       Array->GetCntStat(csRslt9 ) , //
                       Array->GetCntStat(csRsltA ) , //
                       Array->GetCntStat(csRsltB ) , //
                       Array->GetCntStat(csRsltC ) , //
                       Array->GetCntStat(csRsltD ) , //
                       Array->GetCntStat(csRsltE ) , //
                       Array->GetCntStat(csRsltF ) , //
                       Array->GetCntStat(csRsltG ) , //
                       Array->GetCntStat(csRsltH ) , //
                       Array->GetCntStat(csRsltI ) , //
                       Array->GetCntStat(csRsltJ ) , //
                       Array->GetCntStat(csRsltK ) , //
                       Array->GetCntStat(csRsltL )); //

int icsRslt0 = Array->GetCntStat(csRslt0 )  ;
int icsRslt1 = Array->GetCntStat(csRslt1 )  ;
int icsRslt2 = Array->GetCntStat(csRslt2 )  ;
int icsRslt3 = Array->GetCntStat(csRslt3 )  ;
int icsRslt4 = Array->GetCntStat(csRslt4 )  ;
int icsRslt5 = Array->GetCntStat(csRslt5 )  ;
int icsRslt6 = Array->GetCntStat(csRslt6 )  ;
int icsRslt7 = Array->GetCntStat(csRslt7 )  ;
int icsRslt8 = Array->GetCntStat(csRslt8 )  ;
int icsRslt9 = Array->GetCntStat(csRslt9 )  ;
int icsRsltA = Array->GetCntStat(csRsltA )  ;
int icsRsltB = Array->GetCntStat(csRsltB )  ;
int icsRsltC = Array->GetCntStat(csRsltC )  ;
int icsRsltD = Array->GetCntStat(csRsltD )  ;
int icsRsltE = Array->GetCntStat(csRsltE )  ;
int icsRsltF = Array->GetCntStat(csRsltF )  ;
int icsRsltG = Array->GetCntStat(csRsltG )  ;
int icsRsltH = Array->GetCntStat(csRsltH )  ;
int icsRsltI = Array->GetCntStat(csRsltI )  ;
int icsRsltJ = Array->GetCntStat(csRsltJ )  ;
int icsRsltK = Array->GetCntStat(csRsltK )  ;
int icsRsltL = Array->GetCntStat(csRsltL )  ;


    LT.AddLotInfoChips(Array->GetCntStat(csRslt0 ) , //
                       Array->GetCntStat(csRslt1 ) , //
                       Array->GetCntStat(csRslt2 ) , //
                       Array->GetCntStat(csRslt3 ) , //
                       Array->GetCntStat(csRslt4 ) , //
                       Array->GetCntStat(csRslt5 ) , //
                       Array->GetCntStat(csRslt6 ) , //
                       Array->GetCntStat(csRslt7 ) , //
                       Array->GetCntStat(csRslt8 ) , //
                       Array->GetCntStat(csRslt9 ) , //
                       Array->GetCntStat(csRsltA ) , //
                       Array->GetCntStat(csRsltB ) , //
                       Array->GetCntStat(csRsltC ) , //
                       Array->GetCntStat(csRsltD ) , //
                       Array->GetCntStat(csRsltE ) , //
                       Array->GetCntStat(csRsltF ) , //
                       Array->GetCntStat(csRsltG ) , //
                       Array->GetCntStat(csRsltH ) , //
                       Array->GetCntStat(csRsltI ) , //
                       Array->GetCntStat(csRsltJ ) , //
                       Array->GetCntStat(csRsltK ) , //
                       Array->GetCntStat(csRsltL )); //

    if(GetLastStrip()) {
        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",Array->GetLotNo().c_str());
        LT.LotInfo.dEndTime = Now();
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        Trace("WriteLotLog",Array->GetLotNo().c_str());
        //LT.LotEnd();
    }
    Array->SetStat(csNone) ;
}

bool CPstBuff::GetLastStrip()
{
    AnsiString sLastLotNo = DM.ARAY[riPSB].GetCntExist() ? DM.ARAY[riPSB].GetLotNo() : m_sWorkedLotNo ;

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
    bool bWR3Exist = !DM.ARAY[riWR3].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
    bool bWR1Diff  = sLastLotNo != DM.ARAY[riWR1].GetLotNo() ;
    bool bWR2Diff  = sLastLotNo != DM.ARAY[riWR2].GetLotNo() ;
    bool bWR3Diff  = sLastLotNo != DM.ARAY[riWR3].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if( bWR3Exist) return bWR3Diff ;
    else if( bWR2Exist) return bWR2Diff ;
    else if( bWR1Exist) return bWR1Diff ;
    else if( bPRBExist) return bPRBDiff ;
    else if( bLDRExist) return bLDRDiff ;
    else if(!bLastMgz ) return false    ;
    else                return true     ;
}


void CPstBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPstBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPstBuff::CheckStop()
{
    if(!AT_Done(aiPSB_AlgnFwBw   )) return false ;
    if(!AT_Done(aiPSB_IndxUpDn   )) return false ;
    if(!AT_Done(aiPSB_FlprOpCl   )) return false ;
    if(!AT_Done(aiPSB_GripUpDn   )) return false ;
    if(!AT_Done(aiPSB_PnchDnUp   )) return false ;
    if(!AT_Done(aiPSB_SortFwBw   )) return false ;
    if(!AT_Done(aiPSB_SrtStprFwBw)) return false ;
    if(!AT_Done(aiPSB_PshrFwBw   )) return false ;
    if(!AT_Done(aiPSB_PshrDnUp   )) return false ;
    
    if(!MT_GetStop(miPSB_XTrm)) return false ;
    if(!MT_GetStop(miPSB_YTrm)) return false ;
    if(!MT_GetStop(miPSB_TFlp)) return false ;
    if(!MT_GetStop(miPSB_XIns)) return false ;

    return true ;
}

void CPstBuff::Load(bool IsLoad )
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded   " , &m_bLotEnded     );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" , &m_sWorkedLotNo  );
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , &m_sWorkedId     );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded   " ,  m_bLotEnded     );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedLotNo" ,  m_sWorkedLotNo  );
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " ,  m_sWorkedId     );
    }
}

















