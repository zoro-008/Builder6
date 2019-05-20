//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Actuators.h"
#include "FormMsgOk.h"
#include "UserIni.h"
#include "SLogUnit.h"
//#include "FormDllMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//Actuator Object.
CActuators AT;

CActuator::CActuator(int _iActrNo)
{
    m_iActrNo = _iActrNo ;
    m_bSetCntrlPn = false ;

    m_dtFwdTimeOut.Clear();
    m_dtBwdTimeOut.Clear();
    m_dtFwdOnDelay.Clear();
    m_dtBwdOnDelay.Clear();

    memset(&Para , 0 , sizeof(SPara ));
    memset(&Stat , 0 , sizeof(SStat ));
    for(int i = 0 ; i < MAX_X_CNT ; i++) {
        Para.iFwdXAdd[i] = -1 ;
        Para.iBwdXAdd[i] = -1 ;
    }

    Para.iFwdYAdd = -1 ;
    Para.iBwdYAdd = -1 ;

    Para.iFwdTimeOut = 4000 ;
    Para.iBwdTimeOut = 4000 ;

    pLtimg = NULL ;
    pRtimg = NULL ;
    pUpimg = NULL ;
    pDnimg = NULL ;
    pAwimg = NULL ;
    pCwimg = NULL ;

    m_pStCaption = NULL ;
    m_pBtnBw     = NULL ;
    m_pBtnFw     = NULL ;

    //Load();

}

CActuator::~CActuator()
{
   //new로 생성 했으면 자동 삭제 됌.

}
void CActuator::Init ()
{
    //Set exist ID.
    bool bExistFwdX = (Para.iFwdXAdd[0] > -1) ;
    bool bExistBwdX = (Para.iBwdXAdd[0] > -1) ;
    bool bExistFwdY = (Para.iFwdYAdd    > -1) ;
    bool bExistBwdY = (Para.iBwdYAdd    > -1) ;

    bool bFwdX , bVtrlFwdX ;
    bool bBwdX , bVtrlBwdX ;
    bool bFwdY , bVtrlFwdY ;
    bool bBwdY , bVtrlBwdY ;

    //I/O.
    if (bExistFwdX) bFwdX = GetFwdX(true); //IO.GetX(Para.iFwdXAdd,true); //Detect X/Y Val.
    if (bExistBwdX) bBwdX = GetBwdX(true); //IO.GetX(Para.iBwdXAdd,true);
    if (bExistFwdY) bFwdY = IO.GetY(Para.iFwdYAdd,true);
    if (bExistBwdY) bBwdY = IO.GetY(Para.iBwdYAdd,true);

    //Virtual Input Sensor
    if      (bExistFwdX) bVtrlFwdX =  bFwdX ;
    else if (bExistBwdX) bVtrlFwdX = !bBwdX ;
    else if (bExistFwdY) bVtrlFwdX =  bFwdY ;
    else if (bExistBwdY) bVtrlFwdX = !bBwdY ;
    else                 return             ;

    if      (bExistBwdX) bVtrlBwdX =  bBwdX ;
    else if (bExistFwdX) bVtrlBwdX = !bFwdX ;
    else if (bExistBwdY) bVtrlBwdX =  bBwdY ;
    else if (bExistFwdY) bVtrlBwdX = !bFwdY ;
    else                 return             ;

    if(bVtrlFwdX) Stat.bCmdPos = ccFwd ;
    if(bVtrlBwdX) Stat.bCmdPos = ccBwd ;
}

bool CActuator::GetFwdX(bool _bDirct)
{
    for(int i = 0 ; i < MAX_X_CNT ; i++) {
        if(Para.iFwdXAdd[i] != 0 && Para.iFwdXAdd[i] != -1){
           if(!IO.GetX(Para.iFwdXAdd[i],_bDirct)) return false ;
        }
    }
    return true ;
}
bool CActuator::GetBwdX(bool _bDirct)
{
    for(int i = 0 ; i < MAX_X_CNT ; i++) {
        if(Para.iBwdXAdd[i] != 0 && Para.iBwdXAdd[i] != -1 && !IO.GetX(Para.iBwdXAdd[i],_bDirct)) return false ;
    }
    return true ;
}

