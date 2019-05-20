object FrmRepeat: TFrmRepeat
  Left = 1072
  Top = 347
  Width = 356
  Height = 185
  Caption = 'FrmRepeat'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 12
    Width = 63
    Height = 13
    Caption = 'RepeatCount'
  end
  object Label2: TLabel
    Left = 16
    Top = 36
    Width = 81
    Height = 13
    Caption = 'RepeatCountLeft'
  end
  object lbCntLeft: TLabel
    Left = 112
    Top = 36
    Width = 63
    Height = 13
    Caption = 'RepeatCount'
  end
  object Edit1: TEdit
    Left = 112
    Top = 8
    Width = 73
    Height = 21
    TabOrder = 0
    Text = '5'
  end
  object btCrntRepeat: TButton
    Left = 192
    Top = 5
    Width = 121
    Height = 25
    Caption = 'Current Repeat'
    TabOrder = 1
    OnClick = btCrntRepeatClick
  end
  object btFullRepeat: TButton
    Left = 192
    Top = 87
    Width = 121
    Height = 25
    Caption = 'Full Repeat'
    TabOrder = 2
    OnClick = btFullRepeatClick
  end
  object btStop: TButton
    Left = 192
    Top = 114
    Width = 121
    Height = 25
    Caption = 'STOP'
    TabOrder = 3
    OnClick = btStopClick
  end
  object btPinRepeat: TButton
    Left = 192
    Top = 32
    Width = 121
    Height = 25
    Caption = 'Pin Repeat'
    TabOrder = 4
    OnClick = btPinRepeatClick
  end
  object btChcukPinRepeat: TButton
    Left = 192
    Top = 59
    Width = 121
    Height = 25
    Caption = 'Pin Chuck Repeat'
    TabOrder = 5
    OnClick = btChcukPinRepeatClick
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 80
    Top = 56
  end
end
