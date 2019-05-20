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
    util9118.Label1(2).Caption = "5.0"
    util9118.Label1(3).Caption = "2.5"
    util9118.Label1(4).Caption = "0.0"
    util9118.Label1(5).Caption = "-2.5"
    util9118.Label1(6).Caption = "-5.0"
  Case 1
    util9118.Label1(2).Caption = "2.50"
    util9118.Label1(3).Caption = "2.25"
    util9118.Label1(4).Caption = "0.00"
    util9118.Label1(5).Caption = "-2.25"
    util9118.Label1(6).Caption = "-2.50"
  Case 2
    util9118.Label1(2).Caption = "1.250"
    util9118.Label1(3).Caption = "0.625"
    util9118.Label1(4).Caption = "0.00"
    util9118.Label1(5).Caption = "-0.625"
    util9118.Label1(6).Caption = "-1.250"
  Case 3
    util9118.Label1(2).Caption = "0.6250"
    util9118.Label1(3).Caption = "0.3125"
    util9118.Label1(4).Caption = "0.0000"
    util9118.Label1(5).Caption = "-0.3125"
    util9118.Label1(6).Caption = "-0.6250"
  Case 4
    util9118.Label1(2).Caption = "10.00"
    util9118.Label1(3).Caption = "7.500"
    util9118.Label1(4).Caption = "5.000"
    util9118.Label1(5).Caption = "2.500"
    util9118.Label1(6).Caption = "0.000"
  Case 5
    util9118.Label1(2).Caption = "5.000"
    util9118.Label1(3).Caption = "3.750"
    util9118.Label1(4).Caption = "2.500"
    util9118.Label1(5).Caption = "1.250"
    util9118.Label1(6).Caption = "0.000"
  Case 6
    util9118.Label1(2).Caption = "2.500"
    util9118.Label1(3).Caption = "1.875"
    util9118.Label1(4).Caption = "1.250"
    util9118.Label1(5).Caption = "0.625"
    util9118.Label1(6).Caption = "0.000"
  Case 7
    util9118.Label1(2).Caption = "1.2500"
    util9118.Label1(3).Caption = "0.9375"
    util9118.Label1(4).Caption = "0.6250"
    util9118.Label1(5).Caption = "0.3125"
    util9118.Label1(6).Caption = "0.0000"
  End Select
  
  util9118.Label1(19).Caption = util9118.Label1(2).Caption
  util9118.Label1(20).Caption = util9118.Label1(3).Caption
  util9118.Label1(21).Caption = util9118.Label1(4).Caption
  util9118.Label1(22).Caption = util9118.Label1(5).Caption
  util9118.Label1(23).Caption = util9118.Label1(6).Caption
  
End Sub

