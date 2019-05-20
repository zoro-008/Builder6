object FrmOperation: TFrmOperation
  Left = 83
  Top = 94
  BorderStyle = bsNone
  Caption = 'FrmOperation'
  ClientHeight = 870
  ClientWidth = 1272
  Color = clBtnFace
  Constraints.MaxHeight = 908
  Constraints.MaxWidth = 1296
  Constraints.MinHeight = 897
  Constraints.MinWidth = 1280
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1272
    Height = 870
    Align = alClient
    BevelOuter = bvNone
    Color = clBtnText
    TabOrder = 0
    object Panel5: TPanel
      Left = 0
      Top = 0
      Width = 959
      Height = 869
      TabOrder = 0
      object Panel14: TPanel
        Left = 1
        Top = 656
        Width = 688
        Height = 211
        BevelOuter = bvNone
        TabOrder = 0
        object Panel15: TPanel
          Left = 0
          Top = 0
          Width = 688
          Height = 41
          Align = alTop
          BevelWidth = 3
          Caption = 'ERROR'
          Color = 15920326
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -27
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object lbErr: TListBox
          Left = 0
          Top = 41
          Width = 688
          Height = 170
          Align = alClient
          BevelInner = bvNone
          BorderStyle = bsNone
          Color = cl3DLight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = []
          ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
          ItemHeight = 15
          ParentFont = False
          TabOrder = 1
        end
      end
      object Panel2: TPanel
        Left = 1
        Top = 1
        Width = 957
        Height = 661
        TabOrder = 1
        object Panel3: TPanel
          Left = 1
          Top = 1
          Width = 955
          Height = 40
          Align = alTop
          BevelWidth = 3
          Caption = 'WORKING  SHOW'
          Color = 15920326
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -27
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object Panel4: TPanel
          Left = 1
          Top = 41
          Width = 955
          Height = 619
          Align = alClient
          TabOrder = 1
          object Panel7: TPanel
            Left = 1
            Top = 1
            Width = 953
            Height = 617
            Align = alCustom
            Color = clCream
            TabOrder = 0
            object ImBaseT: TImage
              Left = 1
              Top = 41
              Width = 951
              Height = 575
              Align = alClient
              Center = True
              Transparent = True
            end
            object spBaseDetect: TShape
              Left = 215
              Top = 77
              Width = 521
              Height = 501
              Brush.Style = bsClear
              Pen.Color = 5460991
              Pen.Width = 10
              Shape = stCircle
              Visible = False
            end
            object spLaser: TShape
              Left = 408
              Top = 133
              Width = 137
              Height = 116
              Brush.Style = bsClear
              Pen.Color = clLime
              Pen.Width = 10
              Shape = stRoundRect
              Visible = False
            end
            object lbBlockNo: TLabel
              Left = 920
              Top = 49
              Width = 76
              Height = 24
              Caption = 'Block No'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -19
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label9: TLabel
              Left = 830
              Top = 49
              Width = 91
              Height = 24
              Caption = 'Block No : '
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -19
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Panel18: TPanel
              Left = 1
              Top = 1
              Width = 951
              Height = 40
              Align = alTop
              BevelWidth = 3
              Caption = 'CURRENT  MOTOR  POSITION'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object pnBoard2: TPanel
              Left = 390
              Top = 42
              Width = 170
              Height = 30
              Caption = 'BOARD 2'
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
            end
            object pnBoard1: TPanel
              Left = 390
              Top = 584
              Width = 170
              Height = 30
              Caption = 'BOARD 1'
              Color = clBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
            end
            object GroupBox1: TGroupBox
              Left = 11
              Top = 48
              Width = 198
              Height = 97
              Caption = #50892#53356#54252#51648#49496' '#44048#51648' '#48143' '#47112#51060#51200' '#51089#50629' '#54364
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -8
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              object Shape1: TShape
                Left = 10
                Top = 22
                Width = 30
                Height = 30
                Brush.Style = bsClear
                Pen.Color = clLime
                Pen.Width = 4
                Shape = stRoundRect
              end
              object Shape2: TShape
                Left = 10
                Top = 59
                Width = 30
                Height = 30
                Brush.Style = bsClear
                Pen.Color = 5460991
                Pen.Width = 4
                Shape = stCircle
              end
              object Label4: TLabel
                Left = 58
                Top = 67
                Width = 117
                Height = 13
                Caption = #50892#53356' '#54252#51648#49496#50640' '#50948#52824' '#51473
              end
              object Label3: TLabel
                Left = 58
                Top = 30
                Width = 78
                Height = 13
                Caption = #47112#51060#51200' '#51089#50629' '#51473
              end
            end
            object pnTest: TPanel
              Left = 786
              Top = 518
              Width = 166
              Height = 97
              TabOrder = 4
              Visible = False
              object Button6: TButton
                Left = 70
                Top = 5
                Width = 93
                Height = 25
                Caption = 'BlockWork'
                TabOrder = 0
              end
              object Edit3: TEdit
                Left = 4
                Top = 7
                Width = 64
                Height = 21
                ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
                TabOrder = 1
                Text = '20'
              end
              object Edit1: TEdit
                Left = 4
                Top = 38
                Width = 64
                Height = 21
                ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
                TabOrder = 2
                Text = 'GA,0001,'
              end
              object Button3: TButton
                Left = 70
                Top = 37
                Width = 93
                Height = 25
                Caption = 'CheckSumTest'
                TabOrder = 3
                OnClick = Button3Click
              end
              object Edit2: TEdit
                Left = 4
                Top = 69
                Width = 64
                Height = 21
                ImeName = 'Microsoft IME 2010'
                TabOrder = 4
                Text = 'GA,20'
              end
              object Button2: TButton
                Left = 70
                Top = 69
                Width = 93
                Height = 25
                Caption = 'Send Msg'
                TabOrder = 5
                OnClick = Button2Click
              end
            end
          end
        end
      end
      object Panel10: TPanel
        Left = 688
        Top = 661
        Width = 269
        Height = 208
        TabOrder = 2
        object lbOption2: TLabel
          Left = 6
          Top = 44
          Width = 153
          Height = 18
          Caption = 'DOOR SENSOR SKIP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label2: TLabel
          Left = 6
          Top = 85
          Width = 88
          Height = 18
          Caption = 'LASER SKIP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label5: TLabel
          Left = 6
          Top = 126
          Width = 142
          Height = 18
          Caption = 'VCC SENSOR  SKIP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 267
          Height = 40
          Align = alTop
          BevelWidth = 3
          Caption = 'OPTION'
          Color = 15920326
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -27
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object pnDoor: TPanel
          Left = 17
          Top = 61
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          OnClick = pnDoorClick
        end
        object pnLaser: TPanel
          Tag = 1
          Left = 17
          Top = 101
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          OnClick = pnDoorClick
        end
        object pnVaccum: TPanel
          Tag = 2
          Left = 17
          Top = 142
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          OnClick = pnDoorClick
        end
        object btMasterOptn: TButton
          Left = 8
          Top = 174
          Width = 153
          Height = 25
          Caption = 'MASTER  OPTION'
          Enabled = False
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          OnClick = btMasterOptnClick
        end
        object pnMaster: TPanel
          Left = 4
          Top = 40
          Width = 264
          Height = 168
          TabOrder = 5
          Visible = False
          object Label6: TLabel
            Left = 6
            Top = 4
            Width = 154
            Height = 18
            Caption = 'BOARD2 LASER SKIP'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label10: TLabel
            Left = 6
            Top = 45
            Width = 140
            Height = 18
            Caption = 'ONLY USED RIGHT'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label11: TLabel
            Left = 6
            Top = 86
            Width = 119
            Height = 18
            Caption = 'DRY RUN MODE'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label12: TLabel
            Left = 7
            Top = 126
            Width = 129
            Height = 18
            Caption = 'NOT USED V AXIS'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object pnBrd2Skip: TPanel
            Left = 13
            Top = 21
            Width = 100
            Height = 20
            BevelInner = bvLowered
            Caption = 'ON'
            Color = clLime
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            OnClick = pnBrd2SkipClick
          end
          object pnUseRgh: TPanel
            Tag = 1
            Left = 13
            Top = 61
            Width = 100
            Height = 20
            BevelInner = bvLowered
            Caption = 'ON'
            Color = clLime
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            OnClick = pnBrd2SkipClick
          end
          object btClose: TButton
            Left = 172
            Top = 133
            Width = 75
            Height = 25
            Caption = 'CLOSE'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            OnClick = btCloseClick
          end
          object pnDryRun: TPanel
            Tag = 2
            Left = 13
            Top = 102
            Width = 100
            Height = 20
            BevelInner = bvLowered
            Caption = 'ON'
            Color = clLime
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            OnClick = pnBrd2SkipClick
          end
          object pnNotUsedVAxis: TPanel
            Tag = 3
            Left = 14
            Top = 142
            Width = 100
            Height = 20
            BevelInner = bvLowered
            Caption = 'ON'
            Color = clLime
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = pnBrd2SkipClick
          end
        end
      end
    end
    object Panel6: TPanel
      Left = 959
      Top = 0
      Width = 321
      Height = 870
      TabOrder = 1
      object Panel8: TPanel
        Left = 1
        Top = 1
        Width = 319
        Height = 305
        Align = alTop
        BevelOuter = bvNone
        Color = clActiveBorder
        TabOrder = 0
        object lbDInfo3: TLabel
          Left = 29
          Top = 54
          Width = 174
          Height = 24
          Caption = 'TODAY RUN TIME'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label1: TLabel
          Left = 29
          Top = 136
          Width = 142
          Height = 24
          Caption = 'TODAY WORK'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 29
          Top = 224
          Width = 121
          Height = 24
          Caption = 'WORK TIME'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 197
          Top = 256
          Width = 43
          Height = 24
          Caption = 'SEC'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Panel9: TPanel
          Left = 0
          Top = 0
          Width = 319
          Height = 41
          Align = alTop
          BevelWidth = 3
          Caption = 'INFORMATION'
          Color = 15920326
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -27
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object pnDayTotalTime: TPanel
          Left = 26
          Top = 79
          Width = 150
          Height = 42
          BevelInner = bvLowered
          Caption = '00::00::01'
          Color = clBtnText
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnTodayWork: TPanel
          Left = 26
          Top = 163
          Width = 150
          Height = 42
          BevelInner = bvLowered
          Caption = '0'
          Color = clBtnText
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object btTotalTmReset: TButton
          Left = 193
          Top = 79
          Width = 91
          Height = 39
          Caption = 'RESET'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btTotalTmResetClick
        end
        object btTotalWkReset: TButton
          Tag = 1
          Left = 193
          Top = 162
          Width = 91
          Height = 39
          Caption = 'RESET'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = btTotalTmResetClick
        end
        object pnWorkTime: TPanel
          Left = 26
          Top = 247
          Width = 150
          Height = 42
          BevelInner = bvLowered
          Caption = '0'
          Color = clBtnText
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
      end
      object pnManual: TPanel
        Left = 1
        Top = 306
        Width = 319
        Height = 563
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 1
        object Panel20: TPanel
          Left = 0
          Top = 0
          Width = 319
          Height = 41
          Align = alTop
          BevelWidth = 3
          Caption = 'BUTTON'
          Color = 15920326
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -27
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object btStart: TBitBtn
          Left = 2
          Top = 41
          Width = 322
          Height = 104
          Caption = 'START'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -53
          Font.Name = 'MS UI Gothic'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btStartClick
        end
        object btRotate: TBitBtn
          Tag = 3
          Left = 2
          Top = 145
          Width = 322
          Height = 104
          Caption = 'ROTATE'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -53
          Font.Name = 'MS UI Gothic'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btRotateClick
        end
        object btHome: TBitBtn
          Tag = 1
          Left = 2
          Top = 249
          Width = 322
          Height = 104
          Caption = 'HOME'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -53
          Font.Name = 'MS UI Gothic'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btHomeClick
        end
        object btReset: TBitBtn
          Left = 2
          Top = 353
          Width = 322
          Height = 104
          Caption = 'RESET'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -53
          Font.Name = 'MS UI Gothic'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = btResetClick
        end
        object btOperator: TBitBtn
          Left = 2
          Top = 457
          Width = 322
          Height = 104
          Caption = 'OPERATOR'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -53
          Font.Name = 'MS UI Gothic'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = btOperatorClick
        end
      end
    end
  end
  object plPassWord: TPanel
    Left = 1118
    Top = 751
    Width = 159
    Height = 116
    TabOrder = 1
    Visible = False
    object Panel19: TPanel
      Left = 1
      Top = 1
      Width = 157
      Height = 29
      Align = alTop
      Color = clPurple
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindow
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label17: TLabel
        Left = 12
        Top = 4
        Width = 114
        Height = 22
        Caption = 'PASSWORD'
      end
      object btPasswordClose: TBitBtn
        Left = 132
        Top = 4
        Width = 23
        Height = 22
        ModalResult = 3
        TabOrder = 0
        Glyph.Data = {
          DE010000424DDE01000000000000760000002800000024000000120000000100
          0400000000006801000000000000000000001000000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          333333333333333333333333000033338833333333333333333F333333333333
          0000333911833333983333333388F333333F3333000033391118333911833333
          38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
          911118111118333338F3338F833338F3000033333911111111833333338F3338
          3333F8330000333333911111183333333338F333333F83330000333333311111
          8333333333338F3333383333000033333339111183333333333338F333833333
          00003333339111118333333333333833338F3333000033333911181118333333
          33338333338F333300003333911183911183333333383338F338F33300003333
          9118333911183333338F33838F338F33000033333913333391113333338FF833
          38F338F300003333333333333919333333388333338FFF830000333333333333
          3333333333333333333888330000333333333333333333333333333333333333
          0000}
        NumGlyphs = 2
      end
    end
    object btOper: TBitBtn
      Left = 5
      Top = 31
      Width = 150
      Height = 41
      TabOrder = 1
      OnClick = btOperClick
      Glyph.Data = {
        76470000424D7647000000000000360000002800000097000000280000000100
        1800000000004047000012170000121700000000000000000000FFFFFF8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF00
        00008F8F8F8F8F8F808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        808F8F8F8F8F8F0000008F8F8FFFFFFFFFFFFF81818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFE8080808080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898980000000000000000000000000000009E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABAB0000000000000000000000000000000000000000000000
        00000000B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBD000000000000C0C0C0000000000000C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8000000000000CBCBCBCCCCCCCDCDCDCDCDCD0000
        00000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D600000000
        0000000000000000000000DBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0000000
        000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7000000000000E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797970000000000000000009A9A9A9B9B9B9C9C9C0000000000
        000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9
        B9BABABABBBBBBBCBCBC000000000000BFBFBFC0C0C0000000000000C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8000000000000CBCBCBCCCCCC
        CDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D40000
        00000000000000D8D8D8D9D9D9DADADA000000000000000000DDDDDDDEDEDEDF
        DFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6000000000000
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797970000000000009999999A9A9A9B9B9B
        9C9C9C9D9D9D0000000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABAB000000000000AE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000
        000000B9B9B9B9B9B9BABABABBBBBBBCBCBC000000000000BFBFBFC0C0C0C0C0
        C0000000000000C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7000000000000CA
        CACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4000000000000D7D7D7D8D8D8D9D9D9DADADADADADA0000000000
        00DDDDDDDEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6000000000000E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969600000000000098989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0A0A0A1A1A1A2A2A2
        000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        AB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBCBCBC000000000000
        BFBFBFC0C0C0C0C0C00000000000000000000000000000000000000000000000
        00000000000000CACACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6000000000000E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696960000
        000000009898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0
        A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABAB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBB00
        0000000000BEBEBEBFBFBFC0C0C0C0C0C0C1C1C1000000000000C4C4C4C5C5C5
        C6C6C6C6C6C6000000000000C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCD0000
        00000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000
        000000E2E2E2E3E3E3E4E4E4E5E5E5000000000000E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696960000000000009898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E000000000000A0A0A0A1A1A1A2A2A200000000000000000000000000000000
        0000000000A9A9A9AAAAAAABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B60000000000000000000000
        00000000000000000000BDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C100000000
        0000C4C4C4C5C5C5C6C6C6C6C6C6000000000000C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D40000000000
        00D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDF
        DFDFE0E0E0000000000000000000000000000000000000000000E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696960000000000009898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E000000000000A0A0A0A1A1A1A2A2A2000000000000A5A5
        A5A5A5A5A6A6A6A7A7A7000000000000AAAAAAABABABABABAB00000000000000
        0000000000000000000000000000000000B3B3B3B4B4B4B5B5B5B6B6B6000000
        000000B9B9B9B9B9B9BABABABBBBBB000000000000BEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2000000000000C5C5C5C6C6C6000000000000C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDB0000
        00000000DEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E500
        0000000000E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969600000000000098989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0A0A0A1A1A1A2A2A2
        000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABAB
        AB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBCBCBC000000000000
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2000000000000C5C5C5C6C6C60000000000
        00C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6000000000000E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696960000
        000000009898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0
        A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000
        000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBC
        BCBC000000000000BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3000000000000
        000000000000C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCD0000
        00000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000
        000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6000000000000E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797970000000000009999999A9A9A9B9B9B9C9C9C9D9D9D0000
        000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8000000000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9
        B9BABABABBBBBBBCBCBC000000000000BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3000000000000000000000000C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D40000
        00000000D7D7D7D8D8D8D9D9D9DADADADADADA000000000000DDDDDDDEDEDEDF
        DFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6000000000000
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797970000000000000000009A9A9A9B9B9B
        9C9C9C0000000000000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5
        A5A5A5A5A6A6A6A7A7A7000000000000AAAAAAABABABABABAB000000000000AE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000
        000000B9B9B9B9B9B9BABABABBBBBB000000000000BEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4000000000000000000D8D8D8D9D9D9DADADA0000000000000000
        00DDDDDDDEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E500
        0000000000E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989800
        00000000000000000000000000009E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        000000000000000000000000000000000000000000A9A9A9AAAAAAABABABABAB
        AB000000000000000000000000000000000000000000000000000000B4B4B4B5
        B5B5B6B6B6000000000000000000000000000000000000000000BDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
        C7C8C8C8C9C9C9CACACA00000000000000000000000000000000000000000000
        0000000000000000D4D4D4D4D4D4D5D5D5D6D6D6000000000000000000000000
        000000DBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E00000000000000000000000
        00000000000000000000E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFFFFFFFF818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFE8080
        808080808F8F8F0000008F8F8F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFF8F8F8F8F8F8F000000FFFFFF8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF000000}
    end
    object btMast: TBitBtn
      Tag = 2
      Left = 5
      Top = 73
      Width = 150
      Height = 41
      TabOrder = 2
      OnClick = btOperClick
      Glyph.Data = {
        76470000424D7647000000000000360000002800000097000000280000000100
        1800000000004047000012170000121700000000000000000000FFFFFF8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF00
        00008F8F8F8F8F8F808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        8080808080808080808080808080808080808080808080808080808080808080
        808F8F8F8F8F8F0000008F8F8FFFFFFFFFFFFF81818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFE8080808080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1000000000000A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000
        000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4000000000000B7B7B7B8B8B8B9B9B900000000000000000000
        0000000000000000BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000
        000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB0000000000000000000000
        00000000000000000000000000000000D4D4D4D4D4D4D5D5D5000000000000D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDC000000000000DFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8000000000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4000000000000B7B7B7B8B8B80000000000
        00BABABABBBBBBBCBCBCBDBDBD000000000000C0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000
        000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5000000000000D8D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A4A5A5
        A5000000000000A7A7A7A8A8A8000000000000ABABABABABABACACAC00000000
        0000AFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3000000000000B6B6B6B7B7B7
        000000000000B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE000000000000C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADADADADADBDBDB0000
        00000000DEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000
        000000A4A4A4A5A5A5000000000000A7A7A7A8A8A8000000000000ABABABABAB
        ABACACAC00000000000000000000000000000000000000000000000000000000
        0000B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        000000000000C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADA
        DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1000000000000A4A4A4000000000000000000000000A8A8A8000000
        000000ABABABABABABACACACADADAD000000000000B0B0B0B1B1B1B2B2B2B2B2
        B2000000000000B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBE000000000000C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000
        000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8
        D8D8D9D9D9DADADADADADA000000000000DDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A400000000000000000000
        0000A8A8A8000000000000ABABABABABABACACACADADAD000000000000B0B0B0
        B1B1B1B2B2B2B2B2B2000000000000B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBD000000000000C0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000
        000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5000000000000000000000000000000000000000000DCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A10000000000000000000000
        00A5A5A5A6A6A6000000000000000000000000ABABABABABABACACACADADADAE
        AEAE000000000000B1B1B1B2B2B2000000000000B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9000000000000000000000000000000000000BFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCB000000000000000000000000000000000000000000000000D3D3D3
        D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADADADADA0000000000
        00DDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000
        000000000000000000A5A5A5A6A6A6000000000000000000000000ABABABABAB
        ABACACACADADADAEAEAE000000000000B1B1B1B2B2B2000000000000B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8000000000000BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADA
        DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1000000000000000000A5A5A5A5A5A5A6A6A6A7A7A7000000000000
        000000ABABABABABABACACACADADADAEAEAEAFAFAF0000000000000000000000
        00B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7000000000000B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000
        000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8
        D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000000000A5A5A5A5A5A5A6A6A6A7
        A7A7000000000000000000ABABABABABABACACACADADADAEAEAEAFAFAF000000
        000000000000000000B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7000000000000B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBE000000000000C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000
        000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5000000000000D8D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0000000000000B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8000000000000BABABABBBBBBBCBCBCBDBDBD000000000000C0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADADADADA0000000000
        00DDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000
        000000A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0000000000000B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9000000000000000000000000000000000000
        BFBFBFC0C0C0C0C0C00000000000000000000000000000000000000000000000
        00000000000000CACACACBCBCB00000000000000000000000000000000000000
        0000000000000000D4D4D4D4D4D4D5D5D5000000000000000000000000000000
        000000000000DCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
        FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
        84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
        8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
        979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
        A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
        AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
        B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
        BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
        C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
        CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
        D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
        E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
        EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
        F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
        FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
        828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
        8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
        9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
        9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
        A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
        B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
        B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
        C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
        CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
        D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
        DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
        E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
        F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
        FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
        8080808181818282828383838383838484848585858686868787878888888989
        898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
        92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
        9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
        A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
        AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
        B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
        C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
        CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
        D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
        DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
        E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
        EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
        F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
        00008F8F8FFFFFFFFFFFFF818181828282838383838383848484858585868686
        8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
        9091919191919192929293939394949495959596969697979797979798989899
        99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
        A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
        ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
        B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
        BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
        C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
        D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
        DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
        E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
        EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
        F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFE8080
        808080808F8F8F0000008F8F8F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFF8F8F8F8F8F8F000000FFFFFF8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
        8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF000000}
    end
  end
  object tmShowImg: TTimer
    Interval = 50
    OnTimer = tmShowImgTimer
    Left = 200
    Top = 840
  end
  object tmUpdate: TTimer
    Interval = 30
    OnTimer = tmUpdateTimer
    Left = 16
    Top = 816
  end
end
