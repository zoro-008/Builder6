//---------------------------------------------------------------------------

#ifndef CadPositionH
#define CadPositionH
#include "UnitGdi.h"
#include "UserIni.h"
//---------------------------------------------------------------------------
enum EN_COORDINATION {
    cnStg = 0 , //스테이지
    cnRtt     , //로테이터
    MAX_COORDINATION
};

enum EN_CADOBJECT_TYPE {
    ctObject = 0 ,
    ctPoint      ,
    ctCntPoint   ,
    ctVisnPoint  ,
    ctDspCmd     ,
    ctLine       ,
    ctHeight     ,
    ctRect       ,
    ctHghtCmd    ,
    ctRhombus    ,

    MAX_CADOBJECT_TYPE
};

struct TCadPoint{
    TCadPoint(){
        Clear();
    }
    ~TCadPoint(){
    }
    void Clear(){
        dPosX  = 0.0 ;
        dPosY  = 0.0 ;
        dPosZ  = 0.0 ;
    }
    double dPosX ;
    double dPosY ;
    double dPosZ ;

};

class CCadObject
{
    public :
        CCadObject(EN_COORDINATION _eCood ,String _sName);
        ~CCadObject();

//Static 공용함수.==============================================================
    private :
        static CCadObject * m_pLastObject[MAX_COORDINATION]  ;

    public :
        static CCadObject * GetLastObject(EN_COORDINATION _eCood);
        static CCadObject * FindObjectByName(EN_COORDINATION _eCood , String _sName);
        static String GetNameList(EN_COORDINATION _eCood);

//==============================================================================

    protected:
        CCadObject * m_pPreObject  ;

        String m_sName    ;
        TColor m_eColor   ;

        EN_CADOBJECT_TYPE m_eType ;

        bool   m_bEnabled ;
        bool   m_bVisible ;

        double m_dOffsetX ; double m_dGainX   ;
        double m_dOffsetY ; double m_dGainY   ;
        double m_dOffsetZ ; double m_dGainZ   ;


    public :
        CCadObject * GetPreObject();

        String GetName() ;
        void   SetName(String _sName) ;

        TColor GetColor();
        void   SetColor(TColor _eVal);

        EN_CADOBJECT_TYPE GetType();

        bool   GetEnabled();
        void   SetEnabled(bool _bVal);

        bool   GetVisible();
        void   SetVisible(bool _bVal);

        void   SetOffsetX(double _dVal); double GetOffsetX();
        void   SetOffsetY(double _dVal); double GetOffsetY();
        void   SetOffsetZ(double _dVal); double GetOffsetZ();
        void   SetGainX  (double _dVal); double GetGainX  ();
        void   SetGainY  (double _dVal); double GetGainY  ();
        void   SetGainZ  (double _dVal); double GetGainZ  ();

    public :
        virtual void   SetCntrPosX(double _dVal)=0;
        virtual void   SetCntrPosY(double _dVal)=0;

        virtual double GetCntrPosX(bool _bRaw=false)=0;
        virtual double GetCntrPosY(bool _bRaw=false)=0;

        virtual void   Paint(CGdi * _pGdi)=0;
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist)=0;
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist)=0;
        virtual void   LoadSave(bool _bLoad , String _sFilePath);
};


class CCadPoint : public CCadObject {
    public :
        CCadPoint(EN_COORDINATION _eCood ,String _sName);
        ~CCadPoint();

    protected:
        double m_dPosX ;
        double m_dPosY ;

    public :

        //virtual.
        virtual void   SetCntrPosX(double _dVal);
        virtual void   SetCntrPosY(double _dVal);
        virtual double GetCntrPosX(bool _bRaw=false);
        virtual double GetCntrPosY(bool _bRaw=false);
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
        virtual void   LoadSave(bool _bLoad , String _sFilePath);

};

class CCadCntPoint : public CCadPoint {
    public :
        CCadCntPoint(EN_COORDINATION _eCood ,String _sName);
        ~CCadCntPoint();

    protected:
    public:
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
};

class CCadVisnPoint : public CCadPoint {
    public :
        CCadVisnPoint(EN_COORDINATION _eCood ,String _sName);
        ~CCadVisnPoint();

    protected:
    public:
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
};

