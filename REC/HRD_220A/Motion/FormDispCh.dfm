object FrmDispCh: TFrmDispCh
  Left = 341
  Top = 223
  BorderStyle = bsDialog
  Caption = '1Ch Setting'
  ClientHeight = 152
  ClientWidth = 317
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbAmount: TLabel
    Left = 208
    Top = 16
    Width = 57
    Height = 13
    AutoSize = False
    Caption = '%'
  end
  object pnChSetting: TPanel
    Left = 19
    Top = 32
    Width = 283
    Height = 49
    TabOrder = 0
    object Label46: TLabel
      Left = 2
      Top = 4
      Width = 198
      Height = 20
      AutoSize = False
      Caption = 'Dispensor Pressure                '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label47: TLabel
      Left = 2
      Top = 25
      Width = 198
      Height = 20
      AutoSize = False
      Caption = 'Dispensor Vacuum                '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label48: TLabel
      Left = 243
      Top = 7
      Width = 28
      Height = 16
      Caption = 'kPa'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label49: TLabel
      Left = 239
      Top = 28
      Width = 32
      Height = 16
      Caption = ' kPa'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edDspPrsPres: TEdit
      Left = 165
      Top = 4
      Width = 69
      Height = 20
      AutoSize = False
      Color = clMedGray
      ImeName = 'Microsoft IME 2010'
      TabOrder = 0
      Text = '0'
    end
    object edDspVacPres: TEdit
      Left = 165
      Top = 25
      Width = 69
      Height = 20
      AutoSize = False
      ImeName = 'Microsoft IME 2010'
      TabOrder = 1
      Text = '0'
    end
  end
  object btSetDspData: TButton
    Left = 168
    Top = 96
    Width = 137
    Height = 33
    Caption = 'Set Dispensor'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btSetDspDataClick
  end
  object btSigmaMode: TBitBtn
    Left = 24
    Top = 8
    Width = 83
    Height = 25
    Caption = 'SIGMA ON'
    TabOrder = 2
    OnClick = btSigmaModeClick
  end
  object btCheckAmount: TButton
    Left = 112
    Top = 8
    Width = 83
    Height = 25
    Caption = 'CHECK EPOXY'
    TabOrder = 3
    OnClick = btCheckAmountClick
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 32
    Top = 104
  end
end
