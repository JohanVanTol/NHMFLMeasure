object PlotCurvesDlg: TPlotCurvesDlg
  Left = 192
  Top = 116
  AutoSize = True
  Caption = 'PlotCurvesDlg'
  ClientHeight = 260
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 156
    Top = 7
    Width = 215
    Height = 143
    Caption = 'PLOT'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 7
      Top = 52
      Width = 18
      Height = 13
      Caption = 'Left'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 182
      Top = 52
      Width = 25
      Height = 13
      Caption = 'Right'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 91
      Top = 117
      Width = 28
      Height = 13
      Caption = 'X axis'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 150
    Height = 150
    Caption = 'Left Y axis'
    TabOrder = 1
    object YL1: TCheckBox
      Left = 7
      Top = 13
      Width = 65
      Height = 14
      Caption = '1'
      TabOrder = 0
    end
    object YL2: TCheckBox
      Left = 7
      Top = 26
      Width = 65
      Height = 14
      Caption = '2'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object YL3: TCheckBox
      Left = 7
      Top = 39
      Width = 65
      Height = 14
      Caption = '3'
      TabOrder = 2
    end
    object YL4: TCheckBox
      Left = 7
      Top = 52
      Width = 65
      Height = 14
      Caption = '4'
      TabOrder = 3
    end
    object YL5: TCheckBox
      Left = 7
      Top = 65
      Width = 65
      Height = 14
      Caption = '5'
      TabOrder = 4
    end
    object YL6: TCheckBox
      Left = 7
      Top = 78
      Width = 65
      Height = 14
      Caption = '6'
      TabOrder = 5
    end
    object YL7: TCheckBox
      Left = 7
      Top = 91
      Width = 65
      Height = 14
      Caption = '7'
      TabOrder = 6
    end
    object YL8: TCheckBox
      Left = 7
      Top = 104
      Width = 65
      Height = 14
      Caption = '8'
      TabOrder = 7
    end
    object YL9: TCheckBox
      Left = 7
      Top = 117
      Width = 65
      Height = 14
      Caption = '9'
      TabOrder = 8
    end
    object YL10: TCheckBox
      Left = 7
      Top = 130
      Width = 65
      Height = 14
      Caption = '10'
      TabOrder = 9
    end
    object YL11: TCheckBox
      Left = 78
      Top = 13
      Width = 66
      Height = 14
      Caption = '11'
      TabOrder = 10
    end
    object YL12: TCheckBox
      Left = 78
      Top = 26
      Width = 66
      Height = 14
      Caption = '12'
      TabOrder = 11
    end
    object YL13: TCheckBox
      Left = 78
      Top = 39
      Width = 66
      Height = 14
      Caption = '13'
      TabOrder = 12
    end
    object YL14: TCheckBox
      Left = 78
      Top = 52
      Width = 66
      Height = 14
      Caption = '14'
      TabOrder = 13
    end
    object YL15: TCheckBox
      Left = 78
      Top = 65
      Width = 66
      Height = 14
      Caption = '15'
      TabOrder = 14
    end
    object YL16: TCheckBox
      Left = 78
      Top = 78
      Width = 66
      Height = 14
      Caption = '16'
      TabOrder = 15
    end
    object YL17: TCheckBox
      Left = 78
      Top = 91
      Width = 66
      Height = 14
      Caption = '17'
      TabOrder = 16
    end
    object YL18: TCheckBox
      Left = 78
      Top = 104
      Width = 66
      Height = 14
      Caption = '18'
      TabOrder = 17
    end
    object YL19: TCheckBox
      Left = 78
      Top = 117
      Width = 66
      Height = 14
      Caption = '19'
      TabOrder = 18
    end
    object YL20: TCheckBox
      Left = 78
      Top = 130
      Width = 66
      Height = 14
      Caption = '20'
      TabOrder = 19
    end
  end
  object GroupBox2: TGroupBox
    Left = 377
    Top = 0
    Width = 150
    Height = 150
    Caption = 'Right Y axis'
    TabOrder = 2
    object YR1: TCheckBox
      Left = 7
      Top = 13
      Width = 65
      Height = 14
      Caption = '1'
      TabOrder = 0
    end
    object YR2: TCheckBox
      Left = 7
      Top = 26
      Width = 65
      Height = 14
      Caption = '2'
      TabOrder = 1
    end
    object YR3: TCheckBox
      Left = 7
      Top = 39
      Width = 65
      Height = 14
      Caption = '3'
      TabOrder = 2
    end
    object YR4: TCheckBox
      Left = 7
      Top = 52
      Width = 65
      Height = 14
      Caption = '4'
      TabOrder = 3
    end
    object YR5: TCheckBox
      Left = 7
      Top = 65
      Width = 65
      Height = 14
      Caption = '5'
      TabOrder = 4
    end
    object YR6: TCheckBox
      Left = 7
      Top = 78
      Width = 65
      Height = 14
      Caption = '6'
      TabOrder = 5
    end
    object YR7: TCheckBox
      Left = 7
      Top = 91
      Width = 65
      Height = 14
      Caption = '7'
      TabOrder = 6
    end
    object YR8: TCheckBox
      Left = 7
      Top = 104
      Width = 65
      Height = 14
      Caption = '8'
      TabOrder = 7
    end
    object YR9: TCheckBox
      Left = 7
      Top = 117
      Width = 65
      Height = 14
      Caption = '9'
      TabOrder = 8
    end
    object YR10: TCheckBox
      Left = 7
      Top = 130
      Width = 65
      Height = 14
      Caption = '10'
      TabOrder = 9
    end
    object YR11: TCheckBox
      Left = 78
      Top = 13
      Width = 66
      Height = 14
      Caption = '11'
      TabOrder = 10
    end
    object YR12: TCheckBox
      Left = 78
      Top = 26
      Width = 66
      Height = 14
      Caption = '12'
      TabOrder = 11
    end
    object YR13: TCheckBox
      Left = 78
      Top = 39
      Width = 66
      Height = 14
      Caption = '13'
      TabOrder = 12
    end
    object YR14: TCheckBox
      Left = 78
      Top = 52
      Width = 66
      Height = 14
      Caption = '14'
      TabOrder = 13
    end
    object YR15: TCheckBox
      Left = 78
      Top = 65
      Width = 66
      Height = 14
      Caption = '15'
      TabOrder = 14
    end
    object YR16: TCheckBox
      Left = 78
      Top = 78
      Width = 66
      Height = 14
      Caption = '16'
      TabOrder = 15
    end
    object YR17: TCheckBox
      Left = 78
      Top = 91
      Width = 66
      Height = 14
      Caption = '17'
      TabOrder = 16
    end
    object YR18: TCheckBox
      Left = 78
      Top = 104
      Width = 66
      Height = 14
      Caption = '18'
      TabOrder = 17
    end
    object YR19: TCheckBox
      Left = 78
      Top = 117
      Width = 66
      Height = 14
      Caption = '19'
      TabOrder = 18
    end
    object YR20: TCheckBox
      Left = 78
      Top = 130
      Width = 66
      Height = 14
      Caption = '20'
      TabOrder = 19
    end
  end
  object GroupBox3: TGroupBox
    Left = 137
    Top = 156
    Width = 254
    Height = 111
    Caption = 'X axis'
    TabOrder = 3
    object XB1: TRadioButton
      Left = 13
      Top = 13
      Width = 66
      Height = 14
      Caption = '1'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object XB2: TRadioButton
      Left = 13
      Top = 26
      Width = 66
      Height = 14
      Caption = '2'
      TabOrder = 1
    end
    object XB3: TRadioButton
      Left = 13
      Top = 39
      Width = 66
      Height = 14
      Caption = '3'
      TabOrder = 2
    end
    object XB4: TRadioButton
      Left = 13
      Top = 52
      Width = 66
      Height = 14
      Caption = '4'
      TabOrder = 3
    end
    object XB5: TRadioButton
      Left = 13
      Top = 65
      Width = 66
      Height = 14
      Caption = '5'
      TabOrder = 4
    end
    object XB6: TRadioButton
      Left = 13
      Top = 78
      Width = 66
      Height = 14
      Caption = '6'
      TabOrder = 5
    end
    object XB7: TRadioButton
      Left = 13
      Top = 91
      Width = 66
      Height = 14
      Caption = '7'
      TabOrder = 6
    end
    object XB8: TRadioButton
      Left = 98
      Top = 13
      Width = 65
      Height = 14
      Caption = '8'
      TabOrder = 7
    end
    object XB9: TRadioButton
      Left = 98
      Top = 26
      Width = 65
      Height = 14
      Caption = '9'
      TabOrder = 8
    end
    object XB10: TRadioButton
      Left = 98
      Top = 39
      Width = 65
      Height = 14
      Caption = '10'
      TabOrder = 9
    end
    object XB11: TRadioButton
      Left = 98
      Top = 52
      Width = 65
      Height = 14
      Caption = '11'
      TabOrder = 10
    end
    object XB12: TRadioButton
      Left = 98
      Top = 65
      Width = 65
      Height = 14
      Caption = '12'
      TabOrder = 11
    end
    object XB13: TRadioButton
      Left = 98
      Top = 78
      Width = 65
      Height = 14
      Caption = '13'
      TabOrder = 12
    end
    object XB14: TRadioButton
      Left = 98
      Top = 91
      Width = 65
      Height = 14
      Caption = '14'
      TabOrder = 13
    end
    object XB15: TRadioButton
      Left = 182
      Top = 13
      Width = 66
      Height = 14
      Caption = '15'
      TabOrder = 14
    end
    object XB16: TRadioButton
      Left = 182
      Top = 26
      Width = 66
      Height = 14
      Caption = '16'
      TabOrder = 15
    end
    object XB17: TRadioButton
      Left = 182
      Top = 39
      Width = 66
      Height = 14
      Caption = '17'
      TabOrder = 16
    end
    object XB18: TRadioButton
      Left = 182
      Top = 52
      Width = 66
      Height = 14
      Caption = '18'
      TabOrder = 17
    end
    object XB19: TRadioButton
      Left = 182
      Top = 65
      Width = 66
      Height = 14
      Caption = '19'
      TabOrder = 18
    end
    object XB20: TRadioButton
      Left = 182
      Top = 78
      Width = 66
      Height = 14
      Caption = '20'
      TabOrder = 19
    end
    object XBPoint: TRadioButton
      Left = 182
      Top = 91
      Width = 66
      Height = 14
      Caption = 'Point nr'
      TabOrder = 20
    end
  end
  object OKButton: TBitBtn
    Left = 429
    Top = 241
    Width = 98
    Height = 26
    TabOrder = 4
    OnClick = OKButtonClick
    Kind = bkOK
  end
  object CancelButton: TBitBtn
    Left = 429
    Top = 208
    Width = 98
    Height = 27
    TabOrder = 5
    OnClick = CancelButtonClick
    Kind = bkCancel
  end
end
