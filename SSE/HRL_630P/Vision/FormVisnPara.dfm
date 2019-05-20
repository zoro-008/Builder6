object FrmVisnPara: TFrmVisnPara
  Left = 901
  Top = 196
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
    TabIndex = 0
    TabOrder = 0
    TabWidth = 35
    object tsWfr: TTabSheet
      Caption = 'Wfr'
      ImageIndex = 1
      object Label3: TLabel
        Left = 2
        Top = 35
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
        Top = 59
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
        Top = 83
        Width = 68
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51088#51228'  '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object Label21: TLabel
        Left = 150
        Top = 35
        Width = 3
        Height = 13
        ParentShowHint = False
        ShowHint = True
      end
      object Label22: TLabel
        Left = 175
        Top = 35
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label23: TLabel
        Left = 175
        Top = 59
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label24: TLabel
        Left = 175
        Top = 83
        Width = 16
        Height = 13
        Caption = 'mm'
      end
      object Label26: TLabel
        Left = 159
        Top = 155
        Width = 3
        Height = 13
      end
      object Label40: TLabel
        Left = 2
        Top = 107
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51088#51228' '#44033#46020' '#54200#52264
        ParentShowHint = False
        ShowHint = True
      end
      object angle: TLabel
        Left = 175
        Top = 107
        Width = 18
        Height = 13
        Caption = 'ang'
      end
      object Label41: TLabel
        Left = 95
        Top = 107
        Width = 9
        Height = 13
        Caption = '+-'
      end
      object Label1: TLabel
        Left = 2
        Top = 11
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object Label38: TLabel
        Left = 2
        Top = 137
        Width = 163
        Height = 13
        AutoSize = False
        Caption = #52841' '#50808#44285' '#44160#49324' '#50500#50883' '#54589#49472' '#44060#49688
        ParentShowHint = False
        ShowHint = True
      end
      object Label20: TLabel
        Left = 2
        Top = 161
        Width = 103
        Height = 13
        AutoSize = False
        Caption = #54168#51068#52841' '#54589#49472#44060#49688
        ParentShowHint = False
        ShowHint = True
      end
      object Label18: TLabel
        Left = -12
        Top = 283
        Width = 103
        Height = 13
        AutoSize = False
        Caption = #44592#51456#52841' '#54589#49472#44060#49688
        ParentShowHint = False
        ShowHint = True
        Visible = False
      end
      object edWfrWidth: TEdit
        Left = 107
        Top = 32
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
        Left = 107
        Top = 56
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
        Left = 107
        Top = 80
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
        Left = 0
        Top = 185
        Width = 90
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
      object edWfrAngle: TEdit
        Left = 107
        Top = 104
        Width = 64
        Height = 21
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = '1'
      end
      object cbWfrImageSave: TComboBox
        Left = 108
        Top = 8
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
      object edWfrFCPxCnt: TEdit
        Left = 172
        Top = 158
        Width = 67
        Height = 21
        Hint = #54168#51068#52841' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '100'
      end
      object edWfrCCPxCnt: TEdit
        Left = 172
        Top = 134
        Width = 67
        Height = 21
        Hint = #52841#50976#47924' '#54869#51064' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        Text = '100'
      end
      object edWfrStdPxCnt: TEdit
        Left = 46
        Top = 288
        Width = 30
        Height = 21
        Hint = #52841#50976#47924' '#54869#51064' '#54589#49472#44050' '#54217#44512' '#49828#54169'.'
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 8
        Text = '100'
        Visible = False
      end
    end
    object tsWfo: TTabSheet
      Caption = 'Wfo'
      ImageIndex = 4
      object Label16: TLabel
        Left = 4
        Top = 35
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
        Top = 35
        Width = 8
        Height = 13
        Caption = '%'
      end
      object Label2: TLabel
        Left = 2
        Top = 11
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
        Top = 32
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
        Top = 8
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
    object tsWfp: TTabSheet
      Caption = 'Wfp'
      ImageIndex = 3
      object Label4: TLabel
        Left = 2
        Top = 11
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object Label8: TLabel
        Left = 4
        Top = 35
        Width = 93
        Height = 13
        Hint = #54589#49472' '#52852#50868#53944
        AutoSize = False
        Caption = #45796#53356' '#54589#49472' '#52852#50868#53944
        ParentShowHint = False
        ShowHint = True
      end
      object cbWfpImageSave: TComboBox
        Left = 108
        Top = 8
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = 'NONE'
        Items.Strings = (
          'NONE'
          'FAIL ONLY'
          'GOOD ONLY'
          'ALL')
      end
      object edWfpPxCnt: TEdit
        Left = 108
        Top = 32
        Width = 103
        Height = 21
        Hint = #47588#52824#50984
        ImeName = 'Microsoft Office IME 2007'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = '80'
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 181
        Width = 90
        Height = 83
        Caption = 'Common Option'
        TabOrder = 2
        object cbWfpFlipX: TCheckBox
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
        object cbWfpFlipY: TCheckBox
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
    end
    object tsCal: TTabSheet
      Caption = 'Cal'
      ImageIndex = 2
      object Label6: TLabel
        Left = 2
        Top = 11
        Width = 87
        Height = 13
        Hint = #52841#51032' '#45331#51060' '#45458#51060' '#49828#54169' '#50724#54532#49489' '#45331#51060' '#45458#51060#49444#51221#44050#51032' +- Gap'#50504#50640' '#46308#50612' '#50724#47732' Pass'
        AutoSize = False
        Caption = #51060#48120#51648' '#51200#51109
        ParentShowHint = False
        ShowHint = True
      end
      object cbCalImageSave: TComboBox
        Left = 108
        Top = 8
        Width = 106
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
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
