// EtcWindow.cpp : implementation file
//

#include "stdafx.h"
#include "HVSystem.h"
#include "Mainfrm.h"
#include "HVSystemDoc.h"
#include "HVSystemView.h"
#include "EtcWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcWindow dialog


CEtcWindow::CEtcWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CEtcWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtcWindow)
	m_nROffset = 0.0;
	m_nTOffset = 0.0;
	m_iPixel=640;
	m_dDistance=4.8;
	m_iDispMode=0;
	m_iBThr=55;
	m_iEThr=12;
	m_dGTShift=-0.1;
	m_dGTOffset=0.05;
	m_dGTShiftHigh=0.1;
	m_dGTShift2=-0.1;
	m_dGTShiftHigh2=-0.1;
	m_dGTShift3=-0.1;
	m_dGTShiftHigh3=-0.1;
	m_iApcOk=140;
	m_iApcOkHigh=230;
	m_iBeamSize=0;
	m_iBeamSizeHigh=0;
	m_iBcPort=0;
	m_iSaveMode=0;
	//}}AFX_DATA_INIT

	int i;
	m_pwEtched=NULL;
	for(i=0; i<2; i++)
	{
		m_pwName[i]=NULL;
		m_pwRT[i]=NULL;
		m_pwSpin[i]=NULL;

		m_pwButton[i]=NULL;
	}

	for(i=0; i<11; i++)
	{
		m_pwStaticEx[i]=NULL;
		m_pwEditEx[i]=NULL;
		m_pwSpinEx[i]=NULL;
	}
	for(i=0; i<2; i++)
	{
		m_pwStCom[i]=NULL;
		m_pwCombo[i]=NULL;
	}

	for(i=0; i<5; i++)
	{
		m_pwStaticExAdd[i]=NULL;
		m_pwEditExAdd[i]=NULL;
		m_pwSpinExAdd[i]=NULL;
	}

	m_pwButtonMaster=NULL;
    m_pwButtonRoi=NULL;
	m_nWorkMode=0;

	m_pFont=new CFont;
	m_pFont->CreateFont(16,8, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "명조체");

}

void CEtcWindow::SetDefault()
{
	m_iPixel=640;
	m_dDistance=4.2;
	m_iDispMode=0;
	m_iBThr=35;
	m_iEThr=90;
	m_dGTShift=-0.1;
	m_dGTOffset=0.05;
	m_dGTShiftHigh=0.1;
	m_dGTShift2=-0.1;
	m_dGTShiftHigh2=-0.1;
	m_dGTShift3=-0.1;
	m_dGTShiftHigh3=-0.1;
	m_iApcOk=140;
	m_iApcOkHigh=230;

	m_iBeamSize=0;
	m_iBeamSizeHigh=0;
}

void CEtcWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcWindow)
	DDX_Text(pDX, IDC_EDIT_TAB_ROFFSET, m_nROffset);
	DDX_Text(pDX, IDC_EDIT_TAB_TOFFSET, m_nTOffset);
	DDX_Text(pDX, IDC_EDIT_ETC1, m_iPixel);
	DDX_Text(pDX, IDC_EDIT_ETC2, m_dDistance);
	DDX_Text(pDX, IDC_EDIT_ETC3, m_iDispMode);
	DDX_Text(pDX, IDC_EDIT_ETC4, m_iBThr);
	DDX_Text(pDX, IDC_EDIT_ETC5, m_iEThr);
	DDX_Text(pDX, IDC_EDIT_ETC6, m_dGTOffset);
	DDX_Text(pDX, IDC_EDIT_ETC7, m_dGTShift);
	DDX_Text(pDX, IDC_EDIT_ETC8, m_dGTShiftHigh);
	DDX_Text(pDX, IDC_EDIT_ETC9, m_dGTShift2);
	DDX_Text(pDX, IDC_EDIT_ETC10, m_dGTShiftHigh2);
	DDX_Text(pDX, IDC_EDIT_ETC11, m_dGTShift3);
	DDX_Text(pDX, IDC_EDIT_ETC12, m_dGTShiftHigh3);
	DDX_Text(pDX, IDC_EDIT_ETC13, m_iBeamSize);
	DDX_Text(pDX, IDC_EDIT_ETC14, m_iBeamSizeHigh);
	DDX_Text(pDX, IDC_EDIT_ETC15, m_iApcOk);
	DDX_Text(pDX, IDC_EDIT_ETC16, m_iApcOkHigh);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtcWindow, CDialog)
	//{{AFX_MSG_MAP(CEtcWindow)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_TAB_ETCSAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_TAB_ETCAPPLY, OnButtonApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TAB_TOFFSET, OnDeltaposSpinToffset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TAB_ROFFSET, OnDeltaposSpinRoffset)
	ON_BN_CLICKED(IDC_BUTTON_MASTERMODE, OnButtonMastermode)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC1, OnDeltaposSpinEtc1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC2, OnDeltaposSpinEtc2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC3, OnDeltaposSpinEtc3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC4, OnDeltaposSpinEtc4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC5, OnDeltaposSpinEtc5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC6, OnDeltaposSpinEtc6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC7, OnDeltaposSpinEtc7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC8, OnDeltaposSpinEtc8)
	ON_BN_CLICKED(IDC_CHECK_ETC_ROI, OnCheckEtcRoi)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC9, OnDeltaposSpinEtc9)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC10, OnDeltaposSpinEtc10)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC11, OnDeltaposSpinEtc11)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC12, OnDeltaposSpinEtc12)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC13, OnDeltaposSpinEtc13)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC14, OnDeltaposSpinEtc14)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC16, OnDeltaposSpinEtc16)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ETC15, OnDeltaposSpinEtc15)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcWindow message handlers

