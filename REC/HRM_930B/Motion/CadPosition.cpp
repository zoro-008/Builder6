//---------------------------------------------------------------------------


#pragma hdrstop

#include "CadPosition.h"
#include "BaseMaths.h"
#include "OptionMan.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)


const int iMargin = 3 ;


CCadObject * CCadObject::m_pLastObject[MAX_COORDINATION] = {NULL} ;

bool GetPointIn(double _dPntX , double _dPntY, double _dTrgX , double _dTrgY , double _dMargin  )
{
    if(_dPntX-_dMargin <= _dTrgX && _dTrgX <= _dPntX+_dMargin &&
       _dPntY-_dMargin <= _dTrgY && _dTrgY <= _dPntY+_dMargin ){
        return true ;
    }
    return false ;
}

bool GetLineIn(double _dLineX1 , double _dLineY1 , double _dLineX2 ,double _dLineY2 , double _dTrgX , double _dTrgY , double _dMargin ) //_aPoints 4개 포인트 배열 넘겨야함.
{
    double dAngle = CMath::GetLineAngle(_dLineX1 , _dLineY1 , _dLineX2 , _dLineY2);
    //struct TDPoint
    //{
    //    double x , y ;
    //    TDPoint() {x=0.0 ; y=0.0;}
    //    TDPoint(double _fX, double _fY) { x=_fX; y=_fY; }
    //    void Clear(){x=0.0;y=0.0;}
    //    TPoint GetTPoint(){TPoint Point ; Point.x = x ; Point.y = y ; return Point;}
    //};
    TDPoint aRectPnt[4] ;

    CMath::GetPntFromPntByAngLen(_dLineX1 , _dLineY1 ,                //입력 시작점.
                                 dAngle+90.0 ,                //입력 각도.
                                _dMargin     ,                //입력 길이.
                                &aRectPnt[0].x , &aRectPnt[0].y ); //출력 포인트.

    CMath::GetPntFromPntByAngLen(_dLineX2 , _dLineY2 ,                //입력 시작점.
                                 dAngle+90.0 ,                //입력 각도.
                                _dMargin     ,                //입력 길이.
                                &aRectPnt[1].x , &aRectPnt[1].y ); //출력 포인트.

    CMath::GetPntFromPntByAngLen(_dLineX2 , _dLineY2 ,                //입력 시작점.
                                 dAngle-90.0 ,                //입력 각도.
                                _dMargin     ,                //입력 길이.
                                &aRectPnt[2].x , &aRectPnt[2].y ); //출력 포인트.

    CMath::GetPntFromPntByAngLen(_dLineX1 , _dLineY1 ,                //입력 시작점.
                                 dAngle-90.0 ,                //입력 각도.
                                _dMargin     ,                //입력 길이.
                                &aRectPnt[3].x , &aRectPnt[3].y ); //출력 포인트.
    TDPoint tPnt ;
    tPnt.x = _dTrgX ;
    tPnt.y = _dTrgY ;
    if(CMath::IsPntInPolygon(aRectPnt , 4 , tPnt))return true ;
    return false ;
}

//CCadObject============================================
CCadObject * CCadObject::GetLastObject(EN_COORDINATION _eCood)
{
    return m_pLastObject[_eCood] ;
}

CCadObject * CCadObject::FindObjectByName(EN_COORDINATION _eCood , String _sName)
{
    CCadObject * Obj ;
    Obj = m_pLastObject[_eCood] ;
    int iCnt =0;
    while(Obj){
        if(Obj -> m_sName == _sName) {
            return Obj ;
        }
        Obj = Obj -> m_pPreObject ;
        iCnt++;
    }

    return NULL ;
}

String CCadObject::GetNameList(EN_COORDINATION _eCood)
{
    CCadObject * Obj;
    String sName = "";

    Obj = m_pLastObject[_eCood];
    while(Obj){
        if(Obj -> m_bEnabled){
            if(sName == "") sName = Obj -> m_sName;
            else            sName = Obj -> m_sName + "," + sName ;
        }
        Obj = Obj -> m_pPreObject ;
    }
    return sName;
}

//===============================================================================
CCadObject::CCadObject(EN_COORDINATION _eCood , String _sName)
{
    m_eColor   = clRed ;
    m_sName    = _sName ;

    m_eType    = ctObject ;

    m_bEnabled = true  ;
    m_bVisible = true  ;

    m_dOffsetX = 0.0   ;
    m_dOffsetY = 0.0   ;
    m_dOffsetZ = 0.0   ;

    m_dGainX = 1.0   ;
    m_dGainY = 1.0   ;
    m_dGainZ = 1.0   ;



    m_pPreObject = m_pLastObject[_eCood] ;
    //if(m_sNameList == "") m_sNameList += _sName ;
    //else                  m_sNameList += "," + _sName ;

    m_pLastObject[_eCood] = this ;

}

CCadObject::~CCadObject()
{

}

bool CCadObject::GetEnabled()
{
    return m_bEnabled;
}

void CCadObject::SetEnabled(bool _bVal)
{
    m_bEnabled = _bVal ;
}

bool CCadObject::GetVisible()
{
    return m_bVisible ;
}

void CCadObject::SetVisible(bool _bVal)
{
    m_bVisible = _bVal ;
}

void CCadObject::SetOffsetX(double _dVal)
{
    m_dOffsetX = _dVal ;
}

double CCadObject::GetOffsetX()
{
    return m_dOffsetX ;
}

void CCadObject::SetOffsetY(double _dVal)
{
    m_dOffsetY = _dVal ;
}

double CCadObject::GetOffsetY()
{
    return m_dOffsetY ;
}

void CCadObject::SetOffsetZ(double _dVal)
{
    m_dOffsetZ = _dVal ;
}

double CCadObject::GetOffsetZ()
{
    return m_dOffsetZ ;
}

void CCadObject::SetGainX (double _dVal)
{
    m_dGainX = _dVal ;
}

double CCadObject::GetGainX()
{
    return m_dGainX ;
}

void CCadObject::SetGainY (double _dVal)
{
    m_dGainY = _dVal ;
}

double CCadObject::GetGainY()
{
    return m_dGainY ;
}

void CCadObject::SetGainZ (double _dVal)
{
    m_dGainZ = _dVal ;
}

double CCadObject::GetGainZ()
{
    return m_dGainZ ;
}

CCadObject * CCadObject::GetPreObject()
{
    return m_pPreObject ;
}

String CCadObject::GetName()
{
    return m_sName ;
}
void CCadObject::SetName(String _sName)
{
    m_sName = _sName;
}

TColor CCadObject::GetColor()
{
    return m_eColor ;
}

void CCadObject::SetColor(TColor _eVal)
{
    m_eColor = _eVal ;
}

EN_CADOBJECT_TYPE CCadObject::GetType()
{
    return m_eType;

}

void CCadObject::LoadSave(bool _bLoad , String _sFilePath)
{
    TUserINI Ini ;

    int iColor ;
    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_eColor  " , iColor    ); m_eColor = (TColor)iColor ;
        //Ini.Load(_sFilePath , m_sName , "m_bEnabled" , m_bEnabled);
        //Ini.Load(_sFilePath , m_sName , "m_dOffsetX" , m_dOffsetX);
        //Ini.Load(_sFilePath , m_sName , "m_dOffsetY" , m_dOffsetY);
    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_eColor  " , (int)m_eColor);
        //Ini.Save(_sFilePath , m_sName , "m_bEnabled" , m_bEnabled);
        //Ini.Save(_sFilePath , m_sName , "m_dOffsetX" , m_dOffsetX);
        //Ini.Save(_sFilePath , m_sName , "m_dOffsetY" , m_dOffsetY);
    }

    if(m_eColor==0) m_eColor = clSilver ;

}

//CCadPoint============================================
CCadPoint::CCadPoint(EN_COORDINATION _eCood ,String _sName): CCadObject(_eCood ,_sName)
{
    m_eType    = ctPoint ;
    m_dPosX    = 0;
    m_dPosY    = 0;

}
CCadPoint::~CCadPoint()
{
}

//virtual.
void CCadPoint::SetCntrPosX(double _dVal)
{
    m_dPosX = _dVal ;
}

void CCadPoint::SetCntrPosY(double _dVal)
{
    m_dPosY = _dVal ;
}

double CCadPoint::GetCntrPosX(bool _bRaw)
{
    if(_bRaw) return m_dPosX ;
    return m_dPosX * m_dGainX + m_dOffsetX ;
}

double CCadPoint::GetCntrPosY(bool _bRaw)
{
    if(_bRaw) return m_dPosY ;
    return m_dPosY * m_dGainY + m_dOffsetY ;
}

void CCadPoint::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;
        //


    _pGdi -> m_tPen.Color = m_eColor ;
    _pGdi -> m_tText.Color = m_eColor ;
    _pGdi -> LinePlus(m_dPosX  , m_dPosY  , 5);
    //_pGdi -> Text()

}

bool CCadPoint::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    if(GetPointIn(m_dPosX , m_dPosY , _dX , _dY , _dMargin)){
        _dDist = CMath::GetLineLenth(m_dPosX , m_dPosY , _dX , _dY);
        return true ;
    }

    //if(GetLineIn(m_dPosX ,m_dPosY-iMargin , m_dPosX ,m_dPosY+iMargin , _dX , _dY , _dMargin)){
    //    return true ;
    //}
    //if(GetLineIn(m_dPosX-iMargin ,m_dPosY , m_dPosX+iMargin ,m_dPosY , _dX , _dY , _dMargin)){
    //    return true ;
    //}
    return false ;
}

bool CCadPoint::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    if(GetPointIn(m_dPosX , m_dPosY , _dX , _dY , _dMargin)){
        _dDist = CMath::GetLineLenth(m_dPosX , m_dPosY , _dX , _dY);
        _tInPnt.dPosX = m_dPosX ;
        _tInPnt.dPosY = m_dPosY ;
        return true ;
    }

    return false ;

}

void CCadPoint::LoadSave(bool _bLoad , String _sFilePath)
{
    CCadObject::LoadSave(_bLoad , _sFilePath);
    int iColor;
    TUserINI Ini ;

    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_dPosX " , m_dPosX );
        Ini.Load(_sFilePath , m_sName , "m_dPosY " , m_dPosY );
    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_dPosX " , m_dPosX );
        Ini.Save(_sFilePath , m_sName , "m_dPosY " , m_dPosY );
    }
}

//CCadCntPoint============================================
CCadCntPoint::CCadCntPoint(EN_COORDINATION _eCood ,String _sName):CCadPoint(_eCood ,_sName)
{
    m_eType = ctCntPoint ;
}
CCadCntPoint::~CCadCntPoint()
{

}
void CCadCntPoint::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;


    _pGdi -> LinePlus(m_dPosX , m_dPosY  , 500);

}

bool CCadCntPoint::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    bool bRet = false ;
    double dDist = 9999 ;
    if(GetLineIn(m_dPosX ,m_dPosY-500 , m_dPosX ,m_dPosY+500 , _dX , _dY , _dMargin)){
        dDist = fabs(m_dPosX - _dX) ;
        _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dPosX-500 ,m_dPosY , m_dPosX+500 ,m_dPosY , _dX , _dY , _dMargin)){
        dDist = fabs(m_dPosY - _dY) ;
        if(dDist < _dDist) _dDist = dDist ;
        bRet = true ;
    }
    return bRet ;

}

