VERSION 5.00
Begin VB.Form util7452 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7452 Sample Program"
   ClientHeight    =   3255
   ClientLeft      =   1635
   ClientTop       =   2565
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
   ScaleHeight     =   3255
   ScaleWidth      =   6480
   Begin VB.ComboBox InCombo 
      Height          =   315
      Left            =   1560
      TabIndex        =   6
      Text            =   "Combo1"
      Top             =   0
      Width           =   1200
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   5280
      Top             =   2640
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Output#"
      ForeColor       =   &H000000FF&
      Height          =   1515
      Index           =   1
      Left            =   240
      TabIndex        =   1
      Top             =   1440
      Width           =   6015
      Begin VB.ComboBox OutCombo 
         Height          =   315
         Left            =   1440
         TabIndex        =   7
         Text            =   "Combo2"
         Top             =   0
         Width           =   1200
      End
      Begin VB.Label Label1 
         Caption         =   "0"
         Height          =   195
         Left            =   5670
         TabIndex        =   5
         Top             =   1200
         Width           =   195
      End
      Begin VB.Label Label3 
         Caption         =   "31"
         Height          =   195
         Left            =   240
         TabIndex        =   3
         Top             =   660
         Width           =   195
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   31
         Left            =   180
         Picture         =   "Util7452.frx":0000
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   30
         Left            =   540
         Picture         =   "Util7452.frx":0172
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   29
         Left            =   900
         Picture         =   "Util7452.frx":02E4
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   28
         Left            =   1260
         Picture         =   "Util7452.frx":0456
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   27
         Left            =   1620
         Picture         =   "Util7452.frx":05C8
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   26
         Left            =   1980
         Picture         =   "Util7452.frx":073A
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   25
         Left            =   2340
         Picture         =   "Util7452.frx":08AC
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   24
         Left            =   2700
         Picture         =   "Util7452.frx":0A1E
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   23
         Left            =   3060
         Picture         =   "Util7452.frx":0B90
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   22
         Left            =   3420
         Picture         =   "Util7452.frx":0D02
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   21
         Left            =   3780
         Picture         =   "Util7452.frx":0E74
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   20
         Left            =   4140
         Picture         =   "Util7452.frx":0FE6
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   19
         Left            =   4500
         Picture         =   "Util7452.frx":1158
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   18
         Left            =   4860
         Picture         =   "Util7452.frx":12CA
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   17
         Left            =   5220
         Picture         =   "Util7452.frx":143C
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   16
         Left            =   5580
         Picture         =   "Util7452.frx":15AE
         Top             =   360
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7452.frx":1720
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7452.frx":1892
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7452.frx":1A04
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7452.frx":1B76
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7452.frx":1CE8
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7452.frx":1E5A
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7452.frx":1FCC
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7452.frx":213E
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3060
         Picture         =   "Util7452.frx":22B0
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3420
         Picture         =   "Util7452.frx":2422
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3780
         Picture         =   "Util7452.frx":2594
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4140
         Picture         =   "Util7452.frx":2706
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4860
         Picture         =   "Util7452.frx":2878
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4500
         Picture         =   "Util7452.frx":29EA
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5220
         Picture         =   "Util7452.frx":2B5C
         Top             =   870
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5580
         Picture         =   "Util7452.frx":2CCE
         Top             =   870
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Input#"
      ForeColor       =   &H00FF0000&
      Height          =   1275
      Index           =   0
      Left            =   240
      TabIndex        =   0
      Top             =   60
      Width           =   6015
      Begin VB.Label Label4 
         Caption         =   "0"
         Height          =   195
         Left            =   5670
         TabIndex        =   4
         Top             =   1050
         Width           =   195
      End
      Begin VB.Label Label2 
         Caption         =   "31"
         Height          =   195
         Left            =   210
         TabIndex        =   2
         Top             =   540
         Width           =   195
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   31
         Left            =   180
         Picture         =   "Util7452.frx":2E40
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   30
         Left            =   540
         Picture         =   "Util7452.frx":2FB2
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   29
         Left            =   900
         Picture         =   "Util7452.frx":3124
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   28
         Left            =   1260
         Picture         =   "Util7452.frx":3296
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   27
         Left            =   1620
         Picture         =   "Util7452.frx":3408
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   26
         Left            =   1980
         Picture         =   "Util7452.frx":357A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   25
         Left            =   2340
         Picture         =   "Util7452.frx":36EC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   24
         Left            =   2700
         Picture         =   "Util7452.frx":385E
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   23
         Left            =   3060
         Picture         =   "Util7452.frx":39D0
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   22
         Left            =   3420
         Picture         =   "Util7452.frx":3B42
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   21
         Left            =   3780
         Picture         =   "Util7452.frx":3CB4
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   20
         Left            =   4140
         Picture         =   "Util7452.frx":3E26
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   19
         Left            =   4500
         Picture         =   "Util7452.frx":3F98
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   18
         Left            =   4860
         Picture         =   "Util7452.frx":410A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   17
         Left            =   5220
         Picture         =   "Util7452.frx":427C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   16
         Left            =   5580
         Picture         =   "Util7452.frx":43EE
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7452.frx":4560
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7452.frx":46D2
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7452.frx":4844
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7452.frx":49B6
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7452.frx":4B28
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7452.frx":4C9A
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7452.frx":4E0C
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7452.frx":4F7E
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5580
         Picture         =   "Util7452.frx":50F0
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5220
         Picture         =   "Util7452.frx":5262
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4860
         Picture         =   "Util7452.frx":53D4
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4500
         Picture         =   "Util7452.frx":5546
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4140
         Picture         =   "Util7452.frx":56B8
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3780
         Picture         =   "Util7452.frx":582A
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3420
         Picture         =   "Util7452.frx":599C
         Top             =   750
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3060
         Picture         =   "Util7452.frx":5B0E
         Top             =   750
         Width           =   300
      End
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   5880
      Picture         =   "Util7452.frx":5C80
      Top             =   2700
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   6180
      Picture         =   "Util7452.frx":5DF2
      Top             =   2700
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "util7452"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Status(0 To 31) As Long
Dim bit(0 To 31) As Long
Dim iport As Integer
Dim oport As Integer

