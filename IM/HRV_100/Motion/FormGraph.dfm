object FrmGraph: TFrmGraph
  Left = 1456
  Top = 253
  Align = alClient
  BorderStyle = bsNone
  Caption = 'FrmGraph'
  ClientHeight = 250
  ClientWidth = 319
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnBase: TPanel
    Left = 0
    Top = 0
    Width = 319
    Height = 250
    Align = alClient
    Color = clSkyBlue
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = #44404#47548#52404
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    DesignSize = (
      319
      250)
    object Button1: TButton
      Left = 303
      Top = 1
      Width = 15
      Height = 16
      Anchors = [akTop, akRight]
      Caption = #12609
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object tmUpdate: TTimer
    Interval = 10
    OnTimer = tmUpdateTimer
  end
end
