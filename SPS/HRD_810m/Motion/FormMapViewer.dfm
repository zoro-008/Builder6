object FrmMapViewer: TFrmMapViewer
  Left = 397
  Top = 139
  Width = 1222
  Height = 770
  Caption = 'FrmMapViewer'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object btLoadHexMapFile: TButton
    Left = 8
    Top = 671
    Width = 177
    Height = 60
    Caption = #54757#49324' '#47605' '#54028#51068' '#48520#47084#50724#44592
    TabOrder = 0
    OnClick = btLoadHexMapFileClick
  end
  object Memo1: TMemo
    Left = 8
    Top = 25
    Width = 600
    Height = 641
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 1
  end
  object Memo2: TMemo
    Left = 607
    Top = 25
    Width = 600
    Height = 641
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 2
  end
  object Memo3: TMemo
    Left = 224
    Top = 671
    Width = 849
    Height = 58
    ImeName = 'Microsoft IME 2010'
    Lines.Strings = (
      '1. '#50694#51032' '#39#54757#49324' '#47605' '#54028#51068' '#48520#47084#50724#44592#39' '#48260#53948#51012' '#53364#47533' '#54644' '#47605#54028#51068#51012' '#47196#46377' '#54633#45768#45796'.'
      '2. '#50812#51901#50640#45716' '#50896#48376' '#47605#54028#51068' '#50724#47480#51901#50640#45716' '#54757#49324' '#47605#54028#51068#51012' '#52968#48260#53944#54620' '#47605' '#54028#51068#51060' '#45208#53440#45225#45768#45796'.'
      '3. '#44592#51316#50640' '#51077#47141' '#54664#45912' '#48169#49885#44284' '#46041#51068#54616#44172' '#48320#54872#46108' '#47605#54028#51068#50640' '#45936#51060#53552#47484' '#39#47605' '#54028#51068' '#51221#48372#39#50640' '#51077#47141' '#54644' '#51469#45768#45796'.'
      'EX) '#51221#49345#45936#51060#53552' = "A1" '#50668#48177#45936#51060#53552' = "7E" '#47560#49828#53356' '#45936#51060#53552#45716' = "7A"')
    TabOrder = 3
  end
  object Panel1: TPanel
    Left = 8
    Top = 0
    Width = 600
    Height = 25
    Caption = #50896#48376' '#47605#54028#51068
    TabOrder = 4
  end
  object Panel2: TPanel
    Left = 608
    Top = 0
    Width = 600
    Height = 25
    Caption = #48320#54872#54620' '#47605#54028#51068
    TabOrder = 5
  end
  object odMap: TOpenDialog
    DefaultExt = 'Map'
    FileName = 'MapData'
    Filter = 
      'sinf'#54805#49885'|*.sinf|wmap_a'#54805#49885'|*.wmap_a|xml'#54805#49885'|*.xml|log2'#54805#49885'|*.log2|wmap'#54805#49885 +
      '|*.wmap|w05'#54805#49885'|*.w05'
    Left = 1176
    Top = 40
  end
end
