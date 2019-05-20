VERSION 4.00
Begin VB.Form util9112 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "9112 Sample Program"
   ClientHeight    =   6390
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
   Height          =   6795
   Left            =   1035
   LinkTopic       =   "Form1"
   ScaleHeight     =   426
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
      Left            =   6150
      TabIndex        =   56
      Top             =   5895
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
      Left            =   120
      TabIndex        =   2
      Top             =   5040
      Width           =   2055
      Begin VB.ComboBox Gain 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   3
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
         Picture         =   "vb9112.frx":0000
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5160
         Picture         =   "vb9112.frx":0172
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4440
         Picture         =   "vb9112.frx":02E4
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4800
         Picture         =   "vb9112.frx":0456
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4080
         Picture         =   "vb9112.frx":05C8
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3720
         Picture         =   "vb9112.frx":073A
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3360
         Picture         =   "vb9112.frx":08AC
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3000
         Picture         =   "vb9112.frx":0A1E
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2640
         Picture         =   "vb9112.frx":0B90
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2280
         Picture         =   "vb9112.frx":0D02
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1920
         Picture         =   "vb9112.frx":0E74
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1560
         Picture         =   "vb9112.frx":0FE6
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1200
         Picture         =   "vb9112.frx":1158
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   840
         Picture         =   "vb9112.frx":12CA
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   480
         Picture         =   "vb9112.frx":143C
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DOP 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   15
         Left            =   120
         Picture         =   "vb9112.frx":15AE
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
         Picture         =   "vb9112.frx":1720
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   14
         Left            =   480
         Picture         =   "vb9112.frx":1892
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   13
         Left            =   840
         Picture         =   "vb9112.frx":1A04
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   12
         Left            =   1200
         Picture         =   "vb9112.frx":1B76
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   11
         Left            =   1560
         Picture         =   "vb9112.frx":1CE8
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   10
         Left            =   1920
         Picture         =   "vb9112.frx":1E5A
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   9
         Left            =   2280
         Picture         =   "vb9112.frx":1FCC
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   8
         Left            =   2640
         Picture         =   "vb9112.frx":213E
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   7
         Left            =   3000
         Picture         =   "vb9112.frx":22B0
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   6
         Left            =   3360
         Picture         =   "vb9112.frx":2422
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   5
         Left            =   3720
         Picture         =   "vb9112.frx":2594
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   4
         Left            =   4080
         Picture         =   "vb9112.frx":2706
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   3
         Left            =   4440
         Picture         =   "vb9112.frx":2878
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   2
         Left            =   4800
         Picture         =   "vb9112.frx":29EA
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   5160
         Picture         =   "vb9112.frx":2B5C
         Top             =   280
         Width           =   300
      End
      Begin VB.Image DI 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   5520
         Picture         =   "vb9112.frx":2CCE
         Top             =   280
         Width           =   300
      End
   End
   Begin VB.Frame DA_Frame 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Analog Output (D/A)"
      ForeColor       =   &H00FF0000&
      Height          =   2535
      Left            =   6120
      TabIndex        =   7
      Top             =   3270
      Width           =   2415
      Begin VB.ComboBox DA_Wave 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   1080
         Style           =   2  'Dropdown List
         TabIndex        =   8
         Top             =   360
         Width           =   1215
      End
      Begin VB.Line pin 
         BorderColor     =   &H0000FFFF&
         BorderWidth     =   2
         X1              =   1140
         X2              =   1020
         Y1              =   1680
         Y2              =   1800
      End
      Begin VB.Label dal 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Channel 0:"
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
         Left            =   120
         TabIndex        =   9
         Top             =   420
         Width           =   975
      End
      Begin VB.Label dal 
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "Channel 1:"
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
         Left            =   120
         TabIndex        =   10
         Top             =   780
         Width           =   975
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "5"
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
         Index           =   5
         Left            =   1500
         TabIndex        =   11
         Top             =   1980
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "4"
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
         Index           =   4
         Left            =   1620
         TabIndex        =   12
         Top             =   1500
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "3"
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
         Index           =   3
         Left            =   1320
         TabIndex        =   14
         Top             =   1080
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "2"
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
         Index           =   2
         Left            =   840
         TabIndex        =   15
         Top             =   1080
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "1"
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
         Left            =   540
         TabIndex        =   16
         Top             =   1500
         Width           =   195
      End
      Begin VB.Label mark 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H00C0C0C0&
         Caption         =   "0"
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
         Left            =   660
         TabIndex        =   17
         Top             =   1980
         Width           =   195
      End
      Begin VB.Label AO_value 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H0000FFFF&
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0.00"
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
         Left            =   900
         TabIndex        =   18
         Top             =   2160
         Width           =   555
      End
      Begin VB.Image Tuner 
         Appearance      =   0  'Flat
         Height          =   750
         Left            =   720
         Picture         =   "vb9112.frx":2E40
         Top             =   1260
         Width           =   900
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
      TabIndex        =   19
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
         TabIndex        =   20
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
         TabIndex        =   21
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
         TabIndex        =   22
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
         TabIndex        =   23
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
         TabIndex        =   24
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
         TabIndex        =   25
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
         TabIndex        =   26
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
         TabIndex        =   27
         Top             =   240
         Width           =   135
      End
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   8
         Left            =   390
         Picture         =   "vb9112.frx":3502
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
         Picture         =   "vb9112.frx":362C
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
         Picture         =   "vb9112.frx":3756
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
         Picture         =   "vb9112.frx":3880
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
         Picture         =   "vb9112.frx":39AA
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
         Picture         =   "vb9112.frx":3AD4
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
         Picture         =   "vb9112.frx":3BFE
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
         Picture         =   "vb9112.frx":3D28
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
      TabIndex        =   28
      Top             =   360
      Width           =   1575
      Begin VB.Image Channel 
         Appearance      =   0  'Flat
         BorderStyle     =   1  'Fixed Single
         Height          =   210
         Index           =   7
         Left            =   1080
         Picture         =   "vb9112.frx":3E52
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
         Picture         =   "vb9112.frx":3F7C
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
         Picture         =   "vb9112.frx":40A6
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
         Picture         =   "vb9112.frx":41D0
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
         Picture         =   "vb9112.frx":42FA
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
         Picture         =   "vb9112.frx":4424
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
         Picture         =   "vb9112.frx":454E
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
         Picture         =   "vb9112.frx":4678
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
         TabIndex        =   29
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
         TabIndex        =   30
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
         TabIndex        =   31
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
         TabIndex        =   32
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
         TabIndex        =   33
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
         TabIndex        =   34
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
         TabIndex        =   35
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
         TabIndex        =   36
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
      TabIndex        =   42
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
         TabIndex        =   37
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
         TabIndex        =   51
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
         TabIndex        =   38
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
         TabIndex        =   39
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
         TabIndex        =   40
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
         TabIndex        =   41
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
         TabIndex        =   43
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
         TabIndex        =   44
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
         TabIndex        =   45
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
         TabIndex        =   46
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
         TabIndex        =   47
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
         TabIndex        =   48
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
         TabIndex        =   49
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
         TabIndex        =   50
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
         TabIndex        =   52
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
         TabIndex        =   53
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
         TabIndex        =   54
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
         TabIndex        =   55
         Top             =   2910
         Width           =   480
      End
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   70
      Left            =   1920
      Top             =   5760
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
      Left            =   3960
      TabIndex        =   4
      Top             =   5895
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
      Left            =   3060
      TabIndex        =   0
      Top             =   5895
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
      Left            =   4860
      TabIndex        =   1
      Top             =   5895
      Width           =   795
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "15   14  13   12  11   10    9    8    7    6    5     4    3    2    1     0"
      ForeColor       =   &H80000008&
      Height          =   195
      Left            =   240
      TabIndex        =   13
      Top             =   4080
      Width           =   5775
   End
   Begin VB.Image Image0 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   2400
      Picture         =   "vb9112.frx":47A2
      Top             =   5820
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   2700
      Picture         =   "vb9112.frx":4914
      Top             =   5820
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   0
      Left            =   -180
      Picture         =   "vb9112.frx":4A86
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   1
      Left            =   60
      Picture         =   "vb9112.frx":4BB0
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   2
      Left            =   300
      Picture         =   "vb9112.frx":4CDA
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   3
      Left            =   540
      Picture         =   "vb9112.frx":4E04
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   4
      Left            =   780
      Picture         =   "vb9112.frx":4F2E
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   5
      Left            =   1020
      Picture         =   "vb9112.frx":5058
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   6
      Left            =   1260
      Picture         =   "vb9112.frx":5182
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChDn 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   7
      Left            =   1500
      Picture         =   "vb9112.frx":52AC
      Top             =   5760
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   0
      Left            =   -180
      Picture         =   "vb9112.frx":53D6
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   1
      Left            =   60
      Picture         =   "vb9112.frx":5500
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   2
      Left            =   300
      Picture         =   "vb9112.frx":562A
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   3
      Left            =   540
      Picture         =   "vb9112.frx":5754
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   4
      Left            =   780
      Picture         =   "vb9112.frx":587E
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   5
      Left            =   1020
      Picture         =   "vb9112.frx":59A8
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   6
      Left            =   1260
      Picture         =   "vb9112.frx":5AD2
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
   Begin VB.Image ChUp 
      Appearance      =   0  'Flat
      Height          =   210
      Index           =   7
      Left            =   1500
      Picture         =   "vb9112.frx":5BFC
      Top             =   5940
      Visible         =   0   'False
      Width           =   270
   End
