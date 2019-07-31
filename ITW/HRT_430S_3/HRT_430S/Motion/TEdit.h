//---------------------------------------------------------------------------

#ifndef TEditH
#define TEditH

#include <Clipbrd.hpp>

//#include "Trace.h"
//---------------------------------------------------------------------------
// 숫자 입력만 받게도 가능한 TEdit 교체식 컴포넌트
// 	 금액 입력이나, 과학기술용 소수를 가진 숫자를 입력할때 사용할 수 있다.
//	 실무에 쓰기에 충분할 것으로 생각하나, 혹시 모를 버그는 적당히 피해가시길. 
//
// TEdit 컴포넌트명을 TEditNumber 라고 바꾸면 컴포넌트로 인스톨해서 디자인시 사용하는 것도 가능하다.
// 반드시 UseApoint UseComma 프로퍼티 둘 중 하나만 사용할 것. 다 쓴다고 이상이 있거나 한 것은 아니지만.
//
//
// Written by 김태성

class TEdit : public Stdctrls::TEdit			// 컴포넌트 교체식
//class TEditNumber : public Stdctrls::TEdit    // 컴포넌트 설치식
{
private:
	bool	bNumberOnly;		// 숫자입력만 받게 제한할 것인가?
	bool	bUseApoint;    		// 숫자입력받을 시 '.' 의 입력 허가할 것인가?
	bool	bUseComma;			// 숫자입력받을 시 자동으로 , 를 추가할 것인가?
	bool	bUseMinus;			// 숫자입력받을 시 마이너스 값도 입력 받을 것인가?

	TAlignment	FAlignment;		// 정렬 위치 조정.
private:
	// 여기서 숫자만 입력받도록 제한한다.
	MESSAGE bool __fastcall WMChar(Messages::TWMKey *msg)
	{
		USHORT& Key = msg->CharCode;
		if (Key >= '0' && Key <= '9')
			return true;
		if (Key == 8 || Key == 13)
			return true;
		if (bUseApoint && (Key == '.' && Text.Pos(".") == 0))
			return true;
		if (bUseMinus && (Key == '-' && Text.Pos("-") == 0 && SelStart == 0))	// -는 처음만 입력가능하게.
			return true;
		Key = 0;
		return false;
	}
	// 스트링속에 숫자요소 외는 제거한다. '.'는 단 하나만 허용한다.
	String	NumDataFilter(String str)
	{
		// 델파이 클래스 상속자는 new char[] 가 먹지 않아 malloc free로 대치한다.
		char *pbase = (char *)malloc(str.Length() + 1);
		char *p = pbase;
		*p = 0;
		for(int c = 1; c < str.Length() + 1; c++)
		{
			if (isdigit(str[c]))
				*p++ = str[c];
			else if (str[c] == '.' && (strchr(pbase, '.')==NULL && Text.Pos(".") == 0))	// .은 하나만 허용
				*p++ = str[c];
			else if (str[c] == '-' && p == pbase && (strchr(pbase, '-')==NULL && Text.Pos("-") == 0))	// .은 하나만 허용
				*p++ = str[c];
			*p = 0;
		}
		str = pbase;
		free(pbase);
		return str;
	}
	// 스트링에  3칸마다 콤마를 찍은 상태로 반환한다.
	String  ConvertCommaString(const String& str)
	{
		int  len,idx,iSub;
		String ret;

		len = str.Length();
		iSub = len + 1;
		if (len > 3)
			idx = len - 2;
		else
			idx = 0;
		while(idx > 1)
		{
			ret = "," + str.SubString(idx, iSub-idx) + ret;
			iSub = idx;
			idx = idx - 3;
		}
		if (iSub > idx)
			ret = str.SubString(1, iSub-1) + ret;
		if (ret.SubString(1,2) == "-,")		// - 값인경우는 -,123 식으로 되는 경우도 있어 이를 방지. 
			ret.Delete(2,1);
		return ret;
	}
	// Change가 일어난 경우
	void __fastcall CMTextChanged(TMessage& Message)
	{
		String  org = Text;
		int  iSelStart = SelStart;
		int  iOrgLen = org.Length();
		String s = StringReplace(org, ",", "", TReplaceFlags() << rfReplaceAll);
		int  pos = s.Pos("-");
		if (pos >= 2)	// - 가 중간에 있으면 그건 삭제한다.
			s = s.Delete(pos, 1);
		if (bUseComma) s = ConvertCommaString(s);
		Text = s;
		SelStart = s.Length() - iOrgLen + iSelStart; // 캐럿 위치가 원래 위치를 유지하도록 조정.
	}
protected:
	void __fastcall  SetEditAlignment(TAlignment align)
	{
		if (FAlignment == align)
			return;
		FAlignment = align;
		int    style = GetWindowLong(Handle, GWL_STYLE);
		switch(align)
		{
			case taLeftJustify :
				style = style & ~ES_RIGHT;
				break;
			case taRightJustify :
				style = style | ES_RIGHT;
				break;
			case taCenter :
				style = (style & ~ES_RIGHT) | ES_CENTER;
				break;
			default :
				return;
		}
		SetWindowLong(Handle, GWL_STYLE, style);
		Invalidate();
	}
	virtual void __fastcall Dispatch(void *Message)
	{
		TMessage  *msg = (PMessage)Message;
		switch (msg->Msg)
		{
			case WM_PASTE :
				if (bNumberOnly)
				{
					// 숫자요소외는 제거 한다.
					String  str = Clipboard()->AsText;
					Clipboard()->AsText = NumDataFilter(str);
				}
				break;
			case WM_CHAR :
				if (bNumberOnly)
					if (!WMChar((TWMKey*)msg))
						return;
				break;
			case CN_COMMAND :
				if (msg->WParamHi == EN_CHANGE)
				{
					if (bNumberOnly)
						CMTextChanged(*msg);
				}
				break;
		}
		Stdctrls::TEdit::Dispatch(Message);
	}
__published:
	__property bool NumberOnly = {read=bNumberOnly, write=bNumberOnly, default = false};
	__property bool UseApoint = {read=bUseApoint, write=bUseApoint, default = true};
	__property bool UseComma = {read=bUseComma, write=bUseComma, default = false};
	__property bool UseMinus = {read=bUseMinus, write=bUseMinus, default = false};
	__property TAlignment Alignment = {read=FAlignment, write=SetEditAlignment, default = taLeftJustify};

public:
	#pragma option push -w-inl
	/* TCustomEdit.Create */ inline __fastcall virtual TEdit(Classes::TComponent* AOwner) : Stdctrls::TEdit(AOwner) { NumberOnly = true ;  UseApoint = true ; UseMinus = true ; }
	#pragma option pop

public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TEdit(HWND ParentWindow) : Stdctrls::TEdit(ParentWindow) {NumberOnly = true ;  UseApoint = true ; UseMinus = true ; }
	#pragma option pop
	#pragma option push -w-inl
	/* TWinControl.Destroy */ inline __fastcall virtual ~TEdit(void) { }
	#pragma option pop
};

#define TEdit            ::TEdit
#endif

