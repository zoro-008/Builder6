object FrmPixel: TFrmPixel
  Left = 434
  Top = 244
  Width = 565
  Height = 509
  BorderStyle = bsSizeToolWin
  Caption = 'PixelValue'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PPxView: TPanel
    Left = 41
    Top = 54
    Width = 317
    Height = 324
    BevelOuter = bvNone
    Color = 5395026
    TabOrder = 0
    OnMouseDown = PPxViewMouseDown
    OnMouseMove = PPxViewMouseMove
    OnMouseUp = PPxViewMouseUp
    OnResize = PPxViewResize
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 557
    Height = 33
    Align = alTop
    Color = 13619151
    TabOrder = 1
    object Label1: TLabel
      Left = 7
      Top = 10
      Width = 48
      Height = 13
      Caption = 'AvrRange'
    end
    object edAvrRange: TEdit
      Left = 59
      Top = 7
      Width = 20
      Height = 21
      ImeName = 'Microsoft IME 2003'
      TabOrder = 0
      Text = '1'
      OnChange = edAvrRangeChange
    end
    object cbRow: TCheckBox
      Left = 91
      Top = 7
      Width = 41
      Height = 20
      Caption = 'Raw'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = cbRowClick
    end
    object cbAvr: TCheckBox
      Left = 137
      Top = 7
      Width = 40
      Height = 20
      Caption = 'Avr'
      TabOrder = 2
      OnClick = cbRowClick
    end
    object cbDf1: TCheckBox
      Left = 176
      Top = 7
      Width = 40
      Height = 20
      Caption = 'Dif1'
      Checked = True
      State = cbChecked
      TabOrder = 3
      OnClick = cbRowClick
    end
    object cbDf2: TCheckBox
      Left = 215
      Top = 7
      Width = 40
      Height = 20
      Caption = 'Dif2'
      TabOrder = 4
      OnClick = cbRowClick
    end
    object cbHorScale: TCheckBox
      Left = 293
      Top = 7
      Width = 41
      Height = 20
      Caption = 'Hor'
      TabOrder = 5
      OnClick = cbRowClick
    end
    object cbVerScale: TCheckBox
      Left = 254
      Top = 7
      Width = 40
      Height = 20
      Caption = 'Ver'
      TabOrder = 6
      OnClick = cbRowClick
    end
  end
  object tmUpdate: TTimer
    Interval = 10
    OnTimer = tmUpdateTimer
    Left = 344
  end
end
