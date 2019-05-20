object FrmTestCon: TFrmTestCon
  Left = 889
  Top = 205
  BorderStyle = bsDialog
  ClientHeight = 673
  ClientWidth = 899
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
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 467
    Height = 311
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label15: TLabel
      Left = 179
      Top = 41
      Width = 27
      Height = 19
      Caption = 'Min'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label16: TLabel
      Left = 309
      Top = 41
      Width = 31
      Height = 19
      Caption = 'Max'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label17: TLabel
      Left = 427
      Top = 124
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label18: TLabel
      Left = 427
      Top = 202
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label19: TLabel
      Left = 427
      Top = 150
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label6: TLabel
      Left = 427
      Top = 72
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label25: TLabel
      Left = 427
      Top = 97
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label28: TLabel
      Left = 427
      Top = 228
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label37: TLabel
      Left = 426
      Top = 176
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object Label35: TLabel
      Left = 427
      Top = 254
      Width = 16
      Height = 14
      Caption = 'min'
    end
    object cbUseTiltInit: TCheckBox
      Tag = 6
      Left = 10
      Top = 116
      Width = 413
      Height = 24
      Caption = 'Init Tilt Abs'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 2
    end
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 465
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'TILT OK/NG CONDITION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbUseTiltFull: TCheckBox
      Tag = 1
      Left = 10
      Top = 194
      Width = 413
      Height = 24
      Caption = 'Full Tilt'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
    object cbUseTiltDrive: TCheckBox
      Tag = 1
      Left = 10
      Top = 142
      Width = 413
      Height = 24
      Caption = 'Drive Tilt'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 3
    end
    object edMinTiltInit: TEdit
      Left = 163
      Top = 117
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 4
      Text = '1000'
    end
    object edMaxTiltInit: TEdit
      Left = 292
      Top = 117
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 5
      Text = '1000'
    end
    object edMinTiltFull: TEdit
      Left = 163
      Top = 195
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 6
      Text = '1000'
    end
    object edMaxTiltFull: TEdit
      Left = 292
      Top = 195
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 7
      Text = '1000'
    end
    object edMinTiltDrive: TEdit
      Left = 163
      Top = 143
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 8
      Text = '1000'
    end
    object edMaxTiltDrive: TEdit
      Left = 292
      Top = 143
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 9
      Text = '1000'
    end
    object cbUseTiltInitR: TCheckBox
      Tag = 6
      Left = 10
      Top = 64
      Width = 413
      Height = 24
      Caption = 'Init Tilt X Abs'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 10
    end
    object cbUseTiltInitT: TCheckBox
      Tag = 1
      Left = 10
      Top = 90
      Width = 413
      Height = 24
      Caption = 'Init Tilt Y Abs'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 11
    end
    object edMinTiltInitR: TEdit
      Left = 163
      Top = 65
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 12
      Text = '1000'
    end
    object edMaxTiltInitR: TEdit
      Left = 292
      Top = 65
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 13
      Text = '1000'
    end
    object edMinTiltInitT: TEdit
      Left = 163
      Top = 91
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 14
      Text = '1000'
    end
    object edMaxTiltInitT: TEdit
      Left = 292
      Top = 91
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 15
      Text = '1000'
    end
    object cbUseTiltMax: TCheckBox
      Tag = 1
      Left = 10
      Top = 220
      Width = 413
      Height = 24
      Caption = 'Max Tilt'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 16
    end
    object edMinTiltMax: TEdit
      Left = 163
      Top = 221
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 17
      Text = '1000'
    end
    object edMaxTiltMax: TEdit
      Left = 292
      Top = 221
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 18
      Text = '1000'
    end
    object cbUseTiltDriveAbs: TCheckBox
      Tag = 1
      Left = 9
      Top = 168
      Width = 413
      Height = 24
      Caption = 'Total Tilt Abs'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 19
    end
    object edMinTiltDriveAbs: TEdit
      Left = 163
      Top = 169
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 20
      Text = '1000'
    end
    object edMaxTiltDriveAbs: TEdit
      Left = 291
      Top = 169
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 21
      Text = '1000'
    end
    object cbUseTiltLength: TCheckBox
      Tag = 1
      Left = 10
      Top = 246
      Width = 413
      Height = 24
      Caption = 'Length'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 22
    end
    object edMinTiltLength: TEdit
      Left = 163
      Top = 247
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 23
      Text = '1000'
    end
    object edMaxTiltLength: TEdit
      Left = 292
      Top = 247
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 24
      Text = '1000'
    end
  end
  object btSave: TBitBtn
    Left = 584
    Top = 606
    Width = 305
    Height = 59
    Caption = ' SAVE'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = btSaveClick
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
  end
  object Panel7: TPanel
    Left = 467
    Top = 120
    Width = 431
    Height = 482
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object Label7: TLabel
      Left = 10
      Top = 88
      Width = 95
      Height = 19
      Caption = 'Stable Delay'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label8: TLabel
      Left = 393
      Top = 92
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Label9: TLabel
      Left = 10
      Top = 112
      Width = 82
      Height = 19
      Caption = 'Step Delay'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label10: TLabel
      Left = 393
      Top = 116
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Label11: TLabel
      Left = 10
      Top = 136
      Width = 115
      Height = 19
      Caption = 'Step Increment'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label12: TLabel
      Left = 393
      Top = 140
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbDrvTtTestCrnt: TLabel
      Left = 10
      Top = 64
      Width = 169
      Height = 19
      Caption = 'Drive Tilt Test  Current'
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object lbDrvTtTestUnit: TLabel
      Left = 393
      Top = 68
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Label26: TLabel
      Left = 10
      Top = 160
      Width = 161
      Height = 19
      Caption = 'Rated Stroke Current'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label27: TLabel
      Left = 393
      Top = 164
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbPreLoadCheckHt: TLabel
      Left = 10
      Top = 184
      Width = 166
      Height = 19
      Caption = 'Check Preload Height'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label30: TLabel
      Left = 393
      Top = 188
      Width = 14
      Height = 14
      Caption = #956'm'
    end
    object lbImpdCheckCrnt: TLabel
      Left = 10
      Top = 208
      Width = 200
      Height = 19
      Caption = 'Resistance Check Current'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label32: TLabel
      Left = 393
      Top = 212
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbSensiSttUnit: TLabel
      Left = 393
      Top = 236
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbSensiSttCrnt: TLabel
      Left = 10
      Top = 232
      Width = 225
      Height = 19
      Caption = 'Sensitive Check Start Current'
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object lbSensiEndUnit: TLabel
      Left = 393
      Top = 260
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbSensiEndCrnt: TLabel
      Left = 10
      Top = 256
      Width = 220
      Height = 19
      Caption = 'Sensitive Check End Current'
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label39: TLabel
      Left = 393
      Top = 437
      Width = 14
      Height = 14
      Caption = 'um'
    end
    object Label40: TLabel
      Left = 10
      Top = 433
      Width = 92
      Height = 19
      Caption = 'Graph Min Y'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label41: TLabel
      Left = 393
      Top = 461
      Width = 14
      Height = 14
      Caption = 'um'
    end
    object Label42: TLabel
      Left = 10
      Top = 457
      Width = 96
      Height = 19
      Caption = 'Graph Max Y'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbDrvTtStartCrnt: TLabel
      Left = 10
      Top = 40
      Width = 169
      Height = 19
      Caption = 'Drive Tilt Start Current'
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object lbDrvTtStartUnit: TLabel
      Left = 393
      Top = 44
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Label4: TLabel
      Left = 393
      Top = 284
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Label5: TLabel
      Left = 10
      Top = 280
      Width = 221
      Height = 19
      Caption = 'Linearity Check Start Current'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label33: TLabel
      Left = 393
      Top = 308
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Label34: TLabel
      Left = 10
      Top = 304
      Width = 216
      Height = 19
      Caption = 'Linearity Check End Current'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object lbHysteSttUnit: TLabel
      Left = 393
      Top = 332
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbHysteSttCrnt: TLabel
      Left = 10
      Top = 328
      Width = 236
      Height = 19
      Caption = 'Hysteresis Check Start Current'
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object lbHysteEndUnit: TLabel
      Left = 393
      Top = 356
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object lbHysteEndCrnt: TLabel
      Left = 10
      Top = 352
      Width = 231
      Height = 19
      Caption = 'Hysteresis Check End Current'
      Color = clGradientActiveCaption
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label36: TLabel
      Left = 10
      Top = 376
      Width = 195
      Height = 19
      Caption = 'Pre Hysteresis Max Offset'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label43: TLabel
      Left = 393
      Top = 380
      Width = 14
      Height = 14
      Caption = #956'm'
    end
    object Panel8: TPanel
      Left = 1
      Top = 1
      Width = 429
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'TEST SETTING'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object edStableDelay: TEdit
      Left = 271
      Top = 88
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 1
      Text = '1000'
    end
    object edStepDelay: TEdit
      Left = 271
      Top = 112
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 2
      Text = '1000'
    end
    object edStepIncrement: TEdit
      Left = 271
      Top = 136
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 3
      Text = '10'
    end
    object cbFailIOST: TComboBox
      Left = 3
      Top = 8
      Width = 102
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 4
      Text = 'cbFailIOST'
      Visible = False
      Items.Strings = (
        'Bin1'
        'Bin2'
        'Bin3'
        'Bin4'
        'Bin5'
        'Bin6'
        'Bin7'
        'Bin8')
    end
    object edDrvTtTestCrnt: TEdit
      Left = 271
      Top = 64
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 5
      Text = '10'
    end
    object edCustomStroke: TEdit
      Left = 271
      Top = 160
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 6
      Text = '10'
    end
    object edPreLoadCheckHt: TEdit
      Left = 271
      Top = 184
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 7
      Text = '10'
    end
    object edImpdCheckCrnt: TEdit
      Left = 271
      Top = 208
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 8
      Text = '10'
    end
    object edSensiSttCrnt: TEdit
      Left = 271
      Top = 230
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 9
      Text = '10'
    end
    object edSensiEndCrnt: TEdit
      Left = 271
      Top = 254
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 10
      Text = '10'
    end
    object cbUseInitBfCtt: TCheckBox
      Tag = 6
      Left = 10
      Top = 409
      Width = 367
      Height = 24
      Caption = 'Use Before Contact Tilt Offset '
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 11
    end
    object edGraphMinY: TEdit
      Left = 271
      Top = 431
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 12
      Text = '10'
    end
    object edGraphMaxY: TEdit
      Left = 271
      Top = 455
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 13
      Text = '10'
    end
    object edDrvTtStrtCrnt: TEdit
      Left = 271
      Top = 40
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 14
      Text = '10'
    end
    object edLinearSttCrnt: TEdit
      Left = 271
      Top = 278
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 15
      Text = '10'
    end
    object edLinearEndCrnt: TEdit
      Left = 271
      Top = 302
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 16
      Text = '10'
    end
    object edHysteSttCrnt: TEdit
      Left = 271
      Top = 326
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 17
      Text = '10'
    end
    object edHysteEndCrnt: TEdit
      Left = 271
      Top = 350
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 18
      Text = '10'
    end
    object edPreHysteMaxOfst: TEdit
      Left = 271
      Top = 374
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 19
      Text = '10'
    end
  end
  object Panel9: TPanel
    Left = 0
    Top = 311
    Width = 467
    Height = 291
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object Label20: TLabel
      Left = 179
      Top = 41
      Width = 27
      Height = 19
      Caption = 'Min'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label21: TLabel
      Left = 309
      Top = 41
      Width = 31
      Height = 19
      Caption = 'Max'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label22: TLabel
      Left = 427
      Top = 68
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Label23: TLabel
      Left = 427
      Top = 94
      Width = 14
      Height = 14
      Caption = #956'm'
    end
    object Label24: TLabel
      Left = 427
      Top = 120
      Width = 14
      Height = 14
      Caption = #956'm'
    end
    object Label1: TLabel
      Left = 427
      Top = 146
      Width = 33
      Height = 14
      Caption = #956'm/mA'
    end
    object Label2: TLabel
      Left = 427
      Top = 196
      Width = 11
      Height = 14
      Caption = #937' '
    end
    object Label3: TLabel
      Left = 427
      Top = 222
      Width = 14
      Height = 14
      Caption = #956'm'
    end
    object um: TLabel
      Left = 426
      Top = 172
      Width = 14
      Height = 14
      Caption = #956'm'
    end
    object cbUsePreLoad: TCheckBox
      Tag = 6
      Left = 10
      Top = 60
      Width = 413
      Height = 24
      Caption = 'Starting Current'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 2
    end
    object Panel10: TPanel
      Left = 1
      Top = 1
      Width = 465
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'DRIVE OK/NG CONDITION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbUsePeakStroke: TCheckBox
      Tag = 1
      Left = 10
      Top = 86
      Width = 413
      Height = 24
      Caption = 'Max Stroke'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
    object cbUseCustomStroke: TCheckBox
      Tag = 1
      Left = 10
      Top = 112
      Width = 413
      Height = 24
      Caption = 'Rated Stroke'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 3
    end
    object edMinPreLoad: TEdit
      Left = 163
      Top = 61
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 4
      Text = '1000'
    end
    object edMaxPreLoad: TEdit
      Left = 292
      Top = 61
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 5
      Text = '1000'
    end
    object edMinPeakStroke: TEdit
      Left = 163
      Top = 87
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 6
      Text = '1000'
    end
    object edMaxPeakStroke: TEdit
      Left = 292
      Top = 87
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 7
      Text = '1000'
    end
    object edMinCustomStroke: TEdit
      Left = 163
      Top = 113
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 8
      Text = '1000'
    end
    object edMaxCustomStroke: TEdit
      Left = 292
      Top = 113
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 9
      Text = '1000'
    end
    object cbUseSensitivity: TCheckBox
      Tag = 6
      Left = 10
      Top = 138
      Width = 413
      Height = 24
      Caption = 'Sensitivity'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 10
    end
    object cbUseImpedance: TCheckBox
      Tag = 1
      Left = 10
      Top = 190
      Width = 413
      Height = 24
      Caption = 'Resistance'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 11
    end
    object cbUseHysteresis: TCheckBox
      Tag = 1
      Left = 10
      Top = 216
      Width = 413
      Height = 24
      Caption = 'Hysteresis'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 12
    end
    object edMinSensitivity: TEdit
      Left = 163
      Top = 139
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 13
      Text = '1000'
    end
    object edMaxSensitivity: TEdit
      Left = 292
      Top = 139
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 14
      Text = '1000'
    end
    object edMinImpedance: TEdit
      Left = 163
      Top = 191
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 15
      Text = '1000'
    end
    object edMaxImpedance: TEdit
      Left = 292
      Top = 191
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 16
      Text = '1000'
    end
    object edMinHysteresis: TEdit
      Left = 163
      Top = 217
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 17
      Text = '1000'
    end
    object edMaxHysteresis: TEdit
      Left = 292
      Top = 217
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 18
      Text = '1000'
    end
    object cbUseLinearity: TCheckBox
      Tag = 6
      Left = 10
      Top = 164
      Width = 413
      Height = 24
      Caption = 'Linearity'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 19
    end
    object edMinLinearity: TEdit
      Left = 163
      Top = 165
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 20
      Text = '1000'
    end
    object edMaxLinearity: TEdit
      Left = 292
      Top = 165
      Width = 129
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 21
      Text = '1000'
    end
    object cbUseCaseShort: TCheckBox
      Tag = 1
      Left = 10
      Top = 242
      Width = 413
      Height = 24
      Caption = 'Case Short'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 22
    end
  end
  object Panel1: TPanel
    Left = 467
    Top = 0
    Width = 431
    Height = 121
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object Label13: TLabel
      Left = 10
      Top = 96
      Width = 129
      Height = 19
      Caption = 'Max Test Current'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label14: TLabel
      Left = 393
      Top = 100
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Label38: TLabel
      Left = 10
      Top = 48
      Width = 140
      Height = 19
      Caption = 'Current Test Mode'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbTestCrntMin: TLabel
      Left = 10
      Top = 72
      Width = 125
      Height = 19
      Caption = 'Min Test Current'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object lbTestCrntMinUnit: TLabel
      Left = 393
      Top = 76
      Width = 16
      Height = 14
      Caption = 'mA'
    end
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 429
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'VCM MODE SETTING'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object edTestCrntMax: TEdit
      Left = 271
      Top = 96
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 1
      Text = '10'
    end
    object cbUseDuplex: TComboBox
      Left = 270
      Top = 48
      Width = 123
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 14
      ItemIndex = 0
      TabOrder = 2
      Text = 'Simplex Test'
      Items.Strings = (
        'Simplex Test'
        'Duplex Test')
    end
    object edTestCrntMin: TEdit
      Left = 271
      Top = 72
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 3
      Text = '0'
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 8
    Top = 8
  end
end