#define MAX_HGHT_CMD 200
class CCadHghtCmd : public CCadObject {
    public :
        CCadHghtCmd(EN_COORDINATION _eCood ,String _sName);
        ~CCadHghtCmd();

    protected:
        //double m_dLeft   ; double m_dTop    ;
        //double m_dWidth  ; double m_dHeight ;
        
        int    m_iNodeCnt  ;
        struct THghtPos {
            double dPosX   ;
            double dPosY   ;
            double dPosZ   ;
        } HghtPos[MAX_HGHT_CMD];

    public:
        //double GetLeft   (                      ); void   SetLeft   (double _dVal);
        //double GetTop    (                      ); void   SetTop    (double _dVal);
        //double GetWidth  (                      ); void   SetWidth  (double _dVal);
        //double GetHeight (                      ); void   SetHeight (double _dVal);

        //double GetRight  (                      ); void   SetRight  (double _dVal);
        //double GetBottom (                      ); void   SetBottom (double _dVal);

        void   SetNodeCnt(int _iVal             ); int    GetNodeCnt(            );
        void   SetPosX   (int _iId ,double _dVal); double GetPosX   (int _iId ,bool _bRaw=false );
        void   SetPosY   (int _iId ,double _dVal); double GetPosY   (int _iId ,bool _bRaw=false );
        void   SetPosZ   (int _iId ,double _dVal); double GetPosZ   (int _iId ,bool _bRaw=false );

        //virtual.
        virtual void   SetCntrPosX(double _dVal);
        virtual void   SetCntrPosY(double _dVal);
        virtual double GetCntrPosX(bool _bRaw=false);
        virtual double GetCntrPosY(bool _bRaw=false);
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
        virtual void   LoadSave(bool _bLoad , String _sFilePath);

};

#define MAX_DSP_CMD 200
class CCadDspCmd : public CCadObject {
    public :
        CCadDspCmd(EN_COORDINATION _eCood ,String _sName);
        ~CCadDspCmd();

    protected:
        int    m_iNodeCnt  ;
        struct TDspPos {
            double dPosX   ;
            double dPosY   ;
            double dPosZ   ;
            int    iDelay  ;
            double dSpeed  ;
            double dUVSpeed;
            bool   bDispOn ;
        } DspPos[MAX_DSP_CMD];

    public:
        void   SetNodeCnt(int _iVal             ); 
        void   SetPosX   (int _iId ,double _dVal);
        void   SetPosY   (int _iId ,double _dVal); 
        void   SetPosZ   (int _iId ,double _dVal); 
        void   SetDelay  (int _iId ,int    _iVal); 
        void   SetSpeed  (int _iId ,double _dVal);
        void   SetUVSpeed(int _iId ,double _dVal);
        void   SetDispOn (int _iId ,bool   _dVal);

        int    GetNodeCnt(            );
        double GetPosX   (int _iId ,bool _bRaw=false );
        double GetPosY   (int _iId ,bool _bRaw=false );
        double GetPosZ   (int _iId ,bool _bRaw=false );
        int    GetDelay  (int _iId );
        double GetSpeed  (int _iId );
        double GetUVSpeed(int _iId );
        bool   GetDispOn (int _iId );

        //virtual.
        virtual void   SetCntrPosX(double _dVal);
        virtual void   SetCntrPosY(double _dVal);
        virtual double GetCntrPosX(bool _bRaw=false);
        virtual double GetCntrPosY(bool _bRaw=false);
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
        virtual void   LoadSave(bool _bLoad , String _sFilePath);

};

class CCadLine : public CCadObject {
    public :
        CCadLine(EN_COORDINATION _eCood ,String _sName);
        ~CCadLine();

    protected:
        double m_dSttPosX ; double m_dSttPosY ;
        double m_dEndPosX ; double m_dEndPosY ;

    public:
        double GetSttPosX(bool _bRaw=false); double GetSttPosY(bool _bRaw=false);
        double GetEndPosX(bool _bRaw=false); double GetEndPosY(bool _bRaw=false);

        void SetSttPosX(double _dVal); void SetSttPosY(double _dVal);
        void SetEndPosX(double _dVal); void SetEndPosY(double _dVal);



