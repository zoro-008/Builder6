//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "StageOST.h"
#include "Stage.h"
#include "SortCmn.h"
#include "stdlib.h "
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//---------------------------------------------------------------------------

#define OST_FOLDER "d:\\OstLog\\"

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CStageOST OST;

CStageOST::CStageOST(void)
{

}

CStageOST::~CStageOST (void)
{

}

void CStageOST::Init()
{
    m_sPartName = "Stage OST" ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_sRcvMsg = "";

    m_pRs232_OST = new TRS232C ();

    if(!m_pRs232_OST->Open(0)) ShowMessage("OST Tester Rs232 Port Open Fail") ;
    else                       m_pRs232_OST->CallBackReg(ReceiveMsg);

    String sMsg = "[DVC,"+AnsiString(OM.DevOptn.iOSTDeviceNo)+"#" ;
    OST.m_pRs232_OST -> SendData(sMsg.Length(),sMsg.c_str());


    m_iOstNGCntPock1 = 0 ;
    m_iOstNGCntPock2 = 0 ;
    m_iOstNGCntPock3 = 0 ;
    m_iOstNGCntPock4 = 0 ;

    m_iCrtNGCntPock1 = 0 ;
    m_iCrtNGCntPock2 = 0 ;
    m_iCrtNGCntPock3 = 0 ;
    m_iCrtNGCntPock4 = 0 ;

    //m_dLaserWorkTime = 0.0 ;
}
void CStageOST::ReceiveMsg(DWORD _cbInQue)
{
    //Ex : GA,0,CheckSum
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;
    BYTE RcvBuff[512];
    //BYTE * RcvBuff;
    //RcvBuff = new BYTE[_cbInQue] ;

    memset(&RcvBuff, 0 , sizeof(BYTE) * 512);


    OST.m_pRs232_OST -> ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;                                  //d20001001

    Trace("Rcv OST Com",RcvBuff);

    //delete [] RcvBuff ;

/*
]OST1:00000,06451,00511
]IDD1:1504,2192,0031,1613
]IDS1:1503,2188,0029,1612
]OST2:00000,00000,00000
]IDD2:0000,0000,0000,0000
]IDS2:0000,0000,0000,0000
]OST3:00000,00000,00000
]IDD3:0000,0000,0000,0000
]IDS3:0000,0000,0000,0000
]OST4:00000,00000,00000
]IDD4:0000,0000,0000,0000
]IDS4:0000,0000,0000,0000
#
*/

    //검사 결과 종료 문자는 #임.
    if(!sRcvMsg.Pos("#"  )) return ;
    Trace("Rcv OST Total Com",sRcvMsg.c_str());
    if( sRcvMsg == OST.m_pRs232_OST->sSendMsg) { //보낸 메세지의 답변인것 확인.
        sRcvMsg = "";
        return ;
    }

    OST.m_pRs232_OST->sData = sRcvMsg;
    sRcvMsg  = "" ;
}

String IntToBin(unsigned short num)
{
    AnsiString Result;
    int BinDigit = sizeof(num) * 8; 
    for(int i=0; i<BinDigit; i++) 
        Result = AnsiString(num>>i & 1) + Result;
    return Result; 
} 
bool CStageOST::GetOSTFail(int _iPockNo , String _sRcvMsg , bool &_bRslt)//Open Short Test
{
    String sPockRslt ;
    String sPockID   ;

    _bRslt = false ;

    if(_iPockNo < 0) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}
    if(_iPockNo > 3) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}

    sPockID = "]OST" + String(_iPockNo+1) ;

    int iTemp = _sRcvMsg.Pos("#") ;
    if(!iTemp) {
        Trace("Can't Find End# " , _sRcvMsg.c_str());
        return false ;
    }

    iTemp = _sRcvMsg.Pos(sPockID) ;
    if(!iTemp) {
        Trace("Can't Find Pock ID" , _sRcvMsg.c_str());
        return false ;
    }

    sPockRslt = _sRcvMsg.Delete(1, iTemp);  //쓸데이터의 앞쪽 데이터 짜르기
    sPockRslt = sPockRslt.Delete(1,sPockID.Length()); //쓸데이터 해더 짜르기.
    iTemp = sPockRslt.Pos("\r\n") ;
    if(!iTemp) {
        Trace("Can't Find CR" , sPockRslt.c_str());
        return false ;
    }
    sPockRslt = sPockRslt.Delete(iTemp ,sPockRslt.Pos("#"));

    //const int MAX_PIN_CNT = 48;

    String sRslt01_16 = sPockRslt.SubString(1 ,5 );
    String sRslt17_32 = sPockRslt.SubString(7 ,5 );
    String sRslt33_48 = sPockRslt.SubString(13,5 );

    unsigned short iRslt01_16 = sRslt01_16.ToIntDef(32767);  //Default all fail
    unsigned short iRslt17_32 = sRslt17_32.ToIntDef(32767);
    unsigned short iRslt33_48 = sRslt33_48.ToIntDef(32767);



    String sRslt01_16Bin = IntToBin(iRslt01_16);
    String sRslt17_32Bin = IntToBin(iRslt17_32);
    String sRslt33_48Bin = IntToBin(iRslt33_48);


    for(int i = 0 ; i < 16 ; i++){ //한콤마에 16개 데이터 있음.
        OSTRslt[_iPockNo].bRsltOSTFail[0  + i] = sRslt01_16Bin.SubString(16-i,1) == "1" ;
        OSTRslt[_iPockNo].bRsltOSTFail[16 + i] = sRslt17_32Bin.SubString(16-i,1) == "1" ;
        OSTRslt[_iPockNo].bRsltOSTFail[32 + i] = sRslt33_48Bin.SubString(16-i,1) == "1" ;
    }
    _bRslt = false ;
    for(int i = 0 ; i < 48 ; i++){ //한콤마에 16개 데이터 있음.
        if(OSTRslt[_iPockNo].bRsltOSTFail[i]) _bRslt = true ;
    }

    return true ;
}