Private Sub DOP0_Click(Index As Integer)
  Dim i As Integer, result As Integer
  Dim d As Long
  
  If Status(Index) = 1 Then
    DOP0(Index).Picture = Image0.Picture
    Status(Index) = 0
  Else
    DOP0(Index).Picture = Image1.Picture
    Status(Index) = 1
  End If
  d = 0
  For i = 0 To 30
    If Status(i) = 1 Then
      d = d + (2 ^ i)
    End If
  Next
  'Handle the MSB
  'd=d+2^31, but bucause VB has no unsigned long,
  'translate an unsigned long to signed long by minus 2^32
  If Status(31) = 1 Then d = d - 2 ^ 31  'd = d + 2^31 - 2^32
  result = DO_WritePort(card, oport, d)

End Sub

Private Sub Form_Load()
  Dim result As Integer
  Dim i As Integer
    
  SettingForm.Show 1
  If card < 0 Then
    MsgBox "Register card failed."
    End
  End If
  For i = 0 To 31
    Status(i) = 0
  Next
  
  bit(0) = &H1&
  bit(1) = &H2&
  bit(2) = &H4&
  bit(3) = &H8&
  bit(4) = &H10&
  bit(5) = &H20&
  bit(6) = &H40&
  bit(7) = &H80&
  bit(8) = &H100&
  bit(9) = &H200&
  bit(10) = &H400&
  bit(11) = &H800&
  bit(12) = &H1000&
  bit(13) = &H2000&
  bit(14) = &H4000&
  bit(15) = &H8000&
  bit(16) = &H10000
  bit(17) = &H20000
  bit(18) = &H40000
  bit(19) = &H80000
  bit(20) = &H100000
  bit(21) = &H200000
  bit(22) = &H400000
  bit(23) = &H800000
  bit(24) = &H1000000
  bit(25) = &H2000000
  bit(26) = &H4000000
  bit(27) = &H8000000
  bit(28) = &H10000000
  bit(29) = &H20000000
  bit(30) = &H40000000
  bit(31) = &H80000000
  
  iport = 0
  oport = 0
  
  For i = 0 To 3 Step 1
    InCombo.AddItem i
    OutCombo.AddItem i
  Next
  InCombo.ListIndex = 0
  OutCombo.ListIndex = 0
End Sub

Private Sub Form_Unload(Cancel As Integer)
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
  End
End Sub
Private Sub InCombo_Click()
iport = InCombo.ListIndex
End Sub

Private Sub mnuExit_Click()
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If

  End
End Sub

Private Sub OutCombo_Click()
Dim i As Integer
Dim result As Integer
Dim rb_data As Long
Dim p As Long

oport = OutCombo.ListIndex
result = DO_ReadPort(card, oport, rb_data)
For i = 0 To 31
    p = rb_data And bit(i)
    If p <> 0 Then
      DOP0(i) = Image1
      Status(i) = 1
    Else 'p = 0
      DOP0(i) = Image0
      Status(i) = 0
    End If
Next
End Sub

Private Sub Timer1_Timer()
  Dim result As Integer
  Dim di_data As Long
  Dim dl As Long, dh As Long
  Dim i As Integer
  Dim p As Long
  
  result = DI_ReadPort(card, iport, di_data)
  
  For i = 0 To 31
    p = di_data And bit(i)
    If p <> 0 Then
      DI0(i) = Image1
    Else 'p = 0
      DI0(i) = Image0
    End If
  Next
End Sub

