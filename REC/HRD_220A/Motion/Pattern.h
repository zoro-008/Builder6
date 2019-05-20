//---------------------------------------------------------------------------
#ifndef PatternH
#define PatternH
//---------------------------------------------------------------------------
#include <system.hpp>

#define MAX_PATTERN_POS 50

#define PANEL_DRAW_WIDTH 500 //Px
#define REAL_DRAW_WIDTH  50  //mm

class CPattern {
    private :
        double m_dXPos[MAX_PATTERN_POS] ; // 실제 포지션 값이 저장되어 있는 배열
        double m_dYPos[MAX_PATTERN_POS] ;

        //double m_dXScalePos[MAX_PATTERN_POS]; // 실제 스케일 적용된 포지션 값이 저장되어 있는 배열
        //double m_dYScalePos[MAX_PATTERN_POS];

        double m_dXScale     ; // 실제 스케일 값이 저장되어 있는 값
        double m_dYScale     ;

        double m_dXSttOffset ; // Node 1번 처음 시작 위치 X Offset mm
        double m_dYSttOffset ; //                                  mm

        double m_dSpeed      ; // Node 간 속도 제어를 하기 위해 설정

        int    m_iPosCnt     ; // 움직일 노드 갯수.

        double m_dAcc        ; // Dispensor Acc 속도
        double m_dDec        ; // Dispensor Dec 속도

        bool   m_bDispOn[MAX_PATTERN_POS]     ; // Dispensor 켜기 끄기.

        AnsiString m_sCrntDev ;


    public :
        CPattern() ;
        ~CPattern() ;

        void Init();

        // 포지션 값 관련
        double GetPosX      (int _iNo) ; // 순수한 X Pos 값을 가져 온다 (Form이 새로 열릴 때, Get으로 이 값을 가지고 와야함)
        double GetPosY      (int _iNo) ;
        double GetScalePosX (int _iNo) ; // X Scale Pos 을 가져 온다 GetPosX * Scale + Sttofset
        double GetScalePosY (int _iNo) ;
        double GetSpeed     (        ) ; // Speed 을 가져 온다
        double GetSttOffsetX(        ) ; // 스타트 위치 값 가져오기 위해
        double GetSttOffsetY(        ) ;
        double GetScaleX    (        ) ; // X Scale 을 가져 온다
        double GetScaleY    (        ) ;
        double GetAcc       (        ) ; // Acc 을 가져 온다
        double GetDec       (        ) ; // Dec 을 가져 온다
        bool   GetDispOn    (int _iNo) ; // Disp 켜기 끄기.. 가져 온다.

        void SetScale       (double _dX, double _dY) ; // X, Y의 Scale 값을 설정
        void SetSttOffset   (double _dX, double _dY) ; // Node 1번 처음 시작 위치 X Offset
        void SetSpeed       (double _dVal          ) ; // Speed 값을 설정
        void SetPosX        (int _iNo, double _dPos) ; // X Pos 값을 설정
        void SetPosY        (int _iNo, double _dPos) ;
        void SetPosCnt      (int _iCnt) ;
        void SetAccDec      (double _dAcc , double _dDec);
        void SetDispOn      (int _iNo , bool _bOn);

        void SetScalePosX(int _iNo) ; // 해당 Node 넘버에 Scale 값을 설정
        void SetScalePosY(int _iNo) ;

        void Load(AnsiString _sDevName) ;
        void Save(AnsiString _sDevName) ;

        //패턴 파일 단일 로딩 할때...
        void LoadPat(AnsiString _sFileName) ;
        void SavePat(AnsiString _sFileName) ;



        int  GetPosCnt();

        void LoadLastInfo() ;


};

//---------------------------------------------------------------------------
extern CPattern PTT ;
//---------------------------------------------------------------------------
#endif


