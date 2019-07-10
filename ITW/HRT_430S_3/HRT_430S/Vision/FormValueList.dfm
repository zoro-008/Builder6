object FrmValueList: TFrmValueList
  Left = 412
  Top = 119
  Width = 1050
  Height = 875
  Caption = 'Value List Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnProperty: TPanel
    Left = 705
    Top = 0
    Width = 329
    Height = 837
    Align = alRight
    TabOrder = 0
    object Label1: TLabel
      Left = 24
      Top = 52
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label2: TLabel
      Left = 24
      Top = 76
      Width = 44
      Height = 13
      Caption = 'Comment'
    end
    object Label3: TLabel
      Left = 24
      Top = 100
      Width = 24
      Height = 13
      Caption = 'Type'
    end
    object Label4: TLabel
      Left = 24
      Top = 124
      Width = 44
      Height = 13
      Caption = 'MinValue'
      Visible = False
    end
    object Label5: TLabel
      Left = 24
      Top = 148
      Width = 47
      Height = 13
      Caption = 'MaxValue'
      Visible = False
    end
    object DeleteAll: TButton
      Left = 246
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Delete All'
      TabOrder = 0
      OnClick = DeleteAllClick
    end
    object Add: TButton
      Left = 6
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 1
      OnClick = AddClick
    end
    object Delete: TButton
      Left = 166
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = DeleteClick
    end
    object Insert: TButton
      Left = 86
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Insert'
      TabOrder = 3
      OnClick = InsertClick
    end
    object edName: TEdit
      Left = 88
      Top = 48
      Width = 224
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 4
      Text = 'edName'
    end
    object edComment: TEdit
      Left = 88
      Top = 72
      Width = 224
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 5
      Text = 'edCommand'
    end
    object edMinVal: TEdit
      Left = 88
      Top = 120
      Width = 105
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 6
      Text = 'Edit1'
      Visible = False
    end
    object edMaxVal: TEdit
      Left = 88
      Top = 144
      Width = 105
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 7
      Text = 'Edit1'
      Visible = False
    end
    object cbType: TComboBox
      Left = 88
      Top = 96
      Width = 106
      Height = 21
      ImeName = 'Microsoft Office IME 2007'
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 8
      Text = 'String'
      Items.Strings = (
        'String'
        'Address'
        'Double'
        'Int'
        'Enum'
        'Bool')
    end
    object Button1: TButton
      Left = 240
      Top = 136
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 9
      OnClick = Button1Click
    end
  end
  object sgValueList: TStringGrid
    Left = 0
    Top = 0
    Width = 705
    Height = 837
    Align = alClient
    ColCount = 7
    DefaultRowHeight = 18
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
    TabOrder = 1
    OnClick = sgValueListClick
    ColWidths = (
      43
      111
      210
      51
      50
      50
      176)
  end
end
