/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1999, ADLink Technology Inc.  All rights reserved.       ;;
;;                                                                         ;;
;;      File Name   :   Util7442.C                                         ;;
;;      Purpose     :   This sample program demonstrate how to operate     ;;
;;                                              DI & DO for PCI-7442       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "resource.h"
#include "Dask.h"

#define IDM_EXIT        110
#define TIMER1            1
#define IDC_CHAN       214
#define IDC_CHAN2      215

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hInChanComboBox, hOutChanComboBox;
U32 input[2] = {0, 0};
U32 output[2] = {0, 0};
I16 card=-1, card_number=0;
U16 iPort=0, oPort=1;  // default

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush;

POINT input_pos[16]= {{320, 100},  {300, 100},
                      {280, 100},  {260, 100},
                      {240, 100},  {220, 100},
                      {200, 100},  {180, 100},
                      {160, 100},  {140, 100},
                      {120, 100},  {100, 100},
                      { 80, 100},  { 60, 100},
                      { 40, 100},  { 20, 100}
                     };

POINT output_pos[16] ={
                       {320, 200},  {300, 200},
                       {280, 200},  {260, 200},
                       {240, 200},  {220, 200},
                       {200, 200},  {180, 200},
                       {160, 200},  {140, 200},
                       {120, 200},  {100, 200},
                       { 80, 200},  { 60, 200},
                       { 40, 200},  { 20, 200}
                      };

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance, hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
{
  MSG msg;

  if (!hPrevInstance)
     if (!InitApplication(hInstance))  return(FALSE);
  if (!InitInstance(hInstance, nCmdShow)) return(FALSE);
  while (GetMessage(&msg, NULL, NULL, NULL)) {
     TranslateMessage(&msg);
     DispatchMessage(&msg);
  }
  return(msg.wParam);
}

BOOL InitApplication(hInstance)
HANDLE hInstance;
{
  WNDCLASS wc;

  /* Main Window Class */
  wc.style = NULL;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(hInstance, "MyIcon");
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "GenericWClass";
  if (!RegisterClass(&wc)) return(FALSE);
}

