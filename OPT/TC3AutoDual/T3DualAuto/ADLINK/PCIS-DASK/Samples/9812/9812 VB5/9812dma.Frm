VERSION 5.00
Begin VB.Form dma9812 
   Appearance      =   0  'Flat
   AutoRedraw      =   -1  'True
   BackColor       =   &H00C0C0C0&
   Caption         =   "9812 DMA"
   ClientHeight    =   6390
   ClientLeft      =   1020
   ClientTop       =   1650
   ClientWidth     =   7665
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H0000FFFF&
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   6390
   ScaleWidth      =   7665
   Begin VB.Frame Frame4 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Card Type"
      ForeColor       =   &H00FF0000&
      Height          =   735
      Left            =   4920
      TabIndex        =   30
      Top             =   5040
      Width           =   2595
      Begin VB.ComboBox cobCardType 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   540
         Style           =   2  'Dropdown List
         TabIndex        =   31
         Top             =   300
         Width           =   1395
      End
   End
   Begin VB.Frame Frame3 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Post Trigger Count"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   28
      Top             =   5160
      Width           =   2055
      Begin VB.TextBox txtPostCount 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   180
         TabIndex        =   29
         Text            =   "500"
         Top             =   240
         Width           =   975
      End
   End
   Begin VB.Frame Frame2 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Channel Count"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   25
      Top             =   4500
      Width           =   2055
      Begin VB.ComboBox cobChn 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   180
         Style           =   2  'Dropdown List
         TabIndex        =   26
         Top             =   240
         Width           =   855
      End
      Begin VB.Label Label8 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "channels"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   1080
         TabIndex        =   27
         Top             =   300
         Width           =   795
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Trigger"
      ForeColor       =   &H00FF0000&
      Height          =   1935
      Left            =   2280
      TabIndex        =   17
      Top             =   3840
      Width           =   2535
      Begin VB.ComboBox cobTrigLevel 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   38
         Top             =   1500
         Width           =   975
      End
      Begin VB.ComboBox cobTrigPol 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   22
         Top             =   1080
         Width           =   1515
      End
      Begin VB.ComboBox cobTrigSrc 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   19
         Top             =   660
         Width           =   1515
      End
      Begin VB.ComboBox cobTrigMode 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   18
         Top             =   240
         Width           =   1515
      End
      Begin VB.Label Label7 
         Caption         =   "Level:"
         Height          =   195
         Left            =   120
         TabIndex        =   24
         Top             =   1560
         Width           =   735
      End
      Begin VB.Label Label6 
         Caption         =   "Polarity:"
         Height          =   195
         Left            =   120
         TabIndex        =   23
         Top             =   1140
         Width           =   735
      End
      Begin VB.Label Label5 
         Caption         =   "Source:"
         Height          =   195
         Left            =   120
         TabIndex        =   21
         Top             =   720
         Width           =   735
      End
      Begin VB.Label Label4 
         Caption         =   "Mode:"
         Height          =   195
         Left            =   120
         TabIndex        =   20
         Top             =   300
         Width           =   675
      End
   End
   Begin VB.CommandButton btnStart 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Start"
      Height          =   375
      Left            =   3240
      TabIndex        =   3
      Top             =   5940
      Width           =   1005
   End
   Begin VB.Frame frmData 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Data Size (in samples)"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   2
      Top             =   3840
      Width           =   2055
      Begin VB.ComboBox cobDataSize 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   180
         TabIndex        =   12
         Text            =   "cobDataSize"
         Top             =   240
         Width           =   1395
      End
   End
   Begin VB.Frame frmSample 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Clock"
      ForeColor       =   &H00FF0000&
      Height          =   1095
      Left            =   4920
      TabIndex        =   1
      Top             =   3840
      Width           =   2595
      Begin VB.ComboBox cobClkSrc 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   16
         Top             =   240
         Width           =   1515
      End
      Begin VB.ComboBox cobRate 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   11
         Top             =   660
         Width           =   975
      End
      Begin VB.Label Label3 
         Caption         =   "Source:"
         Height          =   195
         Left            =   180
         TabIndex        =   15
         Top             =   300
         Width           =   675
      End
      Begin VB.Label Label2 
         Caption         =   "Rate:"
         Height          =   195
         Left            =   180
         TabIndex        =   14
         Top             =   720
         Width           =   495
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "MHz"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   1980
         TabIndex        =   13
         Top             =   735
         Width           =   450
      End
   End
   Begin VB.Frame dmaFrame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "DMA Input(A/D)"
      ForeColor       =   &H00FF0000&
      Height          =   3735
      Left            =   120
      TabIndex        =   0
      Top             =   60
      Width           =   7395
      Begin VB.HScrollBar scrollView 
         Height          =   255
         LargeChange     =   10
         Left            =   720
         TabIndex        =   39
         Top             =   3360
         Width           =   5655
      End
      Begin VB.PictureBox picScreen 
         Appearance      =   0  'Flat
         AutoRedraw      =   -1  'True
         BackColor       =   &H00000000&
         ForeColor       =   &H80000008&
         Height          =   2595
         Left            =   720
         ScaleHeight     =   171
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   375
         TabIndex        =   4
         Top             =   480
         Width           =   5655
      End
      Begin VB.Image TriggerPos 
         Height          =   120
         Left            =   6300
         Picture         =   "9812DMA.frx":0000
         Top             =   330
         Visible         =   0   'False
         Width           =   120
      End
      Begin VB.Label Label10 
         Caption         =   "3"
         Height          =   195
         Index           =   3
         Left            =   6840
         TabIndex        =   37
         Top             =   1800
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "2"
         Height          =   195
         Index           =   2
         Left            =   6840
         TabIndex        =   36
         Top             =   1380
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "1"
         Height          =   195
         Index           =   1
         Left            =   6840
         TabIndex        =   35
         Top             =   960
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "0"
         Height          =   195
         Index           =   0
         Left            =   6840
         TabIndex        =   34
         Top             =   540
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H00008000&
         BackStyle       =   1  'Opaque
         Height          =   255
         Index           =   3
         Left            =   6600
         Top             =   1740
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H000000FF&
         BackStyle       =   1  'Opaque
         Height          =   255
         Index           =   2
         Left            =   6600
         Top             =   1320
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H00FF0000&
         BackStyle       =   1  'Opaque
         Height          =   255
         Index           =   1
         Left            =   6600
         Top             =   900
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H0000FFFF&
         BackStyle       =   1  'Opaque
         BorderColor     =   &H00000000&
         Height          =   255
         Index           =   0
         Left            =   6600
         Top             =   480
         Width           =   195
      End
      Begin VB.Label lblLeft 
         Caption         =   "0"
         Height          =   195
         Left            =   660
         TabIndex        =   33
         Top             =   3120
         Width           =   795
      End
      Begin VB.Label lblRight 
         Alignment       =   1  'Right Justify
         Caption         =   "0"
         Height          =   195
         Left            =   5520
         TabIndex        =   32
         Top             =   3120
         Width           =   915
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-1.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   4
         Left            =   45
         TabIndex        =   10
         Top             =   2940
         Width           =   600
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-0.50"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   3
         Left            =   45
         TabIndex        =   9
         Top             =   2280
         Width           =   600
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   2
         Left            =   45
         TabIndex        =   8
         Top             =   1680
         Width           =   600
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0.50"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   1
         Left            =   45
         TabIndex        =   7
         Top             =   1020
         Width           =   600
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "1.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   0
         Left            =   45
         TabIndex        =   6
         Top             =   420
         Width           =   600
      End
      Begin VB.Label labVots 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Volt"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   180
         TabIndex        =   5
         Top             =   195
         Width           =   435
      End
   End
   Begin VB.Label txtXfer 
      Alignment       =   1  'Right Justify
      ForeColor       =   &H00C00000&
      Height          =   255
      Left            =   4860
      TabIndex        =   40
      Top             =   6000
      Width           =   2595
   End
   Begin VB.Menu mnuExit 
      Caption         =   "E&xit!"
   End
