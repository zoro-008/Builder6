#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
#include "dask.h"

#define IDM_EXIT        110
#define IDC_START       208
#define IDC_FUNC        212

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL  PASCAL ShowINT(HWND, UINT, WPARAM, LPARAM);
void wait_pulse_start();

HANDLE  hInst;
HWND hFuncComboBox;
HWND hStart;
int func=0;
char txtMsg[128];
time_t *time1,*time2;
I16 card=-1, card_number=0;

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
                      (LPCSTR)"PCI-8554",
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
                     if ((card=Register_Card(PCI_8554, card_number)) < 0) {
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
  U32 value;
  U16 under_flow;
  U16 old_value, new_value;
  float pulse_width, period, frequency;

  switch(message) {
    case WM_CREATE:
        DialogBox(hInst,IDD_DIALOG1, hWnd, setup);
		if(card<0) {
		  SendMessage(hWnd, WM_CLOSE, 0, 0L);
		  break;
		}
        hFuncComboBox = CreateWindow("ComboBox", NULL,
                          CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
                          150, 20, 180, 120, hWnd, IDC_FUNC, hInst, NULL);
        SendMessage(hFuncComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"Square Wave");
        SendMessage(hFuncComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"Pluse Generator");
        SendMessage(hFuncComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"Measure Pulse Width");
        SendMessage(hFuncComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"Measure Frequency");
        SendMessage(hFuncComboBox ,CB_SETCURSEL, 0, 0L);
        hStart = CreateWindow("BUTTON","Start" ,
                      WS_CHILD | WS_VISIBLE ,
                      220, 280, 60, 24, hWnd, IDC_START, hInst, NULL);
        time1 = malloc(sizeof(time_t));
        time2 = malloc(sizeof(time_t));
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        SelectObject(hDC, GetStockObject(GRAY_BRUSH));
        //SetBkMode(hDC,OPAQUE);
        Rectangle(hDC, 20, 60, 460, 250);
        SetTextColor(hDC,RGB(255,255,0));
        SetBkMode(hDC,TRANSPARENT);
        switch (func) {
            case 0:  //Square Wave
              TextOut(hDC, 30, 70, "Generate 250 KHz square wave.", 29);
              TextOut(hDC, 30, 86, "The clock source of the counter #1 is set to C8M.", 49);
              break;
            case 1:  //Pluse Generator
              TextOut(hDC, 30, 70, "Generate one pulse every hour.", 30);
              TextOut(hDC, 30, 86, "<1> the clock source of the counter #1 is set to C8M", 52);
              TextOut(hDC, 30, 102, "<2> the clock source of counter #2, #3 is from #1, #2", 53);
              break;
            case 2:  //Measure Pulse Width
              TextOut(hDC, 30, 70, "Measure pulse width.", 20);
              TextOut(hDC, 30, 86, "<1> the clock source of the counter #1 is set to C8M", 52);
              TextOut(hDC, 30, 102, "<2> the signal to be measured is connected to GATE1", 51);
              break;
            case 3:  //Measure Frequency
              TextOut(hDC, 30, 70, "Measure signal frequency.", 25);
              TextOut(hDC, 30, 86, "<1> Connect the signal to be measured to ECLK1", 46);
              TextOut(hDC, 30, 102, "<2> the clock source of the counter #2,#3 set to C8M, COUT2", 59);
              TextOut(hDC, 30, 118, "<3> the gate source of the counter #1 set to the inverse of COUT3", 65);
              TextOut(hDC, 30, 134, "<4> the gate source of the counter #2, #3 set to High", 53);
              break;
        }
        TextOut(hDC, 30, 150, txtMsg, strlen(txtMsg));

        EndPaint(hWnd, &ps);
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;
           case IDC_START:
                EnableWindow(hStart, FALSE);
                switch (func) {
                  case 0:  //Square Wave
                         CTR_8554_CK1_Config(card, CK1_C8M);
                         CTR_8554_ClkSrc_Config(card, 1, CK1);
                         CTR_8554_ClkSrc_Config(card, 2, CK1);
                         CTR_8554_ClkSrc_Config(card, 3, CK1);
                         CTR_8554_ClkSrc_Config(card, 4, CK1);
                         CTR_8554_ClkSrc_Config(card, 5, CK1);
                         CTR_8554_ClkSrc_Config(card, 6, CK1);
                         CTR_8554_ClkSrc_Config(card, 7, CK1);
                         CTR_8554_ClkSrc_Config(card, 8, CK1);
                         CTR_8554_ClkSrc_Config(card, 9, CK1);
                         CTR_8554_ClkSrc_Config(card, 10, CK1);
                         CTR_Setup(card, 1, 3, 32, BIN);
                         CTR_Setup(card, 2, 3, 32, BIN);
                         CTR_Setup(card, 3, 3, 32, BIN);
                         CTR_Setup(card, 4, 3, 32, BIN);
                         CTR_Setup(card, 5, 3, 32, BIN);
                         CTR_Setup(card, 6, 3, 32, BIN);
                         CTR_Setup(card, 7, 3, 32, BIN);
                         CTR_Setup(card, 8, 3, 32, BIN);
                         CTR_Setup(card, 9, 3, 32, BIN);
                         CTR_Setup(card, 10, 3, 32, BIN);
                         strcpy(txtMsg, "*** 250 KHz square wave is ready on the COUT1 ~ 11 ***");
                         break;
                  case 1:  //Pluse Generator
                         CTR_8554_CK1_Config(card, CK1_C8M);
                         CTR_8554_ClkSrc_Config(card, 1, CK1);
                         CTR_8554_ClkSrc_Config(card, 2, COUTN_1);
                         CTR_8554_ClkSrc_Config(card, 3, COUTN_1);
                         CTR_Setup(card, 1, 3, 200, 0);
                         CTR_Setup(card, 2, 3, 200, 0);
                         CTR_Setup(card, 3, 3, 200, 0);
                         strcpy(txtMsg, " *** A pulse per second is ready on the COUT3 ***");
                         break;
                  case 2:  //Measure Pulse Width
                         CTR_8554_CK1_Config(card, CK1_C8M);
                         CTR_8554_ClkSrc_Config(card, 1, CK1);
                         CTR_Setup(card, 1, 2, 0, BIN);
                         under_flow = 0;
                         wait_pulse_start();
                         time(time1);
                         CTR_Read(card, 1, &old_value);
                         while (1) {
                             CTR_Read(card, 1, &new_value);
                             time(time2);
                             period = difftime(*time2,*time1);
                             if ((new_value == old_value)||(period > 5)) break;
                             if ( old_value < new_value ) under_flow++;
                             old_value=new_value;
                         }
                         if (period < 5) {
                           pulse_width = (65536 - new_value + under_flow * 65536 + 1) * 0.125;
                           sprintf(txtMsg, "*** High level pulse width = %f micro-second ***", pulse_width);
                         }
                         else
                           strcpy(txtMsg, "*** Measure inappropriate pulse width ***");
                         break;
                  case 3:  //Measure Frequency
                         CTR_8554_ClkSrc_Config(card, 1, ECKN);
                         CTR_8554_CK1_Config(card, CK1_C8M);
                         CTR_8554_ClkSrc_Config(card, 2, CK1);
                         CTR_8554_ClkSrc_Config(card, 3, COUTN_1);
                         CTR_Setup(card, 1, 2, 65535, 0);
                         CTR_Setup(card, 2, 2, 4, 0);
                         CTR_Setup(card, 3, 0, 65535, 0);
                         Sleep(1000);
                         CTR_Read(card, 1, &value);
                         period = 65535 - (value&0xffff);
                         frequency = period * 2000000 / 65535;
                         sprintf(txtMsg, "Measured Signal Frequency = %f Hz.",frequency);
                         break;
                }
                EnableWindow(hStart, TRUE);
                InvalidateRect(hWnd, NULL, 0);
                break;
           case IDC_FUNC:
                if (HIWORD(wParam) == CBN_CLOSEUP) {
                   func=(int)SendMessage(GetDlgItem(hWnd,IDC_FUNC),CB_GETCURSEL, 0, 0L);
                }
                for (i=0; i<127;i++) txtMsg[i] = ' ';
                txtMsg[i] = 0;
                InvalidateRect(hWnd, NULL, 0);
                break;
            default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_DESTROY:
        if (card>=0) Release_Card(card);
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}

void wait_pulse_start()
{
  U32 oldvalue,newvalue;

  do
  {
     CTR_Read( card, 1, &oldvalue );
     CTR_Read( card, 1, &newvalue );
  } while( newvalue != oldvalue );
  do
  {
     CTR_Read( card, 1, &oldvalue );
     CTR_Read( card, 1, &newvalue );
  } while( newvalue == oldvalue );
}
