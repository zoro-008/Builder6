object FormManual: TFormManual
  Left = 159
  Top = 94
  Align = alClient
  BorderStyle = bsNone
  Caption = 'FormManual'
  ClientHeight = 863
  ClientWidth = 1272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 1272
    Height = 754
    ActivePage = TabSheet3
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabHeight = 30
    TabIndex = 2
    TabOrder = 0
    TabWidth = 200
    object TabSheet1: TTabSheet
      Caption = 'Loading'
      object btMan1_2: TBitBtn
        Tag = 11
        Left = 152
        Top = 112
        Width = 430
        Height = 41
        Caption = '2. '#47196#45908' '#47588#44144#51652' '#44277#44553
        TabOrder = 0
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_3: TBitBtn
        Tag = 12
        Left = 152
        Top = 160
        Width = 430
        Height = 41
        Caption = '3. '#47196#45908' '#54589
        TabOrder = 1
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_4: TBitBtn
        Tag = 13
        Left = 152
        Top = 208
        Width = 430
        Height = 41
        Caption = '4. '#47196#45908' '#49828#53440#53944' '#54252#51648#49496
        TabOrder = 2
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_5: TBitBtn
        Left = 152
        Top = 256
        Width = 430
        Height = 41
        Caption = '5. '#47196#45908' '#47588#44144#51652' '#48176#52636
        TabOrder = 3
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_6: TBitBtn
        Tag = 15
        Left = 152
        Top = 304
        Width = 430
        Height = 41
        Caption = '6. '#47196#45908' '#54588#52824' '#50629
        TabOrder = 4
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_7: TBitBtn
        Tag = 16
        Left = 152
        Top = 352
        Width = 430
        Height = 41
        Caption = '7. '#47196#45908' '#54588#52824' '#45796#50868
        TabOrder = 5
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_8: TBitBtn
        Tag = 17
        Left = 152
        Top = 400
        Width = 430
        Height = 41
        Caption = '8. '#47196#45908' '#54392#49492' '#51204#51652'/'#54980#51652
        TabOrder = 6
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_9: TBitBtn
        Tag = 18
        Left = 152
        Top = 448
        Width = 430
        Height = 41
        Caption = '9. '#47196#45908' '#53364#47016#54532
        TabOrder = 7
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_10: TBitBtn
        Tag = 19
        Left = 152
        Top = 496
        Width = 430
        Height = 41
        Caption = '10. '#47196#45908' '#44277#44553' AC'#47784#53552' On/Off'
        TabOrder = 8
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_11: TBitBtn
        Tag = 20
        Left = 152
        Top = 544
        Width = 430
        Height = 41
        Caption = '11. '#47196#45908' '#48176#52636' AC'#47784#53552' On/Off'
        TabOrder = 9
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_12: TBitBtn
        Tag = 21
        Left = 152
        Top = 592
        Width = 430
        Height = 41
        Caption = '12. '#54532#47532#48260#54140' '#49828#53664#54140' '#50629
        TabOrder = 10
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual1_2: TPanel
        Tag = 11
        Left = 80
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 11
        Visible = False
      end
      object pnManual1_3: TPanel
        Tag = 12
        Left = 80
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 12
        Visible = False
      end
      object pnManual1_4: TPanel
        Tag = 13
        Left = 80
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 13
        Visible = False
      end
      object pnManual1_5: TPanel
        Tag = 14
        Left = 80
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 14
        Visible = False
      end
      object pnManual1_6: TPanel
        Tag = 15
        Left = 80
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 15
        Visible = False
      end
      object pnManual1_7: TPanel
        Tag = 16
        Left = 80
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 16
        Visible = False
      end
      object pnManual1_8: TPanel
        Tag = 17
        Left = 80
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 17
        Visible = False
      end
      object pnManual1_9: TPanel
        Tag = 18
        Left = 80
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 18
        Visible = False
      end
      object pnManual1_10: TPanel
        Tag = 19
        Left = 80
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 19
        Visible = False
      end
      object pnManual1_11: TPanel
        Tag = 20
        Left = 80
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 20
        Visible = False
      end
      object pnManual1_12: TPanel
        Tag = 21
        Left = 80
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 21
        Visible = False
      end
      object btMan1_13: TBitBtn
        Tag = 22
        Left = 736
        Top = 64
        Width = 430
        Height = 41
        Cursor = crHandPoint
        Caption = '13. '#54532#47532#48260#54140' '#49828#53664#54140' '#45796#50868
        TabOrder = 22
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_14: TBitBtn
        Tag = 23
        Left = 736
        Top = 112
        Width = 430
        Height = 41
        TabOrder = 23
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_15: TBitBtn
        Tag = 24
        Left = 736
        Top = 160
        Width = 430
        Height = 41
        TabOrder = 24
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_16: TBitBtn
        Tag = 25
        Left = 736
        Top = 208
        Width = 430
        Height = 41
        TabOrder = 25
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_17: TBitBtn
        Tag = 26
        Left = 736
        Top = 256
        Width = 430
        Height = 41
        TabOrder = 26
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_18: TBitBtn
        Tag = 27
        Left = 736
        Top = 304
        Width = 430
        Height = 41
        TabOrder = 27
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_19: TBitBtn
        Tag = 28
        Left = 736
        Top = 352
        Width = 430
        Height = 41
        TabOrder = 28
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_20: TBitBtn
        Tag = 29
        Left = 736
        Top = 400
        Width = 430
        Height = 41
        TabOrder = 29
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_22: TBitBtn
        Left = 736
        Top = 496
        Width = 430
        Height = 41
        TabOrder = 30
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_23: TBitBtn
        Left = 736
        Top = 544
        Width = 430
        Height = 41
        TabOrder = 31
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan1_24: TBitBtn
        Left = 736
        Top = 592
        Width = 430
        Height = 41
        TabOrder = 32
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual1_13: TPanel
        Tag = 22
        Left = 664
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 33
        Visible = False
      end
      object pnManual1_14: TPanel
        Left = 664
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 34
        Visible = False
      end
      object pnManual1_15: TPanel
        Left = 664
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 35
        Visible = False
      end
      object pnManual1_16: TPanel
        Left = 664
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 36
        Visible = False
      end
      object pnManual1_17: TPanel
        Left = 664
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 37
        Visible = False
      end
      object pnManual1_18: TPanel
        Left = 664
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 38
        Visible = False
      end
      object pnManual1_19: TPanel
        Left = 664
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 39
        Visible = False
      end
      object pnManual1_20: TPanel
        Left = 664
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 40
        Visible = False
      end
      object pnManual1_21: TPanel
        Left = 664
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 41
        Visible = False
      end
      object pnManual1_22: TPanel
        Left = 664
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 42
        Visible = False
      end
      object pnManual1_23: TPanel
        Left = 664
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 43
        Visible = False
      end
      object pnManual1_24: TPanel
        Left = 664
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 44
        Visible = False
      end
      object btMan1_1: TBitBtn
        Tag = 10
        Left = 152
        Top = 64
        Width = 430
        Height = 41
        Caption = '1. '#47196#45908' '#51204#52404' '#54856
        TabOrder = 45
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object Panel1: TPanel
        Tag = 11
        Left = 80
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 46
        Visible = False
      end
      object btMan1_21: TBitBtn
        Left = 736
        Top = 448
        Width = 430
        Height = 41
        TabOrder = 47
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Vision 1.2.3'
      ImageIndex = 1
      object btMan2_1: TBitBtn
        Tag = 30
        Left = 152
        Top = 64
        Width = 430
        Height = 41
        Caption = '30. '#48708#51204' '#47784#53552' '#51204#52404' '#54856
        TabOrder = 0
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_2: TBitBtn
        Tag = 31
        Left = 152
        Top = 112
        Width = 430
        Height = 41
        Caption = '31. '#52395#48264#51704' '#48708#51260' '#47532#54532#53944' '#49345#49849
        TabOrder = 1
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_3: TBitBtn
        Tag = 32
        Left = 152
        Top = 160
        Width = 430
        Height = 41
        Caption = '32. '#52395#48264#51704' '#48708#51260' '#47532#54532#53944' '#54616#44053
        TabOrder = 2
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_4: TBitBtn
        Tag = 33
        Left = 152
        Top = 208
        Width = 430
        Height = 41
        Caption = '33. '#52395#48264#51704' '#48708#51260' '#49828#53664#54140' '#49345#49849
        TabOrder = 3
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_5: TBitBtn
        Tag = 34
        Left = 152
        Top = 256
        Width = 430
        Height = 41
        Caption = '34. '#52395#48264#51704' '#48708#51260' '#49828#53664#54140' '#54616#44053
        TabOrder = 4
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_6: TBitBtn
        Tag = 35
        Left = 152
        Top = 304
        Width = 430
        Height = 41
        Caption = '35. '#52395#48264#51704' '#48708#51260' '#50620#46972#51064' '#51204#51652
        TabOrder = 5
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_7: TBitBtn
        Tag = 36
        Left = 152
        Top = 352
        Width = 430
        Height = 41
        Caption = '36. '#52395#48264#51704' '#48708#51260' '#50620#46972#51064' '#54980#51652
        TabOrder = 6
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_8: TBitBtn
        Tag = 37
        Left = 152
        Top = 400
        Width = 430
        Height = 41
        Caption = '37. '#46160#48264#51704' '#48708#51260' '#47532#54532#53944' '#49345#49849
        TabOrder = 7
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_9: TBitBtn
        Tag = 38
        Left = 152
        Top = 448
        Width = 430
        Height = 41
        Caption = '38. '#46160#48264#51704' '#48708#51260' '#47532#54532#53944' '#54616#44053
        TabOrder = 8
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_10: TBitBtn
        Tag = 39
        Left = 152
        Top = 496
        Width = 430
        Height = 41
        Caption = '39. '#46160#48264#51704' '#48708#51260' '#49828#53664#54140' '#49345#49849
        TabOrder = 9
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_11: TBitBtn
        Tag = 40
        Left = 152
        Top = 544
        Width = 430
        Height = 41
        Caption = '40. '#46160#48264#51704' '#48708#51260' '#49828#53664#54140' '#54616#44053
        TabOrder = 10
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_12: TBitBtn
        Tag = 41
        Left = 152
        Top = 592
        Width = 430
        Height = 41
        Caption = '41. '#46160#48264#51704' '#48708#51260' '#50620#46972#51064' '#51204#51652
        TabOrder = 11
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual2_0: TPanel
        Tag = 30
        Left = 80
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 12
        Visible = False
      end
      object pnManual2_1: TPanel
        Tag = 31
        Left = 80
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 13
        Visible = False
      end
      object pnManual2_2: TPanel
        Tag = 32
        Left = 80
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 14
        Visible = False
      end
      object pnManual2_3: TPanel
        Tag = 33
        Left = 80
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 15
        Visible = False
      end
      object pnManual2_4: TPanel
        Tag = 34
        Left = 80
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 16
        Visible = False
      end
      object pnManual2_5: TPanel
        Tag = 35
        Left = 80
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 17
        Visible = False
      end
      object pnManual2_6: TPanel
        Tag = 36
        Left = 80
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 18
        Visible = False
      end
      object pnManual2_7: TPanel
        Tag = 37
        Left = 80
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 19
        Visible = False
      end
      object pnManual2_8: TPanel
        Tag = 38
        Left = 80
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 20
        Visible = False
      end
      object pnManual2_9: TPanel
        Tag = 39
        Left = 80
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 21
        Visible = False
      end
      object pnManual2_10: TPanel
        Tag = 40
        Left = 80
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 22
        Visible = False
      end
      object pnManual2_11: TPanel
        Tag = 41
        Left = 80
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 23
        Visible = False
      end
      object btMan2_13: TBitBtn
        Tag = 42
        Left = 736
        Top = 64
        Width = 430
        Height = 41
        Caption = '42. '#46160#48264#51704' '#48708#51260' '#50620#46972#51064' '#54980#51652
        TabOrder = 24
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_14: TBitBtn
        Tag = 43
        Left = 736
        Top = 112
        Width = 430
        Height = 41
        Caption = '43. '#49464#48264#51704' '#48708#51260' '#47532#54532#53944' '#49345#49849
        TabOrder = 25
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_15: TBitBtn
        Tag = 44
        Left = 736
        Top = 160
        Width = 430
        Height = 41
        Caption = '44. '#49464#48264#51704' '#48708#51260' '#47532#54532#53944' '#54616#44053
        TabOrder = 26
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_16: TBitBtn
        Tag = 45
        Left = 736
        Top = 208
        Width = 430
        Height = 41
        Caption = '45. '#49464#48264#51704' '#48708#51260' '#49828#53664#54140' '#49345#49849
        TabOrder = 27
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_17: TBitBtn
        Tag = 46
        Left = 736
        Top = 256
        Width = 430
        Height = 41
        Caption = '46. '#49464#48264#51704' '#48708#51260' '#49828#53664#54140' '#54616#44053
        TabOrder = 28
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_18: TBitBtn
        Tag = 47
        Left = 736
        Top = 304
        Width = 430
        Height = 41
        Caption = '47. '#49464#48264#51704' '#48708#51260' '#50620#46972#51064' '#51204#51652
        TabOrder = 29
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_19: TBitBtn
        Tag = 48
        Left = 736
        Top = 352
        Width = 430
        Height = 41
        Caption = '48. '#49464#48264#51704' '#48708#51260' '#50620#46972#51064' '#54980#51652
        TabOrder = 30
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_20: TBitBtn
        Tag = 49
        Left = 736
        Top = 400
        Width = 430
        Height = 41
        Caption = '49. '#48708#51204' '#52376#51020' '#44160#49324' '#50948#52824#47196' '#51060#46041
        TabOrder = 31
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_21: TBitBtn
        Tag = 50
        Left = 736
        Top = 448
        Width = 430
        Height = 41
        Caption = '50. '#48708#51204' Y '#47784#53552' '#54856' '#50948#52824#47196' '#51060#46041
        TabOrder = 32
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_22: TBitBtn
        Left = 736
        Top = 496
        Width = 430
        Height = 41
        TabOrder = 33
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_23: TBitBtn
        Left = 736
        Top = 544
        Width = 430
        Height = 41
        TabOrder = 34
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan2_24: TBitBtn
        Left = 736
        Top = 592
        Width = 430
        Height = 41
        TabOrder = 35
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual2_12: TPanel
        Tag = 42
        Left = 664
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 36
        Visible = False
      end
      object pnManual2_13: TPanel
        Tag = 43
        Left = 664
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 37
        Visible = False
      end
      object pnManual2_14: TPanel
        Tag = 44
        Left = 664
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 38
        Visible = False
      end
      object pnManual2_15: TPanel
        Tag = 45
        Left = 664
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 39
        Visible = False
      end
      object pnManual2_16: TPanel
        Tag = 46
        Left = 664
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 40
        Visible = False
      end
      object pnManual2_17: TPanel
        Tag = 47
        Left = 664
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 41
        Visible = False
      end
      object pnManual2_19: TPanel
        Left = 664
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 42
        Visible = False
      end
      object pnManual2_20: TPanel
        Left = 664
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 43
        Visible = False
      end
      object pnManual2_21: TPanel
        Left = 664
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 44
        Visible = False
      end
      object pnManual2_22: TPanel
        Left = 664
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 45
        Visible = False
      end
      object pnManual2_23: TPanel
        Left = 664
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 46
        Visible = False
      end
      object pnManual2_18: TPanel
        Left = 664
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 47
        Visible = False
      end
    end
    object TabSheet3: TTabSheet
      Tag = 65
      Caption = 'Trim / Review Vision'
      ImageIndex = 2
      object btMan3_1: TBitBtn
        Tag = 60
        Left = 152
        Top = 64
        Width = 430
        Height = 41
        Caption = '60. '#53944#47548' '#47784#53552' '#51204#52404' '#54856
        TabOrder = 0
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_2: TBitBtn
        Tag = 61
        Left = 152
        Top = 112
        Width = 430
        Height = 41
        Caption = '61. '#53944#47548' '#45824#44592' '#50948#52824#47196' '#51060#46041
        TabOrder = 1
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_3: TBitBtn
        Tag = 52
        Left = 152
        Top = 160
        Width = 430
        Height = 41
        Caption = '62. '#53944#47548' '#52395#48264#51704' '#50948#52824#47196' '#51060#46041
        TabOrder = 2
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_4: TBitBtn
        Tag = 53
        Left = 152
        Top = 208
        Width = 430
        Height = 41
        Caption = '63. '#53944#47548' '#54540#47532#54140' '#53364#47016#54532' '#50676#44592
        TabOrder = 3
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_5: TBitBtn
        Tag = 54
        Left = 152
        Top = 256
        Width = 430
        Height = 41
        Caption = '64. '#53944#47548' '#54540#47532#54140' '#53364#47016#54532' '#45803#44592
        TabOrder = 4
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_6: TBitBtn
        Tag = 55
        Left = 152
        Top = 304
        Width = 430
        Height = 41
        Caption = '65. '#53944#47548' '#54540#47532#54140' '#50620#46972#51064' '#51204#51652
        TabOrder = 5
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_7: TBitBtn
        Tag = 56
        Left = 152
        Top = 352
        Width = 430
        Height = 41
        Caption = '66. '#53944#47548' '#54540#47532#54140' '#51221#49345' '#50948#52824#47196' '#51060#46041
        TabOrder = 6
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_8: TBitBtn
        Tag = 57
        Left = 152
        Top = 400
        Width = 430
        Height = 41
        Caption = '67. '#53944#47548' '#54540#47532#54140' '#46244#51665#55192' '#50948#52824#47196' '#51060#46041
        TabOrder = 7
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_9: TBitBtn
        Tag = 58
        Left = 152
        Top = 448
        Width = 430
        Height = 41
        Caption = '68. '#47784#46304' '#51088#51116' '#53944#47548' '#53580#49828#53944
        TabOrder = 8
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_10: TBitBtn
        Tag = 59
        Left = 152
        Top = 496
        Width = 430
        Height = 41
        Caption = '69. '#53944#47548' '#49892#47536#45908' '#50629' / '#45796#50868
        TabOrder = 9
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_14: TBitBtn
        Tag = 63
        Left = 736
        Top = 112
        Width = 430
        Height = 41
        Caption = '73. '#51064#45937#49828' '#45796#50868
        TabOrder = 10
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_15: TBitBtn
        Tag = 64
        Left = 736
        Top = 160
        Width = 430
        Height = 41
        Caption = '74. '#51064#45937#49828' '#49892#47536#45908' '#45796#50868
        TabOrder = 11
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual3_0: TPanel
        Tag = 50
        Left = 80
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 12
        Visible = False
      end
      object pnManual3_1: TPanel
        Tag = 51
        Left = 80
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 13
        Visible = False
      end
      object pnManual3_2: TPanel
        Tag = 52
        Left = 80
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 14
        Visible = False
      end
      object pnManual3_3: TPanel
        Tag = 53
        Left = 80
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 15
        Visible = False
      end
      object pnManual3_4: TPanel
        Tag = 54
        Left = 80
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 16
        Visible = False
      end
      object pnManual3_5: TPanel
        Tag = 55
        Left = 80
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 17
        Visible = False
      end
      object pnManual3_6: TPanel
        Tag = 56
        Left = 80
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 18
        Visible = False
      end
      object pnManual3_7: TPanel
        Tag = 57
        Left = 80
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 19
        Visible = False
      end
      object pnManual3_8: TPanel
        Tag = 58
        Left = 80
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 20
        Visible = False
      end
      object pnManual3_9: TPanel
        Tag = 59
        Left = 80
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 21
        Visible = False
      end
      object pnManual3_10: TPanel
        Tag = 60
        Left = 80
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 22
        Visible = False
      end
      object pnManual3_11: TPanel
        Tag = 61
        Left = 80
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 23
        Visible = False
      end
      object btMan3_13: TBitBtn
        Tag = 62
        Left = 736
        Top = 64
        Width = 430
        Height = 41
        Caption = '72. '#51064#45937#49828' '#50629
        TabOrder = 24
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_11: TBitBtn
        Tag = 60
        Left = 152
        Top = 544
        Width = 430
        Height = 41
        Caption = '70. '#53944#47548' '#54252#51648#49496' X'#52629#51004#47196' '#51060#46041
        TabOrder = 25
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_12: TBitBtn
        Left = 152
        Top = 592
        Width = 430
        Height = 41
        Caption = '71. '#53944#47548' '#54252#51648#49496' Y'#52629#51004#47196' '#51060#46041
        TabOrder = 26
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_16: TBitBtn
        Left = 736
        Top = 208
        Width = 430
        Height = 41
        Caption = '75. '#47560#53433' '#54172' '#49892#47536#45908' '#50629' / '#45796#50868
        TabOrder = 27
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_17: TBitBtn
        Left = 736
        Top = 256
        Width = 430
        Height = 41
        Caption = '76. '#47560#53433' '#54172' '#44368#52404' '#50948#52824#47196' '#51060#46041
        TabOrder = 28
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_18: TBitBtn
        Left = 736
        Top = 304
        Width = 430
        Height = 41
        Caption = '77. '#47560#53433' '#54172' '#45824#44592' '#50948#52824#47196' '#51060#46041
        TabOrder = 29
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_19: TBitBtn
        Left = 736
        Top = 352
        Width = 430
        Height = 41
        Caption = '78. '#47784#46304' '#51088#51116' '#47560#53433
        TabOrder = 30
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_20: TBitBtn
        Left = 736
        Top = 400
        Width = 430
        Height = 41
        Caption = '79. '#47560#53433' '#54252#51648#49496' X'#52629#51004#47196' '#51060#46041
        TabOrder = 31
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_21: TBitBtn
        Left = 736
        Top = 448
        Width = 430
        Height = 41
        Caption = '80. '#47560#53433' '#54252#51648#49496' Y'#52629#51004#47196' '#51060#46041
        TabOrder = 32
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_22: TBitBtn
        Left = 736
        Top = 496
        Width = 430
        Height = 41
        TabOrder = 33
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_23: TBitBtn
        Left = 736
        Top = 544
        Width = 430
        Height = 41
        TabOrder = 34
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan3_24: TBitBtn
        Left = 736
        Top = 592
        Width = 430
        Height = 41
        TabOrder = 35
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual3_12: TPanel
        Tag = 62
        Left = 664
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 36
        Visible = False
      end
      object pnManual3_13: TPanel
        Tag = 63
        Left = 664
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 37
        Visible = False
      end
      object pnManual3_14: TPanel
        Tag = 64
        Left = 664
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 38
        Visible = False
      end
      object pnManual3_15: TPanel
        Tag = 65
        Left = 664
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 39
        Visible = False
      end
      object pnManual3_16: TPanel
        Left = 664
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 40
        Visible = False
      end
      object pnManual3_17: TPanel
        Left = 664
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 41
        Visible = False
      end
      object pnManual3_18: TPanel
        Left = 664
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 42
        Visible = False
      end
      object pnManual3_19: TPanel
        Left = 664
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 43
        Visible = False
      end
      object pnManual3_20: TPanel
        Left = 664
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 44
        Visible = False
      end
      object pnManual3_21: TPanel
        Left = 664
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 45
        Visible = False
      end
      object pnManual3_22: TPanel
        Left = 664
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 46
        Visible = False
      end
      object pnManual3_23: TPanel
        Left = 664
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 47
        Visible = False
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'UnLoading'
      ImageIndex = 3
      object btMan4_1: TBitBtn
        Tag = 70
        Left = 152
        Top = 64
        Width = 430
        Height = 41
        Caption = '80. '#50616#47196#46377' '#50648#47532#48288#51060#53552' '#51204#52404' '#54856
        TabOrder = 0
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_2: TBitBtn
        Tag = 71
        Left = 152
        Top = 112
        Width = 430
        Height = 41
        Caption = '81. '#50616#47196#46377' '#49345#45800' '#53364#47016#54532' '#50676#44592' '
        TabOrder = 1
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_3: TBitBtn
        Tag = 72
        Left = 152
        Top = 160
        Width = 430
        Height = 41
        Caption = '82. '#50616#47196#46377' '#49345#45800' '#53364#47016#54532' '#45803#44592
        TabOrder = 2
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_4: TBitBtn
        Tag = 73
        Left = 152
        Top = 208
        Width = 430
        Height = 41
        Caption = '83. '#50616#47196#46377' '#51088#51116' '#54392#49492' '#51204#51652
        TabOrder = 3
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_5: TBitBtn
        Tag = 74
        Left = 152
        Top = 256
        Width = 430
        Height = 41
        Caption = '84. '#50616#47196#46377' '#51088#51116' '#54392#49492' '#54980#51652
        TabOrder = 4
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_6: TBitBtn
        Tag = 75
        Left = 152
        Top = 304
        Width = 430
        Height = 41
        Caption = '85. '#50616#47196#46377' '#54616#45800' '#53364#47016#54532' '#50676#44592
        TabOrder = 5
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_7: TBitBtn
        Tag = 76
        Left = 152
        Top = 352
        Width = 430
        Height = 41
        Caption = '86. '#50616#47196#46377' '#54616#45800' '#53364#47016#54532' '#45803#44592
        TabOrder = 6
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_8: TBitBtn
        Tag = 77
        Left = 152
        Top = 400
        Width = 430
        Height = 41
        Caption = '87. '#50616#47196#46377' '#49345#45800' '#48260#54140' '#47588#44144#51652' A.C '#47784#53552' On / Off'
        TabOrder = 7
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_9: TBitBtn
        Tag = 78
        Left = 152
        Top = 448
        Width = 430
        Height = 41
        Caption = '88. '#50616#47196#46377' '#54616#45800' '#48260#54140' '#47588#44144#51652' A.C '#47784#53552' On / Off'
        TabOrder = 8
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_10: TBitBtn
        Tag = 79
        Left = 152
        Top = 496
        Width = 430
        Height = 41
        Caption = '89. '#50616#47196#46377' '#49345#45800' '#47588#44144#51652' '#44277#44553' 1 '#49324#51060#53364
        TabOrder = 9
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_11: TBitBtn
        Tag = 80
        Left = 152
        Top = 544
        Width = 430
        Height = 41
        Caption = '90. '#50616#47196#46377' '#49345#45800' '#47588#44144#51652' '#50648#47532#48288#51060#53552#50640' '#51201#51116#54616#44592' 1 '#49324#51060#53364
        TabOrder = 10
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_12: TBitBtn
        Tag = 81
        Left = 152
        Top = 592
        Width = 430
        Height = 41
        Caption = '91. '#50616#47196#46377' '#54616#45800' '#47588#44144#51652' '#48176#52636#54616#44592' 1 '#49324#51060#53364
        TabOrder = 11
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual4_0: TPanel
        Tag = 70
        Left = 80
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 12
        Visible = False
      end
      object pnManual4_1: TPanel
        Tag = 71
        Left = 80
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 13
        Visible = False
      end
      object pnManual4_2: TPanel
        Tag = 72
        Left = 80
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 14
        Visible = False
      end
      object pnManual4_3: TPanel
        Tag = 73
        Left = 80
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 15
        Visible = False
      end
      object pnManual4_4: TPanel
        Tag = 74
        Left = 80
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 16
        Visible = False
      end
      object pnManual4_5: TPanel
        Tag = 75
        Left = 80
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 17
        Visible = False
      end
      object pnManual4_6: TPanel
        Tag = 76
        Left = 80
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 18
        Visible = False
      end
      object pnManual4_7: TPanel
        Tag = 77
        Left = 80
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 19
        Visible = False
      end
      object pnManual4_8: TPanel
        Tag = 78
        Left = 80
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 20
        Visible = False
      end
      object pnManual4_9: TPanel
        Tag = 79
        Left = 80
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 21
        Visible = False
      end
      object pnManual4_10: TPanel
        Tag = 80
        Left = 80
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 22
        Visible = False
      end
      object pnManual4_11: TPanel
        Left = 80
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 23
        Visible = False
      end
      object btMan4_13: TBitBtn
        Tag = 82
        Left = 736
        Top = 64
        Width = 430
        Height = 41
        Caption = '92. '#50616#47196#46377' '#47112#51068' '#49892#47536#45908' '#51204#51652
        TabOrder = 24
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_14: TBitBtn
        Tag = 83
        Left = 736
        Top = 112
        Width = 430
        Height = 41
        Caption = '93. '#50616#47196#46377' '#47112#51068' '#49892#47536#45908' '#54980#51652
        TabOrder = 25
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_15: TBitBtn
        Tag = 84
        Left = 736
        Top = 160
        Width = 430
        Height = 41
        Caption = '94. '#50616#47196#46377' '#49345#45800' Z'#52629' '#47588#44144#51652' 1 '#54588#52824' '#45796#50868
        TabOrder = 26
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_16: TBitBtn
        Tag = 85
        Left = 736
        Top = 208
        Width = 430
        Height = 41
        Caption = '95. '#50616#47196#46377' '#49345#45800' Z'#52629' '#47588#44144#51652' 1 '#54588#52824' '#50629
        TabOrder = 27
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_17: TBitBtn
        Tag = 86
        Left = 736
        Top = 256
        Width = 430
        Height = 41
        Caption = '96. '#50616#47196#46377' '#54616#45800' Z'#52629' '#47588#44144#51652' 1 '#54588#52824' '#45796#50868
        TabOrder = 28
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_18: TBitBtn
        Tag = 87
        Left = 736
        Top = 304
        Width = 430
        Height = 41
        Caption = '97. '#50616#47196#46377' '#54616#45800' Z'#52629' '#47588#44144#51652' 1 '#54588#52824' '#50629
        TabOrder = 29
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_19: TBitBtn
        Tag = 88
        Left = 736
        Top = 352
        Width = 430
        Height = 41
        Caption = '98. '#47196#45908' '#49345#45800' Z'#52629' '#47588#44144#51652' '#49884#51089' '#50948#52824#47196' '#51060#46041
        TabOrder = 30
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_20: TBitBtn
        Tag = 89
        Left = 736
        Top = 400
        Width = 430
        Height = 41
        Caption = '99. '#47196#45908' '#54616#45800' Z'#52629' '#47588#44144#51652' '#49884#51089' '#50948#52824#47196' '#51060#46041
        TabOrder = 31
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_21: TBitBtn
        Left = 736
        Top = 448
        Width = 430
        Height = 41
        Caption = '100. '#50616#47196#45908' '#49345#45800' '#47588#44144#51652' '#54616#45800#50640' '#51060#49569
        TabOrder = 32
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_22: TBitBtn
        Left = 736
        Top = 496
        Width = 430
        Height = 41
        TabOrder = 33
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_23: TBitBtn
        Left = 736
        Top = 544
        Width = 430
        Height = 41
        TabOrder = 34
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan4_24: TBitBtn
        Left = 736
        Top = 592
        Width = 430
        Height = 41
        TabOrder = 35
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual4_12: TPanel
        Left = 664
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 36
        Visible = False
      end
      object pnManual4_13: TPanel
        Left = 664
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 37
        Visible = False
      end
      object pnManual4_14: TPanel
        Left = 664
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 38
        Visible = False
      end
      object pnManual4_15: TPanel
        Left = 664
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 39
        Visible = False
      end
      object pnManual4_16: TPanel
        Left = 664
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 40
        Visible = False
      end
      object pnManual4_17: TPanel
        Left = 664
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 41
        Visible = False
      end
      object pnManual4_18: TPanel
        Left = 664
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 42
        Visible = False
      end
      object pnManual4_19: TPanel
        Left = 664
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 43
        Visible = False
      end
      object pnManual4_20: TPanel
        Left = 664
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 44
        Visible = False
      end
      object pnManual4_21: TPanel
        Left = 664
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 45
        Visible = False
      end
      object pnManual4_22: TPanel
        Left = 664
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 46
        Visible = False
      end
      object pnManual4_23: TPanel
        Left = 664
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 47
        Visible = False
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'ETC..'
      ImageIndex = 4
      object btMan5_13: TBitBtn
        Tag = 102
        Left = 736
        Top = 64
        Width = 430
        Height = 41
        Caption = '122. '#47700#51064' '#50640#50612' On / Off'
        TabOrder = 0
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_14: TBitBtn
        Tag = 103
        Left = 736
        Top = 112
        Width = 430
        Height = 41
        Caption = '123. '#47784#46304' '#51088#51228' '#48176#52636#54616#44592
        TabOrder = 1
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_1: TBitBtn
        Tag = 92
        Left = 152
        Top = 64
        Width = 430
        Height = 41
        Caption = '110. '#48708#51204' '#44160#49324' 1 '#49324#51060#53364
        TabOrder = 2
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_12: TBitBtn
        Tag = 101
        Left = 152
        Top = 592
        Width = 430
        Height = 41
        Caption = '121. '#47784#46304' '#47532#54532#53944' '#54616#44053
        TabOrder = 3
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_2: TBitBtn
        Tag = 91
        Left = 152
        Top = 112
        Width = 430
        Height = 41
        Caption = '111. '#47784#46304' '#48296#53944' On'
        TabOrder = 4
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_3: TBitBtn
        Tag = 92
        Left = 152
        Top = 160
        Width = 430
        Height = 41
        Caption = '112. '#47784#46304' '#48296#53944' Off'
        TabOrder = 5
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_4: TBitBtn
        Tag = 93
        Left = 152
        Top = 208
        Width = 430
        Height = 41
        Caption = '113. '#47784#46304' '#51064#45937#49828' '#49828#53664#54140' '#49345#49849
        TabOrder = 6
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_6: TBitBtn
        Tag = 95
        Left = 152
        Top = 304
        Width = 430
        Height = 41
        Caption = '115. '#52395#48264#51704' '#48708#51204' '#50620#46972#51064#54616#44256' '#47532#54532#53944' '#49345#49849
        TabOrder = 7
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_7: TBitBtn
        Tag = 96
        Left = 152
        Top = 352
        Width = 430
        Height = 41
        Caption = '116. '#46160#48264#51704' '#48708#51204' '#50620#46972#51064#54616#44256' '#47532#54532#53944' '#49345#49849
        TabOrder = 8
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_8: TBitBtn
        Tag = 97
        Left = 152
        Top = 400
        Width = 430
        Height = 41
        Caption = '117. '#49464#48264#51704' '#48708#51204' '#50620#46972#51064#54616#44256' '#47532#54532#53944' '#49345#49849
        TabOrder = 9
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_9: TBitBtn
        Tag = 98
        Left = 152
        Top = 448
        Width = 430
        Height = 41
        Caption = '118. '#53944#47548' '#50620#46972#51064' '#54616#44256' '#54540#47532#54140' '#46244#51665#55192' '#50948#52824#47196' '#51060#46041
        TabOrder = 10
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_24: TBitBtn
        Left = 736
        Top = 592
        Width = 430
        Height = 41
        TabOrder = 11
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual5_0: TPanel
        Tag = 90
        Left = 80
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 12
        Visible = False
      end
      object pnManual5_1: TPanel
        Tag = 91
        Left = 80
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 13
        Visible = False
      end
      object pnManual5_2: TPanel
        Tag = 92
        Left = 80
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 14
        Visible = False
      end
      object pnManual5_3: TPanel
        Tag = 93
        Left = 80
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 15
        Visible = False
      end
      object pnManual5_4: TPanel
        Tag = 94
        Left = 80
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 16
        Visible = False
      end
      object pnManual5_5: TPanel
        Tag = 95
        Left = 80
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 17
        Visible = False
      end
      object pnManual5_6: TPanel
        Tag = 96
        Left = 80
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 18
        Visible = False
      end
      object pnManual5_7: TPanel
        Tag = 97
        Left = 80
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 19
        Visible = False
      end
      object pnManual5_8: TPanel
        Tag = 97
        Left = 80
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 20
        Visible = False
      end
      object pnManual5_9: TPanel
        Tag = 98
        Left = 80
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 21
        Visible = False
      end
      object pnManual5_10: TPanel
        Tag = 99
        Left = 80
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 22
        Visible = False
      end
      object pnManual5_11: TPanel
        Tag = 100
        Left = 80
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 23
        Visible = False
      end
      object btMan5_10: TBitBtn
        Tag = 99
        Left = 152
        Top = 496
        Width = 430
        Height = 41
        Caption = '119. '#53944#47548' '#54540#47532#54140' '#51221#49345' '#50948#52824#47196' '#51060#46041
        TabOrder = 24
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_11: TBitBtn
        Tag = 100
        Left = 152
        Top = 544
        Width = 430
        Height = 41
        Caption = '120. '#47784#46304' '#50620#46972#51064#54616#44256' '#47532#54532#53944' '#49345#49849
        TabOrder = 25
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_15: TBitBtn
        Tag = 104
        Left = 736
        Top = 160
        Width = 430
        Height = 41
        Caption = '124. '#47784#46304' '#51088#51116' '#54620#52856#50473' '#51060#46041#54616#44592
        TabOrder = 26
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_16: TBitBtn
        Left = 736
        Top = 208
        Width = 430
        Height = 41
        Caption = '125. '#48708#51204' '#52376#51020' '#44160#49324' '#50948#52824#47196' '#51060#46041
        TabOrder = 27
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_17: TBitBtn
        Left = 736
        Top = 256
        Width = 430
        Height = 41
        TabOrder = 28
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_18: TBitBtn
        Left = 736
        Top = 304
        Width = 430
        Height = 41
        TabOrder = 29
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_19: TBitBtn
        Left = 736
        Top = 352
        Width = 430
        Height = 41
        TabOrder = 30
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_20: TBitBtn
        Left = 736
        Top = 400
        Width = 430
        Height = 41
        TabOrder = 31
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_21: TBitBtn
        Left = 736
        Top = 448
        Width = 430
        Height = 41
        TabOrder = 32
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_22: TBitBtn
        Left = 736
        Top = 496
        Width = 430
        Height = 41
        TabOrder = 33
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_23: TBitBtn
        Left = 736
        Top = 544
        Width = 430
        Height = 41
        TabOrder = 34
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object btMan5_5: TBitBtn
        Tag = 94
        Left = 152
        Top = 256
        Width = 430
        Height = 41
        Caption = '114. '#47784#46304' '#51064#45937#49828' '#49828#53664#54140' '#54616#44053
        TabOrder = 35
        Visible = False
        OnClick = btMan1_1Click
        Margin = 10
      end
      object pnManual5_12: TPanel
        Tag = 102
        Left = 664
        Top = 64
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 36
        Visible = False
      end
      object pnManual5_13: TPanel
        Tag = 103
        Left = 664
        Top = 112
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 37
        Visible = False
      end
      object pnManual5_14: TPanel
        Left = 664
        Top = 160
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 38
        Visible = False
      end
      object pnManual5_15: TPanel
        Left = 664
        Top = 208
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 39
        Visible = False
      end
      object pnManual5_16: TPanel
        Left = 664
        Top = 256
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 40
        Visible = False
      end
      object pnManual5_17: TPanel
        Left = 664
        Top = 304
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 41
        Visible = False
      end
      object pnManual5_18: TPanel
        Left = 664
        Top = 352
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 42
        Visible = False
      end
      object pnManual5_19: TPanel
        Left = 664
        Top = 400
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 43
        Visible = False
      end
      object pnManual5_20: TPanel
        Left = 664
        Top = 448
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 44
        Visible = False
      end
      object pnManual5_21: TPanel
        Left = 664
        Top = 496
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 45
        Visible = False
      end
      object pnManual5_22: TPanel
        Left = 664
        Top = 544
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 46
        Visible = False
      end
      object pnManual5_23: TPanel
        Left = 664
        Top = 592
        Width = 49
        Height = 41
        BevelInner = bvRaised
        Color = clSilver
        TabOrder = 47
        Visible = False
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'Set-Up'
      ImageIndex = 5
      object GroupBox1: TGroupBox
        Left = 16
        Top = 16
        Width = 417
        Height = 185
        Caption = 'Cylinder Air Control'
        TabOrder = 0
        object btAlignS: TButton
          Left = 24
          Top = 24
          Width = 100
          Height = 25
          Caption = 'Align Stt'
          TabOrder = 0
        end
        object btLiftS: TButton
          Left = 128
          Top = 24
          Width = 100
          Height = 25
          Caption = 'Lift Stt'
          TabOrder = 1
        end
        object btStopperS: TButton
          Left = 232
          Top = 24
          Width = 100
          Height = 25
          Caption = 'Stopper Stt'
          TabOrder = 2
        end
        object btAlignE: TButton
          Left = 24
          Top = 56
          Width = 100
          Height = 25
          Caption = 'Align End'
          TabOrder = 3
        end
        object btLiftE: TButton
          Left = 128
          Top = 56
          Width = 100
          Height = 25
          Caption = 'Lift End'
          TabOrder = 4
        end
        object btStopperE: TButton
          Left = 232
          Top = 56
          Width = 100
          Height = 25
          Caption = 'Stopper End'
          TabOrder = 5
        end
      end
      object GroupBox2: TGroupBox
        Left = 16
        Top = 216
        Width = 385
        Height = 225
        Caption = 'Motor Test'
        TabOrder = 1
        Visible = False
        object Button2: TButton
          Left = 152
          Top = 40
          Width = 75
          Height = 25
          Caption = 'Button2'
          TabOrder = 0
        end
        object Button1: TButton
          Left = 32
          Top = 40
          Width = 75
          Height = 25
          Caption = 'Button1'
          TabOrder = 1
        end
      end
    end
  end
  object Panel26: TPanel
    Left = 0
    Top = 754
    Width = 1272
    Height = 109
    Align = alBottom
    TabOrder = 1
    object btnInit: TBitBtn
      Tag = 1
      Left = 144
      Top = 36
      Width = 377
      Height = 41
      Caption = 'INITIAL'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnInitClick
    end
    object btLotEnd: TBitBtn
      Tag = 122
      Left = 744
      Top = 36
      Width = 377
      Height = 41
      Caption = 'BY MANUAL LOT END'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btLotEndClick
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimerTimer
    Left = 1208
    Top = 24
  end
end
