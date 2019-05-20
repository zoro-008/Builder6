//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Motion\FormMain.cpp", FrmMain);
USEFORM("Motion\FormMaster.cpp", FrmMaster);
USEFORM("Motion\FormMsgYesNo.cpp", FrmMsgYesNo);
USEFORM("Motion\FormOperation.cpp", FrmOperation);
USEFORM("Motion\FormDevice.cpp", FrmDevice);
USEFORM("Motion\FormValve.cpp", FrmValve);
USEFORM("Motion\FormSpinMotor.cpp", FrmSpinMT);
USEFORM("Motion\FormTiltMotor.cpp", FrmTiltMT);
USEFORM("Motion\FormSetting.cpp", FrmSetting);
USEFORM("Motion\FormMsgOk.cpp", FrmMsgOk);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 Application->CreateForm(__classid(TFrmMsgOk), &FrmMsgOk);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
                 Application->CreateForm(__classid(TFrmValve), &FrmValve);
                 Application->CreateForm(__classid(TFrmSpinMT), &FrmSpinMT);
                 Application->CreateForm(__classid(TFrmSetting), &FrmSetting);
                 Application->CreateForm(__classid(TFrmTiltMT), &FrmTiltMT);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmMsgYesNo), &FrmMsgYesNo);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
