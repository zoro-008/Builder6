VERSION 4.00
Begin VB.Form util7234 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7234 Sample Program"
   ClientHeight    =   2010
   ClientLeft      =   1110
   ClientTop       =   1995
   ClientWidth     =   6525
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
   Height          =   2700
   Left            =   1050
   LinkTopic       =   "Form1"
   ScaleHeight     =   2010
   ScaleWidth      =   6525
   Top             =   1365
   Width           =   6645
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Output Hi"
      ForeColor       =   &H000000FF&
      Height          =   1455
      Index           =   1
      Left            =   180
      TabIndex        =   0
      Top             =   60
      Width           =   6075
      Begin VB.Label Label2 
         Caption         =   "31"
         Height          =   195
         Left            =   210
         TabIndex        =   2
         Top             =   600
         Width           =   255
      End
      Begin VB.Label Label1 
         Caption         =   "0"
         Height          =   195
         Left            =   5670
         TabIndex        =   1
         Top             =   1200
         Width           =   135
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   31
         Left            =   180
         Picture         =   "Util7234.frx":0000
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   30
         Left            =   540
         Picture         =   "Util7234.frx":0172
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   28
         Left            =   1260
         Picture         =   "Util7234.frx":02E4
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   29
         Left            =   900
         Picture         =   "Util7234.frx":0456
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   27
         Left            =   1620
         Picture         =   "Util7234.frx":05C8
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   26
         Left            =   1980
         Picture         =   "Util7234.frx":073A
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   25
         Left            =   2340
         Picture         =   "Util7234.frx":08AC
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   24
         Left            =   2700
         Picture         =   "Util7234.frx":0A1E
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   23
         Left            =   3060
         Picture         =   "Util7234.frx":0B90
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   22
         Left            =   3420
         Picture         =   "Util7234.frx":0D02
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   21
         Left            =   3780
         Picture         =   "Util7234.frx":0E74
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   20
         Left            =   4140
         Picture         =   "Util7234.frx":0FE6
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   19
         Left            =   4500
         Picture         =   "Util7234.frx":1158
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   18
         Left            =   4860
         Picture         =   "Util7234.frx":12CA
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   17
         Left            =   5220
         Picture         =   "Util7234.frx":143C
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   16
         Left            =   5580
         Picture         =   "Util7234.frx":15AE
         Top             =   300
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7234.frx":1720
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7234.frx":1892
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7234.frx":1A04
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7234.frx":1B76
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7234.frx":1CE8
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7234.frx":1E5A
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7234.frx":1FCC
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7234.frx":213E
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3060
         Picture         =   "Util7234.frx":22B0
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3420
         Picture         =   "Util7234.frx":2422
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3780
         Picture         =   "Util7234.frx":2594
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4140
         Picture         =   "Util7234.frx":2706
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4860
         Picture         =   "Util7234.frx":2878
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4500
         Picture         =   "Util7234.frx":29EA
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5220
         Picture         =   "Util7234.frx":2B5C
         Top             =   900
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5580
         Picture         =   "Util7234.frx":2CCE
         Top             =   900
         Width           =   300
      End
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   5640
      Picture         =   "Util7234.frx":2E40
      Top             =   1620
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   5940
      Picture         =   "Util7234.frx":2FB2
      Top             =   1620
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "util7234"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit
Dim status0(0 To 31) As Integer


Private Sub DOP0_Click(Index As Integer)
  Dim i As Integer, result As Integer
  Dim do_data As Long
  
  If status0(Index) = 1 Then
    DOP0(Index).Picture = Image0.Picture
    status0(Index) = 0
  Else
    DOP0(Index).Picture = Image1.Picture
    status0(Index) = 1
  End If
  do_data = 0
  For i = 0 To 30
    If status0(i) = 1 Then do_data = do_data + (2 ^ i)
  Next
  'Handle the MSB
  'do_data=do_data+2^31, but bucause VB has no unsigned long,
  'translate an unsigned long to signed long by minus 2^32
  If status0(31) = 1 Then do_data = do_data - 2 ^ 31  'do_data = do_data + 2^31 - 2^32
  result = DO_WritePort(card, 0, do_data)
  
End Sub


Private Sub Form_Load()
  Dim result As Integer
  Dim i As Integer
  
  SettingForm.Show 1
  If card < 0 Then
    MsgBox "Register Card Failed"
    End
  End If
  For i = 0 To 31
    status0(i) = 0
  Next

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


