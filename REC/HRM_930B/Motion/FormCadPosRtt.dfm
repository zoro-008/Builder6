object FrmCadPosRtt: TFrmCadPosRtt
  Left = 193
  Top = 173
  Width = 1280
  Height = 864
  Caption = 'FrmCadPosRtt'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnTopToolbar: TPanel
    Left = 0
    Top = 0
    Width = 1264
    Height = 25
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object btZoomOut: TSpeedButton
      Tag = 1
      Left = 1
      Top = 0
      Width = 25
      Height = 25
      Hint = 'Zoom Out'
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000000000000000000000000000000000003B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B0000000A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000006868686868680A0A0A3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000003B3B
        3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B0000003B3B3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000003B3B3B6868688080
        800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0A0A0A0A0A0A0A3B
        3B3B0000003B3B3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        0A0A0A2626265B5B5B6D6D6D8181812626263333336868688080800A0A0A3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0A5B5B5B818181AAAAAAAAAAAAAAAAAAAA
        AAAA8181813333330A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B262626
        818181AAAAAAC0C0C0C0C0C0C0C0C0C0C0C0AAAAAA0A0A0A3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B0A0A0A818181AAAAAAC0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0AAAAAA0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0AAAAAAA
        000000000000000000000000000000000000000000C0C0C00000003B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B0A0A0AAAAAAAC0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0DBDBDB0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B000000
        C0C0C0FFFFFFC0C0C0C0C0C0C0C0C0C0C0C0DBDBDB0000003B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B000000C0C0C0C0C0C0FFFFFFC0C0C0C0C0C0DB
        DBDBDDDDDD0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        000000000000DDDDDDDBDBDBDDDDDD0000000000003B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000000000000000003B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B}
      ParentShowHint = False
      ShowHint = True
      OnClick = btZoomOutClick
    end
    object btZoomIn: TSpeedButton
      Tag = 2
      Left = 26
      Top = 0
      Width = 25
      Height = 25
      Hint = 'Zoom In'
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000000000000000000000000000000000003B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B0000000A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000006868686868680A0A0A3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000003B3B
        3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B0000003B3B3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000003B3B3B6868688080
        800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0A0A0A0A0A0A0A3B
        3B3B0000003B3B3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        0A0A0A2626265B5B5B6D6D6D8181812626263333336868688080800A0A0A3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0A5B5B5B818181AAAAAA000000AAAAAAAA
        AAAA8181813333330A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B262626
        818181AAAAAAC0C0C0000000C0C0C0C0C0C0AAAAAA0A0A0A3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B0A0A0A818181AAAAAAC0C0C0C0C0C0000000C0C0C0C0
        C0C0C0C0C0AAAAAA0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0AAAAAAA
        000000000000000000000000000000000000000000C0C0C00000003B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B0A0A0AAAAAAAC0C0C0C0C0C0C0C0C0000000C0C0C0C0
        C0C0C0C0C0DBDBDB0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B000000
        C0C0C0FFFFFFC0C0C0000000C0C0C0C0C0C0DBDBDB0000003B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B000000C0C0C0C0C0C0FFFFFF000000C0C0C0DB
        DBDBDDDDDD0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        000000000000DDDDDDDBDBDBDDDDDD0000000000003B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000000000000000003B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B}
      ParentShowHint = False
      ShowHint = True
      OnClick = btZoomInClick
    end
    object btRealSize: TSpeedButton
      Tag = 3
      Left = 52
      Top = 0
      Width = 25
      Height = 25
      Hint = 'Real Size'
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000000000000000000000000000000000003B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B0000000A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000006868686868680A0A0A3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000003B3B
        3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B0000003B3B3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000003B3B3B6868688080
        800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0A0A0A0A0A0A0A3B
        3B3B0000003B3B3B6868688080800A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        0A0A0A2626265B5B5B6D6D6D8181812626263333336868688080800A0A0A3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0A5B5B5B818181AAAAAAAAAAAAAAAAAAAA
        AAAA8181813333330A0A0A3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B262626
        818181AAAAAAC0C0C0C0C0C0C0C0C0C0C0C0AAAAAA0A0A0A3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B0A0A0A818181AAAAAAC0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0AAAAAA0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0A0A0AAAAAAA
        C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C00000003B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B0A0A0AAAAAAAC0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
        C0C0C0C0C0DBDBDB0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B000000
        C0C0C0FFFFFFC0C0C0C0C0C0C0C0C0C0C0C0DBDBDB0000003B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B000000C0C0C0C0C0C0FFFFFFC0C0C0C0C0C0DB
        DBDBDDDDDD0000003B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B
        000000000000DDDDDDDBDBDBDDDDDD0000000000003B3B3B3B3B3B3B3B3B3B3B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B0000000000000000003B
        3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B3B}
      ParentShowHint = False
      ShowHint = True
      OnClick = btRealSizeClick
    end
    object Label35: TLabel
      Left = 112
      Top = 8
      Width = 6
      Height = 13
      Caption = '0'
    end
    object Label36: TLabel
      Left = 208
      Top = 8
      Width = 6
      Height = 13
      Caption = '0'
    end
  end
  object pnBase: TPanel
    Left = 0
    Top = 25
    Width = 1264
    Height = 801
    Align = alClient
    BevelOuter = bvNone
    Color = clAppWorkSpace
    TabOrder = 1
    object Panel1: TPanel
      Left = 801
      Top = 0
      Width = 713
      Height = 801
      Caption = 'Panel1'
      TabOrder = 0
      object Label22: TLabel
        Left = 8
        Top = 576
        Width = 7
        Height = 13
        Caption = 'X'
      end
      object Label23: TLabel
        Left = 8
        Top = 600
        Width = 7
        Height = 13
        Caption = 'Y'
      end
      object lbPos: TLabel
        Left = 16
        Top = 632
        Width = 49
        Height = 13
        Caption = 'x:0.0,y:0.0'
      end
      object cbObjectList: TComboBox
        Left = 16
        Top = 8
        Width = 289
        Height = 21
        DropDownCount = 50
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 0
        Text = '2'
        OnChange = cbObjectListChange
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6')
      end
      object pcObjType: TPageControl
        Left = 8
        Top = 112
        Width = 453
        Height = 457
        ActivePage = tsCadDspCmd
        TabIndex = 4
        TabOrder = 1
        OnChange = pcObjTypeChange
        object TabSheet1: TTabSheet
          Caption = 'Object'
          ImageIndex = 6
        end
        object tsPoint: TTabSheet
          Caption = 'Point'
          object Label1: TLabel
            Left = 8
            Top = 36
            Width = 119
            Height = 16
            Caption = 'Center Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label2: TLabel
            Left = 8
            Top = 64
            Width = 120
            Height = 16
            Caption = 'Center Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edPntCntrPosX: TEdit
            Left = 152
            Top = 36
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object edPntCntrPosY: TEdit
            Left = 152
            Top = 64
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
            Text = '0'
          end
        end
        object TabSheet2: TTabSheet
          Caption = 'CntPoint'
          ImageIndex = 7
          object Label33: TLabel
            Left = 8
            Top = 36
            Width = 119
            Height = 16
            Caption = 'Center Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label34: TLabel
            Left = 8
            Top = 64
            Width = 120
            Height = 16
            Caption = 'Center Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edCntPntCntrPosX: TEdit
            Left = 152
            Top = 36
            Width = 121
            Height = 21
            Color = clScrollBar
            ImeName = 'Microsoft IME 2010'
            ReadOnly = True
            TabOrder = 0
            Text = '0'
          end
          object edCntPntCntrPosY: TEdit
            Left = 152
            Top = 64
            Width = 121
            Height = 21
            Color = clScrollBar
            ImeName = 'Microsoft IME 2010'
            ReadOnly = True
            TabOrder = 1
            Text = '0'
          end
        end
        object VisnPoint: TTabSheet
          Caption = 'VisnPoint'
          ImageIndex = 9
          object Label20: TLabel
            Left = 8
            Top = 36
            Width = 119
            Height = 16
            Caption = 'Center Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label21: TLabel
            Left = 8
            Top = 64
            Width = 120
            Height = 16
            Caption = 'Center Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edVisnPntCntrPosX: TEdit
            Left = 152
            Top = 36
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object edVisnPntCntrPosY: TEdit
            Left = 152
            Top = 64
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
            Text = '0'
          end
        end
        object tsCadDspCmd: TTabSheet
          Caption = 'DspNodes'
          ImageIndex = 1
          object Label3: TLabel
            Left = 8
            Top = 20
            Width = 82
            Height = 16
            Caption = 'Node Count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edDspCmdNodeCnt: TEdit
            Left = 152
            Top = 20
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object sgDspCmdPos: TStringGrid
            Left = 8
            Top = 48
            Width = 425
            Height = 361
            TabOrder = 1
            OnDrawCell = sgDspCmdPosDrawCell
            OnSelectCell = sgDspCmdPosSelectCell
          end
          object Button2: TButton
            Left = 296
            Top = 16
            Width = 75
            Height = 25
            Caption = 'Insert'
            TabOrder = 2
            OnClick = Button2Click
          end
          object Button3: TButton
            Left = 376
            Top = 16
            Width = 75
            Height = 25
            Caption = 'Delete'
            TabOrder = 3
            OnClick = Button3Click
          end
        end
        object tsCadLine: TTabSheet
          Caption = 'Line'
          ImageIndex = 2
          object Label4: TLabel
            Left = 8
            Top = 20
            Width = 106
            Height = 16
            Caption = 'Start Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label5: TLabel
            Left = 8
            Top = 48
            Width = 107
            Height = 16
            Caption = 'Start Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label6: TLabel
            Left = 8
            Top = 76
            Width = 101
            Height = 16
            Caption = 'End Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label7: TLabel
            Left = 8
            Top = 104
            Width = 102
            Height = 16
            Caption = 'End Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edLineSttPosX: TEdit
            Left = 152
            Top = 20
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object edLineSttPosY: TEdit
            Left = 152
            Top = 48
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
            Text = '0'
          end
          object edLineEndPosX: TEdit
            Left = 152
            Top = 76
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
            Text = '0'
          end
          object edLineEndPosY: TEdit
            Left = 152
            Top = 104
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 3
            Text = '0'
          end
        end
        object tsCadHeight: TTabSheet
          Caption = 'Height'
          ImageIndex = 3
          object Label8: TLabel
            Left = 8
            Top = 20
            Width = 106
            Height = 16
            Caption = 'Start Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label9: TLabel
            Left = 8
            Top = 48
            Width = 107
            Height = 16
            Caption = 'Start Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label10: TLabel
            Left = 8
            Top = 76
            Width = 101
            Height = 16
            Caption = 'Sub Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label11: TLabel
            Left = 8
            Top = 104
            Width = 102
            Height = 16
            Caption = 'Sub Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label12: TLabel
            Left = 8
            Top = 132
            Width = 101
            Height = 16
            Caption = 'End Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label13: TLabel
            Left = 8
            Top = 160
            Width = 102
            Height = 16
            Caption = 'End Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edHghtSttPosX: TEdit
            Left = 152
            Top = 20
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object edHghtSttPosY: TEdit
            Left = 152
            Top = 48
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
            Text = '0'
          end
          object edHghtSubPosX: TEdit
            Left = 152
            Top = 76
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
            Text = '0'
          end
          object edHghtSubPosY: TEdit
            Left = 152
            Top = 104
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 3
            Text = '0'
          end
          object edHghtEndPosX: TEdit
            Left = 152
            Top = 132
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 4
            Text = '0'
          end
          object edHghtEndPosY: TEdit
            Left = 152
            Top = 160
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 5
            Text = '0'
          end
        end
        object tsCadRect: TTabSheet
          Caption = 'Rect'
          ImageIndex = 4
          object Label14: TLabel
            Left = 8
            Top = 20
            Width = 62
            Height = 16
            Caption = 'Rect Left'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label15: TLabel
            Left = 8
            Top = 48
            Width = 65
            Height = 16
            Caption = 'Rect Top'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label16: TLabel
            Left = 8
            Top = 76
            Width = 76
            Height = 16
            Caption = 'Rect Width'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label17: TLabel
            Left = 8
            Top = 104
            Width = 82
            Height = 16
            Caption = 'Rect Height'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label18: TLabel
            Left = 8
            Top = 132
            Width = 73
            Height = 16
            Caption = 'Rect Right'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label19: TLabel
            Left = 8
            Top = 160
            Width = 85
            Height = 16
            Caption = 'Rect Bottom'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edRectLeft: TEdit
            Left = 152
            Top = 20
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object edRectTop: TEdit
            Left = 152
            Top = 48
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
            Text = '0'
          end
          object edRectWidth: TEdit
            Left = 152
            Top = 76
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
            Text = '0'
          end
          object edRectHeight: TEdit
            Left = 152
            Top = 104
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 3
            Text = '0'
          end
          object edRectRight: TEdit
            Left = 152
            Top = 132
            Width = 121
            Height = 21
            Color = clScrollBar
            Enabled = False
            ImeName = 'Microsoft IME 2010'
            TabOrder = 4
            Text = '0'
          end
          object edRectBottom: TEdit
            Left = 152
            Top = 160
            Width = 121
            Height = 21
            Color = clScrollBar
            Enabled = False
            ImeName = 'Microsoft IME 2010'
            TabOrder = 5
            Text = '0'
          end
        end
        object tsHghtCmd: TTabSheet
          Caption = 'HeightNodes'
          ImageIndex = 5
          object Label26: TLabel
            Left = 11
            Top = 16
            Width = 82
            Height = 16
            Caption = 'Node Count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edHghtCmdNodeCnt: TEdit
            Left = 155
            Top = 16
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object sgHghtCmdPos: TStringGrid
            Left = 8
            Top = 44
            Width = 425
            Height = 377
            TabOrder = 1
            OnSelectCell = sgHghtCmdPosSelectCell
          end
        end
        object TabSheet3: TTabSheet
          Caption = 'Rhombus'
          ImageIndex = 8
          object Label27: TLabel
            Left = 8
            Top = 20
            Width = 95
            Height = 16
            Caption = 'Rhombus Left'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label28: TLabel
            Left = 8
            Top = 48
            Width = 98
            Height = 16
            Caption = 'Rhombus Top'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label29: TLabel
            Left = 8
            Top = 76
            Width = 109
            Height = 16
            Caption = 'Rhombus Width'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label30: TLabel
            Left = 8
            Top = 104
            Width = 115
            Height = 16
            Caption = 'Rhombus Height'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label31: TLabel
            Left = 8
            Top = 132
            Width = 106
            Height = 16
            Caption = 'Rhombus Right'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label32: TLabel
            Left = 8
            Top = 160
            Width = 118
            Height = 16
            Caption = 'Rhombus Bottom'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label41: TLabel
            Left = 8
            Top = 188
            Width = 119
            Height = 16
            Caption = 'Centor Position X'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label42: TLabel
            Left = 8
            Top = 216
            Width = 120
            Height = 16
            Caption = 'Centor Position Y'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edRhombusLeft: TEdit
            Left = 152
            Top = 20
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 0
            Text = '0'
          end
          object edRhombusTop: TEdit
            Left = 152
            Top = 48
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
            Text = '0'
          end
          object edRhombusWidth: TEdit
            Left = 152
            Top = 76
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 2
            Text = '0'
          end
          object edRhombusHeight: TEdit
            Left = 152
            Top = 104
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 3
            Text = '0'
          end
          object edRhombusRight: TEdit
            Left = 152
            Top = 132
            Width = 121
            Height = 21
            Color = clScrollBar
            Enabled = False
            ImeName = 'Microsoft IME 2010'
            TabOrder = 4
            Text = '0'
          end
          object edRhombusBottom: TEdit
            Left = 152
            Top = 160
            Width = 121
            Height = 21
            Color = clScrollBar
            Enabled = False
            ImeName = 'Microsoft IME 2010'
            TabOrder = 5
            Text = '0'
          end
          object edRhombusCntrPosX: TEdit
            Left = 152
            Top = 188
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 6
            Text = '0'
          end
          object edRhombusCntrPosY: TEdit
            Left = 152
            Top = 216
            Width = 121
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 7
            Text = '0'
          end
        end
      end
      object btSave: TBitBtn
        Left = 314
        Top = 648
        Width = 137
        Height = 31
        Caption = 'SAVE'
        TabOrder = 2
        OnClick = btSaveClick
      end
      object btMOVE: TButton
        Left = 231
        Top = 650
        Width = 57
        Height = 29
        Caption = 'MOVE'
        TabOrder = 3
        OnClick = btMOVEClick
      end
      object cbVisible: TCheckBox
        Left = 17
        Top = 44
        Width = 97
        Height = 17
        Caption = 'VISIBLE'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        OnClick = cbVisibleClick
      end
      object btColor: TButton
        Left = 16
        Top = 64
        Width = 75
        Height = 25
        Caption = 'COLOR'
        TabOrder = 5
        OnClick = btColorClick
      end
      object pnColor: TPanel
        Left = 96
        Top = 64
        Width = 153
        Height = 25
        Caption = 'pnColor'
        TabOrder = 6
      end
      object cbToolID: TComboBox
        Left = 16
        Top = 652
        Width = 209
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = 'Microsoft Office IME 2007'
        ItemHeight = 20
        ParentFont = False
        TabOrder = 7
        Text = 'RightTool Vision'
        OnChange = cbToolIDChange
        Items.Strings = (
          'None'
          'LeftTool Vision'
          'LeftTool Height')
      end
      object btHxZGet: TButton
        Left = 528
        Top = 64
        Width = 185
        Height = 41
        Caption = 'HEXAPOT Z GET'
        TabOrder = 8
      end
      object btHxZWait: TButton
        Left = 528
        Top = 16
        Width = 185
        Height = 41
        Caption = 'HEXAPOT Z WAIT'
        TabOrder = 9
      end
      object edShiftX: TEdit
        Left = 32
        Top = 576
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 10
        Text = '0.0'
      end
      object edShiftY: TEdit
        Left = 32
        Top = 600
        Width = 121
        Height = 21
        ImeName = 'Microsoft Office IME 2007'
        TabOrder = 11
        Text = '0.0'
      end
      object Button1: TButton
        Left = 160
        Top = 576
        Width = 121
        Height = 25
        Caption = 'ShiftAll'
        TabOrder = 12
        OnClick = Button1Click
      end
      object btOpenAs: TButton
        Left = 320
        Top = 8
        Width = 57
        Height = 25
        Caption = 'Open As'
        TabOrder = 13
        OnClick = btOpenAsClick
      end
      object btSaveAs: TButton
        Left = 392
        Top = 8
        Width = 57
        Height = 25
        Caption = 'Save As'
        TabOrder = 14
        OnClick = btSaveAsClick
      end
      object btSaveAll: TBitBtn
        Left = 314
        Top = 576
        Width = 137
        Height = 31
        Caption = 'SAVE ALL'
        TabOrder = 15
        OnClick = btSaveAllClick
      end
    end
    object pnDraw: TPanel
      Left = 0
      Top = 0
      Width = 800
      Height = 800
      BevelOuter = bvNone
      Color = clBlack
      TabOrder = 1
      OnMouseDown = pnDrawMouseDown
      OnMouseMove = pnDrawMouseMove
      OnMouseUp = pnDrawMouseUp
    end
  end
  object tmUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmUpdateTimer
    Left = 8
    Top = 32
  end
  object cdColor: TColorDialog
    Ctl3D = True
    Left = 1056
    Top = 88
  end
  object dgLoadFilePath: TOpenDialog
    Filter = 'Hanra Pattern File|*.pat|All|*.*'
    Left = 1136
    Top = 64
  end
  object dgSaveFilePath: TSaveDialog
    Filter = 'Hanra Pattern File|*.pat|All File|*.*'
    Left = 1208
    Top = 64
  end
end
