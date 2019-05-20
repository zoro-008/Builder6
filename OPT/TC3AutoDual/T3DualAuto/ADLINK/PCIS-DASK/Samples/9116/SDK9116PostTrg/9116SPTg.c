#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <process.h>
#include "resource.h"
#include "Dask.h"

#define IDM_EXIT        110
#define IDC_SCROLL      207
#define IDC_SPIN        301
#define IDC_START       208
#define IDC_EDIT        211
#define IDC_EDIT1       214
#define IDC_EDIT2       215
#define IDC_SHOW        213
#define IDC_CH          400

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowDMA(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hListBox;
HWND hEdit,hEdit1, hEdit2, hStart;
BOOL bCheck[5];

HBRUSH  hBlackBrush;
HPEN    hRedPen, hBlackPen, hBluePen, hYellowPen, hGreenPen, hLTGreenPen;
HWND    hMainWnd;

long  samp_intrv=240;
U16   channel=0, range = AD_B_5_V, ch_cnt=1;
U32 data_size=2048, mem_size;
static HANDLE hMem=NULL;
BOOLEAN fstop =0;
I16  *ai_buf;
I16 card=-1, card_number = 0, ai_mode;
U32 count, trgCount = 1;
HANDLE hThread1;
LPSTR AI_MODE[2]={"DMA", "INT"};

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

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
void ProcessThread();

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
                      (LPCSTR)"PCI-9116 Single Buffered DMA with Post Trigger Mode",
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
              for (i=0;i<2;i++) {
                SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_ADDSTRING, NULL, (LONG)(LPSTR) AI_MODE[i]);
              }
              SendMessage(GetDlgItem(hWnd,IDC_COMBO1) , CB_SETCURSEL, 0, 0L);
              for (i=0;i<16;i++) {
                wsprintf(n, "%d", i);
                _strupr(n);
                SendMessage(GetDlgItem(hWnd,IDC_COMBO2), CB_ADDSTRING, NULL, (LONG)(LPSTR) n);
              }
              SendMessage(GetDlgItem(hWnd,IDC_COMBO2) , CB_SETCURSEL, 0, 0L);
              break;
         case WM_COMMAND:
              switch(LOWORD(wParam)) {

                case IDC_COMBO1:
                     if (HIWORD(wParam) == CBN_CLOSEUP) {
                        i = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_GETCURSEL, 0, 0L);
                        SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_GETLBTEXT, i, (LPARAM)((LPCSTR)n));
                        ai_mode = i;
                     }
                     return(TRUE);
                case IDC_COMBO2:
                     if (HIWORD(wParam) == CBN_CLOSEUP) {
                        i = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO2), CB_GETCURSEL, 0, 0L);
                        SendMessage(GetDlgItem(hWnd,IDC_COMBO2), CB_GETLBTEXT, i, (LPARAM)((LPCSTR)n));
                        card_number = i;
                     }
                     return(TRUE);
                case IDOK:
                      if ((card=Register_Card(PCI_9116, card_number)) < 0) {
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
  PAINTSTRUCT ps;
  int i;
  char n[6];
  static   int pos=0;
  I16 err;
  char s[50];

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
   	    if(card<0) {
	    	 SendMessage(hWnd, WM_CLOSE, 0, 0L);
		     break;
        }
        hEdit=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x-60, input_pos[15].y+20,70, 20, hWnd, IDC_EDIT, hInst, NULL);

        hEdit1=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x+90, input_pos[15].y+20,60, 20, hWnd, IDC_EDIT1, hInst, NULL);

        hEdit2=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x+90, input_pos[15].y-20,60, 20, hWnd, IDC_EDIT2, hInst, NULL);

        wsprintf(n,"%d",samp_intrv);
        SetWindowText(hEdit,n);

        wsprintf(n,"%d",data_size);
        SetWindowText(hEdit1,n);

        wsprintf(n,"%d",trgCount);
        SetWindowText(hEdit2,n);

        CreateWindow("SCROLLBAR", NULL,
                      WS_CHILD | WS_VISIBLE | SBS_VERT,
                      output_pos[13].x+10, input_pos[15].y+20,20, 20, hWnd, IDC_SCROLL, hInst, NULL);

        hStart = CreateWindow("BUTTON","Start" ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+130, input_pos[10].y+10,50, 30, hWnd, IDC_START, hInst, NULL);

        CreateWindow("BUTTON","Data Value"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+210, input_pos[10].y+10,80, 30, hWnd, IDC_SHOW, hInst, NULL);

        CreateWindow("BUTTON","1"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP ,
                      output_pos[4].x+60, io_pos[0].y-5+20,25,15, hWnd,IDC_CH, hInst, NULL);
        CreateWindow("BUTTON","2"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+60, io_pos[0].y+13+20,25,15, hWnd,IDC_CH+1, hInst, NULL);
        CreateWindow("BUTTON","3"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+60, io_pos[0].y+31+20,25,15, hWnd,IDC_CH+2, hInst, NULL);
        CreateWindow("BUTTON","4"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+60, io_pos[0].y+49+20,25,15, hWnd,IDC_CH+3, hInst, NULL);

        //default channel 0
        for (i=1;i<4;i++) bCheck[i]=FALSE;
        bCheck[0] = TRUE;
        channel = 0;
        SendMessage(GetDlgItem(hWnd,IDC_CH), BM_SETCHECK, 1, 0);

        hBlackBrush= CreateSolidBrush(RGB(0,0,0));
        hRedPen= CreatePen(PS_SOLID,1,RGB(255,0,0));
        hBluePen= CreatePen(PS_SOLID,1,RGB(0,0,255));
        hGreenPen= CreatePen(PS_SOLID,1,RGB(0,255,0));
        hLTGreenPen= CreatePen(PS_SOLID,1,RGB(0,128,0));
        hYellowPen= CreatePen(PS_SOLID,1,RGB(255,255,0));
        hBlackPen= CreatePen(PS_SOLID,1,RGB(0,0,0));
        break;

    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        // DMA analog input box
        SelectObject(hDC, GetStockObject(GRAY_BRUSH));
        Rectangle(hDC, output_pos[15].x-10, io_pos[15].y-24, output_pos[0].x+26, io_pos[0].y+195);
        SetTextColor(hDC,RGB(255,0,0));
        //TextOut(hDC, io_pos[15].x+105, io_pos[15].y-40, " Trigger Source : External Digital Trigger ", 43);
        SetTextColor(hDC,RGB(255,255,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "DMA /INT Input", 15);
        SelectObject(hDC, hBlackBrush);
        Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);
        SetBkMode(hDC,TRANSPARENT);
        TextOut(hDC,output_pos[13].x+70, input_pos[10].y-40,"RetrigCount",11);
        TextOut(hDC,output_pos[13].x-60, input_pos[10].y,"Samping Interval",15);
        TextOut(hDC,output_pos[13].x+70, input_pos[10].y,"Scan Count",10);
        TextOut(hDC,input_pos[2].x-10, io_pos[15].y+118,"Range: +-5V",11);
        TextOut(hDC,input_pos[15].x, io_pos[15].y-19,"Volt",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y-5,"5.00",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y+36,"2.50",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y+77,"0.00",4);
        TextOut(hDC,input_pos[15].x-3, io_pos[15].y+118,"-2.50",5);
        TextOut(hDC,input_pos[15].x-3, io_pos[15].y+159,"-5.00",5);
        TextOut(hDC,input_pos[10].x+10, io_pos[10].y-19,"Signals",7);
        TextOut(hDC,output_pos[4].x+60, io_pos[0].y-15,"No. of CHs",10);
        SetTextColor(hDC,RGB(255,255,0));
        TextOut(hDC,output_pos[4].x+88, io_pos[0].y+15,"Ch0",3);
        SetTextColor(hDC,RGB(0,128,0));
        TextOut(hDC,output_pos[4].x+88, io_pos[0].y+33,"Ch0-ch1",7);
        SetTextColor(hDC,RGB(0,0,255));
        TextOut(hDC,output_pos[4].x+88, io_pos[0].y+51,"Ch0-Ch2",7);
        SetTextColor(hDC,RGB(255,0,0));
        TextOut(hDC,output_pos[4].x+88, io_pos[0].y+69,"Ch0-Ch3",7);
        EndPaint(hWnd, &ps);
        break;
        // Sample Rate Spin Control
    case WM_VSCROLL:
        switch(LOWORD(wParam)){
           case SB_LINEUP :
                samp_intrv+=240;
                if (samp_intrv > 24000 ) samp_intrv=24000;
                break;
           case SB_LINEDOWN:
                samp_intrv-=240;
                if (samp_intrv < 240 ) samp_intrv=240;
                break;
           }
           wsprintf(n,"%d",samp_intrv);
           SetWindowText(hEdit,n);
        break;
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_CH:
           case IDC_CH+1:
           case IDC_CH+2:
           case IDC_CH+3:
               if (!bCheck[wParam-IDC_CH]) {
                  SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
                  SendMessage(GetDlgItem(hWnd,IDC_CH+channel), BM_SETCHECK, 0, 0);
                  bCheck[channel]=FALSE;
               }
               channel=wParam-IDC_CH;
			   ch_cnt = channel + 1;
               bCheck[channel]=TRUE;
               break;
           case IDC_SHOW :
                DialogBox(hInst, "DMABox", hWnd, ShowDMA);
                break;
           case IDC_START:
                if (hMem != NULL) {
                   GlobalUnfix(hMem);
                   GlobalUnlock(hMem);
                   GlobalFree(hMem);
                }
                // allocate a memory for DMA
                trgCount = GetDlgItemInt(hWnd,IDC_EDIT2,NULL ,FALSE);
				if(trgCount<1) { 
                   MessageBox(hWnd,"trgCount must be >= 1", "No Memory", MB_OK);
                   break;
                }
                data_size=GetDlgItemInt(hWnd,IDC_EDIT1,NULL ,FALSE);
   			    data_size *= ch_cnt;
                mem_size=data_size * 2 * trgCount;
                hMem = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,mem_size);
                ai_buf = (I16 *)GlobalLock(hMem);
                if (ai_buf == NULL ) {
                   MessageBox(hWnd,"DMA/INT", "No Memory", MB_OK);
                   SendMessage(hWnd, WM_CLOSE, 0, 0L);
                   break;
                }
                GlobalFix(hMem);

				if(!ai_mode)				  
					AI_9116_Config(card,0,P9116_AI_DMA|P9116_TRGMOD_POST|P9116_AI_ReTrigEn,0,0,trgCount);
				else 
					AI_9116_Config(card,0,P9116_AI_INT|P9116_TRGMOD_POST|P9116_AI_ReTrigEn,0,0,trgCount);

				AI_9116_CounterInterval (card, samp_intrv*ch_cnt, samp_intrv);
                err = AI_ContScanChannels (card, channel, range, ai_buf, trgCount*(data_size/ch_cnt), (F32)1, ASYNCH_OP);
				if(err !=0) {
				   wsprintf(s,"AI_ContScanChannels error: %d",err);
                   MessageBox(hWnd,s, "No Memory", MB_OK);
                   SendMessage(hWnd, WM_CLOSE, 0, 0L);
                   break;
                }
                EnableWindow(hStart, FALSE);
                hMainWnd = hWnd;
                fstop = 0;
                while (!fstop)
                       AI_AsyncCheck(card, &fstop, &count);
                ProcessThread();
                MessageBeep(0);
                break;
            default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_DESTROY:
        if (card >= 0) Release_Card(card);
        DeleteObject(hBlackBrush);
        DeleteObject(hRedPen);
        DeleteObject(hBluePen);
        DeleteObject(hLTGreenPen);
        DeleteObject(hYellowPen);
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