bool CCadCntPoint::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    double dDist = 0 ;
    if(GetPointIn(m_dPosX , m_dPosY , _dX , _dY , _dMargin)){
        _dDist = CMath::GetLineLenth(m_dPosX , m_dPosY , _dX , _dY);
        _tInPnt.dPosX = m_dPosX ;
        _tInPnt.dPosY = m_dPosY ;
        return true ;
    }

    return false ;
}

//CCadVisnPoint============================================
CCadVisnPoint::CCadVisnPoint(EN_COORDINATION _eCood ,String _sName):CCadPoint(_eCood ,_sName)
{
    m_eType = ctVisnPoint ;
}
CCadVisnPoint::~CCadVisnPoint()
{

}
void CCadVisnPoint::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;

    _pGdi -> LinePlus(m_dPosX , m_dPosY  , 10);
    _pGdi -> Rect(false , m_dPosX-10 , m_dPosY-10 , m_dPosX+10 , m_dPosY+10);

}

bool CCadVisnPoint::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    double dDist = 9999 ;
    if(GetLineIn(m_dPosX ,m_dPosY-10 , m_dPosX ,m_dPosY+10 , _dX , _dY , _dMargin)){
        dDist = fabs(m_dPosX - _dX) ;
        _dDist = dDist ;
        return true ;
    }
    if(GetLineIn(m_dPosX-10 ,m_dPosY , m_dPosX+10 ,m_dPosY , _dX , _dY , _dMargin)){
        dDist = fabs(m_dPosY - _dY) ;
        if(dDist < _dDist) _dDist = dDist ;
        return true ;
    }
    return false ;

}

bool CCadVisnPoint::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    if(GetPointIn(m_dPosX , m_dPosY , _dX , _dY , _dMargin)){
        _dDist = CMath::GetLineLenth(m_dPosX , m_dPosY , _dX , _dY);
        _tInPnt.dPosX = m_dPosX ;
        _tInPnt.dPosY = m_dPosY ;
        return true ;
    }

    return false ;
}


//CCadDspCmd============================================
CCadDspCmd::CCadDspCmd(EN_COORDINATION _eCood ,String _sName): CCadObject(_eCood ,_sName)
{

    m_eType    = ctDspCmd ;
    m_iNodeCnt = 0 ;

    memset(DspPos , 0 , sizeof(TDspPos) * MAX_DSP_CMD);
}

CCadDspCmd::~CCadDspCmd()
{

}

void CCadDspCmd::SetNodeCnt(int _iVal)
{
    m_iNodeCnt = _iVal ;
}
void CCadDspCmd::SetPosX(int _iId ,double _dVal)
{
    DspPos[_iId].dPosX =  _dVal ;
}
void CCadDspCmd::SetPosY(int _iId ,double _dVal)
{
    DspPos[_iId].dPosY =  _dVal ;
}
void CCadDspCmd::SetPosZ(int _iId ,double _dVal)
{
    DspPos[_iId].dPosZ =  _dVal ;
}

void CCadDspCmd::SetDelay(int _iId ,int    _iVal)
{
    DspPos[_iId].iDelay =  _iVal ;
}
void CCadDspCmd::SetUVSpeed(int _iId ,double _dVal)
{
    DspPos[_iId].dUVSpeed =  _dVal ;
}
void CCadDspCmd::SetSpeed(int _iId ,double _dVal)
{
    DspPos[_iId].dSpeed =  _dVal ;
}
void CCadDspCmd::SetDispOn(int _iId ,bool   _dVal)
{
    DspPos[_iId].bDispOn =  _dVal ;
}

int CCadDspCmd::GetNodeCnt()
{
     return m_iNodeCnt ;
}
double CCadDspCmd::GetPosX(int _iId,bool _bRaw)
{
    if(_bRaw) return DspPos[_iId].dPosX ;
    return DspPos[_iId].dPosX * m_dGainX + m_dOffsetX;
}
double CCadDspCmd::GetPosY(int _iId,bool _bRaw)
{
    if(_bRaw) return DspPos[_iId].dPosY ;
    return DspPos[_iId].dPosY * m_dGainY + m_dOffsetY ;
}
double CCadDspCmd::GetPosZ(int _iId,bool _bRaw)
{
    if(_bRaw) return DspPos[_iId].dPosZ ;
    double dTemp = DspPos[_iId].dPosZ * m_dGainZ + m_dOffsetZ;
    //return DspPos[_iId].dPosZ * m_dGainZ + m_dOffsetZ ;
    return dTemp;
}
int CCadDspCmd::GetDelay  (int _iId)
{
    return DspPos[_iId].iDelay ;
}
double CCadDspCmd::GetSpeed(int _iId)
{
    return DspPos[_iId].dSpeed ;
}
double CCadDspCmd::GetUVSpeed(int _iId)
{
    return DspPos[_iId].dUVSpeed ;
}
bool CCadDspCmd::GetDispOn(int _iId)
{
    return DspPos[_iId].bDispOn ;
}


//virtual.
void CCadDspCmd::SetCntrPosX(double _dVal)
{
    double dSum = 0 ;
    double dPreCnt = 0;
    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += DspPos[i].dPosX ;
    }

    dPreCnt = dSum / (double)m_iNodeCnt ;
    double dGap    = _dVal - dPreCnt ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        DspPos[i].dPosX += dGap ;
    }
}

void CCadDspCmd::SetCntrPosY(double _dVal)
{
    double dSum = 0 ;
    double dPreCnt  = 0 ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += DspPos[i].dPosY ;
    }

    dPreCnt = dSum / (double)m_iNodeCnt ;
    double dGap    = _dVal - dPreCnt ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        DspPos[i].dPosY += dGap ;
    }
}

double CCadDspCmd::GetCntrPosX(bool _bRaw)
{
    double dSum = 0 ;
    double dCnt = 0 ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += DspPos[i].dPosX ;
    }

    dCnt = dSum / (double)m_iNodeCnt ;

    if(_bRaw) return dCnt ;
    return dCnt * m_dGainX + m_dOffsetX;
}

double CCadDspCmd::GetCntrPosY(bool _bRaw)
{
    double dSum = 0 ;
    double dCnt  = 0 ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += DspPos[i].dPosY ;
    }

    dCnt = dSum / (double)m_iNodeCnt ;

    if(_bRaw) return dCnt ;
    return dCnt * m_dGainY + m_dOffsetY ;
}

void CCadDspCmd::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;


    if(m_iNodeCnt > 0) {
        for(int i = 1 ; i < m_iNodeCnt ; i++){

            if(DspPos[i].bDispOn){_pGdi -> m_tPen.Style = psSolid ;}
            else                 {_pGdi -> m_tPen.Style = psClear ;}
            _pGdi -> Line(DspPos[i-1].dPosX , DspPos[i-1        ].dPosY , DspPos[i].dPosX , DspPos[i].dPosY);
        }
    }

    //for(int i = 1 ; i < m_iNodeCnt ; i++){
    //    _pGdi -> Circle(true , DspPos[i].dPosX , DspPos[i].dPosY , 1 , 0 );
    //}


}

bool CCadDspCmd::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    //Point Check
    if(m_iNodeCnt < 2) {
        for(int i = 0 ; i < m_iNodeCnt ; i++) {
            if(GetPointIn(DspPos[i].dPosX , DspPos[i].dPosY , _dX , _dY , _dMargin)){
                _dDist = CMath::GetLineLenth(DspPos[i].dPosX , DspPos[i].dPosY , _dX , _dY);
                return true ;
            }
        }
    }
    else {
        bool bRet = false ;
        double dDist = 9999 ;
        double dA ;
        double dB ;
        for(int i = 1 ; i <= m_iNodeCnt ; i++) {
            if(GetLineIn(DspPos[i-1].dPosX , DspPos[i-1].dPosY , DspPos[i].dPosX , DspPos[i].dPosY ,_dX , _dY , _dMargin)){
                dDist = CMath::GetLengthPntToLine(_dX , _dY , DspPos[i-1].dPosX , DspPos[i-1].dPosY , DspPos[i].dPosX , DspPos[i].dPosY);
                if(dDist < _dDist) {
                    _dDist = dDist ;

                }
                bRet = true ;
            }
        }

        return bRet ;
    }

    return false ;
}

bool CCadDspCmd::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    double dDist = 0 ;
    bool bRet = false ;
    _dDist = 9999 ;
    for(int i = 0 ; i < m_iNodeCnt ; i++){
        if(GetPointIn(DspPos[i].dPosX , DspPos[i].dPosY , _dX , _dY , _dMargin)){
            dDist = CMath::GetLineLenth(DspPos[i].dPosX , DspPos[i].dPosY , _dX , _dY);
            if(dDist<_dDist){
                _tInPnt.dPosX = DspPos[i].dPosX ;
                _tInPnt.dPosY = DspPos[i].dPosY ;
                _dDist = dDist ;
            }
            bRet = true ;
        }
    }

    return bRet ;
}

void CCadDspCmd::LoadSave(bool _bLoad , String _sFilePath)
{
    CCadObject::LoadSave(_bLoad , _sFilePath);
    TUserINI Ini ;
    int iColor;
    //D:\Work\001_BC6Works\REC\HRM_930B\bin\JobFile\Seph
    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_iNodeCnt",m_iNodeCnt );
        for(int i = 0 ; i < m_iNodeCnt ; i++) {
            Ini.Load(_sFilePath , m_sName , "dPosX"    +String(i),DspPos[i].dPosX   );
            Ini.Load(_sFilePath , m_sName , "dPosY"    +String(i),DspPos[i].dPosY   );
            Ini.Load(_sFilePath , m_sName , "dPosZ"    +String(i),DspPos[i].dPosZ   );
            Ini.Load(_sFilePath , m_sName , "iDelay"   +String(i),DspPos[i].iDelay  );
            Ini.Load(_sFilePath , m_sName , "dSpeed"   +String(i),DspPos[i].dSpeed  );
            Ini.Load(_sFilePath , m_sName , "dUVSpeed" +String(i),DspPos[i].dUVSpeed);
            Ini.Load(_sFilePath , m_sName , "bDispOn"  +String(i),DspPos[i].bDispOn );
        }
    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_iNodeCnt",m_iNodeCnt );
        for(int i = 0 ; i < m_iNodeCnt ; i++) {
            Ini.Save(_sFilePath , m_sName , "dPosX"    +String(i),DspPos[i].dPosX   );
            Ini.Save(_sFilePath , m_sName , "dPosY"    +String(i),DspPos[i].dPosY   );
            Ini.Save(_sFilePath , m_sName , "dPosZ"    +String(i),DspPos[i].dPosZ   );
            Ini.Save(_sFilePath , m_sName , "iDelay"   +String(i),DspPos[i].iDelay  );
            Ini.Save(_sFilePath , m_sName , "dSpeed"   +String(i),DspPos[i].dSpeed  );
            Ini.Save(_sFilePath , m_sName , "dUVSpeed" +String(i),DspPos[i].dUVSpeed);
            Ini.Save(_sFilePath , m_sName , "bDispOn"  +String(i),DspPos[i].bDispOn );
        }
    }
}



//CCadLine============================================
CCadLine::CCadLine(EN_COORDINATION _eCood ,String _sName): CCadObject(_eCood ,_sName)
{
    m_eType    = ctLine ;
    m_dSttPosX = m_dSttPosY = 0 ;
    m_dEndPosX = m_dEndPosY = 0 ;
}

