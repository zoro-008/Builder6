//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "FormLoading.h"
//---------------------------------------------------------------------------
USEFORM("FormLoading.cpp", FrmLoading);
USEFORM("FormMain.cpp", FrmMain);
USEFORM("FormOperator.cpp", FrmOperator);
USEFORM("FormOperation.cpp", FrmOperation);
USEFORM("FormDevice.cpp", FrmDevice);
USEFORM("FormDeviceSet.cpp", FrmDeviceSet);
USEFORM("FormOption.cpp", FrmOption);
USEFORM("FormMaster.cpp", FrmMaster);
USEFORM("FormSPC.cpp", FrmSPC);
USEFORM("FormInform.cpp", FrmInform);
USEFORM("FrameCyl.cpp", FraCyl); /* TFrame: File Type */
USEFORM("FrameMotr.cpp", FraMotr); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE hMutex;
//    int ModalResult;
    try
    {
       	//���α׷� �ߺ����� ����
        //���� ���μ��� �Ǵ�  �����忡 ���� ���� ������ ���Ǵ� OpenMutex() API �� ���� Ư����... 
        //�Ʒ� �ڵ������� ������ �߻��ϰ� �˴ϴ�. 
/*
        const char ProgMutex[] = "EpqBase";
        Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, ProgMutex);
        if(! Mutex)
            CreateMutex(NULL, true, ProgMutex);
        else
            return 0;
*/
        const char ProgMutex[] = "EpqBase";
        hMutex = CreateMutex(NULL, TRUE, ProgMutex);
        if ( hMutex == NULL )
            throw Exception("Error: CreateMutex failed.!");

        if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
            OutputDebugStringA("Named Mutex already exist. skip Application Init..");
            CloseHandle( hMutex );
            return 0;
        }

         FrmLoading = new TFrmLoading(Application);
         FrmLoading->ShowModal();
         delete FrmLoading;

         Application->Initialize();
         Application->CreateForm(__classid(TFrmMain), &FrmMain);
                 Application->CreateForm(__classid(TFrmOperator), &FrmOperator);
                 Application->CreateForm(__classid(TFrmOption), &FrmOption);
                 Application->CreateForm(__classid(TFrmSPC), &FrmSPC);
                 Application->CreateForm(__classid(TFrmDevice), &FrmDevice);
                 Application->CreateForm(__classid(TFrmDeviceSet), &FrmDeviceSet);
                 Application->CreateForm(__classid(TFrmInform), &FrmInform);
                 Application->CreateForm(__classid(TFrmLoading), &FrmLoading);
                 Application->CreateForm(__classid(TFrmMaster), &FrmMaster);
                 Application->CreateForm(__classid(TFrmOperation), &FrmOperation);
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

