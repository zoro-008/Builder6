//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AdlinkUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CAdlinkUnit ADL;
CAdlinkUnit::CAdlinkUnit()
{
    card  =-1;
    card_number =0;
    m_iSetNo = 0 ;
    if ((card=Register_Card(PCI_7230, card_number)) < 0) ShowMessage("Failed PCI_7230 ");
}

CAdlinkUnit::~CAdlinkUnit()
{
    if (card >= 0) Release_Card(card);
}

void __fastcall CAdlinkUnit::Update()
{
    GetInput ();
    SetOutput();

}

void __fastcall CAdlinkUnit::GetInput ()
{
    AnsiString tmpTenkey ;

           U32 TenkeyInput    = 0x00;
    static U32 TenkeyOldInput = 0x00;

    if (card<0) return  ;

    DI_ReadPort(card , PORT_DI_LOW , &TenkeyInput );

    if(TenkeyInput == 0x00 || TenkeyInput == TenkeyOldInput) {
        TenkeyOldInput = TenkeyInput ;
        return ;
    }

    TenkeyOldInput = TenkeyInput ;

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

    if(tmpTenkey == "C") { m_iSetNo = 0 ; return ;}
    if(tmpTenkey == "S") { /*메뉴얼 구동.*/ return ;}
    if(tmpTenkey == "E") { m_iSetNo = 0 ; return ;}

    tmpTenkey = m_iSetNo * 10 + StrToInt(tmpTenkey) ;

    m_iSetNo = StrToInt(tmpTenkey) % 200 ;
}

void __fastcall CAdlinkUnit::SetOutput()
{
    if (card<0) return  ;

    AnsiString sTemp ;
    unsigned long lValue ;

    sTemp = sTemp.sprintf("%x%x%x",m_iSetNo/100 , m_iSetNo/10 , m_iSetNo%10 ) ;
    lValue = StrToInt(AnsiString('0x')+sTemp);
    DO_WritePort(card,0,lValue ^ 0xFF);
}



