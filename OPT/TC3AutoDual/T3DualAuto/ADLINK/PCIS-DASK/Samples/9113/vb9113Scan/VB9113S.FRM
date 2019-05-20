VERSION 4.00
Begin VB.Form int9113 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-9113 AutoScan Interrupt Transfer"
   ClientHeight    =   4755
   ClientLeft      =   660
   ClientTop       =   1410
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
   Height          =   5160
   Left            =   600
   LinkTopic       =   "Form1"
   ScaleHeight     =   4755
   ScaleWidth      =   7665
   Top             =   1065
   Width           =   7785
   Begin VB.CommandButton btnExit 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Exit"
      Height          =   375
      Left            =   6360
      TabIndex        =   5
      Top             =   4080
      Width           =   1005
   End
   Begin VB.CommandButton btnStop 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Stop"
      Height          =   375
      Left            =   5280
      TabIndex        =   4
      Top             =   4080
      Width           =   1005
   End
   Begin VB.CommandButton btnStart 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Start"
      Height          =   375
      Left            =   4080
      TabIndex        =   3
      Top             =   4080
      Width           =   1005
   End
   Begin VB.Frame frmData 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Data Size"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Left            =   1920
      TabIndex        =   2
      Top             =   3960
      Width           =   1935
      Begin VB.ComboBox cobData 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   18
         Top             =   240
         Width           =   975
      End
      Begin VB.Label labData 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "samples"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Left            =   1155
         TabIndex        =   19
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
      Top             =   3960
      Width           =   1700
      Begin VB.ComboBox cobSample 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   17
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
         TabIndex        =   20
         Top             =   260
         Width           =   450
      End
   End
   Begin VB.Frame intFrame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Interrupt Input(A/D)"
      ForeColor       =   &H00FF0000&
      Height          =   3435
      Left            =   120
      TabIndex        =   0
      Top             =   420
      Width           =   7455
      Begin VB.Frame Frame1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Channel"
         ForeColor       =   &H00FF0000&
         Height          =   615
         Left            =   5880
         TabIndex        =   29
         Top             =   2100
         Width           =   1380
         Begin VB.ComboBox cobChn 
            Appearance      =   0  'Flat
            Height          =   315
            Left            =   100
            Style           =   2  'Dropdown List
            TabIndex        =   30
            Top             =   240
            Width           =   1200
         End
      End
      Begin VB.Frame frmRange 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Range"
         ForeColor       =   &H00FF0000&
         Height          =   615
         Left            =   5880
         TabIndex        =   8
         Top             =   2700
         Width           =   1380
         Begin VB.ComboBox cobRange 
            Appearance      =   0  'Flat
            Height          =   315
            Left            =   100
            Style           =   2  'Dropdown List
            TabIndex        =   16
            Top             =   240
            Width           =   1200
         End
      End
      Begin VB.Frame frmChannel 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         ForeColor       =   &H00FF0000&
         Height          =   1755
         Left            =   5880
         TabIndex        =   7
         Top             =   300
         Width           =   1380
         Begin VB.Label Label2 
            Caption         =   "7"
            Height          =   195
            Index           =   7
            Left            =   780
            TabIndex        =   28
            Top             =   1320
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "6"
            Height          =   195
            Index           =   6
            Left            =   780
            TabIndex        =   27
            Top             =   960
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "5"
            Height          =   195
            Index           =   5
            Left            =   780
            TabIndex        =   26
            Top             =   600
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "4"
            Height          =   195
            Index           =   4
            Left            =   780
            TabIndex        =   25
            Top             =   240
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "3"
            Height          =   195
            Index           =   3
            Left            =   120
            TabIndex        =   24
            Top             =   1320
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "2"
            Height          =   195
            Index           =   2
            Left            =   120
            TabIndex        =   23
            Top             =   960
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "1"
            Height          =   195
            Index           =   1
            Left            =   120
            TabIndex        =   22
            Top             =   600
            Width           =   135
         End
         Begin VB.Label Label2 
            Caption         =   "0"
            Height          =   195
            Index           =   0
            Left            =   120
            TabIndex        =   21
            Top             =   240
            Width           =   135
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H0000FF00&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   7
            Left            =   960
            Top             =   1320
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H00FFFF00&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   6
            Left            =   960
            Top             =   960
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H00FF00FF&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   5
            Left            =   960
            Top             =   600
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H000080FF&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   4
            Left            =   960
            Top             =   240
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H00008000&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   3
            Left            =   300
            Top             =   1320
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H00FF0000&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   2
            Left            =   300
            Top             =   960
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H000000FF&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   1
            Left            =   300
            Top             =   600
            Width           =   195
         End
         Begin VB.Shape Shape1 
            BackColor       =   &H0000FFFF&
            FillColor       =   &H0000FFFF&
            FillStyle       =   0  'Solid
            Height          =   255
            Index           =   0
            Left            =   300
            Top             =   240
            Width           =   195
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
         Caption         =   "Volt"
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
   Begin VB.Label Label3 
      Caption         =   "PCI-9113 Auto-scan Interrupt Transfer"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   255
      Left            =   1920
      TabIndex        =   31
      Top             =   120
      Width           =   3555
   End
