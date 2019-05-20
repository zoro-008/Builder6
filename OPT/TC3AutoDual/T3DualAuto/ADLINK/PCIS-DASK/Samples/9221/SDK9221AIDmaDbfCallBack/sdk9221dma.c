#include <windows.h>
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
#define IDC_STOP        209
#define IDC_EDIT        211
#define IDC_EDIT1       214
#define IDC_SHOW        213
#define IDC_CH          400

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowDMA(HWND, UINT, WPARAM, LPARAM);

void AI_DBCallBack();
void AI_CallBack();

HANDLE  hInst;
HWND hListBox;
HWND hEdit,hEdit1, hStart, hStop;
BOOL bCheck[16];

HBRUSH  hBlackBrush;
HPEN    hRedPen, hBlackPen;
HWND    hMainWnd;

unsigned int sample_rate=250000;
U16 channel;
U32 data_size=4000, mem_size;
static HANDLE hMem1=NULL;
static HANDLE hMem2=NULL;
U16 *ai_buf1;
U16 *ai_buf2;
U16 bufferID1;
U16 bufferID2;
U32 SampIntv, ScanIntv;
U8 bView=0;
U32 count;

I16 card=-1, card_number =0;
BOOLEAN clear_op;
HANDLE hThread;

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
void ProcessThread(void* pArg);

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
                      (LPCSTR)"PCI-9221 Double Buffered DMA",
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
                     if ((card=Register_Card(PCI_9221, card_number)) < 0) {
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
                      output_pos[13].x-30, input_pos[15].y+20,40, 20, hWnd, IDC_EDIT, hInst, NULL);

        hEdit1=CreateWindow("EDIT", NULL,
                      WS_CHILD | WS_VISIBLE | ES_LEFT,
                      output_pos[13].x+90, input_pos[15].y+20,60, 20, hWnd, IDC_EDIT1, hInst, NULL);

        wsprintf(n,"%d",sample_rate/1000);
        SetWindowText(hEdit,n);

        wsprintf(n,"%d",data_size);
        SetWindowText(hEdit1,n);

        CreateWindow("SCROLLBAR", NULL,
                      WS_CHILD | WS_VISIBLE | SBS_VERT,
                      output_pos[13].x+10, input_pos[15].y+20,20, 20, hWnd, IDC_SCROLL, hInst, NULL);

        hStart = CreateWindow("BUTTON","Start" ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+110, input_pos[10].y+10,50, 30, hWnd, IDC_START, hInst, NULL);

        hStop = CreateWindow("BUTTON","Stop"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+170, input_pos[10].y+10,50, 30, hWnd, IDC_STOP, hInst, NULL);

        CreateWindow("BUTTON","Data Value"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+230, input_pos[10].y+10,80, 30, hWnd, IDC_SHOW, hInst, NULL);
        EnableWindow(hStop, FALSE);

        CreateWindow("BUTTON","0"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP ,
                      output_pos[4].x+50, io_pos[0].y-5+20,25,15, hWnd,IDC_CH, hInst, NULL);

        CreateWindow("BUTTON","1"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+13+20,25,15, hWnd,IDC_CH+1, hInst, NULL);

        CreateWindow("BUTTON","2"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+31+20,25,15, hWnd,IDC_CH+2, hInst, NULL);

        CreateWindow("BUTTON","3"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+49+20,25,15, hWnd,IDC_CH+3, hInst, NULL);

        CreateWindow("BUTTON","4"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+67+20,25,15, hWnd,IDC_CH+4, hInst, NULL);

        CreateWindow("BUTTON","5"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+85+20,25,15, hWnd, IDC_CH+5, hInst, NULL);

        CreateWindow("BUTTON","6"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+103+20,25,15, hWnd,IDC_CH+6, hInst, NULL);

        CreateWindow("BUTTON","7"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+50, io_pos[0].y+121+20,25,15, hWnd, IDC_CH+7, hInst, NULL);

		CreateWindow("BUTTON","8"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP ,
                      output_pos[4].x+100, io_pos[0].y-5+20,25,15, hWnd,IDC_CH+8, hInst, NULL);

        CreateWindow("BUTTON","9"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+13+20,25,15, hWnd,IDC_CH+9, hInst, NULL);

        CreateWindow("BUTTON","a"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+31+20,25,15, hWnd,IDC_CH+10, hInst, NULL);

        CreateWindow("BUTTON","b"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+49+20,25,15, hWnd,IDC_CH+11, hInst, NULL);

        CreateWindow("BUTTON","c"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+67+20,25,15, hWnd,IDC_CH+12, hInst, NULL);

        CreateWindow("BUTTON","d"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+85+20,25,15, hWnd, IDC_CH+13, hInst, NULL);

        CreateWindow("BUTTON","e"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+103+20,25,15, hWnd,IDC_CH+14, hInst, NULL);

        CreateWindow("BUTTON","f"       ,
                      WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP,
                      output_pos[4].x+100, io_pos[0].y+121+20,25,15, hWnd, IDC_CH+15, hInst, NULL);
	

        //default channel 0
        for (i=1;i<16;i++) bCheck[i]=FALSE;
        bCheck[0] = TRUE;
        channel = 0;
        SendMessage(GetDlgItem(hWnd,IDC_CH), BM_SETCHECK, 1, 0);

        hBlackBrush= CreateSolidBrush(RGB(0,0,0));
        hRedPen= CreatePen(PS_SOLID,1,RGB(255,0,0));
        hBlackPen= CreatePen(PS_SOLID,1,RGB(0,0,0));
        break;

    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        // DMA analog input box
        SelectObject(hDC, GetStockObject(GRAY_BRUSH));
        Rectangle(hDC, output_pos[15].x-10, io_pos[15].y-24, output_pos[0].x+26, io_pos[0].y+195);
        SetTextColor(hDC,RGB(255,255,0));
        SetBkColor(hDC, RGB(0, 0, 255));
        TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "DMA Input", 9);
        SelectObject(hDC, hBlackBrush);
        Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);

        SetBkMode(hDC,TRANSPARENT);
        TextOut(hDC,output_pos[13].x-60, input_pos[10].y,"Samping Rate",12);
        TextOut(hDC,output_pos[13].x+30, input_pos[15].y+20,"KHZ",3);
        TextOut(hDC,output_pos[13].x+70, input_pos[10].y,"Buffer Size",11);
        TextOut(hDC,input_pos[2].x-10, io_pos[15].y+170,"Range: +-5V",11);
        TextOut(hDC,input_pos[15].x, io_pos[15].y-19,"Volt",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y-5,"5.00",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y+36,"2.50",4);
        TextOut(hDC,input_pos[15].x, io_pos[15].y+77,"0.00",4);
        TextOut(hDC,input_pos[15].x-3, io_pos[15].y+118,"-2.50",5);
        TextOut(hDC,input_pos[15].x-3, io_pos[15].y+159,"-5.00",5);
        TextOut(hDC,input_pos[10].x+10, io_pos[10].y-19,"Signals",7);
        TextOut(hDC,output_pos[4].x+60, io_pos[0].y-15,"Channel",7);
        EndPaint(hWnd, &ps);
        break;

        // Sample Rate Spin Control
    case WM_VSCROLL:
        switch (LOWORD(wParam)) {
          case SB_LINEUP :
              sample_rate+=1000;
              if (sample_rate > 250000 ) sample_rate=250000;
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
           case IDC_CH:
           case IDC_CH+1:
           case IDC_CH+2:
           case IDC_CH+3:
           case IDC_CH+4:
           case IDC_CH+5:
           case IDC_CH+6:
           case IDC_CH+7:
		   case IDC_CH+8:
           case IDC_CH+9:
           case IDC_CH+10:
           case IDC_CH+11:
           case IDC_CH+12:
           case IDC_CH+13:
           case IDC_CH+14:
           case IDC_CH+15:	
               if (!bCheck[wParam-IDC_CH]) {
                  SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
                  SendMessage(GetDlgItem(hWnd,IDC_CH+channel), BM_SETCHECK, 0, 0);
                  bCheck[channel]=FALSE;
               }
               channel=wParam-IDC_CH;
               bCheck[channel]=TRUE;
               break;

           case IDC_SHOW :
                DialogBox(hInst, "DMABox", hWnd, ShowDMA);
                break;
           case IDC_START:
bView=0;
                if (hMem1 != NULL) {
                   GlobalUnfix(hMem1);
                   GlobalUnlock(hMem1);
                   GlobalFree(hMem1);
                }
				if (hMem2 != NULL) {
                   GlobalUnfix(hMem2);
                   GlobalUnlock(hMem2);
                   GlobalFree(hMem2);
                }

                // allocate a memory for DMA
                data_size=GetDlgItemInt(hWnd,IDC_EDIT1,NULL ,FALSE);
                mem_size=data_size * 2;
                hMem1 = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,mem_size);
				hMem2 = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,mem_size);

                ai_buf1 = (U16 *)GlobalLock(hMem1);
				ai_buf2 = (U16 *)GlobalLock(hMem2);

                if ((ai_buf1==NULL)||(ai_buf2==NULL)) {
                   MessageBox(hWnd,"DMA", "No Memory", MB_OK);
                   SendMessage(hWnd, WM_CLOSE, 0, 0L);
                   break;
                }

                GlobalFix(hMem1);
				GlobalFix(hMem2);

                if ((err = AI_9221_Config(card, P9221_AI_SingEnded|P9221_AI_IntTimeBase, P9221_TRGMOD_SOFT, 0)) != NoError) {
                      wsprintf(s, "AI_Config Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }

				SampIntv = 40000000/sample_rate;
				ScanIntv = SampIntv;
				if ((err = AI_9221_CounterInterval (card, ScanIntv, SampIntv)) != NoError) {
                      wsprintf(s, "AI_9221_CounterInterval Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }

                if ((err = AI_AsyncDblBufferMode (card, 1)) != NoError) {
                      wsprintf(s, "AI_AsyncDblBufferMode Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }
				
				if ((err = AI_ContBufferSetup (card, ai_buf1, data_size, &bufferID1)) != NoError) {
                      wsprintf(s, "AI_ContBufferSetup Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
				}
				if ((err = AI_ContBufferSetup (card, ai_buf2, data_size, &bufferID2)) != NoError) {
                      wsprintf(s, "AI_ContBufferSetup Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
				}
		
				if ((AI_EventCallBack(card, 1, 1, (U32)AI_DBCallBack)) != NoError) {
                      wsprintf(s, "AI_EventCallBack Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }

				if ((AI_EventCallBack(card, 1, 0, (U32)AI_CallBack)) != NoError) {
                      wsprintf(s, "AI_EventCallBack Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }

                if ((err=AI_ContReadChannel(card, channel, AD_B_5_V, &bufferID1, data_size, 0, ASYNCH_OP)) != NoError) {
                      wsprintf(s, "AI_ContReadPort Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }
                EnableWindow(hStart, FALSE);
                EnableWindow(hStop, TRUE);

                hMainWnd = hWnd;
                clear_op = FALSE;
        //        hThread = (HANDLE)_beginthread(ProcessThread, 0, 0);
                MessageBeep(0);
                break;
            case IDC_STOP:  
				AI_AsyncClear(card, &count);
				AI_ContBufferReset (card);
                EnableWindow(hStart, TRUE);
                EnableWindow(hStop, FALSE);
                break;
            default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_DESTROY:
		AI_ContBufferReset (card);
        if (card >= 0) Release_Card(card);
        DeleteObject(hBlackBrush);
        DeleteObject(hRedPen);
        GlobalUnfix(hMem1);
        GlobalUnlock(hMem1);
        GlobalFree(hMem1);
		GlobalUnfix(hMem2);
        GlobalUnlock(hMem2);
        GlobalFree(hMem2);
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
    U16 *pp;
    U32 pdata_size = (data_size>4000)? 4000:data_size;

    switch (message) {
        case WM_INITDIALOG:
            hListBox = CreateWindow("LISTBOX", "Index                       Data",
                           WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_STANDARD | LBS_USETABSTOPS |WS_CAPTION,
                           50, 40, 240, 180, hDlg, 100, hInst, NULL);
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hListBox, LB_SETTABSTOPS, 2, (DWORD)(LPINT) nTabs);
            SetDlgItemText(hDlg, DMA_DESCP, "This ListBox can show at most 4000 data");
            itoa( pdata_size, string, 10);
            SetDlgItemText(hDlg, DMA_SPACE, string);

			if(!bView)
				pp = (U16 *)ai_buf2;
            else
				pp = (U16 *)ai_buf1;

            for(i=0;i<pdata_size; i++) {
            //     wsprintf(cBuf, "%5d\t                %04x", i, pp[i]);
            wsprintf(cBuf, "%5d\t                %d", i, (I16)pp[i] );
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


void AI_DBCallBack()
{
  BOOLEAN HalfReady, fstop;
  HDC hDC;
  U16 adinput;
  U32 i, oldx, oldy, newx, newy;

  hDC = GetDC(hMainWnd);

  SelectObject(hDC, hBlackBrush);
  SelectObject(hDC, hBlackPen);
  Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);
  SelectObject(hDC, hRedPen);
  for (i=0; i<data_size; i++) {
	 if(!bView)
		adinput = ai_buf1[i];
	 else
		adinput = ai_buf2[i];	
	 newy = io_pos[0].y+85-( 87*((short)adinput)/32768 );
     newx = i*336/data_size+output_pos[15].x+35;
     if (i==0)
         MoveToEx(hDC, newx, newy, NULL);
     else
         MoveToEx(hDC, oldx, oldy, NULL);
     LineTo(hDC, newx, newy);
     oldx = newx;
     oldy = newy;
  }
  bView= !bView;

  ReleaseDC(hMainWnd, hDC);
}

void AI_CallBack()
{
  BOOLEAN HalfReady, fstop;
  HDC hDC;
  U16 adinput;
  U32 i, oldx, oldy, newx, newy;

  hDC = GetDC(hMainWnd);

  SelectObject(hDC, hBlackBrush);
  SelectObject(hDC, hBlackPen);
  Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);
  SelectObject(hDC, hRedPen);

  for (i=0; i<count; i++) {
	 if(!bView)
		adinput = ai_buf1[i];
	 else
		adinput = ai_buf2[i];	
	 newy = io_pos[0].y+85-( 87*((short)adinput)/32768 );
     newx = i*336/data_size+output_pos[15].x+35;
     if (i==0)
         MoveToEx(hDC, newx, newy, NULL);
     else
         MoveToEx(hDC, oldx, oldy, NULL);
     LineTo(hDC, newx, newy);
     oldx = newx;
     oldy = newy;
  }
  bView= !bView;

  ReleaseDC(hMainWnd, hDC);
}
