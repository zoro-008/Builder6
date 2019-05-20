VERSION 4.00
Begin VB.Form int9113 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "9113 INT"
   ClientHeight    =   4980
   ClientLeft      =   1365
   ClientTop       =   1800
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
   Height          =   5385
   Left            =   1305
   LinkTopic       =   "Form1"
   ScaleHeight     =   4980
   ScaleWidth      =   7665
   Top             =   1455
   Width           =   7785
   Begin VB.CommandButton btnExit 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Exit"
      Height          =   500
      Left            =   3720
      TabIndex        =   5
      Top             =   4320
      Width           =   1000
   End
   Begin VB.CommandButton btnStop 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Stop"
      Height          =   500
      Left            =   2520
      TabIndex        =   4
      Top             =   4320
      Width           =   1000
   End
   Begin VB.CommandButton btnStart 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Start"
      Height          =   500
      Left            =   1320
      TabIndex        =   3
      Top             =   4320
      Width           =   1000
   End
   Begin VB.Frame frmData 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Data Size"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   1920
      TabIndex        =   2
      Top             =   3500
      Width           =   1935
      Begin VB.ComboBox cobData 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   17
         Top             =   240
         Width           =   975
      End
      Begin VB.Label labData 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Records"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   1155
         TabIndex        =   18
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
      Top             =   3500
      Width           =   1700
      Begin VB.ComboBox cobSample 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   16
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
         TabIndex        =   19
         Top             =   260
         Width           =   450
      End
   End
   Begin VB.Frame intFrame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Interrupt Input(A/D)"
      ForeColor       =   &H00FF0000&
      Height          =   3375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7455
      Begin VB.Frame frmRange 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Range"
         ForeColor       =   &H00FF0000&
         Height          =   615
         Left            =   5880
         TabIndex        =   8
         Top             =   2520
         Width           =   1380
         Begin VB.ComboBox cobRange 
            Appearance      =   0  'Flat
            Height          =   315
            Left            =   100
            Style           =   2  'Dropdown List
            TabIndex        =   20
            Top             =   240
            Width           =   1200
         End
      End
      Begin VB.Frame frmChannel 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "AD Channel"
         ForeColor       =   &H00FF0000&
         Height          =   1755
         Left            =   5880
         TabIndex        =   7
         Top             =   240
         Width           =   1380
         Begin VB.OptionButton optChn 
            Caption         =   "7"
            Height          =   315
            Index           =   7
            Left            =   720
            TabIndex        =   28
            Top             =   1200
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "6"
            Height          =   315
            Index           =   6
            Left            =   720
            TabIndex        =   27
            Top             =   900
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "5"
            Height          =   315
            Index           =   5
            Left            =   720
            TabIndex        =   26
            Top             =   600
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "4"
            Height          =   315
            Index           =   4
            Left            =   720
            TabIndex        =   25
            Top             =   300
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "3"
            Height          =   315
            Index           =   3
            Left            =   180
            TabIndex        =   24
            Top             =   1200
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "2"
            Height          =   315
            Index           =   2
            Left            =   180
            TabIndex        =   23
            Top             =   900
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "1"
            Height          =   315
            Index           =   1
            Left            =   180
            TabIndex        =   22
            Top             =   600
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "0"
            Height          =   315
            Index           =   0
            Left            =   180
            TabIndex        =   21
            Top             =   300
            Value           =   -1  'True
            Width           =   495
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
         TabIndex        =   6
         Top             =   480
         Width           =   5000
      End
      Begin VB.Label labValue 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-5.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   4
         Left            =   100
         TabIndex        =   15
         Top             =   3050
         Width           =   600
      End
      Begin VB.Label labValue 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-2.50"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   3
         Left            =   100
         TabIndex        =   14
         Top             =   2400
         Width           =   600
      End
      Begin VB.Label labValue 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   2
         Left            =   100
         TabIndex        =   13
         Top             =   1740
         Width           =   600
      End
      Begin VB.Label labValue 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2.50"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   1
         Left            =   100
         TabIndex        =   12
         Top             =   1070
         Width           =   600
      End
      Begin VB.Label labValue 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   0
         Left            =   100
         TabIndex        =   11
         Top             =   420
         Width           =   600
      End
      Begin VB.Label labVots 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Vots"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   120
         TabIndex        =   10
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
         TabIndex        =   9
         Top             =   200
         Width           =   780
      End
   End
End
Attribute VB_Name = "int9113"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim gnADChn As Integer
Dim gnCount As Long
Dim nSample As Long
Dim range_value(8) As Integer
Dim gnBuffer(16384) As Integer
Dim bb(2048) As Integer

Private Sub btnExit_Click()
  If card >= 0 Then Release_Card (card)
    End
End Sub



Private Sub btnStart_Click()
    Dim result As Integer
    picScreen.Cls
    result = AI_9113_Config(card, TRIG_INT_PACER)
    result = AI_ContReadChannel(card, gnADChn, range_value(cobRange.ListIndex), gnBuffer(0), gnCount, CDbl(nSample * 1000), SYNCH_OP)
    ShowData (gnCount)