End
Attribute VB_Name = "int9113"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim nSample As Long
Dim range_value(8) As Integer
Dim gnADChn As Integer
Dim Range As Integer
Dim gnCount As Long
Dim gnBuffer(8192 * 2) As Integer

Private Sub btnExit_Click()
  If card >= 0 Then Release_Card (card)
    End

End Sub



Private Sub btnStart_Click()
    Dim result As Integer
    Dim i As Integer
    
    picScreen.Cls
    result = AI_9113_Config(card, TRIG_INT_PACER)
    
    Range = range_value(cobRange.ListIndex)
    
    result = AI_ContScanChannels(card, gnADChn, Range, gnBuffer(0), gnCount, CDbl(nSample * 1000), SYNCH_OP)
    ShowData (gnCount)
End Sub

Private Sub btnStop_Click()
    picScreen.Cls

End Sub

Private Sub cobChn_Click()
    gnADChn = cobChn.ListIndex
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
    
    cobChn.AddItem "Ch0"
    cobChn.AddItem "Ch0~Ch1"
    cobChn.AddItem "Ch0~Ch2"
    cobChn.AddItem "Ch0~Ch3"
    cobChn.AddItem "Ch0~Ch4"
    cobChn.AddItem "Ch0~Ch5"
    cobChn.AddItem "Ch0~Ch6"
    cobChn.AddItem "Ch0~Ch7"

    cobChn.ListIndex = 0
    gnADChn = 0
    
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
End Sub


Private Sub ShowData(Count As Integer)
    Dim i, last, total As Integer
    Dim x1(0 To 7) As Long, y1(0 To 7) As Long
    Dim x2(0 To 7) As Long, y2(0 To 7) As Long
    Dim idx(0 To 7) As Integer, color(0 To 7) As Long
    Dim w, h, data As Long
    Dim ch As Integer
    
    last = Count / (gnADChn + 1)
    w = picScreen.ScaleWidth
    h = picScreen.ScaleHeight
    For i = 0 To 7
        idx(i) = 0
    Next
    color(0) = &HFFFF&
    color(1) = &HFF&
    color(2) = &HFF0000
    color(3) = &H8000&
    color(4) = &H80FF&
    color(5) = &HFF00FF
    color(6) = &HFFFF00
    color(7) = &HFF00&
    For i = 0 To Count - 1
        ch = (gnBuffer(2 * i + 1) And &HFF)
        If ch > 7 Then
         Exit For
        End If
        data = Fix(gnBuffer(2 * i) And &HFFF)
        x2(ch) = (idx(ch) * w) / last
        y2(ch) = CInt((4095 - CLng(data)) * h / 4095)
        If idx(ch) <> 0 Then
            picScreen.Line (x1(ch), y1(ch))-(x2(ch), y2(ch)), color(ch)
        End If
        x1(ch) = x2(ch)
        y1(ch) = y2(ch)
        idx(ch) = idx(ch) + 1
    Next
End Sub



Private Sub Form_Unload(Cancel As Integer)
  If card >= 0 Then Release_Card (card)
End Sub


