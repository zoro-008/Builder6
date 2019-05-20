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
        double m_dXPos[MAX_PATTERN_POS] ; // ���� ������ ���� ����Ǿ� �ִ� �迭
        double m_dYPos[MAX_PATTERN_POS] ;

        //double m_dXScalePos[MAX_PATTERN_POS]; // ���� ������ ����� ������ ���� ����Ǿ� �ִ� �迭
        //double m_dYScalePos[MAX_PATTERN_POS];

        double m_dXScale     ; // ���� ������ ���� ����Ǿ� �ִ� ��
        double m_dYScale     ;

        double m_dXSttOffset ; // Node 1�� ó�� ���� ��ġ X Offset mm
        double m_dYSttOffset ; //                                  mm

        double m_dSpeed      ; // Node �� �ӵ� ��� �ϱ� ���� ����

        int    m_iPosCnt     ; // ������ ��� ����.

        double m_dAcc        ; // Dispensor Acc �ӵ�
        double m_dDec        ; // Dispensor Dec �ӵ�

        bool   m_bDispOn[MAX_PATTERN_POS]     ; // Dispensor �ѱ� ����.

        AnsiString m_sCrntDev ;


    public :
        CPattern() ;
        ~CPattern() ;

        void Init();

        // ������ �� ����
        double GetPosX      (int _iNo) ; // ������ X Pos ���� ���� �´� (Form�� ���� ���� ��, Get���� �� ���� ������ �;���)
        double GetPosY      (int _iNo) ;
        double GetScalePosX (int _iNo) ; // X Scale Pos �� ���� �´� GetPosX * Scale + Sttofset
        double GetScalePosY (int _iNo) ;
        double GetSpeed     (        ) ; // Speed �� ���� �´�
        double GetSttOffsetX(        ) ; // ��ŸƮ ��ġ �� �������� ����
        double GetSttOffsetY(        ) ;
        double GetScaleX    (        ) ; // X Scale �� ���� �´�
        double GetScaleY    (        ) ;
        double GetAcc       (        ) ; // Acc �� ���� �´�
        double GetDec       (        ) ; // Dec �� ���� �´�
        bool   GetDispOn    (int _iNo) ; // Disp �ѱ� ����.. ���� �´�.

        void SetScale       (double _dX, double _dY) ; // X, Y�� Scale ���� ����
        void SetSttOffset   (double _dX, double _dY) ; // Node 1�� ó�� ���� ��ġ X Offset
        void SetSpeed       (double _dVal          ) ; // Speed ���� ����
        void SetPosX        (int _iNo, double _dPos) ; // X Pos ���� ����
        void SetPosY        (int _iNo, double _dPos) ;
        void SetPosCnt      (int _iCnt) ;
        void SetAccDec      (double _dAcc , double _dDec);
        void SetDispOn      (int _iNo , bool _bOn);

        void SetScalePosX(int _iNo) ; // �ش� Node �ѹ��� Scale ���� ����
        void SetScalePosY(int _iNo) ;

        void Load(AnsiString _sDevName) ;
        void Save(AnsiString _sDevName) ;

        //���� ���� ���� �ε� �Ҷ�...
        void LoadPat(AnsiString _sFileName) ;
        void SavePat(AnsiString _sFileName) ;



        int  GetPosCnt();

        void LoadLastInfo() ;


};

//---------------------------------------------------------------------------
extern CPattern PTT ;
//---------------------------------------------------------------------------
#endif


