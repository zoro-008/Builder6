object FraMotr: TFraMotr
  Left = 0
  Top = 0
  Width = 285
  Height = 135
  TabOrder = 0
  object Label81: TLabel
    Left = 3
    Top = 86
    Width = 73
    Height = 19
    Alignment = taCenter
    Caption = 'ENC POS'
    Color = clBtnFace
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label82: TLabel
    Left = 2
    Top = 34
    Width = 75
    Height = 19
    Alignment = taCenter
    Caption = 'CMD POS'
    Color = clBtnFace
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object lbCmdPos: TLabel
    Left = 3
    Top = 56
    Width = 279
    Height = 23
    Alignment = taCenter
    AutoSize = False
    Caption = '0.0'
    Color = clActiveBorder
    Font.Charset = ARABIC_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object lbEncPos: TLabel
    Left = 3
    Top = 108
    Width = 279
    Height = 23
    Alignment = taCenter
    AutoSize = False
    Caption = '0.0'
    Color = clActiveBorder
    Font.Charset = ARABIC_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat2: TLabel
    Left = 44
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = 'HM'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat3: TLabel
    Left = 84
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = '+L'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat1: TLabel
    Left = 4
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = '-L'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat4: TLabel
    Left = 124
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = 'AR'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat5: TLabel
    Left = 164
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = 'SV'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat6: TLabel
    Left = 204
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = 'ST'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LbStat7: TLabel
    Left = 244
    Top = 3
    Width = 37
    Height = 25
    Alignment = taCenter
    AutoSize = False
    Caption = 'HD'
    Color = clSilver
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 300
    OnTimer = tmUpdateTimer
    Left = 215
    Top = 35
  end
end