BOOL CEtcWindow::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN) {
		switch(pMsg->wParam) {
			case VK_RETURN:
			case VK_ESCAPE: return true;
			default: 
				 break; 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEtcWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	int i;
	
	if(m_pwEtched!=NULL) delete m_pwEtched;

	for(i=0; i<2; i++)
	{
		if(m_pwName[i]!=NULL) delete m_pwName[i];
		if(m_pwRT[i]  !=NULL) delete m_pwRT[i];
		if(m_pwSpin[i]!=NULL) delete m_pwSpin[i];
		if(m_pwButton[i]!=NULL) delete m_pwButton[i];
	}
    
	for(i=0; i<11; i++)
	{
		if(m_pwStaticEx[i]!=NULL) delete m_pwStaticEx[i];
		if(m_pwEditEx[i]!=NULL)   delete m_pwEditEx[i];
		if(m_pwSpinEx[i]!=NULL)   delete m_pwSpinEx[i];
	}

	for(i=0; i<5 ; i++)
	{
		if(m_pwStaticExAdd[i]!=NULL) delete m_pwStaticExAdd[i];
		if(m_pwEditExAdd[i]!=NULL)   delete m_pwEditExAdd[i];
		if(m_pwSpinExAdd[i]!=NULL)   delete m_pwSpinExAdd[i];
	}

	if(m_pwButtonMaster!=NULL) delete m_pwButtonMaster;
    if(m_pwButtonRoi!=NULL)    delete m_pwButtonRoi;

	for(i=0; i<2; i++)
	{
		if(m_pwCombo[i]!=NULL) delete m_pwCombo[i];
		if(m_pwStCom[i]!=NULL) delete m_pwStCom[i];
	}

	if(m_pFont!=NULL) delete m_pFont;
	delete pBmpRgn;		
	// TODO: Add your message handler code here
	
}

BOOL CEtcWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/************************************/
	pBmpRgn = new CBmpRgn(this);
	HRGN rgnH = pBmpRgn->BuildRegion(".\\guiimage\\SetupWindowU_256.bmp", 0x00ff00ff);
	if(rgnH)
	{
		SetWindowRgn(rgnH, TRUE);
		DeleteObject(rgnH);
	}
	CDC *pDC = GetDC();
	m_dcBkGrnd = CreateCompatibleDC(pDC->m_hDC);
	ReleaseDC(pDC);
	// select background image
	HBITMAP hBmp = pBmpRgn->GetBitmap();
	SelectObject(m_dcBkGrnd, hBmp);

	m_nWindowWidth = pBmpRgn->GetBitmapWidth();
	m_nWindowHeight = pBmpRgn->GetBitmapHeight();
	/*************************************/		
	MakeGui_1();
    MakeGui_12();
	MakeGui_2();
	LoadFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcWindow::MakeGui_1()
{
	int i;
	int k;
	int j=0;
    UINT nSIDs[2]={IDC_STATIC_TAB_ROFFSET,IDC_STATIC_TAB_TOFFSET};
	UINT nEIDs[2]={IDC_EDIT_TAB_ROFFSET  ,IDC_EDIT_TAB_TOFFSET  };
	UINT nAIDs[2]={IDC_SPIN_TAB_ROFFSET  ,IDC_SPIN_TAB_TOFFSET  };
	char *pTitle[2]={"R-OFFSET","T-OFFSET"};

	for(i=0; i<2; i++)
	{
		k=((i+2)%2)*168;

		m_pwName[i]=new CStaticSkin;
		m_pwName[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
		m_pwName[i]->SubclassDlgItem(nSIDs[i],this);
		m_pwName[i]->OutTextXY(4,6,pTitle[i]);
		m_pwName[i]->MoveWindow(4+k,4+26*j,128,26);

		m_pwRT[i]=new CEdit;
		m_pwRT[i]->SubclassDlgItem(nEIDs[i],this);
		m_pwRT[i]->MoveWindow(90+k,4+26*j,48,26);

		m_pwSpin[i]=new CSpinButtonCtrl;
		m_pwSpin[i]->SubclassDlgItem(nAIDs[i],this);
		m_pwSpin[i]->MoveWindow(138+k,4+26*j,20,26);

		if(((i+1)%2)==0) j++;
	}
	
	return; 
}
void CEtcWindow::MakeGui_12()
{
	int i;
	int k;
	int j=0;
    UINT nSIDs[11]=
	{
		IDC_STATIC_ETC1,IDC_STATIC_ETC2,IDC_STATIC_ETC3,
		IDC_STATIC_ETC4,IDC_STATIC_ETC5,IDC_STATIC_ETC6,
		IDC_STATIC_ETC7,IDC_STATIC_ETC8,IDC_STATIC_ETC9,
		IDC_STATIC_ETC12,IDC_STATIC_ETC13
	};

	UINT nEIDs[11]=
	{
		IDC_EDIT_ETC1,IDC_EDIT_ETC2,IDC_EDIT_ETC3, 
		IDC_EDIT_ETC4,IDC_EDIT_ETC5,IDC_EDIT_ETC6,
		IDC_EDIT_ETC7,IDC_EDIT_ETC9,IDC_EDIT_ETC11,
		IDC_EDIT_ETC13,IDC_EDIT_ETC15

	};
	UINT nAIDs[11]=
	{
		IDC_SPIN_ETC1,IDC_SPIN_ETC2,IDC_SPIN_ETC3,  
		IDC_SPIN_ETC4,IDC_SPIN_ETC5,IDC_SPIN_ETC6,
		IDC_SPIN_ETC7,IDC_SPIN_ETC9,IDC_SPIN_ETC11,
		IDC_SPIN_ETC13,IDC_SPIN_ETC15
	};

	char *pTitle[11]={"PIXEL","DISTANCE","B-DISP","B-THR","E-THR","GT OFFSET","GT SHIFT","GT 2SHIFT","GT 3SHIFT","BEAM SIZE","APC OK"};

	for(i=0; i<11; i++)
	{
		
        k=((i+2)%2)*168;
		if(i<6) 
		{
			
			
			m_pwStaticEx[i]=new CStaticSkin;
			m_pwStaticEx[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
			m_pwStaticEx[i]->SubclassDlgItem(nSIDs[i],this);
			m_pwStaticEx[i]->OutTextXY(4,6,pTitle[i]);
			m_pwStaticEx[i]->MoveWindow(4+k,40+26*j,100,26);
			
			m_pwEditEx[i]=new CEdit;
			m_pwEditEx[i]->SubclassDlgItem(nEIDs[i],this);
			m_pwEditEx[i]->MoveWindow(90+k,40+26*j,48,26);
			
			m_pwSpinEx[i]=new CSpinButtonCtrl;
			m_pwSpinEx[i]->SubclassDlgItem(nAIDs[i],this);
			m_pwSpinEx[i]->MoveWindow(138+k,40+26*j,20,26);
		}
		if(i==6)
		{
			m_pwStaticEx[i]=new CStaticSkin;
			m_pwStaticEx[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
			m_pwStaticEx[i]->SubclassDlgItem(nSIDs[i],this);
			m_pwStaticEx[i]->OutTextXY(4,6,pTitle[i]);
			m_pwStaticEx[i]->MoveWindow(4,40+26*j,100,26);
			
			m_pwEditEx[i]=new CEdit;
			m_pwEditEx[i]->SubclassDlgItem(nEIDs[i],this);
			m_pwEditEx[i]->MoveWindow(90,40+26*j,48,26);
			
			m_pwSpinEx[i]=new CSpinButtonCtrl;
			m_pwSpinEx[i]->SubclassDlgItem(nAIDs[i],this);
			m_pwSpinEx[i]->MoveWindow(138,40+26*j,20,26);

			m_pwEditExAdd[0]=new CEdit;
			m_pwEditExAdd[0]->SubclassDlgItem(IDC_EDIT_ETC8,this);
			m_pwEditExAdd[0]->MoveWindow(158,40+26*j,48,26);
			
			m_pwSpinExAdd[0]=new CSpinButtonCtrl;
			m_pwSpinExAdd[0]->SubclassDlgItem(IDC_SPIN_ETC8,this);
			m_pwSpinExAdd[0]->MoveWindow(206,40+26*j,20,26);
			j++;
		}
		if(i==7)
		{
			m_pwStaticEx[i]=new CStaticSkin;
			m_pwStaticEx[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
			m_pwStaticEx[i]->SubclassDlgItem(nSIDs[i],this);
			m_pwStaticEx[i]->OutTextXY(4,6,pTitle[i]);
			m_pwStaticEx[i]->MoveWindow(4,40+26*j,100,26);
			
			m_pwEditEx[i]=new CEdit;
			m_pwEditEx[i]->SubclassDlgItem(nEIDs[i],this);
			m_pwEditEx[i]->MoveWindow(90,40+26*j,48,26);
			
			m_pwSpinEx[i]=new CSpinButtonCtrl;
			m_pwSpinEx[i]->SubclassDlgItem(nAIDs[i],this);
			m_pwSpinEx[i]->MoveWindow(138,40+26*j,20,26);

			m_pwEditExAdd[1]=new CEdit;
			m_pwEditExAdd[1]->SubclassDlgItem(IDC_EDIT_ETC10,this);
			m_pwEditExAdd[1]->MoveWindow(158,40+26*j,48,26);
			
			m_pwSpinExAdd[1]=new CSpinButtonCtrl;
			m_pwSpinExAdd[1]->SubclassDlgItem(IDC_SPIN_ETC10,this);
			m_pwSpinExAdd[1]->MoveWindow(206,40+26*j,20,26);
		}
		if(i==8)
		{
			m_pwStaticEx[i]=new CStaticSkin;
			m_pwStaticEx[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
			m_pwStaticEx[i]->SubclassDlgItem(nSIDs[i],this);
			m_pwStaticEx[i]->OutTextXY(4,6,pTitle[i]);
			m_pwStaticEx[i]->MoveWindow(4,40+26*j,100,26);
			
			m_pwEditEx[i]=new CEdit;
			m_pwEditEx[i]->SubclassDlgItem(nEIDs[i],this);
			m_pwEditEx[i]->MoveWindow(90,40+26*j,48,26);
			
			m_pwSpinEx[i]=new CSpinButtonCtrl;
			m_pwSpinEx[i]->SubclassDlgItem(nAIDs[i],this);
			m_pwSpinEx[i]->MoveWindow(138,40+26*j,20,26);

			m_pwEditExAdd[2]=new CEdit;
			m_pwEditExAdd[2]->SubclassDlgItem(IDC_EDIT_ETC12,this);
			m_pwEditExAdd[2]->MoveWindow(158,40+26*j,48,26);
			
			m_pwSpinExAdd[2]=new CSpinButtonCtrl;
			m_pwSpinExAdd[2]->SubclassDlgItem(IDC_SPIN_ETC12,this);
			m_pwSpinExAdd[2]->MoveWindow(206,40+26*j,20,26);
			j++;
		}
		if(i==9)
		{
			m_pwStaticEx[i]=new CStaticSkin;
			m_pwStaticEx[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
			m_pwStaticEx[i]->SubclassDlgItem(nSIDs[i],this);
			m_pwStaticEx[i]->OutTextXY(4,6,pTitle[i]);
			m_pwStaticEx[i]->MoveWindow(4,40+26*j,100,26);
			
			m_pwEditEx[i]=new CEdit;
			m_pwEditEx[i]->SubclassDlgItem(nEIDs[i],this);
			m_pwEditEx[i]->MoveWindow(90,40+26*j,48,26);
			
			m_pwSpinEx[i]=new CSpinButtonCtrl;
			m_pwSpinEx[i]->SubclassDlgItem(nAIDs[i],this);
			m_pwSpinEx[i]->MoveWindow(138,40+26*j,20,26);

			m_pwEditExAdd[3]=new CEdit;
			m_pwEditExAdd[3]->SubclassDlgItem(IDC_EDIT_ETC14,this);
			m_pwEditExAdd[3]->MoveWindow(158,40+26*j,48,26);
			
			m_pwSpinExAdd[3]=new CSpinButtonCtrl;
			m_pwSpinExAdd[3]->SubclassDlgItem(IDC_SPIN_ETC14,this);
			m_pwSpinExAdd[3]->MoveWindow(206,40+26*j,20,26);
		}
		if(i==10)
		{
			m_pwStaticEx[i]=new CStaticSkin;
			m_pwStaticEx[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
			m_pwStaticEx[i]->SubclassDlgItem(nSIDs[i],this);
			m_pwStaticEx[i]->OutTextXY(4,6,pTitle[i]);
			m_pwStaticEx[i]->MoveWindow(4,40+26*j,100,26);
			
			m_pwEditEx[i]=new CEdit;
			m_pwEditEx[i]->SubclassDlgItem(nEIDs[i],this);
			m_pwEditEx[i]->MoveWindow(90,40+26*j,48,26);
			
			m_pwSpinEx[i]=new CSpinButtonCtrl;
			m_pwSpinEx[i]->SubclassDlgItem(nAIDs[i],this);
			m_pwSpinEx[i]->MoveWindow(138,40+26*j,20,26);

			m_pwEditExAdd[4]=new CEdit;
			m_pwEditExAdd[4]->SubclassDlgItem(IDC_EDIT_ETC16,this);
			m_pwEditExAdd[4]->MoveWindow(158,40+26*j,48,26);
			
			m_pwSpinExAdd[4]=new CSpinButtonCtrl;
			m_pwSpinExAdd[4]->SubclassDlgItem(IDC_SPIN_ETC16,this);
			m_pwSpinExAdd[4]->MoveWindow(206,40+26*j,20,26);
		}
		if(((i+1)%2)==0) j++;
	}
	
	j++;

	UINT uSTID2[2]={IDC_STATIC_ETC10,IDC_STATIC_ETC11};
	UINT uCBID2[2]={IDC_COMBO1,IDC_COMBO2};
	char *str[2]={"BC PORT","SAVE MODE"};

	for(i=0; i<2; i++)
	{
		m_pwStCom[i]=new CStaticSkin;
		m_pwStCom[i]->SetBmpFile(".\\guiimage\\st128x26_256.bmp");
		m_pwStCom[i]->SubclassDlgItem(uSTID2[i],this);
		m_pwStCom[i]->OutTextXY(4,6,str[i]);
		//m_pwStCom[i]->MoveWindow(4,40+26*(j+i),128,26);
		m_pwStCom[i]->MoveWindow(4,40+26*(j+i),100,26);//(4,40+26*(j+i),100,26);
		
		m_pwCombo[i]=new CComboBox;
		m_pwCombo[i]->SubclassDlgItem(uCBID2[i],this);
		m_pwCombo[i]->MoveWindow(130,40+26*(j+i),136,74);

		m_pwCombo[i]->SetCurSel(0);
	}
	return; 
}
void CEtcWindow::MakeGui_2()//Save Apply Button.
{
	int i;
	UINT nIDs[2]={IDC_BUTTON_TAB_ETCSAVE,IDC_BUTTON_TAB_ETCAPPLY};

	
	m_pwEtched=new CStatic;
	m_pwEtched->SubclassDlgItem(IDC_STATIC_TAB_FRAME,this);
	m_pwEtched->MoveWindow(5,310,230,80);
	
/////////////////////////////////////////////////////////////////
// Button Displaying
	UINT nIDBMPs[2][9]=
	{
		{IDB_SETUP_SAVEN ,IDB_SETUP_SAVED ,IDB_SETUP_SAVEU ,0,IDB_SETUP_SAVEU ,IDB_SETUP_BTNMASK,0,0,0},
		{IDB_SETUP_APPLYN,IDB_SETUP_APPLYD,IDB_SETUP_APPLYU,0,IDB_SETUP_APPLYU,IDB_SETUP_BTNMASK,0,0,0}
	};

	for(i=0; i<2; i++)
	{
		m_pwButton[i]=new CxSkinButton;
		m_pwButton[i]->SubclassDlgItem(nIDs[i], this);
		m_pwButton[i]->SetToolTipText("");
		m_pwButton[i]->SetSkin(nIDBMPs[i][0],nIDBMPs[i][1],nIDBMPs[i][2],
			                     nIDBMPs[i][3],nIDBMPs[i][4],nIDBMPs[i][5],
							     nIDBMPs[i][6],nIDBMPs[i][7],nIDBMPs[i][8]);
		m_pwButton[i]->MoveWindow(30+100*i,330,80,40);
	}

	m_pwButtonMaster=new CxSkinButton;
	m_pwButtonMaster->SubclassDlgItem(IDC_BUTTON_MASTERMODE,this);
	m_pwButtonMaster->SetSkin(IDB_MASTERN,IDB_MASTERD,0,0,0,IDB_MASK100x50,0,0,0);
	m_pwButtonMaster->SetFont(m_pFont);
	m_pwButtonMaster->MoveWindow(250,310,80,40);//(10,266,80,40);
	m_pwButtonMaster->SetTextColor(RGB(0,0,0));
	m_pwButtonMaster->SetWindowText("MASTER");


	m_pwButtonRoi=new CxSkinButton;
	m_pwButtonRoi->SubclassDlgItem(IDC_CHECK_ETC_ROI,this);
	m_pwButtonRoi->SetSkin(IDB_MASTERN,IDB_MASTERD,0,0,0,IDB_MASK100x50,0,0,0);
	m_pwButtonRoi->SetFont(m_pFont);
	m_pwButtonRoi->MoveWindow(250,350,80,40);//(100,266,80,40);
	m_pwButtonRoi->SetTextColor(RGB(0,0,0));
	m_pwButtonRoi->SetWindowText("ROI");

	return;
}

void CEtcWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_dcBkGrnd)
		BitBlt(dc.m_hDC, 0, 0, m_nWindowWidth, m_nWindowHeight, m_dcBkGrnd, 0, 0, SRCCOPY);
	
}
void CEtcWindow::GetData(SETTING_DATAEX *p)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);


	p->dDistance=m_dDistance;
	p->iDispMode=m_iDispMode;
	p->iBThr=m_iBThr;
	p->iEThr=m_iEThr;
	p->dGTShift=m_dGTShift;
    p->dGTOffset=m_dGTOffset;
    p->r.left=m_pView->m_wImgDisp3.m_cROI.m_r.left;
    p->r.top=m_pView->m_wImgDisp3.m_cROI.m_r.top;
    p->r.right=m_pView->m_wImgDisp3.m_cROI.m_r.right;
    p->r.bottom=m_pView->m_wImgDisp3.m_cROI.m_r.bottom;

	p->iPixel=m_iPixel=p->r.right - p->r.left+1;
	
	p->dGTShiftHigh=m_dGTShiftHigh;
    p->dGTShift2=m_dGTShift2;
	p->dGTShiftHigh2=m_dGTShiftHigh2;
    p->dGTShift3=m_dGTShift3;
	p->dGTShiftHigh3=m_dGTShiftHigh3;
	p->iApcOk=m_iApcOk;
	p->iApcOkHigh=m_iApcOkHigh;

	
	p->iBeamSize=m_iBeamSize;
	p->iBeamSizeHigh=m_iBeamSizeHigh;



	UpdateData(FALSE);

	return;
}

void CEtcWindow::SetData(SETTING_DATAEX *p)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);

	m_dDistance=p->dDistance;
	m_iDispMode=p->iDispMode;
	m_iBThr=p->iBThr;
	m_iEThr=p->iEThr;
	m_dGTShift=p->dGTShift;
    m_dGTOffset=p->dGTOffset;
	m_iBeamSize=p->iBeamSize;
	m_iBeamSizeHigh=p->iBeamSizeHigh;

    


    if(p->r.left <=0 || p->r.right >630 || p->r.top<=0 || p->r.bottom>=470)
    {
		p->r.left=100;
		p->r.top=100;
		p->r.right=200;
		p->r.bottom=200;
	}

	m_pView->m_wImgDisp3.m_cROI.SetRectData(p->r);
	m_iPixel=p->iPixel=p->r.right - p->r.left+1;

	m_dGTShiftHigh=p->dGTShiftHigh;
	m_dGTShift2=p->dGTShift2;
    m_dGTShiftHigh2=p->dGTShiftHigh2;
	m_dGTShift3=p->dGTShift3;
    m_dGTShiftHigh3=p->dGTShiftHigh3;

	m_iBeamSize=p->iBeamSize;
	m_iBeamSizeHigh=p->iBeamSizeHigh;

	m_iApcOk = p->iApcOk;
	m_iApcOkHigh = p->iApcOkHigh;


	UpdateData(FALSE);
	return;
}
void CEtcWindow::GetData2(SETTING_DATAEX2 *p)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);
    
	p->iBPort=m_iBcPort=m_pwCombo[0]->GetCurSel();
	p->iSaveMode=m_iSaveMode=m_pwCombo[1]->GetCurSel();
	UpdateData(FALSE);

	return;
}

