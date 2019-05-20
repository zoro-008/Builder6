VERSION 4.00
Begin VB.Form frm7200 
   Caption         =   "7200 DMA Output Sample"
   ClientHeight    =   3870
   ClientLeft      =   1245
   ClientTop       =   1770
   ClientWidth     =   6195
   BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   4560
   Left            =   1185
   LinkTopic       =   "Form1"
   ScaleHeight     =   3870
   ScaleWidth      =   6195
   Top             =   1140
   Width           =   6315
   Begin VB.CommandButton btnStop 
      Caption         =   "Stop"
      Enabled         =   0   'False
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   405
      Left            =   1860
      TabIndex        =   9
      Top             =   3060
      Width           =   1000
   End
   Begin VB.Frame fraBuf2 
      Caption         =   "Input Buffer"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   1635
      Left            =   3360
      TabIndex        =   4
      Top             =   2040
      Width           =   2655
      Begin VB.ListBox lstBuf2 
         Height          =   1230
         Left            =   90
         TabIndex        =   5
         Top             =   300
         Width           =   2445
      End
   End
   Begin VB.Frame fraBuf1 
      Caption         =   "Output Buffer"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   1635
      Left            =   3360
      TabIndex        =   2
      Top             =   420
      Width           =   2655
      Begin VB.ListBox lstBuf1 
         Height          =   1230
         Left            =   90
         TabIndex        =   3
         Top             =   300
         Width           =   2445
      End
   End
   Begin VB.Frame fraSetup 
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2385
      Left            =   120
      TabIndex        =   1
      Top             =   420
      Width           =   3105
      Begin VB.ComboBox cobInRate 
         Height          =   315
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   12
         Top             =   570
         Width           =   1005
      End
      Begin VB.ComboBox cobCount 
         Height          =   315
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   11
         Top             =   1440
         Width           =   1005
      End
      Begin VB.ComboBox cobOutRate 
         Height          =   315
         Left            =   1440
         Style           =   2  'Dropdown List
         TabIndex        =   10
         Top             =   990
         Width           =   1005
      End
      Begin VB.Label Label4 
         Caption         =   "Samples"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00C00000&
         Height          =   255
         Left            =   2460
         TabIndex        =   16
         Top             =   1500
         Width           =   615
      End
      Begin VB.Label Label3 
         Caption         =   "KHz"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00C00000&
         Height          =   255
         Left            =   2460
         TabIndex        =   15
         Top             =   1020
         Width           =   375
      End
      Begin VB.Label Label2 
         Caption         =   "KHz"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00C00000&
         Height          =   255
         Left            =   2460
         TabIndex        =   14
         Top             =   600
         Width           =   375
      End
      Begin VB.Label labCount 
         Caption         =   "Total Data Count"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FF0000&
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   1500
         Width           =   1335
      End
      Begin VB.Label labOutRate 
         Caption         =   "D/O Sample Rate"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FF0000&
         Height          =   255
         Left            =   120
         TabIndex        =   7
         Top             =   1050
         Width           =   1335
      End
      Begin VB.Label labInRate 
         Caption         =   "D/I Sample Rate"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FF0000&
         Height          =   255
         Left            =   120
         TabIndex        =   6
         Top             =   600
         Width           =   1335
      End
   End
   Begin VB.CommandButton btnStart 
      Caption         =   "Start"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   405
      Left            =   450
      TabIndex        =   0
      Top             =   3060
      Width           =   1000
   End
   Begin VB.Label Label1 
      Caption         =   "This sample performs DMA input and output simulatneously"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C00000&
      Height          =   315
      Left            =   420
      TabIndex        =   13
      Top             =   60
      Width           =   5355
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "frm7200"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim inRate As Double, outRate As Double
Dim dma_i_data(0 To 10000) As Long
Dim dma_o_data(0 To 15000) As Long
Dim glCount As Long

Dim gStr As String

