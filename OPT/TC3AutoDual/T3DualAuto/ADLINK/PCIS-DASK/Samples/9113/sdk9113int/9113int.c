#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "resource.h"
#include "dask.h"

#define IDM_EXIT        110
#define IDC_ADDR        200
#define IDC_SCROLL      207
#define IDC_COMBO1      1015
#define IDC_START       208
#define IDC_STOP        209
#define IDC_PAUSE       210
#define IDC_EDIT        211
#define IDC_CHAN        214
#define IDC_SHOW        213
#define NONE              0

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowINT(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hListBox;
HWND hChanComboBox, hCardComboBox;
HWND hADScroll;
HWND hEdit,hEdit1,hData;
int input=0, output=0;
I32 oldinput[8], adinput;

HBRUSH  hBlackBrush;
HPEN    hRedPen;

long  sample_rate=10000;
unsigned int base_addr=NONE,gain=1;
I16 card = -1, card_number =0;;
long  data_size=10240;
U16   channel=0, range = AD_B_10_V;

DWORD  mem_size = 0;
static HANDLE hMem=NULL;
U32 *ai_buf;

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
                       {395, 40},  {370, 40},
                       {345, 40},  {320, 40},
                       {295, 40},  {270, 40},
                       { 245, 40},  {220, 40},
                       {195, 40},  {170, 40},
                       {145, 40},  {120, 40},
                       {95, 40},  {70, 40},
                       { 45, 40},  { 20, 40},
                      };

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush,hBlackBrush,hWhiteBrush
        ,hLTRedBrush,hLTBlueBrush;