void CActuator::Update ()
{
    //Local Var.
    bool bExistFwdX ;
    bool bExistBwdX ;
    bool bExistFwdY ;
    bool bExistBwdY ;
    int  iFwdX      ;
    int  iBwdX      ;
    int  iFwdY      ;
    int  iBwdY      ;
    bool bFwdX      ;
    bool bBwdX      ;
    bool bFwdY      ;
    bool bBwdY      ;

    bool bVtrlFwdX  ;
    bool bVtrlBwdX  ;
    bool bVtrlFwdY  ;
    bool bVtrlBwdY  ;

    bool bDelayedFwd ;
    bool bDelayedBwd ;

    //Set I/O ID.
    iFwdX = Para.iFwdXAdd[0] ;
    iBwdX = Para.iBwdXAdd[0] ;
    iFwdY = Para.iFwdYAdd ;
    iBwdY = Para.iBwdYAdd ;

    //Check SKIP.
    if (Para.iFwdYAdd < 0 && Para.iBwdYAdd < 0) {
        Reset();
        return ;
    }

    //Set exist ID.
    bExistFwdX = (iFwdX > -1) ;
    bExistBwdX = (iBwdX > -1) ;
    bExistFwdY = (iFwdY > -1) ;
    bExistBwdY = (iBwdY > -1) ;

    //I/O.
    if (bExistFwdX) bFwdX = GetFwdX();//IO.GetX(iFwdX); //Detect X/Y Val.
    if (bExistBwdX) bBwdX = GetBwdX();//IO.GetX(iBwdX);
    if (bExistFwdY) bFwdY = IO.GetY(iFwdY);
    if (bExistBwdY) bBwdY = IO.GetY(iBwdY);

    //Virtual Input Sensor
    if      (bExistFwdX) bVtrlFwdX =  bFwdX ;
    else if (bExistBwdX) bVtrlFwdX = !bBwdX ;
    else if (bExistFwdY) bVtrlFwdX =  bFwdY ;
    else if (bExistBwdY) bVtrlFwdX = !bBwdY ;
    else                 return             ;

    if      (bExistBwdX) bVtrlBwdX =  bBwdX ;
    else if (bExistFwdX) bVtrlBwdX = !bFwdX ;
    else if (bExistBwdY) bVtrlBwdX =  bBwdY ;
    else if (bExistFwdY) bVtrlBwdX = !bFwdY ;
    else                 return             ;

    //Virtual Output Sensor
    if (bExistFwdY) bVtrlFwdY =  bFwdY;
    else            bVtrlFwdY = !bBwdY;
    if (bExistBwdY) bVtrlBwdY =  bBwdY;
    else            bVtrlBwdY = !bFwdY;


    bDelayedFwd = m_dtFwdOnDelay.OnDelay(bVtrlFwdX , Para.iFwdOnDelay );
    bDelayedBwd = m_dtBwdOnDelay.OnDelay(bVtrlBwdX , Para.iBwdOnDelay );

    if(bDelayedFwd)Stat.bActPos = ccFwd ;
    if(bDelayedBwd)Stat.bActPos = ccBwd ;

    //OnDelay Timer.
    m_dtFwdTimeOut.OnDelay(Stat.bCmdPos == ccFwd && Stat.bActPos != ccFwd , Para.iFwdTimeOut);
    m_dtBwdTimeOut.OnDelay(Stat.bCmdPos == ccBwd && Stat.bActPos != ccBwd , Para.iBwdTimeOut);

    Stat.bErr = m_dtFwdTimeOut.m_bOut || m_dtBwdTimeOut.m_bOut ;

    //if(m_pBtnBw) m_pBtnBw -> Font -> Color = Complete(ccBwd) ? clLime : clBlack ;
    //if(m_pBtnFw) m_pBtnFw -> Font -> Color = Complete(ccFwd) ? clLime : clBlack ;
}

void CActuator::Reset()
{
    m_dtFwdTimeOut.Clear();
    m_dtBwdTimeOut.Clear();

    //처음에 공압이 공급 안되고 프로그램 키고 공압 넣으면
    //에러 떠서 클리어 안되는 문제..해결때문에 넣음
    //그래도 공압 넣고 리셑 버튼 눌러야함.
    Init();
}

AnsiString CActuator::GetName (void)
{
    return Para.sName ;
}

bool CActuator::Complete(bool _bCmd)
{
    if(Stat.bCmdPos != Stat.bActPos) return false ;
    return Stat.bCmdPos == _bCmd ;
}

bool CActuator::Complete()
{
    if(Stat.bCmdPos != Stat.bActPos) return false ;
    return true ;
}

bool CActuator::GetCmd(void)
{
    return Stat.bCmdPos ;
}

void CActuator::SetAct(bool _bAct)
{
    Stat.bActPos = _bAct ;
}

bool CActuator::GetAct(void)
{
    return Stat.bActPos ;
}

bool CActuator::Err(void)
{
    return Stat.bErr ;
}

bool CActuator::Move(bool _bCmd)
{
    if(Stat.bCmdPos != _bCmd){
        Trace( "Cylinder", (Para.sEnum + "(" + Para.sName + ")_" + String(_bCmd ? "FWD" : "BWD") ).c_str());
    }

    Stat.bCmdPos = _bCmd ;

    if( _bCmd == ccFwd) {
        if(Para.iFwdYAdd>=0) IO.SetY(Para.iFwdYAdd , true );
        if(Para.iBwdYAdd>=0) IO.SetY(Para.iBwdYAdd , false);
    }
    else {
        if(Para.iFwdYAdd>=0) IO.SetY(Para.iFwdYAdd , false);
        if(Para.iBwdYAdd>=0) IO.SetY(Para.iBwdYAdd , true );
    }

    return Complete(_bCmd);
}

CActuator::SPara * CActuator::GetPara()
{
    //memcpy(&Para , &_Para , sizeof(SPara));
    return &Para ;
}

