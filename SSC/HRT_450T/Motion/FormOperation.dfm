object FrmOperation: TFrmOperation
  Left = 2346
  Top = 81
  BorderStyle = bsNone
  Caption = 'FrmOperation'
  ClientHeight = 969
  ClientWidth = 1271
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1271
    Height = 969
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object Panel4: TPanel
      Left = 653
      Top = 511
      Width = 369
      Height = 351
      BevelOuter = bvNone
      TabOrder = 0
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 369
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'MANUAL'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object pcManual: TPageControl
        Left = 0
        Top = 20
        Width = 369
        Height = 331
        ActivePage = TabSheet12
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        MultiLine = True
        ParentFont = False
        TabHeight = 30
        TabIndex = 5
        TabOrder = 1
        TabPosition = tpLeft
        TabWidth = 54
        object TabSheet1: TTabSheet
          Caption = 'LDR'
          object btMan1_1: TBitBtn
            Left = 4
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_2: TBitBtn
            Tag = 1
            Left = 4
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_7: TBitBtn
            Tag = 6
            Left = 4
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_3: TBitBtn
            Tag = 2
            Left = 4
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_4: TBitBtn
            Tag = 3
            Left = 4
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_5: TBitBtn
            Tag = 4
            Left = 4
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_6: TBitBtn
            Tag = 5
            Left = 4
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_8: TBitBtn
            Tag = 10
            Left = 169
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_9: TBitBtn
            Tag = 11
            Left = 169
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_10: TBitBtn
            Tag = 12
            Left = 169
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_11: TBitBtn
            Tag = 13
            Left = 169
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_12: TBitBtn
            Tag = 14
            Left = 169
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_13: TBitBtn
            Tag = 15
            Left = 169
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan1_14: TBitBtn
            Tag = 16
            Left = 169
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            Visible = False
            OnClick = btMan1_1Click
          end
        end
        object TabSheet2: TTabSheet
          Caption = 'PRB'
          ImageIndex = 1
          object btMan2_1: TBitBtn
            Tag = 20
            Left = 4
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_2: TBitBtn
            Tag = 21
            Left = 4
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_7: TBitBtn
            Tag = 26
            Left = 4
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_3: TBitBtn
            Tag = 2
            Left = 4
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_4: TBitBtn
            Tag = 23
            Left = 4
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_5: TBitBtn
            Tag = 24
            Left = 4
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_6: TBitBtn
            Tag = 25
            Left = 4
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_8: TBitBtn
            Tag = 30
            Left = 169
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_9: TBitBtn
            Tag = 31
            Left = 169
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_10: TBitBtn
            Tag = 32
            Left = 169
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_11: TBitBtn
            Tag = 33
            Left = 169
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_12: TBitBtn
            Tag = 4
            Left = 169
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_13: TBitBtn
            Tag = 35
            Left = 169
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan2_14: TBitBtn
            Tag = 36
            Left = 169
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            Visible = False
            OnClick = btMan1_1Click
          end
        end
        object TabSheet3: TTabSheet
          Caption = 'WRK'
          ImageIndex = 2
          object btMan3_1: TBitBtn
            Tag = 40
            Left = 4
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_2: TBitBtn
            Tag = 41
            Left = 4
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_7: TBitBtn
            Tag = 46
            Left = 4
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_3: TBitBtn
            Tag = 42
            Left = 4
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_4: TBitBtn
            Tag = 43
            Left = 4
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_5: TBitBtn
            Tag = 44
            Left = 4
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_6: TBitBtn
            Tag = 45
            Left = 4
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_8: TBitBtn
            Tag = 50
            Left = 169
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_9: TBitBtn
            Tag = 51
            Left = 169
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_10: TBitBtn
            Tag = 52
            Left = 169
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_11: TBitBtn
            Tag = 53
            Left = 169
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_12: TBitBtn
            Tag = 54
            Left = 169
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_13: TBitBtn
            Tag = 55
            Left = 169
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan3_14: TBitBtn
            Tag = 56
            Left = 169
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            Visible = False
            OnClick = btMan1_1Click
          end
        end
        object TabSheet4: TTabSheet
          Caption = 'PSB'
          ImageIndex = 3
          object btMan4_1: TBitBtn
            Tag = 60
            Left = 4
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_2: TBitBtn
            Tag = 61
            Left = 4
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_7: TBitBtn
            Tag = 66
            Left = 4
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_3: TBitBtn
            Tag = 62
            Left = 4
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_4: TBitBtn
            Tag = 63
            Left = 4
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_5: TBitBtn
            Tag = 64
            Left = 4
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_6: TBitBtn
            Tag = 65
            Left = 4
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_8: TBitBtn
            Tag = 70
            Left = 169
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_9: TBitBtn
            Tag = 71
            Left = 169
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_10: TBitBtn
            Tag = 61
            Left = 169
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_11: TBitBtn
            Tag = 62
            Left = 169
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_12: TBitBtn
            Tag = 63
            Left = 169
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_13: TBitBtn
            Tag = 64
            Left = 169
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan4_14: TBitBtn
            Tag = 76
            Left = 169
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            Visible = False
            OnClick = btMan1_1Click
          end
        end
        object TabSheet11: TTabSheet
          Caption = 'ULD'
          ImageIndex = 4
          object btMan5_1: TBitBtn
            Tag = 80
            Left = 4
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_2: TBitBtn
            Tag = 81
            Left = 4
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_7: TBitBtn
            Tag = 86
            Left = 4
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_3: TBitBtn
            Tag = 82
            Left = 4
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_4: TBitBtn
            Tag = 83
            Left = 4
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_5: TBitBtn
            Tag = 84
            Left = 4
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_6: TBitBtn
            Tag = 85
            Left = 4
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_8: TBitBtn
            Tag = 90
            Left = 169
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_9: TBitBtn
            Tag = 91
            Left = 169
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_10: TBitBtn
            Tag = 92
            Left = 169
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_11: TBitBtn
            Tag = 93
            Left = 169
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_12: TBitBtn
            Tag = 94
            Left = 169
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_13: TBitBtn
            Tag = 95
            Left = 169
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan5_14: TBitBtn
            Tag = 6
            Left = 169
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            Visible = False
            OnClick = btMan1_1Click
          end
        end
        object TabSheet12: TTabSheet
          Caption = 'ETC'
          ImageIndex = 5
          object btMan7_1: TBitBtn
            Left = 4
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_8: TBitBtn
            Tag = 10
            Left = 169
            Top = 0
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_2: TBitBtn
            Tag = 1
            Left = 4
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_9: TBitBtn
            Tag = 11
            Left = 169
            Top = 46
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_3: TBitBtn
            Tag = 2
            Left = 4
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_10: TBitBtn
            Tag = 12
            Left = 169
            Top = 92
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_4: TBitBtn
            Tag = 3
            Left = 4
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_11: TBitBtn
            Tag = 13
            Left = 169
            Top = 138
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_5: TBitBtn
            Tag = 4
            Left = 4
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_12: TBitBtn
            Tag = 14
            Left = 169
            Top = 184
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_6: TBitBtn
            Tag = 5
            Left = 4
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_13: TBitBtn
            Tag = 15
            Left = 169
            Top = 230
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_7: TBitBtn
            Tag = 6
            Left = 4
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            Visible = False
            OnClick = btMan1_1Click
          end
          object btMan7_14: TBitBtn
            Tag = 16
            Left = 169
            Top = 276
            Width = 162
            Height = 46
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            Visible = False
            OnClick = btMan1_1Click
          end
        end
      end
    end
    object Panel6: TPanel
      Left = 1023
      Top = 511
      Width = 256
      Height = 351
      BevelOuter = bvNone
      TabOrder = 1
      object Panel7: TPanel
        Left = 0
        Top = 0
        Width = 256
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'OPERATION'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object btStart: TBitBtn
        Left = 1
        Top = 21
        Width = 255
        Height = 82
        Caption = '  START  '
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -33
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = btStartClick
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7DFDFDFC1C1
          C1A2A2A2878787757575747474828282989898B5B5B5D6D6D6F2F2F2FFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1EFEFC9BFBB9C7969965638A956
          2DBB5829C35A27C15A27BC5625B053249A4A2484462B6F48367A6B63B0ADABE4
          E4E4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFBFBFBD5CBC7A7745DB65B30DE7135F18749F797
          56FBA365FDAF70FDB176FDAB6EFA9D5EF68E4CEA7B3DD2642EA14D2378432C73
          5F58B7B5B4F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFF3F1F0C0A599B76036E97C3EF89A5DFDAD71FCAA6CFA9F
          5FF89755F68E4DF38B49F38949F48C4AF69250F99957FA9C5CF48C4BDD6D33A4
          4D256C4433948D8AE2E2E2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFF1EEEDBD907BCC6733F79958FCAC70FBA666F89756F69150F692
          50F69250F68F4FF68E4CF48B4AF38849F28745F08241F18241F68E4BF99756F3
          8B4AC75C287A4129837771DBDBDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFF5F3F1C1917ADF7238FBA565FDAF71F89857F69250F79353F79352F793
          52F69351F6914FF68F4DF48D4BF38B4AF38847F28645F08342EF7E3EEF8040F8
          9351F89452D76730864428857872E2E2E2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FDFCFCCDA897DF7238FBA668FBA768F79352F79352F79453F79353F79452F693
          51F6924FF69150F68F4DF48D4BF48B4AF3894AF28645F08343EF8142EE7E3FEC
          7B3DF48C4AF79352DB68307E432A958E8AF3F3F3FFFFFFFFFFFFFFFFFFFFFFFF
          E3D2C9CD6A37FBA565FBA668F69251F79352F79451F79453F79451F69351F693
          52F69250F6914FF48F4DF48E4CEA8042F49157F28746F08343EF8142EF7F3FEC
          7C3EEB7A3BF48B49F89452C75F2B724734B7B5B5FFFFFFFFFFFFFFFFFFF9F6F4
          CA7D57F79656FCAC70F79350F69352F79351F79351F69352F79351F69352F691
          51F69150F69150F68E4DF68E4DE2783DFDE9DDF38F52F08544F08240EF7F3EEC
          7C3DEC7B3DEB793AF6914FF48B4BAF5228786259E5E5E4FFFFFFFFFFFFE2C8BB
          E7793BFCAD70F79455F69150F69350F69250F69250F69250F69250F69251F691
          50F6924FF68E4FF68F4DF68E4CE2783DFFFFFFFDE9DCF28C50F08140EF7F3EEC
          7C3DEC7B3CEA773AEB793CF89653E3733785492EB1AEACFFFFFFFDFCFCCF8561
          F89C5DFBA262F48E4CF69150F6914DF6914FF6914FF68F4FF6914FF6914FF691
          4FF6914FF68E4CF48E4CF48E4DE2783DFFFFFFFFFFFFFCE8DCF18A4DEF7F3FEC
          7C3DEC7A3CEB783BE87438F28645F68E4DB7562A7F6C64F2F2F2F3E8E3E07138
          FCAF73F69250F48F4DF48E4DF48E4DF48E4CF48E4DF48E4DF48E4CF48D4BF48E
          4CF48E4CF48E4DF48D4BF48C4BE1773DFFFFFFFFFFFFFFFFFFFCE7DBED874DEC
          7A3CEB783AE97839E97739E97738F99856DE6D337D4F3AD5D5D5E7C7B7F18747
          FBA869F48C4AE2783EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBE6DAEB
          8249EA773AE87538E97538E57235F68D4BEE8041975130B5B5B5DDA387F89A59
          F99A59F48C4AE2773DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB
          E7DBEA7F46E57336E77436E57135EF8041F48C4CB0562C979797D9865DFBAB6E
          F69250F38847E37B40FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFDE9DDEB854BE57235E57034EA773AF99756C85F2C828282D76E3AFDB57D
          F3894AF69757E68B54FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFDECE0F18F54E57335E36F33FBA15FD5652E747474D86C36FDBA82
          F89E5FFCB983E79564FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFDE1CDF38F51E57436FBA362D7692F757575DE8052FDB57B
          FDBC87FDC493E79562FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFEF1E7FAB582FAAD75F39455FBA565D7672F878787E59F7CFCAB6C
          FDC08DFDC18FE79564FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFEF1E7FAB27CFAAC71FBB37BFAAD75FAA263CF642FA2A2A2EEC2ADF99D5D
          FDBC86FDC290E79664FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE
          F1E7FAB27CFAAC71FBAD75FBB37AFBB17AF89757BD6134C1C1C1F8E4DAF38C4B
          FDBA83FDC290E79665FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEF1E7FA
          B27DFBAC73FBAF75FBB077FBB581FCB47BF2894AA9623FDFDFDFFEFAF8EC8E5B
          FDB47AFDBE8CE79765E79564E79360E7925EE7905BE68F5AE68E57E68B55E68A
          54E68952E68950E68851E68851E68851FFFFFFFFFFFFFFFFFFFEF1E7FBB47EFB
          AD75FBAD76FBB17AFBB37DFBB37DFCAD71E97A3DAA816EF7F7F7FFFFFFF1C4AC
          F99D5EFDBA86FDC290FDC08DFDBE8CFDBC86FCB882FCB581FCB47BFBB17AFBB0
          76FBAF73FBAD73FBAC73FAAC71E68A53FFFFFFFFFFFFFEF1E8FBB583FBB076FB
          B17AFBB37BFBB47DFCB782FCB57DF89A59C86A3ACDC2BCFFFFFFFFFFFFFCF2ED
          F29258FDB377FDBC87FDC18FFDBE8CFDBD89FDBC86FCB783FCB581FCB57EFBB4
          7AFBB177FBB077FBB076FBAF76E68C57FFFFFFFEF2E9FBB887FBB17BFBB47AFB
          B57BFBB77EFCBA83FBB57EFCAF73F08545B88065F1F0EFFFFFFFFFFFFFFFFFFF
          F4CCB7F89958FDBC83FCBD8CFDC08DFDBE8CFDBD89FCBC87FCB983FCB882FCB5
          7EFCB57EFBB47DFCB47BFBB37BE68F5BFEF2E9FCBA87FCB57EFCB57DFCB87EFC
          B882FCB986FCB982FCB87EF89C5CCC7041D9CFCAFFFFFFFFFFFFFFFFFFFFFFFF
          FEFBF9F0AE8BFAA565FDBA83FDBE8CFDC08DFDBE8DFDBC8CFDBA87FCBA86FCBA
          83FCB983FCB883FCB881FCB781F0A26DFCBE8DFCB782FCB982FCBA83FCBA83FC
          BA86FCB986FCB881FBA668DF7841CBAD9EFBFBFBFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFCF0EAF2A274FBA768FDBA82FCBC87FDBE8DFDBE8CFDBD8CFDBD89FDBC
          89FDBC87FDBA86FCBA86FCBA86FCB986FCBA86FCBC86FCBA87FCBC89FCBD8CFC
          BA86FCB87EFBAA6CEE8546CD9C83F4F2F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFBECE4F2A376FBA566FDBA83FCB982FDC08DFDBE8CFDBE8CFDBE
          8CFDBD8CFDBD89FDBD89FDBD89FDBD89FDBD89FDBD89FDBE8CFDBE8CFCBA83FD
          B981FBAA6CEF8749D4A084F3EFEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFCF0EAF0B18BF99C5CFDB477FDB781FCBC87FDBE8DFDBD
          8CFDBD8CFDBE8CFDBD8CFDBE8CFDBE8CFDBE8CFDBD89FCBA87FCB77EFDB57BFA
          A262E58247DCB49FF6F3F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFEFBF9F6CFBAF5965DFAA162FDB47AFDB77EFCB5
          7EFCB883FCBC86FDBC8CFCBA87FCB983FCB77EFCB77EFDB57BFBA666F48D4DE0
          9267EAD7CDFDFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF2EDF4C8AFF19760F79655FBA3
          63FCAF71FDB77EFDBC83FDB981FCB075FBA666F89857F28A4CE5996EECCFC0FA
          F7F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFBF8FAE6DCF4C9
          B2EFAB85ED915EEB8044EA8348EB976AEBB090F0CFBDF7EBE5FEFDFCFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      end
      object btStop: TBitBtn
        Left = 1
        Top = 104
        Width = 255
        Height = 82
        Caption = '   STOP   '
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -33
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = btStopClick
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBE3E3E3B0B0B08080807070707070
          70707070707070707070707070707070707070707070707070707070878787C2
          C2C2F0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFBFBFBE3E3E37474950000620000620000620000
          6200006200006200006200006200006200006200006200006200006247475180
          8080C4C4C4F0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFBFBFBE3E3E374749501016228309E6270E46B78E67380
          E77986E87D89E97E8AE97D89E97986E87380E76B78E66270E42329970A0A5D4A
          4A54828282C4C4C4F0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFBFBFBE3E3E374749501016220289B4F5FE14254DF2A3FDB3347
          DD3A4DDE3E51DE4053DF3E51DE3A4DDE3347DD2A3FDB4556E04F5FE11B22930A
          0A5D4A4A54828282C4C4C4F0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFBFBFBE3E3E37474950101621C24994153DF3043DC132AD71B31D92238
          DA273CDB2A3FDB2B40DB2A3FDB273CDB2238DA1B31D9142BD73346DD4153DF17
          1E910A0A5D4A4A54828282C4C4C4F0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFF
          FBFBFBE3E3E37474950101621C24993E50DE273BDA011AD4071FD5293EDB7582
          E8B2BAF2E1E4FAFBFBFEE1E4FAB2BAF27582E8293EDB071FD5021BD42A3EDB3E
          50DE171E910A0A5D4A4A54828282C4C4C4F0F0F0FEFEFEFFFFFFFFFFFFFBFBFB
          E3E3E37474950101621C24993E50DE273BDA0019D40019D45364E2BFC4F4FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFC4F45364E20019D4011AD42A
          3EDB3E50DE171E910A0A5D4A4A54828282C4C4C4F0F0F0FEFEFEFBFBFBE3E3E3
          7474950101621C24993E50DE273BDA0019D40019D45868E3DEE1F9FFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDEE1F95868E30019D401
          1AD42A3EDB3E50DE181F920A0A5D4A4A54828282C4C4C4F0F0F0ECECEC767697
          0101621C24993E50DE273BDA0019D40019D42B40DBC8CDF6FFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC8CDF62B40DB00
          19D4011AD42A3EDB3E50DE181F920A0A5D4A4A54808080C2C2C29090B1020264
          1C24993E50DE273BDA0019D40019D40019D47A87E9FFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7A87E900
          19D40019D4011AD42A3EDB3E50DE181F920A0A5D4747518787870000621C2499
          3E50DE273BDA0019D40019D40019D40013B8BCC3F4FFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCC3F400
          19D40019D40019D4011AD42A3EDB3E50DE181F920000627070700000623E50DE
          273BDA0019D40019D40019D40019D40013B8ECEEFCFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFECEEFC00
          19D40019D40019D40019D4011AD42A3EDB3E50DE0000627070700000623E50DE
          0019D40019D40019D40019D40019D4000FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9FAFEFFFFFFFFFFFF00
          19D40019D40019D40019D40019D40019D43E50DE0000627070700000623E50DE
          0019D40019D40019D40019D40019D4000FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3F4FDC3C9F5FFFFFFFFFFFF00
          19D40019D40019D40019D40019D40019D43E50DE0000627070700000623E50DE
          0019D40019D40019D40019D40019D4000FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC9CEF6B9C0F3FFFFFFFFFFFF00
          19D40019D40019D40019D40019D40019D43E50DE0000627070700000623E50DE
          0019D40019D40019D40019D40019D4000FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9CA6EEA0A9EFFFFFFFFFFFFF00
          19D40019D40019D40019D40019D40019D43E50DE0000627070700000623E50DE
          0019D40019D40019D40019D40019D4000FA4FFFFFFFFFFFFDEE1F9FFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF515CC5A6AFF0FFFFFFFFFFFF00
          19D40019D40019D40019D40019D40019D43E50DE0000627070700000623E50DE
          0019D40019D40019D40A22D61E34D91C28A9FFFFFFFFFFFF929DEDFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF717FE76E76CBFFFFFFFFFFFF0A
          22D60019D40019D40019D40019D40019D43E50DE0000627070700000623E50DE
          172ED83549DD5162E26C7AE67A87E9474EB0FFFFFFFFFFFF5C66C8FFFFFFFFFF
          FFD3D7F8FFFFFFFFFFFFEEEFFCFFFFFFFFFFFF818EEA5C66C8FFFFFFFFFFFF6C
          7AE65162E23549DD172ED80019D40019D43E50DE0000627070700000623E50DE
          717FE87A88EB7A88EA7A87EA7A87EA474EB0FFFFFFFFFFFF5C66C8FFFFFFFFFF
          FF7A87E9FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E95C66C8BDC3F4BDC3F47A
          87E97A87E97A87E9717FE74E5FE1283DDB3E50DE0000627070700000626170E6
          7B89EC7B89ED7B8AED7B89ED7B88EC474EB1FFFFFFFFFFFF5C66C8FFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E95C66C85C66C87A
          87E97A87E97A87E97A87E97A87E97A87E94C5DE10000627070700000626876E8
          7280EC7D8AF17D8BF17D8CF17C8AEF474FB3FFFFFFFFFFFF5C66C8FFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E97A87E97A87E97A
          87E97A87E97A87E97A87E97A87E96977E65767E30000628585850000622C33A3
          6272EB7484F17F8CF4808DF57E8BF2484FB4FFFFFFFFFFFF5C66C9FFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E97A87E97A87E97A
          87E97A87E97A87E97A87E96977E64C5DE11B2398000062BCBCBCA8A8C9050567
          2C33A66474F17382F68190F9808DF64A51B8FFFFFFFFFFFF5C66C8FFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E97A87E97A87E97A
          87E97A87E97A87E96977E64C5DE11B23980C0C65A0A0AFEDEDEDFFFFFFA8A8C9
          0505692D35A96476F27482F8828FF84A51BAC5CBFEC1C7F95C67CAFFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E97A87E97A87E97A
          87E97A87E96977E64C5DE11B23980C0C65A0A0AFEDEDEDFEFEFEFFFFFFFFFFFF
          A8A8C9050568141A936C7DF47483F88190F9606BD25E69CF5D67CBFFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E97A87E97A87E97A
          87E96977E64C5DE11B23980C0C65A0A0AFEDEDEDFEFEFEFFFFFFFFFFFFFFFFFF
          FFFFFFA8A8CA0505692E35AA6677F37482F7818EF67E8CF25D68CCFFFFFFFFFF
          FF5C66C8FFFFFFFFFFFF5C66C8FFFFFFFFFFFF7A87E97A87E97A87E97A87E969
          77E64C5DE11B23980C0C65A0A0AFEDEDEDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFA8A8CA0505692E35AB6575F07281F47E8CF25D68CCBFC5F7BDC3
          F45C66C8FFFFFFFFFFFF5C66C8BDC3F4BDC3F47A87E97A87E97A87E96977E64C
          5DE11B23980C0C65A0A0AFEDEDEDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFA8A8CA0505692E35A86473ED707EEE7B89ED5C66C95C66
          C85C66C8BDC3F4BDC3F47A87E95C66C85C66C87A87E97A87E96977E64C5DE11B
          23980C0C65A0A0AFEDEDEDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFA8A8CA0505682C33A56171E86E7CEA7A87EA7A87
          E97A87E95C66C85C66C87A87E97A87E97A87E97A87E96977E64C5DE11B23980B
          0B64A0A0AFEDEDEDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA8A8C90505662C33A16170E56170E46170
          E46170E46170E46170E46170E46170E46170E46170E46170E41B23980D0D669F
          9FAFEDEDEDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA8A8C90000620000620000620000
          62000062000062000062000062000062000062000062000062000062B9B9C9EF
          EFEFFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      end
      object btReset: TBitBtn
        Left = 1
        Top = 187
        Width = 255
        Height = 82
        Caption = '  RESET  '
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -33
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnClick = btResetClick
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF2F2F2E8E8
          E8DEDEDED7D7D7D2D2D2D2D2D2D7D7D7DEDEDEE8E8E8F2F2F2FCFCFCFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBEFEFEFD9D9D9BABABAA0A0
          A08D8D8D7E7E7E7676767676767E7E7E8D8D8DA0A0A0BABABAD9D9D9EFEFEFFB
          FBFBFFFFFFFFFFFFFFFFFFF9F9F9F3F3F3F9F9F9FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF6F6F6DDDDDDB6B6B6677E68355F381A53
          1D0B510F035307025006094F0D154E1827512A4157426B6B6B8C8C8CB6B6B6DD
          DDDDF6F6F6FEFEFEF9F9F9DCDCDCCACACAE7E7E7FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFEFEFEF2F2F2D1D1D169856B235A261B6F1F429A4862B8
          697ACD8189DC9189DC917ACD8162B869429A481B6F1F164E193652376C6C6C9D
          9D9DD1D1D1ECECECDADADA517A54949494D5D5D5FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFEFEFEF4F4F4B8C1B944704717681C4CA3526CCA745AC9633BBD
          4621B12D10A91C10A91C21B12D3BBD465AC9636CCA744CA35216681A204C2355
          5E55909090B2B2B24D7650005104767676CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFF9F9F9BCC7BD396D3C3B89415EBB6651C65B1FB02A09A71509A7
          1509A71509A71509A71509A71509A71509A7151FB02A51C65B5EBB66277E2C1A
          4E1D4D584E355F383A913F075A0C707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFEFEFEE1E4E24676495397589ED5A269C97109A71509A71509A71509A7
          1509A71509A71509A71509A71509A71509A71509A71509A71538BB435DBF6427
          7E2C0E4F123A913F8DDE94095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFF8F8F8769878387F3B9FD4A398D79C7BCA8249BA5209A71509A71509A7
          1509A71509A71509A71509A71509A71509A71509A71509A71509A71534B93E5A
          BA61469C4C58C86272D47B095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFD0DAD125662990C294A5DDA989CF8F91D2969EDAA291DA9781D98989DE
          9189DE917AD78363CD6D44C04E1FB02A09A71509A71509A71509A71509A71546
          C1505AC96209A71572D47B095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF88A98A4B8C4DB2E0B696D49AA8DDACA3D6A686BD8A5B9C5E25752A0858
          0C055709156A1B2E85344CA3525DBD6348C25211AA1C09A71509A71509A7150C
          A81809A71509A71577D680095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF5084538ABD8EB1E1B59CD3A078AE7B497C4C7FA282B5C8B6DFE7E0F8F9
          F9E7EFE8C1D4C28EB190538856145E18358C3A42B44B18AD2309A71509A71509
          A71509A71509A7157AD782095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF256829AED7B193CB9868966BA4B7A6F2F2F2FCFCFCFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFBDD0BF5585581062143FB44909A71509A71509
          A71509A71509A7157AD782095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF0A580E9FC9A2749375DDDDDDFAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFF9F9F9DCDCDC517A544395476DCD761CAC2710A91C09
          A7150FA91B19AB2488DA8F095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF276B2BA7C2A9E0E0E0F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFF9F9F9DCDCDC517A54509B53A2DDA77FCB857DCA8379CA8077
          C97D74C77A6FC676ADE5B1095C0D707070CFCFCFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF9BBA9DF4F4F4FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFF3F3F3668F68519C56C0EBC3B8E8BDB7E7BBB5E6B9B3E6B8B2
          E5B7B0E5B5AFE5B3B6E9BA095C0D949494DBDBDBFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFE9EDEADADADACFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCF
          CFCFCFCFCFCFCF69916C00510400510400510400510400510400510400510400
          5104005104005104005104005104DADADAF2F2F2FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFDBDBDB9494947070707070707070707070707070707070707070707070
          70707070767676949494CACACAF3F3F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFAFAFAF4F4F4FAFAFAFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF0051040051040051040051040051040051040051040051040051040051
          04005104005104517A54DCDCDCF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFAFAFAD9DDDAD3D3D3ECECECFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF10611497E19C77D68077D68077D68077D68077D68077D68077D6808DDE
          943A913F517A54DCDCDCF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFAFAFAE0E0E06B8B6DA2A2A2DBDBDBFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF116115A7E4AB0CA81809A71509A71509A71509A71509A71558C8623A91
          3F456E48CFCFCFF6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC
          FCFCF2F2F2D7D7D76F9471135717838383D4D4D4FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF136217BDE9C14BBB5309A71509A71509A71509A71557C7613A913F2851
          2B808080B6B6B6DADADAEBEBEBF4F4F4FAFAFAFDFDFDFAFAFAF4F4F4EBEBEBDA
          DADABBBBBB5479574D9F53045108838383D8D8D8FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF136217C1EAC389CF8F47B95009A71509A71509A7153FB4490E61121C4D
          1F4255437070708E8E8EA7A7A7B9B9B9C5C5C5CBCBCBC5C5C5B9B9B9A7A7A76C
          806D376D393AAA4362B8680E5212949494E1E1E1FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF146318C4EBC798D49B8CD0914EBD580FA91B09A71518AD2342B44B358C
          3A06510A194E1C2E51314659475B635B6A6B6A656D655366543A5D3D20562339
          8C3E42B44B50C55A429A48245727ADADADECECECFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF146318C6ECCA9DD6A19AD49D95D39A61C26928B13209A71511AA1C48C2
          525DBD634CA3522E8534156A1B055709055709156A1B2E85344CA3525DBD6348
          C25215AC205EC6671B6F1F4D6E50CCCCCCF8F8F8FFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF156319C8ECCBA3D8A8B2E3B6B0E0B396D39B7ECB844DBC561CAC2709A7
          151FB02A44C04E63CD6D7AD78389DE9189DE917AD78363CD6D44C04E1FB02A09
          A71546C1504CA35215561997A198E9E9E9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF156319CBEDCEAFE1B360A964A2D4A6ABDDAF98D49B92D19779CA7F4EBD
          5829B13409A71509A71509A71509A71509A71509A71509A71509A71509A71534
          B93E5ABA61146818587B5AD8D8D8F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF156319C9EECC4496492060244D9452AADAAEAEDFB19AD49D95D39A8FD1
          9488CE8E6CC5724EBD5839B64325B03118AB230FA91B09A7150DA81942BD4C67
          C26E2C81313C6D3FC7CAC7F4F4F4FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF0C5D1146974B517A54BEC9BF598C5C4E9452A6D6A9B8E4BBA3D9A796D3
          9B92D1978CD09186CD8C81CC877BCA8277C97D71C77877CA7E90D79685C98A39
          883E3B6E3EB6C1B7F2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF005104668F68DCDCDCF9F9F9E3ECE4729E75327B3789BF8EB7E3BAB8E5
          BCAADDAE9CD7A191D1968CD0918FD29596D79BA1DDA79FDAA472B47724712954
          8157C6CFC7F6F6F6FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF7EA781F3F3F3F9F9F9FFFFFFFFFFFFFFFFFFA8C3AA4E855136803A7AB4
          7EA1D1A5B8E2BBC3ECC6C2EBC5B1E0B596CC9A6CAD702E7B323D744192AD94EF
          EFEFFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB9CFBB79A2
          7C457E481F662308570C07560B1E63224079436F9872AAC0ABF2F2F2FCFCFCFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      end
      object btOperator: TBitBtn
        Left = 1
        Top = 270
        Width = 255
        Height = 82
        Caption = 'OPERATOR'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -33
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        OnClick = btOperatorClick
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C000000000000000000000000000000000000DDDDDDDDDDDD
          D7D7D7CFCFCFCFCFCFD0D0D0DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDC6C6C6
          898989707070707070727272989898DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDABCFDF027EB4
          08A1D108A0D108A0D1067BAC757575D2D2D2DDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDABCFDF0484B9
          13D7FF13D7FE13D7FF067CAE757575D2D2D2DDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDADD2E20385BA
          16D8FE16D8FF16D8FF057DB0686868ADADADCFCFCFD3D3D3E2E2E2DDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDB1D6E70386BC
          61E4FE12B8E419D8FE067FB24D4D4D656565707070777777989898CECECEDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD188FC0
          15BDE863E5FF16B8E30DA5D60DA6D50DA5D60B9FD0157299565B5D8E8E8ECECE
          CEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          1891C317BEE866E5FE18B9E33DDFFE22D9FF22DAFF12B0DD14749B565B5D8E8E
          8ECECECEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDD1893C51ABFE969E5FE1BB9E341DFFE27DAFE27DAFE14B1DF15769C565B
          5D979797E0E0E0DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDD1896C71EC1EA6CE6FD1FBAE35CE3FE2BDAFE2CDBFD17B3E01477
          9F737576CECECEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDD1897C921C1EB7EE9FE22BAE25FE3FE32DCFE31DCFE099B
          CD677073CACACADDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDD189ACB26C3EC82E9FE26BAE374E7FE37DDFE0B9D
          D15C6568A7A7A7CDCDCDD2D2D2DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD199DCE29C5ED84EAFE2ABBE378E7FD0CA0
          D34750546262626F6F6F7474748F8F8FBBBBBBD7D7D7D8D8D8D4D4D4D4D4D4DA
          DADADDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD18A0D12DC6ED92ECFD2EBBE258C1
          E30296CD0196CE0197CE1085B04659616D6D6D83838382828278787878787884
          8484989898B4B4B4D6D6D6DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD18A2D531C8EF95ECFD32BC
          E28BEBFD5FE2FD49DEFD36CEF217ABDA177EA4108BB80595C90595C90F89B51F
          7999386C7F646B6C8E8E8EBEBEBEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD18A5D635CAEE99ED
          FD36BDE28FEBFD63E4FD4EE0FD4EDFFC49DBF93ED2F44ADCFB49DBFB3DD1F42D
          C2EA16AEDF177FA43D68777A7A7AB4B4B4DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD18A8D939CB
          F09CEDFD3ABDE292ECFD55E0FC54E0FC55E1FD55E0FC55E0FD55E1FD55E0FC55
          E1FC51DDFB2FC3EB169BC6346C7F7A7A7ABEBEBEDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD18AB
          DC44D1F35BE2FC5AE1FC5BE1FC5BE1FC5BE1FC5AE1FC5BE1FC5AE1FC5AE1FC5A
          E1FC5AE1FC5BE1FC3ACAF0179ECA3D6A788E8E8ED6D6D6DDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDC3E1
          EB47CEF160E2FC60E1FC60E2FC61E2FC60E2FC61E2FC60E2FC60E2FC60E2FC61
          E2FB61E2FC61E2FC60E2FB35C6EE1785AB646B6DB4B4B4DDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD97CF
          E254D8F766E3FC66E3FB66E3FC66E3FB66E2FB66E2FC66E3FB66E3FC66E3FC62
          E0FA59DBF766E3FC66E3FC61E0FB1CB8E7387184989898DDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD5ABF
          E04DD4F56CE3FB6CE3FB6CE3FB6BE4FB6CE3FC6CE3FC6BE3FB6CE3FC6BE3FC62
          DEF92BBBE55EDCF86BE3FB6CE4FB3CCBF01F84A5848484DADADADDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD2BB5
          E382E2F882E7FB71E4FB71E5FB71E4FB71E4FB71E5FB71E5FB71E4FB71E4FB51
          D4F51D99C23BC9F172E4FB71E4FB58D8F60E99C7787878D4D4D4DDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD0DB3
          E8C9F4FDC4F4FDA7EEFC8EE9FC77E5FB76E5FB76E5FB76E4FB76E4FB77E5FB34
          C8F04E9BB31BBDEC77E5FB76E5FB6FE2FA04ACE1787878D4D4D4DDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD0CB6
          EBD0F6FED5F7FED1F6FDCEF5FEB7F1FDA1EDFC8DE9FC7CE6FA7BE6FB55D6F533
          ADD1A3B8BE03B4EB7BE5FB7BE5FB74E3FA05AEE4848484DADADADDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD2DBF
          EDBEF0FCDCF8FED9F8FED5F7FDD2F6FDCEF5FDCBF5FDB9F1FC74DCF844B7DA90
          B8C599C1CE1DC0F17FE6FA80E7FB64DBF810A2D0989898DDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD61CF
          F098E6FAE1F9FEDFF8FEDCF8FED0F5FCC4F2FC97E4FA5BD3F639AED278A2AEA8
          A8A847A7C45AD4F6A4EDFB9BECFB57D5F72A98B8B4B4B4DDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DD56D3F7E0F8FEE4FAFEE1F9FEC2F0FD51C9EE1399C22C7E9751676D4E788530
          91AE4EC7EAADEDFCC3F3FCB9F0FC40CDF65F9DB1DDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DD50CEF49EE8FBE9FBFEE6FAFEE3FAFEC5F1FD73DCFA32C9F806BFF530C9F76D
          DBF9B5EFFCCCF5FDC9F4FD7EE0FA29A7CDB7BFC1DDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDD4ACFF7B4EEFCEBFBFEE9FAFEE6FAFEE3F9FEE1F9FEDEF8FEDBF8FDD8
          F7FDD5F7FDD2F6FD97E7FB33C0E883B7C7DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDD4AD1F8A1E9FCE7FBFEEAFBFEE8FAFEE5FAFEE2F9FEDFF9FEDD
          F8FDD5F6FD8DE5FC38C3EB7CBFD3DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDD4FD2F85AD8FCA1EAFDCCF4FEE1F9FEDEF8FEC4F2FD96
          E7FC51D6FB3BBDE49CD1E0DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD61D5F72DCAF80CC4FB0CC4FB2AC7F559
          CDEF96D8ECDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD}
      end
      object plPassWord: TPanel
        Left = 94
        Top = 194
        Width = 161
        Height = 156
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Visible = False
        object Panel19: TPanel
          Left = 1
          Top = 1
          Width = 159
          Height = 29
          Align = alTop
          Color = clPurple
          Font.Charset = ARABIC_CHARSET
          Font.Color = clWindow
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label17: TLabel
            Left = 14
            Top = 4
            Width = 113
            Height = 22
            Caption = 'PASSWORD'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindow
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object btPasswordClose: TBitBtn
            Left = 132
            Top = 4
            Width = 23
            Height = 22
            ModalResult = 3
            TabOrder = 0
            OnClick = btPasswordCloseClick
            Glyph.Data = {
              DE010000424DDE01000000000000760000002800000024000000120000000100
              0400000000006801000000000000000000001000000000000000000000000000
              80000080000000808000800000008000800080800000C0C0C000808080000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
              333333333333333333333333000033338833333333333333333F333333333333
              0000333911833333983333333388F333333F3333000033391118333911833333
              38F38F333F88F33300003339111183911118333338F338F3F8338F3300003333
              911118111118333338F3338F833338F3000033333911111111833333338F3338
              3333F8330000333333911111183333333338F333333F83330000333333311111
              8333333333338F3333383333000033333339111183333333333338F333833333
              00003333339111118333333333333833338F3333000033333911181118333333
              33338333338F333300003333911183911183333333383338F338F33300003333
              9118333911183333338F33838F338F33000033333913333391113333338FF833
              38F338F300003333333333333919333333388333338FFF830000333333333333
              3333333333333333333888330000333333333333333333333333333333333333
              0000}
            NumGlyphs = 2
          end
        end
        object btEngr: TBitBtn
          Tag = 1
          Left = 5
          Top = 72
          Width = 150
          Height = 41
          TabOrder = 1
          OnClick = btOperClick
          Glyph.Data = {
            76470000424D7647000000000000360000002800000097000000280000000100
            1800000000004047000012170000121700000000000000000000FFFFFF8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF00
            00008F8F8F8F8F8F808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            808F8F8F8F8F8F0000008F8F8FFFFFFFFFFFFF81818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFE8080808080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            9797979798989800000000000000000000000000000000000000000000000000
            0000A1A1A1A2A2A2A3A3A3000000000000A5A5A5A6A6A6A7A7A7A8A8A8000000
            000000ABABABABABABACACACADADADAEAEAEAFAFAF0000000000000000000000
            00000000B4B4B4000000000000B7B7B7B8B8B8B9B9B9B9B9B9000000000000BC
            BCBCBDBDBDBEBEBEBFBFBF000000000000C1C1C1C2C2C2C3C3C3C4C4C4000000
            000000C6C6C6C7C7C7C8C8C8C9C9C90000000000000000000000000000000000
            00000000000000000000D2D2D2D3D3D3D4D4D400000000000000000000000000
            0000000000000000000000000000DCDCDCDDDDDDDEDEDE000000000000E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5000000000000E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898980000000000009B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3000000000000A5A5A5A6A6A6A7
            A7A7000000000000000000ABABABABABABACACACADADAD000000000000000000
            B1B1B1B2B2B2B2B2B2000000000000000000000000B7B7B7B8B8B8B9B9B9B9B9
            B9000000000000BCBCBCBDBDBDBEBEBEBFBFBF000000000000C1C1C1C2C2C2C3
            C3C3000000000000000000C6C6C6C7C7C7C8C8C8C9C9C9000000000000CCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D40000000000
            00D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDE00
            0000000000E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4000000000000E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898980000000000009B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A30000000000
            00A5A5A5A6A6A6A7A7A7000000000000000000ABABABABABABACACACADADAD00
            0000000000B0B0B0B1B1B1B2B2B2B2B2B2B3B3B3000000000000000000B7B7B7
            B8B8B8B9B9B9B9B9B9000000000000BCBCBCBDBDBDBEBEBEBFBFBF0000000000
            00C1C1C1C2C2C2C3C3C3000000000000000000C6C6C6C7C7C7C8C8C8C9C9C900
            0000000000CCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDE000000000000E1E1E1E1E1E1E2E2E2E3E3E3E4E4E400000000
            0000E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989800
            00000000009B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3000000000000A5A5A5A6A6A6000000000000000000000000ABABABABAB
            ABACACAC000000000000AFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B400
            0000000000B7B7B7B8B8B8B9B9B9B9B9B9000000000000BCBCBCBDBDBDBEBEBE
            BFBFBF000000000000C1C1C1C2C2C2000000000000000000000000C6C6C6C7C7
            C7C8C8C8C9C9C9000000000000CCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDE000000000000E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4000000000000E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898980000000000009B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3000000000000A5A5A5A6A6A6000000000000000000
            000000ABABABABABABACACAC000000000000AFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4000000000000B7B7B7B8B8B8B9B9B9B9B9B9000000000000BC
            BCBCBDBDBDBEBEBEBFBFBF000000000000C1C1C1C2C2C2000000000000000000
            000000C6C6C6C7C7C7C8C8C8C9C9C9000000000000CCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDE000000000000E1E1E1
            E1E1E1E2E2E2E3E3E3000000000000E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898980000000000009B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3000000000000A5A5A500000000
            0000A8A8A8000000000000ABABABABABABACACAC000000000000AFAFAFB0B0B0
            B1B1B1000000000000000000000000000000000000B7B7B7B8B8B8B9B9B9B9B9
            B9000000000000BCBCBCBDBDBDBEBEBEBFBFBF000000000000C1C1C100000000
            0000C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9000000000000CCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D40000000000
            00D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDE00
            0000000000000000000000000000000000000000E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            9292939393949494959595969696979797979797989898000000000000000000
            000000000000000000000000000000A0A0A0A1A1A1A2A2A2A3A3A30000000000
            00A5A5A5000000000000A8A8A8000000000000ABABABABABABACACAC00000000
            0000AFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9000000000000BCBCBCBDBDBDBEBEBEBFBFBF0000000000
            00C1C1C1000000000000C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C900
            0000000000000000000000000000000000000000000000D1D1D1D2D2D2D3D3D3
            D4D4D4000000000000000000000000000000000000000000000000DBDBDBDCDC
            DCDDDDDDDEDEDE000000000000E1E1E1E1E1E1E2E2E2E3E3E3000000000000E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989800
            00000000009B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3000000000000000000000000A7A7A7A8A8A8000000000000ABABABABAB
            ABACACAC000000000000AFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9000000000000BCBCBCBDBDBDBEBEBE
            BFBFBF000000000000000000000000C3C3C3C4C4C4000000000000C6C6C6C7C7
            C7C8C8C8C9C9C9000000000000CCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDE000000000000E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4000000000000E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898980000000000009B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3000000000000000000000000A7A7A7A8A8A8000000
            000000ABABABABABABACACAC000000000000AFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4000000000000B7B7B7B8B8B8B9B9B9B9B9B9000000000000BC
            BCBCBDBDBDBEBEBEBFBFBF000000000000000000000000C3C3C3C4C4C4000000
            000000C6C6C6C7C7C7C8C8C8C9C9C9000000000000CCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDE000000000000E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4000000000000E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898980000000000009B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3000000000000000000A6A6A6A7
            A7A7A8A8A8000000000000ABABABABABABACACACADADAD000000000000B0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3000000000000B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9000000000000BCBCBCBDBDBDBEBEBEBFBFBF000000000000000000C2C2C2C3
            C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9000000000000CCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D40000000000
            00D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDE00
            0000000000E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4000000000000E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898980000000000009B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A30000000000
            00000000A6A6A6A7A7A7A8A8A8000000000000ABABABABABABACACACADADAD00
            0000000000000000B1B1B1B2B2B2B2B2B2000000000000000000B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9000000000000BCBCBCBDBDBDBEBEBEBFBFBF0000000000
            00000000C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C900
            0000000000CCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDE000000000000E1E1E1E1E1E1E2E2E2E3E3E3000000000000E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989800
            0000000000000000000000000000000000000000000000000000A1A1A1A2A2A2
            A3A3A3000000000000A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABAB
            ABACACACADADADAEAEAEAFAFAF000000000000000000000000000000B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9000000000000BCBCBCBDBDBDBEBEBE
            BFBFBF000000000000C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
            C7C8C8C8C9C9C900000000000000000000000000000000000000000000000000
            0000D2D2D2D3D3D3D4D4D4000000000000000000000000000000000000000000
            000000000000DCDCDCDDDDDDDEDEDE0000000000000000000000000000000000
            00000000E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFFFFFFFF818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFE8080
            808080808F8F8F0000008F8F8F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFF8F8F8F8F8F8F000000FFFFFF8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF000000}
        end
        object btOper: TBitBtn
          Left = 5
          Top = 31
          Width = 150
          Height = 41
          TabOrder = 2
          OnClick = btOperClick
          Glyph.Data = {
            76470000424D7647000000000000360000002800000097000000280000000100
            1800000000004047000012170000121700000000000000000000FFFFFF8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF00
            00008F8F8F8F8F8F808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            808F8F8F8F8F8F0000008F8F8FFFFFFFFFFFFF81818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFE8080808080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898980000000000000000000000000000009E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABAB0000000000000000000000000000000000000000000000
            00000000B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBD000000000000C0C0C0000000000000C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8000000000000CBCBCBCCCCCCCDCDCDCDCDCD0000
            00000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D600000000
            0000000000000000000000DBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0000000
            000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7000000000000E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797970000000000000000009A9A9A9B9B9B9C9C9C0000000000
            000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9
            B9BABABABBBBBBBCBCBC000000000000BFBFBFC0C0C0000000000000C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8000000000000CBCBCBCCCCCC
            CDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D40000
            00000000000000D8D8D8D9D9D9DADADA000000000000000000DDDDDDDEDEDEDF
            DFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6000000000000
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797970000000000009999999A9A9A9B9B9B
            9C9C9C9D9D9D0000000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABAB000000000000AE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000
            000000B9B9B9B9B9B9BABABABBBBBBBCBCBC000000000000BFBFBFC0C0C0C0C0
            C0000000000000C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7000000000000CA
            CACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4000000000000D7D7D7D8D8D8D9D9D9DADADADADADA0000000000
            00DDDDDDDEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6000000000000E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969600000000000098989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0A0A0A1A1A1A2A2A2
            000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            AB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBCBCBC000000000000
            BFBFBFC0C0C0C0C0C00000000000000000000000000000000000000000000000
            00000000000000CACACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6000000000000E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696960000
            000000009898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0
            A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABAB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBB00
            0000000000BEBEBEBFBFBFC0C0C0C0C0C0C1C1C1000000000000C4C4C4C5C5C5
            C6C6C6C6C6C6000000000000C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCD0000
            00000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000
            000000E2E2E2E3E3E3E4E4E4E5E5E5000000000000E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696960000000000009898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E000000000000A0A0A0A1A1A1A2A2A200000000000000000000000000000000
            0000000000A9A9A9AAAAAAABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B60000000000000000000000
            00000000000000000000BDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C100000000
            0000C4C4C4C5C5C5C6C6C6C6C6C6000000000000C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D40000000000
            00D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDF
            DFDFE0E0E0000000000000000000000000000000000000000000E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696960000000000009898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E000000000000A0A0A0A1A1A1A2A2A2000000000000A5A5
            A5A5A5A5A6A6A6A7A7A7000000000000AAAAAAABABABABABAB00000000000000
            0000000000000000000000000000000000B3B3B3B4B4B4B5B5B5B6B6B6000000
            000000B9B9B9B9B9B9BABABABBBBBB000000000000BEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2000000000000C5C5C5C6C6C6000000000000C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDB0000
            00000000DEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E500
            0000000000E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969600000000000098989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0A0A0A1A1A1A2A2A2
            000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABAB
            AB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBCBCBC000000000000
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2000000000000C5C5C5C6C6C60000000000
            00C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6000000000000E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696960000
            000000009898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E000000000000A0
            A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000
            000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9B9BABABABBBBBBBC
            BCBC000000000000BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3000000000000
            000000000000C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCD0000
            00000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4000000000000D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0000000
            000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6000000000000E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797970000000000009999999A9A9A9B9B9B9C9C9C9D9D9D0000
            000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8000000000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000000000B9B9B9B9B9
            B9BABABABBBBBBBCBCBC000000000000BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3000000000000000000000000C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D40000
            00000000D7D7D7D8D8D8D9D9D9DADADADADADA000000000000DDDDDDDEDEDEDF
            DFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6000000000000
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797970000000000000000009A9A9A9B9B9B
            9C9C9C0000000000000000009F9F9FA0A0A0A1A1A1A2A2A2000000000000A5A5
            A5A5A5A5A6A6A6A7A7A7000000000000AAAAAAABABABABABAB000000000000AE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6000000
            000000B9B9B9B9B9B9BABABABBBBBB000000000000BEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCD000000000000D0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4000000000000000000D8D8D8D9D9D9DADADA0000000000000000
            00DDDDDDDEDEDEDFDFDFE0E0E0000000000000E2E2E2E3E3E3E4E4E4E5E5E500
            0000000000E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989800
            00000000000000000000000000009E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            000000000000000000000000000000000000000000A9A9A9AAAAAAABABABABAB
            AB000000000000000000000000000000000000000000000000000000B4B4B4B5
            B5B5B6B6B6000000000000000000000000000000000000000000BDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
            C7C8C8C8C9C9C9CACACA00000000000000000000000000000000000000000000
            0000000000000000D4D4D4D4D4D4D5D5D5D6D6D6000000000000000000000000
            000000DBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E00000000000000000000000
            00000000000000000000E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFFFFFFFF818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFE8080
            808080808F8F8F0000008F8F8F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFF8F8F8F8F8F8F000000FFFFFF8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF000000}
        end
        object btMast: TBitBtn
          Tag = 2
          Left = 5
          Top = 114
          Width = 150
          Height = 41
          TabOrder = 3
          OnClick = btOperClick
          Glyph.Data = {
            76470000424D7647000000000000360000002800000097000000280000000100
            1800000000004047000012170000121700000000000000000000FFFFFF8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF00
            00008F8F8F8F8F8F808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            808F8F8F8F8F8F0000008F8F8FFFFFFFFFFFFF81818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFE8080808080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1000000000000A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000
            000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4000000000000B7B7B7B8B8B8B9B9B900000000000000000000
            0000000000000000BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000
            000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB0000000000000000000000
            00000000000000000000000000000000D4D4D4D4D4D4D5D5D5000000000000D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDC000000000000DFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8000000000000ABABABABABAB000000000000AEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4000000000000B7B7B7B8B8B80000000000
            00BABABABBBBBBBCBCBCBDBDBD000000000000C0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000
            000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5000000000000D8D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A4A5A5
            A5000000000000A7A7A7A8A8A8000000000000ABABABABABABACACAC00000000
            0000AFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3000000000000B6B6B6B7B7B7
            000000000000B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE000000000000C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADADADADADBDBDB0000
            00000000DEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000
            000000A4A4A4A5A5A5000000000000A7A7A7A8A8A8000000000000ABABABABAB
            ABACACAC00000000000000000000000000000000000000000000000000000000
            0000B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            000000000000C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADA
            DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1000000000000A4A4A4000000000000000000000000A8A8A8000000
            000000ABABABABABABACACACADADAD000000000000B0B0B0B1B1B1B2B2B2B2B2
            B2000000000000B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBE000000000000C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000
            000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8
            D8D8D9D9D9DADADADADADA000000000000DDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A400000000000000000000
            0000A8A8A8000000000000ABABABABABABACACACADADAD000000000000B0B0B0
            B1B1B1B2B2B2B2B2B2000000000000B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBD000000000000C0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000
            000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5000000000000000000000000000000000000000000DCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A10000000000000000000000
            00A5A5A5A6A6A6000000000000000000000000ABABABABABABACACACADADADAE
            AEAE000000000000B1B1B1B2B2B2000000000000B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9000000000000000000000000000000000000BFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCB000000000000000000000000000000000000000000000000D3D3D3
            D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADADADADA0000000000
            00DDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000
            000000000000000000A5A5A5A6A6A6000000000000000000000000ABABABABAB
            ABACACACADADADAEAEAE000000000000B1B1B1B2B2B2000000000000B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8000000000000BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADA
            DADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1000000000000000000A5A5A5A5A5A5A6A6A6A7A7A7000000000000
            000000ABABABABABABACACACADADADAEAEAEAFAFAF0000000000000000000000
            00B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7000000000000B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4000000
            000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000000000CDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5000000000000D8
            D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000000000A5A5A5A5A5A5A6A6A6A7
            A7A7000000000000000000ABABABABABABACACACADADADAEAEAEAFAFAF000000
            000000000000000000B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7000000000000B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBE000000000000C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCB000000
            000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5000000000000D8D8D8D9D9D9DADADADADADADBDBDB000000000000DEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000000000A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0000000000000B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8000000000000BABABABBBBBBBCBCBCBDBDBD000000000000C0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4000000000000C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCB000000000000CDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5000000000000D8D8D8D9D9D9DADADADADADA0000000000
            00DDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1000000
            000000A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8000000000000ABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0000000000000B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9000000000000000000000000000000000000
            BFBFBFC0C0C0C0C0C00000000000000000000000000000000000000000000000
            00000000000000CACACACBCBCB00000000000000000000000000000000000000
            0000000000000000D4D4D4D4D4D4D5D5D5000000000000000000000000000000
            000000000000DCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFF808080818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFF
            FF8080808F8F8F0000008F8F8FFFFFFF80808081818182828283838383838384
            84848585858686868787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D
            8E8E8E8F8F8F9090909191919191919292929393939494949595959696969797
            979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0
            A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9
            AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2
            B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBC
            BCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5
            C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECE
            CECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8
            D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1
            E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEA
            EAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4
            F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFC
            FDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF8080808181818282
            828383838383838484848585858686868787878888888989898A8A8A8A8A8A8B
            8B8B8C8C8C8D8D8D8E8E8E8F8F8F909090919191919191929292939393949494
            9595959696969797979797979898989999999A9A9A9B9B9B9C9C9C9D9D9D9E9E
            9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7
            A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAEAEAEAFAFAFB0B0B0
            B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9
            B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3
            C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CACACACBCBCBCCCCCC
            CDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5
            D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDCDCDDDDDDDEDEDEDF
            DFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8
            E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1
            F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFB
            FBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F0000008F8F8FFFFFFF
            8080808181818282828383838383838484848585858686868787878888888989
            898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F90909091919191919192
            92929393939494949595959696969797979797979898989999999A9A9A9B9B9B
            9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2A3A3A3A4A4A4A5A5
            A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABABABACACACADADADAE
            AEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7
            B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBEBFBFBFC0C0C0C0C0
            C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7C7C8C8C8C9C9C9CA
            CACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1D1D1D2D2D2D3D3D3
            D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADADADADADBDBDBDCDC
            DCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3E3E4E4E4E5E5E5E6
            E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEEEEEE
            EFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5F6F6F6F7F7F7F8F8
            F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFEFFFFFF8080808F8F8F00
            00008F8F8FFFFFFFFFFFFF818181828282838383838383848484858585868686
            8787878888888989898A8A8A8A8A8A8B8B8B8C8C8C8D8D8D8E8E8E8F8F8F9090
            9091919191919192929293939394949495959596969697979797979798989899
            99999A9A9A9B9B9B9C9C9C9D9D9D9E9E9E9E9E9E9F9F9FA0A0A0A1A1A1A2A2A2
            A3A3A3A4A4A4A5A5A5A5A5A5A6A6A6A7A7A7A8A8A8A9A9A9AAAAAAABABABABAB
            ABACACACADADADAEAEAEAFAFAFB0B0B0B1B1B1B2B2B2B2B2B2B3B3B3B4B4B4B5
            B5B5B6B6B6B7B7B7B8B8B8B9B9B9B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBEBE
            BFBFBFC0C0C0C0C0C0C1C1C1C2C2C2C3C3C3C4C4C4C5C5C5C6C6C6C6C6C6C7C7
            C7C8C8C8C9C9C9CACACACBCBCBCCCCCCCDCDCDCDCDCDCECECECFCFCFD0D0D0D1
            D1D1D2D2D2D3D3D3D4D4D4D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9D9D9DADADA
            DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0E1E1E1E1E1E1E2E2E2E3E3
            E3E4E4E4E5E5E5E6E6E6E7E7E7E8E8E8E8E8E8E9E9E9EAEAEAEBEBEBECECECED
            EDEDEEEEEEEEEEEEEFEFEFF0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F5F5F5
            F6F6F6F7F7F7F8F8F8F9F9F9FAFAFAFBFBFBFCFCFCFCFCFCFDFDFDFEFEFE8080
            808080808F8F8F0000008F8F8F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFF8F8F8F8F8F8F000000FFFFFF8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F
            8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FFFFFFF000000}
        end
      end
    end
    object Panel8: TPanel
      Left = 0
      Top = 511
      Width = 199
      Height = 229
      BevelOuter = bvNone
      TabOrder = 2
      object Label1: TLabel
        Left = 16
        Top = 28
        Width = 62
        Height = 16
        Caption = 'RUN TIME'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 16
        Top = 53
        Width = 64
        Height = 16
        Caption = 'IDLE TIME'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label4: TLabel
        Left = 16
        Top = 78
        Width = 62
        Height = 16
        Caption = 'JAM TIME'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label5: TLabel
        Left = 16
        Top = 103
        Width = 77
        Height = 16
        Caption = 'TOTAL TIME'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label6: TLabel
        Left = 16
        Top = 128
        Width = 70
        Height = 16
        Caption = 'CHIP UPEH'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = Label6DblClick
      end
      object Label31: TLabel
        Left = 16
        Top = 153
        Width = 76
        Height = 16
        Caption = 'WORK CHIP'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label32: TLabel
        Left = 16
        Top = 205
        Width = 75
        Height = 16
        Caption = 'LOT COUNT'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label16: TLabel
        Left = 16
        Top = 181
        Width = 63
        Height = 16
        Caption = 'FAIL CHIP'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Panel9: TPanel
        Left = 0
        Top = 0
        Width = 199
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'DAY INFORMATION'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object pnDayRunTime: TPanel
        Left = 113
        Top = 21
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '00::00::00'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnDblClick = pnDayRunTimeDblClick
      end
      object pnDayIdleTime: TPanel
        Left = 113
        Top = 47
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '00::00::00'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnDblClick = pnDayRunTimeDblClick
      end
      object pnDayJamTime: TPanel
        Left = 113
        Top = 73
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '00::00::00'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnDblClick = pnDayRunTimeDblClick
      end
      object pnDayTotalTime: TPanel
        Left = 113
        Top = 98
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '00::00::00'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnDblClick = pnDayRunTimeDblClick
      end
      object pnDayChipUpeh: TPanel
        Left = 113
        Top = 124
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnDblClick = Label6DblClick
      end
      object pnDayWorkChip: TPanel
        Left = 113
        Top = 149
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        OnDblClick = pnDayRunTimeDblClick
      end
      object pnDayLotCount: TPanel
        Left = 113
        Top = 201
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        OnDblClick = pnDayRunTimeDblClick
      end
      object pnDayFailChip: TPanel
        Left = 113
        Top = 176
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBtnText
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        OnDblClick = pnDayRunTimeDblClick
      end
    end
    object Panel10: TPanel
      Left = 200
      Top = 511
      Width = 247
      Height = 229
      BevelOuter = bvNone
      TabOrder = 3
      object Label2: TLabel
        Left = 7
        Top = 27
        Width = 49
        Height = 16
        Caption = 'LOT NO'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label7: TLabel
        Left = 7
        Top = 77
        Width = 76
        Height = 16
        Caption = 'WORK CHIP'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label8: TLabel
        Left = 7
        Top = 152
        Width = 70
        Height = 16
        Caption = 'CHIP UPEH'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = Label8DblClick
      end
      object Label9: TLabel
        Left = 7
        Top = 177
        Width = 77
        Height = 16
        Caption = 'START TIME'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label10: TLabel
        Left = 7
        Top = 203
        Width = 62
        Height = 16
        Caption = 'RUN TIME'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label33: TLabel
        Left = 7
        Top = 102
        Width = 63
        Height = 16
        Caption = 'FAIL CHIP'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label34: TLabel
        Left = 7
        Top = 52
        Width = 58
        Height = 16
        Caption = 'JOB FILE'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label100: TLabel
        Left = 7
        Top = 127
        Width = 74
        Height = 16
        Caption = 'GOOD CHIP'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Panel11: TPanel
        Left = 0
        Top = 0
        Width = 247
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'LOT INFOMATION'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object pnLotNo: TPanel
        Left = 105
        Top = 21
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = 'NONE'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object pnLotWorkChip: TPanel
        Left = 105
        Top = 71
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object pnLotChipUpeh: TPanel
        Left = 105
        Top = 147
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnDblClick = Label8DblClick
      end
      object pnLotStartTime: TPanel
        Left = 105
        Top = 172
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = '00::00::00'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object pnLotRunTime: TPanel
        Left = 105
        Top = 198
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = '00::00::00'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object pnLotFailChip: TPanel
        Left = 105
        Top = 96
        Width = 74
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object pnLotJobFile: TPanel
        Left = 105
        Top = 46
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = 'NONE'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object pnLotGoodChip: TPanel
        Left = 105
        Top = 122
        Width = 140
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object pnLotTrimChip: TPanel
        Left = 177
        Top = 96
        Width = 68
        Height = 28
        BevelInner = bvLowered
        Caption = '0'
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
      end
    end
    object Panel12: TPanel
      Left = 448
      Top = 511
      Width = 204
      Height = 229
      BevelOuter = bvNone
      TabOrder = 4
      object Panel13: TPanel
        Left = 0
        Top = 0
        Width = 204
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'OPTION'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = Panel13Click
      end
      object Panel32: TPanel
        Left = 0
        Top = 21
        Width = 204
        Height = 27
        Color = clSkyBlue
        TabOrder = 1
        object Label14: TLabel
          Left = 6
          Top = 4
          Width = 115
          Height = 15
          Caption = 'IGNORE DOOR OPEN'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnIgnrDoor: TPanel
          Tag = 1
          Left = 162
          Top = 1
          Width = 41
          Height = 25
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
      end
      object Panel34: TPanel
        Left = 0
        Top = 98
        Width = 204
        Height = 26
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #44404#47548
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        object Label15: TLabel
          Left = 6
          Top = 4
          Width = 99
          Height = 15
          Caption = 'INSPECTION SKIP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnInspSkip: TPanel
          Tag = 3
          Left = 162
          Top = 1
          Width = 41
          Height = 24
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
      end
      object Panel22: TPanel
        Left = 0
        Top = 177
        Width = 204
        Height = 26
        Color = clSkyBlue
        TabOrder = 3
        object Label27: TLabel
          Left = 6
          Top = 4
          Width = 119
          Height = 15
          Caption = 'IGNORE HEAT ALRAM'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnIgnrHeatAlm: TPanel
          Tag = 11
          Left = 162
          Top = 1
          Width = 41
          Height = 24
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
      end
      object Panel25: TPanel
        Left = 0
        Top = 72
        Width = 204
        Height = 26
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        object Label36: TLabel
          Left = 6
          Top = 4
          Width = 130
          Height = 15
          Caption = 'IGNORE DOUBLE STRIP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnIgnrStrAlm: TPanel
          Tag = 5
          Left = 162
          Top = 1
          Width = 41
          Height = 24
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
      end
      object Panel23: TPanel
        Left = 0
        Top = 46
        Width = 204
        Height = 26
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        object Label28: TLabel
          Left = 6
          Top = 4
          Width = 86
          Height = 15
          Caption = 'LOADING STOP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnLoadingStop: TPanel
          Tag = 6
          Left = 162
          Top = 1
          Width = 41
          Height = 24
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
      end
      object Panel21: TPanel
        Left = 0
        Top = 124
        Width = 204
        Height = 26
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        object Label11: TLabel
          Left = 6
          Top = 4
          Width = 91
          Height = 15
          Caption = 'CHECK FAIL CNT'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnCheckFailCnt: TPanel
          Tag = 7
          Left = 162
          Top = 1
          Width = 41
          Height = 24
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
        object edLimAllFailCnt: TEdit
          Left = 122
          Top = 3
          Width = 38
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = #44404#47548
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnChange = edLimAllFailCntChange
        end
      end
      object Panel17: TPanel
        Left = 0
        Top = 150
        Width = 204
        Height = 27
        Color = clSkyBlue
        TabOrder = 7
        object Label12: TLabel
          Left = 6
          Top = 4
          Width = 74
          Height = 15
          Caption = 'IGNORE TRIM'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pnIgnoreTrim: TPanel
          Tag = 10
          Left = 162
          Top = 1
          Width = 41
          Height = 25
          Align = alRight
          BevelInner = bvLowered
          Caption = 'ON'
          Color = clLime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnDblClick = pnIgnoreTrimDblClick
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 203
        Width = 204
        Height = 26
        Color = clSkyBlue
        TabOrder = 8
        object lbBinCnt: TLabel
          Left = 6
          Top = 4
          Width = 74
          Height = 15
          Caption = 'BIN NUMBER '
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object edBinCnt: TEdit
          Left = 162
          Top = 3
          Width = 38
          Height = 19
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = #44404#47548
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          ParentFont = False
          TabOrder = 0
          Text = '0'
          OnChange = edLimAllFailCntChange
        end
      end
    end
    object Panel14: TPanel
      Left = 0
      Top = 741
      Width = 447
      Height = 121
      BevelOuter = bvNone
      TabOrder = 5
      object Panel15: TPanel
        Left = 0
        Top = 0
        Width = 447
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'ERROR'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object lbErr: TListBox
        Left = 0
        Top = 20
        Width = 447
        Height = 101
        Align = alClient
        BevelInner = bvNone
        BorderStyle = bsNone
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
        ItemHeight = 15
        ParentFont = False
        TabOrder = 1
      end
    end
    object Panel16: TPanel
      Left = 448
      Top = 741
      Width = 204
      Height = 121
      BevelOuter = bvNone
      TabOrder = 6
      object Panel18: TPanel
        Left = 0
        Top = 0
        Width = 204
        Height = 20
        Align = alTop
        BevelOuter = bvNone
        Caption = 'LOT WORK'
        Color = clSkyBlue
        Font.Charset = HANGEUL_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object btLotOpen: TBitBtn
        Left = 1
        Top = 21
        Width = 203
        Height = 50
        Caption = 'LOT OPEN'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -28
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = btLotOpenClick
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFEFEFEF7F7F7EAEAEAEAEAEAF8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFE
          FEF7F7F7B6C4CBA3AEB4CCCCCCE5E5E5F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3
          F3C3C4C53095BB3BA4CEB2B2B3CACACAE6E6E6F9F9F9FFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3C9CA
          CB2F93BC18B5E34ED8F12B9DCCB0B0B0C9C9C9E5E5E5F9F9F9FEFEFEFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3C3C4C52F93
          BC00D1F918B5E356DAF206DEFC249ACBAEAEAEC9C9C9E4E4E4F8F8F8FEFEFEFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3C9CACB2F93BB00D5
          FA00D1F918B5E355DAF101DFFC00DCFB249BCCA7A8A9C8C8C8E3E3E3F7F7F7FE
          FEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3C3C4C52F94BB00D9FA00D5
          FA00D1F919B5E356DAF200DEFB00DCFB00D9FA229ECEA2A4A5C7C7C7E2E2E2F6
          F6F6FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3C9CACB3093BB00DCFB00D9FA00D5
          FA00D1F919B5E35EDCF200DFFC00DDFB00DAFB00D8FA219FCF999FA2C6C6C6E0
          E0E0F5F5F5FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFEFEFEF3F3F3C3C4C53F9CC000E0FB00DCFB00D9FA00D5
          FA00D1F919B5E36BDFF301E0FD00DDFB00DAFB00D8FA00D5FA1FA2D38F999EC5
          C5C5DEDEDEF4F4F4FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFEFEFEEFEFF0C3C4C55AAACA50EDFD00E0FB00DCFB00D9FA00D5
          FA00D1F91AB6E36CDFF307E0FD00DDFB00DAFB00D8FA00D5FA00D2FA1DA5D588
          979DC4C4C4DDDDDDF3F3F3FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFEFEFEF3F3F3C3C4C55DAAC970F3FE5FEEFE00E0FB00DCFB00D9FA00D5
          FA00D1F91BB6E374E1F413E2FD00DDFB00DAFB00D8FA00D5FA00D2FA00D0F91B
          A7D881959FC3C3C3DCDCDCF2F2F2FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FEFEFEEFEFF0C3C4C560ACCA76F5FE70F3FE69F0FE06E1FC00DCFB00D9FA00D5
          FA00D1F91BB6E37BE2F52DE5FD00DDFB00DAFB00D8FA00D5FA00D2FA00D0F900
          CDF918A9DB7A95A2C2C2C2DBDBDBF2F2F2FDFDFDFFFFFFFFFFFFFFFFFFFFFFFF
          F5F5F5C4C5C561ACC97DF8FE76F5FE70F3FE69F0FE21E4FC00DCFB00D9FA00D5
          FA00D1F91BB6E37BE2F549E9FD00DDFB00DAFB00D8FA00D5FA00D2FA00D0F900
          CDF900CAF916ACDE7798A6C2C2C2DBDBDBF3F3F3FDFDFDFFFFFFFFFFFFFFFFFF
          D4D4D566B0CC8AFAFF83F8FF7DF6FE76F3FE70F0FE43E9FD00DCFB00D9FA00D5
          FA00D1F91CB6E482E4F566ECFE00DDFB00DAFB00D8FA00D5FA00D2FA00D0F900
          CDF900CAF900C8F812ADE07198AAC3C3C3DEDEDEF7F7F7FFFFFFFFFFFFFFFFFF
          61A8C590FAFF8AFAFF83F8FF7DF6FE76F3FE70F0FE61ECFD03DDFC00D9FA00D5
          FA00D1F91DB6E48AE6F67CEFFE1EE1FC00DAFB00D8FA00D5FA00D2FA00D0F900
          CDF900CAF900C8F800C5F810AFE3839AA4CCCCCCEEEEEEFEFEFEFFFFFFFFFFFF
          6BB1CD90FAFF8AFAFF83F8FF7DF6FE76F3FE70F0FE69EDFD28E2FC00D9FA00D5
          FA00D1F91DB6E491E8F684F0FE51E8FD00DBFB00D8FA00D6FA00D3FA00D0F900
          CEF900CBF900C8F800C6F800C3F8219BCFBFC1C2EBEBEBFEFEFEFFFFFFFFFFFF
          6DB3CD97FBFF90FBFF8AF9FF83F6FF7DF4FE76F1FE70EEFD5AE9FD01DAFB00D5
          FA00D1F91DB6E491E8F68CF1FE80EFFE0DDDFC00D8FA00D6FA00D3FA00D0F900
          CEF900CBF900C8F800C6F800C3F81EA2D2C3C4C4ECECECFEFEFEFFFFFFFFFFFF
          6EB3CD97FBFF90FBFF8AF9FF83F6FF7DF4FE76F1FE70EEFD69EBFD27DFFB00D5
          FA00D1F91EB7E498E9F78CF1FE8CF0FE53E7FD00D8FA00D6FA00D3FA00D0F900
          CEF900CBF900C8F800C6F800C3F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          BEC9CE8ED6E690FBFF8AF9FF83F6FF7DF4FE76F1FE70EEFD69EBFD5DE7FC0AD7
          FB00D1F903CAF865BFE094EEFB8CF0FE8BEFFE1ADCFB00D6FA00D3FA00D0F900
          CEF900CBF900C8F800C6F800C3F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFBAC6CB8DD6E67DDAC96CC4B767C1B663C0B65EBEB55ABBB555B9B53EB3
          B40AA9B30AA6B30AADBE3FB7E08DE2F58CEFFE71EAFD04D7FB00D3FA00D1F900
          CEF900CBF900C9F800C6F800C4F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFBCC7CC458033358A3C348A3B348A3B338A3B33893B33893B3289
          3B30893B2B883F2B783100C9F81EB8E77DD1EB8CEEFD56E4FC00D3FA00D1F900
          CEF900C9F700C1EE00C6F800C4F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF5887442DC48725C28425C28425C28425C28425C28425C2
          842CA45F30997500CAF900C9F800C5F807BDF262BDE08DE9FA3ADDFC00D1F900
          CEF914A1931B927600C6F800C4F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF7AA16251CE9C26C38525C28425C28425C28425C284327F
          2749C3C655DEFB3BD7FB04CAF900C5F800C3F800C3F83FB5DF86E2F73CDCFB08
          B7CB2B8339169A8B00C6F800C4F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF7EA46769D5A94ECE9B25C28425C28425C28425C2842D9F
          563B782B57D4EF55DCFB49D9FA16CAF900C3F800C3F805BDF261BCDF87E2F435
          77262B8D420DAEBA00C7F800C4F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF83A76C77D9B177D9B349CC9825C28425C28425C28425C2
          8428BE883A834449B7BF52D4F44FD7FB36D0FA11C7F900BCF02A98A63D792726
          B977387A290BCAF500C7F800C4F8229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF87AA7185DDB985DDBA77D9B34FCE9C26C38525C28425C2
          8425C28427B57134853B3984483C91703B9989369575327E3A2F873925C2832B
          AD68559A6880E5FC33D3FA02C5F9229FCCC3C4C4ECECECFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF8BAD7693E1C193E1C285DDBA77D9B369D5AB37C78E25C2
          8425C28425C28425C28428B36F2AA55A2C9B4D2BA05427B87625C28425C08142
          84398ADDE68CE7FC8CE6FC73DFFC46AED4C2C3C3EBEBEBFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF8BAD76A0E4C99BDDC3688B4E84C39985DDBA77D9B361D3
          A63AC89026C38525C28425C28425C28425C28425C28425C28425C1823C732162
          9BA09CE7F993E9FD8CE6FC8CE5FC81C8E2C2C3C3EBEBEBFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF8FB07B9CD2B1798F71ECECEC94A58F8EB98C85DAB585DD
          BA77D9B366D5A958D1A144CB953FCA9337C78E32C68C37BD7E44752BE4E4E4FA
          FAFA75ABC19BE3F693E8FD8CE5FC86CAE3C2C3C3EBEBEBFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF92B07D809678EDEDEDFCFCFCFFFFFFD8DAD8779A678FC0
          9586D3AC77D8B177D9B369D5AB6AD1A45FB980589049708E5CF1F1F1FBFBFBFF
          FFFFFFFFFF7CABBF9ADFF493E6FD86CAE3C4C5C5EBEBEBFEFEFEFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF8BA46BF7F7F7FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFF3F5
          EFAAC49D85A76D82A66A7BA1628BAC78CAD7C1F6F6F6FCFCFCFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFF83ABBE9ADCF18CCEE5D4D5D5F1F1F1FFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFF8FAFBE8AC8DFEDEEEEFAFAFAFFFFFF}
      end
      object btLotEnd: TBitBtn
        Left = 1
        Top = 72
        Width = 203
        Height = 49
        Caption = 'LOT END'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -28
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = btLotEndClick
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9E6E6E6CACA
          CAACACAC9191917B7B7B777777878787A1A1A1BFBFBFDCDCDCF5F5F5FFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4F4F4D2D2D27A7A9039396A1E1E
          650E0E6A0505710101750404700C0C6619195F2A2A5B43435C7D7D7DBCBCBCEB
          EBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFCFCFCD8D8DB68688A1F1F640F137F212B9D3540
          BA4756CE5364DD5A6AE45262DD4453CE333EB820289D0F137E15155A31315771
          7175C3C3C3F6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFF7F7F7A3A3B62828691218862732B03447D72E43DD263E
          DD1C3ADD0F39DE0037DE0E38DD1B39DC223BDB2D42DC3446D72732B012188617
          17583D3D589E9E9EE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFF5F5F58383A3161974222CA62E41D61D33D90823D50020D70028
          DA002FDB0035DE0037DE0033DD002CDA0024D8001FD70823D51C32D92E41D622
          2CA611146F2D2D55898989E4E4E4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFF8F8F87F7FA31317802B38B82439DA0B22D60019D4001CD50020D70025
          D8002BDB0031DC0033DD0030DB0029DA0022D7001FD7001CD40019D40B22D624
          39DA2B38B812157E282854898989E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FDFDFD9A9AB71418802B39BF1E33D90019D40019D4021AD40821D60A24D60323
          D80027D9002BDB002BDB002ADA0025D80020D70521D60D25D60A22D6031CD500
          19D41D33D92C3ABF12157E2D2D559E9E9EF6F6F6FFFFFFFFFFFFFFFFFFFFFFFF
          CACAD91B1E792B38B81E33D90019D40019D40019D40119D41128D7142BD80720
          D50523D80225D80125D90123D80121D80520D60B23D61E35D90F26D7031BD402
          1BD40019D41E33D92B38B811146F3D3D58C3C3C3FFFFFFFFFFFFFFFFFFF8F8FA
          4D4D89222CA62439DA0019D40019D40019D40014D40D1DD4B1BAF2ABB3F00D22
          D60015D4021CD5011DD5011DD5011BD40013D33649DEDCE0F98591EA020FD200
          14D30019D40019D42439DA222CA6171758717175EBEBEBFFFFFFFFFFFFBABAD0
          1218862E41D60B22D60019D40019D40014D40918CAA0AAEFFFFFFFFFFFFFA2AC
          F01128D70012D30018D40017D40010D22E43DCCCD1F7FFFFFFFFFFFF7C8AEA00
          10D20016D30019D40B22D62E41D6121886313157BCBCBCFFFFFFFDFDFD51518E
          2732B01C32D90019D40019D40016D11420B9AAB0E0FFFFFFFFFFFFFFFFFFFFFF
          FFAAB3F0142BD70011D30010D22A40DBC8CDF6FFFFFFFFFFFFFFFFFFFFFFFF87
          95EE091ED50017D30019D41C32D92732B015155A7D7D7DF5F5F5E1E1EB0E127D
          3143D70820D50019D40019D50014CF1C25AFC4C6DEFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFADB5F1172ED8253BDAC7CCF5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAF
          B4E00D21D20016D40019D40820D53143D70D117C43435CDCDCDCB0B0CB1B2398
          253ADA0019D40019D40019D4001AD60010D20F15ADADB0D6FFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFC1C7F4CBD0F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA0A6DC08
          15C00015D20019D50019D40019D4253ADA1B23982A2A58BFBFBF7676A72833B2
          182ED80019D40019D40019D40019D4001AD50013D2111AB99BA2D7FBFCFEFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFF969DDC0B18C300
          14D30019D40019D40019D40019D4182ED82833B219195AA1A1A14040883241C6
          0D24D60019D40019D40019D40C24D62A3FDC3F52DF4658E03240D19FA5DAFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF969DDD2230D0394DDE36
          4ADD2339DA0B22D60019D40019D40D24D63241C60C0C5D87878715156F394AD5
          051DD50019D41028D72F43DC5868E3606FE45667E34C5EE13F51DF181EB7B4B7
          DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA3A9DF131EC63447DD4153DF4C
          5EE15667E35061E23549DD1028D7051DD5394AD50404607777770909683E4FDD
          0019D42439DA5062E26877E66877E66170E55365E2485AE13C4ED76972D7DBDF
          F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB9C0F2384BDD3B4EDE4658E050
          61E2596AE36271E56877E65062E22339DA3E4FDD0101637B7B7B2A2A7C394AD5
          2C40DC5F6FE46978E66877E66978E66474E56676E66372E58692EBE1E4FAFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFFFFFFFFB7BFF35A6AE35768E357
          68E35D6DE46474E56978E66978E65969E3394AD50505619191915D5D9A3241C6
          5061E26978E66978E66978E66978E66877E66372E58591EAE0E3F9FFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFFFFFFFFBCC2F46474E663
          73E56575E56777E66978E66978E65667E33241C60E0E5FACACAC9999BF2833B2
          4A5CE16978E66978E66978E66978E66677E98490EADFE1F9FFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFDBDBEBDCDDEDFFFFFFFFFFFFFFFFFFFEFEFEFFFFFFBBC2F464
          73E56675E56978E66978E66978E64A5CE12833B21E1E60CACACAD0D0E21B2398
          4153DF6978E66978E66978E76675E37882D9E6EAFEFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFD1D2E76C75CD6C75CED8D8EAFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFC8
          CDF66E7CE66776E66978E66978E64153DF1B2398393968E6E6E6F6F6FA2C2F8D
          3A4CD95D6DE46978E66978E76775E4717ACFCBCDE6FFFFFFFFFFFFFFFFFFFFFF
          FFCFD0E57079D16675E96473E66F78CFD4D5E8FFFFFFFFFFFFFEFEFEFFFFFFD5
          D9F8707EE76776E66A78E65D6DE43A4CD90D117C7A7A90F9F9F9FFFFFF9696BE
          2732B04658E06978E66978E66978E76473E5656FCEC6C7E4FFFFFFFFFFFFCCCD
          E37079D16474E66877E76878E76272E47079D0D4D5E8FFFFFFFFFFFFC4CAF569
          77E66675E56978E66978E64658E02732B01F1F64D2D2D2FFFFFFFFFFFFE6E6F0
          292E923A4CD95969E36978E66978E66A79E76373E5656FCECCCDE8D1D3E8717A
          D16473E56978E76877E66776E56878E76271E57079CFE2E2EBCCD1FA6776E664
          73E56978E66978E65969E33A4CD912188668688AF4F4F4FFFFFFFFFFFFFFFFFF
          A4A4C6222CA64153DF6978E66978E66978E66A79E76473E5727BD1747CD36575
          E66978E76978E66978E66978E66978E66979E76372E47983D8727FE06474E769
          78E66978E66978E64153DF222CA6282869D8D8DBFFFFFFFFFFFFFFFFFFFFFFFF
          F6F6FA6668A82B38B84557E06978E66978E66978E66979E76776E46775E46978
          E76978E66978E66978E66978E66978E66978E66978E66776E46776E46978E669
          78E66978E6485AE02B38B8161974A3A3B6FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFE3E3EE474A9D313FC24557E06978E66978E66978E66978E66978E66978
          E66978E66978E66978E66978E66978E66978E66978E66978E66978E66978E669
          78E64658E03240C31317808383A3F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFDBDBE9474A9D2B38B84153DF5969E36978E66978E66978E66978
          E66978E66978E66978E66978E66978E66978E66978E66978E66978E65969E341
          53DF2B38B81418807F7FA3F5F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE3E3EE6668A8222CA63A4CD94658E05D6DE46978E66978
          E66978E66978E66978E66978E66978E66978E66978E65D6DE44658E03A4CD922
          2CA61B1E799A9AB7F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFF6F6FAA4A4C6292E922732B03A4CD94153DF4A5C
          E15667E36171E56877E66171E55667E34A5CE14153DF3A4CD92732B01218864D
          4D89CACAD9FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE6E6F09696BE2C2F8D1B23982833
          B23241C6394AD53E4FDD394AD53241C62833B21B23980E127D51518EBABAD0F8
          F8FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FAD0D0E29999
          BF5D5D9A2A2A7C09096815156F4040887676A7B0B0CBE1E1EBFDFDFDFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      end
    end
    object PageControl1: TPageControl
      Left = 0
      Top = 0
      Width = 1271
      Height = 510
      ActivePage = TabSheet5
      Align = alTop
      MultiLine = True
      TabHeight = 25
      TabIndex = 0
      TabOrder = 7
      TabPosition = tpRight
      TabWidth = 250
      object TabSheet5: TTabSheet
        Caption = 'TabSheet5'
        object pnDataMap: TPanel
          Left = 0
          Top = 0
          Width = 1238
          Height = 510
          Align = alTop
          BevelOuter = bvNone
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Panel3: TPanel
            Left = 0
            Top = 0
            Width = 1238
            Height = 20
            Align = alTop
            BevelOuter = bvNone
            Caption = 'DATA MAP'
            Color = clSkyBlue
            Font.Charset = HANGEUL_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            OnClick = Panel3Click
            object lbCnt: TLabel
              Left = 1129
              Top = 2
              Width = 42
              Height = 16
              Caption = 'lbCnt'
              Visible = False
            end
            object lbCntRe: TLabel
              Left = 1186
              Top = 2
              Width = 42
              Height = 16
              Caption = 'lbCnt'
              Visible = False
            end
            object lbLDRMgzCnt: TLabel
              Left = 125
              Top = 2
              Width = 42
              Height = 16
              Caption = 'lbCnt'
              Font.Charset = HANGEUL_CHARSET
              Font.Color = clBlack
              Font.Height = -16
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label55: TLabel
              Left = 4
              Top = 2
              Width = 118
              Height = 16
              Caption = 'LDR Mgz Cnt :'
              Font.Charset = HANGEUL_CHARSET
              Font.Color = clBlack
              Font.Height = -16
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
            end
          end
          object GroupBox1: TGroupBox
            Left = 1
            Top = 20
            Width = 125
            Height = 490
            Caption = 'LOADER'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            object pnLoader: TPanel
              Left = 2
              Top = 21
              Width = 121
              Height = 467
              Align = alClient
              TabOrder = 0
            end
          end
          object GroupBox2: TGroupBox
            Left = 953
            Top = 20
            Width = 125
            Height = 490
            Caption = 'UNLOADER'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            object pnUnLoader: TPanel
              Left = 2
              Top = 21
              Width = 121
              Height = 467
              Align = alClient
              TabOrder = 0
            end
          end
          object GroupBox3: TGroupBox
            Left = 127
            Top = 20
            Width = 412
            Height = 231
            Caption = 'PRE BUFFER 1'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            object Label37: TLabel
              Left = 4
              Top = 211
              Width = 31
              Height = 11
              Caption = 'LotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
            end
            object Label38: TLabel
              Left = 223
              Top = 211
              Width = 36
              Height = 11
              Caption = 'MgzNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
            end
            object Label39: TLabel
              Left = 288
              Top = 211
              Width = 36
              Height = 11
              Caption = 'SlotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
            end
            object pcPbz: TPageControl
              Left = 2
              Top = 21
              Width = 407
              Height = 185
              ActivePage = TabSheet7
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabIndex = 0
              TabOrder = 0
              object TabSheet7: TTabSheet
                Caption = 'PreBuff Zone'
                object pnPreBuff1: TPanel
                  Left = 0
                  Top = 0
                  Width = 399
                  Height = 156
                  Align = alClient
                  TabOrder = 0
                end
              end
            end
            object edPR1LotNo: TEdit
              Left = 37
              Top = 209
              Width = 183
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 1
            end
            object edPR1MgzNo: TEdit
              Left = 260
              Top = 209
              Width = 25
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 2
              Text = '1'
            end
            object edPR1SlotNo: TEdit
              Left = 323
              Top = 209
              Width = 23
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 3
            end
            object btPR1LotInput: TButton
              Tag = 1
              Left = 349
              Top = 208
              Width = 60
              Height = 20
              Caption = 'INPUT'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
              TabOrder = 4
              OnClick = btPR1LotInputClick
            end
            object btPR1Input: TButton
              Tag = 1
              Left = 270
              Top = 11
              Width = 68
              Height = 25
              Caption = 'Input Strip'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -12
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              OnClick = btPR1InputClick
            end
            object btPR1Clear: TButton
              Tag = 1
              Left = 341
              Top = 11
              Width = 68
              Height = 25
              Caption = 'Clear Strip'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -12
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 6
              OnClick = btPR1ClearClick
            end
            object Button1: TButton
              Left = 88
              Top = 16
              Width = 75
              Height = 25
              Caption = 'Button1'
              TabOrder = 7
              Visible = False
              OnClick = Button1Click
            end
            object edEdit1: TEdit
              Left = 192
              Top = 16
              Width = 73
              Height = 27
              ImeName = 'Microsoft IME 2010'
              TabOrder = 8
              Text = 'edEdit1'
              Visible = False
            end
          end
          object GroupBox5: TGroupBox
            Left = 127
            Top = 255
            Width = 412
            Height = 231
            Caption = 'INSPECTION'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            object Label43: TLabel
              Left = 4
              Top = 211
              Width = 28
              Height = 14
              Caption = 'LotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object Label44: TLabel
              Left = 223
              Top = 211
              Width = 33
              Height = 14
              Caption = 'MgzNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object Label45: TLabel
              Left = 288
              Top = 211
              Width = 31
              Height = 14
              Caption = 'SlotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object pcIns: TPageControl
              Left = 2
              Top = 21
              Width = 407
              Height = 185
              ActivePage = TabSheet8
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabIndex = 0
              TabOrder = 0
              object TabSheet8: TTabSheet
                Caption = 'Work Zone'
                object pnWorkZone: TPanel
                  Left = 0
                  Top = 0
                  Width = 399
                  Height = 156
                  Align = alClient
                  TabOrder = 0
                end
              end
            end
            object btInspClear: TButton
              Tag = 3
              Left = 341
              Top = 10
              Width = 68
              Height = 25
              Caption = 'Clear Strip'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -12
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnClick = btPR1ClearClick
            end
            object edInspLotNo: TEdit
              Left = 37
              Top = 209
              Width = 183
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 2
            end
            object btInspLotInput: TButton
              Tag = 3
              Left = 349
              Top = 208
              Width = 60
              Height = 20
              Caption = 'INPUT'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
            end
            object edInspSlotNo: TEdit
              Left = 322
              Top = 210
              Width = 23
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 4
            end
            object edInspMgzNo: TEdit
              Left = 260
              Top = 209
              Width = 25
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 5
              Text = '1'
            end
          end
          object GroupBox6: TGroupBox
            Left = 540
            Top = 20
            Width = 412
            Height = 231
            Caption = 'PRE BUFFER 2'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            object Label26: TLabel
              Left = 4
              Top = 211
              Width = 31
              Height = 11
              Caption = 'LotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
            end
            object Label29: TLabel
              Left = 223
              Top = 211
              Width = 36
              Height = 11
              Caption = 'MgzNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
            end
            object Label30: TLabel
              Left = 288
              Top = 211
              Width = 36
              Height = 11
              Caption = 'SlotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
            end
            object PageControl2: TPageControl
              Left = 2
              Top = 21
              Width = 407
              Height = 185
              ActivePage = TabSheet10
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabIndex = 0
              TabOrder = 0
              object TabSheet10: TTabSheet
                Caption = 'PreBuff Zone'
                object pnPreBuff2: TPanel
                  Left = 0
                  Top = 0
                  Width = 399
                  Height = 156
                  Align = alClient
                  TabOrder = 0
                end
              end
            end
            object edPR2LotNo: TEdit
              Left = 37
              Top = 209
              Width = 183
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 1
            end
            object edPR2MgzNo: TEdit
              Left = 260
              Top = 209
              Width = 25
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 2
              Text = '1'
            end
            object edPR2SlotNo: TEdit
              Left = 323
              Top = 209
              Width = 23
              Height = 17
              AutoSize = False
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 3
            end
            object btPR2LotInput: TButton
              Tag = 2
              Left = 349
              Top = 208
              Width = 60
              Height = 20
              Caption = 'INPUT'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = #44404#47548
              Font.Style = []
              ParentFont = False
              TabOrder = 4
            end
            object btPR2Input: TButton
              Tag = 2
              Left = 270
              Top = 11
              Width = 68
              Height = 25
              Caption = 'Input Strip'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -12
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              OnClick = btPR1InputClick
            end
            object btPR2Clear: TButton
              Tag = 2
              Left = 341
              Top = 11
              Width = 68
              Height = 25
              Caption = 'Clear Strip'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -12
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 6
              OnClick = btPR1ClearClick
            end
            object Button2: TButton
              Left = 136
              Top = 16
              Width = 75
              Height = 25
              Caption = 'Button2'
              TabOrder = 7
              Visible = False
              OnClick = Button2Click
            end
          end
          object GroupBox15: TGroupBox
            Left = 1079
            Top = 23
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            object pnCrtTMPR1_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnSetTMPR1_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox16: TGroupBox
            Left = 1079
            Top = 83
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  2'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            object pnSetTMPR2_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR2_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox17: TGroupBox
            Left = 1079
            Top = 143
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  3'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
            object pnSetTMPR3_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR3_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox18: TGroupBox
            Left = 1079
            Top = 203
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
            object pnSetTMPR4_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR4_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox19: TGroupBox
            Left = 1079
            Top = 263
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  5'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
            object pnSetTMPR5_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR5_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox20: TGroupBox
            Left = 1079
            Top = 323
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  6'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
            object pnSetTMPR6_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR6_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox21: TGroupBox
            Left = 1079
            Top = 383
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  7'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
            object pnSetTMPR7_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR7_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox22: TGroupBox
            Left = 1079
            Top = 442
            Width = 154
            Height = 60
            Caption = 'TEMPERATURE  8'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
            object pnSetTMPR8_NotVsn: TPanel
              Tag = 1
              Left = 77
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clSkyBlue
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnDblClick = pnCrtTMPR1DblClick
            end
            object pnCrtTMPR8_NotVsn: TPanel
              Tag = 1
              Left = 2
              Top = 15
              Width = 75
              Height = 43
              Align = alCustom
              Caption = '0'
              Color = clInactiveCaptionText
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -24
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnDblClick = pnCrtTMPR1DblClick
            end
          end
          object GroupBox4: TGroupBox
            Left = 540
            Top = 255
            Width = 412
            Height = 231
            Caption = 'POST BUFFER'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 14
            object Label40: TLabel
              Left = 4
              Top = 211
              Width = 28
              Height = 14
              Caption = 'LotNo'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object Label41: TLabel
              Left = 223
              Top = 211
              Width = 33
              Height = 14
              Caption = 'MgzNo'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object Label42: TLabel
              Left = 288
              Top = 211
              Width = 31
              Height = 14
              Caption = 'SlotNo'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
            end
            object edPSBLotNo: TEdit
              Left = 37
              Top = 209
              Width = 183
              Height = 17
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 0
            end
            object btPSBLotInput: TButton
              Tag = 4
              Left = 349
              Top = 208
              Width = 60
              Height = 20
              Caption = 'INPUT'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object edPSBSlotNo: TEdit
              Left = 323
              Top = 209
              Width = 23
              Height = 17
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 2
            end
            object edPSBMgzNo: TEdit
              Left = 260
              Top = 209
              Width = 23
              Height = 17
              AutoSize = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ImeName = 'Microsoft Office IME 2007'
              ParentFont = False
              TabOrder = 3
              Text = '1'
            end
            object pcPsb: TPageControl
              Left = 2
              Top = 21
              Width = 407
              Height = 185
              ActivePage = TabSheet9
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabIndex = 0
              TabOrder = 4
              object TabSheet9: TTabSheet
                Caption = 'PostBuff Zone'
                object pnPostBuff: TPanel
                  Left = 0
                  Top = 0
                  Width = 399
                  Height = 156
                  Align = alClient
                  TabOrder = 0
                end
              end
            end
            object btPSBClear: TButton
              Tag = 4
              Left = 341
              Top = 11
              Width = 68
              Height = 25
              Caption = 'Clear Strip'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -12
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              OnClick = btPR1ClearClick
            end
          end
        end
        object pnTmpr_NotVsn: TPanel
          Left = 867
          Top = 90
          Width = 209
          Height = 393
          TabOrder = 1
          Visible = False
          object Label18: TLabel
            Left = 5
            Top = 10
            Width = 43
            Height = 13
            Caption = 'Temp 1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label19: TLabel
            Left = 5
            Top = 46
            Width = 43
            Height = 13
            Caption = 'Temp 2'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label20: TLabel
            Left = 5
            Top = 82
            Width = 43
            Height = 13
            Caption = 'Temp 3'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label21: TLabel
            Left = 5
            Top = 118
            Width = 43
            Height = 13
            Caption = 'Temp 4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label22: TLabel
            Left = 5
            Top = 154
            Width = 43
            Height = 13
            Caption = 'Temp 5'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label23: TLabel
            Left = 5
            Top = 190
            Width = 43
            Height = 13
            Caption = 'Temp 6'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label24: TLabel
            Left = 5
            Top = 226
            Width = 43
            Height = 13
            Caption = 'Temp 7'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label25: TLabel
            Left = 5
            Top = 262
            Width = 43
            Height = 13
            Caption = 'Temp 8'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label54: TLabel
            Left = 5
            Top = 298
            Width = 50
            Height = 13
            Caption = 'Temp All'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edTemp1_NotVsn: TEdit
            Left = 56
            Top = 8
            Width = 65
            Height = 21
            Color = clWhite
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object btTemp1_NotVsn: TButton
            Tag = 1
            Left = 128
            Top = 8
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 1
            OnClick = btTemp1Click
          end
          object edTemp2_NotVsn: TEdit
            Left = 56
            Top = 44
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
            Text = '0'
          end
          object btTemp2_NotVsn: TButton
            Tag = 2
            Left = 128
            Top = 44
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 3
            OnClick = btTemp1Click
          end
          object edTemp3_NotVsn: TEdit
            Left = 56
            Top = 80
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 4
            Text = '0'
          end
          object btTemp3_NotVsn: TButton
            Tag = 3
            Left = 128
            Top = 80
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 5
            OnClick = btTemp1Click
          end
          object edTemp4_NotVsn: TEdit
            Left = 56
            Top = 116
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 6
            Text = '0'
          end
          object btTemp4_NotVsn: TButton
            Tag = 4
            Left = 128
            Top = 116
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 7
            OnClick = btTemp1Click
          end
          object edTemp5_NotVsn: TEdit
            Left = 56
            Top = 152
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 8
            Text = '0'
          end
          object btTemp5_NotVsn: TButton
            Tag = 5
            Left = 128
            Top = 152
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 9
            OnClick = btTemp1Click
          end
          object edTemp6_NotVsn: TEdit
            Left = 56
            Top = 188
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 10
            Text = '0'
          end
          object btTemp6_NotVsn: TButton
            Tag = 6
            Left = 128
            Top = 188
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 11
            OnClick = btTemp1Click
          end
          object edTemp7_NotVsn: TEdit
            Left = 56
            Top = 224
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 12
            Text = '0'
          end
          object btTemp7_NotVsn: TButton
            Tag = 7
            Left = 128
            Top = 224
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 13
            OnClick = btTemp1Click
          end
          object edTemp8_NotVsn: TEdit
            Left = 56
            Top = 260
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 14
            Text = '0'
          end
          object btTemp8_NotVsn: TButton
            Tag = 8
            Left = 128
            Top = 260
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 15
            OnClick = btTemp1Click
          end
          object edTempAll_NotVsn: TEdit
            Left = 56
            Top = 296
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 16
            Text = '0'
          end
          object btTempAll_NotVsn: TButton
            Tag = 9
            Left = 128
            Top = 296
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 17
            OnClick = btTemp1Click
          end
          object btOK_NotVsn: TButton
            Left = 56
            Top = 328
            Width = 145
            Height = 49
            Caption = 'OK'
            TabOrder = 18
            OnClick = btOKClick
          end
        end
      end
      object TabSheet6: TTabSheet
        Caption = 'TabSheet6'
        ImageIndex = 1
        object pnTemperature: TPanel
          Left = 1019
          Top = 10
          Width = 209
          Height = 393
          TabOrder = 0
          Visible = False
          object Label35: TLabel
            Left = 5
            Top = 10
            Width = 43
            Height = 13
            Caption = 'Temp 1'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label46: TLabel
            Left = 5
            Top = 46
            Width = 43
            Height = 13
            Caption = 'Temp 2'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label47: TLabel
            Left = 5
            Top = 82
            Width = 43
            Height = 13
            Caption = 'Temp 3'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label48: TLabel
            Left = 5
            Top = 118
            Width = 43
            Height = 13
            Caption = 'Temp 4'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label49: TLabel
            Left = 5
            Top = 154
            Width = 43
            Height = 13
            Caption = 'Temp 5'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label50: TLabel
            Left = 5
            Top = 190
            Width = 43
            Height = 13
            Caption = 'Temp 6'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label51: TLabel
            Left = 5
            Top = 226
            Width = 43
            Height = 13
            Caption = 'Temp 7'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label52: TLabel
            Left = 5
            Top = 262
            Width = 43
            Height = 13
            Caption = 'Temp 8'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label53: TLabel
            Left = 5
            Top = 298
            Width = 50
            Height = 13
            Caption = 'Temp All'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -12
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edTemp1: TEdit
            Left = 56
            Top = 8
            Width = 65
            Height = 21
            Color = clWhite
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object btTemp1: TButton
            Tag = 1
            Left = 128
            Top = 8
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 1
            OnClick = btTemp1Click
          end
          object edTemp2: TEdit
            Left = 56
            Top = 44
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
            Text = '0'
          end
          object btTemp2: TButton
            Tag = 2
            Left = 128
            Top = 44
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 3
            OnClick = btTemp1Click
          end
          object edTemp3: TEdit
            Left = 56
            Top = 80
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 4
            Text = '0'
          end
          object btTemp3: TButton
            Tag = 3
            Left = 128
            Top = 80
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 5
            OnClick = btTemp1Click
          end
          object edTemp4: TEdit
            Left = 56
            Top = 116
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 6
            Text = '0'
          end
          object btTemp4: TButton
            Tag = 4
            Left = 128
            Top = 116
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 7
            OnClick = btTemp1Click
          end
          object edTemp5: TEdit
            Left = 56
            Top = 152
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 8
            Text = '0'
          end
          object btTemp5: TButton
            Tag = 5
            Left = 128
            Top = 152
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 9
            OnClick = btTemp1Click
          end
          object edTemp6: TEdit
            Left = 56
            Top = 188
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 10
            Text = '0'
          end
          object btTemp6: TButton
            Tag = 6
            Left = 128
            Top = 188
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 11
            OnClick = btTemp1Click
          end
          object edTemp7: TEdit
            Left = 56
            Top = 224
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 12
            Text = '0'
          end
          object btTemp7: TButton
            Tag = 7
            Left = 128
            Top = 224
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 13
            OnClick = btTemp1Click
          end
          object edTemp8: TEdit
            Left = 56
            Top = 260
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 14
            Text = '0'
          end
          object btTemp8: TButton
            Tag = 8
            Left = 128
            Top = 260
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 15
            OnClick = btTemp1Click
          end
          object edTempAll: TEdit
            Left = 56
            Top = 296
            Width = 65
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 16
            Text = '0'
          end
          object btTempAll: TButton
            Tag = 9
            Left = 128
            Top = 296
            Width = 75
            Height = 25
            Caption = 'Set'
            TabOrder = 17
            OnClick = btTemp1Click
          end
          object btOK: TButton
            Left = 56
            Top = 328
            Width = 145
            Height = 49
            Caption = 'OK'
            TabOrder = 18
            OnClick = btOKClick
          end
        end
        object GroupBox7: TGroupBox
          Left = 823
          Top = 7
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object pnCrtTMPR1: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnSetTMPR1: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox8: TGroupBox
          Left = 823
          Top = 67
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object pnSetTMPR2: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR2: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox9: TGroupBox
          Left = 823
          Top = 127
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object pnSetTMPR3: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR3: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox13: TGroupBox
          Left = 823
          Top = 187
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          object pnSetTMPR4: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR4: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox12: TGroupBox
          Left = 823
          Top = 247
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  5'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          object pnSetTMPR5: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR5: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox11: TGroupBox
          Left = 823
          Top = 307
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  6'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          object pnSetTMPR6: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR6: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox10: TGroupBox
          Left = 823
          Top = 367
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  7'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          object pnSetTMPR7: TPanel
            Tag = 2
            Left = 96
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR7: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object GroupBox14: TGroupBox
          Left = 823
          Top = 426
          Width = 192
          Height = 60
          Caption = 'TEMPERATURE  8'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          object pnSetTMPR8: TPanel
            Tag = 2
            Left = 95
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnDblClick = pnCrtTMPR1DblClick
          end
          object pnCrtTMPR8: TPanel
            Tag = 2
            Left = 2
            Top = 15
            Width = 93
            Height = 43
            Align = alCustom
            Caption = '0'
            Color = clInactiveCaptionText
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -24
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnDblClick = pnCrtTMPR1DblClick
          end
        end
        object pnOperVisn: TPanel
          Left = 0
          Top = 0
          Width = 649
          Height = 502
          BevelOuter = bvNone
          Caption = 'pnOperVisn'
          Color = clSilver
          TabOrder = 9
        end
      end
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 1240
    Top = 8
  end
end