bool CStageOST::GetIDDFail(int _iPockNo , String _sRcvMsg , bool &_bRslt)//구동 전류 테스트.
{
    String sPockRslt ;
    String sPockID   ;

    _bRslt = false ;

    if(_iPockNo < 0) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}
    if(_iPockNo > 3) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}

    sPockID = "]IDD" + String(_iPockNo+1) ;

    int iTemp = _sRcvMsg.Pos("#") ;
    if(!iTemp) {
        Trace("Can't Find End# " , _sRcvMsg.c_str());
        return false ;
    }

    iTemp = _sRcvMsg.Pos(sPockID) ;
    if(!iTemp) {
        Trace("Can't Find Pock ID" , _sRcvMsg.c_str());
        return false ;
    }

    sPockRslt = _sRcvMsg.Delete(1, iTemp);  //쓸데이터의 앞쪽 데이터 짜르기
    sPockRslt = sPockRslt.Delete(1,sPockID.Length()); //쓸데이터 해더 짜르기.
    iTemp = sPockRslt.Pos("\r\n") ;
    if(!iTemp) {
        Trace("Can't Find CR" , sPockRslt.c_str());
        return false ;
    }
    sPockRslt = sPockRslt.Delete(iTemp ,sPockRslt.Pos("#"));

    //4자리컴마로 아이템 구분함.
    String sRsltAVDD = sPockRslt.SubString(1 ,4 );
    String sRsltCVDD = sPockRslt.SubString(6 ,4 );
    String sRsltHVDD = sPockRslt.SubString(11,4 );
    String sRsltDVDD = sPockRslt.SubString(16,4 );

    int iIntAVDD = StrToIntDef(sRsltAVDD , -1) ;
    int iIntCVDD = StrToIntDef(sRsltCVDD , -1) ;
    int iIntHVDD = StrToIntDef(sRsltHVDD , -1) ;
    int iIntDVDD = StrToIntDef(sRsltDVDD , -1) ;

    if(iIntAVDD == -1) {Trace("Wrong Pock No" , sRsltAVDD.c_str()); return false ; }
    if(iIntCVDD == -1) {Trace("Wrong Pock No" , sRsltCVDD.c_str()); return false ; }
    if(iIntHVDD == -1) {Trace("Wrong Pock No" , sRsltHVDD.c_str()); return false ; }
    if(iIntDVDD == -1) {Trace("Wrong Pock No" , sRsltDVDD.c_str()); return false ; }

    //double dFloatAVDD = iIntAVDD/92.0 ; //미리 암페어 단위. 92는 대겸 컨트롤러 분해능에 따라 바뀐다.
    //double dFloatCVDD = iIntCVDD/92.0 ;
    //double dFloatHVDD = iIntHVDD/92.0 ;
    //double dFloatDVDD = iIntDVDD/92.0 ;

    /*
    //2014.08.14 컨트롤러 하드웨어 및 소프트웨어 수정함.
    double dFloatAVDD = iIntAVDD/20.0 ; //미리 암페어 단위. *50/1000
    double dFloatCVDD = iIntCVDD/20.0 ;
    double dFloatHVDD = iIntHVDD/20.0 ;
    double dFloatDVDD = iIntDVDD/20.0 ;
    */

    //20141209 문실장님 요청으로 배율 없앴음. 단위는 미리 암페어.
    double dFloatAVDD = iIntAVDD ; //미리 암페어 단위. *50/1000
    double dFloatCVDD = iIntCVDD ;
    double dFloatHVDD = iIntHVDD ;
    double dFloatDVDD = iIntDVDD ;



    double dOSTOffset ;
    double dOSTGain  ;
    //if(_iPockNo == 0) {dOSTOffset = OM.MstOptn.dOSTOffset1 ; dOSTGain = OM.MstOptn.dOSTGain1 ; }
    //if(_iPockNo == 1) {dOSTOffset = OM.MstOptn.dOSTOffset2 ; dOSTGain = OM.MstOptn.dOSTGain2 ; }
    //if(_iPockNo == 2) {dOSTOffset = OM.MstOptn.dOSTOffset3 ; dOSTGain = OM.MstOptn.dOSTGain3 ; }
    //if(_iPockNo == 3) {dOSTOffset = OM.MstOptn.dOSTOffset4 ; dOSTGain = OM.MstOptn.dOSTGain4 ; }

    //20141216 대겸프로브에서 오프셑이 스테이지별,포켓별로 발생해서 추가. 선
    dOSTOffset = OM.DevOptn.dAVDD_IDDOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dAVDD_IDDGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltAVDD_IDD = dFloatAVDD * dOSTGain + dOSTOffset ;

    dOSTOffset = OM.DevOptn.dCVDD_IDDOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dCVDD_IDDGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltCVDD_IDD = dFloatCVDD * dOSTGain + dOSTOffset ;

    dOSTOffset = OM.DevOptn.dHVDD_IDDOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dHVDD_IDDGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltHVDD_IDD = dFloatHVDD * dOSTGain + dOSTOffset ;

    dOSTOffset = OM.DevOptn.dDVDD_IDDOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dDVDD_IDDGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltDVDD_IDD = dFloatDVDD * dOSTGain + dOSTOffset ;
    if(OM.DevOptn.iOSTDeviceNo == 2) /*1099일경우 DVDD연결않되어있음.*/ OSTRslt[_iPockNo].dRsltDVDD_IDD = 0 ;


    if(OSTRslt[_iPockNo].dRsltAVDD_IDD < OM.DevOptn.dIDD_AVDDOkMin || OM.DevOptn.dIDD_AVDDOkMax < OSTRslt[_iPockNo].dRsltAVDD_IDD) _bRslt = true ;
    if(OSTRslt[_iPockNo].dRsltCVDD_IDD < OM.DevOptn.dIDD_CVDDOkMin || OM.DevOptn.dIDD_CVDDOkMax < OSTRslt[_iPockNo].dRsltCVDD_IDD) _bRslt = true ;
    if(OSTRslt[_iPockNo].dRsltHVDD_IDD < OM.DevOptn.dIDD_HVDDOkMin || OM.DevOptn.dIDD_HVDDOkMax < OSTRslt[_iPockNo].dRsltHVDD_IDD) _bRslt = true ;
    if(OSTRslt[_iPockNo].dRsltDVDD_IDD < OM.DevOptn.dIDD_DVDDOkMin || OM.DevOptn.dIDD_DVDDOkMax < OSTRslt[_iPockNo].dRsltDVDD_IDD) _bRslt = true ;

    return true ;
}

