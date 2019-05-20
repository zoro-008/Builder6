object FrmManual: TFrmManual
  Left = 470
  Top = 168
  BorderStyle = bsNone
  ClientHeight = 805
  ClientWidth = 1334
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnManual: TPanel
    Left = 0
    Top = 0
    Width = 1334
    Height = 805
    Align = alClient
    Caption = '-'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 1332
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'MANUAL'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object btSTGVccm: TBitBtn
      Tag = 40
      Left = 4
      Top = 367
      Width = 251
      Height = 46
      Caption = 'Stage Vacuum'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btSTGVccmClick
    end
    object btSTGEjct: TBitBtn
      Tag = 40
      Left = 509
      Top = 366
      Width = 251
      Height = 46
      Caption = 'Stage Eject'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = btSTGVccmClick
    end
    object btPCKEjct: TBitBtn
      Tag = 40
      Left = 509
      Top = 414
      Width = 251
      Height = 46
      Caption = 'Picker Eject'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = btSTGVccmClick
    end
    object btPCKVccmOn: TBitBtn
      Tag = 40
      Left = 4
      Top = 415
      Width = 251
      Height = 46
      Caption = 'Picker Vacuum On'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      OnClick = btSTGVccmClick
    end
    object btMan3_1: TBitBtn
      Tag = 40
      Left = 4
      Top = 40
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_2: TBitBtn
      Tag = 41
      Left = 4
      Top = 86
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_7: TBitBtn
      Tag = 46
      Left = 4
      Top = 316
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_3: TBitBtn
      Tag = 42
      Left = 4
      Top = 132
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_4: TBitBtn
      Tag = 43
      Left = 4
      Top = 178
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 9
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_5: TBitBtn
      Tag = 44
      Left = 4
      Top = 224
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_6: TBitBtn
      Tag = 45
      Left = 4
      Top = 270
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 11
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_8: TBitBtn
      Tag = 50
      Left = 257
      Top = 40
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 12
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_9: TBitBtn
      Tag = 51
      Left = 257
      Top = 86
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 13
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_10: TBitBtn
      Tag = 52
      Left = 257
      Top = 132
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 14
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_11: TBitBtn
      Tag = 53
      Left = 257
      Top = 178
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 15
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_12: TBitBtn
      Tag = 54
      Left = 257
      Top = 224
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 16
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_13: TBitBtn
      Tag = 55
      Left = 257
      Top = 270
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 17
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan3_14: TBitBtn
      Tag = 56
      Left = 257
      Top = 316
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 18
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_1: TBitBtn
      Tag = 60
      Left = 509
      Top = 40
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 19
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_2: TBitBtn
      Tag = 61
      Left = 509
      Top = 86
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 20
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_7: TBitBtn
      Tag = 66
      Left = 509
      Top = 316
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 21
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_3: TBitBtn
      Tag = 62
      Left = 509
      Top = 132
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 22
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_4: TBitBtn
      Tag = 63
      Left = 509
      Top = 178
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 23
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_5: TBitBtn
      Tag = 64
      Left = 509
      Top = 224
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 24
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_6: TBitBtn
      Tag = 65
      Left = 509
      Top = 270
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 25
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_8: TBitBtn
      Tag = 70
      Left = 762
      Top = 40
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 26
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_9: TBitBtn
      Tag = 71
      Left = 762
      Top = 86
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 27
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_10: TBitBtn
      Tag = 61
      Left = 762
      Top = 132
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 28
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_11: TBitBtn
      Tag = 62
      Left = 762
      Top = 178
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 29
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_12: TBitBtn
      Tag = 63
      Left = 762
      Top = 224
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 30
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_13: TBitBtn
      Tag = 64
      Left = 762
      Top = 270
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 31
      Visible = False
      OnClick = btMan3_1Click
    end
    object btMan4_14: TBitBtn
      Tag = 76
      Left = 762
      Top = 316
      Width = 251
      Height = 46
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 32
      Visible = False
      OnClick = btMan3_1Click
    end
    object btPCKVccmOff: TBitBtn
      Tag = 40
      Left = 256
      Top = 414
      Width = 251
      Height = 46
      Caption = 'Picker Vacuum Off'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 33
      OnClick = btSTGVccmClick
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 16
    Top = 16
  end
end
