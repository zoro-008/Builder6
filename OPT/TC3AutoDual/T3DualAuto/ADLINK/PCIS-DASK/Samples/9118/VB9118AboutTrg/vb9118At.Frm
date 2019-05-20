VERSION 4.00
Begin VB.Form Atrg9118 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "9118  About Trigger DMA"
   ClientHeight    =   5460
   ClientLeft      =   1800
   ClientTop       =   1770
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
   Height          =   5865
   Left            =   1740
   LinkTopic       =   "Form1"
   ScaleHeight     =   5460
   ScaleWidth      =   7665
   Top             =   1425
   Width           =   7785
   Begin VB.Frame Frame1 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Post Count"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   2160
      TabIndex        =   20
      Top             =   3840
      Width           =   2175
      Begin VB.TextBox postCount 
         Height          =   285
         Left            =   120
         TabIndex        =   22
         Text            =   "1024"
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label labData 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Samples"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   1
         Left            =   1320
         TabIndex        =   21
         Top             =   240
         Width           =   735
      End
   End
   Begin VB.CommandButton btnExit 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Exit"
      Height          =   500
      Left            =   6240
      TabIndex        =   4
      Top             =   4680
      Width           =   1000
   End
   Begin VB.CommandButton btnStart 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Start"
      Height          =   500
      Left            =   4800
      TabIndex        =   3
      Top             =   4680
      Width           =   1000
   End
   Begin VB.Frame frmData 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Buffer Size"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   4680
      TabIndex        =   2
      Top             =   3840
      Width           =   1935
      Begin VB.ComboBox cobData 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   15
         Top             =   240
         Width           =   975
      End
      Begin VB.Label labData 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Samples"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   0
         Left            =   1155
         TabIndex        =   16
         Top             =   255
         Width           =   720
      End
   End
   Begin VB.Frame frmSample 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Sampling Rate"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   120
      TabIndex        =   1
      Top             =   3840
      Width           =   1700
      Begin VB.ComboBox cobSample 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   14
         Top             =   240
         Width           =   975
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "KHz"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   1150
         TabIndex        =   17
         Top             =   260
         Width           =   450
      End
   End
   Begin VB.Frame dmaFrame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "DMA Input(A/D)"
      ForeColor       =   &H00FF0000&
      Height          =   3375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7455
      Begin VB.Frame Card_Frame 
         Caption         =   "Card Number"
         ForeColor       =   &H00FF0000&
         Height          =   615
         Left            =   5880
         TabIndex        =   18
         Top             =   2520
         Width           =   1455
         Begin VB.ComboBox Card_num 
            Height          =   315
            Left            =   120
            TabIndex        =   19
            Text            =   "Number"
            Top             =   240
            Width           =   1215
         End
      End
      Begin VB.Frame frmChannel 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "AD Channel"
         ForeColor       =   &H00FF0000&
         Height          =   1755
         Left            =   5880
         TabIndex        =   6
         Top             =   480
         Width           =   1380
         Begin VB.OptionButton optChn 
            Caption         =   "7"
            Height          =   255
            Index           =   7
            Left            =   720
            TabIndex        =   30
            Top             =   1260
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "6"
            Height          =   255
            Index           =   6
            Left            =   720
            TabIndex        =   29
            Top             =   960
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "5"
            Height          =   255
            Index           =   5
            Left            =   720
            TabIndex        =   28
            Top             =   660
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "4"
            Height          =   255
            Index           =   4
            Left            =   720
            TabIndex        =   27
            Top             =   360
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "3"
            Height          =   255
            Index           =   3
            Left            =   180
            TabIndex        =   26
            Top             =   1260
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "2"
            Height          =   255
            Index           =   2
            Left            =   180
            TabIndex        =   25
            Top             =   960
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "1"
            Height          =   255
            Index           =   1
            Left            =   180
            TabIndex        =   24
            Top             =   660
            Width           =   435
         End
         Begin VB.OptionButton optChn 
            Caption         =   "0"
            Height          =   255
            Index           =   0
            Left            =   180
            TabIndex        =   23
            Top             =   360
            Value           =   -1  'True
            Width           =   435
         End
      End
      Begin VB.PictureBox picScreen 
         Appearance      =   0  'Flat
         AutoRedraw      =   -1  'True
         BackColor       =   &H00000000&
         ForeColor       =   &H80000008&
         Height          =   2700
         Left            =   720
         ScaleHeight     =   178
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   331
         TabIndex        =   5
         Top             =   480
         Width           =   5000
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-5.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   4
         Left            =   120
         TabIndex        =   13
         Top             =   3045
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
         Left            =   120
         TabIndex        =   12
         Top             =   2400
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
         Left            =   120
         TabIndex        =   11
         Top             =   1740
         Width           =   615
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2.50"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   10
         Top             =   1065
         Width           =   615
      End
      Begin VB.Label labValue 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   9
         Top             =   420
         Width           =   615
      End
      Begin VB.Label labVots 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Vots"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   200
         Width           =   495
      End
      Begin VB.Label labSignal 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Signals"
         ForeColor       =   &H0000FFFF&
         Height          =   300
         Left            =   2800
         TabIndex        =   7
         Top             =   200
         Width           =   780
      End
   End
