VERSION 4.00
Begin VB.Form SettingForm 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0C0C0&
   Caption         =   "Card Number Setting"
   ClientHeight    =   1275
   ClientLeft      =   4590
   ClientTop       =   4545
   ClientWidth     =   2820
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
   Height          =   1680
   Left            =   4530
   LinkTopic       =   "Form2"
   ScaleHeight     =   1275
   ScaleWidth      =   2820
   Top             =   4200
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
   Begin VB.ComboBox CardNum 
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
      Caption         =   "Card Number:"
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

Private Sub BaseAddr_Change()

End Sub


Private Sub CancelBtn_Click()
  Unload SettingForm
End Sub

Private Sub Form_Load()
  Dim i As Long
  For i = 0 To 15 Step 1
    CardNum.AddItem i
  Next
  CardNum.ListIndex = 0
End Sub

Private Sub OKBtn_Click()
  card = -1
  card_number = CardNum.ListIndex
  card = Register_Card(PCI_6208V, card_number)
  Unload SettingForm
End Sub

