VERSION 4.00
Begin VB.Form Util7348 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7348 Sample Program"
   ClientHeight    =   1800
   ClientLeft      =   570
   ClientTop       =   2265
   ClientWidth     =   7710
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
   Height          =   2490
   Left            =   510
   LinkTopic       =   "Form1"
   ScaleHeight     =   1800
   ScaleWidth      =   7710
   Top             =   1635
   Width           =   7830
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Port C"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Times New Roman"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   1455
      Index           =   2
      Left            =   5160
      TabIndex        =   4
      Top             =   120
      Width           =   2115
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   1590
         Picture         =   "UTIL7348.frx":0000
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   1380
         Picture         =   "UTIL7348.frx":00E2
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   1170
         Picture         =   "UTIL7348.frx":01C4
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   960
         Picture         =   "UTIL7348.frx":02A6
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7348.frx":0388
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7348.frx":046A
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   360
         Picture         =   "UTIL7348.frx":054C
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   7
         Left            =   120
         Picture         =   "UTIL7348.frx":062E
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   8
         Left            =   1590
         Picture         =   "UTIL7348.frx":0710
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   9
         Left            =   1380
         Picture         =   "UTIL7348.frx":07F2
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   10
         Left            =   1170
         Picture         =   "UTIL7348.frx":08D4
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   11
         Left            =   960
         Picture         =   "UTIL7348.frx":09B6
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   12
         Left            =   750
         Picture         =   "UTIL7348.frx":0A98
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   13
         Left            =   540
         Picture         =   "UTIL7348.frx":0B7A
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   14
         Left            =   330
         Picture         =   "UTIL7348.frx":0C5C
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_c 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   15
         Left            =   120
         Picture         =   "UTIL7348.frx":0D3E
         Top             =   960
         Width           =   180
      End
      Begin VB.Image input_c 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7348.frx":0E20
         Top             =   360
         Width           =   165
      End
      Begin VB.Image input_c 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7348.frx":0EFA
         Top             =   840
         Width           =   165
      End
      Begin VB.Image output_c 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7348.frx":0FD4
         Top             =   555
         Width           =   165
      End
      Begin VB.Image output_c 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7348.frx":10AE
         Top             =   1035
         Width           =   165
      End
      Begin VB.Shape Shape_c 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   0
         Left            =   60
         Top             =   360
         Width           =   1800
      End
      Begin VB.Shape Shape_c 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   1
         Left            =   60
         Top             =   840
         Width           =   1800
      End
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   7260
      Top             =   1440
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Port B"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Times New Roman"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   1455
      Index           =   1
      Left            =   2880
      TabIndex        =   2
      Top             =   120
      Width           =   2115
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   7
         Left            =   120
         Picture         =   "UTIL7348.frx":1188
         Top             =   480
         Width           =   180
      End
      Begin VB.Image output_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7348.frx":126A
         Top             =   1035
         Width           =   165
      End
      Begin VB.Image output_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7348.frx":1344
         Top             =   555
         Width           =   165
      End
      Begin VB.Image input_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7348.frx":141E
         Top             =   840
         Width           =   165
      End
      Begin VB.Image input_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7348.frx":14F8
         Top             =   360
         Width           =   165
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   15
         Left            =   120
         Picture         =   "UTIL7348.frx":15D2
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   14
         Left            =   330
         Picture         =   "UTIL7348.frx":16B4
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   13
         Left            =   540
         Picture         =   "UTIL7348.frx":1796
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   12
         Left            =   750
         Picture         =   "UTIL7348.frx":1878
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   11
         Left            =   960
         Picture         =   "UTIL7348.frx":195A
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   10
         Left            =   1170
         Picture         =   "UTIL7348.frx":1A3C
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   9
         Left            =   1380
         Picture         =   "UTIL7348.frx":1B1E
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   8
         Left            =   1590
         Picture         =   "UTIL7348.frx":1C00
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   330
         Picture         =   "UTIL7348.frx":1CE2
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7348.frx":1DC4
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7348.frx":1EA6
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   960
         Picture         =   "UTIL7348.frx":1F88
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   1170
         Picture         =   "UTIL7348.frx":206A
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   1380
         Picture         =   "UTIL7348.frx":214C
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   1560
         Picture         =   "UTIL7348.frx":222E
         Top             =   480
         Width           =   180
      End
      Begin VB.Shape Shape_b 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   0
         Left            =   60
         Top             =   360
         Width           =   1800
      End
      Begin VB.Shape Shape_b 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   1
         Left            =   60
         Top             =   840
         Width           =   1800
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Port A"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Times New Roman"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   1455
      Index           =   0
      Left            =   660
      TabIndex        =   0
      Top             =   120
      Width           =   2115
      Begin VB.Image output_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7348.frx":2310
         Top             =   1035
         Width           =   165
      End
      Begin VB.Image output_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7348.frx":23EA
         Top             =   555
         Width           =   165
      End
      Begin VB.Image input_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7348.frx":24C4
         Top             =   840
         Width           =   165
      End
      Begin VB.Image input_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7348.frx":259E
         Top             =   360
         Width           =   165
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   15
         Left            =   120
         Picture         =   "UTIL7348.frx":2678
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   14
         Left            =   330
         Picture         =   "UTIL7348.frx":275A
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   13
         Left            =   540
         Picture         =   "UTIL7348.frx":283C
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   12
         Left            =   750
         Picture         =   "UTIL7348.frx":291E
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   11
         Left            =   960
         Picture         =   "UTIL7348.frx":2A00
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   10
         Left            =   1170
         Picture         =   "UTIL7348.frx":2AE2
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   9
         Left            =   1380
         Picture         =   "UTIL7348.frx":2BC4
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   8
         Left            =   1590
         Picture         =   "UTIL7348.frx":2CA6
         Top             =   960
         Width           =   180
      End
      Begin VB.Shape Shape_a 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   1
         Left            =   60
         Top             =   840
         Width           =   1800
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   7
         Left            =   120
         Picture         =   "UTIL7348.frx":2D88
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   330
         Picture         =   "UTIL7348.frx":2E6A
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7348.frx":2F4C
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7348.frx":302E
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   960
         Picture         =   "UTIL7348.frx":3110
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   1170
         Picture         =   "UTIL7348.frx":31F2
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   1380
         Picture         =   "UTIL7348.frx":32D4
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   1590
         Picture         =   "UTIL7348.frx":33B6
         Top             =   480
         Width           =   180
      End
      Begin VB.Shape Shape_a 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   0
         Left            =   60
         Top             =   360
         Width           =   1800
      End
   End
   Begin VB.Image doc_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1380
      Picture         =   "UTIL7348.frx":3498
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dic_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1380
      Picture         =   "UTIL7348.frx":357A
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dib_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1080
      Picture         =   "UTIL7348.frx":365C
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dob_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1080
      Picture         =   "UTIL7348.frx":373E
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Ch1"
      ForeColor       =   &H00FF0000&
      Height          =   195
      Index           =   0
      Left            =   180
      TabIndex        =   1
      Top             =   570
      Width           =   375
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Ch2"
      ForeColor       =   &H00FF0000&
      Height          =   195
      Index           =   1
      Left            =   180
      TabIndex        =   3
      Top             =   1050
      Width           =   375
   End
   Begin VB.Shape Shape3 
      BackColor       =   &H00C0C0C0&
      FillColor       =   &H00C0C0C0&
      FillStyle       =   0  'Solid
      Height          =   1350
      Left            =   120
      Top             =   225
      Width           =   495
   End
   Begin VB.Image doa_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   780
      Picture         =   "UTIL7348.frx":3820
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image do_0 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   480
      Picture         =   "UTIL7348.frx":3902
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dia_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   780
      Picture         =   "UTIL7348.frx":39E4
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image di_0 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   480
      Picture         =   "UTIL7348.frx":3AC6
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Menu mnuExit 
      Caption         =   "E&xit!"
   End