bool CStageOST::GetIDSFail(int _iPockNo , String _sRcvMsg , bool &_bRslt)//대기 전류 테스트.
{
    String sPockRslt ;
    String sPockID   ;

    _bRslt = false ;

    if(_iPockNo < 0) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}
    if(_iPockNo > 3) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}

    sPockID = "]IDS" + String(_iPockNo+1) ;

    int iTemp = _sRcvMsg.Pos("#") ;
    if(!iTemp) {
        Trace("Can't Find End# " , _sRcvMsg.c_str());
        return false ;
    }

    iTemp = _sRcvMsg.Pos(sPockID) ;
    if(!iTemp) {
        Trace("Can't Find Pock ID" , _sRcvMsg.c_str());
        return false ;
    }

    sPockRslt = _sRcvMsg.Delete(1, iTemp);  //쓸데이터의 앞쪽 데이터 짜르기
    sPockRslt = sPockRslt.Delete(1,sPockID.Length()); //쓸데이터 해더 짜르기.
    iTemp = sPockRslt.Pos("\r\n") ;
    if(!iTemp) {
        Trace("Can't Find CR" , sPockRslt.c_str());
        return false ;
    }
    sPockRslt = sPockRslt.Delete(iTemp ,sPockRslt.Pos("#"));

    //4자리컴마로 아이템 구분함.
    String sRsltAVDD = sPockRslt.SubString(1 ,4 );
    String sRsltCVDD = sPockRslt.SubString(6 ,4 );
    String sRsltHVDD = sPockRslt.SubString(11,4 );
    String sRsltDVDD = sPockRslt.SubString(16,4 );


    int iIntAVDD = StrToIntDef(sRsltAVDD , -1) ;
    int iIntCVDD = StrToIntDef(sRsltCVDD , -1) ;
    int iIntHVDD = StrToIntDef(sRsltHVDD , -1) ;
    int iIntDVDD = StrToIntDef(sRsltDVDD , -1) ;

    if(iIntAVDD == -1) {Trace("Wrong Pock No" , sRsltAVDD.c_str()); return false ; }
    if(iIntCVDD == -1) {Trace("Wrong Pock No" , sRsltCVDD.c_str()); return false ; }
    if(iIntHVDD == -1) {Trace("Wrong Pock No" , sRsltHVDD.c_str()); return false ; }
    if(iIntDVDD == -1) {Trace("Wrong Pock No" , sRsltDVDD.c_str()); return false ; }

    //double dFloatAVDD = iIntAVDD * 1000 /92.0 ; //마이크로 암페어 단위. 92는 대겸 컨트롤러 분해능에 따라 바뀐다.
    //double dFloatCVDD = iIntCVDD * 1000 /92.0 ;
    //double dFloatHVDD = iIntHVDD * 1000 /92.0 ;
    //double dFloatDVDD = iIntDVDD * 1000 /92.0 ;

    /*
    //2014.8.14 컨트롤러 하드웨어 및 펌웨어 수정.
    double dFloatAVDD = iIntAVDD * 5.0; //마이크로 암페어 단위.    *50
    double dFloatCVDD = iIntCVDD * 5.0;
    double dFloatHVDD = iIntHVDD * 5.0;
    double dFloatDVDD = iIntDVDD * 5.0;
    */

    //20141209 문실장님 요청으로 배율 없앴음. 단위는 마이크로 암페어.
    double dFloatAVDD = iIntAVDD ; //마이크로 암페어 단위.    *50
    double dFloatCVDD = iIntCVDD ;
    double dFloatHVDD = iIntHVDD ;
    double dFloatDVDD = iIntDVDD ;


    double dOSTOffset ;
    double dOSTGain ;
    //if(_iPockNo == 0) {dOSTOffset = OM.MstOptn.dOSTOffset1 ; dOSTGain = OM.MstOptn.dOSTGain1 ; }
    //if(_iPockNo == 1) {dOSTOffset = OM.MstOptn.dOSTOffset2 ; dOSTGain = OM.MstOptn.dOSTGain2 ; }
    //if(_iPockNo == 2) {dOSTOffset = OM.MstOptn.dOSTOffset3 ; dOSTGain = OM.MstOptn.dOSTGain3 ; }
    //if(_iPockNo == 3) {dOSTOffset = OM.MstOptn.dOSTOffset4 ; dOSTGain = OM.MstOptn.dOSTGain4 ; }
    //20141216 대겸프로브에서 오프셑이 스테이지별,포켓별로 발생해서 추가. 선
    dOSTOffset = OM.DevOptn.dAVDD_IDSOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dAVDD_IDSGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltAVDD_IDS = dFloatAVDD * dOSTGain + dOSTOffset ;

    dOSTOffset = OM.DevOptn.dCVDD_IDSOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dCVDD_IDSGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltCVDD_IDS = dFloatCVDD * dOSTGain + dOSTOffset ;

    dOSTOffset = OM.DevOptn.dHVDD_IDSOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dHVDD_IDSGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltHVDD_IDS = dFloatHVDD * dOSTGain + dOSTOffset ;


    dOSTOffset = OM.DevOptn.dDVDD_IDSOfst[_iPockNo] ;
    dOSTGain   = OM.DevOptn.dDVDD_IDSGain[_iPockNo] ;
    OSTRslt[_iPockNo].dRsltDVDD_IDS = dFloatDVDD * dOSTGain + dOSTOffset ;
    if(OM.DevOptn.iOSTDeviceNo == 2) /*1099일경우 DVDD연결않되어있음. 나중에 자제 추이 보고 A,C,H,D VDD 스킵옵션 넣자.*/ OSTRslt[_iPockNo].dRsltDVDD_IDS = 0 ;

    if(OSTRslt[_iPockNo].dRsltAVDD_IDS < OM.DevOptn.dIDS_AVDDOkMin || OM.DevOptn.dIDS_AVDDOkMax < OSTRslt[_iPockNo].dRsltAVDD_IDS) _bRslt = true ;
    if(OSTRslt[_iPockNo].dRsltCVDD_IDS < OM.DevOptn.dIDS_CVDDOkMin || OM.DevOptn.dIDS_CVDDOkMax < OSTRslt[_iPockNo].dRsltCVDD_IDS) _bRslt = true ;
    if(OSTRslt[_iPockNo].dRsltHVDD_IDS < OM.DevOptn.dIDS_HVDDOkMin || OM.DevOptn.dIDS_HVDDOkMax < OSTRslt[_iPockNo].dRsltHVDD_IDS) _bRslt = true ;
    if(OSTRslt[_iPockNo].dRsltDVDD_IDS < OM.DevOptn.dIDS_DVDDOkMin || OM.DevOptn.dIDS_DVDDOkMax < OSTRslt[_iPockNo].dRsltDVDD_IDS) _bRslt = true ;

    return true ;

}

