VERSION 4.00
Begin VB.Form util9114 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "9114 Sample Program (AI0~AI15)"
   ClientHeight    =   5715
   ClientLeft      =   1095
   ClientTop       =   1650
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
   Height          =   6120
   Left            =   1035
   LinkTopic       =   "Form1"
   ScaleHeight     =   381
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   568
   Top             =   1305
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
      Left            =   6840
      TabIndex        =   45
      Top             =   5160
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
      Left            =   6240
      TabIndex        =   2
      Top             =   3420
      Width           =   2055
      Begin VB.ComboBox Gain 
         Appearance      =   0  'Flat
         Height          =   315
         ItemData        =   "vb9114.frx":0000
         Left            =   120
         List            =   "vb9114.frx":0019
         TabIndex        =   3
         Text            =   "Gain"
         Top             =   320
         Width           =   1575
      End
   End
   Begin VB.Frame DO_Frame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Output"
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
      Height          =   735
      Left            =   120
      TabIndex        =   5
      Top             =   4260
      Width           =   5895
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5520
         Picture         =   "vb9114.frx":0059
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5160
         Picture         =   "vb9114.frx":01CB
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4440
         Picture         =   "vb9114.frx":033D
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4800
         Picture         =   "vb9114.frx":04AF
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4080
         Picture         =   "vb9114.frx":0621
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3720
         Picture         =   "vb9114.frx":0793
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3360
         Picture         =   "vb9114.frx":0905
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3000
         Picture         =   "vb9114.frx":0A77
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2640
         Picture         =   "vb9114.frx":0BE9
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2280
         Picture         =   "vb9114.frx":0D5B
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1920
         Picture         =   "vb9114.frx":0ECD
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1560
         Picture         =   "vb9114.frx":103F
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1200
         Picture         =   "vb9114.frx":11B1
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   840
         Picture         =   "vb9114.frx":1323
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   480
         Picture         =   "vb9114.frx":1495
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   120
         Picture         =   "vb9114.frx":1607
         Top             =   280
         Width           =   300
      End
   End
   Begin VB.Frame DI_Frame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Digital Input"
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
      Height          =   735
      Left            =   120
      TabIndex        =   6
      Top             =   3300
      Width           =   5895
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   120
         Picture         =   "vb9114.frx":1779
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   480
         Picture         =   "vb9114.frx":18EB
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   840
         Picture         =   "vb9114.frx":1A5D
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1200
         Picture         =   "vb9114.frx":1BCF
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1560
         Picture         =   "vb9114.frx":1D41
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1920
         Picture         =   "vb9114.frx":1EB3
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2280
         Picture         =   "vb9114.frx":2025
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2640
         Picture         =   "vb9114.frx":2197
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3000
         Picture         =   "vb9114.frx":2309
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3360
         Picture         =   "vb9114.frx":247B
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3720
         Picture         =   "vb9114.frx":25ED
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4080
         Picture         =   "vb9114.frx":275F
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4440
         Picture         =   "vb9114.frx":28D1
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4800
         Picture         =   "vb9114.frx":2A43
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5160
         Picture         =   "vb9114.frx":2BB5
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5520
         Picture         =   "vb9114.frx":2D27
         Top             =   280
         Width           =   300
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
      TabIndex        =   8
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
         TabIndex        =   9
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
         TabIndex        =   10
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
         TabIndex        =   11
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
         TabIndex        =   12
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
         TabIndex        =   13
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
         TabIndex        =   14
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
         TabIndex        =   15
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
         TabIndex        =   16
         Top             =   240
         Width           =   135
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   8
         Left            =   390
         Picture         =   "vb9114.frx":2E99
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
         Picture         =   "vb9114.frx":2FC3
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
         Picture         =   "vb9114.frx":30ED
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
         Picture         =   "vb9114.frx":3217
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
         Picture         =   "vb9114.frx":3341
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
         Picture         =   "vb9114.frx":346B
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
         Picture         =   "vb9114.frx":3595
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
         Picture         =   "vb9114.frx":36BF
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
      TabIndex        =   17
      Top             =   360
      Width           =   1575
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   7
         Left            =   1080
         Picture         =   "vb9114.frx":37E9
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
         Picture         =   "vb9114.frx":3913
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
         Picture         =   "vb9114.frx":3A3D
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
         Picture         =   "vb9114.frx":3B67
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
         Picture         =   "vb9114.frx":3C91
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
         Picture         =   "vb9114.frx":3DBB
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
         Picture         =   "vb9114.frx":3EE5
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
         Picture         =   "vb9114.frx":400F
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
         TabIndex        =   18
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
         TabIndex        =   19
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
         TabIndex        =   20
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
         TabIndex        =   21
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
         TabIndex        =   22
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
         TabIndex        =   23
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
         TabIndex        =   24
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
         TabIndex        =   25
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
      TabIndex        =   31
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
         TabIndex        =   26
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
         TabIndex        =   40
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
         TabIndex        =   27
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
         TabIndex        =   28
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
         TabIndex        =   29
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
         TabIndex        =   30
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
         TabIndex        =   32
         Top             =   420
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
         TabIndex        =   33
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
         TabIndex        =   34
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
         TabIndex        =   35
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
         TabIndex        =   36
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
         TabIndex        =   37
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
         TabIndex        =   38
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
         TabIndex        =   39
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
         TabIndex        =   41
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
         TabIndex        =   42
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
         TabIndex        =   43
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
         TabIndex        =   44
         Top             =   2910
         Width           =   480
      End
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   70
      Left            =   300
      Top             =   5280
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
      Left            =   3480
      TabIndex        =   4
      Top             =   5160
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
      Left            =   2460
      TabIndex        =   0
      Top             =   5160
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
      Left            =   4560
      TabIndex        =   1
      Top             =   5160
      Width           =   795
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "15   14  13   12  11   10    9    8    7    6    5     4    3    2    1     0"
      ForeColor       =   &H80000008&
      Height          =   195
      Left            =   240
      TabIndex        =   7
      Top             =   4080
      Width           =   5775
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   960
      Picture         =   "vb9114.frx":4139
      Top             =   5400
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   1260
      Picture         =   "vb9114.frx":42AB
      Top             =   5400
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   0
      Left            =   6360
      Picture         =   "vb9114.frx":441D
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   1
      Left            =   6600
      Picture         =   "vb9114.frx":4547
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   2
      Left            =   6840
      Picture         =   "vb9114.frx":4671
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   3
      Left            =   7080
      Picture         =   "vb9114.frx":479B
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   4
      Left            =   7320
      Picture         =   "vb9114.frx":48C5
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   5
      Left            =   7560
      Picture         =   "vb9114.frx":49EF
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   6
      Left            =   7800
      Picture         =   "vb9114.frx":4B19
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   7
      Left            =   8040
      Picture         =   "vb9114.frx":4C43
      Top             =   4320
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   0
      Left            =   6360
      Picture         =   "vb9114.frx":4D6D
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   1
      Left            =   6600
      Picture         =   "vb9114.frx":4E97
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   2
      Left            =   6840
      Picture         =   "vb9114.frx":4FC1
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   3
      Left            =   7080
      Picture         =   "vb9114.frx":50EB
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   4
      Left            =   7320
      Picture         =   "vb9114.frx":5215
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   5
      Left            =   7560
      Picture         =   "vb9114.frx":533F
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   6
      Left            =   7800
      Picture         =   "vb9114.frx":5469
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   7
      Left            =   8040
      Picture         =   "vb9114.frx":5593
      Top             =   4500
      Visible         =   0   'False
      Width           =   270
   End
