object FrmSingle: TFrmSingle
  Left = 447
  Top = 168
  Align = alClient
  BorderStyle = bsSingle
  Caption = 'FrmSingle'
  ClientHeight = 770
  ClientWidth = 1284
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 680
    Top = -12
    Width = 721
    Height = 993
    Caption = 'Graph'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Panel1: TPanel
      Left = 18
      Top = 511
      Width = 640
      Height = 480
      Caption = 'Panel2'
      TabOrder = 0
    end
    object Panel2: TPanel
      Left = 18
      Top = 31
      Width = 640
      Height = 480
      Caption = 'Panel1'
      TabOrder = 1
    end
  end
  object GroupBox3: TGroupBox
    Left = 17
    Top = 80
    Width = 640
    Height = 250
    Caption = 'Tilt'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object sgTilt: TStringGrid
      Left = 2
      Top = 39
      Width = 636
      Height = 209
      Align = alClient
      RowCount = 4
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 18
    Top = 416
    Width = 640
    Height = 250
    Caption = 'Drive Test'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object sgDriveTest: TStringGrid
      Left = 2
      Top = 39
      Width = 636
      Height = 209
      Align = alClient
      RowCount = 7
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 1248
    Top = 16
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    Left = 1040
    Top = 8
  end
  object Timer2: TTimer
    Enabled = False
    Interval = 100
    Left = 1176
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 256
    object miOption: TMenuItem
      Caption = 'Option'
      object ChangeTheModel1: TMenuItem
        Caption = 'Change The Model'
        OnClick = ChangeTheModel1Click
      end
      object LoadTestModel1: TMenuItem
        Caption = 'Load Test Model'
      end
      object Close1: TMenuItem
        Caption = 'Close'
        OnClick = Close1Click
      end
    end
  end
end
