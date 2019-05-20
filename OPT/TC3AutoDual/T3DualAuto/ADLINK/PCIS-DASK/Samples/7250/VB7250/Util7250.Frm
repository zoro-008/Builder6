VERSION 4.00
Begin VB.Form util7250 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7250 Sample Program"
   ClientHeight    =   6360
   ClientLeft      =   2370
   ClientTop       =   1815
   ClientWidth     =   5070
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
   Height          =   7050
   Left            =   2310
   LinkTopic       =   "Form1"
   ScaleHeight     =   6360
   ScaleWidth      =   5070
   Top             =   1185
   Width           =   5190
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00008000&
      Caption         =   "Digital Input"
      ForeColor       =   &H00FF0000&
      Height          =   615
      Index           =   7
      Left            =   1380
      TabIndex        =   7
      Top             =   4860
      Width           =   3195
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   31
         Left            =   180
         Picture         =   "Util7250.frx":0000
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   30
         Left            =   540
         Picture         =   "Util7250.frx":0172
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   29
         Left            =   900
         Picture         =   "Util7250.frx":02E4
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   28
         Left            =   1260
         Picture         =   "Util7250.frx":0456
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   27
         Left            =   1620
         Picture         =   "Util7250.frx":05C8
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   26
         Left            =   1980
         Picture         =   "Util7250.frx":073A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   25
         Left            =   2340
         Picture         =   "Util7250.frx":08AC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   24
         Left            =   2700
         Picture         =   "Util7250.frx":0A1E
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00008000&
      Caption         =   "Digital Output"
      ForeColor       =   &H000000FF&
      Height          =   615
      Index           =   6
      Left            =   1380
      TabIndex        =   6
      Top             =   5520
      Width           =   3195
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   31
         Left            =   180
         Picture         =   "Util7250.frx":0B90
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   30
         Left            =   540
         Picture         =   "Util7250.frx":0D02
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   29
         Left            =   900
         Picture         =   "Util7250.frx":0E74
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   28
         Left            =   1260
         Picture         =   "Util7250.frx":0FE6
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   26
         Left            =   1980
         Picture         =   "Util7250.frx":1158
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   27
         Left            =   1620
         Picture         =   "Util7250.frx":12CA
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   25
         Left            =   2340
         Picture         =   "Util7250.frx":143C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   24
         Left            =   2700
         Picture         =   "Util7250.frx":15AE
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
      Index           =   5
      Left            =   1380
      TabIndex        =   5
      Top             =   3300
      Width           =   3195
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   23
         Left            =   180
         Picture         =   "Util7250.frx":1720
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   22
         Left            =   540
         Picture         =   "Util7250.frx":1892
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   21
         Left            =   900
         Picture         =   "Util7250.frx":1A04
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   20
         Left            =   1260
         Picture         =   "Util7250.frx":1B76
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   19
         Left            =   1620
         Picture         =   "Util7250.frx":1CE8
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   18
         Left            =   1980
         Picture         =   "Util7250.frx":1E5A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   17
         Left            =   2340
         Picture         =   "Util7250.frx":1FCC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   16
         Left            =   2700
         Picture         =   "Util7250.frx":213E
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00008000&
      Caption         =   "Digital Output"
      ForeColor       =   &H000000FF&
      Height          =   615
      Index           =   4
      Left            =   1380
      TabIndex        =   4
      Top             =   3960
      Width           =   3195
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   23
         Left            =   180
         Picture         =   "Util7250.frx":22B0
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   22
         Left            =   540
         Picture         =   "Util7250.frx":2422
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   21
         Left            =   900
         Picture         =   "Util7250.frx":2594
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   20
         Left            =   1260
         Picture         =   "Util7250.frx":2706
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   19
         Left            =   1620
         Picture         =   "Util7250.frx":2878
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   18
         Left            =   1980
         Picture         =   "Util7250.frx":29EA
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   17
         Left            =   2340
         Picture         =   "Util7250.frx":2B5C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   16
         Left            =   2700
         Picture         =   "Util7250.frx":2CCE
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
      Index           =   3
      Left            =   1380
      TabIndex        =   3
      Top             =   1740
      Width           =   3195
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7250.frx":2E40
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7250.frx":2FB2
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7250.frx":3124
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7250.frx":3296
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7250.frx":3408
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7250.frx":357A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7250.frx":36EC
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7250.frx":385E
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00008000&
      Caption         =   "Digital Output"
      ForeColor       =   &H000000FF&
      Height          =   615
      Index           =   2
      Left            =   1380
      TabIndex        =   2
      Top             =   2400
      Width           =   3195
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2700
         Picture         =   "Util7250.frx":39D0
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2340
         Picture         =   "Util7250.frx":3B42
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1620
         Picture         =   "Util7250.frx":3CB4
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1980
         Picture         =   "Util7250.frx":3E26
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1260
         Picture         =   "Util7250.frx":3F98
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   900
         Picture         =   "Util7250.frx":410A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   540
         Picture         =   "Util7250.frx":427C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   180
         Picture         =   "Util7250.frx":43EE
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   180
      Top             =   5340
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
         Picture         =   "Util7250.frx":4560
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   540
         Picture         =   "Util7250.frx":46D2
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   900
         Picture         =   "Util7250.frx":4844
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   1260
         Picture         =   "Util7250.frx":49B6
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   1980
         Picture         =   "Util7250.frx":4B28
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   1620
         Picture         =   "Util7250.frx":4C9A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   2340
         Picture         =   "Util7250.frx":4E0C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DOP0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   2700
         Picture         =   "Util7250.frx":4F7E
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
         Picture         =   "Util7250.frx":50F0
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   2340
         Picture         =   "Util7250.frx":5262
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   1980
         Picture         =   "Util7250.frx":53D4
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   1620
         Picture         =   "Util7250.frx":5546
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   1260
         Picture         =   "Util7250.frx":56B8
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   900
         Picture         =   "Util7250.frx":582A
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   540
         Picture         =   "Util7250.frx":599C
         Top             =   240
         Width           =   300
      End
      Begin VB.Image DI0 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   180
         Picture         =   "Util7250.frx":5B0E
         Top             =   240
         Width           =   300
      End
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      BackColor       =   &H00C00000&
      Caption         =   "PCI-7251#3"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   60
      TabIndex        =   11
      Top             =   4800
      Width           =   1095
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      BackColor       =   &H00C00000&
      Caption         =   "PCI-7251#2"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   60
      TabIndex        =   10
      Top             =   3240
      Width           =   1095
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BackColor       =   &H00C00000&
      Caption         =   "PCI-7251#1"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   60
      TabIndex        =   9
      Top             =   1680
      Width           =   1095
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackColor       =   &H00C00000&
      Caption         =   "PCI-7250"
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   60
      TabIndex        =   8
      Top             =   120
      Width           =   1095
   End
   Begin VB.Shape Shape1 
      BorderWidth     =   2
      FillColor       =   &H00008000&
      FillStyle       =   0  'Solid
      Height          =   1455
      Index           =   3
      Left            =   1260
      Top             =   4800
      Width           =   3495
   End
   Begin VB.Shape Shape1 
      BorderWidth     =   2
      FillColor       =   &H00008000&
      FillStyle       =   0  'Solid
      Height          =   1455
      Index           =   2
      Left            =   1260
      Top             =   3240
      Width           =   3495
   End
   Begin VB.Shape Shape1 
      BorderWidth     =   2
      FillColor       =   &H00008000&
      FillStyle       =   0  'Solid
      Height          =   1455
      Index           =   1
      Left            =   1260
      Top             =   1680
      Width           =   3495
   End
   Begin VB.Shape Shape1 
      BorderWidth     =   2
      FillColor       =   &H00008000&
      FillStyle       =   0  'Solid
      Height          =   1455
      Index           =   0
      Left            =   1260
      Top             =   120
      Width           =   3495
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   660
      Picture         =   "Util7250.frx":5C80
      Top             =   5700
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   660
      Picture         =   "Util7250.frx":5DF2
      Top             =   5400
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Menu mnuExit 
      Caption         =   "&Exit!"
   End
End
Attribute VB_Name = "util7250"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit
Dim status(0 To 31) As Byte
Dim do_data(4) As Long
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
  For i = 0 To 31
    status(i) = 0
  Next
  For i = 0 To 3
    do_data(i) = 0
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

Private Sub Timer1_Timer()
  Dim result As Integer
  Dim di_data As Long
  Dim i As Integer
  Dim p As Integer
  Dim Port As Integer
  
  For Port = 0 To 3
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
  Next
End Sub

