//---------------------------------------------------------------------------

#ifndef FormRsltH
#define FormRsltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "UtilDefine.h"
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmRslt : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnVisn;
        TPanel *pnGraph;
        TPanel *pnRslt;
        TStringGrid *sgDriveRslt;
        TTimer *tmUpdate;
        TStringGrid *sgTiltRslt;
        TLabel *lbPreRslt;
        TLabel *lbRslt;
        void __fastcall tmUpdateTimer(TObject *Sender);
        void __fastcall sgTiltRsltDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall sgTiltRsltClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmRslt(TComponent* Owner);
        void SetStgId(int _iStgId);

        void SetStringGrid();

        int m_iStgId ;

        int m_iPreWorkingStg ; //워킹중이였던것 엣지 잡기위해.


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmRslt *FrmRslt[MAX_STAGE_CNT];
extern TStringGrid *g_pLastSelectedStringGrid ;
//---------------------------------------------------------------------------
#endif
