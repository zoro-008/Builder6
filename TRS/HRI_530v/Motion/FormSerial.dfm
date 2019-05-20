object FrmSerial: TFrmSerial
  Left = 2433
  Top = 288
  BorderStyle = bsDialog
  Caption = 'FrmSerial'
  ClientHeight = 401
  ClientWidth = 340
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 313
    Height = 13
    AutoSize = False
    Caption = #51109#48708#49884#47532#50620' '#46321#47197#51060'  '#54596#50836' '#54633#45768#45796'.'
  end
  object Label2: TLabel
    Left = 16
    Top = 64
    Width = 257
    Height = 13
    AutoSize = False
    Caption = #48376' '#54532#47196#44536#47016#51032' '#44396#46041#44032#45733' '#49884#47532#50620' '#47532#49828#53944
  end
  object Label3: TLabel
    Left = 16
    Top = 32
    Width = 321
    Height = 13
    AutoSize = False
    Caption = #51109#48708' '#51032'  '#47749#54032#51012' '#54869#51064' '#54616#50668' '#47532#49828#53944#50640#49436' '#49440#53469#54644' '#51452#49464#50836
  end
  object lbSerial: TListBox
    Left = 16
    Top = 80
    Width = 305
    Height = 209
    ImeName = 'Microsoft Office IME 2007'
    ItemHeight = 13
    TabOrder = 0
  end
  object edSerial: TEdit
    Left = 16
    Top = 296
    Width = 305
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 1
  end
  object btSetSerial: TButton
    Left = 16
    Top = 336
    Width = 193
    Height = 49
    Caption = 'SET SERIAL'
    TabOrder = 2
    OnClick = btSetSerialClick
  end
  object btClose: TButton
    Left = 224
    Top = 336
    Width = 97
    Height = 49
    Caption = 'CLOSE'
    TabOrder = 3
    OnClick = btCloseClick
  end
end