End
Attribute VB_Name = "Atrg9118"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim gnADChn As Long
Dim gnCount As Long
Dim gnC2 As Long
Dim gnBuffer(4000) As Integer
Dim card As Integer
Dim nRate As Long
Dim range As Integer
Dim card_number As Integer
Dim post_cnt As Long
Private Sub btnExit_Click()
    If card >= 0 Then Release_Card (card)
    End
End Sub

Private Sub btnStart_Click()
    Dim result As Integer
    Dim access_cnt As Long
    Dim fstop As Byte
    
    If card < 0 Then
        card_num.Enabled = False
        card = Register_Card(PCI_9118DG, card_number)
        If card < 0 Then
           MsgBox "Register Card Failed"
        End
        End If
    End If
    fstop = 0
    result = AI_9118_Config(card, P9118_AI_BiPolar Or P9118_AI_SingEnded, P9118_AI_DtrgPositive Or P9118_AI_EtrgPositive Or P9118_AI_AboutTrgEn, 0, post_cnt)
    result = AI_AsyncDblBufferMode(card, 0)
    result = AI_ContReadChannel(card, gnADChn, range, gnBuffer(0), gnCount, CSng(nRate * 1000), ASYNCH_OP)
    While fstop = 0
        result = AI_AsyncCheck(card, fstop, access_cnt)
    Wend
    result = AI_AsyncClear(card, access_cnt)
    btnStart.Enabled = True
    ShowData (access_cnt)
End Sub


Private Sub card_DragDrop(Source As Control, X As Single, Y As Single)

End Sub

Private Sub card_num_Click()
    card_number = Val(Trim$(card_num))
End Sub


Private Sub cobData_Click()
    gnCount = Val(Trim$(cobData))
End Sub

Private Sub cobSample_Click()
    nRate = Val(Trim$(cobSample))
End Sub

Private Sub Command1_Click()

End Sub

Private Sub Form_Load()
    Dim bus As Integer, slot As Integer
    
    card_num.AddItem "0"
    card_num.AddItem "1"
    card_num.AddItem "2"
    card_num.AddItem "3"
    card_num.AddItem "4"
    card_num.AddItem "5"
    card_num.AddItem "6"
    card_num.AddItem "7"
    card_num.AddItem "8"
    card_num.AddItem "9"
    card_num.AddItem "10"
    card_num.AddItem "11"
    card_num.AddItem "12"
    card_num.AddItem "13"
    card_num.AddItem "14"
    card_num.AddItem "15"
    
    card_num.ListIndex = 0
    
    cobData.AddItem "500"
    cobData.AddItem "1000"
    cobData.AddItem "2000"
    cobData.AddItem "3000"
    cobData.AddItem "4000"

    cobData.ListIndex = 0

    cobSample.AddItem "10"
    cobSample.AddItem "20"
    cobSample.AddItem "30"
    cobSample.AddItem "40"
    cobSample.AddItem "50"
    cobSample.AddItem "70"
    cobSample.AddItem "90"
    cobSample.AddItem "100"

    cobSample.ListIndex = 0

    gnADChn = 0
    card = -1
    post_cnt = 1024
    'Range values
    range = AD_B_5_V
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If card >= 0 Then Release_Card (card)
End Sub


Private Sub optChn_Click(Index As Integer)
    gnADChn = Index
End Sub

Private Sub ShowData(count As Integer)
    Dim i As Integer, last As Integer, total As Integer
    Dim x1 As Integer, y1 As Integer, x2 As Integer, y2 As Integer
    Dim w As Long, h As Long, data As Long
    
    picScreen.Cls
    last = count
    w = picScreen.ScaleWidth
    h = picScreen.ScaleHeight
    'gnBuffer contains 16-bit data, the 4 LSB is channel#
    'VB's integer is signed integer, its range is -32768 to 32767
    'We change the range to 0 to 65535
    If gnBuffer(0) >= 0 Then
        data = gnBuffer(0)
    Else
        data = gnBuffer(0) + 65536
    End If
    x1 = (last * w) / count
    y1 = h - ((((data And &HFFF0) / 16) * h) / 4096)
    last = last - 1
    
    While (last > 0)
        If gnBuffer(count - last) >= 0 Then
            data = gnBuffer(count - last)
        Else
            data = gnBuffer(count - last) + 65536
        End If
        x2 = (last * w) / count
        y2 = h - ((((data And &HFFF0) / 16) * h) / 4096)
        last = last - 1
        picScreen.Line (x1, y1)-(x2, y2), &HFFFF&
        x1 = x2
        y1 = y2
    Wend
End Sub



Private Sub postCount_Change()
  post_cnt = Val(Trim$(postCount))
End Sub


