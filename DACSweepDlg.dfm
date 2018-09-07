object DacSweepDialog: TDacSweepDialog
  Left = 0
  Top = 0
  AutoSize = True
  Caption = 'Coil sweep with DAC'
  ClientHeight = 145
  ClientWidth = 179
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 8
    Width = 25
    Height = 13
    Caption = ' Npts'
  end
  object Label2: TLabel
    Left = 0
    Top = 43
    Width = 34
    Height = 13
    Caption = ' Range'
  end
  object Label3: TLabel
    Left = 0
    Top = 101
    Width = 30
    Height = 13
    Caption = ' Delay'
  end
  object NptsComboBox: TComboBox
    Left = 64
    Top = 0
    Width = 81
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 0
    Text = '128'
    Items.Strings = (
      '128'
      '256'
      '512'
      '1024'
      '2048'
      '4096'
      '8192'
      '16384'
      '32768'
      '65536')
  end
  object RangeComboBox: TComboBox
    Left = 64
    Top = 40
    Width = 81
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    ItemIndex = 3
    TabOrder = 1
    Text = '1.25 V'
    Items.Strings = (
      '10 V'
      '5 V'
      '2.5  V'
      '1.25 V'
      '0.625 V'
      '0.3125 V'
      '0.15625 V')
  end
  object Button1: TButton
    Left = 104
    Top = 120
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 0
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = Button2Click
  end
  object DelayComboBox: TComboBox
    Left = 64
    Top = 93
    Width = 81
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    ItemIndex = 2
    TabOrder = 4
    Text = '100'
    Items.Strings = (
      '10'
      '30'
      '100'
      '300'
      '1000')
  end
  object ZeroCheckBox: TCheckBox
    Left = 32
    Top = 70
    Width = 97
    Height = 17
    Caption = 'Start at 0'
    TabOrder = 5
  end
end
