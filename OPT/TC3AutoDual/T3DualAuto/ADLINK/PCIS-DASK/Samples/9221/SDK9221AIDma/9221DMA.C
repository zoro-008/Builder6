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
#define IDC_START       208
#define IDC_STOP        209
#define IDC_PAUSE       210
#define IDC_EDIT        211
#define IDC_CHAN        214
#define IDC_SHOW        213
#define NONE              0

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
long PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL PASCAL ShowINT(HWND, UINT, WPARAM, LPARAM);

HANDLE hInst;
HWND hListBox;
HWND hChanComboBox;
HWND hADScroll;
HWND hEdit,hData;
unsigned int oldinput[16], adinput, oldx[16], oldy[16];

long sample_rate=250000;
I16 card, card_number =0;
long data_size=4000;
U16 channel=0, range=AD_B_5_V;
U32	ScanIntv, SampIntv;
U16 bufferID;			

DWORD  mem_size = 0;
static HANDLE hMem=NULL;
U16 *ai_buf;

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

HBRUSH hBlackBrush,hLTBlueBrush,hBlueBrush,hDPBlueBrush,hLTGreenBrush,hGreenBrush,hDPGreenBrush,hLTRedBrush,hRedBrush,hDPRedBrush,hPurpleBrush,hBlueGreenBrush,hYellowBrush,hDPPurpleBrush,hDPBlueGreenBrush,hDPYellowBrush,hWhiteBrush;
HPEN hRedPen,hBluePen,hGreenPen,hLTRedPen,hLTBluePen,hLTGreenPen,hDPRedPen,hDPBluePen,hDPGreenPen,hPurplePen,hBlueGreenPen,hYellowPen,hDPPurplePen,hDPBlueGreenPen,hDPYellowPen,hWhitePen;

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
                      (LPCSTR)"9221 Autoscan DMA",
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
  int i,iii,k, px, py;
  int cur_channel;
  unsigned long hi,lo;
  long ii;
  char n[6];
  char buf[100];
  FARPROC  lpProc;
  static   int pos=0;
  static int count,status;
  int p, err;
  BOOLEAN bStopped;

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
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH1 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH2 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH3 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH4 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH5 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH6 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH7 ~ CH0");
		   SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH8 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH9 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH10 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH11 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH12 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH13 ~ CH0");
           SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH14 ~ CH0");
		   SendMessage(hChanComboBox, CB_ADDSTRING, NULL, (LONG)(LPSTR)"CH15 ~ CH0");
           SendMessage(hChanComboBox ,CB_SETCURSEL, 0, 0L);
           hEdit=CreateWindow("EDIT", NULL,
                   WS_CHILD | WS_VISIBLE | ES_LEFT,
                   output_pos[13].x-30, input_pos[15].y+20,40, 20, hWnd, IDC_EDIT, hInst, NULL);

           wsprintf(n,"%d",sample_rate/1000);
           SetWindowText(hEdit,n);

           CreateWindow("SCROLLBAR", NULL,
                   WS_CHILD | WS_VISIBLE | SBS_VERT,
                   output_pos[13].x+10, input_pos[15].y+20,20, 20, hWnd, IDC_SCROLL, hInst, NULL);

           CreateWindow("BUTTON","Start" ,
                   WS_CHILD | WS_VISIBLE ,
                   output_pos[10].x+140, input_pos[10].y+10,70, 30, hWnd, IDC_START, hInst, NULL);

           hData=CreateWindow("BUTTON","Data Value"   ,
                   WS_CHILD | WS_VISIBLE ,
                   output_pos[10].x+230, input_pos[10].y+10,80, 30, hWnd, IDC_SHOW, hInst, NULL);


		   hBlackBrush= CreateSolidBrush(RGB(0,0,0));
		   hLTBlueBrush = CreateSolidBrush(RGB(0,0,255));
		   hBlueBrush = CreateSolidBrush(RGB(0,0,128));
		   hDPBlueBrush = CreateSolidBrush(RGB(0,0,64));
		   hLTGreenBrush = CreateSolidBrush(RGB(0,255,0));
		   hGreenBrush = CreateSolidBrush(RGB(0,128,0));
		   hDPGreenBrush = CreateSolidBrush(RGB(0,64,0));
		   hLTRedBrush = CreateSolidBrush(RGB(255,0,0));
		   hRedBrush = CreateSolidBrush(RGB(128,0,0));
		   hDPRedBrush = CreateSolidBrush(RGB(64,0,0));
		   hPurpleBrush = CreateSolidBrush(RGB(255,0,255));
		   hBlueGreenBrush= CreateSolidBrush(RGB(0,255,255));
		   hYellowBrush= CreateSolidBrush(RGB(255,255,0));
		   hDPPurpleBrush = CreateSolidBrush(RGB(128,0,128));
		   hDPBlueGreenBrush= CreateSolidBrush(RGB(0,128,128));
		   hDPYellowBrush= CreateSolidBrush(RGB(128,128,0));
		   hWhiteBrush= CreateSolidBrush(RGB(255,255,255));

		   hRedPen= CreatePen(PS_SOLID,1,RGB(128,0,0));
		   hBluePen= CreatePen(PS_SOLID,1,RGB(0,0,128));
		   hGreenPen= CreatePen(PS_SOLID,1,RGB(0,128,0));
		   hLTRedPen= CreatePen(PS_SOLID,1,RGB(255,0,0));
		   hLTBluePen= CreatePen(PS_SOLID,1,RGB(0,0,255));
		   hLTGreenPen= CreatePen(PS_SOLID,1,RGB(0,255,0));
		   hDPRedPen= CreatePen(PS_SOLID,1,RGB(64,0,0));
		   hDPBluePen= CreatePen(PS_SOLID,1,RGB(0,0,64));
		   hDPGreenPen= CreatePen(PS_SOLID,1,RGB(0,64,0));
		   hPurplePen = CreatePen(PS_SOLID,1,RGB(255,0,255));
		   hBlueGreenPen=CreatePen(PS_SOLID,1,RGB(0,255,255));
		   hYellowPen=CreatePen(PS_SOLID,1,RGB(255,255,0));
		   hDPPurplePen =CreatePen(PS_SOLID,1,RGB(128,0,128));
		   hDPBlueGreenPen=CreatePen(PS_SOLID,1,RGB(0,128,128));
		   hDPYellowPen=CreatePen(PS_SOLID,1,RGB(128,128,0));
		   hWhitePen= CreatePen(PS_SOLID,1,RGB(255,255,255));
           break;

    case WM_PAINT:
           hDC = BeginPaint(hWnd, &ps);

           // Analog Input Box
           SelectObject(hDC, GetStockObject(GRAY_BRUSH));
           Rectangle(hDC, output_pos[15].x-10, io_pos[15].y-24, output_pos[0].x+26, io_pos[0].y+195);
           SetTextColor(hDC,RGB(255,255,0));
           SetBkColor(hDC, RGB(0, 0, 255));
           TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "    DMA Input(A/D)  ", 20);

           SelectObject(hDC, hBlackBrush);
           Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+172);

