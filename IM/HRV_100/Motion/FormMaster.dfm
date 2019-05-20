object FrmMaster: TFrmMaster
  Left = 773
  Top = 225
  AutoScroll = False
  Caption = 'FrmMaster'
  ClientHeight = 453
  ClientWidth = 877
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 877
    Height = 453
    ActivePage = TabSheet3
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    TabPosition = tpBottom
    object TabSheet3: TTabSheet
      Caption = 'Option'
      ImageIndex = 2
      object Label7: TLabel
        Left = 13
        Top = 32
        Width = 235
        Height = 19
        Caption = 'VISION Y MAX CAMERA Minute'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label8: TLabel
        Left = 378
        Top = 36
        Width = 16
        Height = 13
        Caption = 'min'
      end
      object Label23: TLabel
        Left = 445
        Top = 32
        Width = 240
        Height = 19
        Caption = 'DriveTilt,Sensitibity Setting Unit'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label24: TLabel
        Left = 18
        Top = 52
        Width = 130
        Height = 13
        Caption = 'DO NOT Change this value'
      end
      object edVisnYMaxMin: TEdit
        Left = 256
        Top = 32
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 0
        Text = '120'
      end
      object cbSetUnitHeight: TComboBox
        Left = 689
        Top = 28
        Width = 124
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 16
        ParentFont = False
        TabOrder = 1
        Text = 'cbSetUnitHeight'
        Items.Strings = (
          'Current Unit'
          'Height Unit')
      end
      object GroupBox3: TGroupBox
        Left = 440
        Top = 80
        Width = 425
        Height = 241
        Caption = 'Device Output Gain Offset'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        object lbPreLoad: TLabel
          Left = 13
          Top = 40
          Width = 59
          Height = 19
          Caption = 'Preload'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbPeakStroke: TLabel
          Left = 13
          Top = 65
          Width = 92
          Height = 19
          Caption = 'Peak Stroke'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label27: TLabel
          Left = 13
          Top = 91
          Width = 114
          Height = 19
          Caption = 'Custom Stroke'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label28: TLabel
          Left = 13
          Top = 117
          Width = 79
          Height = 19
          Caption = 'Sensitivity'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label29: TLabel
          Left = 13
          Top = 143
          Width = 66
          Height = 19
          Caption = 'Linearity'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label32: TLabel
          Left = 173
          Top = 16
          Width = 35
          Height = 19
          Caption = 'Gain'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label33: TLabel
          Left = 285
          Top = 16
          Width = 45
          Height = 19
          Caption = 'Offset'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbImpedance: TLabel
          Left = 13
          Top = 170
          Width = 84
          Height = 19
          Caption = 'Impedance'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label31: TLabel
          Left = 13
          Top = 196
          Width = 81
          Height = 19
          Caption = 'Hysteresis'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object edGainPreLoad: TEdit
          Left = 163
          Top = 39
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 0
          Text = '1'
        end
        object edOfstPreLoad: TEdit
          Left = 274
          Top = 39
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 1
          Text = '0'
        end
        object edGainPeakStroke: TEdit
          Left = 163
          Top = 65
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
          Text = '1'
        end
        object edOfstPeakStroke: TEdit
          Left = 274
          Top = 65
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
          Text = '0'
        end
        object edGainCustomStroke: TEdit
          Left = 163
          Top = 91
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 4
          Text = '1'
        end
        object edOfstCustomStroke: TEdit
          Left = 274
          Top = 91
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 5
          Text = '0'
        end
        object edGainSensitivity: TEdit
          Left = 163
          Top = 117
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
          Text = '1'
        end
        object edOfstSensitivity: TEdit
          Left = 274
          Top = 117
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
          Text = '0'
        end
        object edGainLinearity: TEdit
          Left = 163
          Top = 143
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 12
          Text = '1'
        end
        object edOfstLinearity: TEdit
          Left = 274
          Top = 143
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 13
          Text = '0'
        end
        object edGainImpedance: TEdit
          Left = 163
          Top = 170
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 8
          Text = '1'
        end
        object edOfstImpedance: TEdit
          Left = 274
          Top = 170
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 9
          Text = '0'
        end
        object edGainHysteresis: TEdit
          Left = 163
          Top = 196
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 10
          Text = '1'
        end
        object edOfstHysteresis: TEdit
          Left = 274
          Top = 196
          Width = 104
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 11
          Text = '0'
        end
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 80
        Width = 417
        Height = 321
        Caption = 'Stage Offset'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        object Label1: TLabel
          Left = 13
          Top = 32
          Width = 218
          Height = 19
          Caption = 'STAGE1 MASTER OFFSET X'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label2: TLabel
          Left = 378
          Top = 36
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label3: TLabel
          Left = 13
          Top = 56
          Width = 217
          Height = 19
          Caption = 'STAGE1 MASTER OFFSET Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label4: TLabel
          Left = 378
          Top = 60
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label5: TLabel
          Left = 13
          Top = 88
          Width = 218
          Height = 19
          Caption = 'STAGE2 MASTER OFFSET X'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label6: TLabel
          Left = 378
          Top = 92
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label9: TLabel
          Left = 13
          Top = 112
          Width = 217
          Height = 19
          Caption = 'STAGE2 MASTER OFFSET Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label10: TLabel
          Left = 378
          Top = 116
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label11: TLabel
          Left = 13
          Top = 144
          Width = 218
          Height = 19
          Caption = 'STAGE3 MASTER OFFSET X'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label12: TLabel
          Left = 378
          Top = 148
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label13: TLabel
          Left = 13
          Top = 168
          Width = 217
          Height = 19
          Caption = 'STAGE3 MASTER OFFSET Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label14: TLabel
          Left = 378
          Top = 172
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label15: TLabel
          Left = 13
          Top = 200
          Width = 218
          Height = 19
          Caption = 'STAGE4 MASTER OFFSET X'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label16: TLabel
          Left = 378
          Top = 204
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label17: TLabel
          Left = 13
          Top = 224
          Width = 217
          Height = 19
          Caption = 'STAGE4 MASTER OFFSET Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label18: TLabel
          Left = 378
          Top = 228
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label19: TLabel
          Left = 13
          Top = 256
          Width = 218
          Height = 19
          Caption = 'STAGE5 MASTER OFFSET X'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label20: TLabel
          Left = 378
          Top = 260
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object Label21: TLabel
          Left = 13
          Top = 280
          Width = 217
          Height = 19
          Caption = 'STAGE5 MASTER OFFSET Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label22: TLabel
          Left = 378
          Top = 284
          Width = 29
          Height = 20
          Caption = 'min'
        end
        object edMstOfsX1: TEdit
          Left = 256
          Top = 32
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 0
          Text = '0'
        end
        object edMstOfsY1: TEdit
          Left = 256
          Top = 56
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 1
          Text = '0'
        end
        object edMstOfsX2: TEdit
          Left = 256
          Top = 88
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
          Text = '0'
        end
        object edMstOfsY2: TEdit
          Left = 256
          Top = 112
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
          Text = '0'
        end
        object edMstOfsX3: TEdit
          Left = 256
          Top = 144
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 4
          Text = '0'
        end
        object edMstOfsY3: TEdit
          Left = 256
          Top = 168
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 5
          Text = '0'
        end
        object edMstOfsX4: TEdit
          Left = 256
          Top = 200
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
          Text = '0'
        end
        object edMstOfsY4: TEdit
          Left = 256
          Top = 224
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
          Text = '0'
        end
        object edMstOfsX5: TEdit
          Left = 256
          Top = 256
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 8
          Text = '0'
        end
        object edMstOfsY5: TEdit
          Left = 256
          Top = 280
          Width = 121
          Height = 28
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 9
          Text = '0'
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Debug'
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 869
        Height = 427
        Align = alClient
        BevelOuter = bvNone
        Caption = 'Debug'
        TabOrder = 0
        object Panel5: TPanel
          Left = 0
          Top = 0
          Width = 869
          Height = 28
          Align = alTop
          BevelOuter = bvNone
          Caption = 'DEBUG'
          Color = clSkyBlue
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object GroupBox8: TGroupBox
          Left = 16
          Top = 43
          Width = 625
          Height = 366
          Caption = 'Hello World'
          TabOrder = 1
          object lbMan: TLabel
            Left = 224
            Top = 420
            Width = 29
            Height = 13
            Caption = 'lbMan'
          end
          object sgCycleStep: TStringGrid
            Left = 142
            Top = 16
            Width = 475
            Height = 257
            ColCount = 7
            RowCount = 7
            TabOrder = 0
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24)
          end
          object btPartAutorun: TButton
            Left = 19
            Top = 128
            Width = 106
            Height = 49
            Caption = 'Part Autorun'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = btPartAutorunClick
          end
          object btPartReset: TButton
            Left = 19
            Top = 72
            Width = 106
            Height = 49
            Caption = 'Part Reset'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = btPartResetClick
          end
          object btAllReset: TButton
            Left = 19
            Top = 16
            Width = 106
            Height = 49
            Caption = 'All Reset'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnClick = btAllResetClick
          end
          object btPartHome: TButton
            Left = 19
            Top = 264
            Width = 106
            Height = 49
            Caption = 'Part Home'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = btPartHomeClick
          end
          object btAllCheck: TButton
            Left = 16
            Top = 184
            Width = 105
            Height = 25
            Caption = 'ALL CHECK'
            TabOrder = 5
            OnClick = btAllCheckClick
          end
          object cbDebugMode: TCheckBox
            Left = 160
            Top = 277
            Width = 185
            Height = 33
            Caption = 'Debug Mode Use'
            Font.Charset = ARABIC_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
        end
        object sgTime: TStringGrid
          Left = 648
          Top = 72
          Width = 169
          Height = 209
          ColCount = 2
          RowCount = 7
          FixedRows = 0
          TabOrder = 2
          RowHeights = (
            24
            24
            24
            24
            24
            24
            24)
        end
        object cbPartSel: TComboBox
          Left = 648
          Top = 48
          Width = 169
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          ItemHeight = 0
          TabOrder = 3
          Text = 'cbPartSel'
          OnChange = cbPartSelChange
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'RS232Test'
      ImageIndex = 1
      object GroupBox1: TGroupBox
        Left = 8
        Top = 16
        Width = 193
        Height = 153
        Caption = 'VCM'
        TabOrder = 0
        object Button10: TButton
          Left = 14
          Top = 56
          Width = 75
          Height = 25
          Caption = 'Set Crnt'
          TabOrder = 0
          OnClick = Button10Click
        end
        object Edit1: TEdit
          Left = 16
          Top = 28
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 1
          Text = '10'
        end
        object Button11: TButton
          Left = 14
          Top = 88
          Width = 75
          Height = 25
          Caption = 'Rqst Crnt Volt'
          TabOrder = 2
          OnClick = Button11Click
        end
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 184
        Width = 185
        Height = 153
        Caption = 'GroupBox2'
        TabOrder = 1
        object Button12: TButton
          Left = 22
          Top = 32
          Width = 75
          Height = 25
          Caption = 'GetCrntHeight'
          TabOrder = 0
          OnClick = Button12Click
        end
        object Button13: TButton
          Left = 14
          Top = 104
          Width = 75
          Height = 25
          Caption = 'Button13'
          TabOrder = 1
          OnClick = Button13Click
        end
        object Edit2: TEdit
          Left = 8
          Top = 80
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
          Text = 'VS,01'
        end
        object Button14: TButton
          Left = 104
          Top = 32
          Width = 75
          Height = 25
          Caption = 'Button14'
          TabOrder = 3
          OnClick = Button14Click
        end
      end
    end
  end
  object btSaveMotor: TBitBtn
    Left = 672
    Top = 352
    Width = 192
    Height = 61
    Caption = 'SAVE '
    Font.Charset = ARABIC_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btSaveMotorClick
    Glyph.Data = {
      360C0000424D360C000000000000360000002800000020000000200000000100
      180000000000000C000000000000000000000000000000000000FFFFFFDBDBDB
      9494947070707070707070707070707070707070707070707070707070707070
      7070707070707070707070707070707070707070707070707070707070707070
      7070707070707070707070707070707070707070707070949494FFFFFF944323
      9443239443239443239443239443239443239443239443239443239443239443
      2394432394432394432394432394432394432394432394432394432394432394
      4323944323944323944323944323944323944323944323707070FFFFFF944323
      E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6
      A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4
      C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2E4C6A2944323707070FFFFFF944323
      E4C6A2CD9954CD9954D09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E
      5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD0
      9E5CD09E5CD09E5CD09E5CCE9953CE9953E4C6A2944323707070FFFFFF944323
      E4C6A2CF9C58CF9C58D09E5CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFD09E5CCF9C58CF9C56E4C6A2944323707070FFFFFF944323
      E4C6A2D09E5CD09E5DD09E5CFFFFFFFDF8F3FDF6F0FDF5EDFDF4EBFCF2E8FCF1
      E6FBEFE2FBEDE0FBECDDFBEBDAFAEAD8FAE8D5F9E6D2F9E5D0F9E4CDF9E3CBF9
      E2CAF8E1C8FFFFFFD09E5CD09E5DD09E5DE4C6A2944323707070FFFFFF944323
      E4C6A2D2A261D2A261D09E5CFFFFFFFDF8F4FDF8F1FDF6EFFDF4EDFDF3EAFCF2
      E8FCF0E5FBEFE2FBEDE0FBECDCFBEADAFAE9D7FAE8D5F9E6D2F9E5CFF9E4CDF9
      E2CAF9E2C9FFFFFFD09E5CD2A262D2A262E4C6A2944323707070FFFFFF944323
      E4C6A2D4A567D4A667D09E5CFFFFFFFEF9F5ECD9C3ECD8C2ECD7C0ECD7BFECD5
      BDECD5BAEBD3B9EBD2B7EBD2B4EBD0B3EAD0B1EACFAFEACEADEACDADEACDAAEA
      CCA8F9E2CAFFFFFFD09E5CD4A567D4A567E4C6A2944323707070FFFFFF944323
      E4C6A2D6A86CD5A96CD09E5CFFFFFFFEFAF7FEF9F4FDF8F2FDF6F0FDF5EDFDF3
      EBFCF2E8FCF1E6FBEFE2FBEDE0FBECDDFBEADAFAEAD8FAE8D5F9E6D2F9E5D0F9
      E4CEF9E2CBFFFFFFD09E5CD5A96CD5A86DE4C6A2944323707070FFFFFF944323
      E4C6A2D7AC72D7AC72D09E5CFFFFFFFEFBF8FEFAF6FEF9F4FDF8F1FDF6EFFDF4
      EDFDF3EAFCF2E8FCF0E5FBEFE2FBEDDFFBECDDFBEADAFAE9D7FAE8D5F9E6D2F9
      E5CFF9E4CDFFFFFFD09E5CD7AC72D7AC72E4C6A2944323707070FFFFFF944323
      E4C6A2DAAE78DAAE77D09E5CFFFFFFFEFCF9ECDAC6ECDAC4ECD9C3ECD8C2ECD7
      BFECD7BEECD5BCECD5BAEBD3B9EBD2B7EBD2B4EBD0B3EAD0B1EACFAFEACEADEA
      CDADF9E4CEFFFFFFD09E5CDAAE77DAAF77E4C6A2944323707070FFFFFF944323
      E4C6A2DCB483DBB27DD09E5CFFFFFFFEFDFBFEFBF9FEFAF7FEF9F4FDF8F3FDF6
      F0FDF5EDFDF4EBFCF2E8FCF1E6FBEFE2FBEDE0FBECDDFBEBDBFAEAD8FAE8D5F9
      E6D3F9E5D0FFFFFFD09E5CDBB17DDBB17DE4C6A2944323707070FFFFFF944323
      E4C6A2E2C199DEB98AD09E5CFFFFFFFEFDFBFEFCFAFEFBF8FEFAF6FEF9F4FDF8
      F2FDF6EFFDF4EDFDF3EAFCF2E8FCF0E5FBEFE2FBEDDFFBECDDFBEADAFAE9D7FA
      E8D5F9E6D2FFFFFFD09E5CDCB481DCB481E4C6A2944323707070FFFFFF944323
      E6CAA9E6C9A8E4C6A2D09E5CFFFFFFFEFEFDEDDCC9EDDBC7ECDAC6ECDAC4ECD9
      C3ECD8C2ECD7BFECD7BFECD5BCECD5BAEBD3B9EBD2B7EBD2B4EBD0B3EAD0B1EA
      CFAFF9E7D3FFFFFFD09E5CDEB886DEB886E6CAA9944323707070FFFFFF944323
      E8CDAFE8CDAEE8CDAED09E5CFFFFFFFFFEFDFEFDFCFEFDFBFEFBF9FEFAF7FEF9
      F4FDF8F2FDF6F0FDF5EDFDF4EBFCF2E8FCF1E6FBEFE2FBEDE0FBECDDFBEBDAFA
      EAD8FAE8D5FFFFFFD09E5CE0BA8AE0BA8AE8CDAE944323707070FFFFFF944323
      EAD0B4EAD1B4EAD0B4D09E5CFFFFFFFFFEFEFFFEFDFEFDFBFEFCFAFEFBF8FEFA
      F6FEF9F4FDF8F1FDF6EFFDF4EDFDF3EAFCF2E8FCF0E5FBEFE2FBEDE0FBECDDFB
      EADAFAE9D7FFFFFFD09E5CE1BD8FE1BD8FEAD0B4944323707070FFFFFF944323
      EBD4BAEBD3B9EBD3B9D09E5CFFFFFFFFFFFFFFFEFEFEFEFDFEFDFBFEFCF9FEFB
      F8FEF9F5FEF8F3FDF7F1FDF6EFFDF4ECFDF3EAFCF1E6FBEFE4FBEEE1FBEDDEFB
      EBDCFAEAD9FFFFFFD09E5CE2BF92E2BF92EAD2B7944323707070FFFFFF944323
      EDD6BEECD7BEECD6BED09E5CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFD09E5CE4C095E4C195ECD5BB944323707070FFFFFF944323
      EED9C3EED9C2EED8C1D09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E
      5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD09E5CD0
      9E5CD09E5CD09E5CD09E5CE5C399E5C298EDD7BF944323707070FFFFFF944323
      EFDCC6EFDBC5EFDAC5EFDBC4EFDAC4EFDAC3EEDAC3EFD9C3EEDAC3EFD9C2EED8
      C2EED8C0EDD8C0EED7BFEDD7BFEDD6BCECD2B5EAD0B0EACCACE9CAA7E8C8A3E8
      C69FE7C69DE6C49BE6C49AE6C59BE6C69DEEDAC3944323707070FFFFFF944323
      F1DEC9F1DEC9F0DDC8F0DDC7F0DCC7F0DCC6F0DCC6F0DCC6EFDCC6EFDBC5EFDB
      C5EFDAC5EFDAC4EFDAC3EFDAC3EFDAC3EFDAC2EFDAC1EFD9C0EED8C0EED8BFEE
      D8BFEED8BFEED7BFEED7BEEDD7BDEDD7BDEDD7BC944323707070FFFFFF944323
      F1DFCCF1DFCCF1DFCBF1DECAF1DECAF1DECAF1DECAF1DEC9F1DEC9F1DDC9F0DD
      C7F0DCC7F0DCC6F0DCC7F0DCC6F0DCC6F0DCC5EFDBC5EFDAC4EFDAC3EFDAC3EF
      DAC3EFDAC3EFDAC2EFDAC2EFD9C1EFD9C0EFD8C0944323707070FFFFFF944323
      F3E1CFF2E1CEF2E0CEF2E0CDF2E0CD7878787878787878787878787878787878
      7878787878787878787878787878787878787878787878787878787878787878
      7878787878F0DBC5F0DCC4F0DBC3EFDAC3EFDBC3944323707070FFFFFF944323
      F3E2D1F3E2D1F3E2D0F3E2D0F3E2D0787878A8A8A8ABABABADADADB0B0B0B4B4
      B4B8B8B8BCBDBDC1C1C1C5C5C5C9C9CACDCDCDD1D0D0D4D4D4D7D7D7DADADADC
      DCDC787878F1DEC8F1DEC8F1DCC7F1DDC6F1DCC6944323707070FFFFFF944323
      F4E4D3F4E4D2F4E3D2F3E3D2F3E3D2787878A8A8A8AAAAABADADADB1B0B0B4B4
      B4B8B8B9BCBDBCC1C1C1C5C5C5C9C9CACDCDCD979797838383636363DADADADC
      DCDC787878F2DFCAF2DFCAF2DECAF1DECAF1DEC9944323707070FFFFFF944323
      F4E5D5F4E4D5F4E5D5F4E4D4F4E4D4787878A8A8A8ABAAABADADADB1B0B1B4B4
      B4B9B8B8BDBCBDC1C1C0C5C5C5C9CACACDCDCD979797838383656563DADADADC
      DCDC787878F3E0CDF3E0CDF3E0CDF3E0CDF3E0CC944323707070FFFFFF944323
      F4E6D6F4E6D6F4E6D5F4E6D5F4E5D5787878A8A8A8AAAAAAADADADB1B0B1B4B4
      B4B9B8B9BCBDBDC1C1C1C5C5C5C9C9CACDCDCD979797838484636563DADADADC
      DCDC787878F3E2CFF3E2CFF3E2CEF3E1CEF3E0CE9A4C2B858585FFFFFF98492A
      D2AF95F5E6D7F5E6D7F5E6D7F5E6D7787878A8A8A8AAABABADADADB1B0B1B4B4
      B4B9B9B8BDBCBDC1C1C1C5C5C5C9C9CACDCDCD979797838384656565DADADADC
      DCDC787878F4E3D2F4E3D1F4E2D1F3E2D0BA8566954426BABABAFFFFFFEADBD5
      974728D7B59BF5E6D7F5E7D8F5E7D8787878A8A8A8ABAAABADADADB1B0B1B4B4
      B4B9B8B8BDBCBDC1C0C0C5C5C5CACAC9CDCDCD979797848483656363DADADADC
      DCDC787878F4E4D3F4E4D2F4E4D2CEA88C954528B6A69FEBEBEBFFFFFFFFFFFF
      E5D1CA97492A944323944323944323787878A8A8A8AAAAABADADADB0B0B1B4B4
      B4B8B8B9BDBCBCC0C1C1C5C5C5C9C9C9CDCDCD979797838384656565DADADADC
      DCDC78787894432394432394432396492AD0C1BCEFEFEFFDFDFDFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF787878A8A8A8ABABABADADADB0B0B1B4B4
      B4B9B8B8BDBCBCC0C1C1C5C5C5CAC9CACDCDCDD1D0D1D4D4D5D7D7D7DADADADC
      DCDC787878FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7878787878787878787878787878787878
      7878787878787878787878787878787878787878787878787878787878787878
      7878787878FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    Layout = blGlyphRight
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tmUpdateTimer
    Left = 8
    Top = 8
  end
  object dgFilePath: TOpenDialog
    Left = 400
    Top = 560
  end
end
