object FrmMaster: TFrmMaster
  Left = 494
  Top = 143
  AutoScroll = False
  Caption = 'FrmMaster'
  ClientHeight = 514
  ClientWidth = 1156
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
    Width = 1156
    Height = 514
    ActivePage = TabSheet6
    Align = alClient
    TabIndex = 5
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Master Main'
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 1148
        Height = 486
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object Panel5: TPanel
          Left = 0
          Top = 0
          Width = 1148
          Height = 28
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
          Left = 16
          Top = 43
          Width = 1033
          Height = 398
          Caption = 'Master Grop'
          TabOrder = 1
          object lbMan: TLabel
            Left = 32
            Top = 366
            Width = 37
            Height = 16
            Caption = 'lbMan'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object sgCycleStep: TStringGrid
            Left = 270
            Top = 16
            Width = 569
            Height = 337
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
            Left = 155
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
            Left = 155
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
            Left = 155
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
            Left = 155
            Top = 304
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
            Left = 152
            Top = 184
            Width = 105
            Height = 25
            Caption = 'ALL CHECK'
            TabOrder = 5
            OnClick = btAllCheckClick
          end
          object cbPartSel: TComboBox
            Left = 848
            Top = 8
            Width = 169
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            TabOrder = 6
            Text = 'cbPartSel'
            OnChange = cbPartSelChange
          end
          object sgTime: TStringGrid
            Left = 848
            Top = 37
            Width = 169
            Height = 337
            ColCount = 2
            RowCount = 7
            FixedRows = 0
            TabOrder = 7
            RowHeights = (
              24
              26
              24
              24
              24
              24
              24)
          end
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Option'
      ImageIndex = 2
      object Label2: TLabel
        Left = 0
        Top = 6
        Width = 86
        Height = 13
        Caption = 'Vision Result File1'
      end
      object Label1: TLabel
        Left = 16
        Top = 88
        Width = 218
        Height = 20
        Caption = 'LeftTool - RightTool Stroke'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label13: TLabel
        Left = 16
        Top = 120
        Width = 276
        Height = 20
        Caption = 'LeftTool Vision - Dispenser Stroke'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label14: TLabel
        Left = 16
        Top = 152
        Width = 263
        Height = 20
        Caption = 'RightTool Transfer-Vision Stroke'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label15: TLabel
        Left = 437
        Top = 88
        Width = 29
        Height = 20
        Caption = 'mm'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label17: TLabel
        Left = 437
        Top = 120
        Width = 29
        Height = 20
        Caption = 'mm'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label18: TLabel
        Left = 437
        Top = 152
        Width = 29
        Height = 20
        Caption = 'mm'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object cbDebugMode: TCheckBox
        Left = 16
        Top = 45
        Width = 185
        Height = 33
        Caption = 'Debug Mode Use'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object btSaveMotor: TBitBtn
        Left = 672
        Top = 360
        Width = 233
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
      object edLTL_RTL_Stroke: TEdit
        Left = 312
        Top = 88
        Width = 121
        Height = 21
        ImeName = 'Microsoft IME 2010'
        TabOrder = 2
        Text = 'edLTL_RTL_Stroke'
      end
      object edLTL_VISN_DISP_Strke: TEdit
        Left = 312
        Top = 120
        Width = 121
        Height = 21
        ImeName = 'Microsoft IME 2010'
        TabOrder = 3
        Text = 'edLTL_RTL_Stroke'
      end
      object edRTL_TRNS_VISN_Strke: TEdit
        Left = 312
        Top = 152
        Width = 121
        Height = 21
        ImeName = 'Microsoft IME 2010'
        TabOrder = 4
        Text = 'edLTL_RTL_Stroke'
      end
      object cbDonUseMasterT: TCheckBox
        Left = 16
        Top = 181
        Width = 185
        Height = 33
        Caption = 'Dont Use Master T'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object cbUseLotInfo: TCheckBox
        Left = 16
        Top = 207
        Width = 185
        Height = 33
        Caption = 'Use Lot Info'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Dispensor && HeightSensor'
      ImageIndex = 2
      object GroupBox9: TGroupBox
        Left = 6
        Top = 13
        Width = 555
        Height = 465
        Caption = 'Dispensor'
        TabOrder = 0
        object Label3: TLabel
          Left = 29
          Top = 34
          Width = 68
          Height = 13
          Caption = 'Error Message'
        end
        object Label4: TLabel
          Left = 256
          Top = 34
          Width = 97
          Height = 13
          Caption = 'Message End Signal'
        end
        object lbDisprMsgEnd: TLabel
          Left = 367
          Top = 34
          Width = 18
          Height = 13
          Caption = 'true'
        end
        object Label35: TLabel
          Left = 29
          Top = 58
          Width = 80
          Height = 13
          Caption = 'Dispensor Select'
        end
        object GroupBox1: TGroupBox
          Left = 24
          Top = 88
          Width = 113
          Height = 105
          Caption = 'On Off'
          TabOrder = 0
          object btDisprOn: TButton
            Left = 16
            Top = 28
            Width = 75
            Height = 25
            Caption = 'On'
            TabOrder = 0
            OnClick = btDisprOnClick
          end
          object btDisprOff: TButton
            Left = 16
            Top = 60
            Width = 75
            Height = 25
            Caption = 'Off'
            TabOrder = 1
            OnClick = btDisprOnClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 280
          Top = 88
          Width = 121
          Height = 105
          Caption = 'Mode Setting'
          TabOrder = 1
          object cbSetMode: TComboBox
            Left = 8
            Top = 32
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 0
            Text = 'Manual'
            Items.Strings = (
              'Manual'
              'Timed')
          end
          object btDisprSetMode: TButton
            Left = 32
            Top = 64
            Width = 73
            Height = 25
            Caption = 'SET'
            TabOrder = 1
            OnClick = btDisprOnClick
          end
        end
        object GroupBox3: TGroupBox
          Left = 152
          Top = 88
          Width = 113
          Height = 105
          Caption = 'Sigma Test Mode'
          TabOrder = 2
          object btDisprSigmaTest: TButton
            Left = 16
            Top = 40
            Width = 75
            Height = 25
            Caption = 'Test '
            TabOrder = 0
            OnClick = btDisprOnClick
          end
        end
        object GroupBox4: TGroupBox
          Left = 24
          Top = 208
          Width = 113
          Height = 105
          Caption = 'Load Channel'
          TabOrder = 3
          object btDisprLoadCh: TButton
            Left = 32
            Top = 64
            Width = 73
            Height = 25
            Caption = 'SET'
            TabOrder = 0
            OnClick = btDisprOnClick
          end
          object cbLoadCh: TComboBox
            Left = 8
            Top = 32
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 1
            Text = '0Ch'
            Items.Strings = (
              '0Ch'
              '1Ch'
              '2Ch'
              '3Ch'
              '4Ch')
          end
        end
        object GroupBox5: TGroupBox
          Left = 152
          Top = 208
          Width = 257
          Height = 105
          Caption = 'Pres Time Vaccum Setting'
          TabOrder = 4
          object Label5: TLabel
            Left = 23
            Top = 22
            Width = 26
            Height = 13
            Caption = 'Press'
          end
          object Label6: TLabel
            Left = 103
            Top = 22
            Width = 23
            Height = 13
            Caption = 'Time'
          end
          object Label7: TLabel
            Left = 165
            Top = 22
            Width = 39
            Height = 13
            Caption = 'Vaccum'
          end
          object edPres: TEdit
            Left = 8
            Top = 40
            Width = 65
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            Text = '0'
          end
          object edTime: TEdit
            Left = 80
            Top = 40
            Width = 65
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 1
            Text = '0'
          end
          object edVacc: TEdit
            Left = 152
            Top = 40
            Width = 65
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            Text = '0'
          end
          object btDisprSetPTV: TButton
            Left = 152
            Top = 64
            Width = 73
            Height = 25
            Caption = 'SET'
            TabOrder = 3
            OnClick = btDisprOnClick
          end
        end
        object GroupBox6: TGroupBox
          Left = 416
          Top = 88
          Width = 113
          Height = 105
          Caption = 'Syringe Size Setting'
          TabOrder = 5
          object btDisprSyrngSize: TButton
            Left = 32
            Top = 64
            Width = 73
            Height = 25
            Caption = 'SET'
            TabOrder = 0
            OnClick = btDisprOnClick
          end
          object cbSyrgSize: TComboBox
            Left = 8
            Top = 32
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 1
            Text = '3CC'
            Items.Strings = (
              '3CC'
              '5CC'
              '10CC'
              '20CC'
              '30CC'
              '50CC'
              '70CC')
          end
        end
        object edDisprErrMsg: TEdit
          Left = 116
          Top = 32
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
          Text = 'edDisprErrMsg'
        end
        object cbDisprSelect: TComboBox
          Left = 116
          Top = 56
          Width = 97
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          ItemHeight = 13
          TabOrder = 7
          Text = 'Manual'
          Items.Strings = (
            'Front Dispensor'
            'Rear  Dispensor')
        end
      end
      object GroupBox10: TGroupBox
        Left = 574
        Top = 13
        Width = 555
        Height = 196
        Caption = 'HeightSensor'
        TabOrder = 1
        object Label9: TLabel
          Left = 24
          Top = 29
          Width = 57
          Height = 13
          Caption = 'Height Data'
        end
        object Label8: TLabel
          Left = 370
          Top = 31
          Width = 97
          Height = 13
          Caption = 'Message End Signal'
        end
        object lbHghtMsgEnd: TLabel
          Left = 481
          Top = 31
          Width = 18
          Height = 13
          Caption = 'true'
        end
        object Label20: TLabel
          Left = 168
          Top = 29
          Width = 57
          Height = 13
          Caption = 'Height Data'
        end
        object Label21: TLabel
          Left = 24
          Top = 53
          Width = 54
          Height = 13
          Caption = 'Height Msg'
        end
        object GroupBox11: TGroupBox
          Left = 24
          Top = 72
          Width = 113
          Height = 105
          Caption = 'Set Zero'
          TabOrder = 0
          object btHghtSetZero: TButton
            Left = 19
            Top = 63
            Width = 75
            Height = 25
            Caption = 'SET'
            TabOrder = 0
            OnClick = btHghtSetZeroClick
          end
          object cbSetZeroCh: TComboBox
            Left = 8
            Top = 27
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 1
            Text = '0Ch && 1Ch'
            Items.Strings = (
              '0Ch && 1Ch'
              '0Ch'
              '1Ch')
          end
        end
        object GroupBox12: TGroupBox
          Left = 152
          Top = 72
          Width = 113
          Height = 105
          Caption = 'Check Height'
          TabOrder = 1
          object btHghtCheck: TButton
            Left = 18
            Top = 63
            Width = 75
            Height = 25
            Caption = 'SET'
            TabOrder = 0
            OnClick = btHghtSetZeroClick
          end
          object cbChkHgtCh: TComboBox
            Left = 8
            Top = 27
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 1
            Text = '0Ch && 1Ch'
            Items.Strings = (
              '0Ch && 1Ch'
              '0Ch'
              '1Ch'
              '')
          end
        end
        object GroupBox13: TGroupBox
          Left = 280
          Top = 72
          Width = 113
          Height = 105
          Caption = 'Get Height Data'
          TabOrder = 2
          object btHghtGetDat: TButton
            Left = 18
            Top = 63
            Width = 75
            Height = 25
            Caption = 'SET'
            TabOrder = 0
            OnClick = btHghtSetZeroClick
          end
          object cbGetHghDataCh: TComboBox
            Left = 8
            Top = 27
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 1
            Text = '0Ch'
            Items.Strings = (
              '0Ch'
              '1Ch')
          end
        end
        object edHghDataCh0: TEdit
          Left = 88
          Top = 27
          Width = 65
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
          Text = '0'
        end
        object edHghDataCh1: TEdit
          Left = 232
          Top = 27
          Width = 65
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 4
          Text = '0'
        end
        object edHghMsg: TEdit
          Left = 88
          Top = 51
          Width = 209
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 5
          Text = '0'
        end
      end
      object GroupBox21: TGroupBox
        Left = 574
        Top = 221
        Width = 555
        Height = 196
        Caption = 'Confocal'
        TabOrder = 2
        object btConfocalRezeroOn: TButton
          Left = 35
          Top = 31
          Width = 75
          Height = 25
          Caption = 'Rezero On'
          TabOrder = 0
          OnClick = btConfocalRezeroOnClick
        end
        object Button8: TButton
          Left = 123
          Top = 31
          Width = 75
          Height = 25
          Caption = 'Rezero Off'
          TabOrder = 1
          OnClick = Button8Click
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'LoadCell && UVCuring'
      ImageIndex = 3
      object GroupBox7: TGroupBox
        Left = 6
        Top = 13
        Width = 555
        Height = 465
        Caption = 'LoadCell'
        TabOrder = 0
        object Label11: TLabel
          Left = 241
          Top = 35
          Width = 97
          Height = 13
          Caption = 'Message End Signal'
        end
        object lbLoadClMsgEnd: TLabel
          Left = 352
          Top = 35
          Width = 18
          Height = 13
          Caption = 'true'
        end
        object Label10: TLabel
          Left = 29
          Top = 34
          Width = 67
          Height = 13
          Caption = 'LoadCell Data'
        end
        object Label19: TLabel
          Left = 29
          Top = 58
          Width = 64
          Height = 13
          Caption = 'LoadCell Msg'
        end
        object GroupBox14: TGroupBox
          Left = 24
          Top = 80
          Width = 113
          Height = 105
          Caption = 'WeightCheck'
          TabOrder = 0
          object btLoadClWghtChck: TButton
            Tag = 1
            Left = 16
            Top = 40
            Width = 75
            Height = 25
            Caption = 'Start'
            TabOrder = 0
            OnClick = btLoadClWghtChckClick
          end
        end
        object GroupBox16: TGroupBox
          Left = 152
          Top = 80
          Width = 113
          Height = 105
          Caption = 'SetHold'
          TabOrder = 1
          object btLoadClHoldOn: TButton
            Tag = 2
            Left = 16
            Top = 24
            Width = 75
            Height = 25
            Caption = 'Hold On'
            TabOrder = 0
            OnClick = btLoadClWghtChckClick
          end
          object btLoadClHoldOff: TButton
            Tag = 3
            Left = 16
            Top = 64
            Width = 75
            Height = 25
            Caption = 'Hold Off'
            TabOrder = 1
            OnClick = btLoadClWghtChckClick
          end
        end
        object edLoadClData: TEdit
          Left = 112
          Top = 32
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
          Text = '0'
        end
        object edLoadClMsg: TEdit
          Left = 112
          Top = 56
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
          Text = '0'
        end
        object GroupBox20: TGroupBox
          Left = 272
          Top = 80
          Width = 113
          Height = 105
          Caption = 'AutoZero'
          TabOrder = 4
          object Button5: TButton
            Tag = 4
            Left = 16
            Top = 24
            Width = 75
            Height = 25
            Caption = 'Zero'
            TabOrder = 0
            OnClick = btLoadClWghtChckClick
          end
        end
      end
      object GroupBox15: TGroupBox
        Left = 574
        Top = 13
        Width = 555
        Height = 465
        Caption = 'UVCuring'
        TabOrder = 1
        object Label16: TLabel
          Left = 29
          Top = 34
          Width = 86
          Height = 13
          Caption = 'Receive Message'
        end
        object lbMessageEndSignal: TLabel
          Left = 256
          Top = 34
          Width = 97
          Height = 13
          Caption = 'Message End Signal'
        end
        object lbUVMsgEnd: TLabel
          Left = 367
          Top = 34
          Width = 18
          Height = 13
          Caption = 'true'
        end
        object GroupBox18: TGroupBox
          Left = 256
          Top = 80
          Width = 217
          Height = 121
          Caption = 'UV Channel On / Off'
          TabOrder = 0
          object btUVOn: TButton
            Tag = 1
            Left = 126
            Top = 22
            Width = 75
            Height = 25
            Caption = 'On'
            TabOrder = 0
            OnClick = btUVChClick
          end
          object btUVOff: TButton
            Tag = 2
            Left = 126
            Top = 54
            Width = 75
            Height = 25
            Caption = 'Off'
            TabOrder = 1
            OnClick = btUVChClick
          end
          object cbUVCh2: TComboBox
            Left = 16
            Top = 24
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 2
            Text = '1Ch'
            Items.Strings = (
              '1Ch'
              '2Ch'
              '3Ch'
              '4Ch'
              '5Ch')
          end
        end
        object GroupBox19: TGroupBox
          Left = 24
          Top = 80
          Width = 217
          Height = 121
          Caption = 'UV Curing Light Setting'
          TabOrder = 1
          object Label12: TLabel
            Left = 16
            Top = 88
            Width = 43
            Height = 13
            Caption = '(0 ~ 255)'
          end
          object edUVLightData: TEdit
            Left = 16
            Top = 56
            Width = 65
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            Text = '0'
          end
          object btUVLight: TButton
            Tag = 3
            Left = 109
            Top = 55
            Width = 73
            Height = 25
            Caption = 'SET'
            TabOrder = 1
            OnClick = btUVChClick
          end
          object cbUVCh1: TComboBox
            Tag = 3
            Left = 16
            Top = 24
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 2
            Text = '1Ch'
            Items.Strings = (
              '1Ch'
              '2Ch'
              '3Ch'
              '4Ch'
              '5Ch')
          end
        end
        object edUVCuringMsg: TEdit
          Left = 120
          Top = 32
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
          Text = 'edUVCuringMsg'
        end
        object GroupBox17: TGroupBox
          Left = 24
          Top = 224
          Width = 217
          Height = 121
          Caption = 'UV Curing Data Check'
          TabOrder = 3
          object Label22: TLabel
            Left = 16
            Top = 92
            Width = 76
            Height = 13
            Caption = 'Error Msg Reset'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edUVDataChk: TEdit
            Left = 16
            Top = 56
            Width = 65
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            Text = '0'
          end
          object btUVLightDataChk: TButton
            Tag = 4
            Left = 109
            Top = 55
            Width = 73
            Height = 25
            Caption = 'Receive'
            TabOrder = 1
            OnClick = btUVChClick
          end
          object cbUVCh3: TComboBox
            Left = 16
            Top = 24
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 2
            Text = '1Ch'
            Items.Strings = (
              '1Ch'
              '2Ch'
              '3Ch'
              '4Ch'
              '5Ch')
          end
          object Button1: TButton
            Tag = 5
            Left = 109
            Top = 87
            Width = 73
            Height = 25
            Caption = 'Reset'
            TabOrder = 3
            OnClick = btUVChClick
          end
        end
        object Button6: TButton
          Left = 264
          Top = 232
          Width = 75
          Height = 25
          Caption = 'StageAir On'
          TabOrder = 4
          OnClick = Button6Click
        end
        object Button7: TButton
          Left = 360
          Top = 232
          Width = 75
          Height = 25
          Caption = 'StageAir Off'
          TabOrder = 5
          OnClick = Button7Click
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'HexaPort Setting'
      ImageIndex = 4
      object Label23: TLabel
        Left = 16
        Top = 73
        Width = 8
        Height = 16
        Caption = 'x'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label24: TLabel
        Left = 16
        Top = 97
        Width = 9
        Height = 16
        Caption = 'y'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label25: TLabel
        Left = 16
        Top = 121
        Width = 8
        Height = 16
        Caption = 'z'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label26: TLabel
        Left = 16
        Top = 145
        Width = 9
        Height = 16
        Caption = 'u'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label27: TLabel
        Left = 16
        Top = 169
        Width = 9
        Height = 16
        Caption = 'v'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label28: TLabel
        Left = 16
        Top = 193
        Width = 11
        Height = 16
        Caption = 'w'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label29: TLabel
        Left = 24
        Top = 224
        Width = 45
        Height = 13
        Caption = 'Error Msg'
      end
      object lbErr: TLabel
        Left = 88
        Top = 224
        Width = 21
        Height = 13
        Caption = 'lbErr'
      end
      object Label30: TLabel
        Left = 312
        Top = 73
        Width = 8
        Height = 16
        Caption = 'x'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label31: TLabel
        Left = 312
        Top = 97
        Width = 9
        Height = 16
        Caption = 'y'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label32: TLabel
        Left = 312
        Top = 121
        Width = 8
        Height = 16
        Caption = 'z'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label36: TLabel
        Left = 464
        Top = 73
        Width = 8
        Height = 16
        Caption = 'x'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label37: TLabel
        Left = 464
        Top = 97
        Width = 9
        Height = 16
        Caption = 'y'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label38: TLabel
        Left = 464
        Top = 121
        Width = 8
        Height = 16
        Caption = 'z'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label39: TLabel
        Left = 464
        Top = 145
        Width = 9
        Height = 16
        Caption = 'u'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label40: TLabel
        Left = 464
        Top = 169
        Width = 9
        Height = 16
        Caption = 'v'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label41: TLabel
        Left = 464
        Top = 193
        Width = 11
        Height = 16
        Caption = 'w'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object btHome: TButton
        Left = 16
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Home'
        TabOrder = 0
        OnClick = btHomeClick
      end
      object btMoveTo: TButton
        Left = 16
        Top = 40
        Width = 265
        Height = 25
        Caption = 'MoveTo'
        TabOrder = 1
        OnClick = btMoveToClick
      end
      object Edit1: TEdit
        Left = 32
        Top = 72
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 2
        Text = '0'
      end
      object Edit2: TEdit
        Left = 32
        Top = 96
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 3
        Text = '0'
      end
      object Edit3: TEdit
        Left = 32
        Top = 120
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 4
        Text = '0'
      end
      object Edit4: TEdit
        Left = 32
        Top = 144
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 5
        Text = '0'
      end
      object Edit5: TEdit
        Left = 32
        Top = 168
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 6
        Text = '0'
      end
      object Edit6: TEdit
        Left = 32
        Top = 192
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 7
        Text = '0'
      end
      object edEncX: TEdit
        Left = 160
        Top = 72
        Width = 121
        Height = 21
        Color = cl3DLight
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 8
        Text = '0'
      end
      object edEncY: TEdit
        Left = 160
        Top = 96
        Width = 121
        Height = 21
        Color = cl3DLight
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 9
        Text = '0'
      end
      object edEncZ: TEdit
        Left = 160
        Top = 120
        Width = 121
        Height = 21
        Color = cl3DLight
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 10
        Text = '0'
      end
      object edEncU: TEdit
        Left = 160
        Top = 144
        Width = 121
        Height = 21
        Color = cl3DLight
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 11
        Text = '0'
      end
      object edEncV: TEdit
        Left = 160
        Top = 168
        Width = 121
        Height = 21
        Color = cl3DLight
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 12
        Text = '0'
      end
      object edEncW: TEdit
        Left = 160
        Top = 192
        Width = 121
        Height = 21
        Color = cl3DLight
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 13
        Text = '0'
      end
      object edPivotX: TEdit
        Left = 328
        Top = 72
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 14
        Text = '0'
      end
      object edPivotY: TEdit
        Left = 328
        Top = 96
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 15
        Text = '0'
      end
      object edPivotZ: TEdit
        Left = 328
        Top = 120
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 16
        Text = '0'
      end
      object Button2: TButton
        Left = 312
        Top = 40
        Width = 137
        Height = 25
        Caption = 'Set Pivot'
        TabOrder = 17
        OnClick = Button2Click
      end
      object edLimMinX: TEdit
        Left = 480
        Top = 72
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 18
        Text = '0'
      end
      object edLimMinY: TEdit
        Left = 480
        Top = 96
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 19
        Text = '0'
      end
      object edLimMinZ: TEdit
        Left = 480
        Top = 120
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 20
        Text = '0'
      end
      object edLimMinU: TEdit
        Left = 480
        Top = 144
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 21
        Text = '0'
      end
      object edLimMinV: TEdit
        Left = 480
        Top = 168
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 22
        Text = '0'
      end
      object edLimMinW: TEdit
        Left = 480
        Top = 192
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 23
        Text = '0'
      end
      object edLimMaxX: TEdit
        Left = 608
        Top = 72
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 24
        Text = '0'
      end
      object edLimMaxY: TEdit
        Left = 608
        Top = 96
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 25
        Text = '0'
      end
      object edLimMaxZ: TEdit
        Left = 608
        Top = 120
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 26
        Text = '0'
      end
      object edLimMaxU: TEdit
        Left = 608
        Top = 144
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 27
        Text = '0'
      end
      object edLimMaxV: TEdit
        Left = 608
        Top = 168
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 28
        Text = '0'
      end
      object edLimMaxW: TEdit
        Left = 608
        Top = 192
        Width = 121
        Height = 21
        Color = clScrollBar
        ImeName = 'Microsoft Office IME 2007'
        ReadOnly = True
        TabOrder = 29
        Text = '0'
      end
      object Panel1: TPanel
        Left = 480
        Top = 38
        Width = 249
        Height = 25
        Caption = 'Limit'
        TabOrder = 30
        OnClick = Panel1Click
      end
      object btHexaServoOn: TButton
        Left = 120
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Servo On'
        TabOrder = 31
        OnClick = btHexaServoOnClick
      end
      object btHexaServoOff: TButton
        Left = 200
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Servo Off'
        TabOrder = 32
        OnClick = btHexaServoOffClick
      end
      object btSaveTrgPos: TButton
        Left = 760
        Top = 8
        Width = 75
        Height = 25
        Caption = 'btSaveTrgPos'
        TabOrder = 33
        OnClick = btSaveTrgPosClick
      end
      object btCMRectCenter: TButton
        Left = 480
        Top = 224
        Width = 129
        Height = 25
        Caption = 'CM_RECT_00 Center'
        TabOrder = 34
        OnClick = btCMRectCenterClick
      end
      object btCycleCmsLAlign: TButton
        Left = 616
        Top = 224
        Width = 129
        Height = 25
        Caption = 'mcCycleCmsLAlign'
        TabOrder = 35
        OnClick = btCycleCmsLAlignClick
      end
      object btDataCopy: TButton
        Left = 32
        Top = 256
        Width = 73
        Height = 33
        Caption = 'Data Copy'
        TabOrder = 36
        OnClick = btDataCopyClick
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'VisionComTest'
      ImageIndex = 5
      object Comman: TLabel
        Left = 48
        Top = 152
        Width = 47
        Height = 13
        Caption = 'Command'
      end
      object Label33: TLabel
        Left = 48
        Top = 184
        Width = 45
        Height = 13
        Caption = 'JobName'
      end
      object Vsdss: TLabel
        Left = 48
        Top = 384
        Width = 76
        Height = 13
        Caption = 'VisionLeftResult'
      end
      object Label34: TLabel
        Left = 48
        Top = 400
        Width = 83
        Height = 13
        Caption = 'VisionRightResult'
      end
      object lbLeftResult: TLabel
        Left = 136
        Top = 384
        Width = 76
        Height = 13
        Caption = 'VisionLeftResult'
      end
      object lbRightResult: TLabel
        Left = 136
        Top = 400
        Width = 76
        Height = 13
        Caption = 'VisionLeftResult'
      end
      object Button3: TButton
        Left = 48
        Top = 344
        Width = 80
        Height = 25
        Caption = 'SendVisnLeft'
        TabOrder = 0
        OnClick = Button3Click
      end
      object cbSendMsg: TComboBox
        Left = 48
        Top = 120
        Width = 145
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 1
        Text = 'Insp'
        Items.Strings = (
          'Insp'
          'Reset'
          'Command'
          'JobChange')
      end
      object Button4: TButton
        Left = 131
        Top = 344
        Width = 80
        Height = 25
        Caption = 'SendVisnRight'
        TabOrder = 2
        OnClick = Button4Click
      end
      object edCmd: TEdit
        Left = 104
        Top = 144
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 3
      end
      object edJobName: TEdit
        Left = 104
        Top = 176
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 4
      end
      object pnInspEndL: TPanel
        Left = 48
        Top = 208
        Width = 80
        Height = 32
        Caption = 'InspEndL'
        TabOrder = 5
      end
      object pnResetEndL: TPanel
        Left = 48
        Top = 240
        Width = 80
        Height = 32
        Caption = 'ResetEndL'
        TabOrder = 6
      end
      object pnCommandEndL: TPanel
        Left = 48
        Top = 272
        Width = 80
        Height = 32
        Caption = 'CommandEndL'
        TabOrder = 7
      end
      object pnJobChangeL: TPanel
        Left = 48
        Top = 304
        Width = 80
        Height = 32
        Caption = 'JobChangeL'
        TabOrder = 8
      end
      object pnInspEndR: TPanel
        Left = 131
        Top = 208
        Width = 80
        Height = 32
        Caption = 'InspEndR'
        TabOrder = 9
      end
      object pnResetEndR: TPanel
        Left = 131
        Top = 240
        Width = 80
        Height = 32
        Caption = 'ResetEndR'
        TabOrder = 10
      end
      object pnCommandEndR: TPanel
        Left = 131
        Top = 272
        Width = 80
        Height = 32
        Caption = 'CommandEndR'
        TabOrder = 11
      end
      object pnJobChangeR: TPanel
        Left = 131
        Top = 304
        Width = 80
        Height = 32
        Caption = 'JobChangeR'
        TabOrder = 12
      end
      object pnInspLRslt: TPanel
        Left = 259
        Top = 216
        Width = 80
        Height = 32
        Caption = 'InspLRslt'
        TabOrder = 13
      end
      object pnInspRRslt: TPanel
        Left = 339
        Top = 216
        Width = 80
        Height = 32
        Caption = 'InspRRslt'
        TabOrder = 14
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tmUpdateTimer
    Left = 488
    Top = 16
  end
  object tmVisn: TTimer
    Interval = 100
    OnTimer = tmVisnTimer
    Left = 8
    Top = 88
  end
end