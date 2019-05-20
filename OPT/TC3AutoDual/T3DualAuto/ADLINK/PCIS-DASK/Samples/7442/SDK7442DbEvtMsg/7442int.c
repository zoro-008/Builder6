/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7442INT.C                                              ;;
;;      Purpose   : This sample program demonstrates how handle interrupt  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <process.h>
#include "Dask.h"
#include "Resource.h"

#define IDC_START       300
#define IDC_OUTPUT      302
#define WM_INT                          2000

HANDLE hInst;          /* current instance */
int int_count[2];
int press_count=0;
I16 card=-1, card_number = 0;
HWND hStart, hMainWnd, hEvent[2];

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long  PASCAL MainWndProc(HWND, unsigned, WPARAM, LPARAM);
LRESULT CALLBACK cbfn();
void mesgfn();

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
    wc.lpszMenuName =  "U7442Menu";
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
    hWnd = CreateWindow("IntClass", "PCI-7442 COS Interrupt Sample", WS_OVERLAPPEDWINDOW,
                        200, 100, 420, 300, NULL, NULL, hInstance, NULL);
    if (!hWnd)    return (FALSE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return (TRUE);
}

BOOL PASCAL setup(HWND hWnd, unsigned message, WPARAM wParam, LPARAM lParam)
{
  int i;
  char n[16];
  char s[50];
      switch (message) {
         case WM_INITDIALOG:
              for (i=0;i<16;i++) {
                wsprintf(n, "%d", i);
                _strupr(n);
                SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_ADDSTRING, NULL, (LONG)(LPSTR) n);
              }
              SendMessage(GetDlgItem(hWnd,IDC_COMBO1) , CB_SETCURSEL, 0, 0L);

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
                     if ((card=Register_Card(PCI_7442, card_number)) < 0) {
                        wsprintf(s, "Register_Card Error = %d\n", card);
                        MessageBox(hWnd, s, "Error", MB_OK);
                     }
                     EndDialog(hWnd, TRUE);
                     InvalidateRect(hWnd, NULL, FALSE);
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
    PAINTSTRUCT ps;
    HDC hDC;
    char buf[20];
    char s[50];

    switch(message) {
        case WM_CREATE:
            DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
		    if(card<0) {
		         SendMessage(hWnd, WM_CLOSE, 0, 0L);
			     break;
		    }
            CreateWindow("BUTTON", "Output", WS_CHILD | WS_VISIBLE,
                      130, 200, 72, 24, hWnd, IDC_OUTPUT, hInst, NULL);
			 DO_WritePort(card, P7442_CH0, 0x0);
			 DO_WritePort(card, P7442_CH1, 0x0);
			 Sleep(1);
			 DIO_SetCOSInterrupt32(card, 0, 0x0001, (HANDLE *) &hEvent[0], 0);
			 DIO_SetCOSInterrupt32(card, 1, 0x0001, (HANDLE *) &hEvent[1], 1);
		     DIO_INT_Event_Message (card, 1, hEvent[0], hWnd, WM_INT, NULL);
			 DIO_INT_Event_Message (card, 1, hEvent[1], hWnd, NULL, (U32) cbfn);
             hMainWnd = hWnd;
             break;
        case WM_PAINT:
             hDC = BeginPaint(hWnd, &ps);
             TextOut(hDC, 20, 20, "This program shows the number of occurrence of interrupt",56);
			 TextOut(hDC, 20, 35, "for COS interrupt.", 18);
             TextOut(hDC, 20, 50, "COS0 interrupt is generated by change of state of DI0 bit-0.", 59);
             TextOut(hDC, 20, 65, "COS1 interrupt is generated by change of state of DI1 bit-1.", 59);
             TextOut(hDC, 20, 80, "If connecting DO0 bit-0 to DI0 bit-0 and DO1 bit-0 to DI1 bit-0,", 63);
             TextOut(hDC, 20, 95, "press 'Output' button can generate two interrupts.", 50);
             TextOut(hDC, 20, 110,"One for COS0 and the other for COS1.", 36);
             wsprintf(buf,"COS0 count = %d", int_count[0]);
             TextOut(hDC, 120, 140, buf, strlen(buf));
			 wsprintf(buf,"COS1 count = %d", int_count[1]);
             TextOut(hDC, 120, 160, buf, strlen(buf));
             EndPaint(hWnd, &ps);
             break;
        case WM_COMMAND:
             switch(LOWORD(wParam)) {
                case IDM_EXIT:
			    	 DIO_SetCOSInterrupt32(card, 0, 0, NULL, 0);
					 DIO_SetCOSInterrupt32(card, 1, 0, NULL, 0);
					 DIO_INT_Event_Message (card, 0, hEvent[0], NULL, NULL, NULL);
					 DIO_INT_Event_Message (card, 0, hEvent[1], NULL, NULL, NULL);
                     SendMessage(hWnd, WM_CLOSE, 0, 0L);
                     break;
                case IDC_OUTPUT:
					 Sleep(1);
					 if(press_count%2 == 0){
						DO_WritePort(card, P7442_CH0, 0x1);
						DO_WritePort(card, P7442_CH1, 0x1);
					 }
					 else{
						DO_WritePort(card, P7442_CH0, 0x0);
						DO_WritePort(card, P7442_CH1, 0x0);
					 }
					 press_count++;
                     break;
             }
             break;
	    case WM_INT:
			 mesgfn();
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

void mesgfn()
{
    int_count[0]++;
    InvalidateRect(hMainWnd, NULL, FALSE);
}

LRESULT CALLBACK cbfn()
{
	int_count[1]++;
	ResetEvent(hEvent[1]);
    InvalidateRect(hMainWnd, NULL, FALSE);
}