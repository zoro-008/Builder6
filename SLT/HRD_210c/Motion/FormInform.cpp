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
���� �̺�Ʈ �߻� �����Դϴ�. 

�� ������ - 
������() -> OnCreate() -> OnShow() -> OnActivate() -> OnCanResize() -> OnConstrainedResize() 
-> OnResize() -> OnPaint() 

MainForm�̶� ���� Ctrl + Shift + F11(������Ʈ ������)�� ��� Form �ǿ� �ִ� MainForm�� ���ϴ� ���̱��� 

MainForm�� Close() �Լ� ȣ�� Ȥ�� X��ư Ŭ���� - 
OnCloseQuery() -> OnClose() -> OnHide() -> OnDestroy() 
  �߿��Ѱ� OnHide �����ε��� OnHide�� ȣ��Ǵ� �ñ�� TApplication �� �Ҹ�Ǵ� �����̶�� ���Դϴ�. 
  �� ������ �������� �κ��� �Ҹ�ɶ� �̷����ٴ� �������� 

MainForm�� �ƴѰ���� Close() �Լ� ȣ�� Ȥ�� X��ư Ŭ���� - 
OnCloseQuery() -> OnClose() -> OnHide() -> OnDeactivate() 
  MainForm�� ���ʹ� �޸� OnDeactivate�� ȣ��˴ϴ�. ���ϳ� �ٸ����� �޸𸮰� �Ұŵ��� �ʴ´ٴ� �������� 
  Destroy�̺�Ʈ�� �ް�����ôٸ�, �� delete ���� ���� ������ �޸� �Ұű��� ���� �ϰ�����ôٸ� 
  OnClose �̺�Ʈ �Լ��� Action = caFree; �� �߰��Ͻø� OnDestroy() �̺�Ʈ���� �̷����� delete ȿ���� �Ͼ�ϴ�. 

MainForm�� delete �� - 
OnHide() -> OnDestroy() 
  �̶��� TApplication�� �Ҹ���� �ʽ��ϴ�. �׷��Ƿ� ������� ������ ������ ���μ����� ����ְԵ����� 
  MainForm�� ��ư�� �ϳ� ����ż� delete this; �غ��ø� Ȯ���Ͻ� �� �־��~ 

MainForm�� �ƴѰ���� delete �� - 
OnHide() -> OnDestroy() 
  �����Ͻô� ��찡 �ִµ� OnCloseQuery()�� OnClose()�̺�Ʈ�� �߻����� �ʽ��ϴ�. 
  ������ �޸𸮸� ���Ÿ� OnClose�� OnCloseQuery�� �ڵ��Ͻ� ��� �ݵ��� �ش� �̺�Ʈ�� �߻��ϵ��� �ڵ��ϼž� ������. ������ OnDestroy�� �Ͻñ� �ٶ��ϴ�. 


TForm���� �Ҹ��� �Լ��� ������ ������? 

__fastcall ~TForm(); �� ����� �ֽø� �Ҹ����� ������ �Ͼ�ϴ�. 
�� ��쿡�� �Ҹ��� ->OnHide() -> OnDestroy() ������ �̺�Ʈ�� �߻��˴ϴ�. 

�Ҹ�ɶ��� ��쿡 �����Ͻø� �޸� ����â�� �� ������ �� ������ �̴ϴ�. &&

*/