CCadLine::~CCadLine()
{

}

double CCadLine::GetSttPosX(bool _bRaw)
{
    if(_bRaw) return m_dSttPosX ;
    return m_dSttPosX * m_dGainX + m_dOffsetX;
}

double CCadLine::GetSttPosY(bool _bRaw)
{
    if(_bRaw) return m_dSttPosY ;
    return m_dSttPosY * m_dGainY + m_dOffsetY ;
}

double CCadLine::GetEndPosX(bool _bRaw)
{
    if(_bRaw) return m_dEndPosX ;
    return m_dEndPosX * m_dGainX + m_dOffsetX;
}

double CCadLine::GetEndPosY(bool _bRaw)
{
    if(_bRaw) return m_dEndPosY ;
    return m_dEndPosY * m_dGainY + m_dOffsetY ;
}

void CCadLine::SetSttPosX(double _dVal)
{
    m_dSttPosX = _dVal ;
}

void CCadLine::SetSttPosY(double _dVal)
{
    m_dSttPosY = _dVal ;
}
void CCadLine::SetEndPosX(double _dVal)
{
    m_dEndPosX = _dVal ;
}
void CCadLine::SetEndPosY(double _dVal)
{
    m_dEndPosY = _dVal ;
}
//virtual.
void CCadLine::SetCntrPosX(double _dVal)
{
    double dPreCtrX = (m_dEndPosX + m_dSttPosX) /2.0 ;
    double dGap     = _dVal - dPreCtrX ;

    m_dSttPosX += dGap ;
    m_dEndPosX += dGap ;


}

void CCadLine::SetCntrPosY(double _dVal)
{
    double dPreCtrY = (m_dEndPosY + m_dSttPosY) /2.0 ;
    double dGap     = _dVal - dPreCtrY ;

    m_dSttPosY += dGap ;
    m_dEndPosY += dGap ;
}

double CCadLine::GetCntrPosX(bool _bRaw)
{
    double dTemp = (m_dEndPosX + m_dSttPosX) /2.0 ;

    if(_bRaw) return dTemp ;

    return dTemp * m_dGainX + m_dOffsetX ;

}

double CCadLine::GetCntrPosY(bool _bRaw)
{
    double dTemp = (m_dEndPosY + m_dSttPosY) /2.0 ;

    if(_bRaw) return dTemp ;
    return dTemp * m_dGainY + m_dOffsetY ;
}

void CCadLine::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;


    _pGdi -> Line(m_dSttPosX  , m_dSttPosY , m_dEndPosX , m_dEndPosY);


}

bool CCadLine::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    //Point Check
    double dA ;
    double dB ;
    double dDist ;
    if(GetLineIn(m_dSttPosX , m_dSttPosY , m_dEndPosX , m_dEndPosY,_dX , _dY , _dMargin)){
        _dDist = CMath::GetLengthPntToLine(_dX , _dY , m_dSttPosX , m_dSttPosY , m_dEndPosX , m_dEndPosY);
        return true ;
    }

    return false ;

}

bool CCadLine::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    if(GetPointIn(m_dSttPosX , m_dSttPosY , _dX , _dY , _dMargin)){
        _dDist = CMath::GetLineLenth(m_dSttPosX , m_dSttPosY , _dX , _dY);
        _tInPnt.dPosX = m_dSttPosX ;
        _tInPnt.dPosY = m_dSttPosY ;
        return true ;
    }
    if(GetPointIn(m_dEndPosX , m_dEndPosY , _dX , _dY , _dMargin)){
        _dDist = CMath::GetLineLenth(m_dEndPosX , m_dEndPosY , _dX , _dY);
        _tInPnt.dPosX = m_dEndPosX ;
        _tInPnt.dPosY = m_dEndPosY ;
        return true ;
    }

    return false ;
}

void CCadLine::LoadSave(bool _bLoad , String _sFilePath)
{
    CCadObject::LoadSave(_bLoad , _sFilePath);

    int iColor;

    TUserINI Ini ;

    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_dSttPosX",m_dSttPosX );
        Ini.Load(_sFilePath , m_sName , "m_dSttPosY",m_dSttPosY );
        Ini.Load(_sFilePath , m_sName , "m_dEndPosX",m_dEndPosX );
        Ini.Load(_sFilePath , m_sName , "m_dEndPosY",m_dEndPosY );
    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_dSttPosX",m_dSttPosX );
        Ini.Save(_sFilePath , m_sName , "m_dSttPosY",m_dSttPosY );
        Ini.Save(_sFilePath , m_sName , "m_dEndPosX",m_dEndPosX );
        Ini.Save(_sFilePath , m_sName , "m_dEndPosY",m_dEndPosY );
    }
}



//CCadHeight=======================================================
CCadHeight::CCadHeight(EN_COORDINATION _eCood ,String _sName): CCadObject(_eCood ,_sName)
{
    m_eType    = ctHeight ;
    m_dSttPosX = m_dSttPosY = 0 ;
    m_dEndPosX = m_dEndPosY = 0 ;
    m_dSubPosX = m_dSubPosY = 0 ;
}
CCadHeight::~CCadHeight()
{

}
double CCadHeight::GetSttPosX(bool _bRaw)
{
    if(_bRaw) return m_dSttPosX ;
    return m_dSttPosX * m_dGainX + m_dOffsetX;
}
double CCadHeight::GetSttPosY(bool _bRaw)
{
    if(_bRaw) return m_dSttPosY ;
    return m_dSttPosY * m_dGainY + m_dOffsetY ;
}
double CCadHeight::GetEndPosX(bool _bRaw)
{
    if(_bRaw) return m_dEndPosX ;
    return m_dEndPosX * m_dGainX + m_dOffsetX;
}
double CCadHeight::GetEndPosY(bool _bRaw)
{
    if(_bRaw) return m_dEndPosY ;
    return m_dEndPosY * m_dGainY + m_dOffsetY ;
}
double CCadHeight::GetSubPosX(bool _bRaw)
{
    if(_bRaw) return m_dSubPosX ;
    return m_dSubPosX * m_dGainX + m_dOffsetX;
}
double CCadHeight::GetSubPosY(bool _bRaw)
{
    if(_bRaw) return m_dSubPosY ;
    return m_dSubPosY * m_dGainY + m_dOffsetY ;
}

void CCadHeight::SetSttPosX(double _dVal)
{
    m_dSttPosX = _dVal ;
}

void CCadHeight::SetSttPosY(double _dVal)
{
    m_dSttPosY = _dVal ;
}

void CCadHeight::SetEndPosX(double _dVal)
{
    m_dEndPosX = _dVal ;
}
void CCadHeight::SetEndPosY(double _dVal)
{
    m_dEndPosY = _dVal ;
}
void CCadHeight::SetSubPosX(double _dVal)
{
    m_dSubPosX = _dVal ;
}
void CCadHeight::SetSubPosY(double _dVal)
{
    m_dSubPosY = _dVal ;
}


void CCadHeight::SetCntrPosX(double _dVal)
{
    double dPreCtrX = (m_dEndPosX + m_dSttPosX + m_dSubPosX) /3.0 ;
    double dGap     = _dVal - dPreCtrX ;

    m_dSttPosX += dGap ;
    m_dEndPosX += dGap ;
    m_dSubPosX += dGap ;

}
void CCadHeight::SetCntrPosY(double _dVal)
{
    double dPreCtrY = (m_dEndPosY + m_dSttPosY + m_dSubPosY) /3.0 ;
    double dGap     = _dVal - dPreCtrY ;

    m_dSttPosY += dGap ;
    m_dEndPosY += dGap ;
    m_dSubPosY += dGap ;
}
double CCadHeight::GetCntrPosX(bool _bRaw)
{
    double dTemp =(m_dEndPosX + m_dSttPosX + m_dSubPosX) /3.0 ;
    if(_bRaw) return dTemp ;
    return dTemp * m_dGainX + m_dOffsetX ;
}
double CCadHeight::GetCntrPosY(bool _bRaw)
{
    double dTemp = (m_dEndPosY + m_dSttPosY + m_dSubPosY) /3.0 ;
    if(_bRaw) return dTemp ;
    return dTemp * m_dGainY + m_dOffsetY ;
}
void CCadHeight::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;

    //_pGdi -> m_tPen.Width += 2  ;
    _pGdi -> Line(m_dSttPosX  , m_dSttPosY , m_dEndPosX , m_dEndPosY);
    //_pGdi -> m_tPen.Width -= 2  ;
    _pGdi -> m_tPen.Style = psDash ; 
    _pGdi -> Line(m_dSttPosX  , m_dSttPosY , m_dSubPosX , m_dSubPosY);
    _pGdi -> Line(m_dSubPosX  , m_dSubPosY , m_dEndPosX , m_dEndPosY);
    _pGdi -> m_tPen.Style = psSolid ;

}

bool CCadHeight::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    //Point Check
    double dDist = 0 ;
    _dDist = 9999 ;
    bool bRet = false ;
    double dA ;
    double dB ;

    if(GetLineIn(m_dSttPosX , m_dSttPosY , m_dEndPosX , m_dEndPosY,_dX , _dY , _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX , _dY , m_dSttPosX , m_dSttPosY , m_dEndPosX , m_dEndPosY);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dEndPosX , m_dEndPosY , m_dSubPosX , m_dSubPosY,_dX , _dY , _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX , _dY , m_dEndPosX , m_dEndPosY , m_dSubPosX , m_dSubPosY);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dSubPosX , m_dSubPosY , m_dSttPosX , m_dSttPosY,_dX , _dY , _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX , _dY , m_dSubPosX , m_dSubPosY , m_dSttPosX , m_dSttPosY);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    return bRet ;
}

bool CCadHeight::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    double dDist = 0 ;
    _dDist = 9999 ;
    bool bRet = false ;

    if(GetPointIn(m_dSttPosX , m_dSttPosY , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dSttPosX , m_dSttPosY , _dX , _dY);
        if(_dDist > dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dSttPosX ;
            _tInPnt.dPosY = m_dSttPosY ;
        }
        bRet = true ;
    }
    if(GetPointIn(m_dEndPosX , m_dEndPosY , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dEndPosX , m_dEndPosY , _dX , _dY);
        if(_dDist > dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dEndPosX ;
            _tInPnt.dPosY = m_dEndPosY ;
        }
        bRet = true ;
    }
    if(GetPointIn(m_dSubPosX , m_dSubPosY , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dSubPosX , m_dSubPosY , _dX , _dY);
        if(_dDist > dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dSubPosX ;
            _tInPnt.dPosY = m_dSubPosY ;
        }
        bRet = true ;
    }

    return bRet ;
}

