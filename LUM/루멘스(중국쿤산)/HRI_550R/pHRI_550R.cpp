//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("FrmMain.cpp", FormMain);
USEFORM("FrmOperation.cpp", FormOperation);
USEFORM("FrmManual.cpp", FormManual);
USEFORM("FrmDevice.cpp", FormDevice);
USEFORM("FrmDeviceSet.cpp", FormDeviceSet);
USEFORM("FrmMotorPos.cpp", FormMotorPos);
USEFORM("FrmInOutState.cpp", FormInOutState);
USEFORM("FrmLotOpen.cpp", FormLotOpen);
USEFORM("FrmSPC.cpp", FormSPC);
USEFORM("FrmOption.cpp", FormOption);
USEFORM("FrmLimitCnt.cpp", FormLimitCnt);
USEFORM("FrmVisionRlstSet.cpp", FormVisionRlstSet);
USEFORM("FrmReviewSelect.cpp", FormReviewSelect);
USEFORM("FrmMaster.cpp", FormMaster);
USEFORM("FrmRsltBig.cpp", FormRsltBig);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE Mutex;

    try
    {
       	//프로그램 중복실행 방지
        const char ProgMutex[] = "EpqBase";
        Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, ProgMutex);
        if(! Mutex)
            CreateMutex(NULL, true, ProgMutex);
        else
            return 0;


         Application->Initialize();
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormOperation), &FormOperation);
         Application->CreateForm(__classid(TFormManual), &FormManual);
         Application->CreateForm(__classid(TFormDevice), &FormDevice);
         Application->CreateForm(__classid(TFormDeviceSet), &FormDeviceSet);
         Application->CreateForm(__classid(TFormMotorPos), &FormMotorPos);
         Application->CreateForm(__classid(TFormInOutState), &FormInOutState);
         Application->CreateForm(__classid(TFormLotOpen), &FormLotOpen);
         Application->CreateForm(__classid(TFormSPC), &FormSPC);
         Application->CreateForm(__classid(TFormOption), &FormOption);
         Application->CreateForm(__classid(TFormLimitCnt), &FormLimitCnt);
         Application->CreateForm(__classid(TFormVisionRlstSet), &FormVisionRlstSet);
         Application->CreateForm(__classid(TFormMaster), &FormMaster);
         Application->CreateForm(__classid(TFormReviewSelect), &FormReviewSelect);
         Application->CreateForm(__classid(TFormRsltBig), &FormRsltBig);
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

