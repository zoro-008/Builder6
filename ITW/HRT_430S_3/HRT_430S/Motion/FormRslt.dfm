object FrmRslt: TFrmRslt
  Left = 862
  Top = 317
  Width = 200
  Height = 150
  Caption = 'Result'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 60
    Width = 52
    Height = 13
    Caption = 'Heat Fail'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label16: TLabel
    Left = 8
    Top = 40
    Width = 43
    Height = 13
    Caption = 'Lot No '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label17: TLabel
    Left = 8
    Top = 82
    Width = 31
    Height = 13
    Caption = 'Good'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label18: TLabel
    Left = 9
    Top = 19
    Width = 28
    Height = 13
    Caption = 'Time'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object edLotNo: TEdit
    Left = 65
    Top = 35
    Width = 121
    Height = 21
    Color = cl3DLight
    ImeName = 'Microsoft Office IME 2007'
    ReadOnly = True
    TabOrder = 0
    Text = '0'
  end
  object edFail: TEdit
    Left = 65
    Top = 55
    Width = 121
    Height = 21
    Color = cl3DLight
    ImeName = 'Microsoft Office IME 2007'
    ReadOnly = True
    TabOrder = 1
    Text = '0'
  end
  object edWork: TEdit
    Left = 65
    Top = 77
    Width = 121
    Height = 21
    Color = cl3DLight
    ImeName = 'Microsoft Office IME 2007'
    ReadOnly = True
    TabOrder = 2
    Text = '0'
  end
  object edTime: TEdit
    Left = 65
    Top = 14
    Width = 121
    Height = 21
    Color = cl3DLight
    ImeName = 'Microsoft Office IME 2007'
    ReadOnly = True
    TabOrder = 3
    Text = '0'
  end
end
