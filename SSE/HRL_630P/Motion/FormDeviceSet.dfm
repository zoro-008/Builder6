object FrmDeviceSet: TFrmDeviceSet
  Left = 2
  Top = 0
  BorderStyle = bsNone
  Caption = 'FrmDeviceSet'
  ClientHeight = 690
  ClientWidth = 1006
  Color = clBtnFace
  Constraints.MaxHeight = 960
  Constraints.MaxWidth = 1288
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pcDeviceSet: TPageControl
    Left = 0
    Top = 0
    Width = 1006
    Height = 690
    ActivePage = tsFeed
    Align = alClient
    Font.Charset = ARABIC_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    Style = tsButtons
    TabHeight = 35
    TabIndex = 0
    TabOrder = 0
    TabWidth = 111
    OnChange = pcDeviceSetChange
    object tsFeed: TTabSheet
      Caption = 'FEED'
      ImageIndex = 2
      object Panel21: TPanel
        Left = 0
        Top = 0
        Width = 999
        Height = 645
        Align = alLeft
        TabOrder = 0
        object Panel26: TPanel
          Left = 1
          Top = 1
          Width = 997
          Height = 33
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvNone
          Caption = 'MOTOR POSITION'
          Color = clSkyBlue
          Font.Charset = HANGEUL_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object Panel27: TPanel
          Left = 1
          Top = 472
          Width = 997
          Height = 172
          Align = alBottom
          TabOrder = 1
          object Panel28: TPanel
            Left = 1
            Top = 1
            Width = 995
            Height = 25
            Align = alTop
            BevelInner = bvRaised
            BevelOuter = bvNone
            Caption = 'CYLINDER'
            Color = clSkyBlue
            Font.Charset = HANGEUL_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnFED1: TPanel
            Left = 8
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 1
          end
          object pnFED2: TPanel
            Left = 200
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 2
          end
          object pnFED3: TPanel
            Left = 392
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 3
          end
          object pnFED4: TPanel
            Left = 584
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 4
          end
          object pnFED5: TPanel
            Left = 776
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 5
            Visible = False
          end
        end
        object gbMotrPos02: TGroupBox
          Left = 8
          Top = 40
          Width = 419
          Height = 274
          Caption = 'Front Feed Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          object pnMotrPos0: TPanel
            Left = 2
            Top = 21
            Width = 416
            Height = 251
            Caption = '0'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
        end
        object gbMotrPos03: TGroupBox
          Left = 504
          Top = 40
          Width = 419
          Height = 274
          Caption = 'Rear Feed Y'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          object pnMotrPos1: TPanel
            Left = 2
            Top = 21
            Width = 416
            Height = 251
            Caption = '1'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
        end
        object Panel4: TPanel
          Left = 1
          Top = 300
          Width = 997
          Height = 172
          Align = alBottom
          TabOrder = 4
          object Panel5: TPanel
            Left = 1
            Top = 1
            Width = 995
            Height = 25
            Align = alTop
            BevelInner = bvRaised
            BevelOuter = bvNone
            Caption = 'CYLINDER'
            Color = clSkyBlue
            Font.Charset = HANGEUL_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnAtcr0: TPanel
            Left = 8
            Top = 36
            Width = 170
            Height = 126
            Caption = '0'
            TabOrder = 1
          end
          object pnAtcr1: TPanel
            Left = 200
            Top = 36
            Width = 170
            Height = 126
            Caption = '1'
            TabOrder = 2
          end
          object pnAtcr2: TPanel
            Left = 392
            Top = 36
            Width = 170
            Height = 126
            Caption = '2'
            TabOrder = 3
          end
          object pnAtcr3: TPanel
            Left = 584
            Top = 36
            Width = 170
            Height = 126
            Caption = '3'
            TabOrder = 4
          end
        end
      end
      object pnJog1: TPanel
        Left = 733
        Top = 0
        Width = 265
        Height = 645
        Align = alRight
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 1
        object PageControl7: TPageControl
          Left = 4
          Top = 123
          Width = 257
          Height = 690
          ActivePage = TabSheet15
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabIndex = 0
          TabOrder = 0
          object TabSheet15: TTabSheet
            Caption = 'Move Motor'
            object Panel49: TPanel
              Left = 127
              Top = 52
              Width = 89
              Height = 25
              BevelOuter = bvNone
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 0
            end
            object gbMotrJog02: TGroupBox
              Left = 0
              Top = 0
              Width = 250
              Height = 163
              Caption = 'Front Feed Y'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
              object pnMotrJog0: TPanel
                Left = 2
                Top = 18
                Width = 246
                Height = 143
                Align = alClient
                BevelOuter = bvNone
                Caption = '0'
                TabOrder = 0
              end
            end
            object GroupBox1: TGroupBox
              Left = 0
              Top = 165
              Width = 250
              Height = 163
              Caption = 'Rear Feed Y'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
              object pnMotrJog1: TPanel
                Left = 2
                Top = 18
                Width = 246
                Height = 143
                Align = alClient
                BevelOuter = bvNone
                Caption = '1'
                TabOrder = 0
              end
            end
          end
        end
        object pnJogUnit: TPanel
          Left = 2
          Top = 2
          Width = 261
          Height = 118
          Align = alTop
          Caption = 'pnJogUnit'
          TabOrder = 1
          object rgJogUnit1: TRadioGroup
            Left = 6
            Top = 2
            Width = 247
            Height = 111
            Caption = 'JOG UNIT'
            Columns = 2
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ItemIndex = 0
            Items.Strings = (
              'JOG'
              '1mm'
              '0.5mm'
              '0.1mm'
              '0.05mm'
              '             mm')
            ParentFont = False
            TabOrder = 1
            OnClick = rgJogUnit1Click
          end
          object edJogUnit: TEdit
            Left = 150
            Top = 78
            Width = 52
            Height = 28
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
            ParentFont = False
            TabOrder = 0
          end
        end
      end
    end
    object tsLaser: TTabSheet
      Caption = 'LASER'
      ImageIndex = 3
      object Panel29: TPanel
        Left = 0
        Top = 0
        Width = 999
        Height = 645
        Align = alLeft
        TabOrder = 0
        object Label1: TLabel
          Left = 787
          Top = 368
          Width = 83
          Height = 19
          Caption = 'Ex)GA,001 '
        end
        object Panel34: TPanel
          Left = 1
          Top = 1
          Width = 997
          Height = 33
          Align = alTop
          BevelInner = bvRaised
          BevelOuter = bvNone
          Caption = 'MOTOR POSITION'
          Color = clSkyBlue
          Font.Charset = HANGEUL_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object GroupBox21: TGroupBox
          Left = 8
          Top = 40
          Width = 419
          Height = 274
          Caption = 'Front Laser T'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          object pnMotrPos2: TPanel
            Left = 2
            Top = 21
            Width = 416
            Height = 251
            Caption = '2'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
        end
        object GroupBox22: TGroupBox
          Left = 504
          Top = 40
          Width = 419
          Height = 274
          Caption = 'Rear Laser T'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          object pnMotrPos3: TPanel
            Left = 2
            Top = 21
            Width = 416
            Height = 251
            Caption = '3'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
        end
        object GroupBox18: TGroupBox
          Left = 426
          Top = 40
          Width = 72
          Height = 274
          Caption = 'LASER'
          TabOrder = 3
          object cbUsedLsrPos1: TCheckBox
            Left = 5
            Top = 62
            Width = 15
            Height = 17
            TabOrder = 0
          end
          object edFrntLsrNo1: TEdit
            Left = 22
            Top = 57
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
          end
          object cbUsedLsrPos2: TCheckBox
            Left = 5
            Top = 92
            Width = 15
            Height = 17
            TabOrder = 2
          end
          object edFrntLsrNo2: TEdit
            Left = 22
            Top = 88
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 3
          end
          object cbUsedLsrPos3: TCheckBox
            Left = 5
            Top = 123
            Width = 15
            Height = 17
            TabOrder = 4
          end
          object edFrntLsrNo3: TEdit
            Left = 22
            Top = 119
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 5
          end
          object cbUsedLsrPos4: TCheckBox
            Left = 5
            Top = 154
            Width = 15
            Height = 17
            TabOrder = 6
          end
          object edFrntLsrNo4: TEdit
            Left = 22
            Top = 150
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 7
          end
          object cbUsedLsrPos5: TCheckBox
            Left = 5
            Top = 185
            Width = 15
            Height = 17
            TabOrder = 8
          end
          object edFrntLsrNo5: TEdit
            Left = 22
            Top = 181
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 9
          end
        end
        object GroupBox19: TGroupBox
          Left = 922
          Top = 40
          Width = 72
          Height = 274
          Caption = 'LASER'
          TabOrder = 4
          object edBackLsrNo1: TEdit
            Left = 22
            Top = 57
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
          end
          object edBackLsrNo2: TEdit
            Left = 22
            Top = 88
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
          end
          object edBackLsrNo3: TEdit
            Left = 22
            Top = 119
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
          end
          object edBackLsrNo4: TEdit
            Left = 22
            Top = 150
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 3
          end
          object edBackLsrNo5: TEdit
            Left = 22
            Top = 181
            Width = 44
            Height = 27
            ImeName = 'Microsoft IME 2010'
            TabOrder = 4
          end
        end
        object edSendGA: TEdit
          Left = 784
          Top = 336
          Width = 121
          Height = 27
          ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
          TabOrder = 5
          Text = 'GA,'
        end
        object btSend: TButton
          Left = 912
          Top = 336
          Width = 85
          Height = 25
          Caption = 'GA SEND'
          TabOrder = 6
          OnClick = btSendClick
        end
        object Panel1: TPanel
          Left = 1
          Top = 645
          Width = 997
          Height = 172
          Align = alBottom
          TabOrder = 7
          object Panel2: TPanel
            Left = 1
            Top = 1
            Width = 995
            Height = 25
            Align = alTop
            BevelInner = bvRaised
            BevelOuter = bvNone
            Caption = 'CYLINDER'
            Color = clSkyBlue
            Font.Charset = HANGEUL_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnLSR1: TPanel
            Left = 8
            Top = 36
            Width = 170
            Height = 126
            Caption = 'pnLSR1'
            TabOrder = 1
          end
          object pnLSR2: TPanel
            Left = 200
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 2
          end
          object pnLSR3: TPanel
            Left = 392
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 3
          end
          object pnLSR4: TPanel
            Left = 584
            Top = 36
            Width = 170
            Height = 126
            TabOrder = 4
          end
        end
        object Panel9: TPanel
          Left = 1
          Top = 473
          Width = 997
          Height = 172
          Align = alBottom
          TabOrder = 8
          object Panel10: TPanel
            Left = 1
            Top = 1
            Width = 995
            Height = 25
            Align = alTop
            BevelInner = bvRaised
            BevelOuter = bvNone
            Caption = 'CYLINDER'
            Color = clSkyBlue
            Font.Charset = HANGEUL_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnAtcr4: TPanel
            Left = 8
            Top = 36
            Width = 170
            Height = 126
            Caption = '0'
            TabOrder = 1
          end
          object pnAtcr5: TPanel
            Left = 200
            Top = 36
            Width = 170
            Height = 126
            Caption = '1'
            TabOrder = 2
          end
        end
      end
      object pnJog2: TPanel
        Left = 733
        Top = 0
        Width = 265
        Height = 645
        Align = alRight
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 1
        object PageControl8: TPageControl
          Left = 4
          Top = 123
          Width = 257
          Height = 690
          ActivePage = TabSheet16
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabIndex = 0
          TabOrder = 0
          object TabSheet16: TTabSheet
            Caption = 'Move Motor'
            object GroupBox23: TGroupBox
              Left = 0
              Top = 0
              Width = 250
              Height = 163
              Caption = 'Front Laser T'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 0
              object pnMotrJog2: TPanel
                Left = 2
                Top = 18
                Width = 246
                Height = 143
                Align = alClient
                BevelOuter = bvNone
                Caption = '2'
                TabOrder = 0
              end
            end
            object GroupBox24: TGroupBox
              Left = 0
              Top = 165
              Width = 250
              Height = 163
              Caption = 'Rear Laser T'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
              object pnMotrJog3: TPanel
                Left = 2
                Top = 18
                Width = 246
                Height = 143
                Align = alClient
                BevelOuter = bvNone
                Caption = '3'
                TabOrder = 0
              end
            end
          end
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'VISION'
      ImageIndex = 2
      object pnSetVisn: TPanel
        Left = 0
        Top = 0
        Width = 1264
        Height = 277
        Align = alTop
        Caption = 'pnSetVisn'
        TabOrder = 0
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 279
        Width = 284
        Height = 538
        Caption = 'Vision Set'
        TabOrder = 1
        object pnSetPara: TPanel
          Left = 2
          Top = 21
          Width = 280
          Height = 515
          Align = alClient
          Caption = 'pnSetPara'
          TabOrder = 0
        end
      end
    end
  end
  object btSaveDevice: TBitBtn
    Left = 1142
    Top = 0
    Width = 130
    Height = 35
    Caption = '  SAVE '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btSavePosClick
    Glyph.Data = {
      F6060000424DF606000000000000360000002800000018000000180000000100
      180000000000C006000000000000000000000000000000000000AF611CAB5916
      AB5916AF611CB26E2DB06B2AB06B2AB06B2AB06B2AB06B2AB06B2AB06B2AB06B
      2AB06B2AB06B2AB06B2AB06B2AB06B2AB06B2AB26E2DAF611CAC5B18AB5915AC
      5B18AF611CEEC86DE59A147F8495DCE0E4D6D9DED6D9DED6D9DED6D9DED6D9DE
      D6D9DED6D9DED6D9DED6D9DED6D9DED6D9DED6D9DED6D9DED6D9DEDCE0E47F84
      95E69D19DE9A22AC5B18BC7228ECC979E39916908F9DDEDDDCDBDAD8DEDCDBDE
      DEDCDFDEDDE0DFDDE0E0DEE1E0DFE2E1DFE2E2E0E3E2E1E4E3E1E5E3E2E5E3E2
      E1E0DEE0DEDD908F9DE49C1DE0A028B6681CBD762DEECD80E49B1E9594A1E4E5
      E3AFA4A69A898E91818787777F7F717576676D6D5F6664575F5D4F5655484E4E
      4249473A433C32395F555BE9E9E89594A1E59E25E2A231BA6B20BF7730EFCF85
      E59E259999A5E5E6E4E7E8E6E7E8E6E7E8E6E7E8E6E7E8E6E7E8E6E7E8E6E7E8
      E6E7E8E6E7E8E6E7E8E6E7E8E6E7E8E6E7E8E6E7E8E69999A5E7A12BE4A738BC
      6C22C37A31F0D18CE6A32B9F9DAAECEAE9DCDADAD9D8D7DAD6D7D8D7D6D8D5D6
      D7D4D4D6D4D3D6D2D3D4D2D1D4D1D1D3D1D0D2D0CFD0CDCED3D2D1EDECEA9F9D
      AAE7A631E5AB3EBE7026C57F35F1D390E7A732A4A2AFF0F0EEB2A8AA97878B8F
      7F8485757D7D6F7374646B6C5D6362555D5B4D5453464C4C3F47453841392F36
      62565DF4F5F3A4A2AFE9AA39E6AE46C07327C78335F0D394E9AB38A8A7B4F0F1
      F0F0F1F0F4F5F3F5F7F5F6F8F6F7F9F7F8F9F8F9FBF8FAFBFAFBFDFAFCFEFCFD
      FEFDFEFFFDFEFFFDF8F9F8F2F2F1A8A7B4EAAE3EE8B34BC37729C98539F2D598
      EBAE3EACAAB8F6F5F5F8F7F7F8F7F7F8F7F7F8F7F7F8F7F7F8F7F7F8F7F7F8F7
      F7F8F7F7F8F7F7F8F7F7F8F7F7F8F7F7F8F7F7F8F7F7ACAAB8ECB143EAB650C6
      7B2CCB8939F3D79DECB244B2AFBDF9FAF8C7C0C2B3A9ABAEA3A6A79D9FA0999B
      9B9194968B908F858B898185847A8080737B78707670686D928B8EFDFEFCB2AF
      BDEDB549EBBA56C87D2ECD8D3DF4D8A0EEB549B6B7C2FAF9F9F7F6F6F9F9F9FA
      F9F9FAFAFAFBFBFBFCFBFBFCFBFBFCFCFCFDFDFCFEFDFDFEFDFDFEFEFDFEFEFD
      FBFBFBFAFAFAB6B7C2EEB74FEDBB5BCA8531D09040F3DAA5EEBA50A6A6B5FFFF
      FFFEFFFFFEFFFFFEFFFFFEFFFFFEFFFFFEFFFFFEFFFFFEFFFFFEFFFFFEFFFFFE
      FFFFFEFFFFFEFFFFFEFFFFFFFFFFA6A6B5EFBC56EEBF61CD8833D29442F5DCAA
      F0BF5CB8926991796B977B67977B65977B65977B65977B65977B65977B65977B
      65977B65977B65977B65977B65977B65977B6791796BB89269F0C163F0C268CF
      8A35D49644F6DDACEEBE63F2C367FAC864A58973B8A28AB6A088B6A088B6A088
      B6A088B6A088B6A088B6A088B6A088B6A088B6A088B8A28AA58973FAC864F2C3
      67EFC068F1C46CD28F37D69A47F7E0AFEFC367F0C56CF6C867A29EAAD8D8D9D4
      D3D5D4D3D5D4D3D5D4D3D5D4D3D5D4D3D5D5D4D5D7D6D7D7D7D8D7D7D7D8D9DA
      A29EABF6C867F0C56CF0C56CF3C970D4933AD99D49F7E1B3F0C66CF0C771F7CA
      6BACA9B4DBD9D8D8D6D5D8D6D5D8D6D5D8D6D5D8D6D5D8D6D5DBD9D9BAB7B9BA
      B8BCB9B6BBD1D0D0AFADB5F7CA6BF0C771F0C771F2CC76D8973CDBA04CF8E4B5
      F1C870F1C975F7CC6FB3AEBAE0DFDEDDDCDBDDDCDBDDDCDBDDDCDBDDDCDBDDDC
      DBE5E6E7654F3DA68257A57F51938F95B8B5BFF7CC6FF1C975F1C975F3CE7ADA
      9B3FDEA64EF9E5B8F1CB74F2CD79F7CE73B8B4BFE4E3E2E1E0DFE1E0DFE1E0DF
      E1E0DFE1E0DFE1E0DFE7E9EBA38052FFD97FFFD67A918F9BC0BDC5F7CE73F2CD
      79F2CD79F4D17EDC9F42E0A951FAE4BAF3CD79F4CE7DF9D077BCB9C4E8E7E6E5
      E4E3E5E4E3E5E4E3E5E4E3E5E4E3E5E4E3EBECF09F7E54FBD580FDD47A96949C
      C3C0CBF9D077F4CE7DF4CE7DF6D382DFA144E2AB52FBE7BCF4D17DF5D281F9D3
      7BC2BEC9ECECEBE9E9E8E9E9E8E9E9E8E9E9E8E9E9E8E9E9E8EFF2F5A28156FF
      DB86FFDA819996A0C9C5D0F9D37BF5D281F5D282F8D98BE0A243E4AF54F9E8BE
      F4D37DF5D483F8D47DC8C5CDF0F0EFEDEDECEDEDECEDEDECEDEDECEDEDECEDED
      ECF5F7F983694ADCB871DAB36A9E9BA1CCCAD4F8D47DF5D584F7D98BE7B453E4
      AB5AE6B359FBEED1F6D98DF5D480F9D57CCBC8D0F4F4F3F1F1F0F1F1F0F1F1F0
      F1F1F0F1F1F0F1F1F0F7F8F69D9AA0A09EA59C9AA1D5D3D4CECBD3F9D67CF8DA
      8DE8B656E5AB51FFFFFFE6AC49FBEDCCFCEED2FBE9C0FFEBBDDBDDEBFBFFFFF9
      FDFFF9FDFFF9FDFFF9FDFFF9FDFFF9FDFFFAFEFFFFFFFFFFFFFFFFFFFFFFFFFF
      DBDEECFFEDC5ECBE66E6AE52FFFFFFFFFFFFE5A942E7B14CEBBB5CEBBA5CEBBB
      5BE7BB69E7BB69EABF6BEABE6BEABE6BEABE6BEABE6BEABE6BEABE6BEABE6BEA
      BE6BEABF6BE7BB69E0AF5DEAB853EAB65EFFFFFFFFFFFFFFFFFF}
    Layout = blGlyphRight
  end
  object btDustSuck: TBitBtn
    Left = 1011
    Top = 0
    Width = 130
    Height = 35
    Caption = 'DST SUCKTION'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = btDustSuckClick
    Layout = blGlyphRight
  end
  object tmUpdateTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimerTimer
    Left = 1232
    Top = 40
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimerTimer
    Left = 1232
    Top = 40
  end
end
