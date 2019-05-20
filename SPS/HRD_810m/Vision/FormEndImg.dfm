object FrmEndImg: TFrmEndImg
  Left = 117
  Top = 234
  BorderStyle = bsDialog
  Caption = 'FrmEndImg'
  ClientHeight = 599
  ClientWidth = 914
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnDraw: TPanel
    Left = 0
    Top = 0
    Width = 800
    Height = 599
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 800
    Top = 0
    Width = 114
    Height = 599
    Align = alClient
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 40
      Width = 70
      Height = 13
      Caption = 'Exposure Time'
    end
    object edExps: TEdit
      Left = 8
      Top = 56
      Width = 87
      Height = 21
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      TabOrder = 0
      Text = '1000'
    end
    object Button1: TButton
      Left = 16
      Top = 88
      Width = 75
      Height = 25
      Caption = 'SET'
      TabOrder = 1
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 16
      Top = 113
      Width = 75
      Height = 25
      Caption = 'GRAB'
      TabOrder = 2
      OnClick = Button2Click
    end
    object btSaveAs: TButton
      Left = 16
      Top = 138
      Width = 75
      Height = 25
      Caption = 'SAVE AS'
      TabOrder = 3
      OnClick = btSaveAsClick
    end
    object Button3: TButton
      Left = 16
      Top = 168
      Width = 75
      Height = 25
      Caption = 'TriggerMode'
      TabOrder = 4
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 16
      Top = 560
      Width = 75
      Height = 25
      Caption = 'LiveMode'
      TabOrder = 5
      OnClick = Button4Click
    end
  end
  object tmClose: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = tmCloseTimer
    Left = 744
    Top = 8
  end
  object SavePictureDialog1: TSavePictureDialog
    Filter = 
      'Bitmaps (*.bmp)|*.bmp|JPEG Image File (*.jpg)|*.jpg|JPEG Image F' +
      'ile (*.jpeg)|*.jpeg'
    Left = 72
    Top = 32
  end
end