void CActuator::Load(EN_LAN_SEL _iLang)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sItem;
    AnsiString sPath;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Actuator.INI";

    if(!FileExists(sPath)) return ;

    AnsiString sSelLan ;
    switch(_iLang) {
        default         : sSelLan = "E_" ; break ;
        case lsKorean   : sSelLan = "K_" ; break ;
        case lsEnglish  : sSelLan = "E_" ; break ;
        case lsChinese  : sSelLan = "C_" ; break ;
    }

    AnsiString sTemp ;
    //Load.
    sItem.printf("ACTUATOR(%03d)" , m_iActrNo);

    UserINI.Load(sPath.c_str() ,          "sEnum      " , sItem.c_str() , Para.sEnum      );
    UserINI.Load(sPath.c_str() , sSelLan +"sName      " , sItem.c_str() , Para.sName      );
    UserINI.Load(sPath.c_str() , sSelLan +"sComment   " , sItem.c_str() , Para.sComment   );
    for(int i = 0 ; i < MAX_X_CNT ; i++) {
        sTemp = "iFwdXAdd"+AnsiString(i) ;
        UserINI.Load(sPath.c_str() ,          "iFwdXAdd"+AnsiString(i), sItem.c_str() , Para.iFwdXAdd[i]   );
        UserINI.Load(sPath.c_str() ,          "iBwdXAdd"+AnsiString(i), sItem.c_str() , Para.iBwdXAdd[i]   );
    }

    UserINI.Load(sPath.c_str() ,          "iFwdYAdd   " , sItem.c_str() , Para.iFwdYAdd   );
    UserINI.Load(sPath.c_str() ,          "iBwdYAdd   " , sItem.c_str() , Para.iBwdYAdd   );
    UserINI.Load(sPath.c_str() ,          "iFwdOnDelay" , sItem.c_str() , Para.iFwdOnDelay);
    UserINI.Load(sPath.c_str() ,          "iBwdOnDelay" , sItem.c_str() , Para.iBwdOnDelay);
    UserINI.Load(sPath.c_str() ,          "iFwdTimeOut" , sItem.c_str() , Para.iFwdTimeOut);
    UserINI.Load(sPath.c_str() ,          "iBwdTimeOut" , sItem.c_str() , Para.iBwdTimeOut);
    UserINI.Load(sPath.c_str() ,          "iDirType   " , sItem.c_str() , Para.iDirType   );

    Init();
}

void CActuator::Save(EN_LAN_SEL _iLang)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sItem;
    AnsiString sPath;

    //Set Path.
    sPath  = EXE_FOLDER + "Util\\Actuator.INI";

    AnsiString sSelLan ;
    switch(_iLang) {
        default         : sSelLan = "E_" ; break ;
        case lsKorean   : sSelLan = "K_" ; break ;
        case lsEnglish  : sSelLan = "E_" ; break ;
        case lsChinese  : sSelLan = "C_" ; break ;
    }

    //Save.
    sItem.printf("ACTUATOR(%03d)" , m_iActrNo);
    UserINI.Save(sPath.c_str() ,          "sEnum      " , sItem.c_str() , Para.sEnum      );
    UserINI.Save(sPath.c_str() , sSelLan +"sName      " , sItem.c_str() , Para.sName      );
    UserINI.Save(sPath.c_str() , sSelLan +"sComment   " , sItem.c_str() , Para.sComment   );
    for(int i = 0 ; i < MAX_X_CNT ; i++) {
        UserINI.Save(sPath.c_str() ,          "iFwdXAdd"+AnsiString(i) , sItem.c_str() , Para.iFwdXAdd[i]   );
        UserINI.Save(sPath.c_str() ,          "iBwdXAdd"+AnsiString(i) , sItem.c_str() , Para.iBwdXAdd[i]   );
    }
    UserINI.Save(sPath.c_str() ,          "iFwdYAdd   " , sItem.c_str() , Para.iFwdYAdd   );
    UserINI.Save(sPath.c_str() ,          "iBwdYAdd   " , sItem.c_str() , Para.iBwdYAdd   );
    UserINI.Save(sPath.c_str() ,          "iFwdOnDelay" , sItem.c_str() , Para.iFwdOnDelay);
    UserINI.Save(sPath.c_str() ,          "iBwdOnDelay" , sItem.c_str() , Para.iBwdOnDelay);
    UserINI.Save(sPath.c_str() ,          "iFwdTimeOut" , sItem.c_str() , Para.iFwdTimeOut);
    UserINI.Save(sPath.c_str() ,          "iBwdTimeOut" , sItem.c_str() , Para.iBwdTimeOut);
    UserINI.Save(sPath.c_str() ,          "iDirType   " , sItem.c_str() , Para.iDirType   );

}

EN_ACTR_DIRECTION CActuator::GetDirType()
{
    return (EN_ACTR_DIRECTION)Para.iDirType ;
}

