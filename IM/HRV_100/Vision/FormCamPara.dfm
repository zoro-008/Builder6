object FrmCamPara: TFrmCamPara
  Left = 946
  Top = 291
  Width = 361
  Height = 405
  Caption = 'FrmCamPara'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnBase: TPanel
    Left = 0
    Top = 0
    Width = 353
    Height = 330
    Align = alClient
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 330
    Width = 353
    Height = 41
    Align = alBottom
    TabOrder = 1
    object btSave: TButton
      Left = 32
      Top = 8
      Width = 75
      Height = 25
      Caption = 'SAVE'
      TabOrder = 0
      OnClick = btSaveClick
    end
  end
end