//타임 아웃 의 진상규명... 범인 찾기.
bool CStageOST::GetI2CFail(int _iPockNo , String _sRcvMsg , bool &_bRslt)//대기 전류 테스트.
{
    String sPockRslt ;
    String sPockID   ;

    _bRslt = false ;

    if(_iPockNo < 0) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}
    if(_iPockNo > 3) {Trace("Wrong Pock No" , String(_iPockNo).c_str()); return false ;}


    //20141216 대겸프로브에서 오프셑이 스테이지별,포켓별로 발생해서 추가. 선

    if(OSTRslt[_iPockNo].dRsltAVDD_IDS == OM.DevOptn.dAVDD_IDSOfst[_iPockNo] &&
       OSTRslt[_iPockNo].dRsltCVDD_IDS == OM.DevOptn.dCVDD_IDSOfst[_iPockNo] &&
       OSTRslt[_iPockNo].dRsltAVDD_IDD == OM.DevOptn.dAVDD_IDDOfst[_iPockNo] &&
       OSTRslt[_iPockNo].dRsltCVDD_IDD == OM.DevOptn.dCVDD_IDDOfst[_iPockNo] ){
        _bRslt = true ;
    }
    return true ;
}

void CStageOST::Close()
{
    delete m_pRs232_OST ;

    Load(false);
}

void CStageOST::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CStageOST::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    if(DM.ARAY[riSTL].FindFrstColRow(csUnkwn , r,c)){
        return true ;
    }
    else {
        r=0 ;
        c=0 ;
        return false ;
    }

}

double CStageOST::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    //int    iR,iC ;
    //FindChip(iR ,iC);

    //double dWorkPos ;
    //dWorkPos = PM.GetValue(miLDR_YIndx , pvLDR_YIndxWorkStt) ;
    //dWorkPos = dWorkPos - (OM.DevInfo.dRowPitch * iR) ;

    //if(_iMotr == miSRT_ZElev){
    //    switch(_iPstnId) {
    //        default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
    //        case piLDR_YIndxWait     : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWait      ); break ;
    //        case piLDR_YIndxGet      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxGet       ); break ;
    //        case piLDR_YIndxWorkStt  : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWorkStt   ); break ;
    //        case piLDR_YIndxOut      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxOut       ); break ;
    //        case piLDR_YIndxWork     : dPos = dWorkPos                                    ; break ;

    //    }
    //}

    //else {
        dPos = MT_GetCmdPos(_iMotr);
    //}

    return dPos ;
}

void CStageOST::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    String sMsg ;
    if(EM_GetLastErr() == eiOST_TimeOut) {
        sMsg = "[RESET#";
        m_pRs232_OST -> sData = "" ;
        m_pRs232_OST -> SendData(sMsg.Length() , sMsg.c_str()) ;
    }
}

bool CStageOST::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStageOST::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CStageOST::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: MoveActr(aiOST_PshrDnUp , ccBwd);


                 Step.iToStart++ ;
                 return false ;

        case 11: if(!MoveActr(aiOST_PshrDnUp , ccBwd))return false;
                 MoveActr(aiOST_PTolUpDn , ccBwd);
                 Step.iToStart++ ;
                 return false ;

        case 12: if(!MoveActr(aiOST_PTolUpDn , ccBwd))return false;

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStageOST::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: MoveActr(aiOST_PshrDnUp , ccBwd);


                 Step.iToStop++ ;
                 return false ;

        case 11: if(!MoveActr(aiOST_PshrDnUp , ccBwd))return false;
                 MoveActr(aiOST_PTolUpDn , ccBwd);
                 Step.iToStop++ ;
                 return false ;

        case 12: if(!MoveActr(aiOST_PTolUpDn , ccBwd))return false;

                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CStageOST::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleWork = ScmGetCntStat(riSTL,csUnkwn) && MT_GetStopInpos(miSTG_TTble) && STG.GetSeqStep() == CStage::scIdle;
        bool isCycleEnd  = ScmCheckAllStat(riSTL,csEmpty);

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CStageOST::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: AT_MoveCyl(aiOST_PshrDnUp , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiOST_PshrDnUp , ccBwd)) return false;
                 AT_MoveCyl(aiOST_PTolUpDn , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 12: if(!AT_MoveCyl(aiOST_PTolUpDn , ccBwd)) return false;

                 Step.iHome = 0;
                 return true ;


    }
}


