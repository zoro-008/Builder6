/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.11.24                                                     */
/* Modify : 2004.11.24                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//---------------------------------------------------------------------------
#ifndef DpDataUnitH
#define DpDataUnitH

//---------------------------------------------------------------------------
struct TDpData {
    //Total
    int    iPassCnt   ;
    int    iTotalCnt  ;
    double dYield     ;

    double dAvrTime    ; //TDateTime AvrTime    ;//
    double dTotalTime  ; //TDateTime TotalTime  ;//

    double dWorkTime   ;
    double dAdjTime    ; //TDateTime AdjTime    ;//
    double dBondTime   ; //TDateTime BondTime   ;//
    double dUVTime     ; //TDateTime UVTime     ;//
    double dWaitTime   ; //TDateTime WaitTime   ;//
};

//---------------------------------------------------------------------------
class CDpData
{
    public:
        __fastcall  CDpData(void);
        __fastcall ~CDpData(void);

        TDpData Data ;

        bool m_bWorkStart ;

        void __fastcall ClearEachData ();
        void __fastcall ClearTotalData();

        void __fastcall SaveData(AnsiString _sID);
        void __fastcall LoadData(AnsiString _sID);

};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
