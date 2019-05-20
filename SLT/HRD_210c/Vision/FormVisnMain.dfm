object FrmVisnMain: TFrmVisnMain
  Left = 855
  Top = 151
  BorderStyle = bsNone
  Caption = 'FrmVisnMain'
  ClientHeight = 304
  ClientWidth = 484
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    484
    304)
  PixelsPerInch = 96
  TextHeight = 13
  object pcVisnBase: TPageControl
    Left = 0
    Top = 0
    Width = 484
    Height = 304
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
    Left = 366
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
    Left = 424
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
end