//One Cycle.
bool CStageOST::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );

        if(Step.iCycle == 14) {
            EM_SetErr(eiOST_TimeOut); //EM_SetErr(eiLDR_CycleTO);
        }
        else {
            EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        }

        MoveActr(aiOST_PshrDnUp , ccBwd);
        MoveActr(aiOST_PTolUpDn , ccBwd);


        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}
    EN_ARAY_ID riTarget ;
    //SubStat Index 
    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;
    bool r1,r2 ;
    int iStat;
    int iTemp ;

    bool bSkipInsp = OM.CmnOptn.bSkipOST;
    const bool bMotionSkipOnInspSkip = true ; //스킵시에 모션도 스킵 하는것 .. 셑업할때 드라이런 써야 되서 한다.

    //매뉴얼 동작 일때는 엠티 안따짐.
    bool bNeedInsp[MAX_TOOL_SOCK_COL];
    bNeedInsp[0] = (DM.ARAY[riSTL].GetStat(0,0)!=csEmpty ||GetSeqStep()==scIdle) && !OM.CmnOptn.bSkip1stTol ;
    bNeedInsp[1] = (DM.ARAY[riSTL].GetStat(1,0)!=csEmpty ||GetSeqStep()==scIdle) && !OM.CmnOptn.bSkip2ndTol ;
    bNeedInsp[2] = (DM.ARAY[riSTL].GetStat(2,0)!=csEmpty ||GetSeqStep()==scIdle) && !OM.CmnOptn.bSkip3thTol ;
    bNeedInsp[3] = (DM.ARAY[riSTL].GetStat(3,0)!=csEmpty ||GetSeqStep()==scIdle) && !OM.CmnOptn.bSkip4thTol ;

    bool bRet;
    EN_CHIP_STAT csStat ;
    String sMsg ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(bMotionSkipOnInspSkip && bSkipInsp) {
                     for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                         iTemp = rand()%100;
                         if(iTemp < OM.MstOptn.iInspRandMask) {
                             iStat = iTemp % 2 ? OM.CmnOptn.iOSTOSTBin  + 1 : OM.CmnOptn.iOSTCrnBin  + 1 ;//콤보박스는 0부터시작 Rslt는 1부터 시작.
                         }
                         else {
                             iStat = csGood;
                         }
                         //if(ScmGetSckStat(ScmGetLeftSTGAray(),0,c)!=csEmpty) ScmSetSckStat(ScmGetLeftSTGAray() , 0,c , (EN_CHIP_STAT)iStat);
                         if(DM.ARAY[riSTL].GetStat(r,0)!=csEmpty)DM.ARAY[riSTL].SetStat(r,0, (EN_CHIP_STAT)iStat);
                     }

                     Step.iCycle=0;
                     return true ;
                  }

                  //if(!bSkipInsp){
                  //    m_pRs232_OST -> sData = "" ;
                  //    sMsg = "[DVC,"+AnsiString(OM.DevOptn.iOSTDeviceNo)+"#" ;
                  //    m_pRs232_OST -> SendData(sMsg.Length(),sMsg.c_str());
                  //}



                  MoveActr(aiOST_PshrDnUp , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  11: if(!MoveActr(aiOST_PshrDnUp , ccFwd)) return false;
                  MoveActr(aiOST_PTolUpDn , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveActr(aiOST_PTolUpDn , ccFwd)) return false;
                  m_tmDelay.Clear();



                  Step.iCycle++ ;
                  return false ;

        case  13: if(!m_tmDelay.OnDelay(true , 100)) return false ; //안정화 시간.
                  if(!bSkipInsp){
                      m_pRs232_OST -> sData = "" ;
                      m_pRs232_OST -> SendData(String("[SRT#").Length() , "[SRT#");
                  }
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        //타임아웃 조건에서 사용함.
        case  14: if(m_tmDelay.OnDelay(true , OM.DevOptn.iOST_I2cFailTime)){ //I2c 핀 페일일 경우 컨텍을 해지 하지 않으면 결과값이 안날라옴.
                      MoveActr(aiOST_PTolUpDn , ccBwd);
                  }


                  if(!bSkipInsp && m_pRs232_OST -> sData == "") return false ;







                  // RS232 통신 결과 도착 확인.
                  MoveActr(aiOST_PTolUpDn , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiOST_PTolUpDn , ccBwd))return false;
                  MoveActr(aiOST_PshrDnUp , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveActr(aiOST_PshrDnUp , ccBwd))return false;
                  //일단 OST 결과 모두 클리어 하고.
                  InitOstRslt();


                  if(!bSkipInsp){
                       for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                           if(bNeedInsp[r]){
                               DM.ARAY[riMOS].SetStat(r,0,csGood);
                               if(GetOSTFail(r , m_pRs232_OST -> sData , bRet)){
                                   if(bRet){ //1개라도 뻬일이 있으면 페일처리하고 OST 검사 안하기 때문에
                                             //원래는 오프셑 먹어서 1ma 이런식이였는데 오프셑 게인 적용 없이 그냥 0.0으로 모두 쎄팅 하도록.. 수정. 20140105테라셈 요청.
                                       DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                                   }
                                   else {
                                       if(GetIDDFail(r , m_pRs232_OST -> sData , bRet)){
                                           if(bRet&&!OM.CmnOptn.bOSTNotUseCrnt)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                                       }
                                       else {
                                           EM_SetErrMsg(eiOST_WrongData , ("IDD"+String(r)).c_str());
                                       }
                                       if(GetIDSFail(r , m_pRs232_OST -> sData , bRet)){
                                           if(bRet&&!OM.CmnOptn.bOSTNotUseCrnt)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                                       }
                                       else {
                                           EM_SetErrMsg(eiOST_WrongData , ("IDS"+String(r)).c_str());
                                       }
                                       if(GetI2CFail(r , m_pRs232_OST -> sData , bRet)){
                                           if(bRet)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTCrnBin +1));
                                       }
                                       else {
                                           EM_SetErrMsg(eiOST_WrongData , ("I2C"+String(r)).c_str());
                                       }
                                   }
                               }
                               else {
                                   EM_SetErrMsg(eiOST_WrongData , ("OST"+String(r)).c_str());
                               }

                           }
                       }
                  }
                  else{
                       for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                           iTemp = rand()%100;
                           if(iTemp < OM.MstOptn.iInspRandMask) {
                               iStat = iTemp % 2 ? OM.CmnOptn.iOSTOSTBin  + 1 : OM.CmnOptn.iOSTCrnBin  + 1 ;//콤보박스는 0부터시작 Rslt는 1부터 시작.
                           }
                           else {
                               iStat = csGood;
                           }
                           DM.ARAY[riMOS].SetStat(r,0, (EN_CHIP_STAT)iStat);
                       }
                  }

                  WriteIDD_IDStoCSV(LOT.GetLotNo()+"_"+OM.GetCrntDev()+ ".csv");

                  for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                      csStat = DM.ARAY[riMOS].GetStat(r,0);
                      if(GetSeqStep() != scIdle) {
                          if(DM.ARAY[riSTL].GetStat(r,0)!=csEmpty) DM.ARAY[riSTL].SetStat(r,0,(EN_CHIP_STAT)csStat);
                      }
                  }


                  if(GetSeqStep() != scIdle) {  //메뉴얼로 세팅할때 자꾸 스테이지 돌려 놓고 검사 하면 삑사리 나서...
                      if(DM.ARAY[riSTL].GetStat(0,0) != csEmpty) { //자제 있는 놈만.
                               if(DM.ARAY[riSTL].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTOSTBin +1) m_iOstNGCntPock1++ ;
                          else if(DM.ARAY[riSTL].GetStat(0,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTCrnBin +1) m_iOstNGCntPock1++ ;
                          else                                                                           m_iOstNGCntPock1=0 ;

                      }
                      if(DM.ARAY[riSTL].GetStat(1,0) != csEmpty) {
                               if(DM.ARAY[riSTL].GetStat(1,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTOSTBin +1) m_iOstNGCntPock2++ ;
                          else if(DM.ARAY[riSTL].GetStat(1,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTCrnBin +1) m_iOstNGCntPock2++ ;
                          else                                                                           m_iOstNGCntPock2=0 ;
                      }
                      if(DM.ARAY[riSTL].GetStat(2,0) != csEmpty) {
                               if(DM.ARAY[riSTL].GetStat(2,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTOSTBin +1) m_iOstNGCntPock3++ ;
                          else if(DM.ARAY[riSTL].GetStat(2,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTCrnBin +1) m_iOstNGCntPock3++ ;
                          else                                                                           m_iOstNGCntPock3=0 ;

                      }
                      if(DM.ARAY[riSTL].GetStat(3,0) != csEmpty) {
                               if(DM.ARAY[riSTL].GetStat(3,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTOSTBin +1) m_iOstNGCntPock4++ ;
                          else if(DM.ARAY[riSTL].GetStat(3,0) == (EN_CHIP_STAT)OM.CmnOptn.iOSTCrnBin +1) m_iOstNGCntPock4++ ;
                          else                                                                           m_iOstNGCntPock4=0 ;

                      }


                      if(OM.CmnOptn.iOSContNGCnt != 0){ //0으로 설정 되어 있으면 사용 안함.
                          sTemp = "";
                          if(OM.CmnOptn.iOSContNGCnt<=m_iOstNGCntPock1){
                              if(sTemp=="") sTemp =  "1";
                              else          sTemp+= ",1";
                          }
                          if(OM.CmnOptn.iOSContNGCnt<=m_iOstNGCntPock2){
                              if(sTemp=="") sTemp =  "2";
                              else          sTemp+= ",2";
                          }
                          if(OM.CmnOptn.iOSContNGCnt<=m_iOstNGCntPock3){
                              if(sTemp=="") sTemp =  "3";
                              else          sTemp+= ",3";
                          }
                          if(OM.CmnOptn.iOSContNGCnt<=m_iOstNGCntPock4){
                              if(sTemp=="") sTemp =  "4";
                              else          sTemp+= ",4";
                          }
                          if(sTemp != "") { //만약 한포켓이라도 메세지가 세팅이 되었으면.
                              Trace("m_iOstNGCntPock",(String(m_iOstNGCntPock1) + "_" + String(m_iOstNGCntPock2) + "_" + String(m_iOstNGCntPock3) + "_" + String(m_iOstNGCntPock4)).c_str() );
                              m_iOstNGCntPock1 = 0 ;
                              m_iOstNGCntPock2 = 0 ;
                              m_iOstNGCntPock3 = 0 ;
                              m_iOstNGCntPock4 = 0 ;
                              EM_SetErrMsg(eiOST_SerialNG , (sTemp + "번포켓 OST 불량 연속").c_str());
                          }
                      }
                 }















                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CStageOST::CycleTest(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 12000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}
    EN_ARAY_ID riTarget ;
    //SubStat Index 
    const int iOSTRslt = 0 ;
    const int iVTRslt  = 1 ;
    const int iVSNRslt = 2 ;
    bool r1,r2 ;
    int iStat;
    int iTemp ;

    bool bSkipInsp = OM.CmnOptn.bSkipOST;
    const bool bMotionSkipOnInspSkip = true ; //스킵시에 모션도 스킵 하는것 .. 셑업할때 드라이런 써야 되서 한다.

    //매뉴얼 동작 일때는 엠티 안따짐.
    bool bNeedInsp[MAX_TOOL_SOCK_COL];
    bNeedInsp[0] = !OM.CmnOptn.bSkip1stTol ;
    bNeedInsp[1] = !OM.CmnOptn.bSkip2ndTol ;
    bNeedInsp[2] = !OM.CmnOptn.bSkip3thTol ;
    bNeedInsp[3] = !OM.CmnOptn.bSkip4thTol ;

    bool bRet;
    EN_CHIP_STAT csStat ;

    String sTemp22 ,sMsg;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //m_pRs232_OST -> sData = "" ;
                  //sMsg = "[DVC,"+AnsiString(OM.DevOptn.iOSTDeviceNo)+"#" ;
                  //m_pRs232_OST -> SendData(sMsg.Length(),sMsg.c_str());
                  //m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  11: //if(!m_tmDelay.OnDelay(true ,300)) return false ;
                  m_pRs232_OST -> sData = "" ;
                  sTemp22 = m_pRs232_OST -> sData ;
                  m_pRs232_OST -> SendData(String("[SRT#").Length() , "[SRT#");
                  Step.iCycle++;
                  return false ;

        case  12: sTemp22 = m_pRs232_OST -> sData ;
                  if(m_pRs232_OST -> sData == "") return false ;

                  //일단 OST 결과 모두 클리어 하고.
                  InitOstRslt();


                  for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                      if(bNeedInsp[r]){
                          DM.ARAY[riMOS].SetStat(r,0,csGood);
                          if(GetOSTFail(r , m_pRs232_OST -> sData , bRet)){
                              if(bRet)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                              else {
                                  if(GetIDDFail(r , m_pRs232_OST -> sData , bRet)){
                                      if(bRet&&!OM.CmnOptn.bOSTNotUseCrnt)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                                  }
                                  else {
                                      EM_SetErrMsg(eiOST_WrongData , ("IDD"+String(r)).c_str());
                                  }
                                  if(GetIDSFail(r , m_pRs232_OST -> sData , bRet)){
                                      if(bRet&&!OM.CmnOptn.bOSTNotUseCrnt)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                                  }
                                  else {
                                      EM_SetErrMsg(eiOST_WrongData , ("IDS"+String(r)).c_str());
                                  }
                                  if(GetI2CFail(r , m_pRs232_OST -> sData , bRet)){
                                      if(bRet)DM.ARAY[riMOS].SetStat(r,0,(EN_CHIP_STAT)(OM.CmnOptn.iOSTOSTBin +1));
                                  }
                                  else {
                                      EM_SetErrMsg(eiOST_WrongData , ("I2C"+String(r)).c_str());
                                  }
                              }
                          }
                          else {
                              EM_SetErrMsg(eiOST_WrongData , ("OST"+String(r)).c_str());
                          }

                      }
                  }

                  WriteIDD_IDStoCSV(LOT.GetLotNo()+"_"+OM.GetCrntDev()+ ".csv");

                  for(int r = 0 ; r < MAX_TOOL_SOCK_COL ; r++){
                      csStat = DM.ARAY[riMOS].GetStat(r,0);
                      if(GetSeqStep() != scIdle) {
                          if(DM.ARAY[riSTL].GetStat(r,0)!=csEmpty) DM.ARAY[riSTL].SetStat(r,0,(EN_CHIP_STAT)csStat);
                      }
                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//---------------------------------------------------------------------------
bool CStageOST::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    //bool bExistPRL   = DM.ARAY[riPRL].GetCntExist() ;
    //bool bExistPSL   = DM.ARAY[riPSL].GetCntExist() ;
    //bool bExistLDR   = DM.ARAY[riLDR].GetCntExist() ;
    //bool bIndxFwd    = !AT_Complete(aiLDR_IndxUpUp , ccBwd);










    //bool bUnderWorkPos = PM.GetValue(miLDR_YIndx , pvLDR_YIndxWorkStt)+10.0 >= MT_GetCmdPos(miLDR_YIndx) ;

    //if(_iMotr == miLDR_YIndx){
    //    if(bIndxFwd) {
    //        switch(_iPstnId) {
    //            default                   :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
    //            case piLDR_YIndxWait      : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
    //            case piLDR_YIndxGet       : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
    //            case piLDR_YIndxWorkStt   :                                                                                                                 break ;
    //            case piLDR_YIndxOut       : if(bExistPSL     && bExistLDR )  { sMsg = AnsiString(DM.ARAY[riPSL].GetName())+" is Exist"    ; bRet = false ;} break ;
    //            case piLDR_YIndxWork      :                                                                                                                 break ;
    //
    //        }
    //    }
    //}
    //
    //else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    //}

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CStageOST::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool bRet = true ;

    bool bStgMtrStop = MT_GetStop(miSTG_TTble) ;

    double dStgPos = MT_GetCmdPos(miSTG_TTble);
    int    iAngle  = dStgPos / 90 ;
    double dGap    = dStgPos - (iAngle * 90);
    double dTemp   = PM.GetValue(miSTG_TTble , pvSTG_TTbleWork) ;   //STG.GetMotrPos(miSTG_TTble , piSTG_TTbleWork) ;  //이곳은 오프셑 먹은게 기준이면 안될듯 하다.
    bool   bInpos  = dTemp - 0.5 < dGap && dGap < 0.5 + dTemp ;


    //bool bStgMtrWorkZone = MT_GetCmdPos(miSTG_TTble) % 90 && MT_GetStop(miSTG_TTble)


    if(_iActr == aiOST_PshrDnUp){
        if(_bFwd == ccFwd) {
            if(!bStgMtrStop) {sMsg = AnsiString("Stage Motor is Running!"); bRet = false ;}
            if(!bInpos     ) {sMsg = AnsiString("Stage Motor is Not Safe Position!"); bRet = false ;}
        }
    }
    else if(_iActr == aiOST_PTolUpDn){
        if(_bFwd == ccFwd) {
            if(!bStgMtrStop) {sMsg = AnsiString("Stage Motor is Running!"); bRet = false ;}
            if(!bInpos     ) {sMsg = AnsiString("Stage Motor is Not Safe Position!"); bRet = false ;}
        }
    }
    else {
      sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
      bRet = false ;
    }


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CStageOST::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CStageOST::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CStageOST::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CStageOST::SetLastCmd()
{
    //m_dLastIdxPos = MT_GetCmdPos(miLDR_YIndx);
    return ; //이파트는 필요 없다.

}

bool CStageOST::CheckMoved()
{
    /*
    if(m_dLastIdxPos != MT_GetCmdPos(miLDR_YIndx)) return true  ; //이파트는 필요 없다.
    else                                           return false ;
    */
    return true;
}

bool CStageOST::CheckStop()
{
    //if(!MT_GetStop(miLDR_YIndx)) return false ;

    if(!AT_Done(aiOST_PshrDnUp)) return false ;
    if(!AT_Done(aiOST_PTolUpDn)) return false ;

    return true ;
}

void CStageOST::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}


AnsiString SetTextCnt(AnsiString _sText , int _iCnt)
{
    AnsiString sRet ;
    int        iGap ;

    if(_sText.Length() < _iCnt) {
        iGap = _iCnt - _sText.Length() ;
        sRet = _sText ;
        for(int i = 0 ; i <  iGap ; i++) {
            sRet = " " + sRet ;
        }
    }
    else if(_sText.Length() > _iCnt) {
        sRet = _sText.SubString(1,_iCnt);
    }
    else {
        sRet = _sText ;
    }

    return sRet ;



}

bool CStageOST::WriteIDD_IDStoCSV(AnsiString _sFileName)
{
    //Set Data.
    AnsiString sData = "";
    int hFile;

    if(!DirectoryExists(OST_FOLDER)) UserFile.CreateDir(OST_FOLDER);

    UserFile.ClearDirDate(OST_FOLDER , Now() - 7); //7days.

    AnsiString sFilePathName = OST_FOLDER + _sFileName ;
    AnsiString sBlank = "" ;
    AnsiString sOSTRslt = "";

    for(int i=0; i <MAX_OST_PIN_CNT-2 ; i++){
        sBlank += " ";
    }


    hFile = FileOpen(sFilePathName.c_str() , fmOpenWrite);
    if (hFile == -1) {
        hFile = FileCreate(sFilePathName.c_str());
        if (hFile == -1) {
            //ShowMessage("Could not make "+ sFilePathName);
            return false;
        }
        sData  = "Time    ,OST1" + sBlank + ",IDD_AVDD1,IDD_CVDD1,IDD_HVDD1,IDD_DVDD1,IDS_AVDD1,IDS_CVDD1,IDS_HVDD1,IDS_DVDD1," ;
        sData +=          "OST2" + sBlank + ",IDD_AVDD2,IDD_CVDD2,IDD_HVDD2,IDD_DVDD2,IDS_AVDD2,IDS_CVDD2,IDS_HVDD2,IDS_DVDD2," ;
        sData +=          "OST3" + sBlank + ",IDD_AVDD3,IDD_CVDD3,IDD_HVDD3,IDD_DVDD3,IDS_AVDD3,IDS_CVDD3,IDS_HVDD3,IDS_DVDD3," ;
        sData +=          "OST4" + sBlank + ",IDD_AVDD4,IDD_CVDD4,IDD_HVDD4,IDD_DVDD4,IDS_AVDD4,IDS_CVDD4,IDS_HVDD4,IDS_DVDD4\r\n";
    }

    bool bOSTFail = false ;

    sData += Now().FormatString("hh:nn:ss") + "," ;
    for(int i = 0 ; i < MAX_TOOL_SOCK_COL ; i++) {
        bOSTFail = false ;
        sOSTRslt = "" ;
        for(int j = 0 ; j < MAX_OST_PIN_CNT ; j++) {
            if(OSTRslt[i].bRsltOSTFail[j]){sOSTRslt+="1" ; bOSTFail = true ;}
            else                          {sOSTRslt+="0" ;                  }
        }
        sData += bOSTFail ? "NG" : "OK" ;
        sData += sOSTRslt +",";
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltAVDD_IDD),9) + "," ;
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltCVDD_IDD),9) + "," ;
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltHVDD_IDD),9) + "," ;
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltDVDD_IDD),9) + "," ;

        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltAVDD_IDS),9) + "," ;
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltCVDD_IDS),9) + "," ;
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltHVDD_IDS),9) + "," ;
        sData += SetTextCnt(AnsiString(OSTRslt[i].dRsltDVDD_IDS),9)  ;
        sData += (i==MAX_TOOL_SOCK_COL-1) ? "\r\n" : "," ;
    }


    FileSeek (hFile , 0             , SEEK_END      );
    FileWrite(hFile , sData.c_str() , sData.Length());

    //Close File.
    FileClose(hFile);

    return true;
}


