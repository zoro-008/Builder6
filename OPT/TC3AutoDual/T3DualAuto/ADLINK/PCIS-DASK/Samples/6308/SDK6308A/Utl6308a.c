#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "dask.h"
#include "resource.h"

#define IDM_EXIT        110
#define IDC_DA          300
#define IDC_SCROLL      400
#define TIMER1            1
#define NONE              0
#define SINE              1
#define SQUARE            2

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hDAComboBox[8];
HWND hDAScroll[4];

I16 card=-1, card_number = 0;
U16 input=0, output=0;
U16 oldinput=0 ;
//EventHandles hEnts;


HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hRedBrush;

double angle[8] = {0, 0, 0, 0, 0, 0, 0, 0};
I16 sq[8] = {0, 0, 0, 0, 0, 0, 0, 0};
I16 pos[4]={0, 0, 0, 0};

POINT input_pos[4]= { {110, 40},  {80, 40}, {50, 40},  {20, 40} };
POINT output_pos[4] ={ {250, 40},  {220, 40}, {190, 40},  {160, 40} };

POINT io_pos[24] ={
                       {595, 120},  {570, 120},
                       {545, 120},  {520, 120},
                       {495, 120},  {470, 120},
                       {445, 120},  {420, 120},
                       {395, 120},  {370, 120},
                       {345, 120},  {320, 120},
                       {295, 120},  {270, 120},
                       {245, 120},  {220, 120},
                       {195, 120},  {170, 120},
                       {145, 120},  {120, 120},
                       {95, 120},   {70, 120},
                       { 45, 120},  { 20, 120}
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
  hSampleMenu = LoadMenu(hInstance, "U6308Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI6308A",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      618,
                      256,
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
  int i,k;
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
                     if ((card=Register_Card(PCI_6308A, card_number)) < 0) {
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
  double Pi = 3.1415926535;
  char s[50];

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
        if(card < 0) {
            SendMessage(hWnd, WM_CLOSE, 0, 0L);
            break;
		}
		for (i=0; i<8; i++) {
            hDAComboBox[i]= CreateWindow("ComboBox", NULL,
                     CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
                     io_pos[i*3].x-60, io_pos[i*3].y, 64, 100, hWnd, IDC_DA+i, hInst, NULL);
            SendMessage(hDAComboBox[i], CB_ADDSTRING, NULL, (LONG)(LPSTR)"(None)");
            SendMessage(hDAComboBox[i], CB_SETCURSEL, 0, 0L);
        }

        for (i=0;i<4; i++) {
            hDAScroll[i]=CreateWindow("SCROLLBAR", NULL,
                 WS_CHILD | WS_VISIBLE | SBS_HORZ,
                 io_pos[5+i*6].x-10, io_pos[5+i*6].y+50, 140, 18, hWnd, IDC_SCROLL+i, hInst, NULL);
            SetScrollRange(hDAScroll[i],SB_CTL,0,4095,TRUE);
            SetScrollPos(hDAScroll[i],SB_CTL,0,FALSE);
        }

        hLTGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
        hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));
        hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0));

        SetTimer(hWnd, TIMER1, 50, NULL);
        srand((unsigned)time(NULL));
        AO_6308A_Config(card, P6308_CURRENT_4_20MA);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        // Digital Input  Box
        SelectObject(hDC, hLTGreenBrush);
        Rectangle(hDC, input_pos[3].x-10, input_pos[3].y-24, input_pos[0].x+26, input_pos[0].y+40);
        SetTextColor(hDC,RGB(255,255,255));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, input_pos[3].x, input_pos[3].y-30, "Digital Input",13);

        // Digital Output Box
        SelectObject(hDC, hGreenBrush);
        Rectangle(hDC, output_pos[3].x-10, output_pos[3].y-24, output_pos[0].x+26, output_pos[0].y+40);
        SetTextColor(hDC,RGB(255,0,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, output_pos[3].x, output_pos[3].y-30, "Digital Output", 14);

        // Analog Output Box
        SelectObject(hDC, GetStockObject(GRAY_BRUSH));
        Rectangle(hDC, io_pos[23].x-15, io_pos[23].y-24, io_pos[0].x+10, io_pos[0].y+80);
        SetTextColor(hDC,RGB(255,0,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, io_pos[23].x, io_pos[23].y-35, "Analog Output", 13);

        // Channel Label
        SetTextColor(hDC,RGB(0,0,0));
        SetBkColor(hDC, RGB(0, 255, 0));
        TextOut(hDC, input_pos[3].x+4, input_pos[3].y+20,   "3                     0", 23);
        SetBkColor(hDC, RGB(0, 128, 0));
        TextOut(hDC, output_pos[3].x+4, output_pos[3].y+20, "3                     0", 23);
        SetBkColor(hDC, RGB(0, 255, 0));

        SetBkMode(hDC,TRANSPARENT);
        TextOut(hDC,io_pos[21].x-45, output_pos[3].y+64, "A7", 2);
        TextOut(hDC,io_pos[18].x-45, output_pos[3].y+64, "A6", 2);
        TextOut(hDC,io_pos[21].x-50, output_pos[3].y+112, " 4         12          20",25);
        TextOut(hDC,io_pos[15].x-45, output_pos[3].y+64, "A5", 2);
        TextOut(hDC,io_pos[12].x-45, output_pos[3].y+64, "A4", 2);
        TextOut(hDC,io_pos[15].x-50, output_pos[3].y+112, " 4         12          20",25);
        TextOut(hDC,io_pos[9].x-45, output_pos[3].y+64, "A3", 2);
        TextOut(hDC,io_pos[6].x-45, output_pos[3].y+64, "A2", 2);
        TextOut(hDC,io_pos[9].x-50, output_pos[3].y+112, " 4          12         20",25);
        TextOut(hDC,io_pos[3].x-45, output_pos[3].y+64, "A1", 2);
        TextOut(hDC,io_pos[0].x-45, output_pos[3].y+64, "A0", 2);
        TextOut(hDC,io_pos[3].x-50, output_pos[3].y+112, " 4          12         20",25);
        SetBkMode(hDC,OPAQUE);

        // Detect Channel Input
        for (i=0; i<4; i++) {
           if ((input>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
           Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+20, input_pos[i].y+20);
        }

        // Channel Output
        for (i=0; i<4; i++) {
           if ((output>>i)&0x01)
              SelectObject(hDC, hRedBrush);
           else
              SelectObject(hDC, GetStockObject(WHITE_BRUSH));
           Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+20, output_pos[i].y+20);
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
                DI_ReadPort(card,0,&input);
        hDC = GetDC(hWnd);
        for (i=0; i<4; i++) {
            if ( ((input>>i)&0x01) != ((oldinput>>i)&0x01) ) {
               if ((input>>i)&0x01)
                  SelectObject(hDC, hRedBrush);
               else
                  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
               Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+20, input_pos[i].y+20);
            }
        }
        oldinput=input;
        ReleaseDC(hWnd, hDC);
        /* Analog Output */
        for (i=0; i<8; i++) {
           AO_WriteChannel(card, i, pos[i/2]);
        }
        break;
    case WM_HSCROLL:
        for (i=0; i<4; i++) {
            if (lParam == hDAScroll[i]) break;
        }
        if (i < 4) {
            switch(LOWORD(wParam)) {
            case SB_THUMBPOSITION :
                  pos[i] = HIWORD(wParam);
                  break;
            }
            SetScrollPos(lParam,SB_CTL,pos[i],TRUE);
        }
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

        hDC = GetDC(hWnd);

        for (i=0; i<4; i++) {
           if (px>output_pos[i].x && px<output_pos[i].x+20 && py>output_pos[i].y && py<output_pos[i].y+20) {
              if ((output>>i)&0x01) {
                 output -= (((U16)1)<<i);
                 SelectObject(hDC, GetStockObject(WHITE_BRUSH));
              }
              else {
                 output += (((U16)1)<<i);
                 SelectObject(hDC, hRedBrush);
              }
              Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+20, output_pos[i].y+20);
           }
        }
        ReleaseDC(hWnd, hDC);
        DO_WritePort(card,0, (U16)output);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}
