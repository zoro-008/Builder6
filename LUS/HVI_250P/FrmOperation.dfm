object FormOperation: TFormOperation
  Left = 51
  Top = 114
  Align = alClient
  BorderStyle = bsNone
  Caption = 'FormOperation'
  ClientHeight = 868
  ClientWidth = 1272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1272
    Height = 868
    Align = alClient
    BevelOuter = bvNone
    Color = clWindowText
    TabOrder = 0
    object Panel3: TPanel
      Left = 0
      Top = 645
      Width = 1272
      Height = 223
      Align = alBottom
      Color = clSkyBlue
      TabOrder = 0
      object Label27: TLabel
        Left = 19
        Top = 194
        Width = 90
        Height = 16
        Caption = 'Error Message'
        Font.Charset = ANSI_CHARSET
        Font.Color = clOlive
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label28: TLabel
        Left = 650
        Top = 194
        Width = 110
        Height = 16
        Caption = 'Release Message'
        Font.Charset = ANSI_CHARSET
        Font.Color = clOlive
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object mmAction: TMemo
        Left = 650
        Top = 8
        Width = 600
        Height = 180
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 1
      end
      object lbErr: TListBox
        Left = 19
        Top = 9
        Width = 600
        Height = 180
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
        ItemHeight = 15
        ParentFont = False
        TabOrder = 0
        OnClick = lbErrClick
      end
    end
    object Panel4: TPanel
      Left = 0
      Top = 0
      Width = 1272
      Height = 146
      Align = alTop
      BevelOuter = bvNone
      Color = clSkyBlue
      TabOrder = 1
      object GroupBox2: TGroupBox
        Left = 307
        Top = 0
        Width = 582
        Height = 145
        Caption = 'LOT Product Infoamtion'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 0
        object Label5: TLabel
          Left = 12
          Top = 44
          Width = 89
          Height = 19
          AutoSize = False
          Caption = 'LOT Name'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label6: TLabel
          Left = 12
          Top = 94
          Width = 121
          Height = 19
          AutoSize = False
          Caption = 'Working Device'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label8: TLabel
          Left = 316
          Top = 44
          Width = 129
          Height = 19
          AutoSize = False
          Caption = 'Product Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label9: TLabel
          Left = 316
          Top = 97
          Width = 89
          Height = 19
          AutoSize = False
          Caption = 'Led Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnLotNo: TPanel
          Left = 146
          Top = 41
          Width = 158
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object pnLotJobFile: TPanel
          Left = 146
          Top = 89
          Width = 158
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
        object pnLotWorkStrip: TPanel
          Left = 434
          Top = 41
          Width = 137
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
        object pnLotWorkChip: TPanel
          Left = 434
          Top = 92
          Width = 137
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
        end
      end
      object GroupBox3: TGroupBox
        Left = 892
        Top = 0
        Width = 212
        Height = 145
        Caption = 'CAM INFO'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 1
        object Label7: TLabel
          Left = 48
          Top = 29
          Width = 117
          Height = 19
          Caption = 'Punch Position'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbWRK_ZPch: TLabel
          Left = 21
          Top = 66
          Width = 170
          Height = 67
          Alignment = taCenter
          AutoSize = False
          Caption = '90'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -53
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
      end
      object GroupBox4: TGroupBox
        Left = 1107
        Top = 0
        Width = 163
        Height = 145
        Caption = 'Skip Option'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 2
        object cbBarCodePrintSkip: TCheckBox
          Left = 6
          Top = 81
          Width = 150
          Height = 17
          Caption = 'Bar Code Print Skip'
          TabOrder = 0
          Visible = False
          OnClick = cbDoorSkipClick
        end
        object cbDoorSkip: TCheckBox
          Left = 6
          Top = 23
          Width = 150
          Height = 17
          Caption = 'Door Skip'
          TabOrder = 1
          OnClick = cbDoorSkipClick
        end
        object cbUseOnly1Bin: TCheckBox
          Left = 7
          Top = 44
          Width = 150
          Height = 17
          Caption = 'Use Only 1Bin'
          TabOrder = 2
          OnClick = cbDoorSkipClick
        end
      end
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 305
        Height = 145
        Caption = 'Total Product Infoamtion'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 3
        object Label1: TLabel
          Left = 8
          Top = 33
          Width = 150
          Height = 19
          AutoSize = False
          Caption = 'Total Frame Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label2: TLabel
          Left = 8
          Top = 72
          Width = 150
          Height = 19
          AutoSize = False
          Caption = 'Total Led Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label14: TLabel
          Left = 8
          Top = 109
          Width = 150
          Height = 19
          AutoSize = False
          Caption = 'Tool Count'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnTotalFrmCnt: TPanel
          Tag = 1
          Left = 161
          Top = 29
          Width = 137
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnTotalFrmCntDblClick
        end
        object pnTotalLedCnt: TPanel
          Tag = 2
          Left = 161
          Top = 66
          Width = 137
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
        object pnTotalToolCnt: TPanel
          Tag = 3
          Left = 161
          Top = 103
          Width = 137
          Height = 27
          Alignment = taRightJustify
          Caption = '0'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
      end
    end
    object Panel2: TPanel
      Left = 0
      Top = 146
      Width = 1272
      Height = 499
      Align = alClient
      BevelOuter = bvNone
      Color = clSkyBlue
      TabOrder = 2
      object GroupBox9: TGroupBox
        Left = 426
        Top = -1
        Width = 420
        Height = 248
        Caption = 'PreBuffer'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 0
        object Label32: TLabel
          Left = 4
          Top = 228
          Width = 28
          Height = 14
          Caption = 'LotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label31: TLabel
          Left = 239
          Top = 229
          Width = 67
          Height = 14
          Caption = 'MgzNo,SlotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object btPRBInput: TButton
          Tag = 1
          Left = 356
          Top = 227
          Width = 59
          Height = 17
          Caption = 'INPUT'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btPRBInputClick
        end
        object btPRBClear: TButton
          Left = 339
          Top = 12
          Width = 75
          Height = 18
          Caption = 'CLEAR'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btPRBClearClick
        end
        object pnPreBuff: TPanel
          Left = 5
          Top = 31
          Width = 410
          Height = 195
          BevelOuter = bvNone
          TabOrder = 2
        end
        object edPRBId: TEdit
          Left = 307
          Top = 227
          Width = 41
          Height = 18
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 3
        end
        object edPRBLotNo: TEdit
          Left = 35
          Top = 228
          Width = 194
          Height = 15
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 4
        end
      end
      object GroupBox6: TGroupBox
        Left = 2
        Top = 248
        Width = 420
        Height = 248
        Caption = 'Pre Work Zone'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 1
        object Label13: TLabel
          Left = 4
          Top = 275
          Width = 28
          Height = 14
          Caption = 'LotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label15: TLabel
          Left = 439
          Top = 275
          Width = 67
          Height = 14
          Caption = 'MgzNo,SlotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label21: TLabel
          Left = 4
          Top = 228
          Width = 28
          Height = 14
          Caption = 'LotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label22: TLabel
          Left = 239
          Top = 229
          Width = 67
          Height = 14
          Caption = 'MgzNo,SlotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object edWk2LotNo: TEdit
          Left = 37
          Top = 274
          Width = 390
          Height = 17
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 0
        end
        object btWk2Input: TButton
          Tag = 2
          Left = 557
          Top = 274
          Width = 59
          Height = 19
          Caption = 'INPUT'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btPRBInputClick
        end
        object edWk2Id: TEdit
          Left = 511
          Top = 274
          Width = 35
          Height = 18
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 2
        end
        object btPWKClear: TButton
          Left = 339
          Top = 12
          Width = 75
          Height = 18
          Caption = 'CLEAR'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btPRBClearClick
        end
        object pnWorkZone: TPanel
          Left = 5
          Top = 31
          Width = 410
          Height = 195
          BevelOuter = bvNone
          TabOrder = 4
        end
        object edPWKLotNo: TEdit
          Left = 35
          Top = 228
          Width = 194
          Height = 15
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 5
        end
        object edPWKId: TEdit
          Left = 307
          Top = 227
          Width = 41
          Height = 18
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 6
        end
      end
      object GroupBox7: TGroupBox
        Left = 1
        Top = -1
        Width = 420
        Height = 248
        Caption = 'IN BUFFER'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 2
        object Label19: TLabel
          Left = 4
          Top = 228
          Width = 28
          Height = 14
          Caption = 'LotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label20: TLabel
          Left = 239
          Top = 229
          Width = 67
          Height = 14
          Caption = 'MgzNo,SlotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Button1: TButton
          Left = 539
          Top = 13
          Width = 75
          Height = 23
          Caption = 'CLEAR'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btPRBClearClick
        end
        object btINBClear: TButton
          Left = 339
          Top = 12
          Width = 75
          Height = 18
          Caption = 'CLEAR'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btPRBClearClick
        end
        object pnInBuff: TPanel
          Left = 5
          Top = 31
          Width = 410
          Height = 195
          BevelOuter = bvNone
          TabOrder = 2
        end
        object edINBLotNo: TEdit
          Left = 35
          Top = 228
          Width = 194
          Height = 15
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 3
        end
        object edINBId: TEdit
          Left = 307
          Top = 227
          Width = 41
          Height = 18
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 4
        end
      end
      object GroupBox5: TGroupBox
        Left = 425
        Top = 248
        Width = 420
        Height = 248
        Caption = 'Work Zone'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 3
        object Label3: TLabel
          Left = 4
          Top = 275
          Width = 28
          Height = 14
          Caption = 'LotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label4: TLabel
          Left = 439
          Top = 275
          Width = 67
          Height = 14
          Caption = 'MgzNo,SlotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label23: TLabel
          Left = 4
          Top = 228
          Width = 28
          Height = 14
          Caption = 'LotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 239
          Top = 229
          Width = 67
          Height = 14
          Caption = 'MgzNo,SlotNo'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Edit1: TEdit
          Left = 37
          Top = 274
          Width = 390
          Height = 17
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 0
        end
        object Button2: TButton
          Tag = 2
          Left = 557
          Top = 274
          Width = 59
          Height = 19
          Caption = 'INPUT'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btPRBInputClick
        end
        object Edit2: TEdit
          Left = 511
          Top = 274
          Width = 35
          Height = 18
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 2
        end
        object btWRKClear: TButton
          Left = 339
          Top = 12
          Width = 75
          Height = 18
          Caption = 'CLEAR'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btPRBClearClick
        end
        object pnWorkLast: TPanel
          Left = 5
          Top = 31
          Width = 410
          Height = 195
          BevelOuter = bvNone
          TabOrder = 4
        end
        object edWRKLotNo: TEdit
          Left = 35
          Top = 228
          Width = 194
          Height = 15
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 5
        end
        object edWRKId: TEdit
          Left = 307
          Top = 227
          Width = 41
          Height = 18
          AutoSize = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clSkyBlue
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 6
        end
      end
      object GroupBox8: TGroupBox
        Left = 850
        Top = -1
        Width = 414
        Height = 478
        Caption = 'Bin Info'
        Color = clSkyBlue
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        TabOrder = 4
        object lbBin5: TLabel
          Left = 184
          Top = 160
          Width = 45
          Height = 19
          AutoSize = False
          Caption = 'BIN5'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label11: TLabel
          Left = 55
          Top = 304
          Width = 74
          Height = 27
          AutoSize = False
          Caption = 'TOTAL'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbBin4: TLabel
          Left = 58
          Top = 160
          Width = 45
          Height = 19
          AutoSize = False
          Caption = 'BIN4'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbBin3: TLabel
          Left = 311
          Top = 21
          Width = 45
          Height = 19
          AutoSize = False
          Caption = 'BIN3'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbBin2: TLabel
          Left = 184
          Top = 21
          Width = 45
          Height = 19
          AutoSize = False
          Caption = 'BIN2'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label18: TLabel
          Left = 58
          Top = 21
          Width = 45
          Height = 19
          AutoSize = False
          Caption = 'BIN1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object btBinBoxRemove: TBitBtn
          Left = 200
          Top = 344
          Width = 193
          Height = 104
          Caption = 'BIN BOX REMOVE'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = btBinBoxRemoveClick
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF2F2F2E3E3E3E5E5
            E5F6F6F6FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9E4E4E4BBBBBB9696969C9C
            9CC8C8C8EDEDEDFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFDFDFDF0F0F0D1D1D19F9E9F8A898B6D6C6D5858
            587D7D7DB3B3B3E0E0E0F8F8F8FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFEFEFEF8F8F8E1E1E1B8B8B88C8B8C908F91A4A3A4A09FA18786
            883D3D3D6A6A6A9C9C9CCFCFCFF2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFCFCFCEEEEEECDCDCD9B9A9B8B8A8CAAA9ABB6B5B7B6B6B6DBDCDCA5A4
            A69190924C4B4C595959868686BDBDBDE7E7E7F9F9F9FEFEFEFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
            F7F7F7DEDEDEB3B3B38A8A8B9B9A9CB1B0B2BDBDBDC7C6C7BCBCBCE3E3E3C5C6
            C6ABACACA09FA17574764E4E4E737373A5A5A5D2D2D2F2F2F2FEFEFEFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3F3F3
            CDCDCD9696968C8C8DA09FA0BFBFC0CECECDCBCACBC8C8C8BCBCBDE3E3E3C7C8
            C8C4C6C5B4B5B5A0A1A18C8B8D4746475A5A5A888888C3C3C3EFEFEFFDFDFDFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD9D9D9
            9291929C9B9CBEBEBED2D2D2D3D3D3CFD0CFCDCCCCC9CAC9BDBDBEE3E3E3C7C8
            C8C5C6C6C2C2C2BCBDBDA2A2A39C7E757D77775151517A7A7AB6B6B6E5E5E5FA
            FAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989799
            ADADAED1D0D1DADBDBD7D8D7D3D3D3D0D0D0CDCCCDCBCACABFBFC0E3E3E3C6C7
            C7C4C5C4C1C1C1BEBEBEBCBBBCBA7C68A86148906B653B33326E6E6EA4A4A4D8
            D8D8F6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989799
            E6E5E6DFDFDFDDDCDDD8D8D8D4D4D4CECFCFCECDCDD0D0CFC4C4C4E3E3E3C6C7
            C7C1C2C3BEC0BFBCBDBDB9BABABA7D68AB5B3EA7563A943728501D1A61616192
            9292CBCBCBF2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9A999B
            EFEFEFE9E8E8E1E2E2DBDBDBD6D6D5D5D5D5D8D8D8D1D0D1B7B7B8E5E5E5C2C2
            C2C4C4C5C0C0C0BABBBBB7B8B7B87C66AB5B3FA7573AA252369636216B140F57
            5454838383BFBFBFEAEAEAFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9B9A9B
            F4F4F4F2F3F2F1F0F0ECECECEAE9EAE8E8E8CBCBCCE8E8E8E6E6E6CECECDE1E1
            E1CFCFCEC5C6C5BCBDBCBABAB9B97A64AB5B3EA7563AA251359C4C2F953D2379
            11073B2E2D767676AFAFAFE0E0E0F9F9F9FFFFFFFFFFFFFFFFFFFFFFFF979698
            F7F7F7F6F6F6F4F4F4F2F2F2E8E8E8ECECECEDEDEDD4D4D4CECECDC6C5C6C0C0
            C0C2C2C2DEDEDECFCFCFC3C4C3B97962AB5A3EA7563AA252369D4C2F96462990
            3D21831A0C4824226969699E9E9ED5D5D5F5F5F5FEFEFEFFFFFFFFFFFF919092
            F8F8F8F8F8F8F7F7F7F3F3F3F1F1F0E3E2E2DDDCDDD6D7D6D0D0D0C9C9C9C3C2
            C2BDBCBDB6B6B7C1C1C0DBD9D9BA7A64B8735CA7563BA252359D4C3097452991
            3F238B381C85220D5B19165E5E5E8F8F8FCACACAF0F0F0FDFDFDFFFFFF919092
            FAFAFAF8F8F8F2F2F2F3F3F3EBEBEAE5E5E4DEDEDFD8D8D8D2D2D2CDCDCDC7C6
            C6C0C0C0C5C6C6C3C3C3BA9C94A25F529B3723B47057A252359C4C3097452A91
            3F238B381C85331680250D650E063B3535828282C0C0C0F0F0F0FFFFFF919092
            F4F4F4F6F6F6F6F6F6F5F4F4F1F2F1E6E6E7E1E0E0DADBDBD5D4D5CECFCEC8C8
            C8CCCBCCBBB2AEAB6954AB593CA9573AA2482D952A19AE654E9D4B2F97462990
            40228A381C8433167F2D107B250A7312063F2B2A888888DADADAFFFFFFBABABB
            A5A4A5D2D2D3F8F8F8F3F3F3F2F2F2E9E8E9E3E3E3DDDDDDD6D7D6D0D0D0D2D1
            D0B58E81AD5D42AC5B3FAA583CA9573AA65134A44F32962E1BA55543A6624990
            3F238B381C8532167F2D107A280B762306820902717171CFCFCFFFFFFFFFFFFF
            DDDDDEA9A8AAC2C1C2F3F3F4F3F3F3F0F0F0E5E5E4DFDFDFDFDFDFD1C3BEAF6B
            54AE5E42AE5C40AC5B3EAB583BA9563AA65234A44F31A34C2F9C3E25902816A1
            5A428A381C8532167F2C107A280B7623078606006E6E6ECECECEFFFFFFFFFFFF
            FFFFFFF8F8F8B2B1B2ADACAEE5E4E5F2F1F2E7E6E7E4E0E0B98A7BB06145AF60
            44AF5E42AE5D41AC5A3EAA593CA95639A65133A44F32A34D2FA14A2C9E462891
            2A169B4E398433167F2D107A270A7721068606006E6E6ECECECEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFD6D5D69A999BD0CFD0DCC8C2C38D7AB76E55B06145AF60
            44AE5E42AD5C40AC5A3EAB583BA95639A55133A44F31A24C2FA04A2C9F48299E
            45279B3F26994534924C347B280A7721078606006E6E6ECFCFCFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFF3F3F3ADAAAAC1988ACF9E8CC78E7AB9735AAF5F
            43AE5D41AD5C3FAC5A3DAB573BA95638A55133A34E31A24C2EA14A2BA35032A7
            593DAC6348AA5C448E2C188E462E782105860600707070CFCFCFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCABEBEA86F65CC9A89CB9885C07F
            69B4694EAD5C40AC5A3DAA583BA85639A75639AA593DAC6045AF664DB36E56B3
            6F57B16B52AE674DAC62489C442E913E2C860600888888DADADAFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFECDDDCA4453FBE7B6BCE9E
            8DC99481C0826BB77259B0644AAC5A3EC28975C08671BE816CBB7C67B87862B6
            745DB47058B16B53AF684FAC644B9C3E2B860903C2C2C2F0F0F0FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFDFCBE7C78A547
            3CCB9887CD9B8ACB9987C99583C7917EC58E7AC28A76C08672BE826EBC7E68B9
            7A63B6755EB4715AAD644D93241890302ACAB6B5F2F2F2FEFEFEFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE4C9
            C79E3A33BD7A6ACD9C8BCB9987C99684C79380C58F7CC28B77C08773BE836EBC
            7F6ABA7B65A54E3C89140DAC7A77E9E9E9FBFBFBFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFAF6F5B76D69A34538D9B2A6CB9988D8B3A6CFA394C58F7CCC9C8DC99A89C3
            8B7A94271C943F3AD2C6C5F5F5F5FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFDAB6B4A94F49CE9E92D7B1A4DEBEB3CFA494CEA191AA56478D
            1F19B68F8DECECECFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF7F0EFAE5B57901C14CD9C90D9B4A8972B219B534FDC
            D6D5F7F7F7FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCA9591A34943962D28CEB3B1F0F0F0FD
            FDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFDFCFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        end
        object pnBin5Cnt: TPanel
          Tag = 5
          Left = 152
          Top = 180
          Width = 110
          Height = 27
          Alignment = taRightJustify
          Caption = '16000/16000'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          OnClick = pnBin1CntClick
        end
        object pnBin5Per: TPanel
          Left = 152
          Top = 208
          Width = 110
          Height = 81
          Caption = '0%'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
        object pnTotalBinPer: TPanel
          Left = 13
          Top = 336
          Width = 156
          Height = 121
          Caption = '0%'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
        end
        object pnBin4Cnt: TPanel
          Tag = 4
          Left = 24
          Top = 180
          Width = 110
          Height = 27
          Alignment = taRightJustify
          Caption = '16000/16000'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          OnClick = pnBin1CntClick
        end
        object pnBin4Per: TPanel
          Left = 24
          Top = 208
          Width = 110
          Height = 81
          Caption = '0%'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 5
        end
        object pnBin3Cnt: TPanel
          Tag = 3
          Left = 280
          Top = 42
          Width = 110
          Height = 27
          Alignment = taRightJustify
          Caption = '16000/16000'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 6
          OnClick = pnBin1CntClick
        end
        object pnBin3Per: TPanel
          Left = 280
          Top = 70
          Width = 110
          Height = 81
          Caption = '0%'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 7
        end
        object pnBin1Per: TPanel
          Left = 24
          Top = 70
          Width = 110
          Height = 81
          Caption = '0%'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 8
        end
        object pnBin1Cnt: TPanel
          Tag = 1
          Left = 24
          Top = 42
          Width = 110
          Height = 27
          Alignment = taRightJustify
          Caption = '16000/16000'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 9
          OnClick = pnBin1CntClick
        end
        object pnBin2Per: TPanel
          Left = 152
          Top = 70
          Width = 110
          Height = 81
          Caption = '0%'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 10
        end
        object pnBin2Cnt: TPanel
          Tag = 2
          Left = 152
          Top = 42
          Width = 110
          Height = 27
          Alignment = taRightJustify
          Caption = '16000/16000'
          Color = clBtnText
          Font.Charset = ANSI_CHARSET
          Font.Color = clLime
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 11
          OnClick = pnBin1CntClick
        end
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 688
    Top = 8
  end
end