void CActuator::SetCntrlPn(TPanel * _pnBase)
{
    if(!m_pStCaption)m_pStCaption = new TStaticText (_pnBase) ;
    if(!m_pBtnBw    )m_pBtnBw     = new TBitBtn     (_pnBase) ;
    if(!m_pBtnFw    )m_pBtnFw     = new TBitBtn     (_pnBase) ;

    m_pStCaption -> BringToFront() ;
    m_pBtnBw     -> BringToFront() ;
    m_pBtnFw     -> BringToFront() ;

    m_pStCaption -> Parent = _pnBase ;
    m_pBtnBw     -> Parent = _pnBase ;
    m_pBtnFw     -> Parent = _pnBase ;

    m_pStCaption -> Caption   = Para.sName ;
    m_pStCaption -> BevelKind = bkFlat     ;
    m_pStCaption -> AutoSize  = false      ;
    m_pStCaption -> Alignment = taCenter   ;

    //모냥 찐딴데 나중에 다듬자.
    const int iMargin = 0 ; //마진을 주면 삐꾸나는데 마진 없이 그냥쓰자. int연산때문에 계산이 복잡하구 드러워서

    int iFontSize = _pnBase -> Height / 8 ;

    int iUDCellSize = _pnBase -> Height / 3 ;

    //해더.
    m_pStCaption -> Left         = iMargin ;
    m_pStCaption -> Top          = iMargin ;
    m_pStCaption -> Width        = _pnBase->Width - iMargin * 2 ;
    m_pStCaption -> Height       = _pnBase->Height / 3 - iMargin * 2 ;
    m_pStCaption -> Font -> Size = iFontSize/2 ;

    //위아래로 배치되는 버튼.
    int iUDBtnLeft     = iMargin ;
    int iUDBtnHeight   = _pnBase->Height / 3 - iMargin * 2 ;
    int iUBtnTop       = _pnBase->Height / 3 + iMargin * 3 ;
    int iDBtnTop       = _pnBase->Height * 2 / 3 + iMargin * 5 ;
    int iUDBtnWidth    = _pnBase->Width  - iMargin * 2 ;

    //양옆으로 배치되는 버튼.
    int iLBtnLeft      = iMargin ;
    int iRBtnLeft      = _pnBase->Width  / 2 + iMargin * 3 ;
    int iLRBtnTop      = _pnBase->Height / 3 + iMargin * 3 ;
    int iLRBtnWidth    = _pnBase->Width  / 2 - iMargin * 4 ;
    int iLRBtnHeight   = _pnBase->Height * 2 / 3 + iMargin * 4 ;

    m_pBtnBw -> Caption = "BWD";
    m_pBtnFw -> Caption = "FWD";

    m_pBtnBw -> Font -> Size = iFontSize ;
    m_pBtnFw -> Font -> Size = iFontSize ;

    m_pBtnBw -> OnMouseDown = btBwd;
    m_pBtnFw -> OnMouseDown = btFwd;

    switch(Para.iDirType) { 
        case adLR : if(pLtimg)m_pBtnBw -> Glyph = pLtimg ;
                    if(pRtimg)m_pBtnFw -> Glyph = pRtimg ;
                    m_pBtnBw -> Left   = iLBtnLeft    ;
                    m_pBtnBw -> Top    = iLRBtnTop    ;
                    m_pBtnBw -> Width  = iLRBtnWidth  ;
                    m_pBtnBw -> Height = iLRBtnHeight ;

                    m_pBtnFw -> Left   = iRBtnLeft    ;
                    m_pBtnFw -> Top    = iLRBtnTop    ;
                    m_pBtnFw -> Width  = iLRBtnWidth  ;
                    m_pBtnFw -> Height = iLRBtnHeight ;

                    break ;

        case adRL : if(pRtimg)m_pBtnBw -> Glyph = pRtimg ;
                    if(pLtimg)m_pBtnFw -> Glyph = pLtimg ;

                    m_pBtnBw -> Left   = iRBtnLeft    ;
                    m_pBtnBw -> Top    = iLRBtnTop    ;
                    m_pBtnBw -> Width  = iLRBtnWidth  ;
                    m_pBtnBw -> Height = iLRBtnHeight ;

                    m_pBtnFw -> Left   = iLBtnLeft    ;
                    m_pBtnFw -> Top    = iLRBtnTop    ;
                    m_pBtnFw -> Width  = iLRBtnWidth  ;
                    m_pBtnFw -> Height = iLRBtnHeight ;
                    break ;

        case adBF : if(pUpimg)m_pBtnBw -> Glyph = pUpimg ;
                    if(pDnimg)m_pBtnFw -> Glyph = pDnimg ;

                    m_pBtnBw -> Left   = iUDBtnLeft   ;
                    m_pBtnBw -> Top    = iUBtnTop     ;
                    m_pBtnBw -> Width  = iUDBtnWidth  ;
                    m_pBtnBw -> Height = iUDBtnHeight ;

                    m_pBtnFw -> Left   = iUDBtnLeft   ;
                    m_pBtnFw -> Top    = iDBtnTop     ;
                    m_pBtnFw -> Width  = iUDBtnWidth  ;
                    m_pBtnFw -> Height = iUDBtnHeight ;
                    break ;

        case adFB : if(pDnimg)m_pBtnBw -> Glyph = pDnimg ;
                    if(pUpimg)m_pBtnFw -> Glyph = pUpimg ;

                    m_pBtnBw -> Left   = iUDBtnLeft   ;
                    m_pBtnBw -> Top    = iDBtnTop     ;
                    m_pBtnBw -> Width  = iUDBtnWidth  ;
                    m_pBtnBw -> Height = iUDBtnHeight ;

                    m_pBtnFw -> Left   = iUDBtnLeft   ;
                    m_pBtnFw -> Top    = iUBtnTop     ;
                    m_pBtnFw -> Width  = iUDBtnWidth  ;
                    m_pBtnFw -> Height = iUDBtnHeight ;
                    break ;

        case adUD : if(pUpimg)m_pBtnBw -> Glyph = pUpimg ;
                    if(pDnimg)m_pBtnFw -> Glyph = pDnimg ;

                    m_pBtnBw -> Left   = iUDBtnLeft   ;
                    m_pBtnBw -> Top    = iUBtnTop     ;
                    m_pBtnBw -> Width  = iUDBtnWidth  ;
                    m_pBtnBw -> Height = iUDBtnHeight ;

                    m_pBtnFw -> Left   = iUDBtnLeft   ;
                    m_pBtnFw -> Top    = iDBtnTop     ;
                    m_pBtnFw -> Width  = iUDBtnWidth  ;
                    m_pBtnFw -> Height = iUDBtnHeight ;
                    break ;

        case adDU : if(pDnimg)m_pBtnBw -> Glyph = pDnimg ;
                    if(pUpimg)m_pBtnFw -> Glyph = pUpimg ;

                    m_pBtnBw -> Left   = iUDBtnLeft   ;
                    m_pBtnBw -> Top    = iDBtnTop     ;
                    m_pBtnBw -> Width  = iUDBtnWidth  ;
                    m_pBtnBw -> Height = iUDBtnHeight ;

                    m_pBtnFw -> Left   = iUDBtnLeft   ;
                    m_pBtnFw -> Top    = iUBtnTop     ;
                    m_pBtnFw -> Width  = iUDBtnWidth  ;
                    m_pBtnFw -> Height = iUDBtnHeight ;
                    break ;

        case adCA : if(pCwimg)m_pBtnBw -> Glyph = pCwimg ;
                    if(pAwimg)m_pBtnFw -> Glyph = pAwimg ;

                    m_pBtnBw -> Left   = iLBtnLeft    ;
                    m_pBtnBw -> Top    = iLRBtnTop    ;
                    m_pBtnBw -> Width  = iLRBtnWidth  ;
                    m_pBtnBw -> Height = iLRBtnHeight ;

                    m_pBtnFw -> Left   = iRBtnLeft    ;
                    m_pBtnFw -> Top    = iLRBtnTop    ;
                    m_pBtnFw -> Width  = iLRBtnWidth  ;
                    m_pBtnFw -> Height = iLRBtnHeight ;
                    break ;

        case adAC : if(pAwimg)m_pBtnBw -> Glyph = pAwimg ;
                    if(pCwimg)m_pBtnFw -> Glyph = pCwimg ;

                    m_pBtnBw -> Left   = iRBtnLeft    ;
                    m_pBtnBw -> Top    = iLRBtnTop    ;
                    m_pBtnBw -> Width  = iLRBtnWidth  ;
                    m_pBtnBw -> Height = iLRBtnHeight ;

                    m_pBtnFw -> Left   = iLBtnLeft    ;
                    m_pBtnFw -> Top    = iLRBtnTop    ;
                    m_pBtnFw -> Width  = iLRBtnWidth  ;
                    m_pBtnFw -> Height = iLRBtnHeight ;
                    break ;
    }

    m_bSetCntrlPn = true ;

}

