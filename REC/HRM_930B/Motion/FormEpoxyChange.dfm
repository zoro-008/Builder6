object FrmEpoxyChange: TFrmEpoxyChange
  Left = 818
  Top = 430
  BorderStyle = bsNone
  Caption = 'FrmEpoxyChange'
  ClientHeight = 206
  ClientWidth = 879
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 879
    Height = 206
    Align = alClient
    Color = clWhite
    TabOrder = 0
    object Label1: TLabel
      Left = 139
      Top = 53
      Width = 166
      Height = 32
      Caption = 'Epoxy Name'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 139
      Top = 94
      Width = 186
      Height = 32
      Caption = 'Epoxy Lot No.'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 877
      Height = 41
      Align = alTop
      Caption = 'Epoxy Change'
      Color = clSkyBlue
      Font.Charset = ARABIC_CHARSET
      Font.Color = clCream
      Font.Height = -27
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object btEpoxyOK: TButton
      Left = 269
      Top = 144
      Width = 161
      Height = 41
      Caption = 'OK'
      Font.Charset = ARABIC_CHARSET
      Font.Color = clWindowText
      Font.Height = -33
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btEpoxyOKClick
    end
    object btEpoxyCancel: TButton
      Left = 453
      Top = 143
      Width = 161
      Height = 41
      Caption = 'CANCLE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -33
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = btEpoxyCancelClick
    end
    object edDspEpoxy: TEdit
      Left = 376
      Top = 54
      Width = 401
      Height = 30
      Color = cl3DLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Times New Roman'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 3
    end
    object edEpoxyLotNo: TEdit
      Left = 376
      Top = 95
      Width = 401
      Height = 30
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Times New Roman'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 4
    end
  end
end
