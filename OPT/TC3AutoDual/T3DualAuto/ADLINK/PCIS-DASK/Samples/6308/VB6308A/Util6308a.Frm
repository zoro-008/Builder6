VERSION 4.00
Begin VB.Form Util6308 
   Caption         =   "PCI-6308A"
   ClientHeight    =   2835
   ClientLeft      =   1095
   ClientTop       =   1800
   ClientWidth     =   2550
   Height          =   3525
   Left            =   1035
   LinkTopic       =   "Form1"
   ScaleHeight     =   2835
   ScaleWidth      =   2550
   Top             =   1170
   Width           =   2670
   Begin VB.ComboBox cobChannel 
      Height          =   315
      Left            =   1560
      Style           =   2  'Dropdown List
      TabIndex        =   8
      TabStop         =   0   'False
      Top             =   240
      Width           =   795
   End
   Begin VB.Frame Frame2 
      Caption         =   "Current Output"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   1815
      Left            =   300
      TabIndex        =   0
      Top             =   780
      Width           =   1935
      Begin VB.Label Label2 
         Caption         =   "mA"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   225
         Left            =   1320
         TabIndex        =   10
         Top             =   1440
         Width           =   375
      End
      Begin VB.Label AO_value 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H0000FFFF&
         BorderStyle     =   1  'Fixed Single
         Caption         =   "-5.0"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   600
         TabIndex        =   7
         Top             =   1440
         Width           =   675
      End
      Begin VB.Line pin 
         BorderColor     =   &H0000FFFF&
         BorderWidth     =   2
         X1              =   960
         X2              =   840
         Y1              =   900
         Y2              =   1020
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   480
         TabIndex        =   6
         Top             =   1200
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "1"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   1
         Left            =   360
         TabIndex        =   5
         Top             =   720
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   660
         TabIndex        =   4
         Top             =   300
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "3"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   3
         Left            =   1140
         TabIndex        =   3
         Top             =   300
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "4"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   4
         Left            =   1440
         TabIndex        =   2
         Top             =   720
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   5
         Left            =   1320
         TabIndex        =   1
         Top             =   1200
         Width           =   195
      End
      Begin VB.Image tuner 
         Appearance      =   0  'Flat
         Height          =   750
         Left            =   540
         Picture         =   "Util6308a.frx":0000
         Top             =   480
         Width           =   900
      End
   End
   Begin VB.Label Label1 
      Caption         =   "Output Channel:"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   9
      Top             =   300
      Width           =   1455
   End
   Begin VB.Menu mnuExit 
      Caption         =   "E&xit!"
   End
End
Attribute VB_Name = "Util6308"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
'For tuner
Const Pi = 3.14159
Const MaxAngle = 4      '5Pi/4
Const MinAngle = -0.8    '-Pi/4

Dim x0 As Integer, y0 As Integer
Dim DragOn As Integer
Dim l As Integer
Dim lb As Single, ub As Single
'Dim hEnts As EventHandles
Private Function ArcCos(p As Single) As Single
  If p <= -1 Then
    ArcCos = Pi
    Exit Function
  ElseIf p >= 1 Then
    ArcCos = 0
    Exit Function
  End If
  ArcCos = Atn(-p / Sqr(-p * p + 1)) + 1.5708
End Function




Private Sub Form_Load()
  Dim result As Integer
  SettingForm.Show 1
  If card < 0 Then
        MsgBox "Register Card Failed"
        End
  End If
  For i = 0 To 7
    cobChannel.AddItem i
  Next
  cobChannel.ListIndex = 0
  'For tuner
  x0 = pin.X1
  y0 = pin.Y1
  l = Sqr(((pin.X2 - x0) ^ 2) + ((pin.Y2 - y0) ^ 2))
  lb = 4
  ub = 20
  For i = 0 To 5
    mark(i).Caption = Format(i * (ub - lb) / 5 + lb, "#0")
  Next
  AO_value.Caption = Format(lb, "#0.00")
  result = AO_6308A_Config(card, P6308_CURRENT_4_20MA)
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


Private Sub tuner_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
  Dim dx As Integer, dy As Integer
  dx = X + tuner.Left - pin.X2
  dy = Y + tuner.Top - pin.Y2
  If dx > -50 And dx < 50 And dy > -50 And dy < 50 Then
    DragOn = True
  End If
End Sub


Private Sub tuner_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
  Dim r As Single, p As Single
  Dim d As Long
  Dim dx As Long, dy As Long
  Dim q As Integer, result As Integer
  If DragOn Then
    dx = X + tuner.Left - x0
    dy = Y + tuner.Top - y0
    d = Sqr(dx ^ 2 + dy ^ 2)
    r = ArcCos(CSng(dx / d))
    If dy > 0 And dx < 0 Then r = 2 * Pi - r
    If dy > 0 And dx >= 0 Then r = -r
    If r <= MaxAngle And r >= MinAngle Then
      pin.X2 = x0 + CInt(l * dx / d)
      pin.Y2 = y0 + CInt(l * dy / d)
      p = ub - (ub - lb) * (2 * r + Pi / 2) / (3 * Pi)
      If p > ub Then
        p = ub
      ElseIf p < lb Then
        p = lb
      End If
      AO_value.Caption = Format(p, "#0.00")
      q = CInt((p - lb) * 4095 / (ub - lb))
      result = AO_WriteChannel(card, cobChannel.ListIndex, q)

    End If
  End If
End Sub


Private Sub tuner_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
  DragOn = False
End Sub


