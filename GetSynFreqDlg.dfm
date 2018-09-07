object GetSynFreqDialog: TGetSynFreqDialog
  Left = 192
  Top = 104
  Caption = 'Synthesizer frequency'
  ClientHeight = 92
  ClientWidth = 299
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 0
    Width = 273
    Height = 57
    Caption = 'Set Frequency  (5500 - 6000 MHz)'
    TabOrder = 0
    object Label1: TLabel
      Left = 216
      Top = 32
      Width = 22
      Height = 13
      Caption = 'MHz'
    end
    object FreqEdit: TEdit
      Left = 88
      Top = 24
      Width = 89
      Height = 21
      TabOrder = 0
      Text = '5600'
    end
    object UpDown1: TUpDown
      Left = 177
      Top = 24
      Width = 13
      Height = 21
      Associate = FreqEdit
      Min = 5200
      Max = 6200
      Position = 5600
      TabOrder = 1
      Thousands = False
    end
  end
  object CancelButton: TBitBtn
    Left = 48
    Top = 72
    Width = 75
    Height = 25
    TabOrder = 1
    OnClick = CancelButtonClick
    Kind = bkCancel
  end
  object OkButton: TBitBtn
    Left = 176
    Top = 72
    Width = 75
    Height = 25
    TabOrder = 2
    OnClick = OkButtonClick
    Kind = bkOK
  end
end