void CActuator::DelCntrlPn()
{
    delete m_pStCaption;
    delete m_pBtnBw    ;
    delete m_pBtnFw    ;
}
void __fastcall CActuator::btBwd(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Move(ccBwd);
}
void __fastcall CActuator::btFwd(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Move(ccFwd);
}

void CActuator::SetBtnImg (Graphics::TBitmap *bmLtimg ,
                           Graphics::TBitmap *bmRtimg ,
                           Graphics::TBitmap *bmUpimg ,
                           Graphics::TBitmap *bmDnimg ,
                           Graphics::TBitmap *bmAwimg ,
                           Graphics::TBitmap *bmCwimg )
{
    pLtimg = bmLtimg ;
    pRtimg = bmRtimg ;
    pUpimg = bmUpimg ;
    pDnimg = bmDnimg ;
    pAwimg = bmAwimg ;
    pCwimg = bmCwimg ;

}

/***************************************************************************/
/* CActuator Array Class                                                   */
/***************************************************************************/
CActuators::CActuators(void)
{
    LoadDnmVar() ;

    Actuator = new CActuator * [m_iMaxAct] ;

    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] = new CActuator(i) ;
    }

    memset(&RptPara , 0 , sizeof(SRptPara));

    m_bRpt = false ;

    Load();

}
CActuators::~CActuators(void)
{
    Trace("","");
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        delete Actuator[i] ;
        Actuator[i] = NULL ;
    }
    delete [] Actuator ;
    Actuator = NULL ;
    Trace("","");    
}

//basic funcions.
void CActuators::Reset(void)
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Reset();
    }
}

AnsiString CActuators::GetName(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return "Error" ;

    return Actuator[_iActrNo] -> GetName();

}

