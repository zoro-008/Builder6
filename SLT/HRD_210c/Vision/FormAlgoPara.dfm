object FrmAlgoPara: TFrmAlgoPara
  Left = 1350
  Top = 111
  Width = 294
  Height = 675
  Caption = 'FrmAlgoPara'
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
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnInterface: TPanel
    Left = 0
    Top = 0
    Width = 286
    Height = 36
    Align = alTop
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 0
    object btParaSave: TButton
      Left = 163
      Top = 3
      Width = 43
      Height = 30
      Caption = 'SAVE'
      TabOrder = 0
      OnClick = btParaSaveClick
    end
    object btPre: TButton
      Left = 3
      Top = 3
      Width = 20
      Height = 30
      Caption = '<'
      TabOrder = 1
      Visible = False
      OnClick = btPreClick
    end
    object btNext: TButton
      Left = 211
      Top = 3
      Width = 20
      Height = 30
      Caption = '>'
      TabOrder = 2
      Visible = False
      OnClick = btNextClick
    end
    object Button5: TButton
      Left = 118
      Top = 3
      Width = 43
      Height = 30
      Caption = 'CHECK'
      TabOrder = 3
      OnClick = Button5Click
    end
    object btTrain: TButton
      Left = 73
      Top = 3
      Width = 43
      Height = 31
      Caption = 'TRAIN'
      TabOrder = 4
      OnClick = btTrainClick
    end
    object btClose: TButton
      Left = 28
      Top = 3
      Width = 43
      Height = 30
      Caption = 'CLOSE'
      TabOrder = 5
      OnClick = btCloseClick
    end
  end
  object pcAlgPara: TPageControl
    Left = 0
    Top = 36
    Width = 286
    Height = 605
    ActivePage = tsWfr
    Align = alClient
    MultiLine = True
    TabIndex = 1
    TabOrder = 1
    TabWidth = 35
    object tsOri: TTabSheet
      Caption = 'Ori'
      object pnParaOri: TPanel
        Left = 0
        Top = 0
        Width = 278
        Height = 577
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnParaOri'
        TabOrder = 0
      end
    end
    object tsWfr: TTabSheet
      Caption = 'Wfr'
      ImageIndex = 1
      object pnParaWfr: TPanel
        Left = 0
        Top = 0
        Width = 278
        Height = 577
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnPara'
        TabOrder = 0
      end
    end
    object tsRlb: TTabSheet
      Caption = 'Rlb'
      ImageIndex = 2
      object pnParaRalBf: TPanel
        Left = 0
        Top = 0
        Width = 278
        Height = 577
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnPara'
        TabOrder = 0
      end
    end
    object tsRla: TTabSheet
      Caption = 'Rla'
      ImageIndex = 3
      object pnParaRalAt: TPanel
        Left = 0
        Top = 0
        Width = 278
        Height = 577
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnPara'
        TabOrder = 0
      end
    end
    object tsWfo: TTabSheet
      Caption = 'Wfo'
      ImageIndex = 4
      object pnParaWfrOr: TPanel
        Left = 0
        Top = 0
        Width = 278
        Height = 577
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnPara'
        TabOrder = 0
      end
    end
  end
  object Timer1: TTimer
    Interval = 300
    OnTimer = Timer1Timer
    Left = 8
    Top = 56
  end
end