BOOL  PASCAL ShowDMA(hDlg, message, wParam, lParam)
HWND hDlg;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
    int nTabs[2] = {32, 92};
    int i;
    char cBuf[100];
    char string[10]="      ";
    char data_str[3]="  ";
    I16 *pp, ch_no;

    switch (message) {
        case WM_INITDIALOG:
            hListBox = CreateWindow("LISTBOX", "Index               Data",
                           WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_STANDARD | LBS_USETABSTOPS |WS_CAPTION,
                           50, 40, 240, 180, hDlg, 100, hInst, NULL);
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hListBox, LB_SETTABSTOPS, 2, (DWORD)(LPINT) nTabs);
            SetDlgItemText(hDlg, DMA_DESCP, "This ListBox can show at most 4000 data");
            itoa( data_size, string, 10);
            SetDlgItemText(hDlg, DMA_SPACE, string);
            pp = (I16 *)ai_buf;
            if (count >4000) count = 4000;
            for(i=0;i<count; i++) {
     		   ch_no = i%(channel+1);
               wsprintf(cBuf, "%5d\t      %5d          %2d", i, pp[i], ch_no);
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

void ProcessThread()
{
  HDC hDC;
  I16 adinput, cur_channel;
  U32 i, oldx[4], oldy[4], newx, newy;

  hDC = GetDC(hMainWnd);
  for (i=0; i<4; i++) {
           oldx[i] = 0;
           oldy[i] = 0;
   }

  EnableWindow(hStart, TRUE);
  AI_AsyncClear(card, &count);
  SelectObject(hDC, hBlackBrush);
  SelectObject(hDC, hBlackPen);
  Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);
  for (i=0; i<count; i++) {
      cur_channel = i%ch_cnt;
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
     }
     newy=io_pos[0].y+87-(87*(I16)ai_buf[i])/32767;
     newx = i*336/count+output_pos[15].x+35;
     if (! oldx[cur_channel])
          MoveToEx(hDC, newx, newy, NULL);
     else
          MoveToEx(hDC, oldx[cur_channel], oldy[cur_channel], NULL);
     LineTo(hDC, newx, newy);
     oldx[cur_channel] = newx;
     oldy[cur_channel]= newy;
  }
  ReleaseDC(hMainWnd, hDC);
}