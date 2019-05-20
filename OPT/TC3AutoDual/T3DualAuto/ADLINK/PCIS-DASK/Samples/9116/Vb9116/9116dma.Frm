VERSION 4.00
Begin VB.Form dma9116 
   Appearance      =   0  'Flat
   AutoRedraw      =   -1  'True
   BackColor       =   &H00C0C0C0&
   Caption         =   "9116 DMA/INT"
   ClientHeight    =   6600
   ClientLeft      =   1170
   ClientTop       =   825
   ClientWidth     =   7665
   BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H0000FFFF&
   Height          =   7290
   Left            =   1110
   LinkTopic       =   "Form1"
   ScaleHeight     =   6600
   ScaleWidth      =   7665
   Top             =   195
   Width           =   7785
   Begin VB.Frame RetrgFrame 
      Caption         =   "Re-Trg Count (Dly/Post)"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   2280
      TabIndex        =   37
      Top             =   5160
      Width           =   2535
      Begin VB.TextBox txtRTrgCnt 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   240
         TabIndex        =   38
         Text            =   "1"
         Top             =   240
         Width           =   1155
      End
   End
   Begin VB.Frame Frame4 
      BackColor       =   &H00C0C0C0&
      Caption         =   "M Count (Pre/Mid)"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   35
      Top             =   5880
      Width           =   2055
      Begin VB.TextBox txtMCount 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   180
         TabIndex        =   36
         Text            =   "0"
         Top             =   240
         Width           =   1155
      End
   End
   Begin VB.Frame Frame3 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Post Count (Dly/Mid)"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   23
      Top             =   5160
      Width           =   2055
      Begin VB.TextBox txtPostCount 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   180
         TabIndex        =   24
         Text            =   "1024"
         Top             =   240
         Width           =   1155
      End
   End
   Begin VB.Frame Frame2 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Channel No."
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   21
      Top             =   4500
      Width           =   2055
      Begin VB.ComboBox cobChn 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   240
         Style           =   2  'Dropdown List
         TabIndex        =   22
         Top             =   240
         Width           =   1335
      End
      Begin VB.Label Label8 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "CH"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   240
         TabIndex        =   33
         Top             =   240
         Width           =   375
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Trigger"
      ForeColor       =   &H00FF0000&
      Height          =   1215
      Left            =   2280
      TabIndex        =   16
      Top             =   3840
      Width           =   2535
      Begin VB.ComboBox cobTrigPol 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   19
         Top             =   720
         Width           =   1515
      End
      Begin VB.ComboBox cobTrigMode 
         Appearance      =   0  'Flat
         Height          =   315
         ItemData        =   "9116DMA.frx":0000
         Left            =   900
         List            =   "9116DMA.frx":0002
         Style           =   2  'Dropdown List
         TabIndex        =   17
         Top             =   240
         Width           =   1515
      End
      Begin VB.Label Label6 
         Caption         =   "Polarity:"
         Height          =   255
         Left            =   120
         TabIndex        =   20
         Top             =   720
         Width           =   735
      End
      Begin VB.Label Label4 
         Caption         =   "Mode:"
         Height          =   195
         Left            =   120
         TabIndex        =   18
         Top             =   300
         Width           =   675
      End
   End
   Begin VB.CommandButton btnStart 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Start"
      Height          =   375
      Left            =   3000
      TabIndex        =   3
      Top             =   6120
      Width           =   1125
   End
   Begin VB.Frame frmData 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Scan Count"
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
      Height          =   1935
      Left            =   4920
      TabIndex        =   1
      Top             =   3840
      Width           =   2595
      Begin VB.TextBox cobScan 
         Enabled         =   0   'False
         Height          =   285
         Left            =   1440
         TabIndex        =   43
         Text            =   "Text1"
         Top             =   1080
         Width           =   975
      End
      Begin VB.ComboBox cobClkSrc 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   900
         Style           =   2  'Dropdown List
         TabIndex        =   15
         Top             =   240
         Width           =   1515
      End
      Begin VB.ComboBox cobRate 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   11
         Top             =   660
         Width           =   975
      End
      Begin VB.Label Label11 
         Caption         =   "ScanInterval"
         Height          =   255
         Left            =   120
         TabIndex        =   34
         Top             =   1080
         Width           =   1335
      End
      Begin VB.Label Label3 
         Caption         =   "Source:"
         Height          =   195
         Left            =   120
         TabIndex        =   14
         Top             =   300
         Width           =   675
      End
      Begin VB.Label Label2 
         Caption         =   "SampInterval"
         Height          =   195
         Left            =   120
         TabIndex        =   13
         Top             =   720
         Width           =   1215
      End
   End
   Begin VB.Frame dmaFrame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "DMA/INT Input(A/D)"
      ForeColor       =   &H00FF0000&
      Height          =   3735
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   7395
      Begin VB.HScrollBar scrollView 
         Height          =   255
         LargeChange     =   10
         Left            =   720
         TabIndex        =   31
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
      Begin VB.Shape shpColor 
         BackColor       =   &H00000080&
         BackStyle       =   1  'Opaque
         BorderColor     =   &H00000000&
         Height          =   255
         Index           =   7
         Left            =   6960
         Top             =   1740
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "7"
         Height          =   195
         Index           =   7
         Left            =   7200
         TabIndex        =   42
         Top             =   1800
         Width           =   135
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H00FF00FF&
         BackStyle       =   1  'Opaque
         BorderColor     =   &H00000000&
         Height          =   255
         Index           =   6
         Left            =   6960
         Top             =   1320
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "6"
         Height          =   195
         Index           =   6
         Left            =   7200
         TabIndex        =   41
         Top             =   1380
         Width           =   135
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H00FFFF00&
         BackStyle       =   1  'Opaque
         BorderColor     =   &H00000000&
         Height          =   255
         Index           =   5
         Left            =   6960
         Top             =   900
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "5"
         Height          =   195
         Index           =   5
         Left            =   7200
         TabIndex        =   40
         Top             =   960
         Width           =   135
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H0000FF00&
         BackStyle       =   1  'Opaque
         BorderColor     =   &H00000000&
         Height          =   255
         Index           =   4
         Left            =   6960
         Top             =   480
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "4"
         Height          =   195
         Index           =   4
         Left            =   7200
         TabIndex        =   39
         Top             =   540
         Width           =   135
      End
      Begin VB.Image TriggerPos 
         Height          =   120
         Left            =   6300
         Picture         =   "9116DMA.frx":0004
         Top             =   330
         Visible         =   0   'False
         Width           =   120
      End
      Begin VB.Label Label10 
         Caption         =   "3"
         Height          =   195
         Index           =   3
         Left            =   6720
         TabIndex        =   30
         Top             =   1800
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "2"
         Height          =   195
         Index           =   2
         Left            =   6720
         TabIndex        =   29
         Top             =   1380
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "1"
         Height          =   195
         Index           =   1
         Left            =   6720
         TabIndex        =   28
         Top             =   960
         Width           =   195
      End
      Begin VB.Label Label10 
         Caption         =   "0"
         Height          =   195
         Index           =   0
         Left            =   6720
         TabIndex        =   27
         Top             =   540
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H00008000&
         BackStyle       =   1  'Opaque
         Height          =   255
         Index           =   3
         Left            =   6480
         Top             =   1740
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H000000FF&
         BackStyle       =   1  'Opaque
         Height          =   255
         Index           =   2
         Left            =   6480
         Top             =   1320
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H00FF0000&
         BackStyle       =   1  'Opaque
         Height          =   255
         Index           =   1
         Left            =   6480
         Top             =   900
         Width           =   195
      End
      Begin VB.Shape shpColor 
         BackColor       =   &H0000FFFF&
         BackStyle       =   1  'Opaque
         BorderColor     =   &H00000000&
         Height          =   255
         Index           =   0
         Left            =   6480
         Top             =   480
         Width           =   195
      End
      Begin VB.Label lblLeft 
         Caption         =   "0"
         Height          =   195
         Left            =   660
         TabIndex        =   26
         Top             =   3120
         Width           =   795
      End
      Begin VB.Label lblRight 
         Alignment       =   1  'Right Justify
         Caption         =   "0"
         Height          =   195
         Left            =   5520
         TabIndex        =   25
         Top             =   3120
         Width           =   915
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-5.00"
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
         Caption         =   "-2.50"
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
         Caption         =   "2.50"
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
         Caption         =   "5.00"
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
      Height          =   375
      Left            =   4920
      TabIndex        =   32
      Top             =   6120
      Width           =   2595
   End
   Begin VB.Menu mnuExit 
      Caption         =   "E&xit!"
   End
