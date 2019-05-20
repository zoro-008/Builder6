/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1999, ADLink Technology Inc.  All rights reserved.       ;;
;;                                                                         ;;
;;      File Name   :   Util7234.C                                         ;;
;;      Purpose     :   This sample program demonstrate how to operate     ;;
;;                                              DO for PCI-7234            ;;
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

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
U32 output_lo=0;
I16 card=-1, card_number=0;

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush;

POINT output_pos[64]= {
                       {320,160},  {300,160},
                       {280,160},  {260,160},
                       {240,160},  {220,160},
                       {200,160},  {180,160},
                       {160,160},  {140,160},
                       {120,160},  {100,160},
                       {80,160},   {60,160},
                       {40,160},   {20,160},
                       {320,120},  {300,120},
                       {280,120},  {260,120},
                       {240,120},  {220,120},
                       {200,120},  {180,120},
                       {160,120},  {140,120},
                       {120,120},  {100,120},
                       { 80,120},  {60,120},
                       { 40,120},  {20,120},
                       {320, 80},  {300, 80},
                       {280, 80},  {260, 80},
                       {240, 80},  {220, 80},
                       {200, 80},  {180, 80},
                       {160, 80},  {140, 80},
                       {120, 80},  {100, 80},
                       {80, 80},   {60, 80},
                       {40, 80},   {20, 80},
                       {320, 40},  {300, 40},
                       {280, 40},  {260, 40},
                       {240, 40},  {220, 40},
                       {200, 40},  {180, 40},
                       {160, 40},  {140, 40},
                       {120, 40},  {100, 40},
                       {80, 40},   {60, 40},
                       { 40, 40},  {20, 40}
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
  hSampleMenu = LoadMenu(hInstance, "U7234Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-7234",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      370,
                      260,
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
                     if ((card=Register_Card(PCI_7234, card_number)) < 0) {
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
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        // Digital Output Box
        SelectObject(hDC, hGreenBrush);
        Rectangle(hDC, output_pos[63].x-10, output_pos[63].y-24, 350, 200);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, output_pos[63].x, output_pos[63].y-30, "Digital Output",14);
        // Channel Label
        SetTextColor(hDC,RGB(0,0,0));
        SetBkColor(hDC, RGB(0, 128, 0));
        //TextOut(hDC, output_pos[63].x, output_pos[63].y+16, "63                                                                       48", 75);
        //TextOut(hDC, output_pos[47].x, output_pos[47].y+16, "47                                                                       32", 75);
        TextOut(hDC, output_pos[31].x, output_pos[31].y+16, "31                                                                       16", 75);
        TextOut(hDC, output_pos[15].x, output_pos[0].y+16, "15                                                                        0", 75);

        // Channel Output
        for (i=0; i<32; i++) {
          if ((output_lo>>i)&0x01)
             SelectObject(hDC, hRedBrush);
          else
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
    case WM_DESTROY:
		if (card >= 0) Release_Card(card);
        DeleteObject(hRedBrush);
        DeleteObject(hGreenBrush);
        DeleteObject(hLTGreenBrush);
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        px = LOWORD(lParam);
        py = HIWORD(lParam);

        for (i=0; i<32; i++) {
          if (px>output_pos[i].x && px<output_pos[i].x+17 && py>output_pos[i].y && py<output_pos[i].y+17) {
             hDC = GetDC(hWnd);
             if ((output_lo>>i)&0x01) {
                output_lo -= (((U32)1L)<<i);
                SelectObject(hDC, GetStockObject(WHITE_BRUSH));
             }
             else {
                output_lo += (((U32)1L)<<i);
                SelectObject(hDC, hRedBrush);
             }
             Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+17, output_pos[i].y+17);
             ReleaseDC(hWnd, hDC);
             DO_WritePort(card, 0, output_lo);

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