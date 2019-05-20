object FrmMstPkg: TFrmMstPkg
  Left = 486
  Top = 222
  Width = 579
  Height = 169
  Caption = 'FrmMstPkg'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 563
    Height = 131
    Align = alClient
    TabOrder = 0
    object Image1: TImage
      Left = 88
      Top = 296
      Width = 105
      Height = 105
      Visible = False
    end
    object Panel5: TPanel
      Left = 1
      Top = 1
      Width = 561
      Height = 38
      Align = alTop
      BevelOuter = bvNone
      Caption = 'MASTER PAKEAGE INSPECTION'
      Color = clSkyBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object btMstInspc: TBitBtn
      Left = 2
      Top = 39
      Width = 560
      Height = 90
      Caption = 'INSPECTION START'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btMstInspcClick
    end
  end
end
