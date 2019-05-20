object FrmMaster: TFrmMaster
  Left = 108
  Top = 276
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
    ActivePage = TabSheet3
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
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
      object Label13: TLabel
        Left = 16
        Top = 96
        Width = 219
        Height = 20
        Caption = 'Picker To Dispensor Stroke'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label1: TLabel
        Left = 376
        Top = 96
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
      object Label22: TLabel
        Left = 16
        Top = 192
        Width = 234
        Height = 20
        Caption = 'Picker To Stage Angle Offset'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label23: TLabel
        Left = 392
        Top = 192
        Width = 57
        Height = 20
        Caption = 'degree'
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
      object edPckDspStroke: TEdit
        Left = 248
        Top = 96
        Width = 121
        Height = 21
        ImeName = 'Microsoft IME 2010'
        TabOrder = 2
        Text = '0'
      end
      object cbSkipPickrDispr: TCheckBox
        Left = 16
        Top = 141
        Width = 208
        Height = 33
        Caption = 'Skip Picker && Dispensor'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object edPckToStgAngleOfs: TEdit
        Left = 264
        Top = 192
        Width = 121
        Height = 21
        ImeName = 'Microsoft IME 2010'
        TabOrder = 4
        Text = '0'
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
        object GroupBox1: TGroupBox
          Left = 24
          Top = 72
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
          Top = 72
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
          Top = 72
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
          Top = 192
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
          Top = 192
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
          Top = 72
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
          Left = 112
          Top = 32
          Width = 121
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
          Text = 'edDisprErrMsg'
        end
      end
      object GroupBox10: TGroupBox
        Left = 574
        Top = 13
        Width = 555
        Height = 465
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
    end
    object TabSheet4: TTabSheet
      Caption = 'LoadCell && Temperature'
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
            Left = 16
            Top = 24
            Width = 75
            Height = 25
            Caption = 'Hold On'
            TabOrder = 0
            OnClick = btLoadClWghtChckClick
          end
          object btLoadClHoldOff: TButton
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
      end
      object GroupBox15: TGroupBox
        Left = 566
        Top = 13
        Width = 555
        Height = 465
        Caption = 'Temperature'
        TabOrder = 1
        object GroupBox17: TGroupBox
          Left = 192
          Top = 28
          Width = 257
          Height = 105
          Caption = 'Set Temperature'
          TabOrder = 0
          object cbTempCh: TComboBox
            Left = 8
            Top = 32
            Width = 97
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 0
            Text = 'Rail Bottom'
            Items.Strings = (
              'Rail Bottom'
              'Rail Top'
              'Stage Bottom')
          end
          object btSetTemp: TButton
            Left = 144
            Top = 64
            Width = 73
            Height = 25
            Caption = 'SET'
            TabOrder = 1
            OnClick = btSetTempClick
          end
          object edSetTemp: TEdit
            Left = 120
            Top = 32
            Width = 92
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            Text = '0'
          end
        end
        object GroupBox18: TGroupBox
          Left = 24
          Top = 28
          Width = 161
          Height = 105
          Caption = 'Crnt Temperature'
          TabOrder = 1
          object Label12: TLabel
            Left = 6
            Top = 26
            Width = 54
            Height = 13
            Caption = 'Rail Bottom'
          end
          object Label14: TLabel
            Left = 6
            Top = 48
            Width = 40
            Height = 13
            Caption = 'Rail Top'
          end
          object Label15: TLabel
            Left = 6
            Top = 71
            Width = 64
            Height = 13
            Caption = 'Stage Bottom'
          end
          object edCrntTemp1: TEdit
            Left = 77
            Top = 24
            Width = 73
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            Text = '0'
          end
          object edCrntTemp2: TEdit
            Left = 77
            Top = 46
            Width = 73
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 1
            Text = '0'
          end
          object edCrntTemp3: TEdit
            Left = 77
            Top = 68
            Width = 73
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            Text = '0'
          end
        end
        object GroupBox20: TGroupBox
          Left = 24
          Top = 144
          Width = 161
          Height = 105
          Caption = 'Get Temperature'
          TabOrder = 2
          object Label16: TLabel
            Left = 6
            Top = 26
            Width = 54
            Height = 13
            Caption = 'Rail Bottom'
          end
          object Label17: TLabel
            Left = 6
            Top = 48
            Width = 40
            Height = 13
            Caption = 'Rail Top'
          end
          object Label18: TLabel
            Left = 6
            Top = 71
            Width = 64
            Height = 13
            Caption = 'Stage Bottom'
          end
          object edGetTemp1: TEdit
            Left = 77
            Top = 24
            Width = 73
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            Text = '0'
          end
          object edGetTemp2: TEdit
            Left = 77
            Top = 46
            Width = 73
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 1
            Text = '0'
          end
          object edGetTemp3: TEdit
            Left = 77
            Top = 68
            Width = 73
            Height = 21
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            Text = '0'
          end
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'TabSheet5'
      ImageIndex = 4
      object Button1: TButton
        Left = 56
        Top = 56
        Width = 75
        Height = 17
        Caption = 'Button1'
        TabOrder = 0
        OnClick = Button1Click
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tmUpdateTimer
    Left = 1120
    Top = 24
  end
  object dgFilePath: TOpenDialog
    Left = 400
    Top = 560
  end
end
