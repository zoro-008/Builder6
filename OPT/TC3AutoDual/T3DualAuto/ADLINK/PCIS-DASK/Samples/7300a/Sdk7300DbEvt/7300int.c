/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-2001, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   7300INT.C                                          ;;
;;      Date        :   04/07/2001                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

 #include <windows.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <dos.h>
 #include "Dask.h"
 #include "resource.h"

 #define IDM_SETUP                       101
 #define IDD_DIALOG1                     101
 #define IDM_EXIT                        110
 #define IDM_GO                          300
 #define IDM_CLEAR                       305
 #define IDC_COMBO1                      1004
 #define IDC_EDIT1                       1007
 #define IDC_EDIT2                       1008


 HANDLE       hInst;          /* current instance */
 HANDLE		  hIntEvent[2];
 HWND         hMainWnd;
 long hThread = -1;

 I16 card=-1, card_number = 0;
 U16 int_count = 0;
 int total_count =40;
 int sample_rate=20000;
 char Buf[80];
 int  c2=10;
 
 int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
 BOOL InitApplication(HANDLE);
 BOOL InitInstance(HANDLE, int);
 long  PASCAL MainWndProc(HWND, unsigned, WPARAM, LPARAM);
 BOOL  PASCAL About(HWND, unsigned, WPARAM, LPARAM);
 BOOL  PASCAL ShowDMA(HWND, unsigned, WPARAM, LPARAM);

 void
IntThread(void* pArg)
{
  while (1) {
       if (WaitForSingleObject(hIntEvent[0], INFINITE) == WAIT_OBJECT_0) {
         int_count++;
         InvalidateRect(hMainWnd, NULL, FALSE);
	   } 
  }
  _endthread();
}

 /**********************************************************************

 FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
 PURPOSE: calls initialization function, processes message loop

 ***********************************************************************/

 int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)

 HANDLE hInstance;
 HANDLE hPrevInstance;
 LPSTR  lpCmdLine;
 int    nCmdShow;

 {
        MSG msg;

        if(!hPrevInstance)
          if(!InitApplication(hInstance))
                return(FALSE);

        if(!InitInstance(hInstance, nCmdShow))
                return(FALSE);

        while ( GetMessage(&msg, NULL, NULL, NULL))
           {
             TranslateMessage(&msg);
             DispatchMessage(&msg);
           }
        return(msg.wParam);
 }

/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    PURPOSE: Initializes window data and registers window class

****************************************************************************/

BOOL InitApplication(hInstance)
HANDLE hInstance;
{
    WNDCLASS  wc;

    wc.style = NULL;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "IntMenu";
    wc.lpszClassName = "IntClass";

    return (RegisterClass(&wc));
}

/****************************************************************************

    FUNCTION:  InitInstance(HANDLE, int)

    PURPOSE:  Saves instance handle and creates main window

****************************************************************************/

BOOL InitInstance(hInstance, nCmdShow)
HANDLE          hInstance;
int             nCmdShow;
{
    HWND            hWnd;

    hInst = hInstance;
    hWnd = CreateWindow("IntClass",
                        "PCI-7300 Local Interrupt Sample",
                        WS_OVERLAPPEDWINDOW,
                        200,
                        100,
                        400,
                        370,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    if (!hWnd)    return (FALSE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return (TRUE);
}

BOOL PASCAL setup(HWND hWnd, unsigned message, WPARAM wParam, LPARAM lParam)
{
  int i,k=0;
  char n[16];
  char s[50];
  BOOL ok;

      switch (message) {
         case WM_INITDIALOG:
              for (i=0;i<16;i++) {
                wsprintf(n, "%d", i);
                _strupr(n);
                SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_ADDSTRING, NULL, (LONG)(LPSTR) n);
              }
              SendMessage(GetDlgItem(hWnd,IDC_COMBO1) , CB_SETCURSEL, 0, 0L);
			  if(card >=0)
			      EnableWindow(GetDlgItem(hWnd,IDC_COMBO1), FALSE);
              SendMessage(GetDlgItem(hWnd,IDC_EDIT2) , WM_SETTEXT, 0, "10000");
              SendMessage(GetDlgItem(hWnd,IDC_EDIT1) , WM_SETTEXT, 0, "40");

              break;
         case WM_COMMAND:
              switch(LOWORD(wParam)) {
                case IDC_COMBO1:
                     if (HIWORD(wParam) == CBN_CLOSEUP) {
                        i = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_GETCURSEL, 0, 0L);
                        SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_GETLBTEXT, i, (LPARAM)((LPCSTR)n));
                        card_number = i;
                     }
                     return(TRUE);
                case IDOK:
					 if(card < 0) {
					   if ((card=Register_Card(PCI_7300A_RevA, card_number))< 0) {
                         wsprintf(s, "Register_Card Error = %d\n", card);
                         MessageBox(hWnd, s, "Error", MB_OK);
						 EndDialog(hWnd, TRUE);
                         break;
					   }
 				       DIO_7300SetInterrupt(card, 1, 1, hIntEvent);
                     }
					 total_count = GetDlgItemInt(hWnd,IDC_EDIT1, &ok, FALSE);
					 if(total_count > 32768) {
				       MessageBox(hWnd,"Count can be assigned at most 32768", "INT", MB_OK); 
				       break;
				     } 

					 sample_rate = GetDlgItemInt(hWnd,IDC_EDIT2, &ok, FALSE);
					 if (sample_rate > 15000 ) sample_rate=15000;
                     c2=10000000/(sample_rate);
		             hMainWnd = hWnd;
                     EndDialog(hWnd, TRUE);
                     return (TRUE);
                case IDCANCEL:
                     EndDialog(hWnd, TRUE);
                     return (TRUE);
              }
              break;
      }
      return (FALSE);
}

