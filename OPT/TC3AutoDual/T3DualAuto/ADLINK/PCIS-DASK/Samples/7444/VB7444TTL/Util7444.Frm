VERSION 5.00
Begin VB.Form util7444 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7444 Sample Program"
   ClientHeight    =   3660
   ClientLeft      =   1635
   ClientTop       =   2565
   ClientWidth     =   6870
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
   ScaleHeight     =   3660
   ScaleWidth      =   6870
   Begin VB.ComboBox InCombo 
      Height          =   315
      Left            =   2160
      TabIndex        =   5
      Text            =   "Combo1"
      Top             =   840
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
      Caption         =   "Digital Output# TTL"
      ForeColor       =   &H000000FF&
      Height          =   1155
      Index           =   1
      Left            =   360
      TabIndex        =   1
      Top             =   2160
      Width           =   6015
      Begin VB.ComboBox OutCombo 
         Height          =   315
         Left            =   1920
         TabIndex        =   6
         Text            =   "Combo2"
         Top             =   0
         Width           =   1200
      End
      Begin VB.Label Label1 
         Caption         =   "0"
         Height          =   195
         Left            =   5640
         TabIndex        =   4
         Top             =   840
         Width           =   195
      End
      Begin VB.Label Label3 
         Caption         =   "15"
         Height          =   195
         Left            =   240
         TabIndex        =   2
         Top             =   840
         Width           =   195
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7444.frx":0000
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7444.frx":0172
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7444.frx":02E4
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7444.frx":0456
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7444.frx":05C8
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7444.frx":073A
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7444.frx":08AC
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7444.frx":0A1E
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3060
         Picture         =   "Util7444.frx":0B90
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3420
         Picture         =   "Util7444.frx":0D02
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3780
         Picture         =   "Util7444.frx":0E74
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4140
         Picture         =   "Util7444.frx":0FE6
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4860
         Picture         =   "Util7444.frx":1158
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4500
         Picture         =   "Util7444.frx":12CA
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5220
         Picture         =   "Util7444.frx":143C
         Top             =   500
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5580
         Picture         =   "Util7444.frx":15AE
         Top             =   500
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Input# TTL"
      ForeColor       =   &H00FF0000&
      Height          =   1155
      Index           =   0
      Left            =   360
      TabIndex        =   0
      Top             =   840
      Width           =   6015
      Begin VB.Label Label2 
         Caption         =   "15"
         Height          =   195
         Left            =   240
         TabIndex        =   7
         Top             =   840
         Width           =   195
      End
      Begin VB.Label Label4 
         Caption         =   "0"
         Height          =   195
         Left            =   5640
         TabIndex        =   3
         Top             =   840
         Width           =   195
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7444.frx":1720
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7444.frx":1892
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7444.frx":1A04
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7444.frx":1B76
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7444.frx":1CE8
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7444.frx":1E5A
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7444.frx":1FCC
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7444.frx":213E
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5580
         Picture         =   "Util7444.frx":22B0
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5220
         Picture         =   "Util7444.frx":2422
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4860
         Picture         =   "Util7444.frx":2594
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4500
         Picture         =   "Util7444.frx":2706
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4140
         Picture         =   "Util7444.frx":2878
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3780
         Picture         =   "Util7444.frx":29EA
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3420
         Picture         =   "Util7444.frx":2B5C
         Top             =   480
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3060
         Picture         =   "Util7444.frx":2CCE
         Top             =   480
         Width           =   300
      End
   End
   Begin VB.Label Label5 
      Caption         =   "Select TTL0 or TTL1 for input or output"
      Height          =   255
      Left            =   360
      TabIndex        =   8
      Top             =   360
      Width           =   3495
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   5880
      Picture         =   "Util7444.frx":2E40
      Top             =   2700
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   6180
      Picture         =   "Util7444.frx":2FB2
      Top             =   2700
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "util7444"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Status(0 To 15) As Long
Dim bit(0 To 15) As Long
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
  For i = 0 To 14
    If Status(i) = 1 Then
      d = d + (2 ^ i)
    End If
  Next
  'Handle the MSB
  'd=d+2^31, but bucause VB has no unsigned long,
  'translate an unsigned long to signed long by minus 2^32
  If Status(15) = 1 Then d = d - 2 ^ 15  'd = d + 2^15 - 2^16
  
  If oport = 0 Then
    result = DO_WritePort(card, P7444_TTL0, d)
  Else
    result = DO_WritePort(card, P7444_TTL1, d)
  End If

End Sub

Private Sub Form_Load()
  Dim result As Integer
  Dim i As Integer
    
  SettingForm.Show 1
  If card < 0 Then
    MsgBox "Register card failed."
    End
  End If
  For i = 0 To 15
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
'  bit(16) = &H10000
'  bit(17) = &H20000
'  bit(18) = &H40000
'  bit(19) = &H80000
'  bit(20) = &H100000
'  bit(21) = &H200000
'  bit(22) = &H400000
'  bit(23) = &H800000
'  bit(24) = &H1000000
'  bit(25) = &H2000000
'  bit(26) = &H4000000
'  bit(27) = &H8000000
'  bit(28) = &H10000000
'  bit(29) = &H20000000
'  bit(30) = &H40000000
'  bit(31) = &H80000000
  
  iport = 0
  oport = 1
  
  For i = 0 To 1 Step 1
    InCombo.AddItem i
    OutCombo.AddItem i
  Next
  InCombo.ListIndex = 0
  OutCombo.ListIndex = 1
  
  result = DIO_PortConfig(card, iport + 4, INPUT_PORT)
  result = DIO_PortConfig(card, oport + 4, OUTPUT_PORT)
End Sub

Private Sub Form_Unload(Cancel As Integer)
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
  End
End Sub
Private Sub InCombo_Click()
Dim i As Integer
Dim result As Integer
Dim rb_data As Long
Dim p As Long

iport = InCombo.ListIndex
If iport = 0 Then
    oport = 1
Else
    oport = 0
End If

OutCombo.ListIndex = oport

result = DIO_PortConfig(card, iport + 4, INPUT_PORT)
result = DIO_PortConfig(card, oport + 4, OUTPUT_PORT)

For i = 0 To 15
    DOP0(i) = Image0
    Status(i) = 0
Next

If oport = 0 Then
    result = DO_WritePort(card, P7444_TTL0, 0)
Else
    result = DO_WritePort(card, P7444_TTL1, 0)
End If

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
If oport = 0 Then
    iport = 1
Else
    iport = 0
End If

InCombo.ListIndex = iport

result = DIO_PortConfig(card, iport + 4, INPUT_PORT)
result = DIO_PortConfig(card, oport + 4, OUTPUT_PORT)

For i = 0 To 15
    DOP0(i) = Image0
    Status(i) = 0
Next

If oport = 0 Then
    result = DO_WritePort(card, P7444_TTL0, 0)
Else
    result = DO_WritePort(card, P7444_TTL1, 0)
End If

End Sub

Private Sub Timer1_Timer()
  Dim result As Integer
  Dim di_data As Long
  Dim dl As Long, dh As Long
  Dim i As Integer
  Dim p As Long
  
  If iport = 0 Then
    result = DI_ReadPort(card, P7444_TTL0, di_data)
  Else
    result = DI_ReadPort(card, P7444_TTL1, di_data)
  End If
  
  For i = 0 To 15
    p = di_data And bit(i)
    If p <> 0 Then
      DI0(i) = Image1
    Else 'p = 0
      DI0(i) = Image0
    End If
  Next
End Sub

