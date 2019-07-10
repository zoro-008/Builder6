//---------------------------------------------------------------------------

#ifndef ProbeCommH
#define ProbeCommH

#include "Array.h"

extern double dCChipVf[MAX_ARRAY_ROW][MAX_ARRAY_COL];
extern double dCChipVr[MAX_ARRAY_ROW][MAX_ARRAY_COL];
extern double dPChipVf[MAX_ARRAY_ROW][MAX_ARRAY_COL];
extern double dPChipVr[MAX_ARRAY_ROW][MAX_ARRAY_COL];

class CProbeComm
{
    public:
        __fastcall  CProbeComm(void);
        __fastcall ~CProbeComm(void);

    private:
        int m_iMaxRow ;
        int m_iMaxCol ;

    public:
        void __fastcall SetMaxColRow   (int c,int r) { m_iMaxCol = c; m_iMaxRow = r;}
        int  __fastcall GetMaxRow      (void ) { return  m_iMaxRow ;}
        int  __fastcall GetMaxCol      (void ) { return  m_iMaxCol ;}

        void __fastcall ClearVf        () { memset(&dCChipVf, 0 , sizeof(double)*MAX_ARRAY_ROW*MAX_ARRAY_COL );
                                            memset(&dCChipVr, 0 , sizeof(double)*MAX_ARRAY_ROW*MAX_ARRAY_COL ); }
        void __fastcall ShiftArrayData ();
        void __fastcall CChip (int iC,int iR,double dVf,double dVr = 0);

        bool __fastcall GetData1Ch(BYTE *bData,int iLen);
        bool __fastcall GetData2Ch(BYTE *bData,int iLen);        
};
//---------------------------------------------------------------------------
extern CProbeComm PCM;
//---------------------------------------------------------------------------
#endif
