#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "resource.h"
#include "DASK.h"

#define IDM_EXIT        110
#define IDC_START       208
#define IDC_EDIT1       214
#define IDC_SHOW        213

#define PI          3.14159
#define MAX_SIZE    1024000   //in words

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowDMA(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hListBox;
HWND hEdit1, hStart;
I16 card=-1, card_number = 0;
U16 in_buf[MAX_SIZE], out_buf[MAX_SIZE];
int oldinput=0,adinput;
U32 data_size=100000;
U32 access_cnt, Memsize;
HWND    hMainWnd;

HBRUSH  hBlackBrush;
HPEN    hRedPen;

POINT input_pos[16]= {{470, 280},  {440, 280},
                       {410, 280},  {380, 280},
                       {350, 280},  {320, 280},
                       { 290, 280},  {260, 280},
                       {230, 280},  {200, 280},
                       {170, 280},  {140, 280},
                       {110, 280},  {80, 280},
                       { 50, 280},  { 20, 280},
                      };
POINT output_pos[16] ={
                       {470, 340},  {440, 340},
                       {410, 340},  {380, 340},
                       {350, 340},  {320, 340},
                       { 290, 340},  {260, 340},
                       {230, 340},  {200, 340},
                       {170, 340},  {140, 340},
                       {110, 340},  {80, 340},
                       { 50, 340},  { 20, 340},
                      };

POINT io_pos[16] ={
                       {395, 90},  {370, 90},
                       {345, 90},  {320, 90},
                       {295, 90},  {270, 90},
                       { 245, 90},  {220, 90},
                       {195, 90},  {170, 90},
                       {145, 90},  {120, 90},
                       {95, 90},  {70, 90},
                       { 45, 90},  { 20, 90},
                      };

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);

