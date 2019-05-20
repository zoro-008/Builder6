//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgResult.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CResult_V01)
CResult_V01::CResult_V01()
{

}

CResult_V01::~CResult_V01()
{
}

void CResult_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}

String CResult_V01::GetRsltMsg()
{
    return Rslt.sResultMsg ;
}



//순수 가상 함수.
//==============================================================================
bool CResult_V01::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;
    Prop.bResult   = true  ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    //리절트는 트레커가 없으나 나중에 TEXTOUT용 라벨같은거 만들지도 모름.
    //CTracker * Tracker = new CTracker();
    //Tracker -> SetOnPaint(OnTrackerChanged);
    //m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());

    return true ;

}
bool CResult_V01::Close()
{
    int iTrackerCnt = m_lTrckInsp.GetDataCnt();
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        delete m_lTrckInsp.GetCrntData(!i);
    }
    m_lTrckInsp.DeleteAll();

    int iLParaCnt = m_lLPara.GetDataCnt();
    for(int i = 0 ; i < iLParaCnt ; i++) {
        delete m_lLPara.GetCrntData(!i);
    }
    m_lLPara.DeleteAll();

    return true ;
}


        //검사 및 러닝 관련.
bool CResult_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CResult_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CResult_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CResult_V01::Run(CImage * _pImg)
{
    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        return true ;
    }
    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.


    //벨류 테이블에서 기준값 오프셑 가져옴.
    String sResultMsg = CPara.sResultMsg ;

    int       iAddSttPos ;
    int       iAddEndPos ;
    String    sAdd       ;
    String    sVal       ;
    CValue2 * Val        ;

    while(sResultMsg.Pos(">")){
        iAddSttPos = sResultMsg.Pos("<") ;
        iAddEndPos = sResultMsg.Pos(">") ;
        sAdd = sResultMsg.SubString(iAddSttPos+1,(iAddEndPos-iAddSttPos-1));

        Val = m_pValueList -> GetValueByName(sAdd);
        if(Val){
                 if(Val->GetValType()=="double")  sVal = sVal.sprintf("%.5f", Val -> GetDouble());
            else if(Val->GetValType()=="int"   )  sVal = Val -> GetInt   ();
            else if(Val->GetValType()=="String")  sVal = Val -> GetString();
        }

        sResultMsg.Delete(iAddSttPos,iAddEndPos-iAddSttPos+1);
        sResultMsg.Insert(sVal      ,iAddSttPos             );
    }

    Rslt.sResultMsg = sResultMsg ;

    Rslt.bRsltOk  = true ;
    Rslt.bInspEnd = true ;

    return true ;

}

String CResult_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CResult_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc);

    Gdi -> m_tText  .Color  = clYellow ;
    Gdi -> m_tPen   .Color  = clYellow ;
    Gdi -> m_tBrush .Color  = clYellow ;
    Gdi -> m_tFont  .Height = 30       ;

    if(Rslt.bRsltOk){

        String sTemp1 = String("RsltMsg : ") + Rslt.sResultMsg ;

        Gdi -> Text(5, 35 ,sTemp1.c_str()) ;
    }

    else {
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }

    delete Gdi;
    

}




void CResult_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
bool CResult_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))bRet = true;
    }
    return bRet;
}


bool CResult_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)) bRet = true ;
    }
    return bRet ;
}

void CResult_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CResult_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &MPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        UpdateMasterPara(true , _vePara );
    }
}

void CResult_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &CPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &CPara.List);

        UpdateCommonPara(true , _vePara );
    }

}
void CResult_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

    }
    else {

        UpdateLocalPara(true , _vePara );
    }

}

        //로드 세이브.
void CResult_V01::LoadSave(bool _bLoad , String _sVisnName)
{
    String sIniPath = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_" + GetStaticPkgLink()->GetPkgName() + ".ini" ;

    TUserINI Ini ;

    /*
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 100 ;
    Tracker -> WidthMax  = 100 ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());
    */


    int iTrackerCnt ;
    CTracker* Tracker ;
    if(_bLoad) {
        //이파트는 무조건 1개만 되기 때문에 필요 없다.
        //m_lTrckInsp.DeleteAll();
        if(FileExists(sIniPath)){
            //Tracker ;
            //Tracker = m_lTrckInsp.GetData(0);
            //Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

            LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
            LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);
        }
        else {
            MPara.SetDefault();
            CPara.SetDefault();
        }

    }
    else {
        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;

        //Tracker ;
        //Tracker = m_lTrckInsp.GetData(0);
        //Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

        LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);


    }
}




