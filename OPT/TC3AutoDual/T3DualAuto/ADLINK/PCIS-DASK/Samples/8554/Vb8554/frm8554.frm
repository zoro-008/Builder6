VERSION 4.00
Begin VB.Form frm8554 
   Caption         =   "PCI-8554"
   ClientHeight    =   4455
   ClientLeft      =   1575
   ClientTop       =   1635
   ClientWidth     =   5910
   Height          =   4860
   Left            =   1515
   LinkTopic       =   "Form1"
   ScaleHeight     =   4455
   ScaleWidth      =   5910
   Top             =   1290
   Width           =   6030
   Begin VB.CommandButton btnExit 
      Caption         =   "Exit"
      Height          =   375
      Left            =   4500
      TabIndex        =   5
      Top             =   3900
      Width           =   855
   End
   Begin VB.CommandButton btnStop 
      Caption         =   "Stop"
      Enabled         =   0   'False
      Height          =   375
      Left            =   1680
      TabIndex        =   4
      Top             =   3900
      Width           =   855
   End
   Begin VB.CommandButton btnStart 
      Caption         =   "Start"
      Height          =   375
      Left            =   540
      TabIndex        =   3
      Top             =   3900
      Width           =   855
   End
   Begin VB.Frame fraSet 
      ForeColor       =   &H0000FFFF&
      Height          =   3555
      Left            =   180
      TabIndex        =   0
      Top             =   120
      Width           =   5535
      Begin VB.ComboBox cobFun 
         Height          =   315
         Left            =   1200
         Style           =   2  'Dropdown List
         TabIndex        =   2
         Top             =   300
         Width           =   1935
      End
      Begin VB.Label txtMsg 
         BorderStyle     =   1  'Fixed Single
         ForeColor       =   &H000000FF&
         Height          =   2415
         Left            =   240
         TabIndex        =   7
         Top             =   960
         Width           =   5055
      End
      Begin VB.Label labMsg 
         Caption         =   "Message :"
         ForeColor       =   &H00FF0000&
         Height          =   300
         Left            =   240
         TabIndex        =   6
         Top             =   720
         Width           =   975
      End
      Begin VB.Label Label2 
         Caption         =   "Function:"
         ForeColor       =   &H00FF0000&
         Height          =   195
         Left            =   240
         TabIndex        =   1
         Top             =   360
         Width           =   975
      End
   End
End
Attribute VB_Name = "frm8554"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim card_seq As Integer


Private Sub ShowFunction()
    Select Case cobFun.ListIndex
    Case 0
        txtMsg = "Generate 250 KHz square wave. The clock source of the counter #1 is set to C8M"
    Case 1
        txtMsg = "Generate one pulse every hour." + Chr$(13) + Chr$(10) + Chr$(13) + Chr$(10) + "<1> the clock source of the counter #1 is set to C8M" + Chr(13) + Chr(10) + "<2> the clock source of counter #2, #3 is from #1, #2 respectively"
    Case 2
        txtMsg = "Measure pulse width." + Chr$(13) + Chr$(10) + Chr$(13) + Chr$(10) + "<1> the clock source of the counter #1 is set to C8M" + Chr$(13) + Chr$(10) + "<2> the signal to be measured is connected to GATE1"
    Case 3
        txtMsg = "Measure signal frequency." + Chr$(13) + Chr$(10) + Chr$(13) + Chr$(10) + "<1> Connect the signal to be measured to the clock source of the counter #1 (ECLK1)" + Chr(13) + Chr(10) + "<2> the clock source of the counter #2,#3 is set to C8M, COUT2 respectively" + Chr$(13) + Chr$(10) + "<3> the gate source of the counter #1 set to the inverse of COUT3" + Chr(13) + Chr(10) + "<4> the gate source of the counter #2, #3 set to High"
    End Select
End Sub


Private Sub btnExit_Click()
  End
End Sub