bool CActuators::Complete(int _iActrNo , bool _bCmd)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> Complete(_bCmd);
}

bool CActuators::Complete (int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> Complete();
}

bool CActuators::Err(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return true ;

    return Actuator[_iActrNo] -> Err();
}

bool CActuators::Move(int _iActrNo , bool _bCmd)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> Move(_bCmd);
}

bool CActuators::GetCmd(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return false ;

    return Actuator[_iActrNo] -> GetCmd();

}
void CActuators::SetAct(int _iActrNo , bool _bAct)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return ;

    Actuator[_iActrNo] -> SetAct(_bAct);
}
//Repeat Control.
void CActuators::GoRpt(int _iDelay , int _iActr1No , int _iActr2No)
{
    if(_iActr1No<0 || m_iMaxAct <= _iActr1No) { MsgOk("Error","실린더 범위가 벗어났습니다."); return ;}
    if(_iActr2No<0 || m_iMaxAct <= _iActr2No) { _iActr2No = -1 ;                                      }

    RptPara.iDelay    = _iDelay   ;
    RptPara.iFrstActr = _iActr1No ;
    RptPara.iScndActr = _iActr2No ;
    m_bRpt = true ;
}

void CActuators::RptActr (void)
{
    if(RptPara.iScndActr < 0) { //실린더 1개 맞출때
        if(m_dtRptFwd.OnDelay(GetCmd(RptPara.iFrstActr) == ccFwd , RptPara.iDelay)) {
            Move(RptPara.iFrstActr , ccBwd);
            m_dtRptFwd.Clear();
        }
        if(m_dtRptBwd.OnDelay(GetCmd(RptPara.iFrstActr) == ccBwd , RptPara.iDelay)) {
            Move(RptPara.iFrstActr , ccFwd);
            m_dtRptBwd.Clear();
        }
    }
    else {                      //실린더 2개 같이 맞출때
        if(m_dtRptFwd.OnDelay(GetCmd(RptPara.iFrstActr) == ccFwd , RptPara.iDelay)) {
            Move(RptPara.iFrstActr , ccBwd);
            Move(RptPara.iScndActr , ccBwd);
            m_dtRptFwd.Clear();
        }
        if(m_dtRptBwd.OnDelay(GetCmd(RptPara.iFrstActr) == ccBwd , RptPara.iDelay)) {
            Move(RptPara.iFrstActr , ccFwd);
            Move(RptPara.iScndActr , ccFwd);
            m_dtRptBwd.Clear();
        }
    }
}

void CActuators::StopRpt (void)
{
    m_bRpt = false ;
    Reset();
}


//Update.
void CActuators::Update(void)
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Update();
    }
    if (m_bRpt ) RptActr ();
}

//Read/Write Para.
void CActuators::Load()
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Load(m_iLangSel);
    }
}
void CActuators::Save()
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i] -> Save(m_iLangSel);
    }

}
void CActuators::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    int iLangSel ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CActuators" , "m_iMaxAct " , m_iMaxAct );
    UserINI.Load(sPath , "Member"     , "m_iLangSel" , iLangSel  ); m_iLangSel = (EN_LAN_SEL)iLangSel ;

    if(m_iMaxAct == 0 ) m_iMaxAct = 1 ;

}
void CActuators::SaveDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CActuators" , "m_iMaxAct " ,      m_iMaxAct );
    UserINI.Save(sPath , "Member"     , "m_iLangSel" , (int)m_iLangSel);
}

EN_ACTR_DIRECTION CActuators::GetDirType(int _iActrNo)
{
    if(_iActrNo < 0 || _iActrNo >= m_iMaxAct) return adNone ;

    return Actuator[_iActrNo] -> GetDirType() ;
}

