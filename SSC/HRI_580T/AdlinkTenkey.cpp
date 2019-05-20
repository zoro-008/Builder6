//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AdlinkTenkey.h"
#include "ManualMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CAdlinkTenkey ADL;
CAdlinkTenkey::CAdlinkTenkey()
{
    card  =-1;
    card_number =0;
    m_iSetNo = 0 ;
    if ((card=Register_Card(PCI_7230, card_number)) < 0) ShowMessage("Failed PCI_7230 ");
}

CAdlinkTenkey::~CAdlinkTenkey()
{
    if (card >= 0) Release_Card(card);
}

void __fastcall CAdlinkTenkey::Update()
{
    GetInput ();
    SetOutput();

}

void __fastcall CAdlinkTenkey::GetInput ()
{
    AnsiString tmpTenkey ;

    int iTempNo ;

           U32 TenkeyInput    = 0x00;
    static U32 TenkeyOldInput = 0x00;

    if (card<0) return  ;

    DI_ReadPort(card , PORT_DI_LOW , &TenkeyInput );

    if(TenkeyInput == 0x00 || TenkeyInput == TenkeyOldInput) {
        TenkeyOldInput = TenkeyInput ;
        return ;
    }

    TenkeyOldInput = TenkeyInput ;

    m_iInputCnt++;
    if(m_iInputCnt > 2000) m_iInputCnt = 0;

         if( TenkeyInput == 0x14 ) tmpTenkey = "1";
    else if( TenkeyInput == 0x24 ) tmpTenkey = "2";
    else if( TenkeyInput == 0x44 ) tmpTenkey = "3";
    else if( TenkeyInput == 0x12 ) tmpTenkey = "4";
    else if( TenkeyInput == 0x22 ) tmpTenkey = "5";
    else if( TenkeyInput == 0x42 ) tmpTenkey = "6";

    else if( TenkeyInput == 0x11 ) tmpTenkey = "7";
    else if( TenkeyInput == 0x21 ) tmpTenkey = "8";
    else if( TenkeyInput == 0x41 ) tmpTenkey = "9";

    else if( TenkeyInput == 0x18 ) tmpTenkey = "C";
    else if( TenkeyInput == 0x28 ) tmpTenkey = "0";
    else if( TenkeyInput == 0x48 ) tmpTenkey = "S";
    else                           tmpTenkey = "E";

    m_iInputChar = tmpTenkey ;

    if(tmpTenkey == "C") { m_iSetNo = 0 ; return ;}
    if(tmpTenkey == "S") { MM.SetManCycle((EN_MANUAL_CYCLE)m_iSetNo);    return ;}
    if(tmpTenkey == "E") {                  return ;}
    if(m_iSetNo != 0) {
        iTempNo = m_iSetNo * 10 + StrToInt(tmpTenkey) ;
        //if(iTempNo > 199)m_iSetNo = iTempNo % 100 ;
        if(iTempNo > 100)m_iSetNo = iTempNo % 100 ;
        else             m_iSetNo = iTempNo ;
    }
    else {
        m_iSetNo = StrToInt(tmpTenkey) ;
    }


}
/*
    lTemp0Value  = ~lValue & 0xF0 ;
    lTemp0Value  = lTemp0Value >> 4 ;
    lTemp10Value = ~lValue & 0xF ;
    lTemp10Value = lTemp10Value << 4 ;
    lValue = lValue & 0x10000 ;
    lValue = lValue | lTemp10Value | lTemp0Value ;

    lLastValue = lValue /*^ 0xFF*/ ;


void __fastcall CAdlinkTenkey::SetOutput()
{
    if (card<0) return  ;

    AnsiString sTemp ;
    unsigned long lValue = 0 ;
    unsigned long l0Value = 0 ;
    unsigned long l10Value = 0 ;
    unsigned long l100Value = 0 ;

    unsigned long lLastValue = 0 ;
    unsigned long lTemp0Value = 0 ;
    unsigned long lTemp10Value = 0 ;

    //sTemp = sTemp.sprintf("%x%x%x",m_iSetNo/100 , m_iSetNo/10 , m_iSetNo%10 ) ;
    //sTemp = sTemp.sprintf("0x%02x",m_iSetNo) ;
    l0Value  = m_iSetNo  % 10; //StrToInt(sTemp);
    l10Value = (m_iSetNo % 100) / 10; //StrToInt(sTemp);
    l100Value = m_iSetNo / 100 ; //StrToInt(sTemp);


//    if(m_iSetNo > 10) {
        lTemp0Value  = (~l0Value) & 0x0F ;
        lTemp0Value  = lTemp0Value << 4 ;

        lTemp10Value = (~l10Value) & 0x0F ;
//        lTemp10Value = lTemp10Value << 4 ;

        l100Value = l100Value & 0x01 ;
        l100Value = l100Value <<8 ;

//        sTemp = sTemp.sprintf("%x%x%x",l100Value , lTemp10Value , lTemp0Value );


        lValue = l100Value | lTemp10Value | lTemp0Value ;

//        lLastValue = lValue /*^ 0xFF*/ ;
//    }
//    else {
//        lLastValue = lValue ^ 0xFF ;
//
//    }

    DO_WritePort(card,0,lValue);
}