End
Attribute VB_Name = "dma9812"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Const PCI9812 = 0
Const PCI9810 = 1
Const DISP_COUNT = 1000
Dim card As Integer
Dim slot As Integer
Dim bus As Integer
Dim card_number As Integer
'Dim hEnts As EventHandles
Dim gnADChn As Long
Dim gnCount As Long
Dim gnClkDiv As Integer
Dim trigLevel As Integer
Dim count1 As Long
Dim ch_cnt As Integer
Dim gnBuffer(10000000) As Integer
Dim dma_size As Long
'Dim chans(4) As Integer
Dim range As Integer

Private Sub btnStart_Click()
  Dim result As Long
  Dim Status As Byte
  Dim i As Long, k As Long
  
  card_number = 0
  If card < 0 Then
     btnStart.Enabled = False
     card = Register_Card(PCI_9812, 0)
     If card < 0 Then
        MsgBox "Register Card Failed"
     End
     End If
  End If
  dma_size = CLng(cobDataSize.Text)
  sample_rate = (40 * 1000000) / gnClkDiv
  ch_cnt = CLng(cobChn.Text)
  'For i = 0 To ch_cnt - 1
    'chans(i) = i  'ascending
    range = AD_B_1_V
  'Next i
  btnStart.Enabled = False
  result = AI_9812_Config(card, CLng(cobTrigMode.ListIndex), CLng(cobTrigSrc.ListIndex) * 8, CLng(cobTrigPol.ListIndex) * 64, P9812_AD2_GT_PCI Or (CLng(cobClkSrc.ListIndex) * 256), CLng(cobTrigLevel.Text), CInt(txtPostCount.Text))
  'result = AI_ContReadMultiChannels(card, ch_cnt, chans(0), ranges(0), gnBuffer(0), dma_size, sample_rate, ASYNCH_OP)
  Status = 0
  result = AI_ContScanChannels(card, ch_cnt - 1, range, gnBuffer(0), dma_size, sample_rate, ASYNCH_OP)
  
  While Status = 0
    result = AI_AsyncCheck(card, Status, count1)
  Wend
  result = AI_AsyncClear(card, count1)
  txtXfer.Caption = "Total xfer: " & CStr(count1)
  scrollView.Value = 0
  scrollView.Max = Fix((count1 - 1) / DISP_COUNT)
  ShowData 0, Min(count1, DISP_COUNT)
  btnStart.Enabled = True
