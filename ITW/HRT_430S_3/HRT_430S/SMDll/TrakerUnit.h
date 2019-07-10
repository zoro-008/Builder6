//---------------------------------------------------------------------------

#ifndef TrakerUnitH
#define TrakerUnitH


#include <Classes.hpp>
#include <Controls.hpp>
#include <Graphics.hpp>
#include <Messages.hpp>
#include <Windows.hpp>

//---------------------------------------------------------------------------

class TRectTracker : public TGraphicControl//TCustomControl
{
private:
	//Graphics::TBitmap   *bm;            //WorkHorse internal bmp
	bool	m_bMIn;
	bool	m_bDrag;
	bool	m_bSelect;
	int		m_iMBMode;
    TPoint	m_MBDnPos;

	void __fastcall SetSelect(bool sel){m_bSelect=sel; Invalidate();};

	void _fastcall vDrawTracker(TCanvas *dc);

	virtual void __fastcall Paint(void);  //Override Paint - entire control is painted.
	MESSAGE void __fastcall CMMouseEnter(Messages::TMessage& Message);
	MESSAGE void __fastcall CMMouseLeave(Messages::TMessage& Message);
	MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMMouse &Message);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);

protected:

public:
	__fastcall TRectTracker(TComponent* Owner);
	__fastcall ~TRectTracker();

__published:

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER (CM_MOUSEENTER,  TMessage, CMMouseEnter)
		VCL_MESSAGE_HANDLER (CM_MOUSELEAVE,  TMessage, CMMouseLeave)
	END_MESSAGE_MAP(inherited);


	//__property bool Select = {read=m_bSelect, write=SetSelect};
};

#endif
