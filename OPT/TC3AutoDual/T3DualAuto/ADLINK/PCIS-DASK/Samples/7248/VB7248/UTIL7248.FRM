VERSION 4.00
Begin VB.Form uti48dio 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "PCI-7248 Sample Program"
   ClientHeight    =   1890
   ClientLeft      =   120
   ClientTop       =   2370
   ClientWidth     =   7680
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
   Height          =   2580
   Left            =   60
   LinkTopic       =   "Form1"
   ScaleHeight     =   1890
   ScaleWidth      =   7680
   Top             =   1740
   Width           =   7800
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   7080
      Top             =   1680
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Port C (Lower)"
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
      Index           =   3
      Left            =   6300
      TabIndex        =   2
      Top             =   120
      Width           =   1275
      Begin VB.Image output_cl 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1035
         Picture         =   "UTIL7248.frx":0000
         Top             =   1035
         Width           =   165
      End
      Begin VB.Image output_cl 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1035
         Picture         =   "UTIL7248.frx":00DA
         Top             =   555
         Width           =   165
      End
      Begin VB.Image input_cl 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1035
         Picture         =   "UTIL7248.frx":01B4
         Top             =   840
         Width           =   165
      End
      Begin VB.Image input_cl 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1035
         Picture         =   "UTIL7248.frx":028E
         Top             =   360
         Width           =   165
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   7
         Left            =   120
         Picture         =   "UTIL7248.frx":0368
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   330
         Picture         =   "UTIL7248.frx":044A
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7248.frx":052C
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7248.frx":060E
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   120
         Picture         =   "UTIL7248.frx":06F0
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   330
         Picture         =   "UTIL7248.frx":07D2
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   540
         Picture         =   "UTIL7248.frx":08B4
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_cl 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   750
         Picture         =   "UTIL7248.frx":0996
         Top             =   480
         Width           =   180
      End
      Begin VB.Shape Shape_cl 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   1
         Left            =   60
         Top             =   840
         Width           =   960
      End
      Begin VB.Shape Shape_cl 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   0
         Left            =   60
         Top             =   360
         Width           =   960
      End
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Port C (Upper)"
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
      Left            =   2820
      TabIndex        =   3
      Top             =   120
      Width           =   1275
      Begin VB.Image output_cu 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1035
         Picture         =   "UTIL7248.frx":0A78
         Top             =   1035
         Width           =   165
      End
      Begin VB.Image output_cu 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1035
         Picture         =   "UTIL7248.frx":0B52
         Top             =   555
         Width           =   165
      End
      Begin VB.Image input_cu 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1035
         Picture         =   "UTIL7248.frx":0C2C
         Top             =   840
         Width           =   165
      End
      Begin VB.Image input_cu 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1035
         Picture         =   "UTIL7248.frx":0D06
         Top             =   360
         Width           =   165
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   7
         Left            =   120
         Picture         =   "UTIL7248.frx":0DE0
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   330
         Picture         =   "UTIL7248.frx":0EC2
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7248.frx":0FA4
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7248.frx":1086
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   120
         Picture         =   "UTIL7248.frx":1168
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   330
         Picture         =   "UTIL7248.frx":124A
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   540
         Picture         =   "UTIL7248.frx":132C
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_cu 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   750
         Picture         =   "UTIL7248.frx":140E
         Top             =   480
         Width           =   180
      End
      Begin VB.Shape Shape_cu 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   0
         Left            =   60
         Top             =   360
         Width           =   960
      End
      Begin VB.Shape Shape_cu 
         BorderColor     =   &H00C0C0C0&
         FillColor       =   &H00FF0000&
         FillStyle       =   0  'Solid
         Height          =   375
         Index           =   1
         Left            =   60
         Top             =   840
         Width           =   960
      End
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
      Left            =   4140
      TabIndex        =   4
      Top             =   120
      Width           =   2115
      Begin VB.Image output_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7248.frx":14F0
         Top             =   1035
         Width           =   165
      End
      Begin VB.Image output_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7248.frx":15CA
         Top             =   555
         Width           =   165
      End
      Begin VB.Image input_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7248.frx":16A4
         Top             =   840
         Width           =   165
      End
      Begin VB.Image input_b 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7248.frx":177E
         Top             =   360
         Width           =   165
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   15
         Left            =   120
         Picture         =   "UTIL7248.frx":1858
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   14
         Left            =   330
         Picture         =   "UTIL7248.frx":193A
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   13
         Left            =   540
         Picture         =   "UTIL7248.frx":1A1C
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   12
         Left            =   750
         Picture         =   "UTIL7248.frx":1AFE
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   11
         Left            =   960
         Picture         =   "UTIL7248.frx":1BE0
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   10
         Left            =   1170
         Picture         =   "UTIL7248.frx":1CC2
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   9
         Left            =   1380
         Picture         =   "UTIL7248.frx":1DA4
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   8
         Left            =   1590
         Picture         =   "UTIL7248.frx":1E86
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   7
         Left            =   120
         Picture         =   "UTIL7248.frx":1F68
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   330
         Picture         =   "UTIL7248.frx":204A
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7248.frx":212C
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7248.frx":220E
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   960
         Picture         =   "UTIL7248.frx":22F0
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   1170
         Picture         =   "UTIL7248.frx":23D2
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   1380
         Picture         =   "UTIL7248.frx":24B4
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_b 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   1590
         Picture         =   "UTIL7248.frx":2596
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
         Picture         =   "UTIL7248.frx":2678
         Top             =   1035
         Width           =   165
      End
      Begin VB.Image output_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7248.frx":2752
         Top             =   555
         Width           =   165
      End
      Begin VB.Image input_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   1
         Left            =   1875
         Picture         =   "UTIL7248.frx":282C
         Top             =   840
         Width           =   165
      End
      Begin VB.Image input_a 
         Appearance      =   0  'Flat
         Height          =   165
         Index           =   0
         Left            =   1875
         Picture         =   "UTIL7248.frx":2906
         Top             =   360
         Width           =   165
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   15
         Left            =   120
         Picture         =   "UTIL7248.frx":29E0
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   14
         Left            =   330
         Picture         =   "UTIL7248.frx":2AC2
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   13
         Left            =   540
         Picture         =   "UTIL7248.frx":2BA4
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   12
         Left            =   750
         Picture         =   "UTIL7248.frx":2C86
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   11
         Left            =   960
         Picture         =   "UTIL7248.frx":2D68
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   10
         Left            =   1170
         Picture         =   "UTIL7248.frx":2E4A
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   9
         Left            =   1380
         Picture         =   "UTIL7248.frx":2F2C
         Top             =   960
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   8
         Left            =   1590
         Picture         =   "UTIL7248.frx":300E
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
         Picture         =   "UTIL7248.frx":30F0
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   6
         Left            =   330
         Picture         =   "UTIL7248.frx":31D2
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   5
         Left            =   540
         Picture         =   "UTIL7248.frx":32B4
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   4
         Left            =   750
         Picture         =   "UTIL7248.frx":3396
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   3
         Left            =   960
         Picture         =   "UTIL7248.frx":3478
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   2
         Left            =   1170
         Picture         =   "UTIL7248.frx":355A
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   1
         Left            =   1380
         Picture         =   "UTIL7248.frx":363C
         Top             =   480
         Width           =   180
      End
      Begin VB.Image bit_a 
         Appearance      =   0  'Flat
         Height          =   180
         Index           =   0
         Left            =   1590
         Picture         =   "UTIL7248.frx":371E
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
      Picture         =   "UTIL7248.frx":3800
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dic_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1380
      Picture         =   "UTIL7248.frx":38E2
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dib_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1080
      Picture         =   "UTIL7248.frx":39C4
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dob_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   1080
      Picture         =   "UTIL7248.frx":3AA6
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
      TabIndex        =   5
      Top             =   1050
      Width           =   375
   End
   Begin VB.Shape Shape3 
      BackColor       =   &H00C0C0C0&
      FillColor       =   &H00C0C0C0&
      FillStyle       =   0  'Solid
      Height          =   1335
      Left            =   120
      Top             =   230
      Width           =   495
   End
   Begin VB.Image doa_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   780
      Picture         =   "UTIL7248.frx":3B88
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image do_0 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   480
      Picture         =   "UTIL7248.frx":3C6A
      Top             =   4680
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image dia_1 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   780
      Picture         =   "UTIL7248.frx":3D4C
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image di_0 
      Appearance      =   0  'Flat
      Height          =   180
      Left            =   480
      Picture         =   "UTIL7248.frx":3E2E
      Top             =   4500
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Menu mnuExit 
      Caption         =   "E&xit!"
   End
