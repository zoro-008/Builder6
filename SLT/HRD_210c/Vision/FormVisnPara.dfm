object FrmVisnPara: TFrmVisnPara
  Left = 969
  Top = 347
  BorderStyle = bsDialog
  Caption = 'FrmVisnPara'
  ClientHeight = 331
  ClientWidth = 260
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pcVisnPara: TPageControl
    Left = 0
    Top = 0
    Width = 260
    Height = 331
    ActivePage = tsWfr
    Align = alClient
    MultiLine = True
    TabIndex = 1
    TabOrder = 0
    TabWidth = 35
    object tsOri: TTabSheet
      Caption = 'Ori'
      object Label1: TLabel
        Left = 20
        Top = 27
        Width = 46
        Height = 13
        Hint = #47588#52824#50984
        AutoSize = False
        Caption = #47588#52824#50984
        ParentShowHint = False
        ShowHint = True
      end
      object Label2: TLabel
        Left = 140
        Top = 27
        Width = 8
        Height = 13
        Caption = '%'
      end
      object Label42: TLabel
        Left = 2
        Top = 3
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object edOriSinc: TEdit
        Left = 108
        Top = 24
        Width = 30
        Height = 21
        Hint = #47588#52824#50984
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '80'
      end
      object cbOriImageSave: TComboBox
        Left = 108
        Top = 0
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 1
        Text = 'NONE'
        Items.Strings = (
          'NONE'
          'FAIL ONLY'
          'GOOD ONLY'
          'ALL')
      end
    end
    object tsWfr: TTabSheet
      Caption = 'Wfr'
      ImageIndex = 1
      object Label3: TLabel
        Left = 2
        Top = 27
        Width = 65
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#49828#54169
        AutoSize = False
        Caption = #51088#51228' '#45331#51060
        ParentShowHint = False
        ShowHint = True
      end
      object Label5: TLabel
        Left = 2
        Top = 51
        Width = 65
        Height = 13
        Hint = #52841#51032' '#45458#51060' '#49828#54169
        AutoSize = False
        Caption = #51088#51228' '#44600#51060
        ParentShowHint = False
        ShowHint = True
      end
      object Label7: TLabel
        Left = 2
        Top = 75
        Width = 68
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51088#51228'  '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object Label18: TLabel
        Left = -44
        Top = 275
        Width = 103
        Height = 13
        AutoSize = False
        Caption = #44592#51456#52841' '#54589#49472#44060#49688
        ParentShowHint = False
        ShowHint = True
        Visible = False
      end
      object Label20: TLabel
        Left = 4
        Top = 155
        Width = 103
        Height = 13
        AutoSize = False
        Caption = #54168#51068#52841' '#54589#49472#44060#49688
        ParentShowHint = False
        ShowHint = True
      end
      object Label21: TLabel
        Left = 151
        Top = 27
        Width = 3
        Height = 13
        ParentShowHint = False
        ShowHint = True
      end
      object Label22: TLabel
        Left = 176
        Top = 27
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label23: TLabel
        Left = 176
        Top = 51
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label24: TLabel
        Left = 176
        Top = 75
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label26: TLabel
        Left = 159
        Top = 179
        Width = 3
        Height = 13
      end
      object Label38: TLabel
        Left = 4
        Top = 131
        Width = 163
        Height = 13
        AutoSize = False
        Caption = #52841' '#50808#44285' '#44160#49324' '#50500#50883' '#54589#49472' '#44060#49688
        ParentShowHint = False
        ShowHint = True
      end
      object Label40: TLabel
        Left = 2
        Top = 99
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51088#51228' '#44033#46020' '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object angle: TLabel
        Left = 176
        Top = 99
        Width = 18
        Height = 13
        Caption = 'ang'
      end
      object Label41: TLabel
        Left = 95
        Top = 99
        Width = 9
        Height = 13
        Caption = '+-'
      end
      object Label43: TLabel
        Left = 2
        Top = 3
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object edWfrWidth: TEdit
        Left = 108
        Top = 24
        Width = 64
        Height = 21
        Hint = #52841#51032' '#45331#51060' '#49828#54169
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '9'
      end
      object edWfrHeight: TEdit
        Left = 108
        Top = 48
        Width = 64
        Height = 21
        Hint = #52841#51032' '#45458#51060' '#49828#54169
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = '9'
      end
      object edWfrGap: TEdit
        Left = 108
        Top = 72
        Width = 64
        Height = 21
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        Text = '1'
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 181
        Width = 185
        Height = 83
        Caption = 'Common Option'
        TabOrder = 3
        object cbWfrFlipX: TCheckBox
          Left = 7
          Top = 18
          Width = 70
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' X '#54540#47549
          Caption = 'X Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
        object cbWfrFlipY: TCheckBox
          Left = 7
          Top = 38
          Width = 70
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' Y '#54540#47549
          Caption = 'Y Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
        end
        object cbWfrFlipT: TCheckBox
          Left = 7
          Top = 59
          Width = 70
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' T '#54540#47549
          Caption = 'T Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
        end
      end
      object edWfrStdPxCnt: TEdit
        Left = 62
        Top = 272
        Width = 30
        Height = 21
        Hint = #52841#50976#47924' '#54869#51064' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = '100'
        Visible = False
      end
      object edWfrFCPxCnt: TEdit
        Left = 174
        Top = 152
        Width = 67
        Height = 21
        Hint = #54168#51068#52841' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        Text = '100'
      end
      object edWfrCCPxCnt: TEdit
        Left = 174
        Top = 128
        Width = 67
        Height = 21
        Hint = #52841#50976#47924' '#54869#51064' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '100'
      end
      object edWfrAngle: TEdit
        Left = 108
        Top = 96
        Width = 64
        Height = 21
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        Text = '1'
      end
      object cbWfrImageSave: TComboBox
        Left = 108
        Top = 0
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 8
        Text = 'NONE'
        Items.Strings = (
          'NONE'
          'FAIL ONLY'
          'GOOD ONLY'
          'ALL')
      end
    end
    object tsRlb: TTabSheet
      Caption = 'Rlb'
      ImageIndex = 2
      object Label15: TLabel
        Left = 4
        Top = 139
        Width = 132
        Height = 13
        AutoSize = False
        Caption = #50640#54253#49884#48157#51008' '#54589#49472' '#49707#51088
        ParentShowHint = False
        ShowHint = True
      end
      object Label4: TLabel
        Left = 5
        Top = 66
        Width = 150
        Height = 13
        AutoSize = False
        Caption = #44592#51456#51216#51004#47196#48512#53552' X'#50724#54532#49489
        ParentShowHint = False
        ShowHint = True
      end
      object Label17: TLabel
        Left = 5
        Top = 90
        Width = 150
        Height = 13
        AutoSize = False
        Caption = #44592#51456#51216#51004#47196#48512#53552' Y'#50724#54532#49489
        ParentShowHint = False
        ShowHint = True
      end
      object Label27: TLabel
        Left = 183
        Top = 137
        Width = 15
        Height = 13
        Caption = 'cnt'
      end
      object Label28: TLabel
        Left = 219
        Top = 67
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label29: TLabel
        Left = 219
        Top = 91
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label25: TLabel
        Left = 4
        Top = 35
        Width = 132
        Height = 13
        AutoSize = False
        Caption = #51088#51228' '#50612#46160#50868' '#54589#49472' '#54217#44512#44050
        ParentShowHint = False
        ShowHint = True
      end
      object Label39: TLabel
        Left = 183
        Top = 33
        Width = 23
        Height = 13
        Caption = '/255'
      end
      object Label44: TLabel
        Left = 2
        Top = 3
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object edRlbPxCnt: TEdit
        Left = 150
        Top = 136
        Width = 30
        Height = 21
        Hint = #48376#46377#46108#44275' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '100'
      end
      object edRlbOffsetX: TEdit
        Left = 161
        Top = 63
        Width = 57
        Height = 21
        Hint = #50620#46972#51064' XY'#44050#50640#49436' '#50724#54532#49489' '#47564#53372' '#46504#50612#51652#44275#51012' '#54648#46308#47084#50640#44172' '#48372#45236' '#51456#45796'. ('#48376#46377' '#54252#51064#53944')'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = '9'
      end
      object edRlbOffsetY: TEdit
        Left = 161
        Top = 87
        Width = 57
        Height = 21
        Hint = #50620#46972#51064' XY'#44050#50640#49436' '#50724#54532#49489' '#47564#53372' '#46504#50612#51652#44275#51012' '#54648#46308#47084#50640#44172' '#48372#45236' '#51456#45796'. ('#48376#46377' '#54252#51064#53944')'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        Text = '9'
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 181
        Width = 185
        Height = 83
        Caption = 'Common Option'
        TabOrder = 3
        object cbRlbFlipX: TCheckBox
          Left = 7
          Top = 18
          Width = 78
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' X'#54540#47549'.'
          Caption = 'X Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
        object cbRlbFlipY: TCheckBox
          Left = 7
          Top = 38
          Width = 78
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' Y'#54540#47549'.'
          Caption = 'Y Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
        end
        object cbRlbFlipT: TCheckBox
          Left = 7
          Top = 59
          Width = 78
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' T'#54540#47549'.'
          Caption = 'T Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
        end
        object cbRlbNotUseAng: TCheckBox
          Left = 87
          Top = 18
          Width = 82
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' X '#54540#47549
          Caption = 'Not Use Ang'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 3
        end
      end
      object edRlbPkgPxAvr: TEdit
        Left = 150
        Top = 32
        Width = 30
        Height = 21
        Hint = #51088#51228' '#50976#47924' '#44048#51648' '#54616#45716' '#54589#49472' '#54217#44512' '#51060#54616' '#49444#51221#44050
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = '100'
      end
      object cbRlbImageSave: TComboBox
        Left = 108
        Top = 0
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 5
        Text = 'NONE'
        Items.Strings = (
          'NONE'
          'FAIL ONLY'
          'GOOD ONLY'
          'ALL')
      end
    end
    object tsRla: TTabSheet
      Caption = 'Rla'
      ImageIndex = 3
      object Label9: TLabel
        Left = 4
        Top = 27
        Width = 65
        Height = 13
        Hint = #52841#51032' '#45331#51060
        AutoSize = False
        Caption = #51088#51228' '#45331#51060
        ParentShowHint = False
        ShowHint = True
      end
      object Label11: TLabel
        Left = 4
        Top = 51
        Width = 65
        Height = 13
        Hint = #52841#51032' '#45458#51060
        AutoSize = False
        Caption = #51088#51228' '#45458#51060
        ParentShowHint = False
        ShowHint = True
      end
      object Label13: TLabel
        Left = 4
        Top = 75
        Width = 65
        Height = 13
        Hint = #45331#51060' '#45458#51060' '#44077
        AutoSize = False
        Caption = #51088#51228' '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object Label6: TLabel
        Left = 4
        Top = 107
        Width = 65
        Height = 13
        Hint = #44033#46020' '#52841#51032' '#50620#46972#51064' '#47676#51200#52824#44256' '#44144#44592#49436' '#44592#51456#44050#51012' '#44032#51648#44256' '#52841#51032' '#44033#46020#47484' '#48376#45796
        AutoSize = False
        Caption = #51088#51228' '#44033#46020
        ParentShowHint = False
        ShowHint = True
      end
      object Label8: TLabel
        Left = 4
        Top = 131
        Width = 89
        Height = 13
        Hint = #44033#46020' '#44077
        AutoSize = False
        Caption = #51088#51228' '#44033#46020' '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object Label10: TLabel
        Left = -116
        Top = 235
        Width = 151
        Height = 13
        Hint = #52841#51032' '#48537#51008' '#50948#52824' X ('#48376#46377#51204' '#44160#49324' '#49828#54169#44284' '#46041#51068#54644#50556#54632'.)'
        AutoSize = False
        Caption = #44592#51456#51216#51004#47196#48512#53552' X '#50724#54532#49489
        ParentShowHint = False
        ShowHint = True
        Visible = False
      end
      object Label12: TLabel
        Left = -116
        Top = 259
        Width = 152
        Height = 13
        Hint = #52841#51032' '#48537#51008' '#50948#52824' Y ('#48376#46377#51204' '#44160#49324' '#49828#54169#44284' '#46041#51068#54644#50556#54632'.)'
        AutoSize = False
        Caption = #44592#51456#51216#51004#47196#48512#53552' Y '#50724#54532#49489
        ParentShowHint = False
        ShowHint = True
        Visible = False
      end
      object Label14: TLabel
        Left = 4
        Top = 195
        Width = 106
        Height = 13
        Hint = #52841#51032' '#48537#51008' '#50948#52824' Gap'
        AutoSize = False
        Caption = #51088#51228' '#50724#54532#49489' '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object Label30: TLabel
        Left = 187
        Top = 27
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label31: TLabel
        Left = 187
        Top = 51
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label32: TLabel
        Left = 187
        Top = 75
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label33: TLabel
        Left = 187
        Top = 107
        Width = 19
        Height = 13
        Caption = 'Ang'
      end
      object Label34: TLabel
        Left = 187
        Top = 131
        Width = 19
        Height = 13
        Caption = 'Ang'
      end
      object Label35: TLabel
        Left = 107
        Top = 232
        Width = 16
        Height = 13
        Caption = 'mm'
        Visible = False
      end
      object Label36: TLabel
        Left = 107
        Top = 259
        Width = 16
        Height = 13
        Caption = 'mm'
        Visible = False
      end
      object Label37: TLabel
        Left = 207
        Top = 195
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label45: TLabel
        Left = 2
        Top = 3
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object edRlaWidth: TEdit
        Left = 108
        Top = 24
        Width = 61
        Height = 21
        Hint = #52841#51032' '#45331#51060
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '9'
      end
      object edRlaHeight: TEdit
        Left = 108
        Top = 48
        Width = 61
        Height = 21
        Hint = #52841#51032' '#45458#51060
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = '9'
      end
      object edRlaGap: TEdit
        Left = 108
        Top = 72
        Width = 61
        Height = 21
        Hint = #45331#51060' '#45458#51060' '#44077
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        Text = '1'
      end
      object edRlaAngle: TEdit
        Left = 108
        Top = 104
        Width = 61
        Height = 21
        Hint = #44033#46020' '#52841#51032' '#50620#46972#51064' '#47676#51200#52824#44256' '#44144#44592#49436' '#44592#51456#44050#51012' '#44032#51648#44256' '#52841#51032' '#44033#46020#47484' '#48376#45796
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        Text = '9'
      end
      object edRlaAngleGap: TEdit
        Left = 108
        Top = 128
        Width = 61
        Height = 21
        Hint = #44033#46020' '#44077
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = '1'
      end
      object edRlaOffsetX: TEdit
        Left = 41
        Top = 232
        Width = 65
        Height = 21
        Hint = #52841#51032' '#48537#51008' '#50948#52824' X ('#48376#46377#51204' '#44160#49324' '#49828#54169#44284' '#46041#51068#54644#50556#54632'.)'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        Text = '9'
        Visible = False
      end
      object edRlaOffsetY: TEdit
        Left = 41
        Top = 256
        Width = 65
        Height = 21
        Hint = #52841#51032' '#48537#51008' '#50948#52824' Y ('#48376#46377#51204' '#44160#49324' '#49828#54169#44284' '#46041#51068#54644#50556#54632'.)'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '9'
        Visible = False
      end
      object edRlaOffsetGap: TEdit
        Left = 168
        Top = 192
        Width = 30
        Height = 21
        Hint = #52841#51032' '#48537#51008' '#50948#52824' Gap'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        Text = '1'
      end
      object cbRlaImageSave: TComboBox
        Left = 108
        Top = 0
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 8
        Text = 'NONE'
        Items.Strings = (
          'NONE'
          'FAIL ONLY'
          'GOOD ONLY'
          'ALL')
      end
    end
    object tsWfo: TTabSheet
      Caption = 'Wfo'
      ImageIndex = 4
      object Label16: TLabel
        Left = 4
        Top = 27
        Width = 45
        Height = 13
        Hint = #47588#52824#50984
        AutoSize = False
        Caption = #47588#52824#50984
        ParentShowHint = False
        ShowHint = True
      end
      object Label19: TLabel
        Left = 140
        Top = 27
        Width = 8
        Height = 13
        Caption = '%'
      end
      object Label46: TLabel
        Left = 2
        Top = 3
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object edWfoSinc: TEdit
        Left = 108
        Top = 24
        Width = 30
        Height = 21
        Hint = #47588#52824#50984
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '80'
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 181
        Width = 90
        Height = 83
        Caption = 'Common Option'
        TabOrder = 1
        object cbWfoFlipX: TCheckBox
          Left = 7
          Top = 18
          Width = 78
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' X'#54540#47549'.'
          Caption = 'X Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
        object cbWfoFlipY: TCheckBox
          Left = 7
          Top = 38
          Width = 78
          Height = 17
          Hint = #54648#46308#47084#50640' '#48372#45236#51452#45716' '#44050' Y'#54540#47549'.'
          Caption = 'Y Flip'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
        end
      end
      object cbWfoImageSave: TComboBox
        Left = 108
        Top = 0
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 2
        Text = 'NONE'
        Items.Strings = (
          'NONE'
          'FAIL ONLY'
          'GOOD ONLY'
          'ALL')
      end
    end
  end
  object btSave: TButton
    Left = 173
    Top = 299
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 1
    OnClick = btSaveClick
  end
  object btClose: TButton
    Left = 93
    Top = 299
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 2
    OnClick = btCloseClick
  end
end
