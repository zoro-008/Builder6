//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormInform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmInform *FrmInform;
//---------------------------------------------------------------------------
__fastcall TFrmInform::TFrmInform(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


/*
폼의 이벤트 발생 순서입니다. 

폼 생성시 - 
생성자() -> OnCreate() -> OnShow() -> OnActivate() -> OnCanResize() -> OnConstrainedResize() 
-> OnResize() -> OnPaint() 

MainForm이라 함은 Ctrl + Shift + F11(프로젝트 관리자)를 열어서 Form 탭에 있는 MainForm을 말하는 것이구요 

MainForm의 Close() 함수 호출 혹은 X버튼 클릭시 - 
OnCloseQuery() -> OnClose() -> OnHide() -> OnDestroy() 
  중요한건 OnHide 부터인데요 OnHide가 호출되는 시기는 TApplication 이 소멸되는 시점이라는 것입니다. 
  이 사항의 공통적인 부분은 소멸될때 이뤄진다는 것이지요 

MainForm이 아닌경우의 Close() 함수 호출 혹은 X버튼 클릭시 - 
OnCloseQuery() -> OnClose() -> OnHide() -> OnDeactivate() 
  MainForm의 경우와는 달리 OnDeactivate가 호출됩니다. 또하나 다른것은 메모리가 소거되지 않는다는 점이지요 
  Destroy이벤트를 받고싶으시다면, 즉 delete 없이 폼이 닫힐때 메모리 소거까지 같이 하고싶으시다면 
  OnClose 이벤트 함수에 Action = caFree; 를 추가하시면 OnDestroy() 이벤트까지 이뤄지며 delete 효과가 일어납니다. 

MainForm을 delete 시 - 
OnHide() -> OnDestroy() 
  이때는 TApplication은 소멸되지 않습니다. 그러므로 윈도우는 보이지 않으나 프로세스는 살아있게되지요 
  MainForm에 버튼을 하나 만드셔서 delete this; 해보시면 확인하실 수 있어요~ 

MainForm이 아닌경우의 delete 시 - 
OnHide() -> OnDestroy() 
  오해하시는 경우가 있는데 OnCloseQuery()와 OnClose()이벤트는 발생하지 않습니다. 
  생성한 메모리를 제거를 OnClose나 OnCloseQuery에 코딩하실 경우 반듯이 해당 이벤트가 발생하도록 코딩하셔야 하지요. 가급적 OnDestroy에 하시기 바랍니다. 


TForm에는 소멸자 함수가 보이지 않지요? 

__fastcall ~TForm(); 을 만들어 주시면 소멸자의 동작은 일어납니다. 
이 경우에는 소멸자 ->OnHide() -> OnDestroy() 순으로 이벤트가 발생됩니다. 

소멸될때의 경우에 유념하시면 메모리 에러창을 좀 덜보실 수 있으실 겁니다. &&

*/