void CCadHeight::LoadSave(bool _bLoad , String _sFilePath)
{
    CCadObject::LoadSave(_bLoad , _sFilePath);
    int iColor;

    TUserINI Ini ;

    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_dSttPosX",m_dSttPosX );
        Ini.Load(_sFilePath , m_sName , "m_dSttPosY",m_dSttPosY );
        Ini.Load(_sFilePath , m_sName , "m_dEndPosX",m_dEndPosX );
        Ini.Load(_sFilePath , m_sName , "m_dEndPosY",m_dEndPosY );
        Ini.Load(_sFilePath , m_sName , "m_dSubPosX",m_dSubPosX );
        Ini.Load(_sFilePath , m_sName , "m_dSubPosY",m_dSubPosY );

    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_dSttPosX",m_dSttPosX );
        Ini.Save(_sFilePath , m_sName , "m_dSttPosY",m_dSttPosY );
        Ini.Save(_sFilePath , m_sName , "m_dEndPosX",m_dEndPosX );
        Ini.Save(_sFilePath , m_sName , "m_dEndPosY",m_dEndPosY );
        Ini.Save(_sFilePath , m_sName , "m_dSubPosX",m_dSubPosX );
        Ini.Save(_sFilePath , m_sName , "m_dSubPosY",m_dSubPosY );
    }
}

//CCadDspCmd============================================
CCadHghtCmd::CCadHghtCmd(EN_COORDINATION _eCood ,String _sName): CCadObject(_eCood ,_sName)
{

    m_eType    = ctHghtCmd ;
    m_iNodeCnt = 0 ;

    memset(HghtPos , 0 , sizeof(THghtPos) * MAX_HGHT_CMD);
}

CCadHghtCmd::~CCadHghtCmd()
{

}
//
//double CCadHghtCmd::GetLeft()
//{
//    return m_dLeft ;
//}
//void CCadHghtCmd::SetLeft(double _dVal)
//{
//    m_dLeft = _dVal ;
//}
//double CCadHghtCmd::GetTop()
//{
//    return m_dTop ;
//}
//void CCadHghtCmd::SetTop(double _dVal)
//{
//    m_dTop = _dVal ;
//
//}

void CCadHghtCmd::SetNodeCnt(int _iVal)
{
    m_iNodeCnt = _iVal ;
}
void CCadHghtCmd::SetPosX(int _iId ,double _dVal)
{
    HghtPos[_iId].dPosX =  _dVal ;
}
void CCadHghtCmd::SetPosY(int _iId ,double _dVal)
{
    HghtPos[_iId].dPosY =  _dVal ;
}
void CCadHghtCmd::SetPosZ(int _iId ,double _dVal)
{
    HghtPos[_iId].dPosZ =  _dVal ;
}

//double CCadHghtCmd::GetWidth()
//{
//    return m_dWidth ;
//}
//void CCadHghtCmd::SetWidth(double _dVal)
//{
//    m_dWidth = _dVal ;
//}
//double CCadHghtCmd::GetHeight()
//{
//    return m_dHeight ;
//}
//void CCadHghtCmd::SetHeight (double _dVal)
//{
//    m_dHeight = _dVal ;
//}
//
//double CCadHghtCmd::GetRight()
//{
//    return m_dLeft + m_dWidth ;
//}
//void CCadHghtCmd::SetRight(double _dVal)
//{
//    double dPreRight = GetRight() ;
//    double dGap      = _dVal - dPreRight ;
//
//    m_dWidth += dGap ;
//}
//
//double CCadHghtCmd::GetBottom()
//{
//    return m_dTop + m_dHeight ;
//}
//void CCadHghtCmd::SetBottom (double _dVal)
//{
//    double dPreBottom = GetBottom() ;
//    double dGap       = _dVal - dPreBottom ;
//
//    m_dHeight += dGap ;
//}


int CCadHghtCmd::GetNodeCnt()
{
     return m_iNodeCnt ;
}
double CCadHghtCmd::GetPosX(int _iId,bool _bRaw)
{
    if(_bRaw) return HghtPos[_iId].dPosX ;
    return HghtPos[_iId].dPosX * m_dGainX + m_dOffsetX;
}
double CCadHghtCmd::GetPosY(int _iId,bool _bRaw)
{
    if(_bRaw) return HghtPos[_iId].dPosY ;
    return HghtPos[_iId].dPosY * m_dGainY + m_dOffsetY ;
}
double CCadHghtCmd::GetPosZ(int _iId,bool _bRaw)
{
    if(_bRaw) return HghtPos[_iId].dPosZ ;
    return HghtPos[_iId].dPosZ * m_dGainZ + m_dOffsetZ ;
}

//Virtual
void CCadHghtCmd::SetCntrPosX(double _dVal)
{
    double dSum = 0 ;
    double dPreCnt = 0 ;
    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += HghtPos[i].dPosX ;
    }
    dPreCnt = dSum / (double)m_iNodeCnt ;

    double dGap    = _dVal - dPreCnt ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        HghtPos[i].dPosX += dGap ;
    }
}

void CCadHghtCmd::SetCntrPosY(double _dVal)
{
    double dSum = 0 ;
    double dPreCnt  = 0 ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += HghtPos[i].dPosY ;
    }

    dPreCnt = dSum / (double)m_iNodeCnt ;
    double dGap    = _dVal - dPreCnt ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        HghtPos[i].dPosY += dGap ;
    }
}

double CCadHghtCmd::GetCntrPosX(bool _bRaw)
{
    double dSum = 0 ;
    double dCnt = 0 ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += HghtPos[i].dPosX ;
    }

    dCnt = dSum / (double)m_iNodeCnt ;

    if(_bRaw) return dCnt ;

    return dCnt * m_dGainX + m_dOffsetX;
}

double CCadHghtCmd::GetCntrPosY(bool _bRaw)
{
    double dSum = 0 ;
    double dCnt  = 0 ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        dSum += HghtPos[i].dPosY ;
    }

    dCnt = dSum / (double)m_iNodeCnt ;

    if(_bRaw) return dCnt ;

    return dCnt * m_dGainY + m_dOffsetY ;
}

bool CCadHghtCmd::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    //Point Check
    double dDist = 0 ;
    _dDist = 9999 ;
    bool bRet = false ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        if(GetPointIn(HghtPos[i].dPosX , HghtPos[i].dPosY , _dX , _dY , _dMargin)){
            dDist = CMath::GetLineLenth(HghtPos[i].dPosX , HghtPos[i].dPosY , _dX , _dY);
            if(_dDist > dDist) {
                _dDist = dDist ;
            }
            bRet = true ;
        }
    }

    return bRet ;

}

bool CCadHghtCmd::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    //Point Check
    double dDist = 0 ;
    _dDist = 9999 ;
    bool bRet = false ;

    for(int i = 0 ; i < m_iNodeCnt ; i++) {
        if(GetPointIn(HghtPos[i].dPosX , HghtPos[i].dPosY , _dX , _dY , _dMargin)){
            dDist = CMath::GetLineLenth(HghtPos[i].dPosX , HghtPos[i].dPosY , _dX , _dY);
            if(_dDist > dDist) {
                _dDist = dDist ;
                _tInPnt.dPosX = HghtPos[i].dPosX ;
                _tInPnt.dPosY = HghtPos[i].dPosY ;
            }
            bRet = true ;
        }
    }

    return bRet ;
}

void CCadHghtCmd::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;

    //_pGdi -> Rect(false , m_dLeft  , m_dTop , GetRight() , GetBottom());

    if(m_iNodeCnt > 0) {
        //for(int i = 1 ; i < m_iNodeCnt ; i++){
        //    _pGdi -> Line(HghtPos[i-1].dPosX , HghtPos[i-1].dPosY , HghtPos[i].dPosX , HghtPos[i].dPosY);
        //}
        for(int i = 0 ; i < m_iNodeCnt ; i++){
            _pGdi -> Circle(true , HghtPos[i].dPosX  , HghtPos[i].dPosY , 1 , 0 );
        }

    }
}
void CCadHghtCmd::LoadSave(bool _bLoad , String _sFilePath)
{
    CCadObject::LoadSave(_bLoad , _sFilePath);
    TUserINI Ini ;
    int iColor;

    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_iNodeCnt",m_iNodeCnt );
        //Ini.Load(_sFilePath , m_sName , "m_dLeft   ",m_dLeft    );
        //Ini.Load(_sFilePath , m_sName , "m_dTop    ",m_dTop     );
        //Ini.Load(_sFilePath , m_sName , "m_dWidth  ",m_dWidth   );
        //Ini.Load(_sFilePath , m_sName , "m_dHeight ",m_dHeight  );
        for(int i = 0 ; i < m_iNodeCnt ; i++) {
            Ini.Load(_sFilePath , m_sName , "dPosX"  +String(i),HghtPos[i].dPosX  );
            Ini.Load(_sFilePath , m_sName , "dPosY"  +String(i),HghtPos[i].dPosY  );
            Ini.Load(_sFilePath , m_sName , "dPosZ"  +String(i),HghtPos[i].dPosZ  );
        }
    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_iNodeCnt",m_iNodeCnt );
        //Ini.Save(_sFilePath , m_sName , "m_dLeft   ",m_dLeft    );
        //Ini.Save(_sFilePath , m_sName , "m_dTop    ",m_dTop     );
        //Ini.Save(_sFilePath , m_sName , "m_dWidth  ",m_dWidth   );
        //Ini.Save(_sFilePath , m_sName , "m_dHeight ",m_dHeight  );
        for(int i = 0 ; i < m_iNodeCnt ; i++) {
            Ini.Save(_sFilePath , m_sName , "dPosX"  +String(i),HghtPos[i].dPosX  );
            Ini.Save(_sFilePath , m_sName , "dPosY"  +String(i),HghtPos[i].dPosY  );
            Ini.Save(_sFilePath , m_sName , "dPosZ"  +String(i),HghtPos[i].dPosZ  );
        }
    }
}

//CCadRect=======================================================
CCadRect::CCadRect(EN_COORDINATION _eCood ,String _sName): CCadObject(_eCood ,_sName)
{
    m_eType = ctRect ;
   m_dLeft  = m_dTop    = 0 ;
   m_dWidth = m_dHeight = 0 ;
}
CCadRect::~CCadRect()
{

}
double CCadRect::GetLeft(bool _bRaw)
{
    if(_bRaw) return m_dLeft ;
    return m_dLeft * m_dGainX + m_dOffsetX;
}
void CCadRect::SetLeft(double _dVal)
{
    m_dLeft = _dVal ;
}
double CCadRect::GetTop(bool _bRaw)
{
    if(_bRaw) return m_dTop ;
    return m_dTop * m_dGainY + m_dOffsetY ;
}
void CCadRect::SetTop(double _dVal)
{
    m_dTop = _dVal ;

}
double CCadRect::GetWidth()
{
    return m_dWidth ;
}
void CCadRect::SetWidth(double _dVal)
{
    m_dWidth = _dVal ;
}
double CCadRect::GetHeight()
{
    return m_dHeight ;
}
void CCadRect::SetHeight (double _dVal)
{
    m_dHeight = _dVal ;
}

double CCadRect::GetRight(bool _bRaw)
{
    double dTemp = m_dLeft + m_dWidth ;
    if(_bRaw) return dTemp ;
    return dTemp * m_dGainX + m_dOffsetX ;
}
void CCadRect::SetRight(double _dVal)
{
    double dPreRight = m_dLeft + m_dWidth ;
    double dGap      = _dVal - dPreRight ;

    m_dWidth += dGap ;
}

double CCadRect::GetBottom(bool _bRaw)
{
    double dTemp = m_dTop + m_dHeight ;
    if(_bRaw) return dTemp ;
    return dTemp * m_dGainY + m_dOffsetY ;
}
void CCadRect::SetBottom (double _dVal)
{
    double dPreBottom = m_dTop + m_dHeight ;
    double dGap       = _dVal - dPreBottom ;

    m_dHeight += dGap ;
}