Private Sub GetString(data As Long)
    Dim p As Long
    Dim i As Integer, j As Integer
    Dim sum(0 To 7) As Integer
            
    gStr = ""
        
    For j = 0 To 7
        sum(j) = 0
    Next j
        
    For j = 0 To 7
        For i = 0 To 3
            p = data Mod 2
            If p = 0 Then
                data = data / 2
            Else 'p = 1
                sum(j) = sum(j) + 2 ^ i
                data = (data - 1) / 2
            End If
        Next i
        gStr = Hex$(sum(j)) + gStr
    Next j
        
End Sub


Private Sub ShowData()
    Dim i As Long
    
    For i = 0 To glCount - 1
        GetString dma_o_data(i)
        lstBuf1.AddItem Format(i, "0000") + " : " + gStr
        'lstBuf1.AddItem CStr(i) + " : " + Hex$(dma_o_data(i))
    Next
    For i = 0 To glCount - 1
        GetString dma_i_data(i)
        lstBuf2.AddItem Format(i, "0000") + " : " + gStr
        'lstBuf2.AddItem CStr(i) + " : " + Hex$(dma_i_data(i))
    Next
End Sub

Private Sub InitData()

  cobInRate.AddItem "10"
  cobInRate.AddItem "20"
  cobInRate.AddItem "40"
  cobInRate.AddItem "50"
  cobInRate.AddItem "80"
  cobInRate.AddItem "100"
  cobInRate.ListIndex = 0
  inRate = 10 * 1000
  
  cobOutRate.AddItem "10"
  cobOutRate.AddItem "20"
  cobOutRate.AddItem "40"
  cobOutRate.AddItem "50"
  cobOutRate.AddItem "80"
  cobOutRate.AddItem "100"
  cobOutRate.ListIndex = 0
  outRate = 10 * 1000

  cobCount.AddItem "200"
  cobCount.AddItem "500"
  cobCount.AddItem "1000"
  cobCount.AddItem "2000"
  cobCount.AddItem "5000"
  cobCount.ListIndex = 0
End Sub



Private Sub btnStart_Click()
    Dim i As Integer
    Dim result As Integer
    Dim diCount As Long, doCount As Long
    Dim gbStopped As Byte
    
    For i = 0 To 14999
        dma_o_data(i) = i
    Next
    
    lstBuf1.Clear
    lstBuf2.Clear
    
    glCount = CLng(cobCount.List(cobCount.ListIndex))
    btnStart.Enabled = False
    result = DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW)
    result = DO_AsyncDblBufferMode(card, 0)
    result = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING)
    result = DI_AsyncDblBufferMode(card, 0)
                 
    result = DO_ContWritePort(card, 0, dma_o_data(0), glCount, 1, outRate, ASYNCH_OP)
    result = DI_ContReadPort(card, 0, dma_i_data(0), glCount, inRate, ASYNCH_OP)
    Do
        result = DO_AsyncCheck(card, gbStopped, doCount)
    Loop Until gbStopped
    Do
        result = DI_AsyncCheck(card, gbStopped, diCount)
    Loop Until gbStopped
    result = DO_AsyncClear(card, doCount)
    result = DI_AsyncClear(card, diCount)
    ShowData
    btnStart.Enabled = True
End Sub














Private Sub btnStop_Click()
    Dim result As Integer
    Dim diCount As Long, doCount As Long
    result = DO_AsyncClear(card, doCount)
    result = DI_AsyncClear(card, diCount)
End Sub


Private Sub cobInRate_Click()
    inRate = CDbl(cobInRate.List(cobInRate.ListIndex)) * 1000
End Sub


Private Sub cobOutRate_Click()
    outRate = CDbl(cobOutRate.List(cobOutRate.ListIndex)) * 1000
End Sub


Private Sub Form_Load()
  SettingForm.Show 1
  If card < 0 Then
    MsgBox "Register Card Failed"
    End
  End If
  InitData
End Sub


Private Sub Form_Unload(Cancel As Integer)
    Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
End Sub


Private Sub mnuExit_Click()
    Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
    End
End Sub