void DI_CallBack();
void DO_CallBack();

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
  hSampleMenu = LoadMenu(hInstance, "U7300Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-7300A Rev.B DMA",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      520,
                      400,
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
                     if ((card=Register_Card(PCI_7300A_RevB, card_number)) < 0) {
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
  char n[16];
  static int pos=0;
  I16 err;
  char s[100];

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG1, hWnd, setup);
		if(card<0) {
		    SendMessage(hWnd, WM_CLOSE, 0, 0L);
			break;
		}	
        err=DI_7300B_Config(card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
        if (err !=0) {
           wsprintf(s, "DI_7300B_Config Error = %d, card=%d\n", err, card);
           MessageBox(hWnd, s, "Error", MB_OK);
           SendMessage(hWnd, WM_CLOSE, 0, 0L);
           break;
        }
        err=DO_7300B_Config (card, 16, TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x40004000);
        if (err !=0) {
           wsprintf(s, "DO_7300B_Config Error = %d, card=%d\n", err, card);
           MessageBox(hWnd, s, "Error", MB_OK);
           SendMessage(hWnd, WM_CLOSE, 0, 0L);
           break;
        }
        for (i=0; i<MAX_SIZE; i++)
           out_buf[i] = (U16)(sin((double)i/5000*PI)*0x7fff)+0x8000;
        for (i=0; i<MAX_SIZE; i++)
           in_buf[i] = (U16)(i%65536);

        hEdit1=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x, input_pos[15].y+38,80, 20, hWnd, IDC_EDIT1, hInst, NULL);

        wsprintf(n,"%d",data_size);
        SetWindowText(hEdit1,n);

        hStart = CreateWindow("BUTTON","Start" ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+140, input_pos[10].y+30,80, 30, hWnd, IDC_START, hInst, NULL);

        CreateWindow("BUTTON","Data Value"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+230, input_pos[10].y+30,80, 30, hWnd, IDC_SHOW, hInst, NULL);
        hBlackBrush= CreateSolidBrush(RGB(0,0,0));
        hRedPen= CreatePen(PS_SOLID,1,RGB(255,0,0));
        break;

    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        // Digital Input Box
        SelectObject(hDC, GetStockObject(GRAY_BRUSH));
        Rectangle(hDC, output_pos[15].x-10, io_pos[15].y-24, output_pos[0].x+26, io_pos[0].y+195);
        SetTextColor(hDC,RGB(255,255,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "DMA Input", 9);
        SelectObject(hDC, hBlackBrush);
        Rectangle(hDC, output_pos[15].x+40/*35*/, io_pos[15].y-2, output_pos[15].x+456/*41*/, io_pos[0].y+172);

        SetBkMode(hDC,TRANSPARENT);
        TextOut(hDC,output_pos[13].x, input_pos[10].y+18,"Count (in words)",16);
        TextOut(hDC,output_pos[15].x-4, io_pos[15].y-8,"FFFFh",5);
        TextOut(hDC,output_pos[15].x+24, io_pos[15].y+162,"0",1);
        //SetTextColor(hDC,RGB(255,0,0));
        TextOut(hDC,output_pos[13].x-60, io_pos[15].y-90,"The configuration is set as DI16DO16. This sample perform DMA DI", 64);
        TextOut(hDC,output_pos[13].x-60, io_pos[15].y-75,"in 10MHz and DMA DO in 5MHZ rate simultaneously. The output data is", 67);
        TextOut(hDC,output_pos[13].x-60, io_pos[15].y-60,"a sine wave. The input data is shown in the panel below.", 56);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_SHOW :
                DialogBox(hInst, "DMABox", hWnd, ShowDMA);
                break;
           case IDC_START:
                data_size=GetDlgItemInt(hWnd,IDC_EDIT1,NULL ,FALSE);
                if (data_size > MAX_SIZE) {
                   data_size = MAX_SIZE;  //at most 1000K samples DMA
                   MessageBox(hWnd, "This sample supports at most 1000K words DMA transfer\nOnly perform 100K words DMA transfer.", "Warning", MB_OK);
                }
                hMainWnd = hWnd;
				DI_EventCallBack (card, 1, DIEnd, (U32) DI_CallBack );
				DO_EventCallBack (card, 1, DOEnd, (U32) DO_CallBack );
                err = DI_ContReadPort (card, 0, in_buf, data_size, 1, ASYNCH_OP);
                if (err < 0) {
                   wsprintf(s, "DI_ContReadPort Error = %d", err);
                   MessageBox(hWnd, s, "Error", MB_OK);
                   SendMessage(hWnd, WM_CLOSE, 0, 0L);
                   break;
                }
                err = DO_ContWritePort (card, 0, out_buf, data_size, 1, 5000000, ASYNCH_OP);
                if (err < 0) {
                   wsprintf(s, "DO_ContWritePort Error = %d", err);
                   MessageBox(hWnd, s, "Error", MB_OK);
                   SendMessage(hWnd, WM_CLOSE, 0, 0L);
                   break;
                }
                /*do {
                     DO_AsyncCheck (card, &stopped, &access_cnt);
                } while (!stopped);
                DO_AsyncClear (card, &access_cnt);
                do {
                     DI_AsyncCheck (card, &stopped, &access_cnt);
                } while (!stopped);
                DI_AsyncClear (card, &access_cnt);
                EnableWindow(hStart, FALSE);
                //Draw the input data in panel
                oldinput=-1;
                hDC = GetDC(hWnd);
                SelectObject(hDC, hBlackBrush);
                Rectangle(hDC,output_pos[15].x+40,io_pos[0].y-3,output_pos[15].x+456,io_pos[0].y+175);
                SelectObject(hDC, hRedPen);
                for (i=0; i<data_size; i++) {
                   adinput = in_buf[i];
                   adinput=io_pos[0].y+173-((175*adinput)/0xffff);
                   if (oldinput!=-1)
                      MoveToEx(hDC,i*416/data_size+output_pos[15].x+40,oldinput,NULL );
                   else
                      MoveToEx(hDC,i*416/data_size+output_pos[15].x+40,adinput,NULL );
                   LineTo(hDC,i*416/data_size+output_pos[15].x+41,adinput);
                   oldinput=adinput;
                }
                ReleaseDC(hWnd, hDC);
                EnableWindow(hStart, TRUE);*/
                break;
            default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_DESTROY:
        if (card >= 0) Release_Card(card);
        DeleteObject(hBlackBrush);
        DeleteObject(hRedPen);
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}

BOOL  PASCAL ShowDMA(hDlg, message, wParam, lParam)
HWND hDlg;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
    int nTabs[2] = {32, 92};
    long i;
    char cBuf[64];
    char string[10]="      ";
    char data_str[3]="  ";
    U16 *pp;
    switch (message) {
        case WM_INITDIALOG:
            hListBox = CreateWindow("LISTBOX", "Index                       Data (Hex)",
                           WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_STANDARD | LBS_USETABSTOPS |WS_CAPTION,
                           50, 40, 240, 180, hDlg, 100, hInst, NULL);
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hListBox, LB_SETTABSTOPS, 2, (DWORD)(LPINT) nTabs);
            SetDlgItemText(hDlg, DMA_DESCP, "This ListBox can show at most 4000 data");
            itoa( data_size, string, 10);
            SetDlgItemText(hDlg, DMA_SPACE, string);

            pp = in_buf;
            if (data_size > 8000) data_size = 8000;
            for(i=0;i<data_size; i++) {
                 wsprintf(cBuf, "%5d\t                %4x", i, pp[i]);
                 SendMessage(hListBox, LB_INSERTSTRING, -1, (DWORD)(LPSTR)cBuf);
            }
            ShowWindow(hListBox, SW_SHOW);
            return (TRUE);
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_STOP) {
                DestroyWindow(hListBox);
                EndDialog(hDlg, TRUE);
                return (TRUE);
            }
            break;
    }
    return (FALSE);
}

void DI_CallBack()
{
  HDC hDC;
  U32 i;
  U32 adinput;

  DI_AsyncClear(card, &access_cnt);
  EnableWindow(hStart, FALSE);
  //Draw the input data in panel
  oldinput=-1;
  hDC = GetDC(hMainWnd);
  SelectObject(hDC, hBlackBrush);
  Rectangle(hDC,output_pos[15].x+40,io_pos[0].y-3,output_pos[15].x+456,io_pos[0].y+175);
  SelectObject(hDC, hRedPen);
  for (i=0; i<data_size; i++) {
      adinput = in_buf[i];
      adinput=io_pos[0].y+173-((175*adinput)/0xffff);
      if (oldinput!=-1)
         MoveToEx(hDC,i*416/data_size+output_pos[15].x+40,oldinput,NULL );
      else
         MoveToEx(hDC,i*416/data_size+output_pos[15].x+40,adinput,NULL );
       LineTo(hDC,i*416/data_size+output_pos[15].x+41,adinput);
       oldinput=adinput;
  }
  ReleaseDC(hMainWnd, hDC);
  EnableWindow(hStart, TRUE);  
}

void DO_CallBack()
{
  DO_AsyncClear(card, &access_cnt);
}
