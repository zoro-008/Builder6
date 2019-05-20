//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameCylOneBt.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFraCylOneBt *FraCylOneBt;
//---------------------------------------------------------------------------
__fastcall TFraCylOneBt::TFraCylOneBt(TComponent* Owner)
        : TFrame(Owner)
{
//__fastcall TFraCylOneBt::TFraCylOneBt(void) : TFrame(Owner)
//{
    m_iActrId = -1;
    m_iType   =  0;
    m_pCheckSafeCallback = NULL;

    m_pBtImgFw = new Graphics::TBitmap() ;
    m_pBtImgBw = new Graphics::TBitmap() ;

    //m_bSetSize = false ;







}
//---------------------------------------------------------------------------
void TFraCylOneBt::SetConfig(EN_ACTR_ID        _iActrId           ,
                             AnsiString        _sTitle            ,
                             EN_ACTR_DIRECTION _iActrType         ,
                             TWinControl *     _wcParent          ,
                             CheckSafeCallback _pCheckSafeCallback)
{
    AnsiString sActrName;
    sActrName = _sTitle ;
    sActrName = sActrName.Delete(11,4);
    sActrName = sActrName.Delete(1 ,2);
    sActrName = StringReplace(sActrName , "_" , " " , TReplaceFlags()<<rfReplaceAll); 

    m_iActrId            = _iActrId            ;
    stTitle -> Caption   = sActrName           ;
    m_iType              = _iActrType          ;
    this -> Parent       = _wcParent           ;
    m_pCheckSafeCallback = _pCheckSafeCallback ;
    this -> Align        = alClient            ;

    //_wcParent -> Width  += 1 ;
    //_wcParent -> Width  -= 1 ;
    //btAction -> Top      = stTitle -> Top + stTitle -> Height + 1 ;
    //btAction -> Height   = this -> Height - btAction -> Top + 11;
    //btAction -> Width    = this -> Width -8;
    //btAction -> Left     = 0 ;

    //m_bPreDone = false ;
    m_bPreCmd = true ;


    tmUpdate -> Enabled = true ;

    //imDirection -> GetBitmap(m_iType*2+1 , m_pBtImgFw); //Fwd
    //imDirection -> GetBitmap(m_iType*2+0 , m_pBtImgBw); //Bwd

    imDirection -> GetBitmap(m_iType*2+0 , m_pBtImgFw);
    imDirection -> GetBitmap(m_iType*2+1 , m_pBtImgBw);

    switch(m_iType) {
        default   : m_sCaptionBw = "RIGHT" ;m_sCaptionFw = "LEFT"  ;  break ;
        case adLR : m_sCaptionBw = "RIGHT" ;m_sCaptionFw = "LEFT"  ;  break ;
        case adRL : m_sCaptionBw = "LEFT"  ;m_sCaptionFw = "RIGHT" ;  break ;
        case adBF : m_sCaptionBw = "FWD"   ;m_sCaptionFw = "BWD"   ;  break ;
        case adFB : m_sCaptionBw = "BWD"   ;m_sCaptionFw = "FWD"   ;  break ;
        case adUD : m_sCaptionBw = "DOWN"  ;m_sCaptionFw = "UP"    ;  break ;
        case adDU : m_sCaptionBw = "UP"    ;m_sCaptionFw = "DOWN"  ;  break ;
        case adCA : m_sCaptionBw = "CW"    ;m_sCaptionFw = "CCW"   ;  break ;
        case adAC : m_sCaptionBw = "CCW"   ;m_sCaptionFw = "CW"    ;  break ;
    }

    /*
    adLR = 0 , //���鿡��   ������ Left �� Bwd Right�� Fwd
    adRL     , //���鿡��   ������ Right�� Bwd Left �� Fwd
    adBF     , //���鿡��   ������ Bwd  �� Bwd Fwd  �� Fwd
    adFB     , //���鿡��   ������ Fwd  �� Bwd Bwd  �� Fwd
    adUD     , //���鿡��   ������ Up   �� Bwd Down �� Fwd
    adDU     , //���鿡��   ������ Down �� Bwd Up   �� Fwd
    adCA     , //ȸ���࿡�� ������ Clock�� Bwd AntiC�� Fwd
    adAC       //ȸ���࿡�� ������ AntiC�� Bwd Clock�� Fwd
    */

}
void TFraCylOneBt::SetSize()
{
    btAction -> Top      = stTitle -> Top + stTitle -> Height + 1 ;
    btAction -> Height   = this -> Height - btAction -> Top ;
    btAction -> Width    = this -> Width ;
    btAction -> Left     = 0 ;
}
void __fastcall TFraCylOneBt::tmUpdateTimer(TObject *Sender)
{
/*
    tmUpdate -> Enabled = false ;

    if(!m_bSetSize && this -> Showing ){
        SetSize() ;
        m_bSetSize = true ;
    }

    bool bDone = AT_Done(m_iActrId) ;
    bool bErr  = AT_Err (m_iActrId) ;


    //For �� �������ٰ�...����...�׷��� ���� �����ϴ°� ��������...dd
         if(bDone)btAction -> Font -> Color = clLime ;
    else if(bErr )btAction -> Font -> Color = clRed  ;
    else          btAction -> Font -> Color = clBlack;

    if(bDone && !m_bPreDone) {
        if(AT_GetCmd(m_iActrId)) {btAction -> Glyph = m_pBtImgFw ; btAction -> Caption = m_sCaptionFw ;}
        else                     {btAction -> Glyph = m_pBtImgBw ; btAction -> Caption = m_sCaptionBw ;}
    }

    m_bPreDone = bDone ;
    tmUpdate -> Enabled = true ; */

    tmUpdate -> Enabled = false ;

    if(!m_bSetSize && this -> Showing ){
        SetSize() ;
        m_bSetSize = true ;
    }

    bool bCmd  = AT_GetCmd(m_iActrId) ;
    bool bErr  = AT_Err   (m_iActrId) ;
    bool bDone = AT_Done  (m_iActrId) ;

    //For �� �������ٰ�...����...�׷��� ���� �����ϴ°� ��������...dd
         if(bDone)btAction -> Font -> Color = clLime ;
    else if(bErr )btAction -> Font -> Color = clRed  ;
    else          btAction -> Font -> Color = clBlack;

    if(bCmd != m_bPreCmd) {
        if(AT_GetCmd(m_iActrId)) {btAction -> Glyph = m_pBtImgFw ; btAction -> Caption = m_sCaptionFw ;}
        else                     {btAction -> Glyph = m_pBtImgBw ; btAction -> Caption = m_sCaptionBw ;}
    }

    m_bPreCmd = bCmd ;
    tmUpdate -> Enabled = true ;
}

//---------------------------------------------------------------------------
void __fastcall TFraCylOneBt::btActionClick(TObject *Sender)
{
    if(m_pCheckSafeCallback && !m_pCheckSafeCallback(m_iActrId , AT_GetCmd(m_iActrId))) return ;

    AT_MoveCyl(m_iActrId , !AT_GetCmd(m_iActrId));

    AnsiString sMsg ;
    sMsg  = "FrameCyl Form_Cylinder Move Button Click" + AnsiString(AT_GetName(m_iActrId)) ;
    sMsg += AT_GetCmd(m_iActrId) ? "(Fwd)" : "(Bwd)" ;

    Trace("Operator", sMsg.c_str());
}
//---------------------------------------------------------------------------