//=============================================================================
//Display.
void CActuators::DisplayList(TStringGrid *_sgTable)
{
    //Local Var.
    int        rCount;
    AnsiString Temp;

    //Check Error.
    if (_sgTable == NULL) return;

    _sgTable -> Options << goRowSelect ;
    _sgTable -> DefaultRowHeight = 20 ;

        //Set StringGrid.
    rCount                 = m_iMaxAct;
    _sgTable -> RowCount      = rCount + 1;  if(_sgTable -> RowCount == 1) _sgTable -> RowCount = 2 ;
    _sgTable -> ColCount      = 11;
    _sgTable -> ColWidths[0 ] = 20;
    _sgTable -> ColWidths[1 ] = 150;
    _sgTable -> ColWidths[2 ] = 200;
    _sgTable -> ColWidths[3 ] = 40;
    _sgTable -> ColWidths[4 ] = 40;
    _sgTable -> ColWidths[5 ] = 40;
    _sgTable -> ColWidths[6 ] = 40;
    _sgTable -> ColWidths[7 ] = 40;
    _sgTable -> ColWidths[8 ] = 40;
    _sgTable -> ColWidths[9 ] = 40;
    _sgTable -> ColWidths[10] = 40;

        //Set Col.
    _sgTable -> Cells[0 ][0] = "No"      ;
    _sgTable -> Cells[1 ][0] = "Name"    ;
    _sgTable -> Cells[2 ][0] = "Comment" ;
    _sgTable -> Cells[3 ][0] = "xFwdID"  ;
    _sgTable -> Cells[4 ][0] = "xBwdID"  ;
    _sgTable -> Cells[5 ][0] = "yFwdID"  ;
    _sgTable -> Cells[6 ][0] = "yBwdID"  ;
    _sgTable -> Cells[7 ][0] = "FwdOD"   ;
    _sgTable -> Cells[8 ][0] = "BwdOD"   ;
    _sgTable -> Cells[9 ][0] = "FwdTO"   ;
    _sgTable -> Cells[10][0] = "BwdTO"   ;

    _sgTable -> FixedCols = 1  ;
    _sgTable -> FixedRows = 1  ;

        //Index No. Cell.
        //Display.

    CActuator::SPara * Para ;

    AnsiString sXAdd ="";

    for (int i = 0 ; i < rCount ; i++) {
        Para = Actuator[i] -> GetPara() ;
        //Display.
        _sgTable -> Cells[0 ][i + 1] = i;
        _sgTable -> Cells[1 ][i + 1] = Para->sName;
        _sgTable -> Cells[2 ][i + 1] = Para->sComment;

        sXAdd = Para->iFwdXAdd[0] ;
        for(int i = 1 ; i < MAX_X_CNT ; i++) {
            if(Para->iFwdXAdd[i] != 0) sXAdd += AnsiString(",") + AnsiString(Para->iFwdXAdd[i]) ;
        }
        _sgTable -> Cells[3 ][i + 1] = sXAdd;

        sXAdd = Para->iBwdXAdd[0] ;
        for(int i = 1 ; i < MAX_X_CNT ; i++) {
            if(Para->iBwdXAdd[i] != 0) sXAdd += AnsiString(",") + AnsiString(Para->iBwdXAdd[i])  ;
        }
        _sgTable -> Cells[4 ][i + 1] = sXAdd;


        _sgTable -> Cells[5 ][i + 1] = Para->iFwdYAdd;
        _sgTable -> Cells[6 ][i + 1] = Para->iBwdYAdd;
        _sgTable -> Cells[7 ][i + 1] = Para->iFwdOnDelay;
        _sgTable -> Cells[8 ][i + 1] = Para->iBwdOnDelay;
        _sgTable -> Cells[9 ][i + 1] = Para->iFwdTimeOut;
        _sgTable -> Cells[10][i + 1] = Para->iBwdTimeOut;
    }

    //Selected Row to 0.
    static bool bFirst = false ;
    if(bFirst) {_sgTable -> TopRow = 1; _sgTable -> Row = 1 ; }
    bFirst = false ;

}
void CActuators::DisplayStatus(int _iActrNo , TLabel   *_lbFwdStat , TLabel  *_lbBwdStat , TLabel *_lbAlarm)
{
    //Check Null.
    if ((_lbFwdStat == NULL) || (_lbBwdStat == NULL) || (_lbAlarm == NULL)) return;

    //Display.
    if (Actuator[_iActrNo]->Complete(ccFwd)) _lbFwdStat ->Color = clLime;
    else                                     _lbFwdStat ->Color = clGray;
    if (Actuator[_iActrNo]->Complete(ccBwd)) _lbBwdStat ->Color = clLime;
    else                                     _lbBwdStat ->Color = clGray;
    if (Actuator[_iActrNo]->Err()          ) _lbAlarm   ->Color = clRed ;
    else                                     _lbAlarm   ->Color = clGray;

}

void CActuators::DisplayButton(int _iActrNo , TButton  *_btBwdStat , TButton *_btFwdStat)
{
    //Check Null.
    if ((_btFwdStat == NULL) || (_btBwdStat == NULL)) return;

    //Display.
    if (Actuator[_iActrNo]->Complete(ccFwd)) {_btFwdStat -> Caption  = "DONE"; }
    else                                     {_btFwdStat -> Caption  = "OFF" ; }
    if (Actuator[_iActrNo]->Complete(ccBwd)) {_btBwdStat -> Caption  = "DONE"; }
    else                                     {_btBwdStat -> Caption  = "OFF" ; }

}