void CEtcWindow::SetData2(SETTING_DATAEX2 *p)
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	UpdateData(TRUE);

	m_iBcPort=p->iBPort;
	m_iSaveMode=p->iSaveMode;
	m_pwCombo[0]->SetCurSel(p->iBPort);
	m_pwCombo[1]->SetCurSel(p->iSaveMode);
	UpdateData(FALSE);
	return;
}


int CEtcWindow::LoadFile()
{
	FILE *fp;
	int rvalue=0;
	char buf[256];
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	fp=fopen("rtoffset.bin","r+b");
	if(fp!=NULL)
	{
		fread(&m_nROffset,sizeof(double),1,fp);
		fread(&m_nTOffset,sizeof(double),1,fp);
		fclose(fp);
		rvalue=1;

		sprintf(buf,"%6.2f",m_nROffset);
		m_pwRT[0]->SetWindowText(buf);
		sprintf(buf,"%6.2f",m_nTOffset);
		m_pwRT[1]->SetWindowText(buf);

		m_pView->m_wImgDisp2.SetOffset(m_nROffset,m_nTOffset);
		m_pView->m_wImgDisp2.Invalidate(FALSE);

	}
	else
	{
		rvalue=0;
		m_nROffset=0.0;
		m_nTOffset=0.0;

		m_pView->m_wImgDisp2.SetOffset(m_nROffset,m_nTOffset);
		m_pView->m_wImgDisp2.Invalidate(FALSE);
	}

	fp=fopen(SET_ETC_FILE,"r+b");
	if(fp!=NULL)
	{
		fread(&m_setEtc,sizeof(SETTING_DATAEX),1,fp);
		fclose(fp);
		rvalue=1;

		SetData(&m_setEtc);
	}
	else
	{
		rvalue=0;
		SetDefault();
	}
	fp=fopen(SET_ETC2_FILE,"r+b");
	if(fp!=NULL)
	{
		fread(&m_setEtc2,sizeof(SETTING_DATAEX2),1,fp);
		fclose(fp);
		SetData2(&m_setEtc2);
		
	}
	else
	{
		m_setEtc2.iBPort=m_iBcPort=0;
		m_setEtc2.iSaveMode=m_iSaveMode=0;
	}

	m_pView->SettingParam(m_setEtc);
	m_pView->SettingParam(m_setEtc2);

	return rvalue;
}

