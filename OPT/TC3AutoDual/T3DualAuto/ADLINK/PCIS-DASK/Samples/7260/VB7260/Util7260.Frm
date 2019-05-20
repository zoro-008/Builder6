VERSION 5.00
Begin VB.Form util7260 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7260 Sample Program"
   ClientHeight    =   1950
   ClientLeft      =   2370
   ClientTop       =   1815
   ClientWidth     =   5070
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
   ScaleHeight     =   1950
   ScaleWidth      =   5070
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   300
      Top             =   1140
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00008000&
      Caption         =   "Digital Output"
      ForeColor       =   &H000000FF&
      Height          =   615
      Index           =   1
      Left            =   1380
      TabIndex        =   1
      Top             =   840
      Width           =   3195
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   180
         Picture         =   "Util7260.frx":0000
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   540
         Picture         =   "Util7260.frx":0172
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   900
         Picture         =   "Util7260.frx":02E4
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   1260
         Picture         =   "Util7260.frx":0456
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   1980
         Picture         =   "Util7260.frx":05C8
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   1620
         Picture         =   "Util7260.frx":073A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   2340
         Picture         =   "Util7260.frx":08AC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   2700
         Picture         =   "Util7260.frx":0A1E
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00008000&
      Caption         =   "Digital Input"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Index           =   0
      Left            =   1380
      TabIndex        =   0
      Top             =   180
      Width           =   3195
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   2700
         Picture         =   "Util7260.frx":0B90
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   2340
         Picture         =   "Util7260.frx":0D02
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   1980
         Picture         =   "Util7260.frx":0E74
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   1620
         Picture         =   "Util7260.frx":0FE6
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   1260
         Picture         =   "Util7260.frx":1158
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   900
         Picture         =   "Util7260.frx":12CA
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   540
         Picture         =   "Util7260.frx":143C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   180
         Picture         =   "Util7260.frx":15AE
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackColor       =   &H00C00000&
      Caption         =   "PCI-7260"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   60
      TabIndex        =   2
      Top             =   120
      Width           =   1095
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   780
      Picture         =   "Util7260.frx":1720
      Top             =   1500
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   780
      Picture         =   "Util7260.frx":1892
      Top             =   1200
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "util7260"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim status(0 To 7) As Byte
Dim do_data(1) As Long
Private Sub DOP0_Click(Index As Integer)
  Dim i As Integer, v As Integer, d As Integer
  Dim Port As Integer
  Port = Fix(Index / 8)
  d = 2 ^ (Index Mod 8)
  If status(Index) = 1 Then
    do_data(Port) = do_data(Port) - d
    DOP0(Index).Picture = Image0.Picture
    status(Index) = 0
  Else
    do_data(Port) = do_data(Port) + d
    DOP0(Index).Picture = Image1.Picture
    status(Index) = 1
  End If
  v = DO_WritePort(card, Port, do_data(Port))

End Sub


Private Sub Form_Load()
  Dim i As Integer, result As Integer
  SettingForm.Show 1
  If card < 0 Then
        MsgBox "Register Card Failed"
        End
  End If
  For i = 0 To 7
    status(i) = 0
  Next
  
  do_data(0) = 0
  
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
  Dim p As Integer
  Dim Port As Integer
  
  Port = 0
    result = DI_ReadPort(card, Port, di_data)
    For i = 0 To 7
      p = di_data Mod 2
      If p = 0 Then
        DI0(Port * 8 + i) = Image0
      Else 'p = 1
        DI0(Port * 8 + i) = Image1
      End If
      di_data = CInt(Int(di_data / 2))
    Next
  End Sub

