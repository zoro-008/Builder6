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
#define IDC_START       208
#define IDC_STOP        209
#define IDC_EDIT        211
#define IDC_EDIT1       214
#define IDC_SHOW        213
#define IDC_DA          400

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowDMA(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hListBox;
HWND hEdit,hEdit1, hStart, hStop;

HBRUSH  hBlackBrush;
HPEN    hRedPen;
I16 card=-1, card_number = 0;
char max_str[12]={0}, min_str[12]={0};
U32 sample_rate=100000;
U32 data_size=1000, mem_size;
static HANDLE hMem=NULL;
U32 *pMem;
HANDLE hThread;

#ifdef SELF_OUTPUT
ULONG DoBuf[10240];
BOOLEAN bOutStopped;
ULONG out_count;
#endif

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
void CheckThread(void* pArg);

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
                      (LPCSTR)"PCI-7200 DMA",
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
                     if ((card=Register_Card(PCI_7200, card_number)) < 0) {
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
  char n[6];
  U32 min_value, max_value;
  I16 err;
  U32 adinput;
  U32 count;
  int oldx, oldy, newx, newy;
  char s[50];

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG1, hWnd, setup);
		if(card<0) {
		    SendMessage(hWnd, WM_CLOSE, 0, 0L);
			break;
		}
#ifdef SELF_OUTPUT
        for (i=0; i<10240; i++)
            DoBuf[i] = (U32)(sin((double)i/128)*1024)+1024;
#endif
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
        TextOut(hDC,output_pos[13].x-60, input_pos[10].y,"Samping Rate",12);
        TextOut(hDC,output_pos[13].x+30, input_pos[15].y+20,"KHZ",3);
        TextOut(hDC,output_pos[13].x+70, input_pos[10].y,"Xfer Count",10);
        EndPaint(hWnd, &ps);
        break;

    // Sample Rate Spin Control
    case WM_VSCROLL:
        switch (LOWORD(wParam)) {
          case SB_LINEUP :
              sample_rate+=50000;
              if (sample_rate > 1000000 ) sample_rate=1000000;
              break;
          case SB_LINEDOWN:
              sample_rate-=50000;
              if (sample_rate < 50000 ) sample_rate=50000;
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
                mem_size=data_size * 4;
                hMem = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,mem_size);

                pMem = (U32 *)GlobalLock(hMem);
                if (pMem == NULL )
                  MessageBox(hWnd,"DMA", "No Memory", MB_OK);

                GlobalFix(hMem);

#ifdef SELF_OUTPUT
                DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW);
                //DO_AsyncDblBufferMode (card, 0);
#endif

                if ((err=DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING)) != NoError) {
                      wsprintf(s, "DI_Config Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }
                if ((err=DI_AsyncDblBufferMode (card, 0)) != NoError) {
                      wsprintf(s, "DI_AsyncDblBufferMode Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }
                EnableWindow(hStart, FALSE);
                EnableWindow(hStop, TRUE);

#ifdef SELF_OUTPUT
                DO_ContWritePort(card, 0, DoBuf, 10240, 0, (F64)sample_rate, ASYNCH_OP);
#endif

                if ((err=DI_ContReadPort(card, 0, pMem, data_size, (F64)sample_rate, ASYNCH_OP)) != NoError) {
                      wsprintf(s, "DI_ContReadPort Error = %d\n", err);
                      MessageBox(hWnd, s, "Error", MB_OK);
                      SendMessage(hWnd, WM_CLOSE, 0, 0L);
                      break;
                }

                hThread = (HANDLE)_beginthread(CheckThread, 0, 0);

                WaitForSingleObject(hThread, INFINITE);

                EnableWindow(hStart, TRUE);
                EnableWindow(hStop, FALSE);
                MessageBeep(0);

                hDC = GetDC(hWnd);
                SelectObject(hDC, hBlackBrush);
                Rectangle(hDC,output_pos[15].x+40,io_pos[0].y-3,output_pos[15].x+456,io_pos[0].y+175);
                SelectObject(hDC, hRedPen);
                SetBkMode(hDC,TRANSPARENT);
                SetROP2(hDC, R2_XORPEN);
                min_value = max_value = pMem[0];
                for (i=1; i<data_size; i++) {
                        if (pMem[i] < min_value) min_value = pMem[i];
                        if (pMem[i] > max_value) max_value = pMem[i];
                }
                TextOut(hDC,input_pos[15].x-10, io_pos[15].y-5,max_str,strlen(max_str));
                TextOut(hDC,input_pos[15].x-10, io_pos[15].y+159,min_str,strlen(min_str));
                wsprintf(max_str, "0x%x", max_value);
                wsprintf(min_str, "0x%x", min_value);
                TextOut(hDC,input_pos[15].x-10, io_pos[15].y-5,max_str,strlen(max_str));
                TextOut(hDC,input_pos[15].x-10, io_pos[15].y+159,min_str,strlen(min_str));
                for (i=0; i<data_size; i++) {
                   adinput = pMem[i];
                   newy = io_pos[0].y+174-(178*(adinput-min_value))/(max_value-min_value+1);
                   newx = i*416/data_size+output_pos[15].x+40;
                   if (i==0)
                      MoveToEx(hDC, newx, newy, NULL);
                   else
                      MoveToEx(hDC, oldx, oldy, NULL);
                   LineTo(hDC, newx, newy);
                   oldx = newx;
                   oldy = newy;
                }
                ReleaseDC(hWnd, hDC);
                break;
            case IDC_STOP:
                DI_AsyncClear(card, &count);
#ifdef SELF_OUTPUT
                DO_AsyncClear(card, &count);
#endif
                break;
            default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_DESTROY:
        if (card >= 0) Release_Card(card);
        DeleteObject(hBlackBrush);
        DeleteObject(hRedPen);
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
    long i;
    char cBuf[100];
    char string[10]="      ";
    char data_str[3]="  ";
    U32 *pp;
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

            pp = (U32 *)pMem;
            if (data_size > 4000) data_size = 4000;
            for(i=0;i<data_size; i++) {
                 wsprintf(cBuf, "%5d\t                %08x", i, pp[i]);
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

void CheckThread(void* pArg)
{
  BOOLEAN bStopped;
  U32 count;
  do {
       DI_AsyncCheck(card, &bStopped, &count);
  } while (!bStopped);
  DI_AsyncClear(card, &count);

#ifdef SELF_OUTPUT
  DO_AsyncClear(card, &out_count);
#endif
  _endthread();
}