void CEtcWindow::OnButtonSave() 
{
	UpdateData(TRUE);
	FILE *fp;
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	fp=fopen("rtoffset.bin","w+b");
	if(fp!=NULL)
	{
		fwrite(&m_nROffset,sizeof(double),1,fp);
		fwrite(&m_nTOffset,sizeof(double),1,fp);
		fclose(fp);
	}

	GetData(&m_setEtc);

	fp=fopen(SET_ETC_FILE,"w+b");
	if(fp!=NULL)
	{
		fwrite(&m_setEtc,sizeof(SETTING_DATAEX),1,fp);
		fclose(fp);
	}
	GetData2(&m_setEtc2);
	fp=fopen(SET_ETC2_FILE,"w+b");

	if(fp!=NULL)
	{
		fwrite(&m_setEtc2,sizeof(SETTING_DATAEX2),1,fp);
		fclose(fp);
		
	}
 

	m_pView->SettingParam(m_setEtc);
	m_pView->SettingParam(m_setEtc2);

	m_pView->m_wImgDisp2.SetOffset(m_nROffset,m_nTOffset);
	m_pView->m_wImgDisp2.Invalidate(FALSE);
	UpdateData(FALSE);
}

void CEtcWindow::OnButtonApply() 
{
	UpdateData(TRUE);
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	m_pView->m_wImgDisp2.SetOffset(m_nROffset,m_nTOffset);
	m_pView->m_wImgDisp2.Invalidate(FALSE);

	GetData(&m_setEtc);
	GetData2(&m_setEtc2);

	m_pView->SettingParam(m_setEtc);
	m_pView->SettingParam(m_setEtc2);
	UpdateData(FALSE);
}