End
Attribute VB_Name = "Util7348"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Const INPUT_COLOR = &HFF00FF
Const OUTPUT_COLOR = &HFF0000

Dim value_a(0 To 1) As Long, value_b(0 To 1) As Long, value_c(0 To 3) As Long
Dim status_a(0 To 1) As Integer, status_b(0 To 1) As Integer, status_c(0 To 1) As Integer
'Dim hEnts As EventHandles
Private Sub bit_a_Click(Index As Integer)
  Dim Channel As Integer
  Dim bit As Integer
  Dim result As Integer
  Channel = Int(Index / 8)
  bit = Index Mod 8
  If status_a(Channel) = OUTPUT_PORT Then
    If (Int((value_a(Channel) / (2 ^ bit))) Mod 2) = 1 Then
      bit_a(Index) = do_0
      value_a(Channel) = value_a(Channel) - (2 ^ bit)
    Else  'bit is 0
      bit_a(Index) = doa_1
      value_a(Channel) = value_a(Channel) + (2 ^ bit)
    End If
       result = DO_WritePort(card, Channel * 5 + Channel_P1A, value_a(Channel))
  End If
End Sub

Private Sub bit_b_Click(Index As Integer)
  Dim Channel As Integer
  Dim bit As Integer
  Dim result As Integer
  Channel = Int(Index / 8)
  bit = Index Mod 8
  If status_b(Channel) = OUTPUT_PORT Then
    If (Int((value_b(Channel) / (2 ^ bit))) Mod 2) = 1 Then
      bit_b(Index) = do_0
      value_b(Channel) = value_b(Channel) - (2 ^ bit)
    Else  'bit is 0
      bit_b(Index) = dob_1
      value_b(Channel) = value_b(Channel) + (2 ^ bit)
    End If
       result = DO_WritePort(card, Channel * 5 + Channel_P1B, value_b(Channel))
  End If
