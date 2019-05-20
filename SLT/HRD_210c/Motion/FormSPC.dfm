object FrmSPC: TFrmSPC
  Left = 142
  Top = 8
  BorderStyle = bsNone
  Caption = 'FrmSPC'
  ClientHeight = 870
  ClientWidth = 1280
  Color = clBtnFace
  Constraints.MaxHeight = 1024
  Constraints.MaxWidth = 1288
  Constraints.MinHeight = 897
  Constraints.MinWidth = 1288
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pcSPC: TPageControl
    Left = 0
    Top = 0
    Width = 1280
    Height = 870
    ActivePage = TabSheet1
    Align = alClient
    Font.Charset = ARABIC_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'LOT INFO'
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 141
        Height = 370
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 0
        object Panel3: TPanel
          Left = 1
          Top = 1
          Width = 139
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'DATE'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgDate: TStringGrid
          Left = 1
          Top = 26
          Width = 139
          Height = 343
          Align = alClient
          ColCount = 2
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 31
          FixedRows = 0
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgDateClick
          ColWidths = (
            30
            258)
        end
      end
      object Panel5: TPanel
        Left = 1
        Top = 370
        Width = 1271
        Height = 375
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 1
        object Panel6: TPanel
          Left = 1
          Top = 1
          Width = 1269
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'INFO'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgLotInfo: TStringGrid
          Left = 1
          Top = 26
          Width = 1269
          Height = 348
          Align = alClient
          ColCount = 33
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 31
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgLotInfoClick
          ColWidths = (
            30
            103
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30)
        end
      end
      object Panel21: TPanel
        Left = 248
        Top = 728
        Width = 1031
        Height = 200
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 2
        Visible = False
        object Panel22: TPanel
          Left = 1
          Top = 1
          Width = 1029
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'MAP'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgMap: TStringGrid
          Left = 9
          Top = 34
          Width = 1263
          Height = 228
          ColCount = 9
          DefaultColWidth = 139
          DefaultRowHeight = 44
          FixedCols = 0
          FixedRows = 0
          TabOrder = 1
          Visible = False
        end
        object pnMap: TPanel
          Left = 1
          Top = 26
          Width = 1029
          Height = 173
          Align = alClient
          TabOrder = 2
        end
      end
      object Button1: TButton
        Left = 240
        Top = 728
        Width = 75
        Height = 25
        Caption = 'Button1'
        TabOrder = 3
        Visible = False
        OnClick = Button1Click
      end
      object Panel19: TPanel
        Left = 120
        Top = 728
        Width = 120
        Height = 200
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 4
        Visible = False
        object Panel20: TPanel
          Left = 1
          Top = 1
          Width = 118
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'SLOT NO'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgSlotNo: TStringGrid
          Left = 1
          Top = 26
          Width = 118
          Height = 173
          Align = alClient
          ColCount = 2
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 31
          FixedRows = 0
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgSlotNoClick
          ColWidths = (
            30
            258)
        end
      end
      object Panel15: TPanel
        Left = 0
        Top = 728
        Width = 120
        Height = 200
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 5
        Visible = False
        object Panel16: TPanel
          Left = 1
          Top = 1
          Width = 118
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'MGZ NO'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgMgzNo: TStringGrid
          Left = 1
          Top = 26
          Width = 118
          Height = 173
          Align = alClient
          ColCount = 2
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 31
          FixedRows = 0
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgMgzNoClick
          ColWidths = (
            30
            258)
        end
      end
      object Panel1: TPanel
        Left = 141
        Top = 0
        Width = 1131
        Height = 369
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 6
        object Panel4: TPanel
          Left = 1
          Top = 1
          Width = 1129
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'LOT NAME'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgLot: TStringGrid
          Left = 1
          Top = 26
          Width = 1129
          Height = 342
          Align = alClient
          ColCount = 3
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 27
          FixedRows = 0
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgLotClick
          ColWidths = (
            30
            775
            235)
        end
      end
      object Panel17: TPanel
        Left = 0
        Top = 761
        Width = 1272
        Height = 77
        TabOrder = 7
        object Label10: TLabel
          Left = 250
          Top = 12
          Width = 46
          Height = 23
          Caption = 'From'
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label11: TLabel
          Left = 454
          Top = 12
          Width = 37
          Height = 23
          Caption = 'Until'
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object btLotInfoView: TButton
          Left = 659
          Top = 24
          Width = 175
          Height = 43
          Caption = 'Lot Info View'
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btLotInfoViewClick
        end
        object edU_LotInfo: TDateTimePicker
          Left = 454
          Top = 41
          Width = 186
          Height = 25
          CalAlignment = dtaLeft
          Date = 40757.5659816204
          Time = 40757.5659816204
          DateFormat = dfShort
          DateMode = dmComboBox
          ImeName = 'Microsoft Office IME 2007'
          Kind = dtkDate
          ParseInput = False
          TabOrder = 1
        end
        object btLotInfoSave: TButton
          Left = 843
          Top = 24
          Width = 175
          Height = 43
          Caption = 'Lot Info Save'
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btLotInfoViewClick
        end
        object edF_LotInfo: TDateTimePicker
          Left = 249
          Top = 41
          Width = 186
          Height = 25
          CalAlignment = dtaLeft
          Date = 40757.5659816204
          Time = 40757.5659816204
          DateFormat = dfShort
          DateMode = dmComboBox
          ImeName = 'Microsoft Office IME 2007'
          Kind = dtkDate
          ParseInput = False
          TabOrder = 3
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'LOT DAY INFO'
      ImageIndex = 4
      object Label14: TLabel
        Left = 254
        Top = 753
        Width = 46
        Height = 23
        Caption = 'From'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label15: TLabel
        Left = 454
        Top = 754
        Width = 37
        Height = 23
        Caption = 'Until'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Panel11: TPanel
        Left = 0
        Top = 0
        Width = 1272
        Height = 350
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 0
        object Panel12: TPanel
          Left = 1
          Top = 1
          Width = 1270
          Height = 35
          Align = alTop
          BevelOuter = bvNone
          Caption = 'DAY'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgLotDayName: TStringGrid
          Left = 1
          Top = 36
          Width = 1270
          Height = 313
          Align = alClient
          ColCount = 3
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 27
          FixedRows = 0
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgLotDayNameClick
          ColWidths = (
            30
            775
            235)
        end
      end
      object Panel13: TPanel
        Left = 0
        Top = 352
        Width = 1272
        Height = 389
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 1
        object Panel14: TPanel
          Left = 1
          Top = 1
          Width = 1270
          Height = 35
          Align = alTop
          BevelOuter = bvNone
          Caption = 'INFO'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgLotDayInfo: TStringGrid
          Left = 1
          Top = 36
          Width = 1270
          Height = 352
          Align = alClient
          ColCount = 33
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 31
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          ColWidths = (
            30
            103
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30
            30)
        end
      end
      object btLotDayView: TButton
        Left = 651
        Top = 768
        Width = 175
        Height = 43
        Caption = 'Lot Day Info View'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = btLotInfoViewClick
      end
      object edF_LotDayInfo: TDateTimePicker
        Left = 254
        Top = 787
        Width = 186
        Height = 25
        CalAlignment = dtaLeft
        Date = 40757.5663197815
        Time = 40757.5663197815
        DateFormat = dfShort
        DateMode = dmComboBox
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        TabOrder = 3
      end
      object edU_LotDayInfo: TDateTimePicker
        Left = 454
        Top = 787
        Width = 186
        Height = 25
        CalAlignment = dtaLeft
        Date = 40757.5663197815
        Time = 40757.5663197815
        DateFormat = dfShort
        DateMode = dmComboBox
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        TabOrder = 4
      end
      object btLotDaySave: TButton
        Left = 835
        Top = 768
        Width = 175
        Height = 43
        Caption = 'Lot Day Info Save'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnClick = btLotInfoViewClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'ERR HISTORY'
      ImageIndex = 1
      object Label5: TLabel
        Left = 254
        Top = 753
        Width = 46
        Height = 23
        Caption = 'From'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 454
        Top = 754
        Width = 37
        Height = 23
        Caption = 'Until'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Panel7: TPanel
        Left = 0
        Top = 0
        Width = 1272
        Height = 729
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 0
        object Panel8: TPanel
          Left = 1
          Top = 1
          Width = 1270
          Height = 35
          Align = alTop
          BevelOuter = bvNone
          Caption = 'ERR DATA'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgErrHistory: TStringGrid
          Left = 1
          Top = 36
          Width = 1270
          Height = 692
          Align = alClient
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 27
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          ColWidths = (
            30
            92
            709
            205
            199)
        end
      end
      object btErrDataView: TButton
        Left = 651
        Top = 768
        Width = 175
        Height = 43
        Caption = 'Err Data View'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btLotInfoViewClick
      end
      object edFDataTime: TDateTimePicker
        Left = 254
        Top = 787
        Width = 186
        Height = 25
        CalAlignment = dtaLeft
        Date = 40757.5663197815
        Time = 40757.5663197815
        DateFormat = dfShort
        DateMode = dmComboBox
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        TabOrder = 2
      end
      object edUDataTime: TDateTimePicker
        Left = 454
        Top = 787
        Width = 186
        Height = 25
        CalAlignment = dtaLeft
        Date = 40757.5663197815
        Time = 40757.5663197815
        DateFormat = dfShort
        DateMode = dmComboBox
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        TabOrder = 3
      end
      object btErrDataSave: TButton
        Left = 835
        Top = 768
        Width = 175
        Height = 43
        Caption = 'Err Data Save'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = btLotInfoViewClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'ERR COUNT'
      ImageIndex = 2
      object Label8: TLabel
        Left = 250
        Top = 752
        Width = 46
        Height = 23
        Caption = 'From'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 450
        Top = 752
        Width = 37
        Height = 23
        Caption = 'Until'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Panel9: TPanel
        Left = -1
        Top = -2
        Width = 1273
        Height = 737
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 0
        object Panel10: TPanel
          Left = 1
          Top = 1
          Width = 1271
          Height = 35
          Align = alTop
          BevelOuter = bvNone
          Caption = 'ERR COUNT'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgErrCnt: TStringGrid
          Left = 1
          Top = 36
          Width = 1271
          Height = 700
          Align = alClient
          ColCount = 4
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 27
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          ColWidths = (
            30
            209
            828
            173)
        end
      end
      object btErrCntView: TButton
        Left = 655
        Top = 765
        Width = 175
        Height = 40
        Caption = 'Err Count View'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btLotInfoViewClick
      end
      object edF_ErrCnt: TDateTimePicker
        Left = 250
        Top = 784
        Width = 186
        Height = 25
        CalAlignment = dtaLeft
        Date = 40757.5663197815
        Time = 40757.5663197815
        DateFormat = dfShort
        DateMode = dmComboBox
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        TabOrder = 2
      end
      object edU_ErrCnt: TDateTimePicker
        Left = 450
        Top = 784
        Width = 186
        Height = 25
        CalAlignment = dtaLeft
        Date = 40757.5663197815
        Time = 40757.5663197815
        DateFormat = dfShort
        DateMode = dmComboBox
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        TabOrder = 3
      end
      object btErrCntSave: TButton
        Left = 839
        Top = 765
        Width = 175
        Height = 40
        Caption = 'Err Count Save'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = btLotInfoViewClick
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'MTBA'
      ImageIndex = 3
      TabVisible = False
      object Label16: TLabel
        Left = 874
        Top = 786
        Width = 278
        Height = 23
        Caption = 'Unit : Sec (RunTime / ErrCount)'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edMtbaDT: TDateTimePicker
        Left = 395
        Top = 785
        Width = 113
        Height = 30
        CalAlignment = dtaLeft
        Date = 40758.545572325
        Format = 'yyyy-MM'
        Time = 40758.545572325
        DateFormat = dfShort
        DateMode = dmComboBox
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = 'Microsoft Office IME 2007'
        Kind = dtkDate
        ParseInput = False
        ParentFont = False
        TabOrder = 0
      end
      object btMtbaView: TButton
        Left = 523
        Top = 783
        Width = 167
        Height = 32
        Caption = 'MTBA VIEW'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btLotInfoViewClick
      end
      object ChatMtba: TChart
        Left = 0
        Top = 0
        Width = 1272
        Height = 761
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clBlue
        Title.Font.Height = -16
        Title.Font.Name = 'Arial'
        Title.Font.Style = []
        Title.Text.Strings = (
          'MTBA')
        TabOrder = 2
        object S1_Mtba: TBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = True
          SeriesColor = clRed
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 0.99999998430675
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 0.99999998430675
          YValues.Order = loNone
        end
      end
      object btMtbaSave: TButton
        Left = 701
        Top = 783
        Width = 167
        Height = 32
        Caption = 'MTBA SAVE'
        Font.Charset = ARABIC_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = btLotInfoViewClick
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 300
    OnTimer = tmUpdateTimer
    Left = 24
    Top = 816
  end
  object sdSPC: TSaveDialog
    DefaultExt = 'csv'
    FileName = 'Map'
    Filter = '*.csv|*.csv'
    InitialDir = 'd:\'
    Left = 1232
    Top = 824
  end
end
