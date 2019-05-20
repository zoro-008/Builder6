object FrmSPC: TFrmSPC
  Left = -3
  Top = 12
  BorderStyle = bsNone
  Caption = 'FrmSPC'
  ClientHeight = 920
  ClientWidth = 1280
  Color = clBtnFace
  Constraints.MaxHeight = 1024
  Constraints.MaxWidth = 1296
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1280
    Height = 920
    ActivePage = TabSheet2
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabIndex = 1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'WORK INFO'
      object Panel2: TPanel
        Left = 1
        Top = 0
        Width = 200
        Height = 754
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 0
        object Panel3: TPanel
          Left = 1
          Top = 1
          Width = 198
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
        object sgLotDate: TStringGrid
          Left = 1
          Top = 26
          Width = 198
          Height = 727
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
          OnClick = sgLotDateClick
          ColWidths = (
            30
            258)
        end
      end
      object Panel5: TPanel
        Left = 202
        Top = 0
        Width = 1071
        Height = 754
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 1
        object Panel6: TPanel
          Left = 1
          Top = 1
          Width = 1069
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'WORK INFO'
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
          Width = 1069
          Height = 727
          Align = alClient
          ColCount = 1
          DefaultColWidth = 30
          DefaultRowHeight = 25
          FixedCols = 0
          RowCount = 31
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
          ParentFont = False
          TabOrder = 1
          OnClick = sgLotInfoClick
          ColWidths = (
            854)
        end
      end
      object Panel19: TPanel
        Left = 83
        Top = 745
        Width = 130
        Height = 293
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 2
        Visible = False
        object Panel20: TPanel
          Left = 1
          Top = 1
          Width = 128
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
        object sgLotSlot: TStringGrid
          Left = 1
          Top = 26
          Width = 128
          Height = 266
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
          OnClick = sgLotSlotClick
          ColWidths = (
            30
            258)
        end
      end
      object Panel21: TPanel
        Left = 5
        Top = 770
        Width = 1011
        Height = 293
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 3
        Visible = False
        object Panel22: TPanel
          Left = 1
          Top = 1
          Width = 1009
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'STRIP MAP'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnLotMap: TPanel
          Left = 1
          Top = 26
          Width = 1009
          Height = 266
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object sgLotMap: TStringGrid
            Left = 0
            Top = 0
            Width = 1009
            Height = 266
            Align = alClient
            ColCount = 10
            DefaultColWidth = 15
            DefaultRowHeight = 15
            RowCount = 10
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -7
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
            ParentFont = False
            TabOrder = 0
            OnDrawCell = sgLotMapDrawCell
          end
        end
      end
      object Panel15: TPanel
        Left = 25
        Top = 737
        Width = 130
        Height = 293
        BevelInner = bvRaised
        BevelOuter = bvNone
        TabOrder = 4
        Visible = False
        object Panel16: TPanel
          Left = 1
          Top = 1
          Width = 128
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
        object sgLotMgz: TStringGrid
          Left = 1
          Top = 26
          Width = 128
          Height = 266
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
          OnClick = sgLotMgzClick
          ColWidths = (
            30
            258)
          RowHeights = (
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25
            25)
        end
      end
      object btSaveCsv: TBitBtn
        Left = 1072
        Top = 759
        Width = 177
        Height = 65
        Caption = 'SAVE TO CSV'
        TabOrder = 5
        OnClick = btSaveCsvClick
      end
      object btSaveXls: TBitBtn
        Left = 880
        Top = 759
        Width = 177
        Height = 65
        Caption = 'SAVE TO XLS'
        TabOrder = 6
        OnClick = btSaveXlsClick
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
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 454
        Top = 754
        Width = 37
        Height = 23
        Caption = 'Until'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Arial'
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
          Caption = 'ERROR DATA'
          Color = clSkyBlue
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object sgErrData: TStringGrid
          Left = 1
          Top = 36
          Width = 1270
          Height = 692
          Align = alClient
          ColCount = 6
          DefaultColWidth = 30
          DefaultRowHeight = 25
          RowCount = 27
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
          ParentFont = False
          TabOrder = 1
          ColWidths = (
            30
            57
            254
            166
            532
            192)
        end
      end
      object btErrDataView: TButton
        Left = 651
        Top = 768
        Width = 175
        Height = 43
        Caption = 'Err Data View'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btErrDataViewClick
      end
      object edErrSttTime: TDateTimePicker
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
      object edErrEndTime: TDateTimePicker
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
      object rgNoTime: TRadioGroup
        Left = 848
        Top = 752
        Width = 137
        Height = 65
        Caption = 'Sorting Option'
        ItemIndex = 1
        Items.Strings = (
          'Err No'
          'Err Time')
        TabOrder = 4
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'MTBA'
      ImageIndex = 3
      TabVisible = False
      object Label16: TLabel
        Left = 874
        Top = 786
        Width = 277
        Height = 23
        Caption = 'Unit : Sec (RunTime / ErrCount)'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Arial'
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
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
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
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
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
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 500
    OnTimer = tmUpdateTimer
    Left = 1216
    Top = 112
  end
  object sdCSV: TSaveDialog
    DefaultExt = '.csv'
    Filter = 'CSV|*.csv'
    Left = 1120
    Top = 800
  end
  object sdXLS: TSaveDialog
    DefaultExt = '.xls'
    Filter = 'XLS|*.xls'
    Left = 1160
    Top = 800
  end
end