void CEtcWindow::DeltaPos(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,double *v,double low,double high,double inc)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	pEdit->GetWindowText(buf);
    *v=atof(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		*v+=(-inc*pNMUpDown->iDelta);
		if(*v>high)	*v=high;
	}
	else {
		*v-=(inc*pNMUpDown->iDelta);
		if(*v<low) *v=low;
	}
	*pResult = 1;
	UpdateData(FALSE);
}

void CEtcWindow::DeltaPos(NMHDR* pNMHDR, LRESULT* pResult,CEdit *pEdit,int *v,int low,int high,int inc)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString buf;

	UpdateData(TRUE);    
	pEdit->GetWindowText(buf);
    *v=atoi(buf.GetBuffer(buf.GetLength()));

	if(pNMUpDown->iDelta<0){
		*v+=(-inc*pNMUpDown->iDelta);
		if(*v>high)	*v=high;
	}
	else {
		*v-=(inc*pNMUpDown->iDelta);
		if(*v<low) *v=low;
	}
	*pResult = 1;
	UpdateData(FALSE);
}

void CEtcWindow::OnDeltaposSpinRoffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaPos(pNMHDR,pResult,m_pwRT[0],&m_nROffset,-40.0,40.0,0.01);

	m_pView->m_wImgDisp2.SetOffset(m_nROffset,m_nTOffset);
	m_pView->m_wImgDisp2.Invalidate(FALSE);

	return;
}

