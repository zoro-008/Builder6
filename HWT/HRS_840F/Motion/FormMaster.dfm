object FrmMaster: TFrmMaster
  Left = 0
  Top = 231
  AutoScroll = False
  Caption = 'FrmMaster'
  ClientHeight = 434
  ClientWidth = 1024
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
    Width = 1024
    Height = 434
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 1016
        Height = 406
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object Panel5: TPanel
          Left = 0
          Top = 0
          Width = 1016
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
          Width = 849
          Height = 366
          Caption = 'Hello World'
          TabOrder = 1
          object lbMan: TLabel
            Left = 224
            Top = 420
            Width = 29
            Height = 13
            Caption = 'lbMan'
          end
          object sgCycleStep: TStringGrid
            Left = 278
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
            Left = 163
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
            Left = 163
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
            Left = 163
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
            Left = 163
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
            Left = 160
            Top = 184
            Width = 105
            Height = 25
            Caption = 'ALL CHECK'
            TabOrder = 5
            OnClick = btAllCheckClick
          end
        end
        object sgTime: TStringGrid
          Left = 872
          Top = 64
          Width = 169
          Height = 337
          ColCount = 2
          RowCount = 6
          FixedRows = 0
          TabOrder = 2
          RowHeights = (
            24
            24
            24
            24
            24
            24)
        end
        object cbPartSel: TComboBox
          Left = 872
          Top = 40
          Width = 169
          Height = 21
          ImeName = 'Microsoft Office IME 2007'
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 3
          Text = 'Loader'
          OnChange = cbPartSelChange
          Items.Strings = (
            'Loader')
        end
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tmUpdateTimer
    Left = 208
  end
  object dgFilePath: TOpenDialog
    Left = 400
    Top = 560
  end
end
