//---------------------------------------------------------------------------

#ifndef SOcrUnitH
#define SOcrUnitH

#include "STraceUnit.h"
//---------------------------------------------------------------------------
#define MAX_OCR_CHAR     100

#define NO_INSP_AREA     0
#define DARK_INSP_AREA   1
#define LIGHT_INSP_AREA  2





class SOcr : STrace {
    private :

    public  :
        struct TTrain : TRect {
            int      OcrBlobCnt            ;
            SOcrBlob OcrBlob[MAX_OCR_CHAR] ;
        } Train;

        struct TRslt {
           int        iResult         ;
           AnsiString sString         ;
           int        iCharCnt        ;
           float      fInspectionTime ;

           SLinkedList<TPoint> LtFailPx ;
           SLinkedList<TPoint> DkFailPx ;

        } Rslt;

        struct TPara : STrace::TPara{
           int iInspOffset ;
           int iDkIgnrCnt  ;
           int iLtIgnrCnt  ;

           int iLtOffset   ;
           int iDkOffset   ;

           int iFBlobLtPxCnt ;
           int iFTotlLtPxCnt ;

           int iFBlobDkPxCnt ;
           int iFTotlDkPxCnt ;

           bool bFitting     ;

        } Para;

        struct TFltr : STrace::TFltr{
        } Fltr ;

        //Functions
        SOcr();
        ~SOcr();

        int  OnTrain     (SImage * image , TRect rect , AnsiString string);
        void OnClearTrain(                                               );
        int  OnInspection(SImage * image , TRect rect , AnsiString string = "");
        void OnPaintRslt (HWND h);
        void OnPaintTrain(HWND h);
        bool OnLoad      (AnsiString FilePath);   //Load Train Para.
        bool OnSave      (AnsiString FilePath);
};
#endif
