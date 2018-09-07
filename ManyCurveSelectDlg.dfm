object ManyCurveDlg: TManyCurveDlg
  Left = 192
  Top = 107
  Width = 201
  Height = 230
  AutoSize = True
  Caption = 'Choose Curves to Display'
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
    Left = 0
    Top = 0
    Width = 193
    Height = 153
    Caption = 'GroupBox1'
    TabOrder = 0
    object Label1: TLabel
      Left = 32
      Top = 40
      Width = 74
      Height = 13
      Caption = 'Start at column '
    end
    object Label2: TLabel
      Left = 80
      Top = 72
      Width = 21
      Height = 13
      Caption = 'Skip'
    end
    object Label3: TLabel
      Left = 72
      Top = 104
      Width = 24
      Height = 13
      Caption = 'Total'
    end
    object StartEdit: TEdit
      Left = 112
      Top = 32
      Width = 41
      Height = 21
      Enabled = False
      TabOrder = 0
      Text = '1'
    end
    object SkipEdit: TEdit
      Left = 112
      Top = 64
      Width = 41
      Height = 21
      Enabled = False
      TabOrder = 1
      Text = '0'
    end
    object TotalEdit: TEdit
      Left = 112
      Top = 96
      Width = 41
      Height = 21
      Enabled = False
      TabOrder = 2
      Text = '1'
    end
    object UpDown1: TUpDown
      Left = 153
      Top = 32
      Width = 15
      Height = 21
      Associate = StartEdit
      Min = 1
      Max = 200
      Position = 1
      TabOrder = 3
      Wrap = False
    end
    object UpDown2: TUpDown
      Left = 153
      Top = 64
      Width = 15
      Height = 21
      Associate = SkipEdit
      Min = 0
      Max = 200
      Position = 0
      TabOrder = 4
      Wrap = False
    end
    object UpDown3: TUpDown
      Left = 153
      Top = 96
      Width = 15
      Height = 21
      Associate = TotalEdit
      Min = 1
      Max = 200
      Position = 1
      TabOrder = 5
      Wrap = False
    end
  end
  object BitBtn1: TBitBtn
    Left = 64
    Top = 168
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkOK
  end
end