bool CCadRect::IsPntRectInOfsGain(double _dX , double _dY)
{
    double dLeft   = m_dLeft;
    double dTop    = m_dTop ;
    double dRight  = m_dLeft + m_dWidth ;
    double dBottom = m_dTop  + m_dHeight;


    if(dLeft < _dX && _dX < dRight &&
       dTop  < _dY && _dY < dBottom){
        return true ;
    }
    return false ;


}


//Virtual
void CCadRect::SetCntrPosX(double _dVal)
{
    double dPreCtrX = (m_dLeft + m_dLeft + m_dWidth) /2.0 ;
    double dGap     = _dVal - dPreCtrX ;

    m_dLeft += dGap ;
}
void CCadRect::SetCntrPosY(double _dVal)
{
    double dPreCtrY = (m_dTop + m_dTop + m_dHeight) /2.0 ;
    double dGap     = _dVal - dPreCtrY ;

    m_dTop += dGap ;
}
double CCadRect::GetCntrPosX(bool _bRaw)
{
    double dTemp = (m_dLeft + m_dLeft + m_dWidth) /2.0 ;
    if(_bRaw) return dTemp ;
    return dTemp * m_dGainX + m_dOffsetX ;
}
double CCadRect::GetCntrPosY(bool _bRaw)
{
    double dTemp = (m_dTop + m_dTop + m_dHeight) /2.0 ;
    if(_bRaw) return dTemp ;
    return dTemp * m_dGainY + m_dOffsetY ;
}
void CCadRect::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;

    _pGdi -> Rect(false , m_dLeft  , m_dTop , m_dLeft + m_dWidth , m_dTop + m_dHeight );

}

bool CCadRect::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;
    //Point Check
    double dDist = 0 ;
    _dDist = 9999;
    bool bRet = false ;

    if(GetLineIn(m_dLeft            , m_dTop             , m_dLeft + m_dWidth , m_dTop             ,_dX , _dY , _dMargin)){
        dDist = fabs(m_dTop - _dY);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dLeft + m_dWidth , m_dTop             , m_dLeft + m_dWidth , m_dTop + m_dHeight ,_dX , _dY , _dMargin)){
        dDist = fabs(m_dLeft + m_dWidth - _dX);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dLeft + m_dWidth , m_dTop + m_dHeight , m_dLeft            , m_dTop + m_dHeight ,_dX , _dY , _dMargin)){
        dDist = fabs(m_dTop + m_dHeight - _dY);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dLeft            , m_dTop + m_dHeight , m_dLeft            , m_dTop             ,_dX , _dY , _dMargin)){
        dDist = fabs(m_dTop + m_dHeight - _dY);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }

    return bRet ;

}

bool CCadRect::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    double dDist = 0 ;
    bool bRet = false ;
    _dDist = 9999;

    if(GetPointIn(m_dLeft , m_dTop , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft , m_dTop , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft ;
            _tInPnt.dPosY = m_dTop  ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft+m_dWidth , m_dTop , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft+m_dWidth , m_dTop , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft+m_dWidth ;
            _tInPnt.dPosY = m_dTop  ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft+m_dWidth , m_dTop+m_dHeight , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft+m_dWidth , m_dTop+m_dHeight , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft+m_dWidth ;
            _tInPnt.dPosY = m_dTop+m_dHeight ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft , m_dTop+m_dHeight , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft , m_dTop+m_dHeight , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft ;
            _tInPnt.dPosY = m_dTop+m_dHeight  ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft + m_dWidth/2.0  , m_dTop + m_dHeight/2.0 , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight/2.0 , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft + m_dWidth/2.0 ;
            _tInPnt.dPosY = m_dTop + m_dHeight/2.0  ;
            bRet =  true ;
        }
    }

    return bRet ;
}

void CCadRect::LoadSave(bool _bLoad , String _sFilePath)
{
    CCadObject::LoadSave(_bLoad , _sFilePath);
    int iColor;

    TUserINI Ini ;

    if(_bLoad){
        Ini.Load(_sFilePath , m_sName , "m_dLeft  ",m_dLeft   );
        Ini.Load(_sFilePath , m_sName , "m_dTop   ",m_dTop    );
        Ini.Load(_sFilePath , m_sName , "m_dWidth ",m_dWidth  );
        Ini.Load(_sFilePath , m_sName , "m_dHeight",m_dHeight );
    }
    else {
        Ini.Save(_sFilePath , m_sName , "m_dLeft  ",m_dLeft   );
        Ini.Save(_sFilePath , m_sName , "m_dTop   ",m_dTop    );
        Ini.Save(_sFilePath , m_sName , "m_dWidth ",m_dWidth  );
        Ini.Save(_sFilePath , m_sName , "m_dHeight",m_dHeight );
    }
}

//CCadRhombus=======================================================
CCadRhombus::CCadRhombus(EN_COORDINATION _eCood ,String _sName): CCadRect(_eCood ,_sName)
{
    m_eType = ctRhombus ;
}
CCadRhombus::~CCadRhombus()
{

}
void CCadRhombus::Paint(CGdi * _pGdi)
{
    if(!m_bEnabled || !m_bVisible) return ;

    _pGdi -> Line(m_dLeft + m_dWidth/2.0 , m_dTop                 , m_dLeft + m_dWidth     , m_dTop + m_dHeight/2.0);
    _pGdi -> Line(m_dLeft + m_dWidth     , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight    );
    _pGdi -> Line(m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight     , m_dLeft                , m_dTop + m_dHeight/2.0);
    _pGdi -> Line(m_dLeft                , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop                );

}

bool CCadRhombus::IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    //Point Check
    //if(GetLineIn(m_dLeft + m_dWidth/2.0 , m_dTop                 , m_dLeft + m_dWidth     , m_dTop + m_dHeight/2.0, _dX, _dY, _dMargin))return true ;
    //if(GetLineIn(m_dLeft + m_dWidth     , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight    , _dX, _dY, _dMargin))return true ;
    //if(GetLineIn(m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight     , m_dLeft                , m_dTop + m_dHeight/2.0, _dX, _dY, _dMargin))return true ;
    //if(GetLineIn(m_dLeft                , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop                , _dX, _dY, _dMargin))return true ;


    double dDist = 0 ;
    _dDist = 9999;
    double dA ;
    double dB ;
    bool bRet = false ;

    if(GetLineIn(m_dLeft + m_dWidth/2.0 , m_dTop , m_dLeft + m_dWidth , m_dTop + m_dHeight/2.0 ,_dX , _dY , _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX,_dY,m_dLeft + m_dWidth/2.0 , m_dTop , m_dLeft + m_dWidth , m_dTop + m_dHeight/2.0);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dLeft + m_dWidth     , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight    , _dX, _dY, _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX,_dY,m_dLeft + m_dWidth , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight , m_dLeft  , m_dTop + m_dHeight/2.0, _dX, _dY, _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX,_dY,m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight , m_dLeft , m_dTop + m_dHeight/2.0);
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }
    if(GetLineIn(m_dLeft                , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop                , _dX, _dY, _dMargin)){
        dDist = CMath::GetLengthPntToLine(_dX,_dY,m_dLeft , m_dTop + m_dHeight/2.0 , m_dLeft + m_dWidth/2.0 , m_dTop );
        if(_dDist > dDist) _dDist = dDist ;
        bRet = true ;
    }



    return bRet ;

}

bool CCadRhombus::IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)
{
    if(!m_bEnabled || !m_bVisible) return false;

    double dDist = 0 ;
    _dDist = 9999;
    bool bRet = false ;

    if(GetPointIn(m_dLeft + m_dWidth/2.0  , m_dTop                 , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft + m_dWidth/2.0  , m_dTop , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft + m_dWidth/2.0 ;
            _tInPnt.dPosY = m_dTop  ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft + m_dWidth      , m_dTop + m_dHeight/2.0 , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft + m_dWidth  , m_dTop + m_dHeight/2.0 , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft + m_dWidth ;
            _tInPnt.dPosY = m_dTop + m_dHeight/2.0 ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft + m_dWidth/2.0  , m_dTop + m_dHeight     , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft + m_dWidth/2.0 , m_dTop + m_dHeight , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft + m_dWidth/2.0 ;
            _tInPnt.dPosY = m_dTop + m_dHeight ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft                 , m_dTop + m_dHeight/2.0 , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft  , m_dTop + m_dHeight/2.0 , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft  ;
            _tInPnt.dPosY = m_dTop + m_dHeight/2.0 ;
            bRet =  true ;
        }
    }
    if(GetPointIn(m_dLeft + m_dWidth/2.0  , m_dTop + m_dHeight/2.0 , _dX , _dY , _dMargin)){
        dDist = CMath::GetLineLenth(m_dLeft + m_dWidth/2.0  , m_dTop + m_dHeight/2.0 , _dX , _dY);
        if(dDist < _dDist) {
            _dDist = dDist ;
            _tInPnt.dPosX = m_dLeft + m_dWidth/2.0  ;
            _tInPnt.dPosY = m_dTop + m_dHeight/2.0 ;
            bRet =  true ;
        }
    }

    return bRet ;
}



//==============================================================================
//스테이지.
//==============================================================================
CCadPositionStg CAD ;
CCadPositionStg::CCadPositionStg()
{
}

CCadPositionStg::~CCadPositionStg()
{
}

