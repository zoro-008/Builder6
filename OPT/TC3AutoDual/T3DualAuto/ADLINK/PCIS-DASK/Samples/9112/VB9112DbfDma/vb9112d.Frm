VERSION 4.00
Begin VB.Form dma9112 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "9112 Double-Buffered DMA"
   ClientHeight    =   4980
   ClientLeft      =   1080
   ClientTop       =   1620
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
   Left            =   1020
   LinkTopic       =   "Form1"
   ScaleHeight     =   4980
   ScaleWidth      =   7665
   Top             =   1275
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
      Enabled         =   0   'False
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
      Caption         =   "Buffer Size"
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
         TabIndex        =   18
         Top             =   240
         Width           =   975
      End
      Begin VB.Label labData 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Samples"
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
      Top             =   3500
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
      Begin VB.Frame Frame1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Range"
         ForeColor       =   &H00FF0000&
         Height          =   615
         Left            =   5880
         TabIndex        =   8
         Top             =   2640
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
            Top             =   1260
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "6"
            Height          =   315
            Index           =   6
            Left            =   720
            TabIndex        =   27
            Top             =   960
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "5"
            Height          =   315
            Index           =   5
            Left            =   720
            TabIndex        =   26
            Top             =   660
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "4"
            Height          =   315
            Index           =   4
            Left            =   720
            TabIndex        =   25
            Top             =   360
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "3"
            Height          =   315
            Index           =   3
            Left            =   120
            TabIndex        =   24
            Top             =   1260
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "2"
            Height          =   315
            Index           =   2
            Left            =   120
            TabIndex        =   23
            Top             =   960
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "1"
            Height          =   315
            Index           =   1
            Left            =   120
            TabIndex        =   22
            Top             =   660
            Width           =   495
         End
         Begin VB.OptionButton optChn 
            Caption         =   "0"
            Height          =   315
            Index           =   0
            Left            =   120
            TabIndex        =   21
            Top             =   360
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
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-5.00"
         ForeColor       =   &H0000FFFF&
         Height          =   255
         Index           =   4
         Left            =   120
         TabIndex        =   15
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
         TabIndex        =   14
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
         TabIndex        =   13
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
         TabIndex        =   12
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
         TabIndex        =   11
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
Attribute VB_Name = "dma9112"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim gnADChn As Long
Dim gnCount As Long
Dim gnC2 As Long
Dim gnBuffer(4000) As Integer
Dim nRate As Long
Dim range(8) As Integer
Dim stop_flag As Integer

Private Sub btnExit_Click()
    If card >= 0 Then Release_Card (card)
    End
End Sub

Private Sub btnStart_Click()
    Dim result As Integer
    Dim half_ready As Byte
    Dim fstop As Byte
    Dim access_cnt As Long
    
    result = AI_9112_Config(card, TRIG_INT_PACER)
    result = AI_AsyncDblBufferMode(card, 1)
    result = AI_ContReadChannel(card, gnADChn, range(cobRange.ListIndex), gnBuffer(0), gnCount, CDbl(nRate * 1000), ASYNCH_OP)
    btnStart.Enabled = False
    btnStop.Enabled = True
    stop_flag = 0
    Do
        Do
            result = AI_AsyncDblBufferHalfReady(card, half_ready, fstop)
        Loop Until half_ready
        result = AI_AsyncDblBufferTransfer(card, gnBuffer(0))
        ShowData (gnCount / 2)
        DoEvents
    Loop Until stop_flag = 1
    result = AI_AsyncClear(card, access_cnt)
    btnStart.Enabled = True
    btnStop.Enabled = False
End Sub

Private Sub btnStop_Click()
    stop_flag = 1
End Sub

Private Sub cobData_Click()
    gnCount = Val(Trim$(cobData))
End Sub



Private Sub cobRange_Click()
Select Case cobRange.ListIndex
Case 0
    labValue(0).Caption = "5.00"
    labValue(1).Caption = "2.50"
    labValue(2).Caption = "0.00"
    labValue(3).Caption = "-2.50"
    labValue(4).Caption = "-5.00"
  Case 1
    labValue(0).Caption = "2.50"
    labValue(1).Caption = "2.25"
    labValue(2).Caption = "0.00"
    labValue(3).Caption = "-2.25"
    labValue(4).Caption = "-2.50"
  Case 2
    labValue(0).Caption = "1.250"
    labValue(1).Caption = "0.625"
    labValue(2).Caption = "0.00"
    labValue(3).Caption = "-0.625"
    labValue(4).Caption = "-1.250"
  Case 3
    labValue(0).Caption = "0.6250"
    labValue(1).Caption = "0.3125"
    labValue(2).Caption = "0.0000"
    labValue(3).Caption = "-0.3125"
    labValue(4).Caption = "-0.6250"
  Case 4
    labValue(0).Caption = "10.0"
    labValue(1).Caption = "7.50"
    labValue(2).Caption = "5.00"
    labValue(3).Caption = "2.50"
    labValue(4).Caption = "0.00"
  Case 5
    labValue(0).Caption = "5.00"
    labValue(1).Caption = "3.75"
    labValue(2).Caption = "2.50"
    labValue(3).Caption = "1.25"
    labValue(4).Caption = "0.00"
  Case 6
    labValue(0).Caption = "2.500"
    labValue(1).Caption = "1.875"
    labValue(2).Caption = "1.250"
    labValue(3).Caption = "0.625"
    labValue(4).Caption = "0.000"
  Case 7
    labValue(0).Caption = "1.2500"
    labValue(1).Caption = "0.9375"
    labValue(2).Caption = "0.6250"
    labValue(3).Caption = "0.3125"
    labValue(4).Caption = "0.0000"
  Case 8
    labValue(0).Caption = "10"
    labValue(1).Caption = "5.0"
    labValue(2).Caption = "0.0"
    labValue(3).Caption = "-5.0"
    labValue(4).Caption = "-10"
  End Select
End Sub

Private Sub cobSample_Click()
    nRate = Val(Trim$(cobSample))
End Sub

Private Sub Form_Load()
    Dim bus As Integer, slot As Integer
    
    SettingForm.Show 1
    If card < 0 Then
        MsgBox "Register Card Failed"
        End
    End If
    
    cobRange.AddItem "+/- 5V"
    cobRange.AddItem "+/- 2.5V"
    cobRange.AddItem "+/- 1.25V"
    cobRange.AddItem "+/- 0.625V"
    cobRange.AddItem "0V~10V"
    cobRange.AddItem "0V~5V"
    cobRange.AddItem "0V~2.5V"
    cobRange.AddItem "0V~1.25V"
    cobRange.AddItem "+/- 10V"

    cobRange.ListIndex = 0
    
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
    
    'Range values
    range(0) = 2
    range(1) = 3
    range(2) = 4
    range(3) = 5
    range(4) = 15
    range(5) = 16
    range(6) = 17
    range(7) = 18
    range(8) = 1
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If card >= 0 Then Release_Card (card)
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




Private Sub optChn_Click(Index As Integer)
  gnADChn = Index
End Sub


