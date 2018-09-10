object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Oxford Monitor'
  ClientHeight = 141
  ClientWidth = 1037
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object FieldLabel: TLabel
    Left = 24
    Top = 24
    Width = 165
    Height = 45
    Caption = '0.00000 T'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object TemperatureLabel: TLabel
    Left = 280
    Top = 24
    Width = 145
    Height = 45
    Caption = '300.00 K'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object HeLevelLabel: TLabel
    Left = 560
    Top = 24
    Width = 159
    Height = 45
    Caption = '000.00 %'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object N2LevelLabel: TLabel
    Left = 808
    Top = 24
    Width = 159
    Height = 45
    Caption = '000.00 %'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object CloseButton: TBitBtn
    Left = 892
    Top = 88
    Width = 75
    Height = 25
    TabOrder = 0
    Kind = bkClose
  end
end