void CEtcWindow::OnDeltaposSpinToffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaPos(pNMHDR,pResult,m_pwRT[1],&m_nTOffset,-40.0,40.0,0.01);

	m_pView->m_wImgDisp2.SetOffset(m_nROffset,m_nTOffset);
	m_pView->m_wImgDisp2.Invalidate(FALSE);

	return;
}

void CEtcWindow::OnButtonMastermode() 
{
	if(m_pwButtonMaster->GetCheck())
	{
		m_nWorkMode=1;
		m_pwButtonMaster->SetTextColor(RGB(255,255,0));
	}
	else
	{
		m_pwButtonMaster->SetTextColor(RGB(0,0,0));
		m_nWorkMode=0;
	}
}

void CEtcWindow::UpdateViewWindowData()
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
	GetData(&m_setEtc);
	GetData2(&m_setEtc2);

	m_pView->SettingParam(m_setEtc);
	m_pView->SettingParam(m_setEtc2);

	m_pView->m_wImgDisp3.Invalidate(FALSE);
	
	return;
}


void CEtcWindow::OnDeltaposSpinEtc1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaPos(pNMHDR,pResult,m_pwEditEx[0],&m_iPixel,0,640,1);
	UpdateViewWindowData();
	m_pView->m_wImgDisp3.Invalidate(FALSE);
	return;
}