End
Attribute VB_Name = "uti48dio"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Const INPUT_COLOR = &HFF00FF
Const OUTPUT_COLOR = &HFF0000

Dim value_a(0 To 1) As Long, value_b(0 To 1) As Long, value_cu(0 To 1) As Long, value_cl(0 To 1) As Long
Dim status_a(0 To 1) As Integer, status_b(0 To 1) As Integer, status_cu(0 To 1) As Long, status_cl(0 To 1) As Integer

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
    'result = W_7248_DO(0, Channel * 5 + Channel_P1A, value_a(Channel))
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
    'result = W_7248_DO(0, Channel * 5 + Channel_P1B, value_b(Channel))
    result = DO_WritePort(card, Channel * 5 + Channel_P1B, value_b(Channel))
  End If
End Sub

Private Sub bit_cl_Click(Index As Integer)
  Dim Channel As Integer
  Dim bit As Integer
  Dim result As Integer
  Channel = Int(Index / 4)
  bit = Index Mod 4
  If status_cl(Channel) = OUTPUT_PORT Then
    If (Int((value_cl(Channel) / (2 ^ bit))) Mod 2) = 1 Then
      bit_cl(Index) = do_0
      value_cl(Channel) = value_cl(Channel) - (2 ^ bit)
    Else  'bit is 0
      bit_cl(Index) = doc_1
      value_cl(Channel) = value_cl(Channel) + (2 ^ bit)
    End If
    'result = W_7248_DO(0, Channel * 5 + Channel_P1CL, value_cl(Channel))
    result = DO_WritePort(card, Channel * 5 + Channel_P1CL, value_cl(Channel))
  End If
