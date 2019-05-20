VERSION 5.00
Begin VB.Form util7258 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7258 Sample Program"
   ClientHeight    =   2700
   ClientLeft      =   1170
   ClientTop       =   3420
   ClientWidth     =   6480
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   2700
   ScaleWidth      =   6480
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Output CH#1"
      ForeColor       =   &H000000FF&
      Height          =   615
      Index           =   2
      Left            =   240
      TabIndex        =   2
      Top             =   960
      Width           =   6015
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5640
         Picture         =   "Util7258.frx":0000
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5280
         Picture         =   "Util7258.frx":0172
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4560
         Picture         =   "Util7258.frx":02E4
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4920
         Picture         =   "Util7258.frx":0456
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4200
         Picture         =   "Util7258.frx":05C8
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3840
         Picture         =   "Util7258.frx":073A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3480
         Picture         =   "Util7258.frx":08AC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3120
         Picture         =   "Util7258.frx":0A1E
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2760
         Picture         =   "Util7258.frx":0B90
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2400
         Picture         =   "Util7258.frx":0D02
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   2040
         Picture         =   "Util7258.frx":0E74
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1680
         Picture         =   "Util7258.frx":0FE6
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1320
         Picture         =   "Util7258.frx":1158
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   960
         Picture         =   "Util7258.frx":12CA
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   600
         Picture         =   "Util7258.frx":143C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP1 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   240
         Picture         =   "Util7258.frx":15AE
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   5280
      Top             =   2160
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Output CH#0"
      ForeColor       =   &H000000FF&
      Height          =   615
      Index           =   1
      Left            =   240
      TabIndex        =   1
      Top             =   1680
      Width           =   6015
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   240
         Picture         =   "Util7258.frx":1720
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   600
         Picture         =   "Util7258.frx":1892
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   960
         Picture         =   "Util7258.frx":1A04
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1320
         Picture         =   "Util7258.frx":1B76
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1680
         Picture         =   "Util7258.frx":1CE8
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   2040
         Picture         =   "Util7258.frx":1E5A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2400
         Picture         =   "Util7258.frx":1FCC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2760
         Picture         =   "Util7258.frx":213E
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3120
         Picture         =   "Util7258.frx":22B0
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3480
         Picture         =   "Util7258.frx":2422
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3840
         Picture         =   "Util7258.frx":2594
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4200
         Picture         =   "Util7258.frx":2706
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4920
         Picture         =   "Util7258.frx":2878
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4560
         Picture         =   "Util7258.frx":29EA
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5280
         Picture         =   "Util7258.frx":2B5C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5640
         Picture         =   "Util7258.frx":2CCE
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Input"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Index           =   0
      Left            =   240
      TabIndex        =   0
      Top             =   180
      Width           =   1455
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   960
         Picture         =   "Util7258.frx":2E40
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   600
         Picture         =   "Util7258.frx":2FB2
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   5760
      Picture         =   "Util7258.frx":3124
      Top             =   2280
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   6120
      Picture         =   "Util7258.frx":3296
      Top             =   2280
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "util7258"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Status(0 To 15) As Integer
Dim do_data As Long
Dim Status2(0 To 15) As Integer
Dim do_data2 As Long
Private Sub DOP0_Click(Index As Integer)
  Dim i As Integer, v As Integer
  Dim d As Integer
  
  If Status(Index) = 1 Then
    do_data = do_data - (2 ^ Index)
    DOP0(Index).Picture = Image0.Picture
    Status(Index) = 0
  Else
    do_data = do_data + (2 ^ Index)
    DOP0(Index).Picture = Image1.Picture
    Status(Index) = 1
  End If
  v = DO_WritePort(card, 0, do_data)
End Sub

Private Sub DOP1_Click(Index As Integer)
  Dim i As Integer, v As Integer
  Dim d As Integer
  
  If Status2(Index) = 1 Then
    do_data2 = do_data2 - (2 ^ Index)
    DOP1(Index).Picture = Image0.Picture
    Status2(Index) = 0
  Else
    do_data2 = do_data2 + (2 ^ Index)
    DOP1(Index).Picture = Image1.Picture
    Status2(Index) = 1
  End If
  v = DO_WritePort(card, 1, do_data2)
End Sub

Private Sub Form_Load()
  Dim result As Integer
  Dim i As Integer, bn As Integer
  SettingForm.Show 1
  If card < 0 Then
        MsgBox "Register Card Failed"
        End
  End If
  For i = 0 To 7
    Status(i) = 0
  Next
  do_data = &H0
End Sub

Private Sub Form_Unload(Cancel As Integer)
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
  End
End Sub


Private Sub mnuExit_Click()
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
  End
End Sub

Private Sub Timer1_Timer()
  Dim result As Integer
  Dim di_data As Long
  Dim i As Integer
  Dim p As Long, d As Long
  
  result = DI_ReadPort(card, 0, di_data)
  'tranform signed integer to unsigned value
  If di_data >= 0 Then
    d = di_data
  Else
    d = CLng(di_data) + 65536
  End If
  For i = 0 To 1
    p = d Mod 2
    If p = 0 Then
      DI0(i) = Image0
    Else 'p = 1
      DI0(i) = Image1
    End If
    d = CLng(Int(d / 2))
  Next
End Sub

