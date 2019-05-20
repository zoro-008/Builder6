#include "windows.h"
#include <stdio.h>
#include <process.h>
#include "dask.h"
#include "resource.h"

#define IDM_EXIT        110
#define IDC_START       208
#define IDC_STOP        209
#define IDC_FUNC        212

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowINT(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hStop, hStart;
I16 card=-1, card_number=0;
HANDLE hEvent[2], hThread1;
U32 event_count=0;
U32 new_value, old_value=65535;
char txtMsg[128];

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
static U16 hua;
void ProcessInterrupt()
{
  U16     mode;
  do {
        WaitForSingleObject(hEvent[0], INFINITE);
        CTR_Read (card, 1, &new_value);
        event_count = old_value-new_value;
        old_value = new_value;
        ResetEvent(hEvent[0]);
  } while (1);
  _endthread();
}

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
                      (LPCSTR)"PCI-8554 Event Count",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      490,
                      370,
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
                     if ((card=Register_Card(PCI_8554, card_number)) < 0) {
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

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
		if(card<0) {
		  SendMessage(hWnd, WM_CLOSE, 0, 0L);
		  break;
		}
        hStart = CreateWindow("BUTTON","Display event count" ,
                      WS_CHILD | WS_VISIBLE ,
                      160, 280, 160, 24, hWnd, IDC_START, hInst, NULL);
        DIO_SetDualInterrupt (card, INT1_COUT12, INT2_DISABLE, hEvent);
        hThread1 = (HANDLE)_beginthread(ProcessInterrupt, 0, 0);
        CTR_8554_ClkSrc_Config (card, 1, ECKN);
        CTR_Setup (card, 11, 3, 4000, BIN);
        CTR_Setup (card, 12, 2, 2000, BIN);
        CTR_Setup (card, 1, 2, 65535, BIN);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        SelectObject(hDC, GetStockObject(GRAY_BRUSH));
        Rectangle(hDC, 20, 80, 460, 250);
        SetTextColor(hDC,RGB(255,255,0));
        SetBkMode(hDC,TRANSPARENT);
        TextOut(hDC, 30, 20, "Connect measured signal to the clock source of the counter #1.", 62);
        TextOut(hDC, 30, 36, "It will measure event count every second.", 41);
        TextOut(hDC, 30, 100, txtMsg, strlen(txtMsg));
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_START:
                sprintf(txtMsg, "Event count value is %d", event_count);
                InvalidateRect(hWnd, NULL, 0);
                break;
            default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_DESTROY:
        DIO_SetDualInterrupt (card, -1, -1, hEvent);
        if (card>=0) Release_Card(card);
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}