End Sub







Private Sub cobRate_Click()
  gnClkDiv = CInt(CSng(40) / Val(cobRate.Text))
End Sub


Private Sub cobTrigLevel_Click()
  DrawTrigLevel trigLevel, CInt(cobTrigLevel.Text)
  trigLevel = CInt(cobTrigLevel.Text)
End Sub


Private Sub Form_Load()
    Dim result As Long
    Dim i As Integer

    cobDataSize.AddItem "2000"
    cobDataSize.AddItem "4000"
    cobDataSize.AddItem "8000"
    cobDataSize.AddItem "20000"
    cobDataSize.AddItem "40000"
    cobDataSize.AddItem "80000"
    cobDataSize.ListIndex = 0

    cobChn.AddItem "1"
    cobChn.AddItem "2"
    cobChn.AddItem "4"
    cobChn.ListIndex = 0

    txtPostCount.Text = 500

    cobTrigMode.AddItem "Soft-Trigger"
    cobTrigMode.AddItem "Post-Trigger"
    cobTrigMode.AddItem "Pre-Trigger"
    cobTrigMode.AddItem "Delay-Trigger"
    cobTrigMode.AddItem "Middle-Trigger"
    cobTrigMode.ListIndex = 0

    cobTrigSrc.AddItem "Channel-0"
    cobTrigSrc.AddItem "Channel-1"
    cobTrigSrc.AddItem "Channel-2"
    cobTrigSrc.AddItem "Channel-3"
    cobTrigSrc.AddItem "Digital"
    cobTrigSrc.ListIndex = 0

    cobTrigPol.AddItem "Positive"
    cobTrigPol.AddItem "Negative"
    cobTrigPol.ListIndex = 0

    For i = 0 To 31
        cobTrigLevel.AddItem i * 8
    Next
    cobTrigLevel.AddItem 255
    trigLevel = -1
    cobTrigLevel.ListIndex = 16  '128, 0V
    trigLevel = 128

    cobClkSrc.AddItem "Internal"
    cobClkSrc.AddItem "Ext Sin"
    cobClkSrc.AddItem "Ext Square"
    cobClkSrc.ListIndex = 0

    cobRate.AddItem "0.5"
    cobRate.AddItem "1"
    cobRate.AddItem "2"
    cobRate.AddItem "5"
    cobRate.AddItem "10"
    cobRate.AddItem "20"
    cobRate.ListIndex = 5
    gnClkDiv = 2

    cobCardType.AddItem "9812"
    cobCardType.AddItem "9810"
    cobCardType.ListIndex = PCI9812

    'result = W_9812_Initial(CLng(0), op_base_addr, pt_base_addr, irq_no, pci_master)
    'memID = -1
    card = -1
    dma_size = 0
