VERSION 4.00
Begin VB.Form SettingForm 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   ClientHeight    =   1275
   ClientLeft      =   1110
   ClientTop       =   1485
   ClientWidth     =   2820
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   0
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   1680
   Left            =   1050
   LinkTopic       =   "Form2"
   ScaleHeight     =   1275
   ScaleWidth      =   2820
   Top             =   1140
   Width           =   2940
   Begin VB.CommandButton OKBtn 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   300
      TabIndex        =   0
      Top             =   720
      Width           =   915
   End
   Begin VB.CommandButton CancelBtn 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   1380
      TabIndex        =   3
      Top             =   720
      Width           =   915
   End
   Begin VB.ComboBox BaseAddr 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   1440
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   120
      Width           =   1035
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H00C0C0C0&
      Caption         =   "Base Address:"
      ForeColor       =   &H00000000&
      Height          =   195
      Index           =   0
      Left            =   180
      TabIndex        =   1
      Top             =   180
      Width           =   1275
   End
End
Attribute VB_Name = "SettingForm"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit
Const ADD = 1
Const MODIFY = 2
'Dim DropDown As Long
Dim card As Long
Dim cmd As Long

Private Sub CancelBtn_Click()
  Unload SettingForm
End Sub

Private Sub Form_Load()
  Dim i As Long
  For i = &H200 To &H3F8 Step 16
    BaseAddr.AddItem Hex$(i) + "H"
  Next
  BaseAddr.ListIndex = 2    '220H
End Sub

Private Sub OKBtn_Click()
  Dim result As Long
  result = W_8112_Initial(CardIndex, Val("&H" + Trim$(BaseAddr)))
  Select Case result
  Case ERR_NoError
    If CardIndex = 0 Then
      GainValue(0) = 0
      util8112.Caption = "8112 - " + "0:" + BaseAddr
      util8112.mnuCard0.Enabled = True
      util8112.mnuCard0.Checked = True
      util8112.mnuCard1.Checked = False
      util8112.gain.Enabled = True
    Else  'CardIndex = 1
      GainValue(1) = 0
      util8112.Caption = "8112 - " + "1:" + BaseAddr
      util8112.mnuCard1.Enabled = True
      util8112.mnuCard0.Checked = False
      util8112.mnuCard1.Checked = True
      util8112.gain.Enabled = True
    End If
    result = W_8112_AD_Set_Range(0)
    util8112.gain.ListIndex = 0
    Scale_Change 0
    DO_lb = 0
    DO_hb = 0
  Case ERR_InvalidBoardNumber
    MsgBox "Invalid Board Number"
  Case ERR_DuplicateBoardSetting
    MsgBox "Base address conflicts with the other card."
  End Select
  Unload SettingForm
End Sub

