object FrmHisto: TFrmHisto
  Left = 549
  Top = 381
  BorderStyle = bsToolWindow
  Caption = 'Histogram'
  ClientHeight = 353
  ClientWidth = 341
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object PView: TPanel
    Left = 8
    Top = 8
    Width = 321
    Height = 305
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 0
    OnMouseDown = PViewMouseDown
    OnMouseMove = PViewMouseMove
    OnMouseUp = PViewMouseUp
  end
  object tmUpdate: TTimer
    Interval = 10
    OnTimer = tmUpdateTimer
    Left = 304
    Top = 8
  end
end