void CCadPositionStg::Init()
{
    m_eCoodination = cnStg ;

    m_pSelectedObj = NULL ;
    m_pMouseInObj  = NULL ;

    m_bSelectedPnt = false ;
    m_bMouseInPnt  = false ;


    m_hDc = NULL ;        //= NULL;
    m_iPanelWidth  = 0;//=0 ;
    m_iPanelHeight = 0;//=0 ;

    m_dCoodWidth  = 400 ;
    m_dCoodHeight = 400 ;


    String sTemp = "";

    CT = new CCadCntPoint(cnStg ,"CT");
    CT->SetColor(clBlue);
    CT->SetCntrPosX(m_dCoodWidth /2.0);
    CT->SetCntrPosX(m_dCoodHeight/2.0);


    MB.MB_Rect      = new CCadRect     (cnStg ,"MB"          );
    MB.MB_LOri      = new CCadVisnPoint(cnStg ,"MB_LOri"     );
    MB.MB_LAlnM     = new CCadVisnPoint(cnStg ,"MB_LAlnM"    );
    MB.MB_LAlnS     = new CCadVisnPoint(cnStg ,"MB_LAlnS"    );
    MB.MB_RHgt      = new CCadRhombus  (cnStg ,"MB_RHgt"     );
    //Jinseop
    MB.MB_RChk      = new CCadHghtCmd  (cnStg ,"MB_RChk"     );

    //MB.MB_RHgtCheck = new CCadHghtCmd  (cnStg ,"MB_RHgtCheck");

    //CM_RHgtFrst     = new CCadRhombus  (cnStg ,"CM_RHgtFrst" );

    //임시로..
    //CM_RHgtScnd     = new CCadRhombus  (cnStg ,"CM_RHgtScnd" );

    for(int r = 0; r < MAX_CMOS_ROW; r++){
        for(int c = 0; c < MAX_CMOS_COL; c++){
            sTemp.printf("PC_Rect_%d%d"     , c,r); CM[c][r].PC_Rect      = new CCadRect     (cnStg ,sTemp);
            sTemp.printf("CM_Rect_%d%d"     , c,r); CM[c][r].CM_Rect      = new CCadRect     (cnStg ,sTemp);
            sTemp.printf("CM_LAlnM_%d%d"    , c,r); CM[c][r].CM_LAlnM     = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_LAlnS_%d%d"    , c,r); CM[c][r].CM_LAlnS     = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RHgtL_%d%d"    , c,r); CM[c][r].CM_RHgtL     = new CCadHeight   (cnStg ,sTemp);
            sTemp.printf("CM_RHgtT_%d%d"    , c,r); CM[c][r].CM_RHgtT     = new CCadHeight   (cnStg ,sTemp);
            sTemp.printf("CM_RHgtR_%d%d"    , c,r); CM[c][r].CM_RHgtR     = new CCadHeight   (cnStg ,sTemp);
            sTemp.printf("CM_RHgtB_%d%d"    , c,r); CM[c][r].CM_RHgtB     = new CCadHeight   (cnStg ,sTemp);
            sTemp.printf("CM_RAlnML_%d%d"   , c,r); CM[c][r].CM_RAlnML    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnSL_%d%d"   , c,r); CM[c][r].CM_RAlnSL    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnMT_%d%d"   , c,r); CM[c][r].CM_RAlnMT    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnST_%d%d"   , c,r); CM[c][r].CM_RAlnST    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnMR_%d%d"   , c,r); CM[c][r].CM_RAlnMR    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnSR_%d%d"   , c,r); CM[c][r].CM_RAlnSR    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnMB_%d%d"   , c,r); CM[c][r].CM_RAlnMB    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_RAlnSB_%d%d"   , c,r); CM[c][r].CM_RAlnSB    = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("CM_LDsp1_%d%d"    , c,r); CM[c][r].CM_LDsp1     = new CCadDspCmd   (cnStg ,sTemp);
            sTemp.printf("CM_LDsp2_%d%d"    , c,r); CM[c][r].CM_LDsp2     = new CCadDspCmd   (cnStg ,sTemp);
            sTemp.printf("CM_RHgtCheck_%d%d", c,r); CM[c][r].CM_RHgtCheck = new CCadHghtCmd  (cnStg ,sTemp);

        }
    }

    /*
    for(int r = 0; r < MAX_CMOS_ROW; r++){
        for(int c = 0; c < MAX_CMOS_COL-1; c++){
            sTemp.printf("VG_RGapM%d%d"   , c,r); VG[c][r].VG_RGapM  = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("VG_RGapS%d%d"   , c,r); VG[c][r].VG_RGapS  = new CCadVisnPoint(cnStg ,sTemp);
        }
    }

    for(int r = 0; r < MAX_CMOS_ROW-1; r++){
        for(int c = 0; c < MAX_CMOS_COL; c++){
            sTemp.printf("HG_RGapM%d%d"   , c,r); HG[c][r].HG_RGapM  = new CCadVisnPoint(cnStg ,sTemp);
            sTemp.printf("HG_RGapS%d%d"   , c,r); HG[c][r].HG_RGapS  = new CCadVisnPoint(cnStg ,sTemp);
        }
    }
    */

    LoadLastInfo() ;
    LoadSave(true , m_sLastDev);

}

void CCadPositionStg::Close()
{
    delete CT ;
    delete MB.MB_Rect      ;
    delete MB.MB_LOri      ;
    delete MB.MB_LAlnM     ;
    delete MB.MB_LAlnS     ;
    delete MB.MB_RHgt      ;
    delete MB.MB_RChk      ;
    //delete MB.MB_RHgtCheck ;

    //delete CM_RHgtFrst     ;
    //delete CM_RHgtScnd     ;

    for(int r = 0; r < MAX_CMOS_ROW; r++){
        for(int c = 0; c < MAX_CMOS_COL; c++){
            delete CM[c][r].PC_Rect   ;
            delete CM[c][r].CM_Rect   ;
            delete CM[c][r].CM_LAlnM  ;
            delete CM[c][r].CM_LAlnS  ;
            delete CM[c][r].CM_RHgtL  ;
            delete CM[c][r].CM_RHgtT  ;
            delete CM[c][r].CM_RHgtR  ;
            delete CM[c][r].CM_RHgtB  ;
            delete CM[c][r].CM_RAlnML ;
            delete CM[c][r].CM_RAlnSL ;
            delete CM[c][r].CM_RAlnMT ;
            delete CM[c][r].CM_RAlnST ;
            delete CM[c][r].CM_RAlnMR ;
            delete CM[c][r].CM_RAlnSR ;
            delete CM[c][r].CM_RAlnMB ;
            delete CM[c][r].CM_RAlnSB ;
            delete CM[c][r].CM_LDsp1  ;
            delete CM[c][r].CM_LDsp2  ;

        }
    }

    /*
    for(int r = 0; r < MAX_CMOS_ROW; r++){
        for(int c = 0; c < MAX_CMOS_COL-1; c++){
            delete VG[c][r].VG_RGapM  ;
            delete VG[c][r].VG_RGapS  ;
        }
    }

    for(int r = 0; r < MAX_CMOS_ROW-1; r++){
        for(int c = 0; c < MAX_CMOS_COL; c++){
            delete HG[c][r].HG_RGapM  ;
            delete HG[c][r].HG_RGapS  ;
        }
    }
    */
}


void CCadPositionStg::SetSelectedObject(CCadObject * _pObject)
{
    m_pSelectedObj = _pObject ;
}

CCadObject * CCadPositionStg::GetSelectedObject()
{
    return m_pSelectedObj ;
}

CCadObject * CCadPositionStg::FindObjectByName(String _sName)
{
    return CCadObject::FindObjectByName(cnStg , _sName) ;
}
String CCadPositionStg::GetNameList()
{
    return CCadObject::GetNameList(cnStg);
}

void CCadPositionStg::Paint(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    m_hDc = _hDc ;
    //m_iPanelWidth  = _iPanelWidth  ;
    //m_iPanelHeight = _iPanelHeight ;

    double dScaleX = GetScaleX();
    double dScaleY = GetScaleY();

    //CGdi * Gdi = new CGdi(m_hDc , dScaleX , dScaleY , GetCoodCntX() , GetCoodCntY());
    CGdi * Gdi = new CGdi(_hDc , _dScaleX , _dScaleY , GetCoodCntX()-_dOffsetX , GetCoodCntY()-_dOffsetY);

    //Gdi -> m_tPen.Color = clBlue ;
    //Gdi -> LinePlus(0,0, 400);

    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(cnStg) ;
    while(Obj){
        Gdi -> m_tPen.Style  = psSolid ;
        if(Obj == m_pSelectedObj) {
            Gdi -> m_tPen.Color  = Obj -> GetColor();
            Gdi -> m_tText.Color = Obj -> GetColor();
            Gdi -> m_tPen.Width  = 2 ;
        }
        else if(Obj == m_pMouseInObj){
            Gdi -> m_tPen.Color  = Obj -> GetColor();
            Gdi -> m_tText.Color = Obj -> GetColor();
            Gdi -> m_tPen.Width  = 1 ;
        }

        else {
            Gdi -> m_tPen.Color  = clSilver ;
            Gdi -> m_tText.Color = clSilver ;
            Gdi -> m_tPen.Width  = 1 ;

        }
        Obj -> Paint(Gdi);
        Obj = Obj -> GetPreObject() ;
    }

    if(m_bSelectedPnt){
        Gdi -> m_tPen.Width = 2 ;
        Gdi -> m_tPen.Color = clLime ;
        Gdi -> LineX(m_tSelectedPnt.dPosX,m_tSelectedPnt.dPosY,3);
        Gdi -> m_tPen.Width = 1 ;
        Gdi -> m_tPen.Color = clRed ;
        Gdi -> LineX(m_tSelectedPnt.dPosX,m_tSelectedPnt.dPosY,2);
    }
    if(m_bMouseInPnt){
        Gdi -> m_tPen.Width = 2 ;
        Gdi -> m_tPen.Color = clAqua ;
        Gdi -> LineX(m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY,3);
        Gdi -> m_tPen.Width = 1 ;
        Gdi -> m_tPen.Color = clRed ;
        Gdi -> LineX(m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY,2);
    }

    delete Gdi;

    Gdi = new CGdi(_hDc );
    if(m_bMouseInPnt && m_bSelectedPnt){
        double dXGap = m_tMouseInPnt.dPosX - m_tSelectedPnt.dPosX ;
        double dYGap = m_tMouseInPnt.dPosY - m_tSelectedPnt.dPosY ;
        String sTemp ; //= String("X:") + () + " Y:" + ();
        sTemp.sprintf("SelX:%0.3f SelY:%0.3f MouseX:%0.3f MouseY:%0.3f X:%0.3f Y:%0.3f",m_tSelectedPnt.dPosX , m_tSelectedPnt.dPosY ,m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY , dXGap , dYGap);
        Gdi -> Text(5 , 5 , sTemp.c_str());
    }
    else if(m_bMouseInPnt){
        String sTemp ; //= String("X:") + () + " Y:" + ();
        sTemp.sprintf("MouseX:%0.3f MouseY:%0.3f",m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY );
        Gdi -> Text(5 , 5 , sTemp.c_str());
    }
    else if(m_bSelectedPnt){
        String sTemp ; //= String("X:") + () + " Y:" + ();
        sTemp.sprintf("SelX:%0.3f SelY:%0.3f",m_tSelectedPnt.dPosX,m_tSelectedPnt.dPosY );
        Gdi -> Text(5 , 5 , sTemp.c_str());
    }

    delete Gdi;

}

void CCadPositionStg::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sLastDev );

    if (m_sLastDev == "") m_sLastDev  = "NONE";
}

void CCadPositionStg::LoadSave(bool _bLoad, String _sDevice)
{       //D:\Work\001_BC6Works\REC\HRM_930B\bin\JobFile\Seph
    String sPath = EXE_FOLDER + "JobFile\\" + _sDevice + "\\CadPos.SPP";

    LoadSaveFile(_bLoad , sPath);

}

void CCadPositionStg::LoadSaveFile(bool _bLoad, String _sFilePath)
{
    TUserINI Ini ;

    int iColor ;
    //if(_bLoad){
    //    Ini.Load(_sFilePath , "Member" , "m_iMaxCol" , m_iMaxCol);
    //    Ini.Load(_sFilePath , "Member" , "m_iMaxRow" , m_iMaxRow);
    //}
    //else {
    //    Ini.Save(_sFilePath , "Member" , "m_iMaxCol" , m_iMaxCol);
    //    Ini.Save(_sFilePath , "Member" , "m_iMaxRow" , m_iMaxRow);
    //}




    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        if(Obj -> GetEnabled()) {
            Obj -> LoadSave(_bLoad , _sFilePath);
        }
        Obj = Obj -> GetPreObject() ;
    }
}