void CEtcWindow::OnDeltaposSpinEtc2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaPos(pNMHDR,pResult,m_pwEditEx[1],&m_dDistance,0.0,20.0,0.01);
	UpdateViewWindowData();
	m_pView->m_wImgDisp3.Invalidate(FALSE);
	return;
}
void CEtcWindow::OnDeltaposSpinEtc3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaPos(pNMHDR,pResult,m_pwEditEx[2],&m_iDispMode,0,1,1);

	UpdateViewWindowData();
	m_pView->m_wImgDisp3.ChangePallete();
	m_pView->m_wImgDisp3.Invalidate(FALSE);
	return;
}
void CEtcWindow::OnDeltaposSpinEtc4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	DeltaPos(pNMHDR,pResult,m_pwEditEx[3],&m_iBThr,10,255,1);
 
	KIP_BinarizeAll(m_pView->m_wImgDisp3.m_pSrc,m_pView->m_wImgDisp3.m_pOut,m_pView->m_nWidth,m_pView->m_nHeight,1,m_iBThr);
	m_pView->m_wImgDisp3.ImgUpdate(m_pView->m_wImgDisp3.m_pOut);

	return;
}
void CEtcWindow::OnDeltaposSpinEtc5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
    RECT r;

	r.left=1;
	r.right=m_pView->m_nWidth-2;
	r.top=1;
	r.bottom=m_pView->m_nHeight-2;

	DeltaPos(pNMHDR,pResult,m_pwEditEx[4],&m_iEThr,1,255,1);
