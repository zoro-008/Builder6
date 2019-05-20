//---------------------------------------------------------------------------
#ifndef ArrayH
#define ArrayH

//---------------------------------------------------------------------------
#include "Chip.h"

#define MAX_ARRAY_COL  300     //Default Col Size of Tray. X.
#define MAX_ARRAY_ROW  300    //Default Row Size of Tray. Y.

#define CHECK_RETURN if(r<0 || c<0 || r>=m_iMaxRow || c>=m_iMaxCol)return

extern TColor     clStat   [MAX_CHIP_STAT];
//extern AnsiString sMenuName[MAX_CHIP_STAT];
class CArray
{
    public:
        __fastcall  CArray(void);
        __fastcall ~CArray(void);

    private:
        enum EN_STATUS_POPUP_ID {
            spOne  = 0 ,
            spCol      ,
            spRow      ,
            spAll      ,
            spDrag     ,

            MAX_STATUS_POPUP_ID
        };

        int m_iMaxRow ;
        int m_iMaxCol ;

        bool m_bSameCellSize   ; //셀의 크기를 모두 일치 시킨다.
        AnsiString m_sName     ; //어레이의 이름 ex)PreBufferZone
        AnsiString m_sLotNo    ; //장비에 따라 랏 넘버를 붙여 줄수 있다.
        AnsiString m_sID       ; //스트립의 메가진 No 슬롯 No ;  m_sID/100 = 메가진 카운트 , m_sID%100 = 슬롯 카운터.
        int        m_iStep     ; //어레이에서 어떤 작업을 함에 있어서 스텝. 데이터를 건드리지 않고 작업의 단계를 나타낼때 쓴다.
        int        m_iSubStep  ; //어레이에서 어떤 작업을 함에 있어서 스텝. 데이터를 건드리지 않고 작업의 단계를 나타낼때 쓴다.(Sub Step)
        bool       m_bLastStrip;


        TImage     * imArray ;
        TPopupMenu * pmStatus[MAX_STATUS_POPUP_ID] ;
        TMenuItem  * miStatus[MAX_STATUS_POPUP_ID][MAX_CHIP_STAT] ;

        AnsiString   sMenuName[MAX_CHIP_STAT];

        CChip CHPS[MAX_ARRAY_ROW][MAX_ARRAY_COL]; //나중에 동적 생성으로 바꾼다.

        void __fastcall MenuDrawItem(TObject *Sender, TCanvas *ACanvas,const TRect &ARect, bool Selected) ;

        void __fastcall OneClick (TObject *Sender);
        void __fastcall AllClick (TObject *Sender);
        void __fastcall RowClick (TObject *Sender);
        void __fastcall ColClick (TObject *Sender);
        void __fastcall DragClick(TObject *Sender);

