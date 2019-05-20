object FraOutput: TFraOutput
  Left = 0
  Top = 0
  Width = 119
  Height = 124
  TabOrder = 0
  object btAction: TBitBtn
    Left = 0
    Top = 42
    Width = 119
    Height = 82
    Caption = 'LEFT'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btActionClick
    Layout = blGlyphTop
  end
  object stTitle: TStaticText
    Left = 0
    Top = 0
    Width = 119
    Height = 41
    Align = alTop
    Alignment = taCenter
    AutoSize = False
    BevelKind = bkFlat
    Caption = 'CYLINDER NAME'
    Color = clMoneyGreen
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 1
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 300
    OnTimer = tmUpdateTimer
  end
end
