object FrmOption: TFrmOption
  Left = 97
  Top = 113
  BorderStyle = bsNone
  Caption = 'FrmOption'
  ClientHeight = 863
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
    Width = 519
    Height = 593
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 223
      Top = 202
      Width = 19
      Height = 18
      Caption = 'EA'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 204
      Top = 249
      Width = 43
      Height = 18
      Caption = 'Count'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 10
      Top = 225
      Width = 236
      Height = 18
      AutoSize = False
      Caption = 'Dispenser Clean Count'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Label2: TLabel
      Left = 220
      Top = 305
      Width = 25
      Height = 18
      Caption = 'min'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label8: TLabel
      Left = 10
      Top = 281
      Width = 236
      Height = 18
      AutoSize = False
      Caption = 'Dispensor Stop To Shot TIme'
      Color = clScrollBar
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
    end
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 517
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'DEVICE SELECTION'
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
    object cbIgnrDispsr: TCheckBox
      Tag = 4
      Left = 10
      Top = 104
      Width = 238
      Height = 22
      Caption = 'Ignore Dispenser'
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
    object cbEmptyStrpCnt: TCheckBox
      Tag = 4
      Left = 10
      Top = 176
      Width = 238
      Height = 22
      Caption = 'Used Loader Empty Count'
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
    object edEmptyStrpCnt: TEdit
      Left = 139
      Top = 200
      Width = 81
      Height = 22
      ImeName = 'Microsoft IME 2010'
      TabOrder = 5
      Text = '0'
    end
    object cbSkipDspsSnsr: TCheckBox
      Tag = 4
      Left = 10
      Top = 128
      Width = 238
      Height = 22
      Caption = 'Ignore Dispensor Sensor'
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
    object GroupBox1: TGroupBox
      Left = 265
      Top = 54
      Width = 241
      Height = 134
      Caption = 'LIQUID SETTING'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      object Label3: TLabel
        Left = 7
        Top = 54
        Width = 106
        Height = 19
        Caption = 'SETTING DAY'
        Color = clScrollBar
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
      end
      object Label4: TLabel
        Left = 204
        Top = 28
        Width = 29
        Height = 18
        Caption = 'DAY'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label7: TLabel
        Left = 7
        Top = 25
        Width = 105
        Height = 19
        AutoSize = False
        Caption = 'EXPIRY DAY'
        Color = clScrollBar
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
      end
      object edExpiryDay: TEdit
        Left = 124
        Top = 23
        Width = 77
        Height = 27
        ImeName = 'Microsoft IME 2010'
        TabOrder = 0
      end
      object btLiquidReset: TButton
        Left = 107
        Top = 82
        Width = 126
        Height = 43
        Caption = 'Reset'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = btLiquidResetClick
      end
      object edSettingDay: TEdit
        Left = 124
        Top = 51
        Width = 110
        Height = 27
        Color = cl3DLight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
        ParentFont = False
        TabOrder = 2
      end
    end
    object cbIngrDsprExpr: TCheckBox
      Tag = 4
      Left = 10
      Top = 151
      Width = 238
      Height = 22
      Caption = 'Ignore Dispensor Expiry'
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
    object edDisprCleanCnt: TEdit
      Left = 120
      Top = 249
      Width = 81
      Height = 22
      ImeName = 'Microsoft IME 2010'
      TabOrder = 9
      Text = '0'
    end
    object GroupBox2: TGroupBox
      Left = 17
      Top = 366
      Width = 480
      Height = 84
      Caption = 'SERVER FOLDER PATH'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      object edServerPath: TEdit
        Left = 13
        Top = 39
        Width = 401
        Height = 23
        AutoSize = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = 'Microsoft Office IME 2007'
        ParentFont = False
        TabOrder = 0
        Text = '1'
      end
      object btServerFolder: TButton
        Left = 429
        Top = 37
        Width = 36
        Height = 25
        Caption = '...'
        TabOrder = 1
        OnClick = btServerFolderClick
      end
    end
    object edStopToShotTime: TEdit
      Left = 137
      Top = 305
      Width = 81
      Height = 22
      ImeName = 'Microsoft IME 2010'
      TabOrder = 11
      Text = '0'
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
  object odLoadOnlyOneMapPath: TOpenDialog
    DefaultExt = 'JobFile'
    FileName = 'zzzz'
    Filter = #47784#46304' '#54028#51068' '#54805#49885'|*.*'
    Left = 1176
  end
end