Private Sub btnStart_Click()
    Dim result As Integer, err As Integer
    Dim Mode As Integer
    Dim v As Integer
    Dim old_value As Long, new_value As Long
    Dim Value As Long
    Dim period As Single, frequency As Single
    
    btnStart.Enabled = False
    err = 0
    Select Case cobFun.ListIndex
        Case 0  'Square Wave
            result = CTR_8554_CK1_Config(card, CK1_C8M)
            result = CTR_8554_ClkSrc_Config(card, 1, CK1)
            result = CTR_8554_ClkSrc_Config(card, 2, CK1)
            result = CTR_8554_ClkSrc_Config(card, 3, CK1)
            result = CTR_8554_ClkSrc_Config(card, 4, CK1)
            result = CTR_8554_ClkSrc_Config(card, 5, CK1)
            result = CTR_8554_ClkSrc_Config(card, 6, CK1)
            result = CTR_8554_ClkSrc_Config(card, 7, CK1)
            result = CTR_8554_ClkSrc_Config(card, 8, CK1)
            result = CTR_8554_ClkSrc_Config(card, 9, CK1)
            result = CTR_8554_ClkSrc_Config(card, 10, CK1)
            result = CTR_Setup(card, 1, 3, 32, BIN)
            result = CTR_Setup(card, 2, 3, 32, BIN)
            result = CTR_Setup(card, 3, 3, 32, BIN)
            result = CTR_Setup(card, 4, 3, 32, BIN)
            result = CTR_Setup(card, 5, 3, 32, BIN)
            result = CTR_Setup(card, 6, 3, 32, BIN)
            result = CTR_Setup(card, 7, 3, 32, BIN)
            result = CTR_Setup(card, 8, 3, 32, BIN)
            result = CTR_Setup(card, 9, 3, 32, BIN)
            result = CTR_Setup(card, 10, 3, 32, BIN)
            txtMsg = "*** 250 KHz square wave is ready on the COUT1 ~ 11 ***"
        Case 1  'Pluse Generator
            result = CTR_8554_CK1_Config(card, CK1_C8M)
            result = CTR_8554_ClkSrc_Config(card, 1, CK1)
            result = CTR_8554_ClkSrc_Config(card, 2, COUTN_1)
            result = CTR_8554_ClkSrc_Config(card, 3, COUTN_1)
            result = CTR_Setup(card, 1, 3, 4000, 0)
            result = CTR_Setup(card, 2, 3, 2000, 0)
            result = CTR_Setup(card, 3, 3, 3600, 0)
            txtMsg = " *** A pulse per hour is ready on the COUT3 ***"
        Case 2  'Measure Pulse Width
            result = CTR_8554_CK1_Config(card, CK1_C8M)
            result = CTR_8554_ClkSrc_Config(card, 1, CK1)
            result = CTR_Setup(card, 1, 2, 0, BIN)
            under_flow = 0
            Call wait_pulse_start
            time1 = Timer
            result = CTR_Read(card, 1, old_value)
            Do
                result = CTR_Read(card, 1, new_value)
                time2 = Timer
                period = time2 - time1
                If (new_value = old_value) Or (period > 5) Then Exit Do
                If old_value < new_value Then under_flow = under_flow + 1
                old_value = new_value
            Loop While True
            If period < 5 Then
              pulse_width = (65536 - new_value + under_flow * 65536 + 1) * 0.125
              txtMsg = "*** High level pulse width = " & pulse_width & " micro-second ***"
            Else
              txtMsg = "*** Measure inappropriate pulse width ***"
            End If
        Case 3  'Measure Frequency
            result = CTR_8554_ClkSrc_Config(card, 1, ECKN)
            result = CTR_8554_ClkSrc_Config(card, 2, CK1)
            result = CTR_8554_ClkSrc_Config(card, 3, COUTN_1)
            result = CTR_Setup(card, 1, 2, 65535, 0)
            result = CTR_Setup(card, 2, 2, 4, 0)
            result = CTR_Setup(card, 3, 0, 65535, 0)
            Call Sleep(1000)
            result = CTR_Read(card, 1, Value)
            period = 65535 - Value
            frequency = period * 2000000 / 65535
            txtMsg = "*** Measured Signal Frequency = " & frequency & "Hz ***"
    End Select
    btnStop.Enabled = True
End Sub

Private Sub btnStop_Click()
    btnStart.Enabled = True
    btnStop.Enabled = False
    ShowFunction
End Sub


Private Sub cobFun_Click()
    ShowFunction
End Sub


Private Sub Form_Load()
  Dim result As Integer
  
  SettingForm.Show 1
  If card < 0 Then
    MsgBox "PCI-8554 Register_Card Error"
    End
  End If
  cobFun.AddItem "Square Wave"
  cobFun.AddItem "Pluse Generator"
  cobFun.AddItem "Measure Pulse Width"
  cobFun.AddItem "Measure Frequency"
  
  cobFun.ListIndex = 0
  
End Sub



Public Sub wait_pulse_start()
  Dim result As Integer
  Dim old_value As Long, new_value As Long
  Dim Mode As Integer

  Do
    result = CTR_Read(card, 1, old_value)
    result = CTR_Read(card, 1, new_value)
  Loop While new_value <> old_value
  Do
    result = CTR_Read(card, 1, old_value)
    result = CTR_Read(card, 1, new_value)
  Loop While new_value = old_value
End Sub

Private Sub Form_Unload(Cancel As Integer)
  Dim result As Integer
    If card >= 0 Then
        result = Release_Card(card)
    End If
  End
End Sub