        //virtual.
        virtual void   SetCntrPosX(double _dVal);
        virtual void   SetCntrPosY(double _dVal);
        virtual double GetCntrPosX(bool _bRaw=false);
        virtual double GetCntrPosY(bool _bRaw=false);
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
        virtual void   LoadSave(bool _bLoad , String _sFilePath);

};

class CCadHeight : public CCadObject {
    public :
        CCadHeight(EN_COORDINATION _eCood ,String _sName);
        ~CCadHeight();

    protected:
        double m_dSttPosX ; double m_dSttPosY ;
        double m_dEndPosX ; double m_dEndPosY ;

        double m_dSubPosX ; double m_dSubPosY ;

    public :
        double GetSttPosX(bool _bRaw=false); double GetSttPosY(bool _bRaw=false);
        double GetEndPosX(bool _bRaw=false); double GetEndPosY(bool _bRaw=false);
        double GetSubPosX(bool _bRaw=false); double GetSubPosY(bool _bRaw=false);

        void SetSttPosX(double _dVal); void SetSttPosY(double _dVal);
        void SetEndPosX(double _dVal); void SetEndPosY(double _dVal);
        void SetSubPosX(double _dVal); void SetSubPosY(double _dVal);

        //virtual.
        virtual void   SetCntrPosX(double _dVal);
        virtual void   SetCntrPosY(double _dVal);
        virtual double GetCntrPosX(bool _bRaw=false);
        virtual double GetCntrPosY(bool _bRaw=false);
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
        virtual void   LoadSave(bool _bLoad , String _sFilePath);

};

class CCadRect : public CCadObject {
    public :
        CCadRect(EN_COORDINATION _eCood ,String _sName);
        ~CCadRect();

    protected:
        double m_dLeft   ; double m_dTop    ;
        double m_dWidth  ; double m_dHeight ;
        //bool   m_bFill   ;

    public :
        double GetLeft   (bool _bRaw=false);  void SetLeft   (double _dVal);
        double GetTop    (bool _bRaw=false);  void SetTop    (double _dVal);
        double GetWidth  ();                  void SetWidth  (double _dVal);
        double GetHeight ();                  void SetHeight (double _dVal);

        double GetRight  (bool _bRaw=false);  void SetRight  (double _dVal);
        double GetBottom (bool _bRaw=false);  void SetBottom (double _dVal);

        bool IsPntRectInOfsGain(double _dX , double _dY);



        //virtual.
        virtual void   SetCntrPosX(double _dVal);
        virtual void   SetCntrPosY(double _dVal);
        virtual double GetCntrPosX(bool _bRaw=false);
        virtual double GetCntrPosY(bool _bRaw=false);
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
        virtual void   LoadSave(bool _bLoad , String _sFilePath);




};

class CCadRhombus : public CCadRect {
    public :
        CCadRhombus(EN_COORDINATION _eCood ,String _sName);
        ~CCadRhombus();

    protected:
    public :
        //virtual.
        virtual void   Paint(CGdi * _pGdi);
        virtual bool   IsMouseIn(double _dX , double _dY, double _dMargin,double &_dDist);
        virtual bool   IsMousePntIn(double _dX , double _dY, double _dMargin , TCadPoint &_tInPnt,double &_dDist);
};

#define MAX_CMOS_COL     3  //6개가 현재 MAX인데 가로로 붙을지 세로로 붙을지 몰라서 2X2로 잡는다.
#define MAX_CMOS_ROW     3
#define MAX_CMOS_COL_GAP 1  //피치의 갯수.
#define MAX_CMOS_ROW_GAP 1
class CCadPositionStg {
    public :
        CCadPositionStg();
        ~CCadPositionStg();

        void Init();
        void Close();

    protected :
        EN_COORDINATION m_eCoodination ;

        String m_sLastDev ;

        CCadObject * m_pSelectedObj ;
        CCadObject * m_pMouseInObj  ;

        TCadPoint    m_tSelectedPnt ; bool m_bSelectedPnt ;
        TCadPoint    m_tMouseInPnt  ; bool m_bMouseInPnt  ;



        HDC m_hDc ;        //= NULL;
        int m_iPanelWidth  ;//=0 ;
        int m_iPanelHeight ;//=0 ;

        double m_dCoodWidth  ;//WORK_WIDTH  = 400 ;
        double m_dCoodHeight ;//WORK_HEIGHT = WORK_WIDTH ;