End Sub


Private Sub ShowData(ByVal startidx As Long, ByVal acq_count As Long)
    Dim i As Long, k As Long, n As Long
    Dim w As Long, h As Long
    Dim X As Long, Y As Long
    Dim last_x(3) As Long, last_y(3) As Long
    Dim samp As Integer, trig_pos As Long
    picScreen.Cls
    DrawTrigLevel -1, trigLevel
    lblLeft.Caption = CStr(CLng(scrollView.Value) * DISP_COUNT)
    lblRight.Caption = CStr(CLng(lblLeft) + DISP_COUNT - 1)
    w = picScreen.ScaleWidth
    h = picScreen.ScaleHeight

    For i = 0 To 3
      last_x(i) = -1
      last_y(i) = -1
    Next
    For i = 0 To acq_count - 1
        k = i Mod ch_cnt
        n = (startidx + i) Mod count1
        X = (i * w) / (DISP_COUNT - 1)
        samp = gnBuffer(n)
        If cobCardType.ListIndex = PCI9812 Then
            samp = samp / 16
            Y = h * (2047 - samp) / 4096
        Else  'PCI9810
            samp = samp / 64
            Y = h * (511 - samp) / 1024
        End If
        If last_y(k) <> -1 Then
            picScreen.Line (last_x(k), last_y(k))-(X, Y), shpColor(k).BackColor
        End If
        last_x(k) = X
        last_y(k) = Y
    Next
    'Set trigger position
    Select Case cobTrigMode.ListIndex
      Case P9812_TRGMOD_POST
        trig_pos = 0
      Case P9812_TRGMOD_PRE
        trig_pos = count1 - 1
      Case P9812_TRGMOD_DELAY
        trig_pos = -1
      Case P9812_TRGMOD_MIDL
        trig_pos = count1 - CLng(txtPostCount.Text) * CLng(cobChn.Text)
    End Select
    If CLng(lblLeft) <= trig_pos And CLng(lblRight) >= trig_pos Then
      TriggerPos.Visible = True
      TriggerPos.Left = (picScreen.Left - 60) + ((trig_pos - CLng(lblLeft)) * w * Screen.TwipsPerPixelX) / (DISP_COUNT - 1)
    Else
      TriggerPos.Visible = False
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
  Dim result As Long
  'If memID >= 0 Then
   ' result = W_9812_Free_DMA_Mem(memID)
   If card >= 0 Then
      Release_Card (card)
   End If
End Sub

Private Sub mnuExit_Click()
  If card >= 0 Then Release_Card (card)
  End
End Sub

Private Sub scrollView_Change()
  Dim idx As Long
  idx = CLng(scrollView.Value) * DISP_COUNT
  ShowData idx Mod count1, Min(count1 - idx, DISP_COUNT)
End Sub

Private Sub spinPostCount_SpinDown()
  If Val(txtPostCount.Text) >= 500 Then txtPostCount.Text = Val(txtPostCount.Text) - 500
End Sub

Private Sub spinPostCount_SpinUp()
  If Val(txtPostCount.Text) <= 3500 Then txtPostCount.Text = Val(txtPostCount.Text) + 500
End Sub











Private Sub DrawTrigLevel(ByVal old_level As Integer, ByVal level As Integer)
    Dim w As Long, h As Long
    Dim Y As Long

    w = picScreen.ScaleWidth
    h = picScreen.ScaleHeight
    If old_level <> -1 Then
        Y = h * (255 - old_level) / 255
        picScreen.Line (0, Y)-(w, Y), &H0
    End If
    Y = h * (255 - level) / 255
    picScreen.Line (0, Y)-(w, Y), &H909090
End Sub

Private Function Min(ByVal v1 As Long, ByVal v2 As Long) As Long
  If v1 < v2 Then
    Min = v1
  Else
    Min = v2
  End If
End Function

