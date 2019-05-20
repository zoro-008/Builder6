/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1997, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   UTIL7249.C                                         ;;
;;      Purpose     :   This PCI-DASK sample program demonstrate how to    ;;
;;                      operate  DI & DO        for PCI-7249                               ;;
;;      Date        :   09/20/1998                                         ;;
;;      Revision    :   1.0                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Dask.h"
#include "resource.h"

#define IDM_EXIT        110
#define TIMER             2

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hAddrComboBox;
I16 card=-1, card_number = 0;

U32 inputA[2], inputB[2], inputC[2] , inputD[2];
U32 inputAE[2], inputBE[2], inputCE[2];
U32 outputA[2],outputB[2],outputC[2] ,outputD[2];
U32 oldinputA[2],  oldinputB[2], oldinputC[2], oldinputD[2];
U32 oldinputAE[2],  oldinputBE[2], oldinputCE[2];
U32 temp_output;

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush;

int port_a[2];
int port_b[2];
int port_c_upper[2];
int port_c_lower[2];
int port_ae[2];
int port_be[2];
int port_ce[2];

POINT input_pos[2][8];
POINT output_pos[2][8];
POINT io_pos[2][8];
POINT input_posE[2][8];
POINT output_posE[2][8];
POINT io_posE[2][8];

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
  while (GetMessage(&msg, NULL, 0, 0)) {
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
  wc.style = 0;
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
  hSampleMenu = LoadMenu(hInstance, "U7120Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-7249",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      640,
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
                     if ((card=Register_Card(PCI_7249, card_number)) < 0) {
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
  int i, k, px, py;

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
        input_pos[0][0].x=180;
        input_pos[0][0].y=40;
        input_pos[0][1].x=160;
        input_pos[0][1].y=40;
        input_pos[0][2].x=140;
        input_pos[0][2].y=40;
        input_pos[0][3].x=120;
        input_pos[0][3].y=40;
        input_pos[0][4].x=100;
        input_pos[0][4].y=40;
        input_pos[0][5].x=80;
        input_pos[0][5].y=40;
        input_pos[0][6].x=60;
        input_pos[0][6].y=40;
        input_pos[0][7].x=40;
        input_pos[0][7].y=40;

        output_pos[0][0].x=480;
        output_pos[0][0].y=40;
        output_pos[0][1].x=460;
        output_pos[0][1].y=40;
        output_pos[0][2].x=440;
        output_pos[0][2].y=40;
        output_pos[0][3].x=420;
        output_pos[0][3].y=40;
        output_pos[0][4].x=400;
        output_pos[0][4].y=40;
        output_pos[0][5].x=380;
        output_pos[0][5].y=40;
        output_pos[0][6].x=360;
        output_pos[0][6].y=40;
        output_pos[0][7].x=340;
        output_pos[0][7].y=40;

        io_pos[0][0].x=590;
        io_pos[0][0].y=40;
        io_pos[0][1].x=570;
        io_pos[0][1].y=40;
        io_pos[0][2].x=550;
        io_pos[0][2].y=40;
        io_pos[0][3].x=530;
        io_pos[0][3].y=40;
        io_pos[0][4].x=290;
        io_pos[0][4].y=40;
        io_pos[0][5].x=270;
        io_pos[0][5].y=40;
        io_pos[0][6].x=250;
        io_pos[0][6].y=40;
        io_pos[0][7].x=230;
        io_pos[0][7].y=40;

        for (k=0;k<8;k++)
        {
          input_pos[1][k].x  =input_pos[0][k].x;
          input_pos[1][k].y  =input_pos[0][k].y+50;
          output_pos[1][k].x =output_pos[0][k].x;
          output_pos[1][k].y =output_pos[0][k].y+50;
          io_pos[1][k].x     =io_pos[0][k].x;
          io_pos[1][k].y     =io_pos[0][k].y+50;
        }
        
        for (i=0;i<2;i++)
        {
         inputA[i]=0;
         outputA[i]=0;
         oldinputA[i]=0;

         inputB[i]=0;
         outputB[i]=0;
         oldinputB[i]=0;

         inputC[i]=0;
         outputC[i]=0;
         oldinputC[i]=0;

         inputD[i]=0;
         outputD[i]=0;
         oldinputD[i]=0;

         port_a[i]=0;
         DIO_PortConfig(card,5*i+Channel_P1A, INPUT_PORT);
         port_b[i]=0;
         DIO_PortConfig(card,5*i+Channel_P1B, INPUT_PORT);
         port_c_upper[i]=0;
         DIO_PortConfig(card,5*i+Channel_P1CL, INPUT_PORT);
         port_c_lower[i]=0;
         DIO_PortConfig(card,5*i+Channel_P1CH, INPUT_PORT);
        }

        input_posE[0][0].x=180;
        input_posE[0][0].y=180;
        input_posE[0][1].x=160;
        input_posE[0][1].y=180;
        input_posE[0][2].x=140;
        input_posE[0][2].y=180;
        input_posE[0][3].x=120;
        input_posE[0][3].y=180;
        input_posE[0][4].x=100;
        input_posE[0][4].y=180;
        input_posE[0][5].x=80;
        input_posE[0][5].y=180;
        input_posE[0][6].x=60;
        input_posE[0][6].y=180;
        input_posE[0][7].x=40;
        input_posE[0][7].y=180;

        output_posE[0][0].x=370;
        output_posE[0][0].y=180;
        output_posE[0][1].x=350;
        output_posE[0][1].y=180;
        output_posE[0][2].x=330;
        output_posE[0][2].y=180;
        output_posE[0][3].x=310;
        output_posE[0][3].y=180;
        output_posE[0][4].x=290;
        output_posE[0][4].y=180;
        output_posE[0][5].x=270;
        output_posE[0][5].y=180;
        output_posE[0][6].x=250;
        output_posE[0][6].y=180;
        output_posE[0][7].x=230;
        output_posE[0][7].y=180;

        io_posE[0][0].x=560;
        io_posE[0][0].y=180;
        io_posE[0][1].x=540;
        io_posE[0][1].y=180;
        io_posE[0][2].x=520;
        io_posE[0][2].y=180;
        io_posE[0][3].x=500;
        io_posE[0][3].y=180;
        io_posE[0][4].x=480;
        io_posE[0][4].y=180;
        io_posE[0][5].x=460;
        io_posE[0][5].y=180;
        io_posE[0][6].x=440;
        io_posE[0][6].y=180;
        io_posE[0][7].x=420;
        io_posE[0][7].y=180;

		for (k=0;k<8;k++) {
          input_posE[1][k].x  =input_posE[0][k].x;
          input_posE[1][k].y  =input_posE[0][k].y+50;
          output_posE[1][k].x =output_posE[0][k].x;
          output_posE[1][k].y =output_posE[0][k].y+50;
          io_posE[1][k].x     =io_posE[0][k].x;
          io_posE[1][k].y     =io_posE[0][k].y+50;
		}
        for (i=0; i<2; i++) {
		 inputAE[i]=0;
         oldinputAE[i]=0;
         inputBE[i]=0;
         oldinputBE[i]=0;
         inputCE[i]=0;
         oldinputCE[i]=0;
		}
        SetTimer(hWnd, TIMER, 50, NULL);
        srand((unsigned)time(NULL));
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        //  Label
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, 5, 40, "CN1", 3);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, 5, 80, "CN2", 3);
        TextOut(hDC, 5, 180, "CN1", 3);
        TextOut(hDC, 5, 220, "CN2", 3);

        // Digital Input  Box
        for (i=0;i<2;i++ )
        {
          if ( port_a[i]==0 )
            SelectObject(hDC, hLTGreenBrush);
          else
            SelectObject(hDC, hGreenBrush);
          Rectangle(hDC, input_pos[i][7].x-5, input_pos[i][7].y-12, input_pos[i][0].x+26, input_pos[i][0].y+30);
          SetTextColor(hDC,RGB(0,0,0));
          SelectObject(hDC, hLTGreenBrush);
          Rectangle(hDC, input_pos[i][0].x+25, input_pos[i][0].y-12, input_pos[i][0].x+41, input_pos[i][0].y+9);
          SetBkColor(hDC, RGB(0, 255, 0));
          TextOut(hDC, input_pos[i][0].x+30, input_pos[i][7].y-11, "I", 1);
          SelectObject(hDC, hGreenBrush);
          Rectangle(hDC, input_pos[i][0].x+25, input_pos[i][0].y+9, input_pos[i][0].x+41, input_pos[i][0].y+30);
          SetBkColor(hDC, RGB(0, 128, 0));
          TextOut(hDC, input_pos[i][0].x+27, input_pos[i][7].y+7, "O", 1);
          SelectObject(hDC, hLTGreenBrush);
          Rectangle(hDC, input_posE[i][7].x-5, input_posE[i][7].y-12, input_posE[i][0].x+26, input_posE[i][0].y+30);
         }

         for (i=0;i<2;i++ )
         {
         if ( port_b[i]==0 )
           SelectObject(hDC, hLTGreenBrush);
         else
           SelectObject(hDC, hGreenBrush);
         Rectangle(hDC, output_pos[i][7].x-5, output_pos[i][7].y-12, output_pos[i][0].x+26, output_pos[i][0].y+30);
         SelectObject(hDC, hGreenBrush);
         SetTextColor(hDC,RGB(0,0,0));
         Rectangle(hDC, output_pos[i][0].x+25, output_pos[i][0].y+9, output_pos[i][0].x+41, output_pos[i][0].y+30);
         SetBkColor(hDC, RGB(0, 128, 0));
         TextOut(hDC, output_pos[i][0].x+27, output_pos[i][7].y+9, "O", 1);
         SelectObject(hDC, hLTGreenBrush);
         Rectangle(hDC, output_pos[i][0].x+25, output_pos[i][0].y-12, output_pos[i][0].x+41, output_pos[i][0].y+9);
         SetBkColor(hDC, RGB(0, 255, 0));
         TextOut(hDC, output_pos[i][0].x+30, output_pos[i][7].y-11, "I", 1);
		 SelectObject(hDC, hLTGreenBrush);
         Rectangle(hDC, output_posE[i][7].x-5, output_posE[i][7].y-12, output_posE[i][0].x+26, output_posE[i][0].y+30);
        }

        for (i=0;i<2;i++ )
        {
         if ( port_c_upper[i]==0 )
          SelectObject(hDC, hLTGreenBrush);
         else
           SelectObject(hDC, hGreenBrush);
         Rectangle(hDC, io_pos[i][7].x-5, io_pos[i][7].y-12, io_pos[i][4].x+21, io_pos[i][4].y+30);
         SelectObject(hDC, hLTGreenBrush);
         Rectangle(hDC, io_pos[i][4].x+25, io_pos[i][4].y-12, io_pos[i][4].x+41, io_pos[i][4].y+9);
         SetBkColor(hDC, RGB(0, 255, 0));
         SetTextColor(hDC,RGB(0,0,0));
         TextOut(hDC, io_pos[i][4].x+30, io_pos[i][4].y-11, "I", 1);
         SelectObject(hDC, hGreenBrush);
         Rectangle(hDC, io_pos[i][4].x+25, io_pos[i][4].y+9, io_pos[i][4].x+41, io_pos[i][4].y+30);
         SetBkColor(hDC, RGB(0, 128, 0));
         TextOut(hDC, io_pos[i][4].x+28, io_pos[i][4].y+9, "O", 1);
         SelectObject(hDC, hLTGreenBrush);
         Rectangle(hDC, io_posE[i][7].x-5, io_posE[i][7].y-12, io_posE[i][0].x+21, io_posE[i][0].y+30);
        }
        for (i=0;i<2;i++ )
        {
         if ( port_c_lower[i]==0 )
            SelectObject(hDC, hLTGreenBrush);
         else
            SelectObject(hDC, hGreenBrush);
         Rectangle(hDC, io_pos[i][3].x-5, io_pos[i][3].y-12, io_pos[i][0].x+21, io_pos[i][0].y+30);
         SelectObject(hDC, hLTGreenBrush);
         Rectangle(hDC, io_pos[i][0].x+25, io_pos[i][0].y-12, io_pos[i][0].x+41, io_pos[i][0].y+9);
         SetBkColor(hDC, RGB(0, 255, 0));
         SetTextColor(hDC,RGB(0,0,0));
         TextOut(hDC, io_pos[i][0].x+30, io_pos[i][7].y-11, "I", 1);
         SelectObject(hDC, hGreenBrush);
         Rectangle(hDC, io_pos[i][0].x+25, io_pos[i][0].y+9, io_pos[i][0].x+41, io_pos[i][0].y+30);
         SetBkColor(hDC, RGB(0, 128, 0));
         TextOut(hDC, io_pos[i][0].x+28, io_pos[i][7].y+9, "O", 1);
        }
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, input_pos[0][7].x, input_pos[0][7].y-20,   "Port A", 6);
        TextOut(hDC, output_pos[0][7].x, output_pos[0][7].y-20, "Port B", 6);
        TextOut(hDC, io_pos[0][7].x, io_pos[0][7].y-20, "Port C(Upper)", 13);
        TextOut(hDC, io_pos[0][3].x, io_pos[0][3].y-20, "Port C(Lower)", 13);
        TextOut(hDC, input_posE[0][7].x, input_posE[0][7].y-20,   "Port A", 6);
        TextOut(hDC, output_posE[0][7].x, output_posE[0][7].y-20, "Port B", 6);
        TextOut(hDC, io_posE[0][7].x, io_posE[0][7].y-20, "Port C", 6);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(255, 0, 0));
        TextOut(hDC, input_posE[0][7].x-40, input_posE[0][7].y-40,   "External Trigger DI", 19);
        // Channel Label
        SetBkMode(hDC,TRANSPARENT);
        SetTextColor(hDC,RGB(0,0,0));
        TextOut(hDC, input_pos[0][7].x, input_pos[0][7].y+16, " 7                                 0", 36);
        TextOut(hDC, io_pos[0][7].x, io_pos[0][7].y+16, " 3             0", 16);
        TextOut(hDC, io_pos[0][3].x, io_pos[0][3].y+16, " 3             0", 16);
        TextOut(hDC, output_pos[0][7].x,output_pos[0][7].y+16," 7                                 0", 36);

        TextOut(hDC, input_posE[0][7].x, input_posE[0][7].y+16, " 7                                 0", 36);
        TextOut(hDC, io_posE[0][7].x, io_posE[0][7].y+16, " 7                                 0", 36);
        TextOut(hDC, output_posE[0][7].x,output_posE[0][7].y+16," 7                                 0", 36);

        SetBkMode(hDC,OPAQUE);
        // Detect Channel Input
        for (k=0;k<2;k++ )
        {
         if ( port_a[k]==0 )
         {
          for (i=0; i<8; i++) {
           if ((inputA[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
           Ellipse(hDC, input_pos[k][i].x, input_pos[k][i].y, input_pos[k][i].x+17, input_pos[k][i].y+17);
          }
         }
         else
         {
             // Detect Channel Output
          for (i=0; i<8; i++) {
           if ((outputA[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
           Ellipse(hDC, input_pos[k][i].x, input_pos[k][i].y, input_pos[k][i].x+17, input_pos[k][i].y+17);
          }
         }
         if ( port_b[k]==0 )
         {
          for (i=0; i<8; i++) {
            if ((inputB[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
            else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, output_pos[k][i].x, output_pos[k][i].y, output_pos[k][i].x+17, output_pos[k][i].y+17);
          }
         }
         else
         {
          for (i=0; i<8; i++) {
            if ((outputB[k]>>i)&0x01)
               SelectObject(hDC, hRedBrush);
            else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, output_pos[k][i].x, output_pos[k][i].y, output_pos[k][i].x+17, output_pos[k][i].y+17);
          }
         }         
		 if ( port_c_lower[k]==0)
         {
         for (i=0; i<4; i++) {
           if ((inputC[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, io_pos[k][i].x, io_pos[k][i].y, io_pos[k][i].x+17, io_pos[k][i].y+17);
          }
         }
         else
         {
         for (i=0; i<4; i++) {
           if ((outputC[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, io_pos[k][i].x, io_pos[k][i].y, io_pos[k][i].x+17, io_pos[k][i].y+17);
          }
        }
        if ( port_c_upper[k]==0)
        {
         for (i=0; i<4; i++) {
           if ((inputD[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, io_pos[k][i+4].x, io_pos[k][i+4].y, io_pos[k][i+4].x+17, io_pos[k][i+4].y+17);
          }
         }
        else
        {
         for (i=0; i<4; i++) {
           if ((outputD[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, io_pos[k][i+4].x, io_pos[k][i+4].y, io_pos[k][i+4].x+17, io_pos[k][i+4].y+17);
          }
         }
		//external trigger
         for (i=0; i<8; i++) {
           if ((inputAE[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
           Ellipse(hDC, input_posE[k][i].x, input_posE[k][i].y, input_posE[k][i].x+17, input_posE[k][i].y+17);
         }
         for (i=0; i<8; i++) {
            if ((inputBE[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
            else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, output_posE[k][i].x, output_posE[k][i].y, output_posE[k][i].x+17, output_posE[k][i].y+17);
		 }
         for (i=0; i<8; i++) {
           if ((inputCE[k]>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, io_posE[k][i].x, io_posE[k][i].y, io_posE[k][i].x+17, io_posE[k][i].y+17);
         }
        } //k
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
          hDC=GetDC(hWnd);
          for(k=0;k<2;k++)
          {
            if ( port_a[k]==0) {
              DI_ReadPort(card,5*k+Channel_P1A,&inputA[k]);
              for (i=0; i<8; i++) {
               if (((inputA[k]>>i)&0x01) != ((oldinputA[k]>>i)&0x01) )
               {
                 if ((inputA[k]>>i)&0x01)
                    SelectObject(hDC, hRedBrush);
                 else
                    SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                 Ellipse(hDC, input_pos[k][i].x, input_pos[k][i].y, input_pos[k][i].x+17, input_pos[k][i].y+17);
               }
              }
              oldinputA[k]=inputA[k];
            }
            else {
              DO_WritePort(card,5*k+Channel_P1A, outputA[k]);
            }
            if ( port_b[k]==0){
              DI_ReadPort(card,5*k+Channel_P1B,&inputB[k]);
              for (i=0; i<8; i++) {
               if( ((inputB[k]>>i)&0x01) != ((oldinputB[k]>>i)&0x01) )
               {
                if ((inputB[k]>>i)&0x01)
                   SelectObject(hDC, hRedBrush);
                else
                   SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                Ellipse(hDC, output_pos[k][i].x, output_pos[k][i].y, output_pos[k][i].x+17, output_pos[k][i].y+17);
               }
              }
              oldinputB[k]=inputB[k];
            }
            else {
                DO_WritePort(card,5*k+Channel_P1B,outputB[k]);
            }
            if ( port_c_lower[k]==0){
               DI_ReadPort(card,5*k+Channel_P1CL,&inputC[k]);
               for (i=0; i<4; i++) {
                if( ((inputC[k]>>i)&0x01) != ((oldinputC[k]>>i)&0x01) )
                {
                 if ((inputC[k]>>i)&0x01)
                   SelectObject(hDC, hRedBrush);
                 else
                   SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                 Ellipse(hDC, io_pos[k][i].x, io_pos[k][i].y, io_pos[k][i].x+17, io_pos[k][i].y+17);
                }
               } //for
               oldinputC[k]=inputC[k];
            } //if
            else {
               DO_WritePort(card,5*k+Channel_P1CL, outputC[k]);
            }
            if ( port_c_upper[k]==0){
               DI_ReadPort(card,5*k+Channel_P1CH, &inputD[k]);
               for (i=0; i<4; i++) {
                if( ((inputD[k]>>i)&0x01) != ((oldinputD[k]>>i)&0x01) )
                {
                 if ((inputD[k]>>i)&0x01)
                   SelectObject(hDC, hRedBrush);
                 else
                   SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                 Ellipse(hDC, io_pos[k][i+4].x, io_pos[k][i+4].y, io_pos[k][i+4].x+17, io_pos[k][i+4].y+17);
                }
               }
               oldinputD[k]=inputD[k];
            }
            else
            {
               DO_WritePort(card,5*k+Channel_P1CH,outputD[k]);
            }
			//external trigger input
            DI_ReadPort(card,5*k+Channel_P1AE,&inputAE[k]);
            for (i=0; i<8; i++) {
               if (((inputAE[k]>>i)&0x01) != ((oldinputAE[k]>>i)&0x01) )
               {
                 if ((inputAE[k]>>i)&0x01)
                    SelectObject(hDC, hRedBrush);
                 else
                    SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                 Ellipse(hDC, input_posE[k][i].x, input_posE[k][i].y, input_posE[k][i].x+17, input_posE[k][i].y+17);
               }
            }
            oldinputAE[k]=inputAE[k];
            DI_ReadPort(card,5*k+Channel_P1BE,&inputBE[k]);
            for (i=0; i<8; i++) {
               if( ((inputBE[k]>>i)&0x01) != ((oldinputBE[k]>>i)&0x01) )
               {
                if ((inputBE[k]>>i)&0x01)
                   SelectObject(hDC, hRedBrush);
                else
                   SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                Ellipse(hDC, output_posE[k][i].x, output_posE[k][i].y, output_posE[k][i].x+17, output_posE[k][i].y+17);
               }
            }
            oldinputBE[k]=inputBE[k];
            DI_ReadPort(card,5*k+Channel_P1CE,&inputCE[k]);
            for (i=0; i<8; i++) {
               if( ((inputCE[k]>>i)&0x01) != ((oldinputCE[k]>>i)&0x01) )
               {
                if ((inputCE[k]>>i)&0x01)
                   SelectObject(hDC, hRedBrush);
                else
                   SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                Ellipse(hDC, io_posE[k][i].x, io_posE[k][i].y, io_posE[k][i].x+17, io_posE[k][i].y+17);
               }
            } //for
            oldinputCE[k]=inputCE[k];			 
          } //k
          ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
        if (card >= 0) Release_Card(card);
        DeleteObject(hRedBrush);
        DeleteObject(hGreenBrush);
        DeleteObject(hLTGreenBrush);
        KillTimer(hWnd, TIMER);
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
          px = LOWORD(lParam);
          py = HIWORD(lParam);

         for (k=0;k<2;k++ )
         {
          if (px>(input_pos[k][0].x+25) && px<(input_pos[k][0].x+41))
          {
           if (py>(input_pos[k][0].y-12) && py<(input_pos[k][0].y+7))
           {
            port_a[k]=0;
            DIO_PortConfig(card,5*k+Channel_P1A, INPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
            }
           else
           if (py>(input_pos[k][0].y+7) && py<(input_pos[k][0].y+26))
           {
            port_a[k]=1;
            DIO_PortConfig(card,5*k+Channel_P1A, OUTPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
            }
           }
          if (px>(output_pos[k][0].x+25) && px<(output_pos[k][0].x+41))
          {
           if (py>(output_pos[k][0].y-12) && py<(output_pos[k][0].y+7))
           {
            port_b[k]=0;
            DIO_PortConfig(card,5*k+Channel_P1B, INPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
            }
           else
           if (py>(output_pos[k][0].y+7) && py<(output_pos[k][0].y+26))
           {
            port_b[k]=1;
            DIO_PortConfig(card,5*k+Channel_P1B, OUTPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
           }
          }
          if (px>(io_pos[k][0].x+25) && px<(io_pos[k][0].x+41))
          {
           if (py>(io_pos[k][0].y-12) && py<(io_pos[k][0].y+7))
           {
            port_c_lower[k]=0;
            DIO_PortConfig(card,5*k+Channel_P1CL, INPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
           }
           else
           if (py>(io_pos[k][0].y+7) && py<(io_pos[k][0].y+26))
           {
            port_c_lower[k]=1;
            DIO_PortConfig(card,5*k+Channel_P1CL, OUTPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
            }
          }
          if (px>(io_pos[k][4].x+25) && px<(io_pos[k][4].x+41))
          {
           if (py>(io_pos[k][4].y-12) && py<(io_pos[k][4].y+7))
           {
            port_c_upper[k]=0;
            DIO_PortConfig(card,5*k+Channel_P1CH, INPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
            }
           else
           if (py>(io_pos[k][4].y+7) && py<(io_pos[k][4].y+26))
           {
            port_c_upper[k]=1;
            DIO_PortConfig(card,5*k+Channel_P1CH, OUTPUT_PORT);
            InvalidateRect(hWnd, NULL, FALSE);
           }
          }
         }//k
         hDC = GetDC(hWnd);
         for (k=0;k<2;k++)
         {
           if ( port_a[k]==1 ){
            for (i=0; i<8; i++) {
             if (px>input_pos[k][i].x && px<input_pos[k][i].x+17 && py>input_pos[k][i].y && py<input_pos[k][i].y+17) {
               if ((outputA[k]>>i)&0x01) {
                  outputA[k] -= (((unsigned char)1)<<i);
                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
               }
               else {
                  outputA[k] += (((unsigned char)1)<<i);
                  SelectObject(hDC, hRedBrush);
               }
               Ellipse(hDC, input_pos[k][i].x, input_pos[k][i].y, input_pos[k][i].x+17, input_pos[k][i].y+17);

               DO_WritePort(card,5*k+Channel_P1A, outputA[k]);
               break;
             }
            }
           }
           if ( port_b[k]==1 )
           {
            for (i=0; i<8; i++) {
             if (px>output_pos[k][i].x && px<output_pos[k][i].x+17 && py>output_pos[k][i].y && py<output_pos[k][i].y+17) {
               if ((outputB[k]>>i)&0x01) {
                  outputB[k] -= (((unsigned char)1)<<i);
                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
               }
               else {
                  outputB[k] += (((unsigned char)1)<<i);
                  SelectObject(hDC, hRedBrush);
               }
               Ellipse(hDC, output_pos[k][i].x, output_pos[k][i].y, output_pos[k][i].x+17, output_pos[k][i].y+17);
               DO_WritePort(card,5*k+Channel_P1B, outputB[k]);
               break;
             }
            }//for
           }
           if ( port_c_lower[k]==1 )
           {
            for (i=0; i<4; i++) {
             if (px>io_pos[k][i].x && px<io_pos[k][i].x+17 && py>io_pos[k][i].y && py<io_pos[k][i].y+17) {
               if ((outputC[k]>>i)&0x01) {
                  outputC[k] -= (((unsigned char)1)<<i);
                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
               }
               else {
                  outputC[k] += (((unsigned char)1)<<i);
                  SelectObject(hDC, hRedBrush);
               }
               Ellipse(hDC, io_pos[k][i].x, io_pos[k][i].y, io_pos[k][i].x+17, io_pos[k][i].y+17);
               DO_WritePort(card,5*k+Channel_P1CL, outputC[k]);
               break;
             }
            }
           }
           if ( port_c_upper[k]==1 )
           {
            for (i=0; i<4; i++) {
             if (px>io_pos[k][i+4].x && px<io_pos[k][i+4].x+17 && py>io_pos[k][i+4].y && py<io_pos[k][i+4].y+17) {
               if ((outputD[k]>>i)&0x01) {
                  outputD[k] -= (((unsigned char)1)<<i);
                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
               }
               else {
                  outputD[k] += (((unsigned char)1)<<i);
                  SelectObject(hDC, hRedBrush);
               }
               Ellipse(hDC, io_pos[k][i+4].x, io_pos[k][i+4].y, io_pos[k][i+4].x+17, io_pos[k][i+4].y+17);
               DO_WritePort(card,5*k+Channel_P1CH, outputD[k]);
               break;
             }
            }
           }
         }//k
         ReleaseDC(hWnd,hDC);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(0);
}
