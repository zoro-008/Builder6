VERSION 4.00
Begin VB.Form util9113 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "9113 Sample Program"
   ClientHeight    =   4170
   ClientLeft      =   1020
   ClientTop       =   1230
   ClientWidth     =   8520
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
   Height          =   4575
   Left            =   960
   LinkTopic       =   "Form1"
   ScaleHeight     =   278
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   568
   Top             =   885
   Width           =   8640
   Begin VB.CommandButton ExitBtn 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Exit"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   5130
      TabIndex        =   42
      Top             =   3615
      Width           =   795
   End
   Begin VB.Frame Frame3D2 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Range"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   735
      Left            =   6180
      TabIndex        =   2
      Top             =   3300
      Width           =   2055
      Begin VB.ComboBox Gain 
         Height          =   315
         Left            =   300
         TabIndex        =   3
         Text            =   "Gain"
         Top             =   315
         Width           =   1395
      End
   End
   Begin VB.Frame Channel_Frame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Channel"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   1335
      Index           =   1
      Left            =   6360
      TabIndex        =   5
      Top             =   1680
      Width           =   1575
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "15"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   24
         Left            =   780
         TabIndex        =   6
         Top             =   960
         Width           =   240
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "14"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   25
         Left            =   780
         TabIndex        =   7
         Top             =   720
         Width           =   240
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "13"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   26
         Left            =   780
         TabIndex        =   8
         Top             =   480
         Width           =   240
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "12"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   27
         Left            =   780
         TabIndex        =   9
         Top             =   240
         Width           =   240
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "11"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   28
         Left            =   120
         TabIndex        =   10
         Top             =   960
         Width           =   240
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "10"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   29
         Left            =   120
         TabIndex        =   11
         Top             =   720
         Width           =   240
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "9"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   30
         Left            =   210
         TabIndex        =   12
         Top             =   480
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "8"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   31
         Left            =   210
         TabIndex        =   13
         Top             =   240
         Width           =   135
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   8
         Left            =   390
         Picture         =   "vb9113.frx":0000
         Stretch         =   -1  'True
         Top             =   240
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   9
         Left            =   390
         Picture         =   "vb9113.frx":012A
         Stretch         =   -1  'True
         Top             =   480
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   10
         Left            =   390
         Picture         =   "vb9113.frx":0254
         Stretch         =   -1  'True
         Top             =   720
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   11
         Left            =   390
         Picture         =   "vb9113.frx":037E
         Stretch         =   -1  'True
         Top             =   960
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   12
         Left            =   1050
         Picture         =   "vb9113.frx":04A8
         Stretch         =   -1  'True
         Top             =   240
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   13
         Left            =   1050
         Picture         =   "vb9113.frx":05D2
         Stretch         =   -1  'True
         Top             =   480
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   14
         Left            =   1050
         Picture         =   "vb9113.frx":06FC
         Stretch         =   -1  'True
         Top             =   720
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   15
         Left            =   1050
         Picture         =   "vb9113.frx":0826
         Stretch         =   -1  'True
         Top             =   960
         Width           =   270
      End
   End
   Begin VB.Frame Channel_Frame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Channel"
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   1335
      Index           =   0
      Left            =   6360
      TabIndex        =   14
      Top             =   360
      Width           =   1575
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   7
         Left            =   1080
         Picture         =   "vb9113.frx":0950
         Stretch         =   -1  'True
         Top             =   960
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   6
         Left            =   1080
         Picture         =   "vb9113.frx":0A7A
         Stretch         =   -1  'True
         Top             =   720
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   5
         Left            =   1080
         Picture         =   "vb9113.frx":0BA4
         Stretch         =   -1  'True
         Top             =   480
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   4
         Left            =   1080
         Picture         =   "vb9113.frx":0CCE
         Stretch         =   -1  'True
         Top             =   240
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   3
         Left            =   420
         Picture         =   "vb9113.frx":0DF8
         Stretch         =   -1  'True
         Top             =   960
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   2
         Left            =   420
         Picture         =   "vb9113.frx":0F22
         Stretch         =   -1  'True
         Top             =   720
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   1
         Left            =   420
         Picture         =   "vb9113.frx":104C
         Stretch         =   -1  'True
         Top             =   480
         Width           =   270
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   0
         Left            =   420
         Picture         =   "vb9113.frx":1176
         Stretch         =   -1  'True
         Top             =   240
         Width           =   270
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   14
         Left            =   240
         TabIndex        =   15
         Top             =   240
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "1"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   13
         Left            =   240
         TabIndex        =   16
         Top             =   480
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   12
         Left            =   240
         TabIndex        =   17
         Top             =   720
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "3"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   11
         Left            =   240
         TabIndex        =   18
         Top             =   960
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "4"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   10
         Left            =   900
         TabIndex        =   19
         Top             =   240
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   9
         Left            =   900
         TabIndex        =   20
         Top             =   480
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "6"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   8
         Left            =   900
         TabIndex        =   21
         Top             =   720
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "7"
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   7
         Left            =   900
         TabIndex        =   22
         Top             =   960
         Width           =   135
      End
   End
   Begin VB.Frame AD_Frame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Analog Input (A/D)"
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
      Height          =   3255
      Left            =   240
      TabIndex        =   28
      Top             =   0
      Width           =   7935
      Begin VB.PictureBox Signal 
         Appearance      =   0  'Flat
         BackColor       =   &H00000000&
         BorderStyle     =   0  'None
         ForeColor       =   &H80000008&
         Height          =   1200
         Index           =   0
         Left            =   900
         ScaleHeight     =   80
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   333
         TabIndex        =   23
         Top             =   480
         Width           =   4995
      End
      Begin VB.PictureBox Signal 
         Appearance      =   0  'Flat
         BackColor       =   &H00000000&
         BorderStyle     =   0  'None
         ForeColor       =   &H80000008&
         Height          =   1200
         Index           =   1
         Left            =   900
         ScaleHeight     =   80
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   333
         TabIndex        =   37
         Top             =   1800
         Width           =   4995
      End
      Begin VB.Line Line1 
         Index           =   4
         X1              =   840
         X2              =   900
         Y1              =   1665
         Y2              =   1665
      End
      Begin VB.Line Line1 
         Index           =   3
         X1              =   840
         X2              =   900
         Y1              =   1380
         Y2              =   1380
      End
      Begin VB.Line Line1 
         Index           =   2
         X1              =   840
         X2              =   900
         Y1              =   1080
         Y2              =   1080
      End
      Begin VB.Line Line1 
         Index           =   1
         X1              =   840
         X2              =   900
         Y1              =   780
         Y2              =   780
      End
      Begin VB.Line Line1 
         Index           =   0
         X1              =   840
         X2              =   900
         Y1              =   480
         Y2              =   480
      End
      Begin VB.Line Line1 
         Index           =   5
         X1              =   840
         X2              =   900
         Y1              =   1800
         Y2              =   1800
      End
      Begin VB.Line Line1 
         Index           =   6
         X1              =   840
         X2              =   900
         Y1              =   2100
         Y2              =   2100
      End
      Begin VB.Line Line1 
         Index           =   7
         X1              =   840
         X2              =   900
         Y1              =   2400
         Y2              =   2400
      End
      Begin VB.Line Line1 
         Index           =   8
         X1              =   840
         X2              =   900
         Y1              =   2700
         Y2              =   2700
      End
      Begin VB.Line Line1 
         Index           =   9
         X1              =   840
         X2              =   900
         Y1              =   2985
         Y2              =   2985
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-5.00"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   6
         Left            =   345
         TabIndex        =   24
         Top             =   1590
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-2.50"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   5
         Left            =   345
         TabIndex        =   25
         Top             =   1350
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0.00"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   4
         Left            =   345
         TabIndex        =   26
         Top             =   1050
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2.50"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   3
         Left            =   345
         TabIndex        =   27
         Top             =   750
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5.00"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   2
         Left            =   345
         TabIndex        =   29
         Top             =   450
         Width           =   480
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "V"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   1
         Left            =   240
         TabIndex        =   30
         Top             =   690
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Signals"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   0
         Left            =   3060
         TabIndex        =   31
         Top             =   240
         Width           =   735
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "O"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   15
         Left            =   240
         TabIndex        =   32
         Top             =   1110
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "L"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   16
         Left            =   240
         TabIndex        =   33
         Top             =   1470
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "T"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   17
         Left            =   240
         TabIndex        =   34
         Top             =   1890
         Width           =   135
      End
      Begin VB.Label Label1 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "S"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   18
         Left            =   240
         TabIndex        =   35
         Top             =   2310
         Width           =   135
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5.00"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   19
         Left            =   345
         TabIndex        =   36
         Top             =   1800
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2.50"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   20
         Left            =   345
         TabIndex        =   38
         Top             =   2070
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0.00"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   21
         Left            =   345
         TabIndex        =   39
         Top             =   2370
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-2.50"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   22
         Left            =   345
         TabIndex        =   40
         Top             =   2670
         Width           =   480
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "-5.00"
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Serif"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H80000008&
         Height          =   195
         Index           =   23
         Left            =   345
         TabIndex        =   41
         Top             =   2910
         Width           =   480
      End
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   70
      Left            =   840
      Top             =   3360
   End
   Begin VB.CommandButton StopBtn 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
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
      Height          =   315
      Left            =   2940
      TabIndex        =   4
      Top             =   3615
      Width           =   795
   End
   Begin VB.CommandButton StartBtn 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
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
      Height          =   315
      Left            =   2040
      TabIndex        =   0
      Top             =   3615
      Width           =   795
   End
   Begin VB.CommandButton PauseBtn 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Pause"
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
      Height          =   315
      Left            =   3840
      TabIndex        =   1
      Top             =   3615
      Width           =   795
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   2280
      Picture         =   "vb9113.frx":12A0
      Top             =   3900
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   2580
      Picture         =   "vb9113.frx":1412
      Top             =   3900
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   0
      Left            =   -300
      Picture         =   "vb9113.frx":1584
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   1
      Left            =   -60
      Picture         =   "vb9113.frx":16AE
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   2
      Left            =   180
      Picture         =   "vb9113.frx":17D8
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   3
      Left            =   420
      Picture         =   "vb9113.frx":1902
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   4
      Left            =   660
      Picture         =   "vb9113.frx":1A2C
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   5
      Left            =   900
      Picture         =   "vb9113.frx":1B56
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   6
      Left            =   1140
      Picture         =   "vb9113.frx":1C80
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   7
      Left            =   1380
      Picture         =   "vb9113.frx":1DAA
      Top             =   3840
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   0
      Left            =   -300
      Picture         =   "vb9113.frx":1ED4
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   1
      Left            =   -60
      Picture         =   "vb9113.frx":1FFE
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   2
      Left            =   180
      Picture         =   "vb9113.frx":2128
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   3
      Left            =   420
      Picture         =   "vb9113.frx":2252
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   4
      Left            =   660
      Picture         =   "vb9113.frx":237C
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   5
      Left            =   900
      Picture         =   "vb9113.frx":24A6
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   6
      Left            =   1140
      Picture         =   "vb9113.frx":25D0
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   7
      Left            =   1380
      Picture         =   "vb9113.frx":26FA
      Top             =   4020
      Visible         =   0   'False
      Width           =   270
   End