int CStageOST::GetCrntOSTStgNo()
{
    double dStgPos     = MT_GetCmdPos(miSTG_TTble);
    double dStgZeroPos = STG.GetMotrPos(miSTG_TTble , piSTG_TTbleWork) ;
    const double dMargin = 5.0 ; //마진은 5도만
    int    iRetStageNo ;
    //double dAngle  = (dStgPos - PM.GetValue(miSTG_TTble , pvSTG_TTbleWork));
    //double dAngle  = (dStgPos - STG.GetMotrPos(miSTG_TTble , piSTG_TTbleWork));

    //마진은 5도정도로.. 스테이지는 0,1,2,3 이고 스티커는 1,2,3,4로 붙어 있는데 배열 때문에 0,1,2,3 으로 쓴다.
    //홈잡았을때 장비 전면쪽에 있는 스테이지가 0번 ; 스티커는

    //0도 일때는 OST는 3번.
         if(dStgZeroPos+  0.0-dMargin < dStgPos && dStgPos < dStgZeroPos+  0.0+dMargin) iRetStageNo = 3 ;
    else if(dStgZeroPos+ 90.0-dMargin < dStgPos && dStgPos < dStgZeroPos+ 90.0+dMargin) iRetStageNo = 0 ;
    else if(dStgZeroPos+180.0-dMargin < dStgPos && dStgPos < dStgZeroPos+180.0+dMargin) iRetStageNo = 1 ;
    else if(dStgZeroPos+270.0-dMargin < dStgPos && dStgPos < dStgZeroPos+270.0+dMargin) iRetStageNo = 2 ;
    else   {Trace("StageAngle",("strange "+ String(dStgPos)).c_str());iRetStageNo = 0 ;}

    return iRetStageNo ;
}




