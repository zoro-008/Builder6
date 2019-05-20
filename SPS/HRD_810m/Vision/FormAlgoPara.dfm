object FrmAlgoPara: TFrmAlgoPara
  Left = 276
  Top = 140
  Width = 500
  Height = 800
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
  PixelsPerInch = 96
  TextHeight = 13
  object pcAlgPara: TPageControl
    Left = 0
    Top = 58
    Width = 492
    Height = 715
    ActivePage = tsWfr
    Align = alClient
    MultiLine = True
    TabIndex = 0
    TabOrder = 0
    TabWidth = 35
    object tsWfr: TTabSheet
      Caption = 'Wfr'
      ImageIndex = 1
      object pnParaWfr: TPanel
        Left = 0
        Top = 0
        Width = 484
        Height = 687
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnPara'
        TabOrder = 0
      end
    end
    object tsWfo: TTabSheet
      Caption = 'Wfo'
      ImageIndex = 4
      object pnParaWfo: TPanel
        Left = 0
        Top = 0
        Width = 297
        Height = 577
        Align = alClient
        BevelOuter = bvNone
        Caption = 'pnPara'
        TabOrder = 0
      end
    end
    object tsWfp: TTabSheet
      Caption = 'Wfp'
      ImageIndex = 3
      object pnParaWfp: TPanel
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
    object tsCal: TTabSheet
      Caption = 'Cal'
      ImageIndex = 2
      object pnParaCal: TPanel
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
  object pnInterface: TPanel
    Left = 0
    Top = 0
    Width = 492
    Height = 58
    Align = alTop
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 1
    object lbMessage: TLabel
      Left = 5
      Top = 40
      Width = 359
      Height = 13
      Caption = #52841#44160#49324' '#53944#47112#52964#44032' '#50880#51649#50688#49845#45768#45796'. '#49468#53552#47484' '#49688#54665#44284' '#49464#51060#48652' '#54644#51452#49464#50836
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -8
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object btParaSave: TButton
      Left = 187
      Top = 3
      Width = 49
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
      Left = 243
      Top = 3
      Width = 20
      Height = 30
      Caption = '>'
      TabOrder = 2
      Visible = False
      OnClick = btNextClick
    end
    object btInsp: TButton
      Left = 134
      Top = 3
      Width = 49
      Height = 30
      Caption = 'INSP'
      TabOrder = 3
      OnClick = btInspClick
    end
    object btTrain: TButton
      Left = 81
      Top = 3
      Width = 49
      Height = 31
      Caption = 'TRAIN'
      TabOrder = 4
      OnClick = btTrainClick
    end
    object btCenter: TButton
      Left = 28
      Top = 3
      Width = 49
      Height = 30
      Caption = 'CENTER'
      TabOrder = 5
      Visible = False
      OnClick = btCenterClick
    end
    object Panel1: TPanel
      Left = 270
      Top = 1
      Width = 19
      Height = 11
      BevelOuter = bvNone
      Color = clInfoText
      TabOrder = 6
      OnClick = Panel1Click
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 300
    OnTimer = tmUpdateTimer
    Left = 272
    Top = 8
  end
end