End
Attribute VB_Name = "util9112"
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

Sub Scale_Change(ByVal g As Long)
  Select Case g
  Case 0
    Label1(2).Caption = "5.0"
    Label1(3).Caption = "2.5"
    Label1(4).Caption = "0.0"
    Label1(5).Caption = "-2.5"
    Label1(6).Caption = "-5.0"
  Case 1
    Label1(2).Caption = "2.50"
    Label1(3).Caption = "1.25"
    Label1(4).Caption = "0.00"
    Label1(5).Caption = "-1.25"
    Label1(6).Caption = "-2.50"
  Case 2
    Label1(2).Caption = "1.250"
    Label1(3).Caption = "0.625"
    Label1(4).Caption = "0.00"
    Label1(5).Caption = "-0.625"
    Label1(6).Caption = "-1.250"
  Case 3
    Label1(2).Caption = "0.6250"
    Label1(3).Caption = "0.3125"
    Label1(4).Caption = "0.0000"
    Label1(5).Caption = "-0.3125"
    Label1(6).Caption = "-0.6250"
  Case 4
    Label1(2).Caption = "10"
    Label1(3).Caption = "7.5"
    Label1(4).Caption = "5.0"
    Label1(5).Caption = "2.5"
    Label1(6).Caption = "0.0"
  Case 5
    Label1(2).Caption = "5.00"
    Label1(3).Caption = "3.75"
    Label1(4).Caption = "2.50"
    Label1(5).Caption = "1.25"
    Label1(6).Caption = "0.00"
  Case 6
    Label1(2).Caption = "2.500"
    Label1(3).Caption = "1.875"
    Label1(4).Caption = "1.250"
    Label1(5).Caption = "0.625"
    Label1(6).Caption = "0.000"
  Case 7
    Label1(2).Caption = "1.2500"
    Label1(3).Caption = "0.9375"
    Label1(4).Caption = "0.6250"
    Label1(5).Caption = "0.3125"
    Label1(6).Caption = "0.0000"
  Case 8
    Label1(2).Caption = "10"
    Label1(3).Caption = "5.0"
    Label1(4).Caption = "0.0"
    Label1(5).Caption = "-5.0"
    Label1(6).Caption = "-10"
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
  'gnDoValue = 0

  gain.AddItem "+/- 5V"
  gain.AddItem "+/- 2.5V"
  gain.AddItem "+/- 1.25V"
  gain.AddItem "+/- 0.625V"
  gain.AddItem "0V~10V"
  gain.AddItem "0V~5V"
  gain.AddItem "0V~2.5V"
  gain.AddItem "0V~1.25V"
  gain.AddItem "+/- 10V"

  range_value(0) = 2
  range_value(1) = 3
  range_value(2) = 4
  range_value(3) = 5
  range_value(4) = 15
  range_value(5) = 16
  range_value(6) = 17
  range_value(7) = 18
  range_value(8) = 1
  
  gain.ListIndex = 0

  DA_Wave.AddItem "Sine Wave"
  DA_Wave.AddItem "Square Wave"
  
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

  'For tuner
  x0 = pin.X1
  y0 = pin.Y1
  l = Sqr(((pin.X2 - x0) ^ 2) + ((pin.Y2 - y0) ^ 2))
  lb = 0
  ub = 5
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
        If l < 0 Then
            l = l + 65536
        End If
        l = (l And &HFFF0) / 16
        
        If i < 8 Then
          p = (4095 - CSng(l)) / 4095 * (Signal(0).Height / Screen.TwipsPerPixelY - 2)
          If PrevP(i) <> -1 Then Signal(0).Line (4, p)-(8, PrevP(i)), SignalColor(i)
        Else
          p = (4095 - CSng(l)) / 4095 * (Signal(1).Height / Screen.TwipsPerPixelY - 2)
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
  