End
Attribute VB_Name = "dma9116"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Const DISP_COUNT = 1000
Dim gnADChn As Long
Dim gnCount As Long
Dim gnClkDiv As Integer
Dim gnClkDiv2 As Integer
Dim count1 As Long
Dim ch_cnt As Integer
Dim gnBuffer(160000) As Integer
Dim dma_size As Long
Dim range As Integer
Dim ConfigCtrl As Integer
Dim TrigCtrl As Integer

Private Sub btnStart_Click()
  Dim result As Long
  Dim status As Byte
  Dim i As Long, k As Long
  Dim reTrgCnt As Integer, MCnt As Integer, PostCnt
  
  dma_size = CLng(cobDataSize.Text)
  sample_rate = 1
  ch_cnt = CLng(cobChn.ListIndex)
  ch_cnt = ch_cnt + 1
  range = AD_B_5_V
  btnStart.Enabled = False
  reTrgCnt = CInt(txtRTrgCnt.Text)
  MCnt = CInt(txtMCount.Text)
  PostCnt = CInt(txtPostCount.Text)
  Select Case cobTrigMode.ListIndex
    Case 0
      TrigCtrl = ai_type Or P9116_TRGMOD_SOFT
      reTrgCnt = 1
      MCnt = 0
      PostCnt = 0
    Case 1
      TrigCtrl = ai_type Or P9116_TRGMOD_POST Or P9116_AI_ReTrigEn
      MCnt = 0
      PostCnt = 0
      dma_size = dma_size * reTrgCnt
    Case 2
      TrigCtrl = ai_type Or P9116_TRGMOD_DELAY Or P9116_AI_ReTrigEn
      MCnt = 0
      dma_size = dma_size * reTrgCnt
    Case 3
      TrigCtrl = ai_type Or P9116_TRGMOD_PRE Or P9116_AI_MCounterEn
      reTrgCnt = 1
      PostCnt = 0
   Case 4
      TrigCtrl = ai_type Or P9116_TRGMOD_MIDL Or P9116_AI_MCounterEn
      reTrgCnt = 1
  End Select
  If CLng(cobClkSrc.ListIndex) = 1 Then
      TrigCtrl = TrigCtrl Or P9116_AI_ExtTimeBase
  End If
    
  If CLng(cobTrigPol.ListIndex) = 1 Then
      TrigCtrl = TrigCtrl Or P9116_AI_TrgNegative
  End If
 
  result = AI_9116_Config(card, 0, TrigCtrl, PostCnt, MCnt, reTrgCnt)
  result = AI_9116_CounterInterval(card, Val(cobScan.Text), (CLng(cobRate.ListIndex) + 1) * 240)
  status = 0
  result = AI_ContScanChannels(card, ch_cnt - 1, range, gnBuffer(0), dma_size, sample_rate, ASYNCH_OP)
    If result <> 0 Then
     MsgBox "Continuous AI Failed"
     End
  End If
  While status = 0
    result = AI_AsyncCheck(card, status, count1)
  Wend
  result = AI_AsyncClear(card, count1)
  txtXfer.Caption = "Total xfer: " & CStr(count1)
  scrollView.Value = 0
  scrollView.Max = Fix((count1 - 1) / DISP_COUNT)
  ShowData 0, Min(count1, DISP_COUNT)
  btnStart.Enabled = True
