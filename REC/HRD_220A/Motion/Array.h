//---------------------------------------------------------------------------
#ifndef ArrayH
#define ArrayH
#include <stdio.h>
//
//---------------------------------------------------------------------------
#include "Chip.h"
#include "SLogUnit.h"

#define CHECK_RETURN if(r<0 || c<0 || r>=m_iMaxRow || c>=m_iMaxCol)return

class CArrayData
{
    public:
        __fastcall  CArrayData(void);
        __fastcall ~CArrayData(void);

        CChip **CHPS ;                              //�ٲ��. 20140315�����.

    protected:

        int m_iMaxRow ;
        int m_iMaxCol ;

        AnsiString m_sName     ; //����� �̸� ex)PreBufferZone
        AnsiString m_sLotNo    ; //��� ���� �� �ѹ��� �ٿ� �ټ� �ִ�.
        AnsiString m_sID       ; //��Ʈ���� �ް��� No ���� No ;  m_sID/100 = �ް��� ī��Ʈ , m_sID%100 = ���� ī����.
        int        m_iStep     ; //��̿��� � �۾��� �Կ� �־ ����. �����͸� �ǵ帮�� �ʰ� �۾��� �ܰ踦 ��Ÿ���� ����.
        int        m_iSubStep  ; //��̿��� � �۾��� �Կ� �־ ����. �����͸� �ǵ帮�� �ʰ� �۾��� �ܰ踦 ��Ÿ���� ����.(Sub Step)
        bool       m_bLastStrip;

    protected:
        virtual void OnChange(){}

    public:
        void __fastcall SetName       (AnsiString _sName);

        void __fastcall SetID         (AnsiString _sID);
        AnsiString __fastcall GetID   (void);
        AnsiString __fastcall GetName (void);
        void __fastcall SetLotNo      (AnsiString _sLotNo);
        AnsiString __fastcall GetLotNo(void);
        void __fastcall SetStep       (int        _iStep );
        int  __fastcall GetStep       (void);
        void __fastcall SetSubStep    (int      _iSubStep);
        int  __fastcall GetSubStep    (void);

        void __fastcall ClearMap(void);

        void __fastcall FlipX();
        void __fastcall FlipY();
        void __fastcall FlipXY();
        void __fastcall TurnCw90();  //�ð� ����.
        void __fastcall TurnCw180();
        void __fastcall TurnCw270();
        void __fastcall SetMaxColRow (int c,int r);

        //Search Chip.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
        void __fastcall Load(bool IsLoad);


        //inline func
        //======================================================================
        bool __fastcall SetStat( int r , int c , EN_CHIP_STAT _iStat) {
            CHECK_RETURN false ;

            AnsiString sTemp ;
            sTemp.sprintf("R=%03d C=%03d To %02d",r,c,_iStat);
            String sHead = "SetStat "+GetName() ;
            Trace( sHead.c_str() , sTemp.c_str());
            CHPS[r][c].SetStat(_iStat);
            OnChange();
            return true;
        }
        void __fastcall SetStat(EN_CHIP_STAT _iStat) {
            String sHead = "SetStatAll "+GetName() ;
            Trace( sHead.c_str() , String(_iStat).c_str());

            for(int r = 0 ; r < m_iMaxRow ; r++) {
                for(int c = 0 ; c < m_iMaxCol ; c++) {
                    CHPS[r][c].SetStat(_iStat);
                }
            }

            if(_iStat == csNone){
                SetStep(0);
            }
            OnChange();

        }//{SetID(""); SetLotNo(""); SetStep(0);}} ���� ���� ������ ����...�δ�2�Ű��� �̻��϶� �����Ǽ� ����.

        void __fastcall SetData(int r, int c, EN_DATA_ID _iData, double _dData){
            CHPS[r][c].SetData(_iData, _dData);
            return ;
        }


        bool __fastcall RangeSetStat(int sr , int sc , int er , int ec , EN_CHIP_STAT _iStat){
            if(sr<0 || sc<0 || sr>=m_iMaxRow || sc>=m_iMaxCol)return false ;
            if(er<0 || ec<0 || er>=m_iMaxRow || ec>=m_iMaxCol)return false ;
            if(sr>er|| sc>ec                                 )return false ;
            for(int r = sr ; r <= er ; r++) {
                for(int c = sc ; c <= ec ; c++) {
                    CHPS[r][c].SetStat(_iStat);
                }
            }
            OnChange();
            return true ;
        }

        void __fastcall ChangeStat(EN_CHIP_STAT _iFrom , EN_CHIP_STAT _iTo) {
            for(int r = 0 ; r < m_iMaxRow ; r++)
                for(int c = 0 ; c < m_iMaxCol ; c++)
                    if(CHPS[r][c].GetStat() == _iFrom) CHPS[r][c].SetStat(_iTo);

            OnChange();
        }



        int  __fastcall GetMaxRow (void ) { return  m_iMaxRow ;}
        int  __fastcall GetMaxCol (void ) { return  m_iMaxCol ;}

        EN_CHIP_STAT __fastcall GetStat   (int r , int c                    ) { CHECK_RETURN csRetFail ; return CHPS[r][c].GetStat()         ; }
        bool         __fastcall CheckStat (int r , int c , EN_CHIP_STAT Stat) { CHECK_RETURN false     ; return GetStat(r,c) == Stat         ; }
        bool         __fastcall CheckExist(int r , int c                    ) { CHECK_RETURN false     ; return CHPS[r][c].CheckExist()      ; }
        bool         __fastcall CheckRst  (int r , int c                    ) { CHECK_RETURN false     ; return CHPS[r][c].CheckRst()        ; }
        double       __fastcall GetData   (int r , int c , EN_DATA_ID _iData) { CHECK_RETURN 0.0       ; return CHPS[r][c].GetData(_iData)   ; }

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