End Sub

Private Sub bit_cu_Click(Index As Integer)
  Dim Channel As Integer
  Dim bit As Integer
  Dim result As Integer
  Channel = Int(Index / 4)
  bit = Index Mod 4
  If status_cu(Channel) = OUTPUT_PORT Then
    If (Int((value_cu(Channel) / (2 ^ bit))) Mod 2) = 1 Then
      bit_cu(Index) = do_0
      value_cu(Channel) = value_cu(Channel) - (2 ^ bit)
    Else  'bit is 0
      bit_cu(Index) = doc_1
      value_cu(Channel) = value_cu(Channel) + (2 ^ bit)
    End If
    'result = W_7248_DO(0, Channel * 5 + Channel_P1CH, value_cu(Channel))
    result = DO_WritePort(card, Channel * 5 + Channel_P1CH, value_cu(Channel))
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

    result = DIO_PortConfig(card, i * 5 + Channel_P1CH, OUTPUT_PORT)
    Shape_cu(i).FillColor = OUTPUT_COLOR
    status_cu(i) = OUTPUT_PORT
    For j = 0 To 3
      bit_cu(i * 4 + j) = doc_1
    Next j
    value_cu(i) = &HF
    result = DO_WritePort(card, i * 5 + Channel_P1CH, value_cu(i))

    result = DIO_PortConfig(card, i * 5 + Channel_P1CL, OUTPUT_PORT)
    Shape_cl(i).FillColor = OUTPUT_COLOR
    status_cl(i) = OUTPUT_PORT
    For j = 0 To 3
      bit_cl(i * 4 + j) = doc_1
    Next j
    value_cl(i) = &HF
    result = DO_WritePort(card, i * 5 + Channel_P1CL, value_cl(i))
  Next i
End Sub

Private Sub Form_Load()
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
    'result = W_7248_Config_Port(0, Index * 5 + Channel_P1A, INPUT_PORT)
    result = DIO_PortConfig(card, Index * 5 + Channel_P1A, INPUT_PORT)
End Sub

Private Sub input_b_Click(Index As Integer)
    Dim result As Integer
    Shape_b(Index).FillColor = INPUT_COLOR
    InputAndShow Index, Channel_P1B
    status_b(Index) = INPUT_PORT
    'result = W_7248_Config_Port(0, Index * 5 + Channel_P1B, INPUT_PORT)
    result = DIO_PortConfig(card, Index * 5 + Channel_P1B, INPUT_PORT)
End Sub

Private Sub input_cl_Click(Index As Integer)
    Dim result As Integer
    Shape_cl(Index).FillColor = INPUT_COLOR
    InputAndShow Index, Channel_P1CL
    status_cl(Index) = INPUT_PORT
    'result = W_7248_Config_Port(0, Index * 5 + Channel_P1CL, INPUT_PORT)
    result = DIO_PortConfig(card, Index * 5 + Channel_P1CL, INPUT_PORT)
