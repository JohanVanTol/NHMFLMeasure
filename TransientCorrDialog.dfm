object TransientCorrDlg: TTransientCorrDlg
  Left = 192
  Top = 116
  Width = 251
  Height = 256
  AutoSize = True
  Caption = 'TransientCorrDlg'
  Color = clBtnFace
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
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 235
    Height = 93
    Caption = 'Substract Prepulse noise'
    TabOrder = 0
    object Label1: TLabel
      Left = 24
      Top = 48
      Width = 98
      Height = 13
      Caption = 'Columns to substract'
    end
    object Label3: TLabel
      Left = 56
      Top = 24
      Width = 93
      Height = 13
      Caption = 'Data Columns in file'
    end
    object NrColumnsLabel: TLabel
      Left = 24
      Top = 24
      Width = 6
      Height = 13
      Caption = '0'
    end
    object ColumnEdit: TEdit
      Left = 144
      Top = 40
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '1'
    end
    object UpDown1: TUpDown
      Left = 185
      Top = 40
      Width = 15
      Height = 21
      Associate = ColumnEdit
      Min = 1
      Position = 1
      TabOrder = 1
      Wrap = False
    end
    object SkipFirstCheckBox: TCheckBox
      Left = 24
      Top = 64
      Width = 185
      Height = 17
      Caption = 'Skip first (calibration) column'
      TabOrder = 2
    end
  end
  object BitBtn1: TBitBtn
    Left = 162
    Top = 204
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkClose
  end
  object BitBtn2: TBitBtn
    Left = 2
    Top = 204
    Width = 75
    Height = 25
    TabOrder = 2
    OnClick = BitBtn2Click
    Kind = bkCancel
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 92
    Width = 233
    Height = 65
    Caption = 'Rephase'
    TabOrder = 3
    object Label2: TLabel
      Left = 24
      Top = 40
      Width = 85
      Height = 13
      Caption = 'Microwave Phase'
    end
    object PhaseEdit: TEdit
      Left = 152
      Top = 32
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object UpDown2: TUpDown
      Left = 193
      Top = 32
      Width = 15
      Height = 21
      Associate = PhaseEdit
      Min = -180
      Max = 360
      Position = 0
      TabOrder = 1
      Wrap = False
    end
  end
  object ApplyButton: TBitBtn
    Left = 82
    Top = 204
    Width = 75
    Height = 25
    Caption = 'Apply'
    TabOrder = 4
    OnClick = ApplyButtonClick
    Kind = bkRetry
  end
  object OldTypeCheckBox: TCheckBox
    Left = 10
    Top = 156
    Width = 233
    Height = 17
    Caption = 'Old file type (altenating columns)'
    TabOrder = 5
  end
  object ErrorCheckBox: TCheckBox
    Left = 10
    Top = 176
    Width = 57
    Height = 25
    Caption = '5x error'
    TabOrder = 6
  end
  object EndSubstractCheckBox: TCheckBox
    Left = 96
    Top = 180
    Width = 129
    Height = 17
    Caption = 'Pre-pulse columns last'
    TabOrder = 7
  end
end