HPEN    hRedPen,hYellowPen,hGreenPen,hBluePen,hLTBluePen,hLTRedPen,hLTGreenPen,hWhitePen;

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
  hSampleMenu = LoadMenu(hInstance, "U9113Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"9113 Autoscan Interrupt",
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
                     if ((card=Register_Card(PCI_9113, card_number)) < 0) {
                        wsprintf(s, "Register_Card Error = %d\n", card);
                        MessageBox(hWnd, s, "Error", MB_OK);
                        SendMessage(hWnd, WM_CLOSE, 0, 0L);
                        break;
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
  int i;
  int cur_channel;
  char n[6];
  static   int pos=0;
  static int count,status;
  PAINTSTRUCT ps;

  switch(message) {
    case WM_CREATE:
           DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
     	   if(card<0) {
	    	 SendMessage(hWnd, WM_CLOSE, 0, 0L);
		     break;
	       }
           hChanComboBox = CreateWindow("ComboBox", NULL,
                   CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
                   input_pos[10].x, input_pos[15].y+18, 100, 160, hWnd, IDC_CHAN, hInst, NULL);
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH1");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH2");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH3");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH4");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH5");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH6");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH0 ~ CH7");
           SendMessage(hChanComboBox ,CB_SETCURSEL, 0, 0L);
           hEdit=CreateWindow("EDIT", NULL,
                   WS_CHILD | WS_VISIBLE | ES_LEFT,
                   output_pos[13].x-30, input_pos[15].y+20,40, 20, hWnd, IDC_EDIT, hInst, NULL);
           wsprintf(n,"%d",sample_rate/1000);
           SetWindowText(hEdit,n);
           SendMessage(hCardComboBox ,CB_SETCURSEL, 0, 0L);
                   CreateWindow("SCROLLBAR", NULL,
                   WS_CHILD | WS_VISIBLE | SBS_VERT,
                   output_pos[13].x+10, input_pos[15].y+20,20, 20, hWnd, IDC_SCROLL, hInst, NULL);
           CreateWindow("BUTTON","Start" ,
                   WS_CHILD | WS_VISIBLE ,
                   output_pos[10].x+140, input_pos[10].y+10,70, 30, hWnd, IDC_START, hInst, NULL);
           hData=CreateWindow("BUTTON","Data Value"   ,
                   WS_CHILD | WS_VISIBLE ,
                   output_pos[10].x+230, input_pos[10].y+10,80, 30, hWnd, IDC_SHOW, hInst, NULL);
           hLTGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
           hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));
           hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
           hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
           hYellowBrush= CreateSolidBrush(RGB(255,255,0));
           hBlackBrush= CreateSolidBrush(RGB(0,0,0));
           hLTBlueBrush= CreateSolidBrush(RGB(0,255,255));
           hLTRedBrush= CreateSolidBrush(RGB(255,128,0));
           hWhiteBrush= CreateSolidBrush(RGB(255,255,255));
           hRedPen= CreatePen(PS_SOLID,1,RGB(255,0,0));
           hYellowPen= CreatePen(PS_SOLID,1,RGB(255,255,0));
           hBluePen= CreatePen(PS_SOLID,1,RGB(0,0,255));
           hGreenPen= CreatePen(PS_SOLID,1,RGB(0,255,0));
           hLTBluePen= CreatePen(PS_SOLID,1,RGB(0,128,255));
           hLTRedPen= CreatePen(PS_SOLID,1,RGB(128,128,0));
           hLTGreenPen= CreatePen(PS_SOLID,1,RGB(0,128,0));
           hWhitePen= CreatePen(PS_SOLID,1,RGB(255,255,255));
           break;
    case WM_PAINT:
           hDC = BeginPaint(hWnd, &ps);
           // Analog Input Box
           SelectObject(hDC, GetStockObject(GRAY_BRUSH));
           Rectangle(hDC, output_pos[15].x-10, io_pos[15].y-24, output_pos[0].x+26, io_pos[0].y+195);
           SetTextColor(hDC,RGB(255,255,0));
           SetBkColor(hDC, RGB(0, 0, 255));
           TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "Interrupt Input(A/D)  ", 22);

           SelectObject(hDC, hBlackBrush);
           Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);

           SelectObject(hDC,hYellowBrush );
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y-5+18,output_pos[4].x+80, io_pos[0].y+10+18);

           SelectObject(hDC, hGreenBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+13+18,output_pos[4].x+80, io_pos[0].y+28+18);

           SelectObject(hDC, hBlueBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+31+18,output_pos[4].x+80, io_pos[0].y+46+18);

           SelectObject(hDC, hRedBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+49+18,output_pos[4].x+80, io_pos[0].y+64+18);

           SelectObject(hDC, hLTBlueBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y-5+18,output_pos[4].x+120, io_pos[0].y+10+18);

           SelectObject(hDC, hLTRedBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+13+18,output_pos[4].x+120, io_pos[0].y+28+18);

           SelectObject(hDC, hLTGreenBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+31+18,output_pos[4].x+120, io_pos[0].y+46+18);

           SelectObject(hDC, hWhiteBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+49+18,output_pos[4].x+120, io_pos[0].y+64+18);

           SetBkMode(hDC,TRANSPARENT);
           TextOut(hDC,output_pos[13].x-60, input_pos[10].y,"Samping Rate",12);
           TextOut(hDC,output_pos[13].x+30, input_pos[15].y+20,"KHZ",3);
           TextOut(hDC,output_pos[13].x+80, input_pos[10].y,"Channel",7);
           TextOut(hDC,input_pos[2].x-10, io_pos[15].y+118,"Range: +/-10V",13);
           TextOut(hDC,input_pos[15].x, io_pos[15].y-19,"Volts",5);
           TextOut(hDC,input_pos[15].x, io_pos[15].y-5,"10.0",4);
           TextOut(hDC,input_pos[15].x, io_pos[15].y+36,"5.00",4);
           TextOut(hDC,input_pos[15].x, io_pos[15].y+77,"0.00",4);
           TextOut(hDC,input_pos[15].x-3, io_pos[15].y+118,"-5.00",5);
           TextOut(hDC,input_pos[15].x-3, io_pos[15].y+159,"-10.0",5);
           TextOut(hDC,input_pos[10].x+10, io_pos[10].y-19,"Signals",7);

           TextOut(hDC,output_pos[4].x+58, io_pos[0].y-5+18,"0",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+13+18,"1",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+31+18,"2",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+49+18,"3",1);
           TextOut(hDC,output_pos[4].x+98, io_pos[0].y-5+18,"4",1);
           TextOut(hDC,output_pos[4].x+98, io_pos[0].y+13+18,"5",1);
           TextOut(hDC,output_pos[4].x+98, io_pos[0].y+31+18,"6",1);
           TextOut(hDC,output_pos[4].x+98, io_pos[0].y+49+18,"7",1);

           TextOut(hDC,output_pos[13].x-63, input_pos[10].y-35,"Card",4);
           TextOut(hDC,output_pos[13].x-63, input_pos[10].y-22,"Type",4);

           EndPaint(hWnd, &ps);
           break;
    case WM_VSCROLL:
           // Sample Rate Spin Control
           switch(LOWORD(wParam)){
             case SB_LINEUP :
                sample_rate+=1000;
                if (sample_rate > 25000 ) sample_rate=25000;
                break;
             case SB_LINEDOWN:
                sample_rate-=1000;
                if (sample_rate < 1000 ) sample_rate=1000;
                break;
           }
           wsprintf(n,"%d",sample_rate/1000);
           SetWindowText(hEdit,n);
           break;
    case WM_COMMAND:
         switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_SHOW :
                DialogBox(hInst, "IntBox", hWnd, ShowINT);
                break;
           case IDC_CHAN:
              if (HIWORD(wParam) == CBN_CLOSEUP) {
                 if ((i=(int)SendMessage(GetDlgItem(hWnd,IDC_CHAN),CB_GETCURSEL, 0, 0L))>=0 ) {
                    channel=i;
                 }
              }
              break;
           case IDC_START:
              EnableWindow (hData, FALSE);
              if (hMem != NULL)
              {
                 GlobalUnfix(hMem);
                 GlobalUnlock(hMem);
                 GlobalFree(hMem);
              }
              // allocate a memory for user DMA buffer
              mem_size=data_size*4;
              hMem = GlobalAlloc(GMEM_ZEROINIT,mem_size);
              ai_buf = GlobalLock(hMem);
              if (ai_buf == NULL )
              {
                 MessageBox(hWnd,"INT", "No Memory", MB_OK);
              }
              GlobalFix(hMem);
              AI_9113_Config(card,TRIG_INT_PACER);
              AI_AsyncDblBufferMode(card, 0);
              AI_ContScanChannels (card, channel, range, ai_buf, data_size, (F64)sample_rate, SYNCH_OP);
              EnableWindow (hData, TRUE);
              for (i=0;i<8;i++) oldinput[i]=-1;
              hDC = GetDC(hWnd);
              SelectObject(hDC, hBlackBrush);
              Rectangle(hDC,output_pos[15].x+35,io_pos[0].y-3,output_pos[15].x+370,io_pos[0].y+175);

              for (i=0;i<data_size;i++) {
               adinput = ai_buf[i];
               cur_channel = (adinput>>16) & 0x1f;
               switch(cur_channel) {
                 case 0:
                    SelectObject(hDC, hYellowPen);
                    break;
                 case 1:
                    SelectObject(hDC, hLTGreenPen);
                    break;
                 case 2:
                    SelectObject(hDC, hBluePen);
                    break;
                 case 3:
                    SelectObject(hDC, hRedPen);
                    break;
                 case 4:
                    SelectObject(hDC, hLTBluePen);
                    break;
                 case 5:
                    SelectObject(hDC, hLTRedPen);
                    break;
                 case 6:
                    SelectObject(hDC, hGreenPen);
                    break;
                 case 7:
                    SelectObject(hDC, hWhitePen);
                    break;
               }
               adinput = adinput & 0xfff;
               adinput=io_pos[0].y+172-(174*adinput)/4096;
               if (oldinput[cur_channel]!=-1)
                  MoveToEx(hDC,(i*334)/data_size+output_pos[15].x+35,oldinput[cur_channel],NULL);
               else
                  MoveToEx(hDC,(i*334)/data_size+output_pos[15].x+35,adinput,NULL);
               LineTo(hDC,(i*334)/data_size+output_pos[15].x+36,adinput);
               oldinput[cur_channel]=adinput;
              } // for
              ReleaseDC(hWnd, hDC);
              break;
           default:
              return(DefWindowProc(hWnd, message, wParam, lParam));
         }
         break;
    case WM_DESTROY:
         if (card >= 0) Release_Card (card);
         DeleteObject(hRedBrush);
         DeleteObject(hGreenBrush);
         DeleteObject(hLTGreenBrush);
         DeleteObject(hYellowBrush);
         DeleteObject(hBlueBrush) ;
         DeleteObject(hBlackBrush);
         DeleteObject(hRedPen);
         DeleteObject(hYellowPen);
         DeleteObject(hBluePen);
         DeleteObject(hGreenPen);
         DeleteObject(hLTBluePen);
         DeleteObject(hLTRedPen);
         DeleteObject(hLTGreenPen);
         DeleteObject(hWhitePen);
         GlobalUnfix(hMem);
         GlobalUnlock(hMem);
         GlobalFree(hMem);
         PostQuitMessage(0);
         break;
    default:
         return(DefWindowProc(hWnd, message, wParam, lParam));
   }
  return(NULL);
}

