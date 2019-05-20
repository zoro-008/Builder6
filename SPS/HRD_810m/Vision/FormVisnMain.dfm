object FrmVisnMain: TFrmVisnMain
  Left = 144
  Top = 253
  BorderStyle = bsNone
  Caption = 'FrmVisnMain'
  ClientHeight = 749
  ClientWidth = 1016
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  DesignSize = (
    1016
    749)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 864
    Top = 8
    Width = 3
    Height = 13
  end
  object pcVisnBase: TPageControl
    Left = 0
    Top = 0
    Width = 1016
    Height = 749
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnChange = pcVisnBaseChange
  end
  object btJobFile: TButton
    Left = 898
    Top = 1
    Width = 55
    Height = 20
    Anchors = [akTop, akRight]
    Caption = 'JobFile'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btJobFileClick
  end
  object btSetup: TButton
    Left = 956
    Top = 1
    Width = 55
    Height = 20
    Anchors = [akTop, akRight]
    Caption = 'Setup'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    Visible = False
    OnClick = btSetupClick
  end
  object tmYame: TTimer
    Interval = 100
    OnTimer = tmYameTimer
    Left = 8
    Top = 8
  end
end
