/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7432INT.C                                              ;;
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
#define WM_INT         2000 //interrupt comming message

HANDLE hInst;          /* current instance */
int int_count=0;
I16 card=-1, card_number = 0;
HWND hStart, hMainWnd;

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long  PASCAL MainWndProc(HWND, unsigned, WPARAM, LPARAM);
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
    wc.lpszMenuName =  "U7432Menu";
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
    hWnd = CreateWindow("IntClass", "PCI-7432 Interrupt Sample (INT1)", WS_OVERLAPPEDWINDOW,
                        200, 100, 360, 300, NULL, NULL, hInstance, NULL);
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
                     if ((card=Register_Card(PCI_7432, card_number)) < 0) {
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

			 int_count = 0;		     
			 DIO_INT1_EventMessage (card, INT1_EXT_SIGNAL, hWnd, WM_INT, NULL);
             hMainWnd = hWnd;
             break;
        case WM_PAINT:
             hDC = BeginPaint(hWnd, &ps);
             TextOut(hDC, 20, 30, "This program shows the number of occurrence", 43);
             TextOut(hDC, 20, 45, "of INT1 interupt. INT1 interrupt is generated", 45);
             TextOut(hDC, 20, 60, "by external signal from DI bit-0. If you connect", 48);
             TextOut(hDC, 20, 75, "DO bit-0 to DI bit-0, press 'Output' button can", 47);
             TextOut(hDC, 20, 90, "generate an interrupt.", 22);
             wsprintf(s,"INT1 Count = %d", int_count);
             TextOut(hDC, 120, 140, s, strlen(s));
             EndPaint(hWnd, &ps);
             break;
        case WM_COMMAND:
             switch(LOWORD(wParam)) {
                case IDM_EXIT:
					 DIO_INT1_EventMessage (card, INT1_DISABLE, hMainWnd, NULL, NULL);
                     SendMessage(hWnd, WM_CLOSE, 0, 0L);
                     break;
                case IDC_OUTPUT:
					 DO_WritePort(card,0, 0x1);
					 Sleep(1);
					 DO_WritePort(card,0, 0x0);			 					 
                     break;
             }
             break;
	    case WM_INT: //interrupt comming message 
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
	HDC hDC;
    char  Buf[80];

    hDC=GetDC(hMainWnd);
	int_count++;

    wsprintf(Buf,"INT1 Count = %d ", int_count);
    TextOut(hDC,120,140,Buf,strlen(Buf));
    
    ReleaseDC(hMainWnd, hDC);
}