End Sub

Private Sub input_cu_Click(Index As Integer)
    Dim result As Integer
    Shape_cu(Index).FillColor = INPUT_COLOR
    InputAndShow Index, Channel_P1CH
    status_cu(Index) = INPUT_PORT
    result = DIO_PortConfig(card, Index * 5 + Channel_P1CH, INPUT_PORT)
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
  Case Channel_P1CH
    k = Channel * 4
    For i = k To k + 3
      r = p Mod 2
      If r = 0 Then
        bit_cu(i) = di_0
      Else    'r = 1
        bit_cu(i) = dic_1
      End If
      p = Int(p / 2)
    Next
    value_cu(Channel) = databyte
  Case Channel_P1CL
    k = Channel * 4
    For i = k To k + 3
      r = p Mod 2
      If r = 0 Then
        bit_cl(i) = di_0
      Else    'r = 1
        bit_cl(i) = dic_1
      End If
      p = Int(p / 2)
    Next
    value_cl(Channel) = databyte
'  Case PORTC
'    q = 0
'    k = channel * 8
'    For i = k To k + 3
'      r = p Mod 2
'      If r = 0 Then
'        bit_cl(i) = di_0
'      Else    'r = 1
'        bit_cl(i) = dic_1
'      End If
'      q = q + bit_cl(i) * 2 ^ (i - k)
'      p = Int(p / 2)
'    Next
'    value_cl(channel) = q
'    value_cu(channel) = p
'    For i = k To k + 3
'      If r = 0 Then
'        bit_cu(i) = di_0
'      Else    'r = 1
'        bit_cu(i) = dic_1
'      End If
'      p = Int(p / 2)
'    Next
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
  'k = W_7248_Config_Port(0, Index * 5 + Channel_P1A, OUTPUT_PORT)
  k = DIO_PortConfig(card, Index * 5 + Channel_P1A, OUTPUT_PORT)
  'k = W_7248_DO(0, Index * 5 + Channel_P1A, &HFF)
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
  'k = W_7248_Config_Port(0, Index * 5 + Channel_P1B, OUTPUT_PORT)
  k = DIO_PortConfig(card, Index * 5 + Channel_P1B, OUTPUT_PORT)
  'k = W_7248_DO(0, Index * 5 + Channel_P1B, &HFF)
  k = DO_WritePort(card, Index * 5 + Channel_P1B, &HFF)
  status_b(Index) = OUTPUT_PORT
End Sub

Private Sub output_cl_Click(Index As Integer)
  Dim i As Integer, k As Integer
  Shape_cl(Index).FillColor = OUTPUT_COLOR
  value_cl(Index) = &HF
  k = Index * 4
  For i = k To k + 3
    bit_cl(i) = doc_1
  Next
  'k = W_7248_Config_Port(0, Index * 5 + Channel_P1CL, OUTPUT_PORT)
  k = DIO_PortConfig(card, Index * 5 + Channel_P1CL, OUTPUT_PORT)
  'k = W_7248_DO(0, Index * 5 + Channel_P1CL, &HF)
  k = DO_WritePort(card, Index * 5 + Channel_P1CL, &HF)
  status_cl(Index) = OUTPUT_PORT
End Sub

Private Sub output_cu_Click(Index As Integer)
  Dim i As Integer, k As Integer
  Shape_cu(Index).FillColor = OUTPUT_COLOR
  value_cu(Index) = &HF
  k = Index * 4
  For i = k To k + 3
    bit_cu(i) = doc_1
  Next
  'k = W_7248_Config_Port(0, Index * 5 + Channel_P1CH, OUTPUT_PORT)
  k = DIO_PortConfig(card, Index * 5 + Channel_P1CH, OUTPUT_PORT)
  'k = W_7248_DO(0, Index * 5 + Channel_P1CH, &HF)
  k = DO_WritePort(card, Index * 5 + Channel_P1CH, &HF)
  status_cu(Index) = OUTPUT_PORT
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
    If status_cl(i) = INPUT_PORT Then
      InputAndShow i, Channel_P1CL
    End If
    If status_cu(i) = INPUT_PORT Then
      InputAndShow i, Channel_P1CH
    End If
  Next
End Sub