End
Attribute VB_Name = "util9114"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Dim range_value(9) As Integer
Dim PrevP(15) As Long
Dim ChannelOn(15) As Long
Dim SignalColor(15) As Long
Dim Active As Long
Dim NewStart As Long
Dim status(15) As Long
Dim angle As Single
Dim sq As Integer
Dim gnDoValue As Long

Const SINE_WAVE = 0
Const SQUARE_WAVE = 1

'For tuner
Const Pi = 3.14159
Const MaxAngle = 4      '5Pi/4
Const MinAngle = -0.8    '-Pi/4

Dim x0 As Long, y0 As Long
Dim DragOn As Long
Dim l As Long
Dim lb As Single, ub As Single


Sub Scale_Change(ByVal g As Long)
  Select Case g
  Case 0
    Label1(2).Caption = "10.0"
    Label1(3).Caption = "5.0"
    Label1(4).Caption = "0.0"
    Label1(5).Caption = "-5.0"
    Label1(6).Caption = "-10.0"
  Case 1
    Label1(2).Caption = "5.0"
    Label1(3).Caption = "2.5"
    Label1(4).Caption = "0.0"
    Label1(5).Caption = "-2.5"
    Label1(6).Caption = "-5.0"
  Case 2
    Label1(2).Caption = "2.50"
    Label1(3).Caption = "1.25"
    Label1(4).Caption = "0.00"
    Label1(5).Caption = "-1.25"
    Label1(6).Caption = "-2.50"
  Case 3
    Label1(2).Caption = "1.250"
    Label1(3).Caption = "0.625"
    Label1(4).Caption = "0.00"
    Label1(5).Caption = "-0.625"
    Label1(6).Caption = "-1.250"
  Case 4
    Label1(2).Caption = "1.0"
    Label1(3).Caption = "0.5"
    Label1(4).Caption = "0.0"
    Label1(5).Caption = "-0.5"
    Label1(6).Caption = "-1.0"
  Case 5
    Label1(2).Caption = "0.1"
    Label1(3).Caption = "0.05"
    Label1(4).Caption = "0.0"
    Label1(5).Caption = "-0.05"
    Label1(6).Caption = "-0.1"
  Case 6
    Label1(2).Caption = "0.01"
    Label1(3).Caption = "0.005"
    Label1(4).Caption = "0.00"
    Label1(5).Caption = "-0.005"
    Label1(6).Caption = "-0.01"
