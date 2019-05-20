/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   7348int.C                                          ;;
;;      Purpose     :   This sample program demonstrates DIO through       ;;
;;                      interrupt operation                                ;;
;;      Date        :   07/20/1998                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

 #include <windows.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <dos.h>
 #include "Dask.h"

 #define IDM_SETUP                       101
 #define IDD_DIALOG1                     101
 #define IDM_EXIT                        110
 #define IDM_GO                          300
 #define IDM_CLEAR                       305
 #define IDC_COMBO1                      1004
 #define IDC_COMBO2                      1005
 #define IDC_EDIT1                       1007
 #define IDC_EDIT2                       1008


 HANDLE       hInst;          /* current instance */
 HANDLE       hIntEvent[2];

 U16 wBus, wSlot;
 I16 card=-1, card_number = 0;

 int   total_count =40;
 int   sample_rate=5000;
 unsigned int buffer[32768], in_buffer[32768];
 char  Buf[80];
 int   c1=20,c2=20;

 int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
 BOOL InitApplication(HANDLE);
 BOOL InitInstance(HANDLE, int);
 long  PASCAL MainWndProc(HWND, unsigned, WPARAM, LPARAM);
 BOOL  PASCAL About(HWND, unsigned, WPARAM, LPARAM);
 BOOL  PASCAL ShowDMA(HWND, unsigned, WPARAM, LPARAM);

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
                        "PCI-7348 Interrupt Samples (INT2)",
                        WS_OVERLAPPEDWINDOW,
                        200,
                        100,
                        375,
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
              SendMessage(GetDlgItem(hWnd,IDC_EDIT2) , WM_SETTEXT, 0, "5000");
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
                     if(card <0) {
                        if ((card=Register_Card(PCI_7396, card_number))< 0) {
                           wsprintf(s, "Register_Card Error = %d\n", card);
                           MessageBox(hWnd, s, "Error", MB_OK);
                        }
                     }
                     total_count = GetDlgItemInt(hWnd,IDC_EDIT1, &ok, FALSE);
                     if (total_count > 32768) {
                       MessageBox(hWnd,"Count can be assigned at most 32768", "INT", MB_OK);
                       break;
                     }
                     for (i=0;i<total_count;i++) {
                             buffer[i]=k;
                             k ++;
                             if (k>255) k=0;
                     }
                     for (i=0;i<total_count;i++) in_buffer[i]=0;

                     sample_rate = GetDlgItemInt(hWnd,IDC_EDIT2, &ok, FALSE);
                     if (sample_rate > 15000 ) sample_rate=15000;
                     c2=2000000/(sample_rate*c1);

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
    int  i, k;
    int  cur_count=0;
    HDC  hDC, hDC1;
    PAINTSTRUCT ps;
    FARPROC  lpProc;

    switch(message) {

        case WM_CREATE:
             break;
        case WM_PAINT:
             hDC1=BeginPaint(hWnd, &ps);
             wsprintf(Buf,"This program performs digital output on Port A of");
             TextOut(hDC1,20,10,Buf,strlen(Buf));
             wsprintf(Buf,"P2 and digital input on Port A of P1 when an");
             TextOut(hDC1,20,30,Buf,strlen(Buf));
             wsprintf(Buf,"interrupt happens. The INT2 is used, and interrupt");
             TextOut(hDC1,20,50,Buf,strlen(Buf));
             wsprintf(Buf,"signals are generated by internal Timer Output.");
             TextOut(hDC1,20,70,Buf,strlen(Buf));
             SetTextColor(hDC1, RGB(255,0,0));
             wsprintf(Buf,"Push 'Setup' button first ...");
             TextOut(hDC1,20,90,Buf,strlen(Buf));
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
                  DIO_PortConfig(card,Channel_P1A, INPUT_PORT);
                  DIO_PortConfig(card,Channel_P2A, OUTPUT_PORT);

                  DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_TIMER_COUNTER, hIntEvent);
                  CTR_Setup (card, 1, RATE_GENERATOR, c1, BIN);
                  CTR_Setup (card, 2, RATE_GENERATOR, c2, BIN);
                  k=0; i=0;
                  while (k<total_count) {
                    if (WaitForSingleObject(hIntEvent[1], 4000)== WAIT_OBJECT_0) {
                          DO_WritePort(card,Channel_P2A, buffer[k]);
                          DI_ReadPort(card,Channel_P1A,&in_buffer[k]);
                          wsprintf(Buf,"Count = %d ",k+1);
                          TextOut(hDC,20,120,Buf,strlen(Buf));
                          k++;
                    }
                  }
                  DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, hIntEvent);
                  CTR_Setup (card, 1, RATE_GENERATOR, 0, BIN);
                  CTR_Setup (card, 2, RATE_GENERATOR, 0, BIN);

                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                  SelectObject(hDC, GetStockObject(WHITE_PEN));
                  Rectangle(hDC, 10, 180, 280, 320);
                  SelectObject(hDC, GetStockObject(BLACK_PEN));

                  wsprintf(Buf,"Data from Input port: ");
                  TextOut(hDC,20,140,Buf,strlen(Buf));
                  wsprintf(Buf,"** only shows at most 50 data **");
                  TextOut(hDC,20,160,Buf,strlen(Buf));

                  for (i=0;i<total_count;i++) {
                   if(i<10) {
                       wsprintf(Buf,"  %d, ",in_buffer[i]);
                       TextOut(hDC,20+25*i,185,Buf,strlen(Buf));
                   }
                   if(i>=10 && i<20) {
                       wsprintf(Buf,"%d, ",in_buffer[i]);
                       TextOut(hDC,20+25*(i-10),210,Buf,strlen(Buf));
                   }
                   if(i>=20 && i<30) {
                       wsprintf(Buf,"%d, ",in_buffer[i]);
                       TextOut(hDC,20+25*(i-20),235,Buf,strlen(Buf));
                   }
                   if (i>=30 && i<40) {
                       wsprintf(Buf,"%d, ",in_buffer[i]);
                       TextOut(hDC,20+25*(i-30),260,Buf,strlen(Buf));
                   }
                   if (i>=40 && i<50) {
                       wsprintf(Buf,"%d, ",in_buffer[i]);
                       TextOut(hDC,20+25*(i-40),285,Buf,strlen(Buf));
                   }
                  }
                  ReleaseDC(hWnd, hDC);
                  break;

                case IDM_CLEAR :
                  ResetEvent(hIntEvent[1]);
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
