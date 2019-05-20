//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormErr.cpp", FrmErr);
USEFORM("FormPassword.cpp", FrmPassword);
USEFORM("FormMsgOk.cpp", FrmMsgOk);
USEFORM("FormMsgYesNo.cpp", FrmMsgYesNo);
USEFORM("FormMode.cpp", FrmMode);
USEFORM("FormErrList.cpp", FrmErrList);
USEFORM("FormDisplay.cpp", FrmDisplay);
USEFORM("FormUpdate.cpp", FrmUpdate);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE Mutex;
    int ModalResult;
    try
    {
    	//프로그램 중복실행 방지
        const char ProgMutex[] = "TCosmos3_Motion";
        Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, ProgMutex);
        if(! Mutex)
            CreateMutex(NULL, true, ProgMutex);
        else
            return 0;
                 Application->Initialize();
                 Application->Title = "Hanra_TCosmos3_Motion";
                 Application->CreateForm(__classid(TFrmMain), &FrmMain);
         Application->CreateForm(__classid(TFrmErr), &FrmErr);
         Application->CreateForm(__classid(TFrmMsgYesNo), &FrmMsgYesNo);
         Application->CreateForm(__classid(TFrmMsgOk), &FrmMsgOk);
         Application->CreateForm(__classid(TFrmPassword), &FrmPassword);
         Application->CreateForm(__classid(TFrmUpdate), &FrmUpdate);
         Application->CreateForm(__classid(TFrmMode), &FrmMode);
         Application->CreateForm(__classid(TFrmErrList), &FrmErrList);
         Application->CreateForm(__classid(TFrmDisplay), &FrmDisplay);
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