End Sub









Private Sub cobChn_Click()
  gnClkDiv = Val(cobRate.Text) * (CLng(cobChn.ListIndex) + 1)
  cobScan.Text = Str(gnClkDiv)
End Sub


Private Sub cobRate_Click()
  gnClkDiv = Val(cobRate.Text) * (CLng(cobChn.ListIndex) + 1)
  cobScan.Text = Str(gnClkDiv)
End Sub

Private Sub Form_Load()
    Dim result As Long
    Dim i As Integer

    SettingForm.Show 1
    If card < 0 Then
      MsgBox "Register card failed."
      End
    End If

    cobDataSize.AddItem "1000"
    cobDataSize.AddItem "2000"
    cobDataSize.AddItem "4000"
    cobDataSize.AddItem "8000"
    cobDataSize.AddItem "10000"
    cobDataSize.AddItem "20000"
    cobDataSize.ListIndex = 0

    cobChn.AddItem "CH0"
    cobChn.AddItem "CH0~CH1"
    cobChn.AddItem "CH0~CH2"
    cobChn.AddItem "CH0~CH3"
    cobChn.AddItem "CH0~CH4"
    cobChn.AddItem "CH0~CH5"
    cobChn.AddItem "CH0~CH6"
    cobChn.AddItem "CH0~CH7"
    cobChn.ListIndex = 0

    txtPostCount.Text = 500
    If ai_type = P9116_AI_DMA Then
     cobTrigMode.AddItem "Soft-Trigger"
     cobTrigMode.AddItem "Post-Trigger"
     cobTrigMode.AddItem "Delay-Trigger"
     cobTrigMode.AddItem "Pre-Trigger"
     cobTrigMode.AddItem "Middle-Trigger"
     cobTrigMode.ListIndex = 0
     txtMCount.Enabled = True
     Frame4.Enabled = True
    Else
     cobTrigMode.AddItem "Soft-Trigger"
     cobTrigMode.AddItem "Post-Trigger"
     cobTrigMode.AddItem "Delay-Trigger"
     cobTrigMode.ListIndex = 0
     txtMCount.Enabled = False
     Frame4.Enabled = False
   End If

    cobTrigPol.AddItem "Positive"
    cobTrigPol.AddItem "Negative"
    cobTrigPol.ListIndex = 0

    cobClkSrc.AddItem "Internal(24M)"
    cobClkSrc.AddItem "External"
    cobClkSrc.ListIndex = 0

    cobRate.AddItem "240"
    cobRate.AddItem "480"
    cobRate.AddItem "720"
    cobRate.AddItem "960"
    cobRate.AddItem "1200"
    cobRate.AddItem "1440"
    cobRate.AddItem "1680"
    cobRate.AddItem "1920"
    cobRate.AddItem "2160"
    cobRate.AddItem "2400"
    cobRate.ListIndex = 0
    gnClkDiv = 240
    cobScan.Text = cobRate.Text
    dma_size = 0