End Sub

Private Sub btnStop_Click()
    picScreen.Cls

End Sub

Private Sub cobData_Click()
    gnCount = Val(Trim$(cobData))
End Sub


Private Sub cobRange_Click()
Select Case cobRange.ListIndex
  Case 0
    labValue(0).Caption = "10.0"
    labValue(1).Caption = "5.0"
    labValue(2).Caption = "0.0"
    labValue(3).Caption = "-5.0"
    labValue(4).Caption = "-10.0"
  Case 1
    labValue(0).Caption = "1.0"
    labValue(1).Caption = "0.5"
    labValue(2).Caption = "0.0"
    labValue(3).Caption = "-0.5"
    labValue(4).Caption = "-1.0"
  Case 2
    labValue(0).Caption = "0.10"
    labValue(1).Caption = "0.05"
    labValue(2).Caption = "0.00"
    labValue(3).Caption = "-0.05"
    labValue(4).Caption = "-0.10"
  Case 3
    labValue(0).Caption = "5.0"
    labValue(1).Caption = "2.5"
    labValue(2).Caption = "0.0"
    labValue(3).Caption = "-2.5"
    labValue(4).Caption = "-5.0"
  Case 4
    labValue(0).Caption = "0.50"
    labValue(1).Caption = "0.25"
    labValue(2).Caption = "0.00"
    labValue(3).Caption = "-0.25"
    labValue(4).Caption = "-0.50"
  Case 5
    labValue(0).Caption = "0.050"
    labValue(1).Caption = "0.025"
    labValue(2).Caption = "0.000"
    labValue(3).Caption = "-0.025"
    labValue(4).Caption = "-0.050"
  Case 6
    labValue(0).Caption = "10.0"
    labValue(1).Caption = "7.50"
    labValue(2).Caption = "5.00"
    labValue(3).Caption = "2.50"
    labValue(4).Caption = "0.00"
  Case 7
    labValue(0).Caption = "1.00"
    labValue(1).Caption = "0.75"
    labValue(2).Caption = "0.50"
    labValue(3).Caption = "0.25"
    labValue(4).Caption = "0.00"
  Case 8
    labValue(0).Caption = "0.100"
    labValue(1).Caption = "0.075"
    labValue(2).Caption = "0.050"
    labValue(3).Caption = "0.025"
    labValue(4).Caption = "0.000"
  End Select
End Sub

Private Sub cobSample_Click()
    nSample = Val(Trim$(cobSample))
End Sub

Private Sub Form_Load()
    Dim result As Integer
    
    SettingForm.Show 1
    If card < 0 Then
      MsgBox "Register card failed."
      End
    End If

    gnC1 = 10
    cobRange.AddItem "+/- 10V"
    cobRange.AddItem "+/- 1V"
    cobRange.AddItem "+/- 0.1V"
    cobRange.AddItem "+/- 5V"
    cobRange.AddItem "+/- 0.5V"
    cobRange.AddItem "+/- 0.05V"
    cobRange.AddItem "0~10V"
    cobRange.AddItem "0~1V"
    cobRange.AddItem "0~0.1V"

    cobRange.ListIndex = 0
    
    range_value(0) = 1
    range_value(1) = 10
    range_value(2) = 11
    range_value(3) = 2
    range_value(4) = 7
    range_value(5) = 8
    range_value(6) = 15
    range_value(7) = 19
    range_value(8) = 20
    
    cobData.AddItem "1024"
    cobData.AddItem "2048"
    cobData.AddItem "4096"
    cobData.AddItem "6144"
    cobData.AddItem "8192"

    cobData.ListIndex = 0

    cobSample.AddItem "5"
    cobSample.AddItem "10"
    cobSample.AddItem "15"
    cobSample.AddItem "20"
    cobSample.AddItem "25"

    cobSample.ListIndex = 0

    gnADChn = 0

End Sub

Private Sub Form_Unload(Cancel As Integer)
  If card >= 0 Then Release_Card (card)
End Sub


Private Sub optChn_Click(Index As Integer)
    gnADChn = Index
End Sub

Private Sub ShowData(Count As Integer)
    Dim i As Integer
    Dim x1, y1, x2, y2 As Integer
    Dim w, h, data As Integer
    
    w = picScreen.ScaleWidth
    h = picScreen.ScaleHeight
    
    data = Fix(gnBuffer(0) And &HFFF)
    x1 = 0
    y1 = CInt((4095 - CLng(data)) * h / 4095)
    For i = 1 To Count - 1
        data = Fix(gnBuffer(i * 2) And &HFFF)
        x2 = (i * w) / Count
        y2 = CInt((4095 - CLng(data)) * h / 4095)
        picScreen.Line (x1, y1)-(x2, y2), &HFFFF&
        x1 = x2
        y1 = y2
    Next
End Sub


