object FrmPassword: TFrmPassword
  Left = 936
  Top = 376
  BorderStyle = bsDialog
  Caption = 'FrmPassword'
  ClientHeight = 163
  ClientWidth = 335
  Color = clSkyBlue
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = [fsBold]
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object SpeedButton1: TSpeedButton
    Left = 114
    Top = 117
    Width = 106
    Height = 43
    Caption = 'Enter'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 223
    Top = 117
    Width = 106
    Height = 43
    Caption = 'Esc'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    OnMouseDown = SpeedButton2MouseDown
  end
  object SpeedButton_Change: TSpeedButton
    Left = 7
    Top = 117
    Width = 105
    Height = 43
    Caption = 'Change'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    OnMouseDown = SpeedButton_ChangeMouseDown
  end
  object Panel_Input: TPanel
    Left = 3
    Top = 4
    Width = 330
    Height = 109
    TabOrder = 1
    object Label_Mode: TLabel
      Left = 11
      Top = 47
      Width = 131
      Height = 13
      Caption = 'INPUT PASSWORD'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = #44404#47548
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 328
      Height = 28
      Align = alTop
      Alignment = taLeftJustify
      BevelInner = bvLowered
      Caption = '       PASSWORD Check'
      Color = 11889258
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object Image4: TImage
        Left = 6
        Top = 7
        Width = 16
        Height = 16
        AutoSize = True
        Picture.Data = {
          07544269746D6170F6000000424DF60000000000000076000000280000001000
          000010000000010004000000000080000000C40E0000C40E0000100000000000
          0000000000000000BF0000BF000000BFBF00BF000000BF00BF00BFBF0000C0C0
          C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
          FF00FFFFFFFFFFFFFFFFF0000000000000FFF0877777777780FFF08777808777
          80FFF0877780877780FFF0877780877780FFF0877700077780FFF08777777777
          80FFF0877777777780FFF0000000000000FFFFF070FFF070FFFFFFF0708F8070
          FFFFFFF087000780FFFFFFFF0877780FFFFFFFFFF00000FFFFFFFFFFFFFFFFFF
          FFFF}
        Transparent = True
      end
    end
    object Edit1: TEdit
      Left = 10
      Top = 69
      Width = 303
      Height = 28
      AutoSize = False
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = 14737632
      Ctl3D = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = #44404#47548
      Font.Style = [fsBold]
      ImeName = #54620#44397#50612'('#54620#44544') (MS-IME98)'
      ParentCtl3D = False
      ParentFont = False
      PasswordChar = '*'
      TabOrder = 1
    end
  end
  object Panel_Change: TPanel
    Left = 3
    Top = 4
    Width = 330
    Height = 108
    TabOrder = 0
    object Label1: TLabel
      Left = 6
      Top = 42
      Width = 101
      Height = 13
      Caption = 'Old Password'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = #44404#47548
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 8
      Top = 77
      Width = 107
      Height = 13
      Caption = 'New Password'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = #44404#47548
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 328
      Height = 28
      Align = alTop
      Alignment = taLeftJustify
      BevelInner = bvLowered
      Caption = '       PASSWORD Change'
      Color = 11889258
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object Image3: TImage
        Left = 4
        Top = 5
        Width = 18
        Height = 18
        AutoSize = True
        Picture.Data = {
          07544269746D61709E050000424D9E0500000000000036040000280000001200
          000012000000010008000000000068010000C40E0000C40E0000000100000000
          000000000000000080000080000000808000800000008000800080800000C0C0
          C000C0DCC000F0CAA60000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000F0FBFF00A4A0
          A000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
          FF00030303030303040404040404030303030303000003020403030402020202
          0202040403030303000003020204040202020202020202020403030300000302
          0202020202FAFAFAFAFA0202020403030000030202020202FA0303030303FA02
          02020403000003020202020203030303030303FA020204030000030202020202
          02030303030303FA04040403000003FAFAFAFAFAFAFA03030303030303030303
          0000030303030303030303030303030303030303000003030303030303030303
          0303040404040404000003FA040404030303030303FA020202020204000003FA
          02020403030303030303FA020202020400000303FA0202040303030303040402
          0202020400000303FA0202020404040404020202020202040000030303FA0202
          0202020202020202FAFA0204000003030303FAFA020202020202FAFA0303FA03
          0000030303030303FAFAFAFAFAFA030303030303000003030303030303030303
          03030303030303030000}
        Transparent = True
      end
    end
    object Edit2: TEdit
      Left = 122
      Top = 34
      Width = 200
      Height = 29
      AutoSize = False
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = 14211288
      Ctl3D = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ImeName = #54620#44397#50612'('#54620#44544') (MS-IME98)'
      ParentCtl3D = False
      ParentFont = False
      PasswordChar = '*'
      TabOrder = 1
    end
    object Edit3: TEdit
      Left = 122
      Top = 71
      Width = 200
      Height = 29
      AutoSize = False
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = 14211288
      Ctl3D = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ImeName = #54620#44397#50612'('#54620#44544') (MS-IME98)'
      ParentCtl3D = False
      ParentFont = False
      PasswordChar = '*'
      TabOrder = 2
    end
  end
end