        void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //TObject *Sender,
        void __fastcall ImageMouseMove(TObject *Sender,                      TShiftState Shift, int X, int Y); //TObject *Sender,
        void __fastcall ImageMouseUp  (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //TObject *Sender,

        void __fastcall ImageArayRC   (int X , int Y , int &R , int &C);
        void __fastcall ImageArayRCFix(int X , int Y , int &R , int &C);

        void __fastcall UpdateArayNrm(); //셀크기 상관 없이 그냥 일율적으로 나누는 그림.
        void __fastcall UpdateArayFix(); //셀크기를 픽스 해서 그리는 그림.

        struct SClickStat {
            bool bMouseDown;
            int  iSelX1,iSelY1,iSelX2,iSelY2;
            int  iSelR1,iSelR2,iSelC1,iSelC2;
        }ClickStat;

    public:
        void __fastcall SetParent     (TPanel * _pnBase);
        void __fastcall SetConfig     (bool _bSameCellSize , AnsiString _sName);
        void __fastcall SetID         (AnsiString _sID);
        AnsiString __fastcall GetID   (void);

        void __fastcall SetLotNo      (AnsiString _sLotNo);
        AnsiString __fastcall GetLotNo(void);

        void __fastcall SetStep       (int        _iStep );
        int  __fastcall GetStep       (void);

        void __fastcall SetSubStep    (int      _iSubStep);
        int  __fastcall GetSubStep    (void);


        void __fastcall ClearDispItem();
        void __fastcall SetDispItem  (EN_CHIP_STAT _iStat , AnsiString _sSubCaption ) ;
        void __fastcall SetDispColor (EN_CHIP_STAT _iStat , TColor     _cColor      ) ;

        void __fastcall UpdateAray   ();

        void __fastcall ClearMap(void);

        void __fastcall FlipX();
        void __fastcall FlipY();
        void __fastcall FlipXY();

        void __fastcall SetStat( int r , int c , EN_CHIP_STAT _iStat) { CHPS[r][c].SetStat(_iStat); }
        void __fastcall SetStat(EN_CHIP_STAT _iStat) {
            for(int r = 0 ; r < m_iMaxRow ; r++) {
                for(int c = 0 ; c < m_iMaxCol ; c++) {
                    CHPS[r][c].SetStat(_iStat);
                }
            }
            if(_iStat == csNone) {
                SetStep(0);
            }
        }//{SetID(""); SetLotNo(""); SetStep(0);}} 별로 쓸일 없을거 같음...로더2매거진 이상일때 문제되서 삭제.
        bool __fastcall SetVf( int r , int c , double _dVf) {
            CHECK_RETURN false ;
            CHPS[r][c].SetVf(_dVf);
            return true;
        }
        //Vf Value를 입력하게 하여 값을 가지고 있는다.
        bool __fastcall SetVr( int r , int c , double _dVr) {
            CHECK_RETURN false ;
            CHPS[r][c].SetVr(_dVr);
            return true;
        }

        void __fastcall ChangeStat(EN_CHIP_STAT _iFrom , EN_CHIP_STAT _iTo) {
            for(int r = 0 ; r < m_iMaxRow ; r++) {
                for(int c = 0 ; c < m_iMaxCol ; c++) {
                    if(CHPS[r][c].GetStat() == _iFrom) {
                        CHPS[r][c].SetStat(_iTo);
                    }
                }
            }
        }
        void __fastcall ClearAllVf() {
            for(int r = 0 ; r < m_iMaxRow ; r++) {
                for(int c = 0 ; c < m_iMaxCol ; c++) {
                    CHPS[r][c].SetVf(0);
                }
            }
        }

        void __fastcall ClearAllVr() {
            for(int r = 0 ; r < m_iMaxRow ; r++) {
                for(int c = 0 ; c < m_iMaxCol ; c++) {
                    CHPS[r][c].SetVr(0);
                }
            }
        }

        void __fastcall SetFailStat(int r , int c , EN_FAIL_STAT _iFsStat) { CHPS[r][c].SetFailStat(_iFsStat); }

        void __fastcall SetMaxColRow (int c,int r) { m_iMaxCol = c; m_iMaxRow = r;}
        void __fastcall SetMaxRow    (int n      ) { m_iMaxRow = n     ;}
        void __fastcall SetMaxCol    (int n      ) { m_iMaxCol = n     ;}

        int  __fastcall GetMaxRow (void ) { return  m_iMaxRow ;}
        int  __fastcall GetMaxCol (void ) { return  m_iMaxCol ;}

        EN_CHIP_STAT __fastcall GetStat    (int r , int c                    ) { return CHPS[r][c].GetStat()           ; }
        bool         __fastcall CheckStat  (int r , int c , EN_CHIP_STAT Stat) { return            GetStat(r,c) == Stat; }
        bool         __fastcall CheckExist (int r , int c                    ) { return CHPS[r][c].CheckExist()        ; }
        bool         __fastcall CheckRst   (int r , int c                    ) { return CHPS[r][c].CheckRst()          ; }

        double       __fastcall GetVf      (int r , int c                    ) { CHECK_RETURN 0         ; return CHPS[r][c].GetVf()     ; }
        double       __fastcall GetVr      (int r , int c                    ) { CHECK_RETURN 0         ; return CHPS[r][c].GetVr()     ; }

        EN_FAIL_STAT __fastcall GetFailStat(int r , int c                    ) { return CHPS[r][c].GetFailStat()       ; }

        bool         __fastcall CheckAllStat (EN_CHIP_STAT Stat) { for(int r = 0 ; r < m_iMaxRow ; r++) for (int c = 0 ; c < m_iMaxCol ; c++) if (!CheckStat (r,c,Stat)) return false; return true;}
        bool         __fastcall CheckAllExist(                 ) { for(int r = 0 ; r < m_iMaxRow ; r++) for (int c = 0 ; c < m_iMaxCol ; c++) if (!CheckExist(r,c)     ) return false; return true;}

        //Get Row Count by ChipStatus.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int  __fastcall GetCntRowStat  (int r, EN_CHIP_STAT Stat) { int iCnt = 0; for (int c = 0 ; c < m_iMaxCol ; c++) if (CheckStat (r,c,Stat)) iCnt++; return iCnt; }
        int  __fastcall GetCntRowExist (int r                   ) { int iCnt = 0; for (int c = 0 ; c < m_iMaxCol ; c++) if (CheckExist(r,c)     ) iCnt++; return iCnt; }
        int  __fastcall GetCntRowRst   (int r                   ) { int iCnt = 0; for (int c = 0 ; c < m_iMaxCol ; c++) if (CheckRst  (r,c)     ) iCnt++; return iCnt; }

        //Get Col Count by ChipStatus.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int  __fastcall GetCntColStat  (int c, EN_CHIP_STAT Stat) { int iCnt = 0; for (int r = 0 ; r < m_iMaxRow ; r++) if (CheckStat (r,c,Stat)) iCnt++; return iCnt; }
        int  __fastcall GetCntColExist (int c                   ) { int iCnt = 0; for (int r = 0 ; r < m_iMaxRow ; r++) if (CheckExist(r,c)     ) iCnt++; return iCnt; }

        //Get All Count by ChipStatus.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int  __fastcall GetCntStat (EN_CHIP_STAT Stat) { int iCnt = 0; for (int r = 0 ; r < m_iMaxRow ; r++) iCnt += GetCntRowStat (r,Stat); return iCnt; }
        int  __fastcall GetCntExist(void             ) { int iCnt = 0; for (int r = 0 ; r < m_iMaxRow ; r++) iCnt += GetCntRowExist(r     ); return iCnt; }
        int  __fastcall GetCntRst  (void             ) { int iCnt = 0; for (int r = 0 ; r < m_iMaxRow ; r++) iCnt += GetCntRowRst  (r     ); return iCnt; }



        //Search Chip.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall FindChip          (EN_CHIP_STAT FindMode                   );
        bool __fastcall FindChip          (EN_CHIP_STAT FindMode , int  r , int  c );

        int  __fastcall FindFrstRow       (EN_CHIP_STAT FindMode                   );
        int  __fastcall FindFrstCol       (EN_CHIP_STAT FindMode                   );
        int  __fastcall FindLastRow       (EN_CHIP_STAT FindMode                   );
        int  __fastcall FindLastCol       (EN_CHIP_STAT FindMode                   );
        bool __fastcall FindFrstRowCol    (EN_CHIP_STAT FindMode , int &r , int &c );
        bool __fastcall FindFrstColRow    (EN_CHIP_STAT FindMode , int &r , int &c );
        bool __fastcall FindLastRowCol    (EN_CHIP_STAT FindMode , int &r , int &c );
        bool __fastcall FindFrstRowLastCol(EN_CHIP_STAT FindMode , int &r , int &c );
        bool __fastcall FindLastRowFrstCol(EN_CHIP_STAT FindMode , int &r , int &c );
        bool __fastcall FindLastColFrstRow(EN_CHIP_STAT _iStat   , int &r , int &c );
        bool __fastcall FindFrstColLastRow(EN_CHIP_STAT FindMode , int &r , int &c );
        bool __fastcall FindLastColRow    (EN_CHIP_STAT FindMode , int &r , int &c );

        bool __fastcall FindFrstRowCol_Indx     (EN_CHIP_STAT FindMode , int iStrCol , int iEndCol , int &R , int &C);
        bool __fastcall FindFrstRowLastCol_Indx (EN_CHIP_STAT FindMode , int iStrCol , int iEndCol , int &R , int &C);

        bool __fastcall FindFrstColRow_Indx     (EN_CHIP_STAT FindMode , int iStrCol , int iEndCol , int &R , int &C);
        bool __fastcall FindFrstColLastRow_Indx (EN_CHIP_STAT FindMode , int iStrCol , int iEndCol , int &R , int &C);

        //Loading Para.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall Load(bool IsLoad , FILE *fp , int _iNo);

        //PopupMenu
        void __fastcall PopupMenuEnable(bool bState);

        //Overriding the Operator
        void operator  = (CArray& rhs) {
            m_iMaxRow  = rhs.m_iMaxRow ;
            m_iMaxCol  = rhs.m_iMaxCol ;

            m_sID      = rhs.m_sID     ;
            m_sLotNo   = rhs.m_sLotNo  ;
            m_iStep    = rhs.m_iStep   ;
            m_iSubStep = rhs.m_iSubStep;

            memcpy(CHPS , rhs.CHPS , sizeof(CChip) * MAX_ARRAY_COL * MAX_ARRAY_ROW );
        }
};

//---------------------------------------------------------------------------
#endif