    public :
        //미들블럭.
        CCadCntPoint * CT ;
        struct TMiddleBlock {
            CCadRect      * MB_Rect     ; //미들블럭.
            CCadVisnPoint * MB_LOri     ; //미들블럭 오리엔테이션.
            CCadVisnPoint * MB_LAlnM    ; //미들블럭의 검사기준점 포지션.
            CCadVisnPoint * MB_LAlnS    ; //미들블럭의 검사보조점 포지션.
            CCadRhombus   * MB_RHgt     ; //미들블럭의 틸트상태및 높이 파악.
            CCadHghtCmd   * MB_RChk     ; //미들블럭 체크모드에서 측정 포지션

            //CCadHghtCmd   * MB_RHgtCheck; //미들블럭 심스 다붙이고 체크함.
        }MB;

        //시모스 작업들.
        //CCadRhombus * CM_RHgtFrst   ; //처음붙는 시모스의  틸트상태및 높이 파악.
        //CCadRhombus * CM_RHgtScnd   ; //처음붙는 시모스의  틸트상태및 높이 파악.
        struct TCmos{
            CCadRect      * PC_Rect           ;
            CCadRect      * CM_Rect           ;

            CCadVisnPoint * CM_LAlnM          ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * CM_LAlnS          ; //시모스 프리얼라인 검사의 보조점 포지션.

            CCadHeight    * CM_RHgtL          ; //시모스 높이측정 접합면
            CCadHeight    * CM_RHgtT          ; //시모스 높이측정 접합면
            CCadHeight    * CM_RHgtR          ; //시모스 높이측정 접합면
            CCadHeight    * CM_RHgtB          ; //시모스 높이측정 접합면

            CCadVisnPoint * CM_RAlnML         ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * CM_RAlnSL         ; //시모스 프리얼라인 검사의 보조점 포지션.
            CCadVisnPoint * CM_RAlnMT         ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * CM_RAlnST         ; //시모스 프리얼라인 검사의 보조점 포지션.
            CCadVisnPoint * CM_RAlnMR         ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * CM_RAlnSR         ; //시모스 프리얼라인 검사의 보조점 포지션.
            CCadVisnPoint * CM_RAlnMB         ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * CM_RAlnSB         ; //시모스 프리얼라인 검사의 보조점 포지션.

            CCadDspCmd    * CM_LDsp1          ; //미들블럭에 풀칠 하기.
            CCadDspCmd    * CM_LDsp2          ;

            CCadHghtCmd   * CM_RHgtCheck      ; //시모스 높이 측정용 패턴.

        }CM[MAX_CMOS_COL][MAX_CMOS_ROW];

        //Cmos와 Cmos사이에서 작업.
        //Vertical Gap
        /*
        struct  TVerGap{
            CCadVisnPoint * VG_RGapM          ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * VG_RGapS          ; //시모스 프리얼라인 검사의 보조점 포지션.
        }VG[MAX_CMOS_COL-1][MAX_CMOS_ROW];
        //Horizental Gap
        struct THorGap{
            CCadVisnPoint * HG_RGapM          ; //시모스 프리얼라인 검사의 기준점 포지션.
            CCadVisnPoint * HG_RGapS          ; //시모스 프리얼라인 검사의 보조점 포지션.
        }HG[MAX_CMOS_COL][MAX_CMOS_ROW-1];
        */

        int m_iMaxCol ;
        int m_iMaxRow ;

        void         SetSelectedObject(CCadObject * _pObject);
        CCadObject * GetSelectedObject();

        CCadObject * FindObjectByName(String _sName);
        String       GetNameList();
        void         Paint(HDC _hDc , double _dScaleX , double _dScaleY , double _dDispImgX , double _dDispImgY); // , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);

        void         LoadLastInfo() ;
        void         LoadSave    (bool _bLoad, String _sDevice  ) ;
        void         LoadSaveFile(bool _bLoad, String _sFilePath) ;

