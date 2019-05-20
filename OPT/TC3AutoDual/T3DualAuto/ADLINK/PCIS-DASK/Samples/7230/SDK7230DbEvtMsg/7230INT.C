/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7230INT.C                                              ;;
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
int int_count[2]= {0, 0};
I16 card=-1, card_number = 0;
HWND hStart, hMainWnd;

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
    wc.lpszMenuName =  "U7230Menu";
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
    hWnd = CreateWindow("IntClass", "PCI-7230 Dual Interrupt Sample", WS_OVERLAPPEDWINDOW,
                        200, 100, 415, 300, NULL, NULL, hInstance, NULL);
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
                     if ((card=Register_Card(PCI_7230, card_number)) < 0) {
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

    switch(message) {
        case WM_CREATE:
             DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
		     if(card<0) {
		         SendMessage(hWnd, WM_CLOSE, 0, 0L);
			     break;
		     }
             CreateWindow("BUTTON", "Output", WS_CHILD | WS_VISIBLE,
                      130, 200, 72, 24, hWnd, IDC_OUTPUT, hInst, NULL);
			 int_count[0]=0;
		     int_count[1]=0;
			 DIO_INT1_EventMessage (card, INT1_EXT_SIGNAL, hWnd, WM_INT, NULL);
			 DIO_INT2_EventMessage (card, INT2_EXT_SIGNAL, hWnd, NULL, (void *) cbfn);
             hMainWnd = hWnd;
             break;
        case WM_PAINT:
             hDC = BeginPaint(hWnd, &ps);
             TextOut(hDC, 20, 20, "This program shows the number of occurrence of interrupt",56);
			 TextOut(hDC, 20, 35, "for Dual interrupt lines.", 25);
             TextOut(hDC, 20, 50, "INT1 interrupt is generated by external signal from DI bit-0.", 61);
             TextOut(hDC, 20, 65, "INT2 interrupt is generated by external signal from DI bit-1.", 61);
             TextOut(hDC, 20, 80, "If you connect DO bit-0 to DI bit-0 and DO bit-1 to DI bit-1,", 61);
             TextOut(hDC, 20, 95, "press 'Output' button can generate two interrupts.", 50);
             TextOut(hDC, 20, 110,"One for INT1 and the other for INT2.", 36);
			 wsprintf(buf,"INT1 count = %d", int_count[0]);
             TextOut(hDC, 120, 140, buf, strlen(buf));
	         wsprintf(buf,"INT2 count = %d", int_count[1]);
             TextOut(hDC, 120, 160, buf, strlen(buf));
             EndPaint(hWnd, &ps);
             break;
        case WM_COMMAND:
             switch(LOWORD(wParam)) {
                case IDM_EXIT:
					 DIO_INT1_EventMessage (card, INT1_DISABLE, hMainWnd, NULL, NULL);
					 DIO_INT2_EventMessage (card, INT2_DISABLE, hMainWnd, NULL, NULL);
                     SendMessage(hWnd, WM_CLOSE, 0, 0L);
                     break;
                case IDC_OUTPUT:
					 DO_WritePort(card,0, 0x3);
					 DO_WritePort(card,0, 0x0);
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
    InvalidateRect(hMainWnd, NULL, FALSE);
}