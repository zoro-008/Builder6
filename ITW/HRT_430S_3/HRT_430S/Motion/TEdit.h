//---------------------------------------------------------------------------

#ifndef TEditH
#define TEditH

#include <Clipbrd.hpp>

//#include "Trace.h"
//---------------------------------------------------------------------------
// ���� �Է¸� �ްԵ� ������ TEdit ��ü�� ������Ʈ
// 	 �ݾ� �Է��̳�, ���б���� �Ҽ��� ���� ���ڸ� �Է��Ҷ� ����� �� �ִ�.
//	 �ǹ��� ���⿡ ����� ������ �����ϳ�, Ȥ�� �� ���״� ������ ���ذ��ñ�. 
//
// TEdit ������Ʈ���� TEditNumber ��� �ٲٸ� ������Ʈ�� �ν����ؼ� �����ν� ����ϴ� �͵� �����ϴ�.
// �ݵ�� UseApoint UseComma ������Ƽ �� �� �ϳ��� ����� ��. �� ���ٰ� �̻��� �ְų� �� ���� �ƴ�����.
//
//
// Written by ���¼�

class TEdit : public Stdctrls::TEdit			// ������Ʈ ��ü��
//class TEditNumber : public Stdctrls::TEdit    // ������Ʈ ��ġ��
{
private:
	bool	bNumberOnly;		// �����Է¸� �ް� ������ ���ΰ�?
	bool	bUseApoint;    		// �����Է¹��� �� '.' �� �Է� �㰡�� ���ΰ�?
	bool	bUseComma;			// �����Է¹��� �� �ڵ����� , �� �߰��� ���ΰ�?
	bool	bUseMinus;			// �����Է¹��� �� ���̳ʽ� ���� �Է� ���� ���ΰ�?

	TAlignment	FAlignment;		// ���� ��ġ ����.
private:
	// ���⼭ ���ڸ� �Է¹޵��� �����Ѵ�.
	MESSAGE bool __fastcall WMChar(Messages::TWMKey *msg)
	{
		USHORT& Key = msg->CharCode;
		if (Key >= '0' && Key <= '9')
			return true;
		if (Key == 8 || Key == 13)
			return true;
		if (bUseApoint && (Key == '.' && Text.Pos(".") == 0))
			return true;
		if (bUseMinus && (Key == '-' && Text.Pos("-") == 0 && SelStart == 0))	// -�� ó���� �Է°����ϰ�.
			return true;
		Key = 0;
		return false;
	}
	// ��Ʈ���ӿ� ���ڿ�� �ܴ� �����Ѵ�. '.'�� �� �ϳ��� ����Ѵ�.
	String	NumDataFilter(String str)
	{
		// ������ Ŭ���� ����ڴ� new char[] �� ���� �ʾ� malloc free�� ��ġ�Ѵ�.
		char *pbase = (char *)malloc(str.Length() + 1);
		char *p = pbase;
		*p = 0;
		for(int c = 1; c < str.Length() + 1; c++)
		{
			if (isdigit(str[c]))
				*p++ = str[c];
			else if (str[c] == '.' && (strchr(pbase, '.')==NULL && Text.Pos(".") == 0))	// .�� �ϳ��� ���
				*p++ = str[c];
			else if (str[c] == '-' && p == pbase && (strchr(pbase, '-')==NULL && Text.Pos("-") == 0))	// .�� �ϳ��� ���
				*p++ = str[c];
			*p = 0;
		}
		str = pbase;
		free(pbase);
		return str;
	}
	// ��Ʈ����  3ĭ���� �޸��� ���� ���·� ��ȯ�Ѵ�.
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
		if (ret.SubString(1,2) == "-,")		// - ���ΰ��� -,123 ������ �Ǵ� ��쵵 �־� �̸� ����. 
			ret.Delete(2,1);
		return ret;
	}
	// Change�� �Ͼ ���
	void __fastcall CMTextChanged(TMessage& Message)
	{
		String  org = Text;
		int  iSelStart = SelStart;
		int  iOrgLen = org.Length();
		String s = StringReplace(org, ",", "", TReplaceFlags() << rfReplaceAll);
		int  pos = s.Pos("-");
		if (pos >= 2)	// - �� �߰��� ������ �װ� �����Ѵ�.
			s = s.Delete(pos, 1);
		if (bUseComma) s = ConvertCommaString(s);
		Text = s;
		SelStart = s.Length() - iOrgLen + iSelStart; // ĳ�� ��ġ�� ���� ��ġ�� �����ϵ��� ����.
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
					// ���ڿ�ҿܴ� ���� �Ѵ�.
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