//	KIP_Smoothing(m_pView->m_wImgDisp3.m_pSrc,m_pView->m_wImgDisp3.m_pOut,m_pView->m_nWidth,m_pView->m_nHeight,1);
    m_pView->m_wImgDisp3.MorphImage(m_pView->m_wImgDisp3.m_pSrc,m_pView->m_wImgDisp3.m_pOut,r);
	memcpy(m_pView->m_wImgDisp3.m_pOut,m_pView->m_wImgDisp3.m_pSrc,m_pView->m_nWidth*m_pView->m_nHeight);
	KIP_BinarizeAll(m_pView->m_wImgDisp3.m_pSrc,m_pView->m_wImgDisp3.m_pOut,m_pView->m_nWidth,m_pView->m_nHeight,1,m_iEThr);
	m_pView->m_wImgDisp3.ImgUpdate(m_pView->m_wImgDisp3.m_pOut);
	return;
}

void CEtcWindow::OnDeltaposSpinEtc6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditEx[5],&m_dGTOffset,-5.0,5.0,0.001);
	return;
}

void CEtcWindow::OnDeltaposSpinEtc7(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditEx[6],&m_dGTShift,-5.0,5.0,0.001);
	return;
}

void CEtcWindow::OnDeltaposSpinEtc8(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditExAdd[0],&m_dGTShiftHigh,-5.0,5.0,0.001);
	return;
}
void CEtcWindow::OnCheckEtcRoi() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();

	if(m_pwButtonRoi->GetCheck())
	{
		m_pwButtonRoi->SetTextColor(RGB(255,255,0));
		m_pView->m_wImgDisp3.SetRoiFlag(1);
	}
	else
	{
		m_pwButtonRoi->SetTextColor(RGB(0,0,0));
		m_pView->m_wImgDisp3.SetRoiFlag(0);
	}
	m_pView->m_wImgDisp3.Invalidate(FALSE);
}
//////////////////////////////////////
void CEtcWindow::OnSelchangeCombo1() 
{
	CMainFrame *m_pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CHVSystemView *m_pView = (CHVSystemView *)m_pFrame->GetActiveView();
		
	m_iBcPort=m_pwCombo[0]->GetCurSel();

    m_pView->ReInitComport(m_iBcPort);
	
}


void CEtcWindow::OnDeltaposSpinEtc9(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditEx[7],&m_dGTShift2,-5.0,5.0,0.001);
	return;

}


void CEtcWindow::OnDeltaposSpinEtc10(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditExAdd[1],&m_dGTShiftHigh2,-5.0,5.0,0.001);
	return;
}




void CEtcWindow::OnDeltaposSpinEtc11(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditEx[8],&m_dGTShift3,-5.0,5.0,0.001);
	return;

}

void CEtcWindow::OnDeltaposSpinEtc12(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditExAdd[2],&m_dGTShiftHigh3,-5.0,5.0,0.001);
	return;
}

void CEtcWindow::OnDeltaposSpinEtc13(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditEx[9],&m_iBeamSize,0,20000,1);
	return;
}

void CEtcWindow::OnDeltaposSpinEtc14(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditExAdd[3],&m_iBeamSizeHigh,0,20000,1);
	return;
}
   


void CEtcWindow::OnDeltaposSpinEtc15(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditEx[10],&m_iApcOk,130,185,1);//140 230  사이.
	return;
}


void CEtcWindow::OnDeltaposSpinEtc16(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DeltaPos(pNMHDR,pResult,m_pwEditExAdd[4],&m_iApcOkHigh,185,240,1); //140 230  사이.
	return;
}