End Sub


Private Sub ShowData(ByVal startidx As Long, ByVal acq_count As Long)
    Dim i As Long, k As Long, n As Long
    Dim w As Long, h As Long
    Dim X As Long, Y As Long
    Dim last_x(8) As Long, last_y(8) As Long
    Dim samp As Integer, trig_pos As Long
    picScreen.Cls
    DrawTrigLevel -1, trigLevel
    lblLeft.Caption = CStr(CLng(scrollView.Value) * DISP_COUNT)
    lblRight.Caption = CStr(CLng(lblLeft) + DISP_COUNT - 1)
    w = picScreen.ScaleWidth
    h = picScreen.ScaleHeight

    For i = 0 To 7
      last_x(i) = -1
      last_y(i) = -1
    Next
    For i = 0 To acq_count - 1
        k = (startidx + i) Mod ch_cnt
        n = (startidx + i) Mod count1
        X = (i * w) / (DISP_COUNT - 1)
        samp = gnBuffer(n)
        'Y = h * (2047 - samp) / 4096
        Y = CInt((32767 - CLng(samp)) * h / 65535)
        If last_y(k) <> -1 Then
            picScreen.Line (last_x(k), last_y(k))-(X, Y), shpColor(k).BackColor
        End If
        last_x(k) = X
        last_y(k) = Y
    Next
    'Set trigger position
    Select Case cobTrigMode.ListIndex
      Case 1
        trig_pos = 0
      Case 2
        trig_pos = -1
      Case 3
        trig_pos = count1 - 1
      Case 4
        trig_pos = count1 - CLng(txtPostCount.Text) * ch_cnt
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
   If card >= 0 Then
      Release_Card (card)
   End If
End Sub

Private Sub Label7_Click()

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

Private Sub Text1_Change()

End Sub


