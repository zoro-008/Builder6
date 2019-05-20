object FrmMaster: TFrmMaster
  Left = 151
  Top = 143
  AutoScroll = False
  Caption = 'FrmMaster'
  ClientHeight = 618
  ClientWidth = 876
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
    Width = 876
    Height = 618
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 192
      Width = 122
      Height = 13
      Caption = 'Mark Vision Trigger Offset'
    end
    object Label2: TLabel
      Left = 24
      Top = 492
      Width = 87
      Height = 13
      Caption = 'Mark Visn Rslt File'
    end
    object Label3: TLabel
      Left = 24
      Top = 555
      Width = 99
      Height = 13
      Caption = 'Laser Send Data File'
    end
    object Label4: TLabel
      Left = 416
      Top = 579
      Width = 144
      Height = 13
      Caption = 'Control\\DATA\\LaserData.ini'
    end
    object Label5: TLabel
      Left = 408
      Top = 492
      Width = 140
      Height = 13
      Caption = 'Mark Visn DeviceChange File'
    end
    object Label8: TLabel
      Left = 16
      Top = 376
      Width = 67
      Height = 13
      Caption = 'Laser X Offset'
    end
    object Label9: TLabel
      Left = 16
      Top = 400
      Width = 67
      Height = 13
      Caption = 'Laser Y Offset'
    end
    object Label10: TLabel
      Left = 16
      Top = 424
      Width = 67
      Height = 13
      Caption = 'Laser T Offset'
    end
    object lbRsvMsg: TLabel
      Left = 16
      Top = 304
      Width = 150
      Height = 13
      Caption = 'Vision && Laser Y Center Position'
    end
    object Panel5: TPanel
      Left = 0
      Top = 0
      Width = 876
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      Caption = 'MASTER OPTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object cbDebugMode: TCheckBox
      Left = 16
      Top = 48
      Width = 185
      Height = 33
      Caption = 'Debug Mode Use'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object btSaveMotor: TBitBtn
      Left = 583
      Top = 547
      Width = 281
      Height = 61
      Caption = 'SAVE '
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
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
    object GroupBox8: TGroupBox
      Left = 240
      Top = 51
      Width = 625
      Height = 438
      Caption = 'Hello World'
      TabOrder = 3
      object lbMan: TLabel
        Left = 224
        Top = 420
        Width = 29
        Height = 13
        Caption = 'lbMan'
      end
      object sgCycleStep: TStringGrid
        Left = 224
        Top = 16
        Width = 394
        Height = 390
        ColCount = 6
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
      object cb1: TCheckBox
        Left = 11
        Top = 19
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object cb2: TCheckBox
        Left = 11
        Top = 49
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object cb4: TCheckBox
        Left = 11
        Top = 111
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object cb6: TCheckBox
        Left = 11
        Top = 173
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object btPartAutorun: TButton
        Left = 115
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
        TabOrder = 5
        OnClick = btPartAutorunClick
      end
      object btPartReset: TButton
        Left = 115
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
        TabOrder = 6
        OnClick = btPartResetClick
      end
      object btAllReset: TButton
        Left = 115
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
        TabOrder = 7
        OnClick = btAllResetClick
      end
      object cb5: TCheckBox
        Left = 11
        Top = 142
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object cb7: TCheckBox
        Left = 11
        Top = 204
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
      object cb3: TCheckBox
        Left = 11
        Top = 80
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object cb8: TCheckBox
        Left = 11
        Top = 235
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        Visible = False
      end
      object cb9: TCheckBox
        Left = 11
        Top = 265
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
        Visible = False
      end
      object cb11: TCheckBox
        Left = 11
        Top = 327
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
        Visible = False
      end
      object cb13: TCheckBox
        Left = 11
        Top = 389
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
        Visible = False
      end
      object cb12: TCheckBox
        Left = 11
        Top = 358
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
        Visible = False
      end
      object cb10: TCheckBox
        Left = 11
        Top = 296
        Width = 89
        Height = 30
        Caption = 'PART'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
        Visible = False
      end
      object btPartHome: TButton
        Left = 115
        Top = 352
        Width = 106
        Height = 49
        Caption = 'Part Home'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
        OnClick = btPartHomeClick
      end
    end
    object cbLdr_YHome: TCheckBox
      Left = 16
      Top = 82
      Width = 116
      Height = 33
      Caption = 'Ldr_YHome'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
    object cbDryRun: TCheckBox
      Left = 16
      Top = 117
      Width = 132
      Height = 33
      Caption = 'Dry Run'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object edTrigerOffset: TEdit
      Left = 16
      Top = 208
      Width = 121
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 6
      Text = '0.0'
    end
    object edVisnRsltFile: TEdit
      Left = 24
      Top = 515
      Width = 345
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 7
      Text = '0.0'
    end
    object btVisnRsltFile: TButton
      Left = 368
      Top = 515
      Width = 25
      Height = 25
      Caption = '...'
      TabOrder = 8
      OnClick = btVisnRsltFileClick
    end
    object edLaserDataFile: TEdit
      Left = 24
      Top = 579
      Width = 345
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 9
      Text = '0.0'
    end
    object Button1: TButton
      Left = 368
      Top = 579
      Width = 25
      Height = 25
      Caption = '...'
      TabOrder = 10
      OnClick = Button1Click
    end
    object Button3: TButton
      Left = 784
      Top = 515
      Width = 75
      Height = 25
      Caption = 'OneShotTrigger'
      TabOrder = 11
      OnClick = Button3Click
    end
    object edVisnDeviceFile: TEdit
      Left = 408
      Top = 515
      Width = 345
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 12
      Text = '0.0'
    end
    object Button2: TButton
      Left = 752
      Top = 515
      Width = 25
      Height = 25
      Caption = '...'
      TabOrder = 13
      OnClick = Button2Click
    end
    object LaserReset: TButton
      Left = 64
      Top = 232
      Width = 75
      Height = 25
      Caption = 'LaserReset'
      TabOrder = 14
    end
    object cbAllStrpSendLsrJob: TCheckBox
      Left = 16
      Top = 152
      Width = 201
      Height = 33
      Caption = 'Send All Strip Lsr JobFile'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 15
    end
    object edLSRSendMsg: TEdit
      Left = 16
      Top = 342
      Width = 121
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 16
      Text = '0.0'
    end
    object edLsrXOfs: TEdit
      Left = 102
      Top = 372
      Width = 121
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 17
      Text = '0.0'
    end
    object edLsrYOfs: TEdit
      Left = 102
      Top = 396
      Width = 121
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 18
      Text = '0.0'
    end
    object edLsrTOfs: TEdit
      Left = 102
      Top = 420
      Width = 121
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 19
      Text = '0.0'
    end
    object Button5: TButton
      Left = 8
      Top = 448
      Width = 75
      Height = 25
      Caption = 'SendMsg'
      TabOrder = 20
      OnClick = Button5Click
    end
    object Button6: TButton
      Left = 88
      Top = 448
      Width = 75
      Height = 25
      Caption = 'LSR_START'
      TabOrder = 21
      OnClick = Button6Click
    end
    object Button7: TButton
      Left = 168
      Top = 448
      Width = 75
      Height = 25
      Caption = 'LSR_MARK'
      TabOrder = 22
      Visible = False
      OnClick = Button7Click
    end
    object Button8: TButton
      Left = 144
      Top = 334
      Width = 75
      Height = 25
      Caption = 'LaserReset'
      TabOrder = 23
      OnClick = Button8Click
    end
    object cbMsgCmd: TComboBox
      Left = 15
      Top = 318
      Width = 145
      Height = 21
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 24
      Text = 'DAT'
      Items.Strings = (
        'DAT'
        'VAD'
        'CHM'
        'RDY')
    end
    object edVsnLsrYCntrPos: TEdit
      Left = 16
      Top = 264
      Width = 121
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 25
      Text = '0.0'
    end
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
