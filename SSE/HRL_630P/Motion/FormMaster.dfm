object FrmMaster: TFrmMaster
  Left = 529
  Top = 211
  Width = 742
  Height = 571
  Caption = 'FrmMaster'
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 726
    Height = 533
    Align = alClient
    TabOrder = 0
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 724
      Height = 531
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 724
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
      object GroupBox8: TGroupBox
        Left = 4
        Top = 39
        Width = 981
        Height = 486
        Caption = 'Hello World'
        TabOrder = 1
        object sgCycleStep: TStringGrid
          Left = 120
          Top = 16
          Width = 446
          Height = 321
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
        object btPartAutorun: TButton
          Left = 9
          Top = 233
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
          Left = 9
          Top = 180
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
          Left = 9
          Top = 127
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
        object btSaveMotor: TBitBtn
          Left = 487
          Top = 341
          Width = 226
          Height = 140
          Caption = 'SAVE '
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -32
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
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
        object cbDebugMode: TCheckBox
          Left = 11
          Top = 384
          Width = 148
          Height = 33
          Caption = 'Debug Mode Use'
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object cbDryRun: TCheckBox
          Left = 11
          Top = 352
          Width = 80
          Height = 33
          Caption = 'Dry Run'
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object edModelName: TEdit
          Left = 0
          Top = 453
          Width = 155
          Height = 26
          AutoSize = False
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
        end
        object StaticText3: TStaticText
          Left = 0
          Top = 432
          Width = 91
          Height = 20
          AutoSize = False
          BevelKind = bkFlat
          Caption = 'Equip Name'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object btPartHome: TButton
          Left = 9
          Top = 287
          Width = 106
          Height = 49
          Caption = 'Part Home'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
          OnClick = btPartHomeClick
        end
        object sgTime: TStringGrid
          Left = 568
          Top = 40
          Width = 148
          Height = 297
          ColCount = 2
          RowCount = 7
          FixedRows = 0
          TabOrder = 10
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
          Left = 568
          Top = 16
          Width = 150
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          ItemHeight = 13
          TabOrder = 11
          Text = 'cbPartSel'
          OnChange = cbPartSelChange
        end
        object edMtrOfset: TEdit
          Left = 318
          Top = 453
          Width = 155
          Height = 26
          AutoSize = False
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 12
        end
        object StaticText1: TStaticText
          Left = 318
          Top = 432
          Width = 89
          Height = 20
          AutoSize = False
          BevelKind = bkFlat
          Caption = 'Motor Offset'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
        end
        object StaticText2: TStaticText
          Left = 158
          Top = 432
          Width = 112
          Height = 20
          AutoSize = False
          BevelKind = bkFlat
          Caption = 'Package Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 14
        end
        object edPkgCnt: TEdit
          Left = 158
          Top = 453
          Width = 155
          Height = 26
          AutoSize = False
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 15
        end
        object GroupBox1: TGroupBox
          Left = 304
          Top = 342
          Width = 174
          Height = 79
          Caption = 'LASER COMMUNICATION'
          TabOrder = 16
          object edGAMsg: TEdit
            Left = 8
            Top = 20
            Width = 81
            Height = 21
            ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
            TabOrder = 0
            Text = 'GA,'
          end
          object btSend: TButton
            Left = 94
            Top = 16
            Width = 75
            Height = 25
            Caption = 'SEND'
            TabOrder = 1
            OnClick = btSendClick
          end
          object Button5: TButton
            Left = 94
            Top = 48
            Width = 75
            Height = 25
            Caption = 'Check Sum'
            TabOrder = 2
            OnClick = Button5Click
          end
        end
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tmUpdateTimer
    Left = 80
  end
end