        CCadObject * OnMouseDown(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        void         OnMouseMove(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);


        void         SetGainX  (double _dVal);
        void         SetGainY  (double _dVal);
        void         SetGainZ  (double _dVal);
        void         SetOffsetX(double _dVal);
        void         SetOffsetY(double _dVal);
        void         SetOffsetZ(double _dVal);


        void         SetRowCol (int _iCol , int _iRow);
        int          GetRow    ();
        int          GetCol    ();

        void   ShiftAll(double _dX , double _dY);


        double GetCoodWidth() {return m_dCoodWidth  ;}
        double GetCoodHeight(){return m_dCoodHeight ;}

        double GetCoodCntX(){return m_dCoodWidth / 2.0;}
        double GetCoodCntY(){return m_dCoodHeight/ 2.0;}
        double GetScaleX()  {return m_iPanelWidth  /m_dCoodWidth  ;}
        double GetScaleY()  {return m_iPanelHeight /m_dCoodHeight ;}

        double GetSelectedPndX() {return m_tSelectedPnt.dPosX * m_pSelectedObj -> GetGainX() + m_pSelectedObj -> GetOffsetX();}
        double GetSelectedPndY() {return m_tSelectedPnt.dPosY * m_pSelectedObj -> GetGainY() + m_pSelectedObj -> GetOffsetY();}
        bool   IsPntSelected  () {return m_bSelectedPnt  ;}

        void   SetVisibleAllSub(bool _bVal);



};
extern CCadPositionStg CAD ;


class CCadPositionRtt {
    public :
        CCadPositionRtt();
        ~CCadPositionRtt();

        void Init();
        void Close();

    protected :
        EN_COORDINATION m_eCoodination ;

        String m_sLastDev ;

        CCadObject * m_pSelectedObj ;
        CCadObject * m_pMouseInObj  ;

        TCadPoint    m_tSelectedPnt ; bool m_bSelectedPnt ;
        TCadPoint    m_tMouseInPnt  ; bool m_bMouseInPnt  ;



        HDC m_hDc ;        //= NULL;
        int m_iPanelWidth  ;//=0 ;
        int m_iPanelHeight ;//=0 ;

        double m_dCoodWidth  ;//WORK_WIDTH  = 400 ;
        double m_dCoodHeight ;//WORK_HEIGHT = WORK_WIDTH ;

    public :
        //미들블럭.
        CCadCntPoint * CT ;

        //시모스 작업들.
        struct TCmos{
            CCadRect      * CM_Rect           ;
            CCadHghtCmd   * CM_RHgtCheck      ; //미들블럭 심스 다붙이고 체크함.
        }CM;

        int m_iMaxCol ;
        int m_iMaxRow ;

        void         SetSelectedObject(CCadObject * _pObject);
        CCadObject * GetSelectedObject();

        CCadObject * FindObjectByName(String _sName);
        String       GetNameList();
        void         Paint(HDC _hDc , double _dScaleX , double _dScaleY , double _dDispImgX , double _dDispImgY); // , int _iDstX, int _iDstY, int _iDstW, int _iDstH , int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH);

        void         LoadLastInfo() ;
        void         LoadSave    (bool _bLoad, String _sDevice  ) ;
        void         LoadSaveFile(bool _bLoad, String _sFilePath) ;
        
        CCadObject * OnMouseDown(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);
        void         OnMouseMove(double _dX , double _dY , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY);


        void         SetGainX  (double _dVal);
        void         SetGainY  (double _dVal);
        void         SetGainZ  (double _dVal);
        void         SetOffsetX(double _dVal);
        void         SetOffsetY(double _dVal);
        void         SetOffsetZ(double _dVal);


        void         SetRowCol (int _iCol , int _iRow);
        int          GetRow    ();
        int          GetCol    ();


        void   ShiftAll(double _dX , double _dY);


        double GetCoodWidth() {return m_dCoodWidth  ;}
        double GetCoodHeight(){return m_dCoodHeight ;}

        double GetCoodCntX(){return m_dCoodWidth / 2.0;}
        double GetCoodCntY(){return m_dCoodHeight/ 2.0;}
        double GetScaleX()  {return m_iPanelWidth  /m_dCoodWidth  ;}
        double GetScaleY()  {return m_iPanelHeight /m_dCoodHeight ;}

        double GetSelectedPndX() {return m_tSelectedPnt.dPosX ;}
        double GetSelectedPndY() {return m_tSelectedPnt.dPosY ;}
        bool   IsPntSelected  () {return m_bSelectedPnt  ;}


};

extern CCadPositionRtt CAD_Rtt ;

#endif
