object FrmMaster: TFrmMaster
  Left = 0
  Top = 11
  BorderStyle = bsNone
  Caption = 'FrmMaster'
  ClientHeight = 700
  ClientWidth = 1016
  Color = 16744576
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1016
    Height = 700
    Align = alClient
    Color = 16744576
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 40
      Top = 317
      Width = 326
      Height = 369
      Caption = 'Logic / Velocity Setup'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Panel2: TPanel
        Left = 24
        Top = 32
        Width = 81
        Height = 24
        Caption = 'Emg'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel3: TPanel
        Left = 24
        Top = 56
        Width = 81
        Height = 24
        Caption = 'Enc'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Panel4: TPanel
        Left = 24
        Top = 80
        Width = 81
        Height = 24
        Caption = 'Enc(Z)'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object Panel5: TPanel
        Left = 24
        Top = 104
        Width = 81
        Height = 24
        Caption = 'Enc Input'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object Panel6: TPanel
        Left = 24
        Top = 128
        Width = 81
        Height = 24
        Caption = 'Near'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object Panel7: TPanel
        Left = 24
        Top = 152
        Width = 81
        Height = 24
        Caption = 'Minus Limit'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object Panel8: TPanel
        Left = 24
        Top = 176
        Width = 81
        Height = 24
        Caption = 'Plus Limit'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object Panel9: TPanel
        Left = 24
        Top = 200
        Width = 81
        Height = 24
        Caption = 'Alarm'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object Panel10: TPanel
        Left = 24
        Top = 224
        Width = 81
        Height = 24
        Caption = 'Inpo'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object Panel11: TPanel
        Left = 24
        Top = 248
        Width = 81
        Height = 24
        Caption = 'Pulse'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object Panel12: TPanel
        Left = 24
        Top = 272
        Width = 81
        Height = 24
        Caption = 'Busy Off'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object Panel13: TPanel
        Left = 24
        Top = 296
        Width = 81
        Height = 24
        Caption = 'Srv Ready'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
      end
      object Panel14: TPanel
        Left = 24
        Top = 320
        Width = 81
        Height = 24
        Caption = 'LMT Stop'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object cbEmg: TComboBox
        Left = 105
        Top = 32
        Width = 195
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 1
        ParentFont = False
        TabOrder = 13
        Text = '1'
        OnChange = cbEmgChange
        Items.Strings = (
          '0'
          '1')
      end
      object cbEncCount1: TComboBox
        Tag = 1
        Left = 105
        Top = 56
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 1
        ParentFont = False
        TabOrder = 14
        Text = '2'#52404#48176
        OnClick = cbEncCount1Click
        Items.Strings = (
          '4'#52404#48176
          '2'#52404#48176
          '1'#52404#48176)
      end
      object cbEncZ1: TComboBox
        Tag = 1
        Left = 105
        Top = 80
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 15
        Text = '0'
        OnClick = cbEncZ1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbEncDir1: TComboBox
        Tag = 1
        Left = 105
        Top = 104
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 16
        Text = 'A|B'
        OnClick = cbEncDir1Click
        Items.Strings = (
          'A|B'
          'B|A'
          'Up|Down'
          'Down|Up')
      end
      object cbNear1: TComboBox
        Tag = 1
        Left = 105
        Top = 128
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 17
        Text = '0'
        OnClick = cbNear1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbMLimit1: TComboBox
        Tag = 1
        Left = 105
        Top = 152
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 18
        Text = '0'
        OnClick = cbMLimit1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbPLimit1: TComboBox
        Tag = 1
        Left = 105
        Top = 176
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 19
        Text = '0'
        OnClick = cbPLimit1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbAlarm1: TComboBox
        Tag = 1
        Left = 105
        Top = 200
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 20
        Text = '0'
        OnClick = cbAlarm1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbInpo1: TComboBox
        Tag = 1
        Left = 105
        Top = 224
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 21
        Text = '0'
        OnClick = cbInpo1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbPulse1: TComboBox
        Tag = 1
        Left = 105
        Top = 248
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 22
        Text = '2P, LOW, CW/CCW'
        OnClick = cbPulse1Click
        Items.Strings = (
          '2P, LOW, CW/CCW'
          '2P, LOW, CCW/CW'
          '2P, HIGH, CW/CCW'
          '2P, HIGH, CCW/CW'
          '1P, LOW, CW/CCW'
          '1P, LOW, CCW/CW'
          '1P, HIGH, CW/CCW'
          '1P, HIGH, CCW/CW')
      end
      object cbBusy1: TComboBox
        Tag = 1
        Left = 105
        Top = 272
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 23
        Text = 'Pulse'
        OnClick = cbBusy1Click
        Items.Strings = (
          'Pulse'
          'Inpos')
      end
      object cbSvReady1: TComboBox
        Tag = 1
        Left = 105
        Top = 296
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 24
        Text = '0'
        OnClick = cbSvReady1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbStop1: TComboBox
        Tag = 1
        Left = 105
        Top = 320
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 25
        Text = 'Sudden'
        OnClick = cbStop1Click
        Items.Strings = (
          'Sudden'
          'Decelerate')
      end
      object cbEncCount2: TComboBox
        Tag = 2
        Left = 203
        Top = 55
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 26
        Text = '4'#52404#48176
        OnClick = cbEncCount1Click
        Items.Strings = (
          '4'#52404#48176
          '2'#52404#48176
          '1'#52404#48176)
      end
      object cbEncZ2: TComboBox
        Tag = 2
        Left = 203
        Top = 79
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 27
        Text = '0'
        OnClick = cbEncZ1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbEncDir2: TComboBox
        Tag = 2
        Left = 203
        Top = 103
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 28
        Text = 'A|B'
        OnClick = cbEncDir1Click
        Items.Strings = (
          'A|B'
          'B|A'
          'Up|Down'
          'Down|Up')
      end
      object cbNear2: TComboBox
        Tag = 2
        Left = 203
        Top = 127
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 29
        Text = '0'
        OnClick = cbNear1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbMLimit2: TComboBox
        Tag = 2
        Left = 203
        Top = 151
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 30
        Text = '0'
        OnClick = cbMLimit1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbPLimit2: TComboBox
        Tag = 2
        Left = 203
        Top = 175
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 31
        Text = '0'
        OnClick = cbPLimit1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbAlarm2: TComboBox
        Tag = 2
        Left = 203
        Top = 199
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 32
        Text = '0'
        OnClick = cbAlarm1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbInpo2: TComboBox
        Tag = 2
        Left = 203
        Top = 223
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 33
        Text = '0'
        OnClick = cbInpo1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbPulse2: TComboBox
        Tag = 2
        Left = 203
        Top = 247
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 34
        Text = '2P, LOW, CW/CCW'
        OnClick = cbPulse1Click
        Items.Strings = (
          '2P, LOW, CW/CCW'
          '2P, LOW, CCW/CW'
          '2P, HIGH, CW/CCW'
          '2P, HIGH, CCW/CW'
          '1P, LOW, CW/CCW'
          '1P, LOW, CCW/CW'
          '1P, HIGH, CW/CCW'
          '1P, HIGH, CCW/CW')
      end
      object cbBusy2: TComboBox
        Tag = 2
        Left = 203
        Top = 271
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 35
        Text = 'Pulse'
        OnClick = cbBusy1Click
        Items.Strings = (
          'Pulse'
          'Inpos')
      end
      object cbSvReady2: TComboBox
        Tag = 2
        Left = 203
        Top = 295
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 36
        Text = '0'
        OnClick = cbSvReady1Click
        Items.Strings = (
          '0'
          '1')
      end
      object cbStop2: TComboBox
        Tag = 2
        Left = 203
        Top = 319
        Width = 97
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ItemHeight = 16
        ItemIndex = 0
        ParentFont = False
        TabOrder = 37
        Text = 'Sudden'
        OnClick = cbStop1Click
        Items.Strings = (
          'Sudden'
          'Decelerate')
      end
    end
    object GroupBox2: TGroupBox
      Left = 384
      Top = 382
      Width = 270
      Height = 300
      Caption = 'Logic / Velocity Setup'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Panel15: TPanel
        Left = 25
        Top = 32
        Width = 81
        Height = 24
        Caption = 'Emergence'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel16: TPanel
        Left = 25
        Top = 56
        Width = 81
        Height = 24
        Caption = 'Busy'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Panel17: TPanel
        Left = 25
        Top = 80
        Width = 81
        Height = 24
        Caption = 'Enc(Z)'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object Panel18: TPanel
        Left = 25
        Top = 104
        Width = 81
        Height = 24
        Caption = 'Near(Org)'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object Panel19: TPanel
        Left = 25
        Top = 128
        Width = 81
        Height = 24
        Caption = '-Limit'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object Panel20: TPanel
        Left = 25
        Top = 152
        Width = 81
        Height = 24
        Caption = '+Limit'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object Panel21: TPanel
        Left = 25
        Top = 176
        Width = 81
        Height = 24
        Caption = 'Alarm'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object Panel22: TPanel
        Left = 25
        Top = 200
        Width = 81
        Height = 24
        Caption = 'Inpo'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object Panel23: TPanel
        Left = 25
        Top = 224
        Width = 81
        Height = 24
        Caption = 'HomeDone'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object Panel24: TPanel
        Left = 25
        Top = 248
        Width = 81
        Height = 24
        Caption = 'S Ready'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object pnEmg1: TPanel
        Left = 106
        Top = 32
        Width = 120
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object pnBusy1: TPanel
        Tag = 1
        Left = 106
        Top = 56
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
      end
      object pnNear1: TPanel
        Tag = 1
        Left = 106
        Top = 104
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object pnEncZ1: TPanel
        Tag = 1
        Left = 106
        Top = 80
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
      object pnHPLimit1: TPanel
        Tag = 1
        Left = 106
        Top = 152
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
      end
      object pnHMLimit1: TPanel
        Tag = 1
        Left = 106
        Top = 128
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object pnInpo1: TPanel
        Tag = 1
        Left = 106
        Top = 200
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
      end
      object pnAlarm1: TPanel
        Tag = 1
        Left = 106
        Top = 176
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
      end
      object pnSReady1: TPanel
        Tag = 1
        Left = 106
        Top = 248
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
      end
      object pnHomeDone1: TPanel
        Tag = 1
        Left = 106
        Top = 224
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
      end
      object pnBusy2: TPanel
        Tag = 2
        Left = 166
        Top = 56
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
      end
      object pnNear2: TPanel
        Tag = 2
        Left = 166
        Top = 104
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
      end
      object pnEncZ2: TPanel
        Tag = 2
        Left = 166
        Top = 80
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
      end
      object pnHPLimit2: TPanel
        Tag = 2
        Left = 166
        Top = 152
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 23
      end
      object pnHMLimit2: TPanel
        Tag = 2
        Left = 166
        Top = 128
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 24
      end
      object pnInpo2: TPanel
        Tag = 2
        Left = 166
        Top = 200
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 25
      end
      object pnAlarm2: TPanel
        Tag = 2
        Left = 166
        Top = 176
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 26
      end
      object pnSReady2: TPanel
        Tag = 2
        Left = 166
        Top = 248
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 27
      end
      object pnHomeDone2: TPanel
        Tag = 2
        Left = 166
        Top = 224
        Width = 60
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 28
      end
      object pnSMLimit1: TPanel
        Tag = 1
        Left = 136
        Top = 128
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 29
      end
      object pnSPLimit1: TPanel
        Tag = 1
        Left = 136
        Top = 152
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 30
      end
      object pnSMLimit2: TPanel
        Tag = 2
        Left = 196
        Top = 128
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 31
      end
      object pnSPLimit2: TPanel
        Tag = 2
        Left = 196
        Top = 152
        Width = 30
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 32
      end
    end
    object GroupBox3: TGroupBox
      Left = 40
      Top = 9
      Width = 325
      Height = 299
      Caption = 'Motion'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object Panel25: TPanel
        Left = 28
        Top = 47
        Width = 85
        Height = 24
        Caption = 'Out'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel26: TPanel
        Left = 28
        Top = 71
        Width = 85
        Height = 24
        Caption = 'Inc'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Panel27: TPanel
        Left = 28
        Top = 95
        Width = 85
        Height = 24
        Caption = 'Abs'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object Panel28: TPanel
        Left = 28
        Top = 119
        Width = 85
        Height = 24
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object cbStop: TCheckBox
          Left = 11
          Top = 3
          Width = 97
          Height = 17
          Caption = 'Stop'
          TabOrder = 0
        end
      end
      object Panel29: TPanel
        Left = 28
        Top = 143
        Width = 85
        Height = 24
        Caption = 'Jog'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object Panel30: TPanel
        Left = 28
        Top = 261
        Width = 85
        Height = 27
        Caption = 'DCC'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object Panel31: TPanel
        Left = 28
        Top = 233
        Width = 85
        Height = 27
        Caption = 'AL Reset'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object Panel32: TPanel
        Left = 28
        Top = 205
        Width = 85
        Height = 27
        Caption = 'Servo'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object Panel33: TPanel
        Left = 28
        Top = 177
        Width = 85
        Height = 27
        Caption = 'Current'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object edOut1: TEdit
        Left = 113
        Top = 47
        Width = 90
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 9
        Text = '0'
      end
      object btIncMinus1: TButton
        Tag = 1
        Left = 114
        Top = 71
        Width = 45
        Height = 24
        Caption = '-'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        OnClick = btIncMinus1Click
      end
      object btAbsMinus1: TButton
        Tag = 1
        Left = 114
        Top = 95
        Width = 45
        Height = 24
        Caption = '-'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        OnClick = btAbsMinus1Click
      end
      object btStop1: TButton
        Tag = 1
        Left = 114
        Top = 119
        Width = 90
        Height = 24
        Caption = 'Stop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
        OnClick = btStop1Click
      end
      object btJogCCw1: TButton
        Tag = 1
        Left = 114
        Top = 143
        Width = 45
        Height = 24
        Caption = '<'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
        OnMouseDown = btJogCCw1MouseDown
        OnMouseUp = btJogCCw1MouseUp
      end
      object btIncPlus1: TButton
        Tag = 1
        Left = 159
        Top = 71
        Width = 45
        Height = 24
        Caption = '+'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
        OnClick = btIncPlus1Click
      end
      object btAbsPlus1: TButton
        Tag = 1
        Left = 159
        Top = 95
        Width = 45
        Height = 24
        Caption = '+'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
        OnClick = btAbsPlus1Click
      end
      object btJogCw1: TButton
        Tag = 1
        Left = 159
        Top = 143
        Width = 45
        Height = 24
        Caption = '>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
        OnMouseDown = btJogCw1MouseDown
        OnMouseUp = btJogCCw1MouseUp
      end
      object btIncMinus2: TButton
        Tag = 2
        Left = 205
        Top = 71
        Width = 45
        Height = 24
        Caption = '-'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
        OnClick = btIncMinus1Click
      end
      object btAbsMinus2: TButton
        Tag = 2
        Left = 205
        Top = 95
        Width = 45
        Height = 24
        Caption = '-'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
        OnClick = btAbsMinus1Click
      end
      object btStop2: TButton
        Tag = 2
        Left = 205
        Top = 119
        Width = 90
        Height = 24
        Caption = 'Stop'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
        OnClick = btStop1Click
      end
      object btJogCCw2: TButton
        Tag = 2
        Left = 205
        Top = 143
        Width = 45
        Height = 24
        Caption = '<'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
        OnMouseDown = btJogCCw1MouseDown
        OnMouseUp = btJogCCw1MouseUp
      end
      object btIncPlus2: TButton
        Tag = 2
        Left = 250
        Top = 71
        Width = 45
        Height = 24
        Caption = '+'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
        OnClick = btIncPlus1Click
      end
      object btAbsPlus2: TButton
        Tag = 2
        Left = 250
        Top = 95
        Width = 45
        Height = 24
        Caption = '+'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
        OnClick = btAbsPlus1Click
      end
      object btJogCw2: TButton
        Tag = 2
        Left = 250
        Top = 143
        Width = 45
        Height = 24
        Caption = '>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 23
        OnMouseDown = btJogCw1MouseDown
        OnMouseUp = btJogCCw1MouseUp
      end
      object Panel34: TPanel
        Left = 113
        Top = 23
        Width = 90
        Height = 25
        Caption = '1'
        Color = clMedGray
        TabOrder = 24
      end
      object edOut2: TEdit
        Left = 204
        Top = 47
        Width = 90
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 25
        Text = '0'
      end
      object Panel35: TPanel
        Left = 204
        Top = 23
        Width = 90
        Height = 25
        Caption = '2'
        Color = clMedGray
        TabOrder = 26
      end
      object btCurrent1: TBitBtn
        Tag = 1
        Left = 114
        Top = 178
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 27
        OnClick = btCurrent1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btCurrent2: TBitBtn
        Tag = 2
        Left = 204
        Top = 178
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 28
        OnClick = btCurrent1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btServo1: TBitBtn
        Tag = 1
        Left = 114
        Top = 206
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 29
        OnClick = btServo1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btServo2: TBitBtn
        Tag = 2
        Left = 204
        Top = 206
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 30
        OnClick = btServo1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btAlReset1: TBitBtn
        Tag = 1
        Left = 114
        Top = 234
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 31
        OnClick = btAlReset1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btAlReset2: TBitBtn
        Tag = 2
        Left = 204
        Top = 234
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 32
        OnClick = btAlReset1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btDCC1: TBitBtn
        Tag = 1
        Left = 114
        Top = 262
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 33
        OnClick = btDCC1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
      object btDCC2: TBitBtn
        Tag = 2
        Left = 204
        Top = 262
        Width = 90
        Height = 28
        Caption = 'Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 34
        OnClick = btDCC1Click
        Glyph.Data = {
          7A080000424D7A08000000000000360000002800000017000000170000000100
          2000000000004408000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00848484004C4C4C003C3C3C005C5C5C0084848400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C005454
          540084848400FF00FF00C4C4C400A4A4A4007C7C7C005C5C5C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C006C6C6C00FF00FF00D4D4
          D400E4E4E400E4E4E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF005454540094949400DCDCDC00DCDCDC00E4E4
          E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4A4005C5C5C00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF009494940074747400FF00FF00CCCCCC00D4D4D400D4D4D400E4E4
          E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00646464008C8C8C00B4B4B400FF00FF00CCCCCC00CCCCCC00D4D4D400CCCC
          CC00CCCCCC00B4B4B400FF00FF007C7C7C0064646400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF003434
          34008484840094949400A4A4A400B4B4B400BCBCBC00C4C4C400C4C4C400B4B4
          B400FF00FF00949494008C8C8C003C3C3C00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00444444007474
          74008484840094949400A4A4A400FF00FF00FF00FF00FF00FF00A4A4A4008C8C
          8C0084848400848484004C4C4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF007C7C7C00646464007474
          74007C7C7C009494940094949400949494008C8C8C0094949400848484007474
          74005C5C5C007C7C7C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00545454006C6C6C007474
          74007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C6C004C4C
          4C00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF008C8C8C005C5C5C00A4A4A400B4B4
          B400BCBCBC00BCBCBC00B4B4B400B4B4B4009C9C9C005C5C5C0094949400FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF009C9C9C008C8C8C00CCCCCC00ECEC
          EC00ECECEC00E4E4E400D4D4D4008C8C8C0094949400FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00B4B4B400C4C4C400CCCC
          CC00C4C4C400B4B4B400FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
      end
    end
    object GroupBox4: TGroupBox
      Left = 378
      Top = 10
      Width = 343
      Height = 127
      Caption = 'Pulse / Encoder'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object Panel36: TPanel
        Left = 41
        Top = 51
        Width = 70
        Height = 25
        Caption = 'Cmd Value'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel37: TPanel
        Left = 41
        Top = 77
        Width = 70
        Height = 23
        Caption = 'Enc Value'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object edCmdValue1: TEdit
        Tag = 1
        Left = 112
        Top = 51
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 2
        Text = '0'
        OnDblClick = edCmdValue1DblClick
      end
      object edEncValue1: TEdit
        Tag = 1
        Left = 112
        Top = 76
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 3
        Text = '0'
        OnDblClick = edEncValue1DblClick
      end
      object edCmdValue2: TEdit
        Tag = 2
        Left = 200
        Top = 51
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 4
        Text = '0'
        OnDblClick = edCmdValue1DblClick
      end
      object edEncValue2: TEdit
        Tag = 2
        Left = 200
        Top = 76
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 5
        Text = '0'
        OnDblClick = edEncValue1DblClick
      end
      object Panel46: TPanel
        Left = 111
        Top = 25
        Width = 88
        Height = 25
        Caption = '1'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object Panel47: TPanel
        Left = 200
        Top = 25
        Width = 87
        Height = 25
        Caption = '2'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object edTrgValue2: TEdit
        Tag = 2
        Left = 200
        Top = 99
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 8
        Text = '0'
        OnDblClick = edEncValue1DblClick
      end
    end
    object GroupBox5: TGroupBox
      Left = 380
      Top = 138
      Width = 341
      Height = 237
      Caption = 'Speed Setup'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      object Panel38: TPanel
        Left = 16
        Top = 72
        Width = 59
        Height = 23
        Caption = 'Start'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel39: TPanel
        Left = 16
        Top = 96
        Width = 59
        Height = 23
        Caption = 'Acc'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Panel40: TPanel
        Left = 76
        Top = 24
        Width = 87
        Height = 23
        Caption = '1'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object Panel41: TPanel
        Left = 164
        Top = 24
        Width = 87
        Height = 23
        Caption = '2'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object edStart1: TEdit
        Tag = 1
        Left = 76
        Top = 72
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 4
        Text = '0'
      end
      object edAcc1: TEdit
        Tag = 1
        Left = 76
        Top = 96
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 5
        Text = '0'
      end
      object edStart2: TEdit
        Tag = 2
        Left = 163
        Top = 72
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 6
        Text = '0'
      end
      object edAcc2: TEdit
        Tag = 2
        Left = 163
        Top = 96
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 7
        Text = '0'
      end
      object Panel42: TPanel
        Left = 16
        Top = 120
        Width = 59
        Height = 23
        Caption = 'Dec'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object Panel43: TPanel
        Left = 16
        Top = 144
        Width = 59
        Height = 23
        Caption = 'Max'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object edDec1: TEdit
        Tag = 1
        Left = 76
        Top = 120
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 10
        Text = '0'
      end
      object edMax1: TEdit
        Tag = 1
        Left = 76
        Top = 144
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 11
        Text = '0'
      end
      object edDec2: TEdit
        Tag = 2
        Left = 163
        Top = 120
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 12
        Text = '0'
      end
      object edMax2: TEdit
        Tag = 2
        Left = 163
        Top = 144
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 13
        Text = '0'
      end
      object btSpdSet1: TButton
        Tag = 1
        Left = 77
        Top = 168
        Width = 87
        Height = 24
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
        OnClick = btSpdSet1Click
      end
      object btScvSet1: TButton
        Tag = 1
        Left = 77
        Top = 192
        Width = 87
        Height = 25
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
        OnClick = btScvSet1Click
      end
      object Panel48: TPanel
        Left = 16
        Top = 48
        Width = 59
        Height = 23
        Caption = 'Ratio'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
      end
      object edRatio1: TEdit
        Tag = 1
        Left = 76
        Top = 48
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 17
        Text = '0'
      end
      object edRatio2: TEdit
        Tag = 2
        Left = 163
        Top = 48
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 18
        Text = '0'
      end
      object btSetRatio1: TButton
        Tag = 1
        Left = 252
        Top = 49
        Width = 72
        Height = 25
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
        OnClick = btSetRatio1Click
      end
      object Panel49: TPanel
        Left = 252
        Top = 24
        Width = 71
        Height = 23
        Caption = 'Ratio'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
      end
      object btSpdSet2: TButton
        Tag = 2
        Left = 164
        Top = 168
        Width = 87
        Height = 24
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
        OnClick = btSpdSet1Click
      end
      object Panel44: TPanel
        Left = 16
        Top = 168
        Width = 59
        Height = 23
        Caption = 'Speed'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
      end
      object Panel50: TPanel
        Left = 16
        Top = 192
        Width = 59
        Height = 23
        Caption = 'Scureve'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 23
      end
      object btScvSet2: TButton
        Tag = 2
        Left = 164
        Top = 192
        Width = 87
        Height = 25
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 24
      end
      object btSetRatio2: TButton
        Tag = 2
        Left = 252
        Top = 73
        Width = 72
        Height = 25
        Caption = 'Setup'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 25
        OnClick = btSetRatio1Click
      end
    end
    object GroupBox6: TGroupBox
      Left = 666
      Top = 382
      Width = 346
      Height = 114
      Caption = 'Motor Limit Setting'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      object Panel45: TPanel
        Left = 7
        Top = 53
        Width = 83
        Height = 25
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object cbAxis0Limit: TCheckBox
          Left = 2
          Top = 4
          Width = 80
          Height = 17
          Caption = 'TiltMotor'
          TabOrder = 0
        end
      end
      object edAxis0MLimt: TEdit
        Tag = 1
        Left = 92
        Top = 53
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 1
        Text = '0'
        OnDblClick = edCmdValue1DblClick
      end
      object edAxis1MLimt: TEdit
        Tag = 1
        Left = 90
        Top = 78
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 2
        Text = '0'
        OnDblClick = edEncValue1DblClick
      end
      object edAxis0PLimt: TEdit
        Tag = 2
        Left = 178
        Top = 53
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 3
        Text = '0'
        OnDblClick = edCmdValue1DblClick
      end
      object edAxis1PLimt: TEdit
        Tag = 2
        Left = 178
        Top = 78
        Width = 87
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 4
        Text = '0'
      end
      object Panel52: TPanel
        Left = 89
        Top = 27
        Width = 88
        Height = 25
        Caption = '- LIMIT'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object Panel53: TPanel
        Left = 178
        Top = 27
        Width = 87
        Height = 25
        Caption = '+ LIMIT'
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object Button1: TButton
        Left = 270
        Top = 32
        Width = 75
        Height = 73
        Caption = 'OK'
        TabOrder = 7
        OnClick = Button1Click
      end
      object Panel51: TPanel
        Left = 7
        Top = 79
        Width = 83
        Height = 25
        Color = clMedGray
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        object cbAxis1Limit: TCheckBox
          Left = 2
          Top = 4
          Width = 80
          Height = 17
          Caption = 'SpinMotor'
          TabOrder = 0
        end
      end
    end
    object GroupBox7: TGroupBox
      Left = 736
      Top = 10
      Width = 260
      Height = 199
      Caption = 'In / Out Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      object Label1: TLabel
        Left = 24
        Top = 27
        Width = 100
        Height = 22
        Caption = 'Input Status'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 24
        Top = 107
        Width = 114
        Height = 22
        Caption = 'Output Status'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object pnIn11: TPanel
        Left = 103
        Top = 73
        Width = 27
        Height = 25
        Caption = '11'
        TabOrder = 0
      end
      object pnIn12: TPanel
        Left = 130
        Top = 73
        Width = 27
        Height = 25
        Caption = '12'
        TabOrder = 1
      end
      object pnIn13: TPanel
        Left = 157
        Top = 73
        Width = 27
        Height = 25
        Caption = '13'
        TabOrder = 2
      end
      object pnIn4: TPanel
        Left = 130
        Top = 48
        Width = 27
        Height = 25
        Caption = '4'
        TabOrder = 3
      end
      object pnIn5: TPanel
        Left = 157
        Top = 48
        Width = 27
        Height = 25
        Caption = '5'
        TabOrder = 4
      end
      object pnIn6: TPanel
        Left = 184
        Top = 48
        Width = 27
        Height = 25
        Caption = '6'
        TabOrder = 5
      end
      object pnIn3: TPanel
        Left = 103
        Top = 48
        Width = 27
        Height = 25
        Caption = '3'
        TabOrder = 6
      end
      object pnIn2: TPanel
        Left = 76
        Top = 48
        Width = 27
        Height = 25
        Caption = '2'
        TabOrder = 7
      end
      object pnIn1: TPanel
        Left = 49
        Top = 48
        Width = 27
        Height = 25
        Caption = '1'
        TabOrder = 8
      end
      object pnIn15: TPanel
        Left = 211
        Top = 73
        Width = 27
        Height = 25
        Caption = '15'
        TabOrder = 9
      end
      object pnIn9: TPanel
        Left = 49
        Top = 73
        Width = 27
        Height = 25
        Caption = '9'
        TabOrder = 10
      end
      object pnIn14: TPanel
        Left = 184
        Top = 73
        Width = 27
        Height = 25
        Caption = '14'
        TabOrder = 11
      end
      object pnIn0: TPanel
        Left = 22
        Top = 48
        Width = 27
        Height = 25
        Caption = '0'
        TabOrder = 12
      end
      object pnIn7: TPanel
        Left = 211
        Top = 48
        Width = 27
        Height = 25
        Caption = '7'
        TabOrder = 13
      end
      object pnIn8: TPanel
        Left = 22
        Top = 73
        Width = 27
        Height = 25
        Caption = '8'
        TabOrder = 14
      end
      object pnIn10: TPanel
        Left = 76
        Top = 73
        Width = 27
        Height = 25
        Caption = '10'
        TabOrder = 15
      end
      object pnOut11: TPanel
        Tag = 11
        Left = 103
        Top = 161
        Width = 27
        Height = 25
        Caption = '11'
        TabOrder = 16
        OnClick = pnOut0Click
      end
      object pnOut12: TPanel
        Tag = 12
        Left = 130
        Top = 161
        Width = 27
        Height = 25
        Caption = '12'
        TabOrder = 17
        OnClick = pnOut0Click
      end
      object pnOut13: TPanel
        Tag = 13
        Left = 157
        Top = 161
        Width = 27
        Height = 25
        Caption = '13'
        TabOrder = 18
        OnClick = pnOut0Click
      end
      object pnOut4: TPanel
        Tag = 4
        Left = 130
        Top = 136
        Width = 27
        Height = 25
        Caption = '4'
        TabOrder = 19
        OnClick = pnOut0Click
      end
      object pnOut5: TPanel
        Tag = 5
        Left = 157
        Top = 136
        Width = 27
        Height = 25
        Caption = '5'
        TabOrder = 20
        OnClick = pnOut0Click
      end
      object pnOut6: TPanel
        Tag = 6
        Left = 184
        Top = 136
        Width = 27
        Height = 25
        Caption = '6'
        TabOrder = 21
        OnClick = pnOut0Click
      end
      object pnOut3: TPanel
        Tag = 3
        Left = 103
        Top = 136
        Width = 27
        Height = 25
        Caption = '3'
        TabOrder = 22
        OnClick = pnOut0Click
      end
      object pnOut2: TPanel
        Tag = 2
        Left = 76
        Top = 136
        Width = 27
        Height = 25
        Caption = '2'
        TabOrder = 23
        OnClick = pnOut0Click
      end
      object pnOut1: TPanel
        Tag = 1
        Left = 49
        Top = 136
        Width = 27
        Height = 25
        Caption = '1'
        TabOrder = 24
        OnClick = pnOut0Click
      end
      object pnOut15: TPanel
        Tag = 15
        Left = 211
        Top = 161
        Width = 27
        Height = 25
        Caption = '15'
        TabOrder = 25
        OnClick = pnOut0Click
      end
      object pnOut9: TPanel
        Tag = 9
        Left = 49
        Top = 161
        Width = 27
        Height = 25
        Caption = '9'
        TabOrder = 26
        OnClick = pnOut0Click
      end
      object pnOut14: TPanel
        Tag = 14
        Left = 184
        Top = 161
        Width = 27
        Height = 25
        Caption = '14'
        TabOrder = 27
        OnClick = pnOut0Click
      end
      object pnOut0: TPanel
        Left = 22
        Top = 136
        Width = 27
        Height = 25
        Caption = '0'
        TabOrder = 28
        OnClick = pnOut0Click
      end
      object pnOut7: TPanel
        Tag = 7
        Left = 211
        Top = 136
        Width = 27
        Height = 25
        Caption = '7'
        TabOrder = 29
        OnClick = pnOut0Click
      end
      object pnOut8: TPanel
        Tag = 8
        Left = 22
        Top = 161
        Width = 27
        Height = 25
        Caption = '8'
        TabOrder = 30
        OnClick = pnOut0Click
      end
      object pnOut10: TPanel
        Tag = 10
        Left = 76
        Top = 161
        Width = 27
        Height = 25
        Caption = '10'
        TabOrder = 31
        OnClick = pnOut0Click
      end
    end
    object Edit1: TEdit
      Left = 752
      Top = 248
      Width = 121
      Height = 21
      ImeName = 'Microsoft IME 2010'
      TabOrder = 7
      Text = 'Edit1'
    end
    object Button3: TButton
      Tag = 2
      Left = 832
      Top = 288
      Width = 75
      Height = 25
      Caption = 'Button3'
      TabOrder = 8
      OnClick = Button2Click
    end
    object Button2: TButton
      Tag = 1
      Left = 752
      Top = 288
      Width = 75
      Height = 25
      Caption = 'Button2'
      TabOrder = 9
      OnClick = Button2Click
    end
    object GroupBox8: TGroupBox
      Left = 665
      Top = 510
      Width = 346
      Height = 114
      Caption = 'Motor Limit Setting'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 10
      object Button4: TButton
        Left = 270
        Top = 32
        Width = 75
        Height = 73
        Caption = 'OK'
        TabOrder = 0
        OnClick = Button4Click
      end
      object cbFrstmachine: TCheckBox
        Left = 17
        Top = 81
        Width = 152
        Height = 17
        Caption = '1'#54840#44592' '#49324#50857#49884
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object cbDryRun: TCheckBox
        Left = 17
        Top = 57
        Width = 160
        Height = 17
        Caption = 'Dry Running'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object cbIngrCvrSnsr: TCheckBox
        Left = 17
        Top = 33
        Width = 212
        Height = 17
        Caption = #45236#48512' '#52964#48260' '#49468#49436' '#47924#49884
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
    end
  end
  object ImageList1: TImageList
    Height = 23
    Width = 23
    Left = 792
    Top = 8
    Bitmap = {
      494C010102000400040017001700FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      00000000000036000000280000005C0000001700000001002000000000001021
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000848484004C4C4C003C3C
      3C005C5C5C008484840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800042424200333333005555
      5500808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000009C9C
      9C00545454008484840000000000C4C4C400A4A4A4007C7C7C005C5C5C009C9C
      9C00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000969696003366
      33003399660066CC990066CC9900669999003399660033663300969696000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000009C9C9C006C6C6C0000000000D4D4D400E4E4E400E4E4
      E400E4E4E400DCDCDC00B4B4B4006C6C6C009C9C9C0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000999999003366660066CC990066FFCC0099FFCC0099FFCC0099FF
      CC0099FFCC0066CC990033996600969696000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000545454009494
      9400DCDCDC00DCDCDC00E4E4E400E4E4E400DCDCDC00E4E4E400DCDCDC00A4A4
      A4005C5C5C000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000003366330033CC660066FF
      CC0099FFCC0099FFCC0099FFCC0066FFCC0066FFCC0066FFCC0066CC99003366
      3300000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000949494007474740000000000CCCCCC00D4D4D400D4D4D400E4E4
      E400E4E4E400DCDCDC00CCCCCC00BCBCBC007C7C7C0094949400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000669999003399660066CC990066FF990066FF990099FFCC0099FFCC0099FF
      CC0066FFCC0066FF990066CC9900339966008686860000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000646464008C8C8C00B4B4
      B40000000000CCCCCC00CCCCCC00D4D4D400CCCCCC00CCCCCC00B4B4B4000000
      00007C7C7C006464640000000000000000000000000000000000000000000000
      000000000000000000000000000000000000555555006699660066CC990033CC
      990066FF990066FF990099FFCC0066FF990066FF990033CC990033CC99003399
      6600555555000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000343434008484840094949400A4A4A400B4B4B400BCBCBC00C4C4
      C400C4C4C400B4B4B40000000000949494008C8C8C003C3C3C00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000222222003399660033CC660033CC660066CC990066CC990066CC990066CC
      990066CC990033CC660033CC6600339966002222220000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000044444400747474008484
      840094949400A4A4A400000000000000000000000000A4A4A4008C8C8C008484
      8400848484004C4C4C0000000000000000000000000000000000000000000000
      00000000000000000000000000000000000033663300339966003399660033CC
      660033CC990033CC990066CC990066CC990033CC990033CC6600339966003399
      6600336633000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007C7C7C0064646400747474007C7C7C0094949400949494009494
      94008C8C8C009494940084848400747474005C5C5C007C7C7C00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007777770033663300339966000099660033CC660033CC660033CC66003399
      660033CC66003399660033996600336633007777770000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000545454006C6C
      6C00747474007C7C7C008C8C8C009494940094949400848484007C7C7C006C6C
      6C004C4C4C000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000033663300339933003399
      6600339966003399660033CC660033CC66003399660033996600339966003366
      3300000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000008C8C8C005C5C5C00A4A4A400B4B4B400BCBCBC00BCBC
      BC00B4B4B400B4B4B4009C9C9C005C5C5C009494940000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000868686004D4D4D0099CC990099CC990099CC990099CC990099CC
      990099CC9900669999004D4D4D00969696000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000009C9C
      9C008C8C8C00CCCCCC00ECECEC00ECECEC00E4E4E400D4D4D4008C8C8C009494
      9400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000969696008686
      8600C0DCC000EAEAEA00EAEAEA00E3E3E300CCCCCC0086868600969696000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000B4B4B400C4C4C400CCCC
      CC00C4C4C400B4B4B40000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000B2B2B200C0C0C000CCCCCC00CBCB
      CB00B2B2B2000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000424D3E000000000000003E000000280000005C0000001700000001000100
      00000000140100000000000000000000000000000000000000000000FFFFFF00
      FFFFFFFFFFFC000000000000FFFFFFFFFFFC000000000000FFFFFFFFFFFC0000
      00000000FFFFFFFFFFFC000000000000FFFFFFFFFFFC000000000000FFFFFFFF
      FFFC000000000000FF83FFFF07FC000000000000FE20FFFC01FC000000000000
      FC807FF800FC000000000000FC007FF800FC000000000000F9003FF0007C0000
      00000000F8813FF0007C000000000000F8023FF0007C000000000000F8383FF0
      007C000000000000F8003FF0007C000000000000FC007FF800FC000000000000
      FC007FF800FC000000000000FE00FFFC01FC000000000000FF83FFFF07FC0000
      00000000FFFFFFFFFFFC000000000000FFFFFFFFFFFC000000000000FFFFFFFF
      FFFC000000000000FFFFFFFFFFFC000000000000000000000000000000000000
      00000000000000000000}
  end
  object Timer1: TTimer
    Interval = 300
    OnTimer = Timer1Timer
    Left = 976
    Top = 32
  end
  object Timer2: TTimer
    Interval = 100
    OnTimer = Timer2Timer
    Left = 24
    Top = 8
  end
end
