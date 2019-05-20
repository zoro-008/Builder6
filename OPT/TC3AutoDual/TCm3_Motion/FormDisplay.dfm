object FrmDisplay: TFrmDisplay
  Left = 722
  Top = 254
  BorderStyle = bsNone
  Caption = 'FrmDisplay'
  ClientHeight = 36
  ClientWidth = 892
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  OnDestroy = FormDestroy
  OnDeactivate = FormDeactivate
  OnHide = tmUpdateTimer
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 51
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total'
    Color = clGradientInactiveCaption
    TabOrder = 0
    OnDblClick = Panel2DblClick
  end
  object Panel3: TPanel
    Left = 103
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Yeild'
    Color = clGradientInactiveCaption
    TabOrder = 1
    OnDblClick = Panel2DblClick
  end
  object Panel4: TPanel
    Left = 155
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Avr Time'
    Color = clGradientInactiveCaption
    TabOrder = 2
    OnDblClick = Panel2DblClick
  end
  object Panel6: TPanel
    Left = 207
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Adj Time'
    Color = clInactiveCaptionText
    TabOrder = 3
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Pass'
    Color = clGradientInactiveCaption
    TabOrder = 4
    OnDblClick = Panel2DblClick
  end
  object Panel7: TPanel
    Left = 259
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Bond Time'
    Color = clInactiveCaptionText
    TabOrder = 5
  end
  object Panel8: TPanel
    Left = 184
    Top = 41
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Wait Time'
    Color = clInactiveCaptionText
    TabOrder = 6
  end
  object Panel5: TPanel
    Left = 140
    Top = 41
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total Time'
    Color = clGradientInactiveCaption
    TabOrder = 7
    OnDblClick = Panel2DblClick
  end
  object Panel9: TPanel
    Left = 311
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'UV Time'
    Color = clInactiveCaptionText
    TabOrder = 8
  end
  object pnLTotal: TPanel
    Left = 51
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total'
    Color = clGradientInactiveCaption
    TabOrder = 9
    OnDblClick = Panel2DblClick
  end
  object pnLYield: TPanel
    Left = 103
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Yeild'
    Color = clGradientInactiveCaption
    TabOrder = 10
    OnDblClick = Panel2DblClick
  end
  object pnLAvr: TPanel
    Left = 155
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Avr Time'
    Color = clGradientInactiveCaption
    TabOrder = 11
    OnDblClick = Panel2DblClick
  end
  object pnLAdj: TPanel
    Left = 207
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Adj Time'
    Color = clInactiveCaptionText
    TabOrder = 12
  end
  object pnLPass: TPanel
    Left = 0
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Pass'
    Color = clGradientInactiveCaption
    TabOrder = 13
    OnDblClick = Panel2DblClick
  end
  object pnLBond: TPanel
    Left = 259
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Bond Time'
    Color = clInactiveCaptionText
    TabOrder = 14
  end
  object pnLTotalTime: TPanel
    Left = 140
    Top = 58
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total Time'
    Color = clGradientInactiveCaption
    TabOrder = 16
    OnDblClick = Panel2DblClick
  end
  object pnLUv: TPanel
    Left = 311
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'UV Time'
    Color = clInactiveCaptionText
    TabOrder = 17
  end
  object Panel19: TPanel
    Left = 466
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total'
    Color = 9225983
    TabOrder = 18
    OnDblClick = Panel19DblClick
  end
  object Panel20: TPanel
    Left = 518
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Yeild'
    Color = 9225983
    TabOrder = 19
    OnDblClick = Panel19DblClick
  end
  object Panel21: TPanel
    Left = 570
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Avr Time'
    Color = 9225983
    TabOrder = 20
    OnDblClick = Panel19DblClick
  end
  object Panel22: TPanel
    Left = 622
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Adj Time'
    Color = 13559807
    TabOrder = 21
  end
  object Panel23: TPanel
    Left = 415
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Pass'
    Color = 9225983
    TabOrder = 22
    OnDblClick = Panel19DblClick
  end
  object Panel24: TPanel
    Left = 674
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Bond Time'
    Color = 13559807
    TabOrder = 23
  end
  object Panel25: TPanel
    Left = 644
    Top = 41
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Wait Time'
    Color = 13559807
    TabOrder = 24
  end
  object Panel26: TPanel
    Left = 598
    Top = 41
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total Time'
    Color = 9225983
    TabOrder = 25
    OnDblClick = Panel19DblClick
  end
  object Panel27: TPanel
    Left = 726
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'UV Time'
    Color = 13559807
    TabOrder = 26
  end
  object pnRTotal: TPanel
    Left = 466
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total'
    Color = 9225983
    TabOrder = 27
    OnDblClick = Panel19DblClick
  end
  object pnRYield: TPanel
    Left = 518
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Yeild'
    Color = 9225983
    TabOrder = 28
    OnDblClick = Panel19DblClick
  end
  object pnRAvr: TPanel
    Left = 570
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Avr Time'
    Color = 9225983
    TabOrder = 29
    OnDblClick = Panel19DblClick
  end
  object pnRAdj: TPanel
    Left = 622
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Adj Time'
    Color = 13559807
    TabOrder = 30
  end
  object pnRPass: TPanel
    Left = 414
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Pass'
    Color = 9225983
    TabOrder = 31
    OnDblClick = Panel19DblClick
  end
  object pnRBond: TPanel
    Left = 674
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Bond Time'
    Color = 13559807
    TabOrder = 32
  end
  object pnRWait: TPanel
    Left = 644
    Top = 58
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Wait Time'
    Color = 13559807
    TabOrder = 33
  end
  object pnRTotalTime: TPanel
    Left = 598
    Top = 58
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Total Time'
    Color = 9225983
    TabOrder = 34
    OnDblClick = Panel19DblClick
  end
  object pnRUv: TPanel
    Left = 726
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'UV Time'
    Color = 13559807
    TabOrder = 35
  end
  object pnLWait: TPanel
    Left = 184
    Top = 58
    Width = 46
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Wait Time'
    Color = clInactiveCaptionText
    TabOrder = 15
  end
  object Panel10: TPanel
    Left = 363
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Work Time'
    Color = clInactiveCaptionText
    TabOrder = 36
  end
  object pnLWork: TPanel
    Left = 363
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Work Time'
    Color = clInactiveCaptionText
    TabOrder = 37
  end
  object Panel12: TPanel
    Left = 778
    Top = 0
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Work Time'
    Color = 13559807
    TabOrder = 38
  end
  object pnRWork: TPanel
    Left = 778
    Top = 17
    Width = 52
    Height = 17
    BevelOuter = bvLowered
    Caption = 'Work Time'
    Color = 13559807
    TabOrder = 39
  end
  object btClr: TButton
    Left = 831
    Top = -1
    Width = 60
    Height = 36
    Caption = 'Reset CNT'
    TabOrder = 40
    OnClick = btClrClick
  end
  object tmUpdate: TTimer
    Enabled = False
    OnTimer = tmUpdateTimer
    Left = 957
    Top = 17
  end
  object tmSave: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = tmSaveTimer
    Left = 957
    Top = 65520
  end
end