End Select
  
  Label1(19).Caption = Label1(2).Caption
  Label1(20).Caption = Label1(3).Caption
  Label1(21).Caption = Label1(4).Caption
  Label1(22).Caption = Label1(5).Caption
  Label1(23).Caption = Label1(6).Caption
End Sub

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

Private Sub DOP_Click(Index As Integer)
  Dim i As Integer, v As Integer, d As Long
  If Active Then
    If status(Index) = 1 Then
      DOP(Index) = Image0
      status(Index) = 0
        d = 1
        For i = 1 To Index
          d = d * 2
        Next
        gnDoValue = gnDoValue - d
    Else
      DOP(Index) = Image1
      status(Index) = 1
        d = 1
        For i = 1 To Index
          d = d * 2
        Next
        gnDoValue = gnDoValue + d
    End If
    v = DO_WritePort(card, 0, gnDoValue)
  End If
End Sub

Private Sub ExitBtn_Click()
    Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
    End
End Sub

Private Sub Form_Load()
  Dim i As Long
  Dim result As Integer
  
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

  range_value(0) = 1
  range_value(1) = 2
  range_value(2) = 3
  range_value(3) = 4
  range_value(4) = 10
  range_value(5) = 11
  range_value(6) = 12
  
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
    gnDoValue = 0
    angle = 0
    sq = 0
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
    DOP(i) = Image0
  Next
  i = DO_WritePort(card, 0, 0)
  Active = False
  NewStart = True
  StartBtn.Enabled = True
  StopBtn.Enabled = False
  PauseBtn.Enabled = False
  
End Sub

Private Sub Timer1_Timer()
  Dim result As Integer
  Dim i As Integer, j As Integer
  Dim d As Integer, l As Long
  Dim p As Integer
  Dim h As Long
  Dim m As Single
  
'  Do While Contd
'Analog-Digital Input
    result = BitBlt(Signal(0).hDC, 4, 0, Signal(0).Width / Screen.TwipsPerPixelX, Signal(0).Height / Screen.TwipsPerPixelY, Signal(0).hDC, 0, 0, SRCCOPY)
    result = BitBlt(Signal(1).hDC, 4, 0, Signal(1).Width / Screen.TwipsPerPixelX, Signal(1).Height / Screen.TwipsPerPixelY, Signal(1).hDC, 0, 0, SRCCOPY)
    For i = 0 To 15
      If ChannelOn(i) Then
        result = AI_ReadChannel(card, i, range_value(gain.ListIndex), d)
        l = d
        
        If i < 8 Then
          p = (32767 - CSng(l)) / 65535 * (Signal(0).Height / Screen.TwipsPerPixelY - 2)
          If PrevP(i) <> -1 Then Signal(0).Line (4, p)-(8, PrevP(i)), SignalColor(i)
        Else
          p = (32767 - CSng(l)) / 65535 * (Signal(1).Height / Screen.TwipsPerPixelY - 2)
          If PrevP(i) <> -1 Then
            Signal(1).Line (4, p)-(8, PrevP(i)), SignalColor(i)
          End If
        End If
        PrevP(i) = p
      End If
    Next i

'Digital Input
    result = DI_ReadPort(card, 0, l)
    For i = 0 To 15
      p = l Mod 2
      If p = 0 Then
        DI(i) = Image0
      Else 'p = 1
        DI(i) = Image1
      End If
      l = CInt(Int(l / 2))
    Next i
End Sub