End
Attribute VB_Name = "util9113"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim PrevP(15) As Long
Dim ChannelOn(15) As Long
Dim SignalColor(15) As Long
Dim range_value(8) As Integer
Dim Active As Long
Dim NewStart As Long
Dim status(15) As Long
Dim gnAddr As Long
Dim gnIrq As Long

Private Sub Channel_Click(Index As Integer)
  If ChannelOn(Index) Then
    If Index < 8 Then
      Channel(Index) = ChUp(Index)
    Else
      Channel(Index) = ChUp(Index - 8)
    End If
  Else
    If Index < 8 Then
      Channel(Index) = ChDn(Index)
    Else
      Channel(Index) = ChDn(Index - 8)
    End If
  End If
  ChannelOn(Index) = Not ChannelOn(Index)
End Sub



Private Sub ExitBtn_Click()
    Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
    End
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
    PrevP(i) = -1
    ChannelOn(i) = False
  Next
  For i = 0 To 15
    status(i) = 0
  Next
  NewStart = True
  Active = False

  gain.AddItem "+/- 10V"
  gain.AddItem "+/- 1V"
  gain.AddItem "+/- 0.1V"
  gain.AddItem "+/- 5V"
  gain.AddItem "+/- 0.5V"
  gain.AddItem "+/- 0.05V"
  gain.AddItem "0~10V"
  gain.AddItem "0~1V"
  gain.AddItem "0~0.1V"
  
  range_value(0) = 1
  range_value(1) = 10
  range_value(2) = 11
  range_value(3) = 2
  range_value(4) = 7
  range_value(5) = 8
  range_value(6) = 15
  range_value(7) = 19
  range_value(8) = 20
  
  gain.ListIndex = 0
  
  SignalColor(0) = &HFFFF&
  SignalColor(1) = &H8000&
  SignalColor(2) = &HFF0000
  SignalColor(3) = &HFF&
  SignalColor(4) = &HFFFF00
  SignalColor(5) = &HFF00FF
  SignalColor(6) = &HFF00&
  SignalColor(7) = &HFFFFFF
  SignalColor(8) = &HFFFF&
  SignalColor(9) = &H8000&
  SignalColor(10) = &HFF0000
  SignalColor(11) = &HFF&
  SignalColor(12) = &HFFFF00
  SignalColor(13) = &HFF00FF
  SignalColor(14) = &HFF00&
  SignalColor(15) = &HFFFFFF

