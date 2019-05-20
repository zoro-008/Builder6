/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1999, ADLink Technology Inc.  All rights reserved.       ;;
;;                                                                         ;;
;;      File Name   :   Util7233.C                                         ;;
;;      Purpose     :   This sample program demonstrate how to operate     ;;
;;                                              DI for PCI-7233            ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Dask.h"
#include "resource.h"

#define IDM_EXIT        110
#define TIMER1            1

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
U32 input_value=0;
U32 oldinput_value=0;
I16 card=-1, card_number=0;

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush;

POINT input_pos[64]= {
                      {320, 80},  {300, 80},
                      {280, 80},  {260, 80},
                      {240, 80},  {220, 80},
                      {200, 80},  {180, 80},
                      {160, 80},  {140, 80},
                      {120, 80},  {100, 80},
                      { 80, 80},  { 60, 80},
                      { 40, 80},  { 20, 80},
                      {320, 40},  {300, 40},
                      {280, 40},  {260, 40},
                      {240, 40},  {220, 40},
                      {200, 40},  {180, 40},
                      {160, 40},  {140, 40},
                      {120, 40},  {100, 40},
                      { 80, 40},  { 60, 40},
                      { 40, 40},  { 20, 40}
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
  hSampleMenu = LoadMenu(hInstance, "U7233Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-7233",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      370,
                      180,
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
                     if ((card=Register_Card(PCI_7233, card_number)) < 0) {
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
  int i;

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
        Rectangle(hDC, input_pos[31].x-10, input_pos[31].y-24, input_pos[0].x+26, input_pos[0].y+40);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, input_pos[31].x, input_pos[31].y-30, "Digital Input",13);
        // Channel Label
        SetTextColor(hDC,RGB(0,0,0));
        SetBkColor(hDC, RGB(0, 255, 0));
     // TextOut(hDC, input_pos[63].x, input_pos[63].y+16, "63                                                                       48", 75);
     // TextOut(hDC, input_pos[47].x, input_pos[47].y+16, "47                                                                       32", 75);
        TextOut(hDC, input_pos[31].x, input_pos[31].y+16, "31                                                                       16", 75);
        TextOut(hDC, input_pos[15].x, input_pos[15].y+16, "15                                                                        0", 75);
        SetBkColor(hDC, RGB(0, 255, 0));
        //Detect Channel Input
        for (i=0; i<32; i++) {
          if ((input_value>>i)&0x01)
             SelectObject(hDC, hRedBrush);
          else
             SelectObject(hDC, GetStockObject(WHITE_BRUSH));
          Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+17, input_pos[i].y+17);
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
        /* Digital Input */
        DI_ReadPort(card,PORT_DI_LOW,&input_value);

        hDC = GetDC(hWnd);
        for (i=0; i<32; i++) {
         if( ((input_value>>i)&0x01) != ((oldinput_value>>i)&0x01) )
         {
          if ((input_value>>i)&0x01)
             SelectObject(hDC, hRedBrush);
          else
             SelectObject(hDC, GetStockObject(WHITE_BRUSH));
          Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+17, input_pos[i].y+17);
         }
        }
        oldinput_value = input_value;
        ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
		if (card >= 0) Release_Card(card);
        DeleteObject(hRedBrush);
        DeleteObject(hGreenBrush);
        DeleteObject(hLTGreenBrush);
        KillTimer(hWnd, TIMER1);
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}