'Analog Output
    Select Case DA_Wave.ListIndex
      Case SINE_WAVE:
        m = Sin(angle)
        angle = angle + Pi / 36
        p = CInt(m * 2048 + 2048)
        If p >= 4096 Then p = 4095
        result = AO_WriteChannel(card, 0, p)
      Case SQUARE_WAVE:
        result = AO_WriteChannel(card, 0, sq)
        If (angle Mod 36) = 0 Then
          If sq = 0 Then sq = 4095 Else sq = 0
        End If
        angle = angle + 1
      End Select
End Sub

Private Sub Tuner_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
  Dim dx As Long, dy As Long
  dx = X + Tuner.Left - pin.X2
  dy = Y + Tuner.Top - pin.Y2
  If dx > -50 And dx < 50 And dy > -50 And dy < 50 Then
    DragOn = True
  End If

End Sub

Private Sub Tuner_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
  Dim r As Single, p As Single
  Dim d As Long
  Dim dx As Long, dy As Long
  Dim q As Integer, result As Integer
  If DragOn Then
    dx = X + Tuner.Left - x0
    dy = Y + Tuner.Top - y0
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
        result = AO_WriteChannel(card, 1, q)
    End If
  End If
End Sub

Private Sub Tuner_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
  DragOn = False
End Sub