BOOL  PASCAL ShowINT(hDlg, message, wParam, lParam)
HWND hDlg;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
    int nTabs[2] = {32, 92};
    long i;
    I16 ch_no;
    char cBuf[64];
    char string[10]="      ";
    char data_str[3]="  ";
    U32 *pp;
    switch (message)
    {
       case WM_INITDIALOG:
            hListBox = CreateWindow("LISTBOX", "Index                   Data     CH#",\
                            WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_STANDARD | LBS_USETABSTOPS |WS_CAPTION,
                            50, 40, 240, 180, hDlg, 100, hInst, NULL);

            SendMessage(hListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hListBox, LB_SETTABSTOPS, 2, (DWORD)(LPINT)nTabs);
            pp = ai_buf;
            for(i=0; i<data_size; i++) {
               ch_no = (ai_buf[i]>>16) & 0x1f;
               wsprintf(cBuf, "%5d\t                 %5d     %2d", i, ai_buf[i]&0xfff, ch_no);
               SendMessage(hListBox, LB_INSERTSTRING, -1, (DWORD)(LPSTR)cBuf);
            }
            itoa(data_size, string, 10);
            SetDlgItemText(hDlg, DMA_SPACE, string);
            ShowWindow(hListBox, SW_SHOW);
            return (TRUE);
       case WM_COMMAND:
            if (LOWORD(wParam) == ID_STOP)
            {
               DestroyWindow(hListBox);
               EndDialog(hDlg, TRUE);
               return (TRUE);
            }
       break;
    } //switch
    return (FALSE);
}