BOOL InitInstance(hInstance, nCmdShow)
HANDLE hInstance;
int nCmdShow;
{
  HWND hWnd;
  HMENU hSampleMenu;

  hInst = hInstance;
  hSampleMenu = LoadMenu(hInstance, "U7442Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-7442",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      370,
                      340,
                      NULL,
                      hSampleMenu,
                      hInstance,
                      NULL);
  if (!hWnd) return(FALSE);
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  return(TRUE);
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

long  PASCAL MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
  HDC hDC;
  PAINTSTRUCT ps;
  int i, px, py;
  char n[16];

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG1, hWnd, setup);
		if(card<0) {
		    SendMessage(hWnd, WM_CLOSE, 0, 0L);
			break;
		}
        
        hInChanComboBox = CreateWindow("ComboBox", NULL,
               CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
               input_pos[10].x-10, input_pos[10].y-35, 60, 80, hWnd, IDC_CHAN, hInst, NULL);
        for (i=0;i<2;i++) {
                wsprintf(n, "TTL%d", i);
                _strupr(n);
              SendMessage(hInChanComboBox, CB_ADDSTRING, i, (LPARAM)((LPCSTR)n));
        }
        SendMessage(hInChanComboBox ,CB_SETCURSEL, 0, 0L);

		hOutChanComboBox = CreateWindow("ComboBox", NULL,
               CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
               output_pos[10].x, output_pos[10].y-35, 60, 80, hWnd, IDC_CHAN2, hInst, NULL);
        for (i=0;i<2;i++) {
                wsprintf(n, "TTL%d", i);
                _strupr(n);
              SendMessage(hOutChanComboBox, CB_ADDSTRING, i, (LPARAM)((LPCSTR)n));
        }
        SendMessage(hOutChanComboBox ,CB_SETCURSEL, 1, 0L);

		DIO_PortConfig(card, iPort+2, INPUT_PORT);
		DIO_PortConfig(card, oPort+2, OUTPUT_PORT);
             
        hLTGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
        hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));
        hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
        SetTimer(hWnd, TIMER1, 50, NULL);
        srand((unsigned)time(NULL));
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        // Digital Input  Box
        SelectObject(hDC, hLTGreenBrush);
        Rectangle(hDC, input_pos[15].x-10, input_pos[15].y-24, input_pos[0].x+26, input_pos[0].y+40);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, input_pos[15].x, input_pos[15].y-30, "Digital Input#",14);
        // Digital Output Box
        SelectObject(hDC, hGreenBrush);
        Rectangle(hDC, output_pos[15].x-10, output_pos[15].y-24, output_pos[0].x+26, output_pos[0].y+40);
        SetTextColor(hDC,RGB(255,0,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, output_pos[15].x, output_pos[15].y-30, "Digital Output#", 15);
		//
		SelectObject(hDC, hLTGreenBrush);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, input_pos[15].x, input_pos[15].y-80, "Select TTL port 0 or 1 for DI or DO",35);

        // Channel Label
        SetTextColor(hDC,RGB(0,0,0));
        SetBkColor(hDC, RGB(0, 255, 0));
        TextOut(hDC, input_pos[15].x, input_pos[15].y+16, "15                                                                        0", 75);
        SetBkColor(hDC, RGB(0, 128, 0));
        TextOut(hDC, output_pos[15].x, output_pos[15].y+16, "15                                                                        0", 75);
        SetBkColor(hDC, RGB(0, 255, 0));

        for (i=0; i<16; i++) {
			if ((input[iPort]>>i)&0x01)
				SelectObject(hDC, hRedBrush);
			else
				SelectObject(hDC, GetStockObject(WHITE_BRUSH));
			//SelectObject(hDC, GetStockObject(WHITE_BRUSH));
			Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+17, input_pos[i].y+17);
        }
        for (i=0; i<16; i++) {
            if ((output[oPort]>>i)&0x01)
				SelectObject(hDC, hRedBrush);
			else
				SelectObject(hDC, GetStockObject(WHITE_BRUSH));
			//SelectObject(hDC, GetStockObject(WHITE_BRUSH));
			Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_CHAN:
             if (HIWORD(wParam) == CBN_CLOSEUP) {
               if ((i=(int)SendMessage(GetDlgItem(hWnd,IDC_CHAN),CB_GETCURSEL, 0, 0L))>=0 ) {
				   if(i==0){
						iPort=0;
						oPort=1;
				   }
				   else{
						iPort=1;
						oPort=0;
				   }
					SendMessage(hOutChanComboBox ,CB_SETCURSEL, oPort, 0L);
					DIO_PortConfig(card, iPort+2, INPUT_PORT);
					DIO_PortConfig(card, oPort+2, OUTPUT_PORT);

					output[oPort] = 0;
					hDC = GetDC(hWnd);
					SelectObject(hDC, GetStockObject(WHITE_BRUSH));
					for(i=0; i<16; i++)
						Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
					ReleaseDC(hWnd, hDC);

					if(oPort == 0)
						DO_WritePort(card,P7442_TTL0, output[oPort]);				
					else
						DO_WritePort(card,P7442_TTL1, output[oPort]);

					ReleaseDC(hWnd, hDC);
			   }
			}
            break;
           case IDC_CHAN2:
              if (HIWORD(wParam) == CBN_CLOSEUP) {
               if ((i=(int)SendMessage(GetDlgItem(hWnd,IDC_CHAN2),CB_GETCURSEL, 0, 0L))>=0 ) {
				   if(i==0){
						oPort=0;
						iPort=1;
				   }
				   else{
						oPort=1;
						iPort=0;
				   }
					SendMessage(hInChanComboBox ,CB_SETCURSEL, iPort, 0L);

					DIO_PortConfig(card, iPort+2, INPUT_PORT);
					DIO_PortConfig(card, oPort+2, OUTPUT_PORT);

					output[oPort] = 0;
					hDC = GetDC(hWnd);
					SelectObject(hDC, GetStockObject(WHITE_BRUSH));
					for(i=0; i<16; i++)
						Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
					ReleaseDC(hWnd, hDC);

					if(oPort == 0)
						DO_WritePort(card,P7442_TTL0, output[oPort]);				
					else
						DO_WritePort(card,P7442_TTL1, output[oPort]);
			   }
			}
			break;
           default:
                  return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_TIMER:
        /* Digital Input */
            if(iPort == 0)	
				DI_ReadPort(card,P7442_TTL0,&input[iPort]);
            else
            	DI_ReadPort(card,P7442_TTL1,&input[iPort]);
		hDC = GetDC(hWnd);
        for (i=0; i<16; i++) {
          if ((input[iPort]>>i)&0x01)
             SelectObject(hDC, hRedBrush);
          else
             SelectObject(hDC, GetStockObject(WHITE_BRUSH));
          Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+17, input_pos[i].y+17);
        }
        ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
        DeleteObject(hRedBrush);
        DeleteObject(hGreenBrush);
        DeleteObject(hLTGreenBrush);
        KillTimer(hWnd, TIMER1);
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        px = LOWORD(lParam);
        py = HIWORD(lParam);
        for (i=0; i<16; i++) {
          if (px>output_pos[i].x && px<output_pos[i].x+17 && py>output_pos[i].y && py<output_pos[i].y+17) {
             hDC = GetDC(hWnd);
             if ((output[oPort]>>i)&0x01) {
                output[oPort] -= ((1L)<<i);
                SelectObject(hDC, GetStockObject(WHITE_BRUSH));
             }
             else {
                output[oPort] += ((1L)<<i);
                SelectObject(hDC, hRedBrush);
             }
             Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
             ReleaseDC(hWnd, hDC);

			 if(oPort==0) // TTL0
				DO_WritePort(card,P7442_TTL0, output[oPort]);
			 else
				DO_WritePort(card,P7442_TTL1, output[oPort]);
	
             break;
          }
        }
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}
