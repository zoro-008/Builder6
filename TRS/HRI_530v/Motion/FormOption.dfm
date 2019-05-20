object FrmOption: TFrmOption
  Left = 433
  Top = 110
  BorderStyle = bsNone
  ClientHeight = 862
  ClientWidth = 1272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel3: TPanel
    Left = 0
    Top = 0
    Width = 255
    Height = 337
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
      Width = 253
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'SKIP OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbIgnrDoor: TCheckBox
      Tag = 1
      Left = 10
      Top = 80
      Width = 238
      Height = 22
      Caption = 'Ignore Door Open'
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
    object cbLoadingStop: TCheckBox
      Tag = 6
      Left = 10
      Top = 56
      Width = 238
      Height = 22
      Caption = 'Loading Stop'
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
    object cbSkipVT: TCheckBox
      Tag = 4
      Left = 9
      Top = 128
      Width = 238
      Height = 22
      Caption = 'Skip VT'
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
    object cbSkipVSN1: TCheckBox
      Tag = 4
      Left = 9
      Top = 152
      Width = 238
      Height = 22
      Caption = 'Skip Vision1'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 4
    end
    object cbSkipOST: TCheckBox
      Tag = 1
      Left = 9
      Top = 104
      Width = 238
      Height = 22
      Caption = 'Skip OST'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 5
    end
    object cbSkipVSN2: TCheckBox
      Tag = 4
      Left = 9
      Top = 176
      Width = 238
      Height = 22
      Caption = 'Skip Vision2'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 6
    end
    object cbOSTNotUseCrnt: TCheckBox
      Tag = 4
      Left = 9
      Top = 216
      Width = 238
      Height = 22
      Caption = 'Not Use Current Result'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 7
    end
    object cbOSTNotUseOst: TCheckBox
      Tag = 4
      Left = 9
      Top = 240
      Width = 238
      Height = 22
      Caption = 'Not Use OST Result'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 8
    end
  end
  object btSave: TBitBtn
    Left = 944
    Top = 760
    Width = 305
    Height = 73
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
  object Panel4: TPanel
    Left = -1
    Top = 336
    Width = 255
    Height = 337
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object Label41: TLabel
      Left = 4
      Top = 160
      Width = 184
      Height = 18
      Caption = '* Lot'#51652#54665#51473' '#49828#53429#51012' '#54616#47140#47732
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label17: TLabel
      Left = 1
      Top = 178
      Width = 220
      Height = 18
      Caption = '   '#49828#53580#51060#51648#51032' '#54644#45817' '#54589#52964' '#51088#51228#47484' '
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label18: TLabel
      Left = 12
      Top = 196
      Width = 84
      Height = 18
      Caption = #51228#44144' '#54616#49464#50836
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Panel6: TPanel
      Left = 1
      Top = 1
      Width = 253
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'TOOL OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbSkip2ndTol: TCheckBox
      Tag = 1
      Left = 10
      Top = 80
      Width = 238
      Height = 22
      Caption = 'Skip 2nd Tool'
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
    object cbSkip1stTol: TCheckBox
      Tag = 6
      Left = 10
      Top = 56
      Width = 238
      Height = 22
      Caption = 'Skip 1st Tool'
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
    object cbSkip4thTol: TCheckBox
      Tag = 4
      Left = 10
      Top = 128
      Width = 238
      Height = 22
      Caption = 'Skip 4th Tool'
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
    object cbSkip3thTol: TCheckBox
      Tag = 1
      Left = 10
      Top = 104
      Width = 238
      Height = 22
      Caption = 'Skip 3th Tool'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 4
    end
  end
  object Panel7: TPanel
    Left = 255
    Top = 0
    Width = 506
    Height = 337
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object Label7: TLabel
      Left = 5
      Top = 56
      Width = 176
      Height = 19
      Caption = 'Before VT Power Delay'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label8: TLabel
      Left = 378
      Top = 60
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Label9: TLabel
      Left = 5
      Top = 80
      Width = 160
      Height = 19
      Caption = 'Before VT Insp Delay'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label10: TLabel
      Left = 378
      Top = 84
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Label11: TLabel
      Left = 5
      Top = 120
      Width = 222
      Height = 19
      Caption = 'Continuous BLK Error Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label12: TLabel
      Left = 378
      Top = 124
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Label23: TLabel
      Left = 5
      Top = 144
      Width = 237
      Height = 19
      Caption = 'Continuous Defect Error Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label24: TLabel
      Left = 378
      Top = 148
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Label25: TLabel
      Left = 5
      Top = 192
      Width = 211
      Height = 19
      Caption = 'Continuous VH Error Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label26: TLabel
      Left = 378
      Top = 196
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Label27: TLabel
      Left = 5
      Top = 168
      Width = 246
      Height = 19
      Caption = 'Continuous Defect2 Error Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label28: TLabel
      Left = 326
      Top = 172
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Label29: TLabel
      Left = 378
      Top = 172
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Panel8: TPanel
      Left = 1
      Top = 1
      Width = 504
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'VT OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object edBfrVTPowrDelay: TEdit
      Left = 256
      Top = 56
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 1
      Text = '1000'
    end
    object edBfrVTInspDelay: TEdit
      Left = 256
      Top = 80
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 2
      Text = '1000'
    end
    object edVTContBlkCnt: TEdit
      Left = 256
      Top = 120
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 3
      Text = '10'
    end
    object edVTContDftCnt: TEdit
      Left = 256
      Top = 144
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 4
      Text = '10'
    end
    object edVTContVHCnt: TEdit
      Left = 256
      Top = 192
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 5
      Text = '10'
    end
    object edVTContDft2Cnt: TEdit
      Left = 257
      Top = 168
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 6
      Text = '10'
    end
  end
  object Panel9: TPanel
    Left = 254
    Top = 336
    Width = 507
    Height = 337
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    object Label13: TLabel
      Left = 5
      Top = 120
      Width = 267
      Height = 19
      Caption = 'Continuous Vision Fail Error Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label14: TLabel
      Left = 405
      Top = 124
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Panel10: TPanel
      Left = 1
      Top = 1
      Width = 505
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'VISION OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbVisnAutoConv: TCheckBox
      Tag = 6
      Left = 10
      Top = 56
      Width = 238
      Height = 22
      Caption = 'Vision Auto Conversion'
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
    object edVSContNGCnt: TEdit
      Left = 280
      Top = 120
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 2
      Text = '10'
    end
  end
  object Panel11: TPanel
    Left = 761
    Top = 0
    Width = 510
    Height = 337
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    object Label19: TLabel
      Left = 5
      Top = 48
      Width = 221
      Height = 19
      Caption = 'Continuous OST Error Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label20: TLabel
      Left = 362
      Top = 52
      Width = 12
      Height = 14
      Caption = 'ea'
    end
    object Label21: TLabel
      Left = 5
      Top = 80
      Width = 187
      Height = 19
      Caption = 'OST LOG DELETE DAYS'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label22: TLabel
      Left = 362
      Top = 84
      Width = 24
      Height = 14
      Caption = 'days'
    end
    object Panel12: TPanel
      Left = 1
      Top = 1
      Width = 508
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'OST OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object edOSContNGCnt: TEdit
      Left = 240
      Top = 48
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 1
      Text = '10'
    end
    object edOSTLogDays: TEdit
      Left = 240
      Top = 80
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 2
      Text = '10'
    end
  end
  object pnRejOpotn: TPanel
    Left = 256
    Top = 528
    Width = 255
    Height = 337
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Visible = False
    object Label1: TLabel
      Left = 5
      Top = 56
      Width = 102
      Height = 19
      Caption = 'OST OST BIN'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 5
      Top = 130
      Width = 66
      Height = 19
      Caption = 'DEFECT'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 5
      Top = 195
      Width = 29
      Height = 19
      Caption = 'DIM'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 5
      Top = 231
      Width = 34
      Height = 19
      Caption = 'BLK'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 5
      Top = 267
      Width = 23
      Height = 19
      Caption = 'VH'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 5
      Top = 304
      Width = 54
      Height = 19
      Caption = 'VISION'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label51: TLabel
      Left = 5
      Top = 93
      Width = 115
      Height = 19
      Caption = 'OST CRNT BIN'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label52: TLabel
      Left = 7
      Top = 162
      Width = 75
      Height = 19
      Caption = 'DEFECT2'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 253
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'REJECT  OPTION('#44288#44228#51088#50808#51312#51089#44552#51648')'
      Color = clHotLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbFailIOST: TComboBox
      Left = 123
      Top = 56
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 1
      Text = 'cbFailIOST'
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
    object cbFailIDFT: TComboBox
      Left = 123
      Top = 130
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 2
      Text = 'ComboBox1'
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
    object cbFailIDIM: TComboBox
      Left = 123
      Top = 195
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 3
      Text = 'ComboBox1'
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
    object cbFailIBLK: TComboBox
      Left = 123
      Top = 231
      Width = 127
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
      Text = 'ComboBox1'
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
    object cbFailIVH: TComboBox
      Left = 123
      Top = 267
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 5
      Text = 'ComboBox1'
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
    object cbFailIVSN: TComboBox
      Left = 123
      Top = 304
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 6
      Text = 'ComboBox1'
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
    object cbFailICrnt: TComboBox
      Left = 123
      Top = 93
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 7
      Text = 'cbFailIOST'
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
    object cbFailIDFT2: TComboBox
      Left = 122
      Top = 162
      Width = 127
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 16
      ParentFont = False
      TabOrder = 8
      Text = 'cbFailIDFT2'
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
  end
  object Panel13: TPanel
    Left = 761
    Top = 336
    Width = 510
    Height = 337
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    object Label15: TLabel
      Left = 5
      Top = 48
      Width = 170
      Height = 19
      Caption = 'Stage After Stop Delay'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label16: TLabel
      Left = 362
      Top = 52
      Width = 14
      Height = 14
      Caption = 'ms'
    end
    object Panel14: TPanel
      Left = 1
      Top = 1
      Width = 508
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'SETTING OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object edStgAftStopDelay: TEdit
      Left = 240
      Top = 48
      Width = 121
      Height = 22
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 1
      Text = '10'
    end
  end
  object btRejectOptn: TButton
    Left = 0
    Top = 672
    Width = 17
    Height = 17
    TabOrder = 8
    OnClick = btRejectOptnClick
  end
end