        //Sub Stat
        void __fastcall SetSubStat(int r , int c , int _iIdx , EN_CHIP_STAT _iSubStat) { CHPS[r][c].SetSubStat(_iIdx , _iSubStat); }
        EN_CHIP_STAT __fastcall GetSubStat(int r , int c , int _iIdx ) { return CHPS[r][c].GetSubStat(_iIdx); }
        void __fastcall SetSubStat(int _iIdx, EN_CHIP_STAT _iSubStat) {
            for(int r = 0 ; r < m_iMaxRow ; r++)
                for(int c = 0 ; c < m_iMaxCol ; c++)
                    CHPS[r][c].SetSubStat(_iIdx , _iSubStat);
        }
        //Overriding the Operator
        /*��� ������ UI�и��ϸ鼭 ������ �ʸ¾Ƽ� �Լ� �ٽ� ����.
        void operator  = (CArrayData& rhs) {
            SetMaxColRow(rhs.m_iMaxCol , rhs.m_iMaxRow) ;

            m_sID      = rhs.m_sID     ;
            m_sLotNo   = rhs.m_sLotNo  ;
            m_iStep    = rhs.m_iStep   ;
            m_iSubStep = rhs.m_iSubStep;

            //memcpy(CHPS , rhs.CHPS , sizeof(CChip) * m_iMaxCol * m_iMaxRow );

            for(int r = 0 ; r < m_iMaxRow ; r++){
                memcpy(CHPS[r],rhs.CHPS[r],sizeof(CChip)*m_iMaxCol);
            }

        }*/
        void CopyData(CArrayData& _cArray){
            SetMaxColRow(_cArray.m_iMaxCol , _cArray.m_iMaxRow) ;

            m_sID      = _cArray.m_sID     ;
            m_sLotNo   = _cArray.m_sLotNo  ;
            m_iStep    = _cArray.m_iStep   ;
            m_iSubStep = _cArray.m_iSubStep;

            for(int r = 0 ; r < m_iMaxRow ; r++){
                //memcpy(CHPS[r],_cArray.CHPS[r],sizeof(CChip)*m_iMaxCol);
                for(int c = 0 ; c < m_iMaxCol ; c++) CHPS[r][c] = _cArray.CHPS[r][c];

            }

            OnChange();
        }


};

//UI ��� �߰�.
class CArray : public CArrayData
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

        TColor m_iClStat[MAX_CHIP_STAT];

        TImage     * imArray ;
        TPopupMenu * pmStatus[MAX_STATUS_POPUP_ID] ;
        TMenuItem  * miStatus[MAX_STATUS_POPUP_ID][MAX_CHIP_STAT] ;

        AnsiString   m_sMenuName[MAX_CHIP_STAT];

        bool m_bSameCellSize ; //�������� ����.

        bool m_bDispLock ; //Display Lock

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


        void __fastcall UpdateArayNrm(); //��ũ�� ��� ���� �׳� ���������� ������ �׸�.
        void __fastcall UpdateArayFix(); //��ũ�⸦ �Ƚ� �ؼ� �׸��� �׸�.

        struct SClickStat {
            bool bMouseDown;
            int  iSelX1,iSelY1,iSelX2,iSelY2;
            int  iSelR1,iSelR2,iSelC1,iSelC2;
        }ClickStat;

    protected :
        virtual void OnChange(){
            //UpdateAray();
            //�̺�Ʈ������� ȭ�� ���� �Ϸ��ߴµ� �׸� �׸��ٰ� ���� �͵� �����.
            //530V���� ���� �ű�� ������ �� �߻�.
            //�ٽ� 530v���� UpdateAray ������� �ٲ�.
        }

    public:
        void __fastcall SetParent     (TPanel * _pnBase);
        void __fastcall SetCellSize   (bool _bSameCellSize);
        void __fastcall ClearDispItem ();
        void __fastcall SetDispItem   (EN_CHIP_STAT _iStat , AnsiString _sSubCaption ) ;
        void __fastcall SetDispColor  (EN_CHIP_STAT _iStat , TColor     _cColor      ) ;

        void __fastcall UpdateAray   (); //�ϴ� �ܺ� �＼�� ����.  �̺�Ʈ ��Ŀ��� �ٽ� �ٲ�.
        void SetDispLock(bool _bLock) {  //���Լ��� �ϴ� ���� ���µ� ���� �ʴ´�.
            //m_bDispLock = _bLock ;
            //if(!_bLock) UpdateAray();
        }

        //PopupMenu
        void __fastcall PopupMenuEnable(bool bState);

        int m_iTempDpCnt ;

};




//---------------------------------------------------------------------------
#endif


 /*
 CRITICAL_SECTION    csc; ������ �ɹ� ������ ���� ���� �۷ι��� ��������. 
�׸��� ũ��Ƽ�� ���ÿ��� �׻� �Ʒ��� ���� try __finally �� ó���� ������մϴ�. 
���� Entetr.. �Ŀ� Leave.. �ϱ� ���� error�� �߻��ϸ� Leave�� ȣ����� ���ϰ� �ǰ� 
���α׷��� ������� �ɸ��ϴ�. 
    try { 
        EnterCriticalSection(&csc); 
        ....... 
    } 
    __finally{ 
        LeaveCriticalSection(&csc); 
    }
    */








