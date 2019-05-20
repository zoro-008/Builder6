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
#define IDC_EDIT3       216
#define IDC_SHOW        213

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowDMA(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hListBox;
HWND hEdit,hEdit1, hEdit2, hEdit3, hStart;
BOOL bCheck_mCtr=FALSE;

HBRUSH  hBlackBrush;
HPEN    hRedPen, hBlackPen, hBluePen, hYellowPen, hGreenPen, hLTGreenPen;
HWND    hMainWnd;

long  samp_intrv=240;
U16   channel=0, range = AD_B_5_V;
U32 data_size=2000, mem_size, mCount = 100;
static HANDLE hMem=NULL;
BOOLEAN fstop =0;
I16  *ai_buf;
I16 card=-1, card_number = 0, ai_mode;
U32 count, postCount= 1024;
HANDLE hThread1;
LPSTR AI_MODE[2]={"Middle Trigger", "Pre Trigger"};

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
                      (LPCSTR)"PCI-9116 Single Buffered DMA with Middle Trigger Mode",
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
                case IDC_RADIO1:
					 if (!bCheck_mCtr) {
						SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
						bCheck_mCtr=TRUE;
					 } else {
						SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
						bCheck_mCtr=FALSE;
					 }
                     break;
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
                      output_pos[13].x-60, input_pos[15].y-20,70, 20, hWnd, IDC_EDIT, hInst, NULL);

        hEdit3=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x-60, input_pos[15].y+20,60, 20, hWnd, IDC_EDIT3, hInst, NULL);

        hEdit1=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x+90, input_pos[15].y+20,60, 20, hWnd, IDC_EDIT1, hInst, NULL);

        hEdit2=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x+90, input_pos[15].y-20,60, 20, hWnd, IDC_EDIT2, hInst, NULL);

		if(ai_mode) {
			EnableWindow(hEdit2, FALSE);
			postCount = 0;
		}
        wsprintf(n,"%d",samp_intrv);
        SetWindowText(hEdit,n);

        wsprintf(n,"%d",data_size);
        SetWindowText(hEdit1,n);

        wsprintf(n,"%d",postCount);
        SetWindowText(hEdit2,n);

		if(!bCheck_mCtr) {
			EnableWindow(hEdit3, FALSE);
			mCount = 0;
		}
        wsprintf(n,"%d",mCount);
        SetWindowText(hEdit3,n);

        CreateWindow("SCROLLBAR", NULL,
                      WS_CHILD | WS_VISIBLE | SBS_VERT,
                      output_pos[13].x+10, input_pos[15].y-20,20, 20, hWnd, IDC_SCROLL, hInst, NULL);

        hStart = CreateWindow("BUTTON","Start" ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+130, input_pos[10].y+10,50, 30, hWnd, IDC_START, hInst, NULL);

        CreateWindow("BUTTON","Data Value"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+210, input_pos[10].y+10,80, 30, hWnd, IDC_SHOW, hInst, NULL);

        channel = 0;

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
//        TextOut(hDC, io_pos[15].x+105, io_pos[15].y-40, " Trigger Source : External Digital Trigger ", 43);
        SetTextColor(hDC,RGB(255,255,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "DMA / INT Input", 15);
        SelectObject(hDC, hBlackBrush);
        Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);

        SetBkMode(hDC,TRANSPARENT);
        TextOut(hDC,output_pos[13].x+70, input_pos[10].y-40,"PostCount",9);
        TextOut(hDC,output_pos[13].x-60, input_pos[10].y-40,"Sample Interval",15);
        TextOut(hDC,output_pos[13].x-60, input_pos[10].y,"M Counter",9);
        //TextOut(hDC,output_pos[13].x+30, input_pos[15].y+20,"KHZ",3);
        TextOut(hDC,output_pos[13].x+70, input_pos[10].y,"Buffer Size",11);
        TextOut(hDC,input_pos[2].x-10, io_pos[15].y+118,"Range: +-5V",11);
        TextOut(hDC,input_pos[15].x, io_pos[15].y-19,"Volt",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y-5,"5.00",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y+36,"2.50",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y+77,"0.00",4);
        TextOut(hDC,input_pos[15].x-3, io_pos[15].y+118,"-2.50",5);
        TextOut(hDC,input_pos[15].x-3, io_pos[15].y+159,"-5.00",5);
        TextOut(hDC,input_pos[10].x+10, io_pos[10].y-19,"Signals",7);
        TextOut(hDC,output_pos[4].x+60, io_pos[0].y,"Channel 0",9);
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
                data_size=GetDlgItemInt(hWnd,IDC_EDIT1,NULL ,FALSE);
                mCount=GetDlgItemInt(hWnd,IDC_EDIT3,NULL ,FALSE);
				if(bCheck_mCtr) 
					data_size += mCount;
                mem_size=data_size * 2;
                hMem = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,mem_size);
                ai_buf = (I16 *)GlobalLock(hMem);
                if (ai_buf == NULL ) {
                   MessageBox(hWnd,"DMA", "No Memory", MB_OK);
                   SendMessage(hWnd, WM_CLOSE, 0, 0L);
                   break;
                }
                GlobalFix(hMem);

                postCount = GetDlgItemInt(hWnd,IDC_EDIT2,NULL ,FALSE);
				if(!bCheck_mCtr)
					err = AI_9116_Config(card,0,P9116_AI_DMA|(ai_mode?P9116_TRGMOD_PRE:P9116_TRGMOD_MIDL), (ai_mode?0:postCount),0,0);
				else 
					err = AI_9116_Config(card,0,P9116_AI_DMA|(ai_mode?P9116_TRGMOD_PRE:P9116_TRGMOD_MIDL)|P9116_AI_MCounterEn, (ai_mode?0:postCount),mCount,0);
				err = AI_9116_CounterInterval (card, samp_intrv, samp_intrv);
                err = AI_ContReadChannel (card, channel, range, ai_buf, data_size, (F32)1, ASYNCH_OP);
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
    I16 *pp;

    switch (message) {
        case WM_INITDIALOG:
            hListBox = CreateWindow("LISTBOX", "Index               Data          CH#",
                           WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_STANDARD | LBS_USETABSTOPS |WS_CAPTION,
                           50, 40, 240, 180, hDlg, 100, hInst, NULL);
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hListBox, LB_SETTABSTOPS, 2, (DWORD)(LPINT) nTabs);
            SetDlgItemText(hDlg, DMA_DESCP, "This ListBox can show at most 4000 data");
            itoa( /*data_size*/count, string, 10);
            SetDlgItemText(hDlg, DMA_SPACE, string);
            pp = (I16 *)ai_buf;
            //if (count >4000) count = 4000;
            for(i=0;i<count; i++) {
                 wsprintf(cBuf, "%5d\t      %5d", i, pp[i]);
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
  I16 cur_channel;
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
      cur_channel = /*i%ch_cnt*/0;
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
     //adinput = ai_buf[i]>>4;
     newy=io_pos[0].y+87-(87*(I16)ai_buf[i])/32767;
     //newy = io_pos[0].y+86-(87*(long)adinput)/2048;
     newx = i*336/data_size+output_pos[15].x+35;
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