/****************************************************************************

    FUNCTION: MainWndProc(HWND, UINT, WPARAM, LPARAM)

    PURPOSE:  Processes messages


****************************************************************************/

long  PASCAL MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;
unsigned message;
WPARAM wParam;
LPARAM lParam;
{    
    int      i, k;
	int      cur_count=0;
    HDC      hDC, hDC1;
    PAINTSTRUCT ps;   
	FARPROC  lpProc;
	char s[50];

    switch(message) {
        case WM_CREATE:
        break;
        case WM_PAINT:
             hDC1=BeginPaint(hWnd, &ps);
             wsprintf(Buf,"This program demostrates how to use local interrupt line.");
             TextOut(hDC1,20,10,Buf,strlen(Buf));
             wsprintf(Buf,"The IRQ source of INT1 is AUXDI0.");
             TextOut(hDC1,20,30,Buf,strlen(Buf));
             wsprintf(Buf," The IRQ source of INT2 is internal Timer2 Output");
             TextOut(hDC1,20,50,Buf,strlen(Buf));
             SetTextColor(hDC1, RGB(255,0,0));
             wsprintf(Buf,"Push 'Setup' button first ...");
             TextOut(hDC1,20,70,Buf,strlen(Buf));
             EndPaint(hWnd, &ps);
             break;
        case WM_COMMAND:
             switch(wParam) {
				case IDM_SETUP:
                 lpProc =  MakeProcInstance(setup, hInst);
                 DialogBox(hInst,IDD_DIALOG1,hWnd,lpProc);
                 FreeProcInstance(lpProc);
			     if(card<0)
		             SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
  
                case IDM_GO:
                 //  Interrupt Trigger
                 hDC=GetDC(hWnd);
			     if(hThread<0)
  			       hThread = _beginthread(IntThread, 0, 0);
				 CTR_Setup (card, 2, 2, c2, BIN);
				 k=0; i=0;int_count=0;
				 while (k<total_count) {
				   if (WaitForSingleObject(hIntEvent[1], 10000)== WAIT_OBJECT_0) {
	                 /*wsprintf(Buf,"INT2 Count = %d ",k);
				     TextOut(hDC,20,130,Buf,strlen(Buf));
					 wsprintf(Buf,"INT1 Count = %d ",int_count);
				     TextOut(hDC,20,110,Buf,strlen(Buf));*/
					 k++;
				   }
                   else {
					wsprintf(s, "****** It is too long to wait INT2 Event *****\n");
                    MessageBox(hWnd, s, "INT2 Err", MB_OK);
					break;
	               }
                 }
				 CTR_Setup (card, 2, RATE_GENERATOR, 0, BIN);
			     wsprintf(Buf,"INT2 Count = %d ",k);
				 TextOut(hDC,20,130,Buf,strlen(Buf));
				 wsprintf(Buf,"INT1 Count = %d ",int_count);
				 TextOut(hDC,20,110,Buf,strlen(Buf));

				ReleaseDC(hWnd, hDC);
                break;

				case IDM_CLEAR :
				 if(card >=0) {
	               DIO_7300SetInterrupt(card, 0, 0, hIntEvent);
				   for (i=0; i<2; i++) {
				    if(hIntEvent[i])
				      ResetEvent(hIntEvent[i]);
				   }
				 }
                 PostQuitMessage(0);
             	break;
             }
             break;

        case WM_DESTROY:
			 if (card >= 0) Release_Card(card);
             PostQuitMessage(0);
             break;

        default:
             return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return(0L);
}

