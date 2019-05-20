object FrmOperation: TFrmOperation
  Left = 739
  Top = 227
  BorderStyle = bsNone
  Caption = 'FrmOperation'
  ClientHeight = 694
  ClientWidth = 1016
  Color = 16744576
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 787
    Top = 104
    Width = 177
    Height = 33
    Caption = 'SPIN MOTOR'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 787
    Top = 153
    Width = 171
    Height = 33
    Caption = 'TILT MOTOR'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 787
    Top = 202
    Width = 128
    Height = 33
    Caption = 'AIR1 SOL'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 787
    Top = 252
    Width = 128
    Height = 33
    Caption = 'AIR2 SOL'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 787
    Top = 301
    Width = 214
    Height = 33
    Caption = 'WATER SUPPLY'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 787
    Top = 449
    Width = 128
    Height = 33
    Caption = 'DOOR UP'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label7: TLabel
    Left = 787
    Top = 499
    Width = 178
    Height = 33
    Caption = 'DOOR DOWN'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label8: TLabel
    Left = 211
    Top = 24
    Width = 586
    Height = 60
    Caption = 'GLASS CLEAN SYSTEM'
    Color = 16744576
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -51
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label9: TLabel
    Left = 639
    Top = 539
    Width = 155
    Height = 36
    Caption = 'Tilting'#50948#52824
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -31
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label10: TLabel
    Left = 850
    Top = 539
    Width = 128
    Height = 36
    Caption = #54924#51204#49549#46020
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -31
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbModelName: TLabel
    Left = 830
    Top = 40
    Width = 179
    Height = 38
    Caption = 'HRW_130G'
    Color = 16744576
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clAqua
    Font.Height = -33
    Font.Name = 'Arial'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnDblClick = lbModelNameDblClick
  end
  object Label12: TLabel
    Left = 787
    Top = 350
    Width = 194
    Height = 33
    Caption = 'WATER DRAIN'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label13: TLabel
    Left = 787
    Top = 400
    Width = 170
    Height = 33
    Caption = 'VACUUM ON'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbRemote: TLabel
    Left = 6
    Top = 2
    Width = 175
    Height = 38
    Caption = 'OFFLINE_1'
    Color = 16744576
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -33
    Font.Name = 'Arial'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnDblClick = lbModelNameDblClick
  end
  object lbHoming: TLabel
    Left = 6
    Top = 42
    Width = 136
    Height = 38
    Caption = 'HOMING'
    Color = 16744576
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -33
    Font.Name = 'Arial'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnDblClick = lbModelNameDblClick
  end
  object btStart: TPanel
    Tag = 1
    Left = 19
    Top = 116
    Width = 166
    Height = 67
    BevelWidth = 3
    Caption = 'START'
    Color = clSilver
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btStartClick
    OnMouseDown = btStartMouseDown
    OnMouseUp = btStartMouseUp
  end
  object btStop: TPanel
    Tag = 2
    Left = 218
    Top = 116
    Width = 166
    Height = 67
    BevelWidth = 3
    Caption = 'STOP'
    Color = clSilver
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btStartClick
    OnMouseDown = btStartMouseDown
    OnMouseUp = btStartMouseUp
  end
  object btEmg: TPanel
    Tag = 3
    Left = 417
    Top = 116
    Width = 166
    Height = 67
    BevelWidth = 3
    Caption = 'EMERGENCY'
    Color = clSilver
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = btStartClick
    OnMouseDown = btStartMouseDown
    OnMouseUp = btStartMouseUp
  end
  object btReset: TPanel
    Tag = 4
    Left = 218
    Top = 193
    Width = 366
    Height = 66
    BevelWidth = 3
    Caption = 'RESET'
    Color = clSilver
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    OnClick = btResetClick
    OnMouseDown = btStartMouseDown
    OnMouseUp = btStartMouseUp
  end
  object Panel6: TPanel
    Left = 16
    Top = 274
    Width = 597
    Height = 378
    BevelOuter = bvNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object Panel7: TPanel
      Left = 3
      Top = 3
      Width = 126
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 0
      object pnDeviceNo: TPanel
        Left = 4
        Top = 4
        Width = 118
        Height = 83
        BevelOuter = bvNone
        Caption = '1'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clYellow
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel8: TPanel
      Left = 3
      Top = 97
      Width = 126
      Height = 90
      BevelOuter = bvNone
      Caption = 'CLEAN'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
    object Panel9: TPanel
      Left = 3
      Top = 191
      Width = 126
      Height = 90
      BevelOuter = bvNone
      Caption = 'AIR'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
    end
    object Panel10: TPanel
      Left = 3
      Top = 285
      Width = 126
      Height = 90
      BevelOuter = bvNone
      Caption = 'DRY'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
    end
    object Panel11: TPanel
      Left = 132
      Top = 3
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Caption = 'STEP'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
    end
    object Panel12: TPanel
      Left = 132
      Top = 97
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 5
      object pnCleanSignal: TPanel
        Left = 6
        Top = 8
        Width = 140
        Height = 73
        BevelWidth = 3
        Caption = 'OFF'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -23
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel13: TPanel
      Left = 132
      Top = 191
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 6
      object pnAirSignal: TPanel
        Left = 6
        Top = 8
        Width = 140
        Height = 73
        BevelWidth = 3
        Caption = 'OFF'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -23
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel14: TPanel
      Left = 132
      Top = 285
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 7
      object pnDrySignal: TPanel
        Left = 6
        Top = 8
        Width = 140
        Height = 73
        BevelWidth = 3
        Caption = 'OFF'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -23
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel15: TPanel
      Left = 287
      Top = 3
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Caption = 'RPM'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
    end
    object Panel16: TPanel
      Left = 287
      Top = 97
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 9
      object pnCleanRPM: TPanel
        Left = 8
        Top = 8
        Width = 136
        Height = 73
        BevelOuter = bvNone
        Caption = '1234'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clYellow
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel17: TPanel
      Left = 287
      Top = 191
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 10
      object pnAirRPM: TPanel
        Left = 8
        Top = 8
        Width = 136
        Height = 73
        BevelOuter = bvNone
        Caption = '1234'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clYellow
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel18: TPanel
      Left = 287
      Top = 285
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 11
      object pnDryRPM: TPanel
        Left = 8
        Top = 8
        Width = 136
        Height = 73
        BevelOuter = bvNone
        Caption = '1234'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clYellow
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel19: TPanel
      Left = 442
      Top = 3
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Caption = 'P-TIME'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 12
    end
    object Panel20: TPanel
      Left = 442
      Top = 97
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 13
      object pnCleanTime: TPanel
        Left = 8
        Top = 8
        Width = 136
        Height = 73
        BevelOuter = bvNone
        Caption = '1234'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clFuchsia
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel21: TPanel
      Left = 442
      Top = 191
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 14
      object pnAirTime: TPanel
        Left = 8
        Top = 8
        Width = 136
        Height = 73
        BevelOuter = bvNone
        Caption = '1234'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clFuchsia
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel22: TPanel
      Left = 442
      Top = 285
      Width = 152
      Height = 90
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 15
      object pnDryTime: TPanel
        Left = 8
        Top = 8
        Width = 136
        Height = 73
        BevelOuter = bvNone
        Caption = '1234'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clFuchsia
        Font.Height = -35
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
  end
  object Panel39: TPanel
    Left = 624
    Top = 96
    Width = 157
    Height = 441
    BevelOuter = bvNone
    Caption = 'Panel23'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    object pnVaccumOn: TPanel
      Left = 6
      Top = 297
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 0
    end
    object pnWtrDrain: TPanel
      Left = 6
      Top = 248
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 1
    end
    object pnWtrSupply: TPanel
      Left = 6
      Top = 200
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 2
    end
    object pnAir2Sol: TPanel
      Left = 6
      Top = 151
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 3
    end
    object pnAir1Sol: TPanel
      Left = 6
      Top = 103
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 4
    end
    object pnTiltMotor: TPanel
      Left = 6
      Top = 54
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 5
    end
    object pnSpinMotor: TPanel
      Left = 6
      Top = 6
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 6
    end
    object pnDoorUp: TPanel
      Left = 6
      Top = 345
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 7
    end
    object pnDoorDn: TPanel
      Left = 6
      Top = 394
      Width = 145
      Height = 41
      BevelOuter = bvNone
      Color = 16744576
      TabOrder = 8
    end
  end
  object Panel2: TPanel
    Left = 635
    Top = 588
    Width = 169
    Height = 64
    BevelOuter = bvNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -35
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 6
    object Panel3: TPanel
      Left = 91
      Top = 2
      Width = 76
      Height = 59
      BevelOuter = bvNone
      Caption = 'mm'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object pnTiltCmdPos: TPanel
      Left = 0
      Top = 0
      Width = 91
      Height = 63
      BevelOuter = bvNone
      Caption = '1234'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -35
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
  end
  object Panel4: TPanel
    Left = 833
    Top = 588
    Width = 169
    Height = 64
    BevelOuter = bvNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -35
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    object Panel5: TPanel
      Left = 91
      Top = 2
      Width = 76
      Height = 59
      BevelOuter = bvNone
      Caption = 'rpm'
      Color = 16744576
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -31
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object pnSpinSpeed: TPanel
      Left = 0
      Top = 0
      Width = 91
      Height = 63
      BevelOuter = bvNone
      Caption = '1234'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -35
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
  end
  object tmUpdate: TTimer
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 992
    Top = 8
  end
  object tmAxisStatus: TTimer
    Interval = 30
    OnTimer = tmAxisStatusTimer
    Left = 960
    Top = 8
  end
end
