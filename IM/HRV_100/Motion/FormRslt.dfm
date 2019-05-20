object FrmRslt: TFrmRslt
  Left = 863
  Top = 69
  Align = alClient
  BorderStyle = bsNone
  ClientHeight = 1002
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pnVisn: TPanel
    Left = 0
    Top = 51
    Width = 320
    Height = 265
    Align = alTop
    BevelOuter = bvNone
    Caption = 'pnVisn'
    Color = clBackground
    TabOrder = 0
  end
  object pnGraph: TPanel
    Left = 0
    Top = 545
    Width = 320
    Height = 240
    Align = alTop
    Caption = 'pnGraph'
    TabOrder = 1
  end
  object pnRslt: TPanel
    Left = 0
    Top = 0
    Width = 320
    Height = 51
    Align = alTop
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    object lbPreRslt: TLabel
      Left = 8
      Top = 28
      Width = 59
      Height = 20
      Caption = 'PreInsp:'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lbRslt: TLabel
      Left = 126
      Top = 4
      Width = 71
      Height = 23
      Alignment = taCenter
      AutoSize = False
      Caption = 'OK'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
  end
  object sgDriveRslt: TStringGrid
    Left = 0
    Top = 785
    Width = 320
    Height = 229
    Align = alTop
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
    ScrollBars = ssNone
    TabOrder = 3
    OnClick = sgTiltRsltClick
    OnDrawCell = sgTiltRsltDrawCell
  end
  object sgTiltRslt: TStringGrid
    Left = 0
    Top = 316
    Width = 320
    Height = 229
    Align = alTop
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
    ScrollBars = ssNone
    TabOrder = 4
    OnClick = sgTiltRsltClick
    OnDrawCell = sgTiltRsltDrawCell
  end
  object tmUpdate: TTimer
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 288
    Top = 8
  end
end