End Sub

Private Sub bit_c_Click(Index As Integer)
  Dim Channel As Integer
  Dim bit As Integer
  Dim result As Integer
  Channel = Int(Index / 8)
  bit = Index Mod 8
  If status_c(Channel) = OUTPUT_PORT Then
    If (Int((value_c(Channel) / (2 ^ bit))) Mod 2) = 1 Then
      bit_c(Index) = do_0
      value_c(Channel) = value_c(Channel) - (2 ^ bit)
    Else  'bit is 0
      bit_c(Index) = doc_1
      value_c(Channel) = value_c(Channel) + (2 ^ bit)
    End If
       result = DO_WritePort(card, Channel * 5 + Channel_P1C, value_c(Channel))
  End If
End Sub

Private Sub Card_Initial()
  Dim i As Integer, j As Integer
  Dim result As Integer
  
  For i = 0 To 1  'Initial status is Output for all channels
    result = DIO_PortConfig(card, i * 5 + Channel_P1A, OUTPUT_PORT)
    Shape_a(i).FillColor = OUTPUT_COLOR
    status_a(i) = OUTPUT_PORT
    For j = 0 To 7
      bit_a(i * 8 + j) = doa_1
    Next j
    value_a(i) = &HFF
    result = DO_WritePort(card, i * 5 + Channel_P1A, value_a(i))

    result = DIO_PortConfig(card, i * 5 + Channel_P1B, OUTPUT_PORT)
    Shape_b(i).FillColor = OUTPUT_COLOR
    status_b(i) = OUTPUT_PORT
    For j = 0 To 7
      bit_b(i * 8 + j) = dob_1
    Next j
    value_b(i) = &HFF
    result = DO_WritePort(card, i * 5 + Channel_P1B, value_b(i))

    result = DIO_PortConfig(card, i * 5 + Channel_P1C, OUTPUT_PORT)
    Shape_c(i).FillColor = OUTPUT_COLOR
    status_c(i) = OUTPUT_PORT
    For j = 0 To 7
      bit_c(i * 8 + j) = doc_1
    Next j
    value_c(i) = &HFF
    result = DO_WritePort(card, i * 5 + Channel_P1C, value_c(i))
  Next i
End Sub

Private Sub Form_Load()
  Dim result As Integer
  SettingForm.Show 1
  If card < 0 Then
        MsgBox "Register Card Failed"
        End
  End If
  Card_Initial
End Sub

Private Sub Form_Unload(Cancel As Integer)
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
  End
End Sub

Private Sub input_a_Click(Index As Integer)
    Dim result As Integer
    Shape_a(Index).FillColor = INPUT_COLOR
    InputAndShow Index, Channel_P1A
    status_a(Index) = INPUT_PORT
        result = DIO_PortConfig(card, Index * 5 + Channel_P1A, INPUT_PORT)
