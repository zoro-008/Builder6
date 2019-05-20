/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   Util7260.C                                         ;;
;;      Purpose     :   This sample program demonstrate how to operate     ;;
;;						DI & DO	for PCI-7260                                  ;;
;;      Date        :   07/20/1999                                         ;;
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
#define IDC_ADDR        200
#define IDC_CLR         208
#define TIMER1            1
#define NONE              0

int   PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
I16 card=-1, card_number = 0;
U16 input[1], output[1], ot, tt;
U8 oldinput[1] ; 
U8 bcheck = 1;

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush;

POINT input_pos[8] = {{300, 40},  {270, 40}, 
                      {240, 40},  {210, 40}, 
                      {180, 40},  {150, 40}, 
                      {120, 40},  {90, 40}, 
                      };
POINT output_pos[8] ={{300, 95},  {270, 95}, 
                      {240,95},  {210, 95},
                      {180,95}, {150, 95}, 
                      {120,95},  {90, 95}, 
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
  hSampleMenu = LoadMenu(hInstance, "U7120Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-7260",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      350,
                      210,
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
              SendMessage(GetDlgItem(hWnd,IDC_CHECK_EMG), BM_SETCHECK, 1, 0);
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
                case IDC_CHECK_EMG:
                     if (bcheck) {
                        SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 0, 0);
                        bcheck=FALSE;
                     }
                     else {
                        SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
                        bcheck=TRUE;
                     }
                     break;
                case IDOK:
                     if ((card=Register_Card(PCI_7260, card_number)) < 0) {
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
  char s[50];
  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG1, hWnd, setup);	
		if(card<0) {
		  SendMessage(hWnd, WM_CLOSE, 0, 0L);
		  break;
		}
		//EMG shutdown control
		if(!bcheck) 
			EMGShutDownControl (card, EMGSHDN_OFF);
		else
			EMGShutDownControl (card, EMGSHDN_ON);

		hLTGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
        hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));        
        hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));               
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0)); 
		for (i=0;i<1;i++) { 
         input[i]=0;
         output[i]=0;
         oldinput[i]=0;
        }
		if(bcheck)
			CreateWindow("BUTTON","EMG_SHDN CLR" ,
                   WS_CHILD | WS_VISIBLE ,
                   output_pos[7].x+30, output_pos[i].y+30, 150, 30, hWnd, IDC_CLR, hInst, NULL);     
        SetTimer(hWnd, TIMER1, 50, NULL);
        srand((unsigned)time(NULL));
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);     
         
		SetTextColor(hDC,RGB(255,255,255));  
        SetBkColor(hDC, RGB(0, 0, 255)); 
        TextOut(hDC, 5, 20, "PCI7260   ", 9);      

        // Digital Input  Box
	    for (k=0;k<1;k++) {
          SelectObject(hDC, hLTGreenBrush);
          Rectangle(hDC, input_pos[7].x-10, k*110+input_pos[7].y-24, input_pos[0].x+26, k*110+input_pos[0].y+20);        
          SetTextColor(hDC,RGB(255,255,255));  
          SetBkColor(hDC, RGB(0, 0, 255));
          TextOut(hDC, input_pos[7].x, k*110+input_pos[7].y-30, "Digital Input",13);
        
          // Digital Output Box
          SelectObject(hDC, hGreenBrush);
          Rectangle(hDC, output_pos[7].x-10, k*110+output_pos[7].y-24, output_pos[0].x+26, k*110+output_pos[0].y+20);
          SetTextColor(hDC,RGB(255,0,0));  
          SetBkColor(hDC, RGB(0, 0, 255));
          TextOut(hDC, output_pos[7].x, k*110+output_pos[7].y-30, "Digital Output", 14);
         
          // Channel Label 
          SetTextColor(hDC,RGB(0,0,0));  
          SetBkColor(hDC, RGB(0, 255, 0));
          TextOut(hDC, input_pos[7].x, k*110+input_pos[7].y+1, " 7                                                   0", 54);       
          SetBkColor(hDC, RGB(0, 128, 0));
          TextOut(hDC, output_pos[7].x,k*110+output_pos[7].y+1," 7                                                   0", 54);
          SetBkColor(hDC, RGB(0, 255, 0));      
              
          // Detect Channel Input
          for (i=0; i<8; i++) {
            if ((input[k]>>i)&0x01)
             SelectObject(hDC, hRedBrush);
            else
             SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, input_pos[i].x, k*110+input_pos[i].y-13, input_pos[i].x+15, k*110+input_pos[i].y+2);
          }                
          // Channel Output
          for (i=0; i<8; i++) {
            if ((output[k]>>i)&0x01)
             SelectObject(hDC, hRedBrush);
            else
             SelectObject(hDC, GetStockObject(WHITE_BRUSH));
            Ellipse(hDC, output_pos[i].x, k*110+output_pos[i].y-13, output_pos[i].x+15, k*110+output_pos[i].y+2);
          }           
	    } 
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_CLR:
                //EMGShutDownRecovery (card);
			   EMGShutDownControl(card, EMGSHDN_RECOVERY);
                break;
           default:
                  return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_TIMER:
              /* Digital Input */
		    hDC = GetDC(hWnd);
		    for(k=0;k<1;k++) {
			  DI_ReadPort(card,k,&input[k]);
              for (i=0; i<8; i++) {
			   //DI_ReadLine(card, k, i, &tt);// If using DI_ReadLine
               if( ((input[k]>>i)&0x01) != ((oldinput[k]>>i)&0x01) ) 
			   //if ((tt&0x01) !=((oldinput[k]>>i)&0x01))// If using DI_ReadLine
               {
                 if ((input[k]>>i)&0x01)
				 //if (tt &0x01) // If using DI_ReadLine
                    SelectObject(hDC, hRedBrush);
                 else
                    SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                 Ellipse(hDC, input_pos[i].x, k*110+input_pos[i].y-13, input_pos[i].x+15, k*110+input_pos[i].y+2);
                }                
              } 
              oldinput[k]=input[k];
			}//k
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
    case WM_LBUTTONDOWN:
          px = LOWORD(lParam);
          py = HIWORD(lParam);
         for (k=0;k<1;k++) {
          for (i=0; i<8; i++) {
            if (px>output_pos[i].x && px<output_pos[i].x+15 && py>k*110+output_pos[i].y-13 && py<k*110+output_pos[i].y+2) {
               hDC = GetDC(hWnd);              
               if ((output[k]>>i)&0x01) {
                  output[k] -= (((unsigned long)1L)<<i);
				  //ot = (output[k]>>i) & 0x01; //If using DI_WriteLine
                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
               }
               else {
                  output[k] += (((unsigned long)1L)<<i);
				  //ot = (output[k]>>i)& 0x01; //If using DI_WriteLine
                  SelectObject(hDC, hRedBrush);
               }
               Ellipse(hDC, output_pos[i].x, k*110+output_pos[i].y-13, output_pos[i].x+15, k*110+output_pos[i].y+2);
               ReleaseDC(hWnd, hDC);
			   DO_WritePort(card,k, output[k]);
			   //DO_WriteLine (card, k, i, ot); //If using DI_WriteLine
               break;
            }
         }
		}//k
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}
