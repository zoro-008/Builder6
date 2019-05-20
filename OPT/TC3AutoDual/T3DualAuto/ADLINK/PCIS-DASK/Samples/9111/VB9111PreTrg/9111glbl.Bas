Attribute VB_Name = "Module1"
Option Explicit

Declare Function BitBlt Lib "GDI32" (ByVal hDestDC As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal XSrc As Long, ByVal YSrc As Long, ByVal dwRop As Long) As Long

Global Const SRCCOPY = &HCC0020 ' (DWORD) dest = source
Global card As Integer
Global card_number As Integer
Global card_type As Integer

Sub Scale_Change(ByVal g As Long)
  Select Case g
  Case 0
    util9111.Label1(2).Caption = "10.0"
    util9111.Label1(3).Caption = "5.0"
    util9111.Label1(4).Caption = "0.0"
    util9111.Label1(5).Caption = "-5.0"
    util9111.Label1(6).Caption = "-10.0"
  Case 1
    util9111.Label1(2).Caption = "5.0"
    util9111.Label1(3).Caption = "2.5"
    util9111.Label1(4).Caption = "0.0"
    util9111.Label1(5).Caption = "-2.5"
    util9111.Label1(6).Caption = "-5.0"
  Case 2
    util9111.Label1(2).Caption = "2.50"
    util9111.Label1(3).Caption = "2.25"
    util9111.Label1(4).Caption = "0.00"
    util9111.Label1(5).Caption = "-2.25"
    util9111.Label1(6).Caption = "-2.50"
  Case 3
    util9111.Label1(2).Caption = "1.250"
    util9111.Label1(3).Caption = "0.625"
    util9111.Label1(4).Caption = "0.00"
    util9111.Label1(5).Caption = "-0.625"
    util9111.Label1(6).Caption = "-1.250"
  Case 3
    util9111.Label1(2).Caption = "0.6250"
    util9111.Label1(3).Caption = "0.3125"
    util9111.Label1(4).Caption = "0.0000"
    util9111.Label1(5).Caption = "-0.3125"
    util9111.Label1(6).Caption = "-0.6250"
  End Select
  
  util9111.Label1(19).Caption = util9111.Label1(2).Caption
  util9111.Label1(20).Caption = util9111.Label1(3).Caption
  util9111.Label1(21).Caption = util9111.Label1(4).Caption
  util9111.Label1(22).Caption = util9111.Label1(5).Caption
  util9111.Label1(23).Caption = util9111.Label1(6).Caption
End Sub