/*0*/
           SelectObject(hDC,hRedBrush );
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y-5+18,output_pos[4].x+80, io_pos[0].y+10+18);

           SelectObject(hDC, hBlueBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+13+18,output_pos[4].x+80, io_pos[0].y+28+18);

           SelectObject(hDC, hGreenBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+31+18,output_pos[4].x+80, io_pos[0].y+46+18);

           SelectObject(hDC, hLTRedBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+49+18,output_pos[4].x+80, io_pos[0].y+64+18);


           SelectObject(hDC,hLTBlueBrush );
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+67+18,output_pos[4].x+80, io_pos[0].y+82+18);

           SelectObject(hDC, hLTGreenBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+85+18,output_pos[4].x+80, io_pos[0].y+100+18);

           SelectObject(hDC, hDPRedBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+103+18,output_pos[4].x+80, io_pos[0].y+118+18);

           SelectObject(hDC, hDPBlueBrush);
           Rectangle(hDC,output_pos[4].x+68, io_pos[0].y+121+18,output_pos[4].x+80, io_pos[0].y+136+18);

/*8*/
           SelectObject(hDC, hDPGreenBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y-5+18,output_pos[4].x+120, io_pos[0].y+10+18);

           SelectObject(hDC, hPurpleBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+13+18,output_pos[4].x+120, io_pos[0].y+28+18);

           SelectObject(hDC, hBlueGreenBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+31+18,output_pos[4].x+120, io_pos[0].y+46+18);

           SelectObject(hDC, hYellowBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+49+18,output_pos[4].x+120, io_pos[0].y+64+18);

           SelectObject(hDC, hDPPurpleBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+67+18,output_pos[4].x+120, io_pos[0].y+82+18);

           SelectObject(hDC, hDPBlueGreenBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+85+18,output_pos[4].x+120, io_pos[0].y+100+18);

           SelectObject(hDC, hDPYellowBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+103+18,output_pos[4].x+120, io_pos[0].y+118+18);

           SelectObject(hDC, hWhiteBrush);
           Rectangle(hDC,output_pos[4].x+108, io_pos[0].y+121+18,output_pos[4].x+120, io_pos[0].y+136+18);


           SetBkMode(hDC,TRANSPARENT);
           TextOut(hDC,output_pos[13].x-60, input_pos[10].y,"Samping Rate",12);
           TextOut(hDC,output_pos[13].x+30, input_pos[15
                           ].y+20,"KHZ",3);
           TextOut(hDC,output_pos[13].x+80, input_pos[10].y,"Channel",7);
           TextOut(hDC,input_pos[2].x-10, io_pos[15].y+160,"Range: +-5V",11);
           TextOut(hDC,input_pos[15].x, io_pos[15].y-19,"Volts",5);
           TextOut(hDC,input_pos[15].x, io_pos[15].y-5,"5.00",4);
           TextOut(hDC,input_pos[15].x, io_pos[15].y+36,"2.50",4);
           TextOut(hDC,input_pos[15].x, io_pos[15].y+77,"0.00",4);
           TextOut(hDC,input_pos[15].x-3, io_pos[15].y+118,"-2.50",5);
           TextOut(hDC,input_pos[15].x-3, io_pos[15].y+159,"-5.00",5);
           TextOut(hDC,input_pos[10].x+10, io_pos[10].y-19,"Signals",7);

           TextOut(hDC,output_pos[4].x+58, io_pos[0].y-5+18,"0",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+13+18,"1",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+31+18,"2",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+49+18,"3",1);

           TextOut(hDC,output_pos[4].x+98, io_pos[0].y-5+18,"8",1);
           TextOut(hDC,output_pos[4].x+98, io_pos[0].y+13+18,"9",1);
           TextOut(hDC,output_pos[4].x+90, io_pos[0].y+31+18,"10",2);
           TextOut(hDC,output_pos[4].x+90, io_pos[0].y+49+18,"11",2);

		   TextOut(hDC,output_pos[4].x+58, io_pos[0].y+67+18,"4",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+85+18,"5",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+103+18,"6",1);
           TextOut(hDC,output_pos[4].x+58, io_pos[0].y+121+18,"7",1);

           TextOut(hDC,output_pos[4].x+90, io_pos[0].y+67+18,"12",2);
           TextOut(hDC,output_pos[4].x+90, io_pos[0].y+85+18,"13",2);
           TextOut(hDC,output_pos[4].x+90, io_pos[0].y+103+18,"14",2);
           TextOut(hDC,output_pos[4].x+90, io_pos[0].y+121+18,"15",2);

           EndPaint(hWnd, &ps);
           break;

    // Sample Rate Spin Control
    case WM_VSCROLL:
           switch(LOWORD(wParam)){
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

           case IDC_SHOW :
                DialogBox(hInst,
                          "DMABox",
                          hWnd,
                          ShowINT);
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
                  mem_size=data_size*2;
                  hMem = GlobalAlloc(GMEM_ZEROINIT,mem_size);
                  ai_buf = GlobalLock(hMem);
                  if (ai_buf == NULL )
                  {
                     MessageBox(hWnd,"INT",
                                "No Memory", MB_OK);
                  }
                  GlobalFix(hMem);
				  AI_9221_Config (card, P9221_AI_SingEnded|P9221_AI_IntTimeBase, P9221_TRGMOD_SOFT, 0);
				  //AI_AsyncDblBufferMode(card, 0);
				  SampIntv = 40000000/sample_rate;
				  ScanIntv = SampIntv*(channel+1);
				  AI_9221_CounterInterval (card, ScanIntv, SampIntv);

				  AI_ContBufferSetup (card, ai_buf, data_size, &bufferID);
			
				  AI_ContScanChannels (card, channel, range, &bufferID, data_size, 0, SYNCH_OP);
                  
				  EnableWindow (hData, TRUE);
                  for (i=0;i<16;i++) oldinput[i]=-1;
                  hDC = GetDC(hWnd);
                  SelectObject(hDC, hBlackBrush);
                  Rectangle(hDC,output_pos[15].x+35,io_pos[0].y-3,output_pos[15].x+370,io_pos[0].y+175);

                  for (i=0;i<data_size;i++) {
                   adinput = ai_buf[i];
                   cur_channel=i%(channel+1);
                   switch(cur_channel) {
                     case 0:
                        SelectObject(hDC, hRedPen);
                        break;
                     case 1:
                        SelectObject(hDC, hBluePen);
                        break;
                     case 2:
                        SelectObject(hDC, hGreenPen);
                        break;
                     case 3:
                        SelectObject(hDC, hLTRedPen);
                        break;
                     case 4:
                        SelectObject(hDC, hLTBluePen);
                        break;
                     case 5:
                        SelectObject(hDC, hLTGreenPen);
                        break;
                     case 6:
                        SelectObject(hDC, hDPRedPen);
                        break;
                     case 7:
                        SelectObject(hDC, hDPBluePen);
                        break;
					 case 8:
                        SelectObject(hDC, hDPGreenPen);
                        break;
                     case 9:
                        SelectObject(hDC, hPurplePen);
                        break;
                     case 10:
                        SelectObject(hDC, hBlueGreenPen);
                        break;
                     case 11:
                        SelectObject(hDC, hYellowPen);
                        break;
                     case 12:
                        SelectObject(hDC, hDPPurplePen);
                        break;
                     case 13:
                        SelectObject(hDC, hDPBlueGreenPen);
                        break;
                     case 14:
                        SelectObject(hDC, hDPYellowPen);
                        break;
                     case 15:
                        SelectObject(hDC, hWhitePen);
                        break;
                   }
                   //adinput = adinput >> 4;
                   adinput=io_pos[0].y+86-( 89*((short)adinput)/32768 );
                   if (oldinput[cur_channel]!=-1)
                      MoveToEx(hDC, oldx[cur_channel], oldinput[cur_channel],NULL);
                   else{
                      MoveToEx(hDC,(i*334)/data_size+output_pos[15].x+35,adinput,NULL);
				   }
				   LineTo(hDC,(i*334)/data_size+output_pos[15].x+35,adinput);
				   oldx[cur_channel] = (i*334)/data_size+output_pos[15].x+35;	
                   oldinput[cur_channel]=adinput;
                } // for
                ReleaseDC(hWnd, hDC);
				AI_ContBufferReset(card);

                break;

             default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
    break;

    case WM_DESTROY:
         if (card >= 0) Release_Card (card);
		 DeleteObject(hRedBrush);
		 DeleteObject(hBlueBrush);
		 DeleteObject(hGreenBrush);
		 DeleteObject(hLTRedBrush);
		 DeleteObject(hLTBlueBrush);
		 DeleteObject(hLTGreenBrush);
		 DeleteObject(hDPRedBrush);
		 DeleteObject(hDPBlueBrush);
		 DeleteObject(hDPGreenBrush);
		 DeleteObject(hPurpleBrush);
		 DeleteObject(hBlueGreenBrush);
		 DeleteObject(hYellowBrush);
		 DeleteObject(hDPPurpleBrush);
		 DeleteObject(hDPBlueGreenBrush);
		 DeleteObject(hDPYellowBrush);
		 DeleteObject(hWhiteBrush);
		 DeleteObject(hBlackBrush);
         DeleteObject(hRedPen);
		 DeleteObject(hBluePen);
		 DeleteObject(hGreenPen);
		 DeleteObject(hLTRedPen);
		 DeleteObject(hLTBluePen);
		 DeleteObject(hLTGreenPen);
		 DeleteObject(hDPRedPen);
		 DeleteObject(hDPBluePen);
		 DeleteObject(hDPGreenPen);
		 DeleteObject(hPurplePen);
		 DeleteObject(hBlueGreenPen);
		 DeleteObject(hYellowPen);
		 DeleteObject(hDPPurplePen);
		 DeleteObject(hDPBlueGreenPen);
		 DeleteObject(hDPYellowPen);
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
    long j;
    long i;
    char cBuf[100] = "\t131072 \tFF";
    char string[10]="      ";
    char data_str[3]="  ";
    U16 *pp;
    switch (message)
    {
       case WM_INITDIALOG:
            hListBox = CreateWindow("LISTBOX", "Memory Count                            Data",\
            WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS\
            | LBS_STANDARD | LBS_USETABSTOPS |WS_CAPTION,\
            50, 40, 240, 180, hDlg, 100, hInst, NULL);

            SendMessage(hListBox, LB_RESETCONTENT, 0, 0L);
            SendMessage(hListBox, LB_SETTABSTOPS, 2, (DWORD)\
                        (LPINT) nTabs);
            pp=(U16 *) ai_buf;
            for(i=0; i<data_size; i++) {
               wsprintf(cBuf, "%5d\t\t%04x", i, pp[i]/*&0xfff*/);
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