void CActuators::DisplayProperty(bool         _bToTble , TEdit       * _edEnum    ,
                                 TEdit       * _edName , TEdit       * _edComt    ,
                                 TEdit       * _edFx   , TEdit       * _edBx      ,
                                 TEdit       * _edFy   , TEdit       * _edBy      ,
                                 TEdit       * _edFd   , TEdit       * _edBd      ,
                                 TEdit       * _edFt   , TEdit       * _edBt      ,
                                 TStringGrid *_sgTable , TComboBox   * _cbDirType )
{
    //Local Var.
    AnsiString Temp;
    int        iActrNo;
    CActuator::SPara * Para ;
    //    SPara Para ;

    //Check Null.
    if (_edEnum    == NULL ||
        _edName    == NULL ||
        _edComt    == NULL ||
        _edFx      == NULL ||
        _edBx      == NULL ||
        _edFy      == NULL ||
        _edBy      == NULL ||
        _edFd      == NULL ||
        _edBd      == NULL ||
        _edFt      == NULL ||
        _edBt      == NULL ||
        _sgTable   == NULL ||
        _cbDirType == NULL ) return ;

    //Get Index.
    iActrNo = _sgTable -> Row - 1;

    if(iActrNo >= m_iMaxAct) return ;

    Para = Actuator[iActrNo] -> GetPara() ;

    AnsiString sXAdd ="" ;
    //Display.
    if (_bToTble) {
        _edEnum    -> Text      = Para->sEnum       ;
        _edName    -> Text      = Para->sName       ;
        _edComt    -> Text      = Para->sComment    ;

        sXAdd = Para->iFwdXAdd[0] ;
        for(int i = 1 ; i < MAX_X_CNT ; i++) {
            if(Para->iFwdXAdd[i] != 0) sXAdd += AnsiString(",") + AnsiString(Para->iFwdXAdd[i]) ;
        }
        _edFx      -> Text      = sXAdd             ;

        sXAdd = Para->iBwdXAdd[0] ;
        for(int i = 1 ; i < MAX_X_CNT ; i++) {
            if(Para->iBwdXAdd[i] != 0) sXAdd += AnsiString(",") + AnsiString(Para->iBwdXAdd[i]) ;
        }
        _edBx      -> Text      = sXAdd             ;

        _edFy      -> Text      = Para->iFwdYAdd    ;
        _edBy      -> Text      = Para->iBwdYAdd    ;
        _edFd      -> Text      = Para->iFwdOnDelay ;
        _edBd      -> Text      = Para->iBwdOnDelay ;
        _edFt      -> Text      = Para->iFwdTimeOut ;
        _edBt      -> Text      = Para->iBwdTimeOut ;
        _cbDirType -> ItemIndex = Para->iDirType    ;
    }
    else {
        Para->sEnum         = _edEnum    -> Text                ;
        Para->sName         = _edName    -> Text                ;
        Para->sComment      = _edComt    -> Text                ;
        if(_edFx -> Text.Pos(",")) {
            AnsiString sTemp ;
            AnsiString sBuff  = _edFx -> Text ;
            int i = 0 ;
            for(i = 0 ; i < MAX_X_CNT ; i++) {
                sTemp = sBuff.SubString(1,sBuff.Pos(",")-1);
                        sBuff.Delete(1,sBuff.Pos(","));
                Para->iFwdXAdd[i] = sTemp.ToIntDef(-1);
                if(!sBuff.Pos(",")) break ;
            }
            Para->iFwdXAdd[i+1] = sBuff.ToIntDef(-1);
        }
        else {
            Para->iFwdXAdd[0]      = _edFx      -> Text.ToIntDef(-1)   ;
        }

        if(_edBx -> Text.Pos(",")) {
            AnsiString sTemp ;
            AnsiString sBuff  = _edBx -> Text ;
            int i = 0 ;
            for(i = 0 ; i < MAX_X_CNT ; i++) {
                sTemp = sBuff.SubString(1,sBuff.Pos(",")-1);
                        sBuff.Delete(1,sBuff.Pos(","));
                Para->iBwdXAdd[i] = sTemp.ToIntDef(-1);
                if(!sBuff.Pos(",")) break ;
            }
             Para->iBwdXAdd[i+1] = sBuff.ToIntDef(-1);
        }
        else {
            Para->iBwdXAdd[0]      = _edBx      -> Text.ToIntDef(-1)   ;
        }



        //Para->iFwdXAdd      = _edFx      -> Text.ToIntDef(-1)   ;
        //Para->iBwdXAdd      = _edBx      -> Text.ToIntDef(-1)   ;
        Para->iFwdYAdd      = _edFy      -> Text.ToIntDef(-1)   ;
        Para->iBwdYAdd      = _edBy      -> Text.ToIntDef(-1)   ;
        Para->iFwdOnDelay   = _edFd      -> Text.ToIntDef(0)    ;
        Para->iBwdOnDelay   = _edBd      -> Text.ToIntDef(0)    ;
        Para->iFwdTimeOut   = _edFt      -> Text.ToIntDef(4000) ;
        Para->iBwdTimeOut   = _edBt      -> Text.ToIntDef(4000) ;
        Para->iDirType      = _cbDirType -> ItemIndex           ;
    }

}

void CActuators::SetCntrlPn(int _iActrNo , TPanel * _pnBase)
{
    Actuator[_iActrNo]->SetCntrlPn(_pnBase);
}
void CActuators::DelCntrlPn (int _iActrNo)
{
    Actuator[_iActrNo]->DelCntrlPn();
}

void CActuators::SetBtnImg (Graphics::TBitmap *bmLtimg ,
                            Graphics::TBitmap *bmRtimg ,
                            Graphics::TBitmap *bmUpimg ,
                            Graphics::TBitmap *bmDnimg ,
                            Graphics::TBitmap *bmAwimg ,
                            Graphics::TBitmap *bmCwimg )
{
    for(int i = 0 ; i < m_iMaxAct ; i++) {
        Actuator[i]->SetBtnImg(bmLtimg,
                               bmRtimg,
                               bmUpimg,
                               bmDnimg,
                               bmAwimg,
                               bmCwimg) ;
    }
}

















































