End Sub

Private Sub input_b_Click(Index As Integer)
    Dim result As Integer
    Shape_b(Index).FillColor = INPUT_COLOR
    InputAndShow Index, Channel_P1B
    status_b(Index) = INPUT_PORT
        result = DIO_PortConfig(card, Index * 5 + Channel_P1B, INPUT_PORT)
End Sub

Private Sub input_c_Click(Index As Integer)
    Dim result As Integer
    Shape_c(Index).FillColor = INPUT_COLOR
    InputAndShow Index, Channel_P1C
    status_c(Index) = INPUT_PORT
    result = DIO_PortConfig(card, Index * 5 + Channel_P1C, INPUT_PORT)
End Sub

Private Sub InputAndShow(ByVal Channel As Integer, ByVal Port As Integer)
  Dim databyte As Long
  Dim result As Integer
  Dim i As Integer, k As Integer
  Dim p As Integer, q As Integer, r As Integer
    result = DI_ReadPort(card, Channel * 5 + Port, databyte)
  p = databyte
  Select Case Port
  Case Channel_P1A
    k = Channel * 8
    For i = k To k + 7
      r = p Mod 2
      If r = 0 Then
        bit_a(i) = di_0
      Else    'r = 1
        bit_a(i) = dia_1
      End If
      p = Int(p / 2)
    Next
    value_a(Channel) = databyte
  Case Channel_P1B
    k = Channel * 8
    For i = k To k + 7
      r = p Mod 2
      If r = 0 Then
        bit_b(i) = di_0
      Else    'r = 1
        bit_b(i) = dib_1
      End If
      p = Int(p / 2)
    Next
    value_b(Channel) = databyte
  Case Channel_P1C
    k = Channel * 8
    For i = k To k + 7
      r = p Mod 2
      If r = 0 Then
        bit_c(i) = di_0
      Else    'r = 1
        bit_c(i) = dic_1
      End If
      p = Int(p / 2)
    Next
    value_c(Channel) = databyte
 End Select
End Sub

Private Sub mnuExit_Click()
Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
    End
End Sub

Private Sub output_a_Click(Index As Integer)
  Dim i As Integer, k As Integer
  Shape_a(Index).FillColor = OUTPUT_COLOR
  value_a(Index) = &HFF
  k = Index * 8
  For i = k To k + 7
    bit_a(i) = doa_1
  Next
  k = DIO_PortConfig(card, Index * 5 + Channel_P1A, OUTPUT_PORT)
  k = DO_WritePort(card, Index * 5 + Channel_P1A, &HFF)
  status_a(Index) = OUTPUT_PORT
End Sub

Private Sub output_b_Click(Index As Integer)
  Dim i As Integer, k As Integer
  Shape_b(Index).FillColor = OUTPUT_COLOR
  value_b(Index) = &HFF
  k = Index * 8
  For i = k To k + 7
    bit_b(i) = dob_1
  Next
  k = DIO_PortConfig(card, Index * 5 + Channel_P1B, OUTPUT_PORT)
  k = DO_WritePort(card, Index * 5 + Channel_P1B, &HFF)
  status_b(Index) = OUTPUT_PORT
End Sub

Private Sub output_c_Click(Index As Integer)
  Dim i As Integer, k As Integer
  Shape_c(Index).FillColor = OUTPUT_COLOR
  value_c(Index) = &HFF
  k = Index * 8
  For i = k To k + 7
    bit_c(i) = doc_1
  Next
  k = DIO_PortConfig(card, Index * 5 + Channel_P1C, OUTPUT_PORT)
  k = DO_WritePort(card, Index * 5 + Channel_P1C, &HFF)
  status_c(Index) = OUTPUT_PORT
End Sub

Private Sub Timer1_Timer()
  Dim i As Integer, result As Long
  For i = 0 To 1
    If status_a(i) = INPUT_PORT Then
      InputAndShow i, Channel_P1A
    End If
    If status_b(i) = INPUT_PORT Then
      InputAndShow i, Channel_P1B
    End If
    If status_c(i) = INPUT_PORT Then
      InputAndShow i, Channel_P1C
    End If
  Next
End Sub