CCadObject * CCadPositionStg::OnMouseDown(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    double dPosX , dPosY , dMargin;
    dPosX   = _dX - GetCoodCntX(); // * m_dCoodWidth /m_iPanelWidth  - GetCoodCntX();
    dPosY   = _dY - GetCoodCntY(); // * m_dCoodHeight/m_iPanelHeight - GetCoodCntY();

    dMargin = iMargin;///_dScaleX ;//    * m_dCoodWidth /m_iPanelWidth ;   //-34.5
                                                         //-125.13
    CCadObject * Obj ;

    //Obj = MB.MB_Rect ;
    //int a ;
    //if(Obj -> IsMouseIn(dPosX , dPosY , dMargin)){
    //    a++;
    //}



    Obj = CCadObject::GetLastObject(m_eCoodination) ;

    TCadPoint Pnt ;
    double dDist ;
    if(m_pSelectedObj) {
        if(m_pSelectedObj -> IsMousePntIn(dPosX , dPosY, dMargin , Pnt , dDist)){
            m_tSelectedPnt = Pnt ;
            m_bSelectedPnt = true ;
            return m_pSelectedObj ;
        }
    }

    m_pSelectedObj = NULL ;
    double dNearDist = 9999 ;
    while(Obj){
        if(Obj -> IsMouseIn(dPosX , dPosY, dMargin , dDist)) {
            if(dNearDist > dDist) {
                dNearDist = dDist ;
                m_pSelectedObj = Obj;
                m_tSelectedPnt.Clear();
                m_bSelectedPnt = false ;
            }

        }
        Obj = Obj -> GetPreObject() ;
    }

    return m_pSelectedObj;
}

void CCadPositionStg::OnMouseMove(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    double dPosX , dPosY , dMargin;
    //dPosX   = _iX  * m_dCoodWidth /m_iPanelWidth  - GetCoodCntX();
    //dPosY   = _iY  * m_dCoodHeight/m_iPanelHeight - GetCoodCntY();

    dPosX   = _dX- GetCoodCntX();//_dOffsetX + _iX / _dScaleX ;  //-34.5
    dPosY   = _dY- GetCoodCntY();//_dOffsetY + _iY / _dScaleY ;  //-125.13
    dMargin = iMargin;///_dScaleX ;// * m_dCoodWidth /m_iPanelWidth ;

    CCadObject * Obj ;                   
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    m_tMouseInPnt.Clear() ;
    m_bMouseInPnt = false ;
    m_pMouseInObj = NULL ;

    TCadPoint Pnt ;
    //if(m_pSelectedObj) {
    //    if(m_pSelectedObj -> IsMousePntIn(dPosX , dPosY, dMargin , Pnt)){
    //        m_tMouseInPnt = Pnt            ;
    //        m_bMouseInPnt = true ;
    //        m_pMouseInObj = m_pSelectedObj ;
    //        return ;
    //    }
    //}
    double dDist ;
    double dNearDistPnt = 9999 ;
    double dNearDist    = 9999 ;
    while(Obj){
        if(Obj -> IsMousePntIn(dPosX , dPosY, dMargin,Pnt,dDist)) {
            if(dNearDistPnt > dDist) {
                dNearDistPnt = dDist ;
                m_tMouseInPnt = Pnt  ;
                m_bMouseInPnt = true ;
                m_pMouseInObj = Obj ;
            }
        }
        if(Obj -> IsMouseIn(dPosX , dPosY, dMargin,dDist)) {
            if(dNearDist > dDist) {
                dNearDist = dDist ;
                m_pMouseInObj = Obj ;
            }
        }
        Obj = Obj -> GetPreObject() ;
    }

}

