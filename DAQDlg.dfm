object DAQ_Dialog: TDAQ_Dialog
  Left = 266
  Top = 140
  AutoSize = True
  Caption = 'DAQ Dialog'
  ClientHeight = 249
  ClientWidth = 441
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 360
    Top = 224
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
  object ADCGroupBox: TGroupBox
    Left = 0
    Top = 0
    Width = 273
    Height = 249
    Caption = 'ADC Acquisition channels '
    TabOrder = 1
    object LabelCh0: TLabel
      Left = 136
      Top = 24
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh1: TLabel
      Left = 136
      Top = 48
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh2: TLabel
      Left = 136
      Top = 72
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh3: TLabel
      Left = 136
      Top = 96
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh4: TLabel
      Left = 136
      Top = 120
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh5: TLabel
      Left = 136
      Top = 144
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh6: TLabel
      Left = 136
      Top = 168
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelCh7: TLabel
      Left = 136
      Top = 192
      Width = 73
      Height = 16
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0.0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object CheckBoxCh0: TCheckBox
      Left = 8
      Top = 24
      Width = 57
      Height = 17
      Caption = 'Ch 0'
      TabOrder = 0
    end
    object CheckBoxCh1: TCheckBox
      Left = 8
      Top = 48
      Width = 57
      Height = 17
      Caption = 'Ch 1'
      TabOrder = 1
    end
    object CheckBoxCh2: TCheckBox
      Left = 8
      Top = 72
      Width = 57
      Height = 17
      Caption = 'Ch 2'
      TabOrder = 2
    end
    object CheckBoxCh3: TCheckBox
      Left = 8
      Top = 96
      Width = 57
      Height = 17
      Caption = 'Ch 3'
      TabOrder = 3
    end
    object CheckBoxCh4: TCheckBox
      Left = 8
      Top = 120
      Width = 57
      Height = 17
      Caption = 'Ch 4'
      TabOrder = 4
    end
    object CheckBoxCh5: TCheckBox
      Left = 8
      Top = 144
      Width = 57
      Height = 17
      Caption = 'Ch 5'
      TabOrder = 5
    end
    object CheckBoxCh6: TCheckBox
      Left = 8
      Top = 168
      Width = 57
      Height = 17
      Caption = 'Ch 6'
      TabOrder = 6
    end
    object CheckBoxCh7: TCheckBox
      Left = 8
      Top = 192
      Width = 49
      Height = 17
      Caption = 'Ch 7'
      TabOrder = 7
    end
    object GainComboBoxCh0: TComboBox
      Left = 56
      Top = 24
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 8
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh1: TComboBox
      Left = 56
      Top = 48
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 9
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh2: TComboBox
      Left = 56
      Top = 72
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 10
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh3: TComboBox
      Left = 56
      Top = 96
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 11
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh4: TComboBox
      Left = 56
      Top = 120
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 12
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh5: TComboBox
      Left = 56
      Top = 144
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 13
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh6: TComboBox
      Left = 56
      Top = 168
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 14
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object GainComboBoxCh7: TComboBox
      Left = 56
      Top = 192
      Width = 65
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 15
      Text = '10 V'
      Items.Strings = (
        '10 V'
        '5 V'
        '2.5 V'
        '1.25 V'
        '625 mV'
        '312 mV'
        '156 mV')
    end
    object UpdateADCButton: TButton
      Left = 8
      Top = 216
      Width = 75
      Height = 25
      Caption = 'Update'
      TabOrder = 16
      OnClick = UpdateADCButtonClick
    end
  end
  object IOGroupBox: TGroupBox
    Left = 280
    Top = 0
    Width = 161
    Height = 129
    Caption = 'I/O channels'
    TabOrder = 2
    object Label9: TLabel
      Left = 24
      Top = 24
      Width = 31
      Height = 13
      Caption = 'Byte A'
    end
    object Label10: TLabel
      Left = 24
      Top = 48
      Width = 31
      Height = 13
      Caption = 'Byte B'
    end
    object Label11: TLabel
      Left = 24
      Top = 72
      Width = 31
      Height = 13
      Caption = 'Byte C'
    end
    object IO_ExecuteButton: TButton
      Left = 72
      Top = 96
      Width = 75
      Height = 25
      Caption = 'Execute'
      TabOrder = 0
      OnClick = IO_ExecuteButtonClick
    end
    object IOComboBoxChA1: TComboBox
      Left = 80
      Top = 16
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 1
      Text = '0'
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        'A'
        'B'
        'C'
        'D'
        'E'
        'F')
    end
    object IOComboBoxChA2: TComboBox
      Left = 112
      Top = 16
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 2
      Text = '0'
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        'A'
        'B'
        'C'
        'D'
        'E'
        'F')
    end
    object IOComboBoxChB1: TComboBox
      Left = 80
      Top = 40
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 3
      Text = '0'
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        'A'
        'B'
        'C'
        'D'
        'E'
        'F')
    end
    object IOComboBoxChB2: TComboBox
      Left = 112
      Top = 40
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 4
      Text = '0'
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        'A'
        'B'
        'C'
        'D'
        'E'
        'F')
    end
    object IOComboBoxChC2: TComboBox
      Left = 112
      Top = 64
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 5
      Text = '0'
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        'A'
        'B'
        'C'
        'D'
        'E'
        'F')
    end
    object IOComboBoxChC1: TComboBox
      Left = 80
      Top = 64
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 6
      Text = '0'
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        'A'
        'B'
        'C'
        'D'
        'E'
        'F')
    end
  end
end
