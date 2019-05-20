object FrmMode: TFrmMode
  Left = 428
  Top = 148
  BorderStyle = bsNone
  Caption = 'FrmMode'
  ClientHeight = 59
  ClientWidth = 414
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  OnDestroy = FormDestroy
  OnHide = tmUpdateTimer
  PixelsPerInch = 96
  TextHeight = 13
  object pnMode: TPanel
    Left = 0
    Top = 0
    Width = 414
    Height = 59
    Align = alClient
    BevelOuter = bvLowered
    Caption = 'RETEST MODE'
    Color = clDefault
    Font.Charset = ARABIC_CHARSET
    Font.Color = clYellow
    Font.Height = -40
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 500
    OnTimer = tmUpdateTimer
    Left = 301
  end
end