void CCadPositionStg::SetGainX(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetGainX(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionStg::SetGainY(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetGainY(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionStg::SetGainZ(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetGainZ(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionStg::SetOffsetX(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetOffsetX(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionStg::SetOffsetY(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetOffsetY(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionStg::SetOffsetZ(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetOffsetZ(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionStg::SetRowCol (int _iCol , int _iRow)
{
    m_iMaxCol = _iCol ;
    m_iMaxRow = _iRow ;


    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetEnabled(true);
        Obj = Obj -> GetPreObject() ;
    }



    for(int c = 0; c < _iCol; c++){
        CM[c][0      ].CM_RHgtT ->SetEnabled(false);
        CM[c][0      ].CM_RAlnMT->SetEnabled(false);
        CM[c][0      ].CM_RAlnST->SetEnabled(false);

        CM[c][_iRow-1].CM_RHgtB ->SetEnabled(false);
        CM[c][_iRow-1].CM_RAlnMB->SetEnabled(false);
        CM[c][_iRow-1].CM_RAlnSB->SetEnabled(false);

        //HG[c][_iRow-1].HG_RGapM ->SetEnabled(false);
        //HG[c][_iRow-1].HG_RGapS ->SetEnabled(false);

    }
    for(int r = 0; r < _iRow; r++){
        CM[0      ][r].CM_RHgtL ->SetEnabled(false);
        CM[0      ][r].CM_RAlnML->SetEnabled(false);
        CM[0      ][r].CM_RAlnSL->SetEnabled(false);

        CM[_iCol-1][r].CM_RHgtR ->SetEnabled(false);
        CM[_iCol-1][r].CM_RAlnMR->SetEnabled(false);
        CM[_iCol-1][r].CM_RAlnSR->SetEnabled(false);

        //VG[_iCol-1][r].VG_RGapM ->SetEnabled(false);
        //VG[_iCol-1][r].VG_RGapS ->SetEnabled(false);

    }

    for(int r = 0; r < MAX_CMOS_ROW ; r++){
        for(int c = _iCol; c < MAX_CMOS_COL; c++){
            CM[c][r].PC_Rect  ->SetEnabled(false);
            CM[c][r].CM_Rect  ->SetEnabled(false);

            CM[c][r].CM_LAlnM ->SetEnabled(false);
            CM[c][r].CM_LAlnS ->SetEnabled(false);

            CM[c][r].CM_RHgtL ->SetEnabled(false);
            CM[c][r].CM_RHgtT ->SetEnabled(false);
            CM[c][r].CM_RHgtR ->SetEnabled(false);
            CM[c][r].CM_RHgtB ->SetEnabled(false);

            CM[c][r].CM_RAlnML->SetEnabled(false);
            CM[c][r].CM_RAlnSL->SetEnabled(false);
            CM[c][r].CM_RAlnMT->SetEnabled(false);
            CM[c][r].CM_RAlnST->SetEnabled(false);
            CM[c][r].CM_RAlnMR->SetEnabled(false);
            CM[c][r].CM_RAlnSR->SetEnabled(false);
            CM[c][r].CM_RAlnMB->SetEnabled(false);
            CM[c][r].CM_RAlnSB->SetEnabled(false);

            CM[c][r].CM_LDsp1 ->SetEnabled(false);
            CM[c][r].CM_LDsp2 ->SetEnabled(false);

            CM[c][r].CM_RHgtCheck ->SetEnabled(false);



        }
    }

    for(int r = _iRow; r < MAX_CMOS_ROW ; r++){
        for(int c = 0; c < MAX_CMOS_COL; c++){
            CM[c][r].PC_Rect  ->SetEnabled(false);
            CM[c][r].CM_Rect  ->SetEnabled(false);

            CM[c][r].CM_LAlnM ->SetEnabled(false);
            CM[c][r].CM_LAlnS ->SetEnabled(false);

            CM[c][r].CM_RHgtL ->SetEnabled(false);
            CM[c][r].CM_RHgtT ->SetEnabled(false);
            CM[c][r].CM_RHgtR ->SetEnabled(false);
            CM[c][r].CM_RHgtB ->SetEnabled(false);

            CM[c][r].CM_RAlnML->SetEnabled(false);
            CM[c][r].CM_RAlnSL->SetEnabled(false);
            CM[c][r].CM_RAlnMT->SetEnabled(false);
            CM[c][r].CM_RAlnST->SetEnabled(false);
            CM[c][r].CM_RAlnMR->SetEnabled(false);
            CM[c][r].CM_RAlnSR->SetEnabled(false);
            CM[c][r].CM_RAlnMB->SetEnabled(false);
            CM[c][r].CM_RAlnSB->SetEnabled(false);

            CM[c][r].CM_LDsp1 ->SetEnabled(false);
            CM[c][r].CM_LDsp2 ->SetEnabled(false);


            CM[c][r].CM_RHgtCheck ->SetEnabled(false);
        }
    }

    //if(OM.DevOptn.iDispSeq != 2){
    //    for(int r = 0; r < MAX_CMOS_ROW ; r++){
    //        for(int c = 0; c < MAX_CMOS_COL; c++){
    //            CM[c][r].CM_LDsp2 ->SetEnabled(false);
    //        }
    //    }
    //}

    /*
    for(int r = 0; r < MAX_CMOS_ROW - 1 ; r++){
        for(int c = _iCol; c < MAX_CMOS_COL; c++){
            HG[c][r].HG_RGapM ->SetEnabled(false);
            HG[c][r].HG_RGapS ->SetEnabled(false);
        }
    }
    for(int r = _iRow; r < MAX_CMOS_ROW - 1; r++){
        for(int c = 0; c < MAX_CMOS_COL; c++){
            HG[c][r].HG_RGapM ->SetEnabled(false);
            HG[c][r].HG_RGapS ->SetEnabled(false);
        }
    }

    for(int r = 0; r < MAX_CMOS_ROW  ; r++){
        for(int c = _iCol; c < MAX_CMOS_COL - 1; c++){
            VG[c][r].VG_RGapM ->SetEnabled(false);
            VG[c][r].VG_RGapS ->SetEnabled(false);
        }
    }
    for(int r = 0; r < MAX_CMOS_ROW  ; r++){
        for(int c = 0; c < MAX_CMOS_COL - 1; c++){
            VG[c][r].VG_RGapM ->SetEnabled(false);
            VG[c][r].VG_RGapS ->SetEnabled(false);
        }
    }
    */
}

int CCadPositionStg::GetRow()
{
    return m_iMaxRow ;
}

int CCadPositionStg::GetCol()
{
    return m_iMaxCol ;
}

void CCadPositionStg::ShiftAll(double _dX , double _dY)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    double dTempX , dTempY ;
    while(Obj){
        if(!Obj -> GetEnabled()) {
            Obj = Obj -> GetPreObject() ;
            continue ;
        }
        if(Obj -> GetType() == ctCntPoint){
            Obj = Obj -> GetPreObject() ;
            continue ;
        }
        dTempX = Obj -> GetCntrPosX();
        dTempY = Obj -> GetCntrPosY();
        Obj -> SetCntrPosX(dTempX + _dX);
        Obj -> SetCntrPosY(dTempY + _dY);

        Obj = Obj -> GetPreObject() ;
    }

}


void CCadPositionStg::SetVisibleAllSub(bool _bVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        if(Obj -> GetType() != ctCntPoint && Obj -> GetType() != ctRect) {
            Obj -> SetVisible(_bVal);


        }

        Obj = Obj -> GetPreObject();
        continue ;
    }

}




//==============================================================================
//로테이터
//==============================================================================
CCadPositionRtt CAD_Rtt ;
CCadPositionRtt::CCadPositionRtt()
{
}

CCadPositionRtt::~CCadPositionRtt()
{
}

void CCadPositionRtt::Init()
{
    m_eCoodination = cnRtt ;

    m_pSelectedObj = NULL ;
    m_pMouseInObj  = NULL ;

    m_bSelectedPnt = false ;
    m_bMouseInPnt  = false ;


    m_hDc = NULL ;        //= NULL;
    m_iPanelWidth  = 0;//=0 ;
    m_iPanelHeight = 0;//=0 ;

    m_dCoodWidth  = 400 ;
    m_dCoodHeight = 400 ;


    String sTemp = "";

    CT = new CCadCntPoint(cnRtt ,"CT");
    CT->SetColor(clBlue);
    CT->SetCntrPosX(m_dCoodWidth /2.0);
    CT->SetCntrPosX(m_dCoodHeight/2.0);

    CM.CM_RHgtCheck  = new CCadHghtCmd  (cnRtt ,"CM_RHgtCheck");

    CM.CM_Rect = new CCadRect     (cnRtt ,"CM_Rect");

    LoadLastInfo() ;
    LoadSave(true , m_sLastDev);

}

void CCadPositionRtt::Close()
{
    delete CT ;
    delete CM.CM_RHgtCheck ;

    delete CM.CM_Rect      ;
}


void CCadPositionRtt::SetSelectedObject(CCadObject * _pObject)
{
    m_pSelectedObj = _pObject ;
}

CCadObject * CCadPositionRtt::GetSelectedObject()
{
    return m_pSelectedObj ;
}

CCadObject * CCadPositionRtt::FindObjectByName(String _sName)
{
    return CCadObject::FindObjectByName(cnRtt , _sName) ;
}
String CCadPositionRtt::GetNameList()
{
    return CCadObject::GetNameList(cnRtt);
}

void CCadPositionRtt::Paint(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    m_hDc = _hDc ;
    //m_iPanelWidth  = _iPanelWidth  ;
    //m_iPanelHeight = _iPanelHeight ;

    double dScaleX = GetScaleX();
    double dScaleY = GetScaleY();

    //CGdi * Gdi = new CGdi(m_hDc , dScaleX , dScaleY , GetCoodCntX() , GetCoodCntY());
    CGdi * Gdi = new CGdi(_hDc , _dScaleX , _dScaleY , GetCoodCntX()-_dOffsetX , GetCoodCntY()-_dOffsetY);

    //Gdi -> m_tPen.Color = clBlue ;
    //Gdi -> LinePlus(0,0, 400);

    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(cnRtt) ;
    while(Obj){
        Gdi -> m_tPen.Style  = psSolid ;
        if(Obj == m_pSelectedObj) {
            Gdi -> m_tPen.Color  = Obj -> GetColor();
            Gdi -> m_tText.Color = Obj -> GetColor();
            Gdi -> m_tPen.Width  = 2 ;
        }
        else if(Obj == m_pMouseInObj){
            Gdi -> m_tPen.Color  = Obj -> GetColor();
            Gdi -> m_tText.Color = Obj -> GetColor();
            Gdi -> m_tPen.Width  = 1 ;
        }

        else {
            Gdi -> m_tPen.Color  = clSilver ;
            Gdi -> m_tText.Color = clSilver ;
            Gdi -> m_tPen.Width  = 1 ;

        }
        Obj -> Paint(Gdi);
        Obj = Obj -> GetPreObject() ;
    }

    if(m_bSelectedPnt){
        Gdi -> m_tPen.Width = 2 ;
        Gdi -> m_tPen.Color = clLime ;
        Gdi -> LineX(m_tSelectedPnt.dPosX,m_tSelectedPnt.dPosY,3);
        Gdi -> m_tPen.Width = 1 ;
        Gdi -> m_tPen.Color = clRed ;
        Gdi -> LineX(m_tSelectedPnt.dPosX,m_tSelectedPnt.dPosY,2);
    }
    if(m_bMouseInPnt){
        Gdi -> m_tPen.Width = 2 ;
        Gdi -> m_tPen.Color = clAqua ;
        Gdi -> LineX(m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY,3);
        Gdi -> m_tPen.Width = 1 ;
        Gdi -> m_tPen.Color = clRed ;
        Gdi -> LineX(m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY,2);
    }

    delete Gdi;

    Gdi = new CGdi(_hDc );

    if(m_bMouseInPnt && m_bSelectedPnt){
        double dXGap = m_tMouseInPnt.dPosX - m_tSelectedPnt.dPosX ;
        double dYGap = m_tMouseInPnt.dPosY - m_tSelectedPnt.dPosY ;
        String sTemp ; //= String("X:") + () + " Y:" + ();
        sTemp.sprintf("SelX:%0.3f SelY:%0.3f MouseX:%0.3f MouseY:%0.3f X:%0.3f Y:%0.3f",m_tSelectedPnt.dPosX , m_tSelectedPnt.dPosY ,m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY , dXGap , dYGap);
        Gdi -> Text(5 , 5 , sTemp.c_str());
    }
    else if(m_bMouseInPnt){
        String sTemp ; //= String("X:") + () + " Y:" + ();
        sTemp.sprintf("MouseX:%0.3f MouseY:%0.3f",m_tMouseInPnt.dPosX,m_tMouseInPnt.dPosY );
        Gdi -> Text(5 , 5 , sTemp.c_str());
    }
    else if(m_bSelectedPnt){
        String sTemp ; //= String("X:") + () + " Y:" + ();
        sTemp.sprintf("SelX:%0.3f SelY:%0.3f",m_tSelectedPnt.dPosX,m_tSelectedPnt.dPosY );
        Gdi -> Text(5 , 5 , sTemp.c_str());
    }

    delete Gdi;
}

void CCadPositionRtt::LoadLastInfo()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath= EXE_FOLDER + "SeqData\\LastInfo.INI" ;

    //Load
    UserINI.Load(sPath, "LAST WORK INFO" , "Device"  , m_sLastDev );

    if (m_sLastDev == "") m_sLastDev  = "NONE";
}

void CCadPositionRtt::LoadSave(bool _bLoad, String _sDevice)
{
    String sPath = EXE_FOLDER + "JobFile\\" + _sDevice + "\\CadPos.RPP";

    LoadSaveFile(_bLoad , sPath);

}

void CCadPositionRtt::LoadSaveFile(bool _bLoad, String _sFilePath)
{
    TUserINI Ini ;

    int iColor ;
    //if(_bLoad){
    //    Ini.Load(_sFilePath , "Member" , "m_iMaxCol" , m_iMaxCol);
    //    Ini.Load(_sFilePath , "Member" , "m_iMaxRow" , m_iMaxRow);
    //}
    //else {
    //    Ini.Save(_sFilePath , "Member" , "m_iMaxCol" , m_iMaxCol);
    //    Ini.Save(_sFilePath , "Member" , "m_iMaxRow" , m_iMaxRow);
    //}

    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        if(Obj -> GetEnabled()) {
            Obj -> LoadSave(_bLoad , _sFilePath);
        }
        Obj = Obj -> GetPreObject() ;
    }
}

CCadObject * CCadPositionRtt::OnMouseDown(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    double dPosX , dPosY , dMargin;
    dPosX   = _dX - GetCoodCntX(); // * m_dCoodWidth /m_iPanelWidth  - GetCoodCntX();
    dPosY   = _dY - GetCoodCntY(); // * m_dCoodHeight/m_iPanelHeight - GetCoodCntY();

    dMargin = iMargin;///_dScaleX ;//    * m_dCoodWidth /m_iPanelWidth ;   //-34.5
                                                         //-125.13
    CCadObject * Obj ;

    Obj = CCadObject::GetLastObject(m_eCoodination) ;

    TCadPoint Pnt ;
    double dDist ;
    if(m_pSelectedObj) {
        if(m_pSelectedObj -> IsMousePntIn(dPosX , dPosY, dMargin , Pnt , dDist)){
            m_tSelectedPnt = Pnt ;
            m_bSelectedPnt = true ;
            return m_pSelectedObj ;
        }
    }
    while(Obj){
        if(Obj -> IsMouseIn(dPosX , dPosY, dMargin,dDist)) {
            m_pSelectedObj = Obj;
            m_tSelectedPnt.Clear();
            m_bSelectedPnt = false ;
            return m_pSelectedObj;
        }
        Obj = Obj -> GetPreObject() ;
    }
    //m_pSelectedObj = NULL ;
    //m_tSelectedPnt.Clear();
    //m_bSelectedPnt = false ;
    return NULL ;
}

void CCadPositionRtt::OnMouseMove(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    double dPosX , dPosY , dMargin;
    //dPosX   = _iX  * m_dCoodWidth /m_iPanelWidth  - GetCoodCntX();
    //dPosY   = _iY  * m_dCoodHeight/m_iPanelHeight - GetCoodCntY();

    dPosX   = _dX- GetCoodCntX();//_dOffsetX + _iX / _dScaleX ;  //-34.5
    dPosY   = _dY- GetCoodCntY();//_dOffsetY + _iY / _dScaleY ;  //-125.13
    dMargin = iMargin;///_dScaleX ;// * m_dCoodWidth /m_iPanelWidth ;

    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    m_tMouseInPnt.Clear() ;
    m_bMouseInPnt = false ;
    m_pMouseInObj = NULL ;

    TCadPoint Pnt ;
    //if(m_pSelectedObj) {
    //    if(m_pSelectedObj -> IsMousePntIn(dPosX , dPosY, dMargin , Pnt)){
    //        m_tMouseInPnt = Pnt            ;
    //        m_bMouseInPnt = true ;
    //        m_pMouseInObj = m_pSelectedObj ;
    //        return ;
    //    }
    //}
    double dDist ;
    while(Obj){
        if(Obj -> IsMousePntIn(dPosX , dPosY, dMargin,Pnt,dDist)) {
            m_tMouseInPnt = Pnt  ;
            m_bMouseInPnt = true ;
            m_pMouseInObj = Obj ;
        }
        if(Obj -> IsMouseIn(dPosX , dPosY, dMargin,dDist)) {
            m_pMouseInObj = Obj ;
        }
        Obj = Obj -> GetPreObject() ;
    }

}

void CCadPositionRtt::SetGainX(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetGainX(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionRtt::SetGainY(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetGainY(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionRtt::SetGainZ(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetGainZ(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionRtt::SetOffsetX(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetOffsetX(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionRtt::SetOffsetY(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetOffsetY(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionRtt::SetOffsetZ(double _dVal)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetOffsetZ(_dVal);
        Obj = Obj -> GetPreObject() ;
    }
}

void CCadPositionRtt::SetRowCol (int _iCol , int _iRow)
{
    m_iMaxCol = _iCol ;
    m_iMaxRow = _iRow ;


    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    while(Obj){
        Obj -> SetEnabled(true);
        Obj = Obj -> GetPreObject() ;
    }
}

int CCadPositionRtt::GetRow()
{
    return m_iMaxRow ;
}

int CCadPositionRtt::GetCol()
{
    return m_iMaxCol ;
}

void CCadPositionRtt::ShiftAll(double _dX , double _dY)
{
    CCadObject * Obj ;
    Obj = CCadObject::GetLastObject(m_eCoodination) ;
    double dTempX , dTempY ;
    while(Obj){
        if(!Obj -> GetEnabled()) {
            Obj = Obj -> GetPreObject() ;
            continue ;
        }
        if(Obj -> GetType() == ctCntPoint){
            Obj = Obj -> GetPreObject() ;
            continue ;
        }
        dTempX = Obj -> GetCntrPosX();
        dTempY = Obj -> GetCntrPosY();
        Obj -> SetCntrPosX(dTempX + _dX);
        Obj -> SetCntrPosY(dTempY + _dY);

        Obj = Obj -> GetPreObject() ;
    }

}