End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
End Sub


Private Sub Gain_Click()
  Dim result As Long
        Scale_Change gain.ListIndex
End Sub













Private Sub PauseBtn_Click()
  Timer1.Enabled = False
  Active = False
  NewStart = False
  StartBtn.Enabled = True
  PauseBtn.Enabled = False
  
End Sub




Private Sub StartBtn_Click()
  StartBtn.Enabled = False
  StopBtn.Enabled = True
  PauseBtn.Enabled = True
  
  Active = True

  If NewStart Then
    Signal(0).Cls
    Signal(1).Cls
  End If
  Timer1.Enabled = True
End Sub

Private Sub StopBtn_Click()
  Dim i As Integer
  Timer1.Enabled = False
  For i = 0 To 15
    PrevP(i) = -1
  Next
  For i = 0 To 15
    status(i) = 0
  Next
  Active = False
  NewStart = True
  StartBtn.Enabled = True
  StopBtn.Enabled = False
  PauseBtn.Enabled = False
End Sub

Private Sub Timer1_Timer()
  Dim result As Integer
  Dim i As Integer, j As Integer, d As Integer
  Dim l As Long
  Dim di_data As Long
  Dim p As Integer
  Dim m As Single
  Static vt(15) As Variant
  
'  Do While Contd
'Analog Input
    result = BitBlt(Signal(0).hDC, 4, 0, Signal(0).Width / Screen.TwipsPerPixelX, Signal(0).Height / Screen.TwipsPerPixelY, Signal(0).hDC, 0, 0, SRCCOPY)
    result = BitBlt(Signal(1).hDC, 4, 0, Signal(1).Width / Screen.TwipsPerPixelX, Signal(1).Height / Screen.TwipsPerPixelY, Signal(1).hDC, 0, 0, SRCCOPY)
    For i = 0 To 15
      If ChannelOn(i) Then
         result = AI_ReadChannel(card, i, range_value(gain.ListIndex), d)
         l = d
         If i < 8 Then
           p = (4095 - CSng(l)) / 4095 * (Signal(0).Height / Screen.TwipsPerPixelY - 2)
           If PrevP(i) <> -1 Then Signal(0).Line (4, p)-(8, PrevP(i)), SignalColor(i)
         Else
           p = (4095 - CSng(l)) / 4095 * (Signal(1).Height / Screen.TwipsPerPixelY - 2)
           If PrevP(i) <> -1 Then Signal(1).Line (4, p)-(8, PrevP(i)), SignalColor(i)
         End If
         PrevP(i) = p
      End If
    Next i
End Sub




