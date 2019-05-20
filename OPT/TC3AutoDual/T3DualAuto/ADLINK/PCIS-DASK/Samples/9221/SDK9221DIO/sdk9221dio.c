/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1999, ADLink Technology Inc.  All rights reserved.       ;;
;;                                                                         ;;
;;      File Name   :   sdk9221dio.C                                       ;;
;;      Purpose     :   This sample program demonstrate how to operate     ;;
;;                                              DI & DO for PCI-9221       ;;
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

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
long PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE hInst;
HWND hInChanComboBox, hOutChanComboBox;
U32 input = 0;
U32 output = 0;
I16 card=-1, card_number=0;

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush;

POINT input_pos[8]= { {180, 50},  {160, 50},
                      {140, 50},  {120, 50},
                      {100, 50},  { 80, 50},
                      { 60, 50},  { 40, 50}
                     };

POINT output_pos[4] ={ {140, 150},  {120, 150},
                       {100, 150},  {80, 150}
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
	if (!RegisterClass(&wc)) 
		return(FALSE);
}


BOOL InitInstance(hInstance, nCmdShow)
HANDLE hInstance;
int nCmdShow;
{
	HWND hWnd;
	HMENU hSampleMenu;

	hInst = hInstance;
	hSampleMenu = LoadMenu(hInstance, "U9221Menu");
	hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-9221 DIO",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      250,
                      250,
                      NULL,
                      hSampleMenu,
                      hInstance,
                      NULL);
	if (!hWnd) 
		return(FALSE);
  
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
                     if ((card=Register_Card(PCI_9221, card_number)) < 0) {
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
			Rectangle(hDC, input_pos[7].x-10, input_pos[7].y-24, input_pos[0].x+26, input_pos[0].y+40);
			SetTextColor(hDC,RGB(255,255,255));
			SetBkColor(hDC, RGB(0, 0, 255));
			TextOut(hDC, input_pos[5].x-6, input_pos[7].y-30, "Digital Input",13);
			// Digital Output Box
			SelectObject(hDC, hGreenBrush);
			Rectangle(hDC, output_pos[3].x-10, output_pos[3].y-24, output_pos[0].x+26, output_pos[0].y+40);
			SetTextColor(hDC,RGB(255,0,0));
			SetBkColor(hDC, RGB(0, 0, 255));
			TextOut(hDC, output_pos[3].x-6, output_pos[3].y-30, "Digital Output", 14);
			// Channel Label
			SetTextColor(hDC,RGB(0,0,0));
			SetBkColor(hDC, RGB(0, 255, 0));
			TextOut(hDC, input_pos[7].x, input_pos[7].y+16, "7                                  0", 36);
			SetBkColor(hDC, RGB(0, 128, 0));
			TextOut(hDC, output_pos[3].x, output_pos[3].y+16, "3              0", 16);
			SetBkColor(hDC, RGB(0, 255, 0));
        
			for (i=0; i<8; i++) {
				SelectObject(hDC, GetStockObject(WHITE_BRUSH));
				Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+17, input_pos[i].y+17);
			}
        
			for (i=0; i<4; i++) {
				SelectObject(hDC, GetStockObject(WHITE_BRUSH));
				Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
			}
			EndPaint(hWnd, &ps);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDM_EXIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0L);
					break;
				default:
                  return(DefWindowProc(hWnd, message, wParam, lParam));
			}
			break;
		case WM_TIMER:
			DI_ReadPort(card,0,&input);       
			hDC = GetDC(hWnd);
			for (i=0; i<8; i++) {
			if ((input>>i)&0x01)
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
			for (i=0; i<4; i++) {
				if (px>output_pos[i].x && px<output_pos[i].x+17 && py>output_pos[i].y && py<output_pos[i].y+17) {
					hDC = GetDC(hWnd);
					if ((output>>i)&0x01) {
						output -= ((1L)<<i);
						SelectObject(hDC, GetStockObject(WHITE_BRUSH));
					}
					else {
						output += ((1L)<<i);
						SelectObject(hDC, hRedBrush);
					}
					Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
					ReleaseDC(hWnd, hDC);

					DO_WritePort(card, 0, output);
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