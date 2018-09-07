object RF2Dialog: TRF2Dialog
  Left = 0
  Top = 0
  Caption = 'RF 2 Dialog'
  ClientHeight = 286
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object RFGroupBox: TGroupBox
    Left = 0
    Top = 0
    Width = 156
    Height = 130
    Caption = 'Current settings'
    TabOrder = 0
    object Label1: TLabel
      Left = 26
      Top = 26
      Width = 13
      Height = 13
      Caption = 'RF'
    end
    object Label2: TLabel
      Left = 124
      Top = 26
      Width = 20
      Height = 13
      Caption = 'MHz'
    end
    object Label6: TLabel
      Left = 124
      Top = 52
      Width = 20
      Height = 13
      Caption = 'dBm'
    end
    object Label7: TLabel
      Left = 26
      Top = 52
      Width = 30
      Height = 13
      Caption = 'Power'
    end
    object Label8: TLabel
      Left = 13
      Top = 78
      Width = 39
      Height = 13
      Caption = 'Mod osc'
    end
    object Label9: TLabel
      Left = 124
      Top = 78
      Width = 17
      Height = 13
      Caption = 'kHz'
    end
    object FreqEdit: TEdit
      Left = 46
      Top = 20
      Width = 72
      Height = 21
      Hint = 'Main RF frequency :  Valid range from 0.005 to 2080 MHz'
      MaxLength = 12
      TabOrder = 0
      Text = 'FreqEdit'
    end
    object CheckBox1: TCheckBox
      Left = 247
      Top = 26
      Width = 79
      Height = 14
      Caption = 'Power On'
      TabOrder = 1
    end
    object PowerCheckBox: TCheckBox
      Left = 65
      Top = 104
      Width = 79
      Height = 14
      Caption = 'Power ON'
      TabOrder = 2
    end
    object PowerEdit: TEdit
      Left = 72
      Top = 46
      Width = 46
      Height = 21
      Hint = 'Output power : Valid range -140 to +19 dBm'
      TabOrder = 3
      Text = '0'
    end
    object ModFreqEdit: TEdit
      Left = 65
      Top = 72
      Width = 53
      Height = 21
      Hint = 
        'Auxiliary frequency (internal modulation) : Valid range 0.001 to' +
        ' 500 kHz'
      TabOrder = 4
      Text = 'ModFreqEdit'
    end
  end
  object CalcelButton: TButton
    Left = 351
    Top = 188
    Width = 60
    Height = 20
    Hint = 'Cancel operation'
    Caption = 'Cancel'
    TabOrder = 1
  end
  object SetButton: TButton
    Left = 351
    Top = 6
    Width = 60
    Height = 20
    Hint = 'Set the RF source with these data. Does not quit window.'
    Caption = 'Do it'
    TabOrder = 2
    OnClick = SetButtonClick
  end
  object OKButton: TButton
    Left = 351
    Top = 214
    Width = 60
    Height = 59
    Hint = 'Use these values and close window'
    Caption = 'OK'
    TabOrder = 3
    OnClick = OKButtonClick
  end
  object ModulationGroupBox: TGroupBox
    Left = 0
    Top = 130
    Width = 345
    Height = 117
    Caption = 'Modulation'
    Enabled = False
    TabOrder = 4
    Visible = False
    object Label10: TLabel
      Left = 26
      Top = 33
      Width = 15
      Height = 13
      Caption = 'AM'
    end
    object Label15: TLabel
      Left = 104
      Top = 33
      Width = 11
      Height = 13
      Caption = '%'
    end
    object FMUnitLabel: TLabel
      Left = 104
      Top = 85
      Width = 17
      Height = 13
      Caption = 'kHz'
    end
    object AMEdit: TEdit
      Left = 59
      Top = 26
      Width = 39
      Height = 21
      TabOrder = 0
      Text = 'AMEdit'
    end
    object FMEdit: TEdit
      Left = 59
      Top = 78
      Width = 39
      Height = 21
      TabOrder = 1
      Text = 'FMEdit'
    end
    object AMIntPanel: TPanel
      Left = 130
      Top = 13
      Width = 209
      Height = 46
      TabOrder = 2
      object AMIntRadioButton: TRadioButton
        Left = 7
        Top = 26
        Width = 39
        Height = 14
        Caption = 'Int'
        TabOrder = 0
      end
      object AMOffRadioButton: TRadioButton
        Left = 7
        Top = 7
        Width = 40
        Height = 13
        Caption = 'OFF'
        TabOrder = 1
      end
      object AMExtAcRadioButton: TRadioButton
        Left = 65
        Top = 7
        Width = 49
        Height = 13
        Caption = 'Ext AC'
        TabOrder = 2
      end
      object AMExtDcRadioButton: TRadioButton
        Left = 65
        Top = 26
        Width = 49
        Height = 14
        Caption = 'Ext DC'
        TabOrder = 3
      end
      object AMDualACRadioButton: TRadioButton
        Left = 124
        Top = 7
        Width = 81
        Height = 13
        Caption = 'Int + Ext AC'
        TabOrder = 4
      end
      object AMDualDCRadioButton: TRadioButton
        Left = 124
        Top = 26
        Width = 81
        Height = 14
        Caption = 'Int + Ext DC'
        TabOrder = 5
      end
    end
    object AMExtPanel: TPanel
      Left = 130
      Top = 65
      Width = 209
      Height = 46
      TabOrder = 3
      object FMOFFRadioButton: TRadioButton
        Left = 7
        Top = 7
        Width = 48
        Height = 13
        Caption = 'OFF'
        TabOrder = 0
      end
      object FMIntRadioButton: TRadioButton
        Left = 7
        Top = 26
        Width = 48
        Height = 14
        Caption = 'Int'
        TabOrder = 1
      end
      object FMExtACRadioButton: TRadioButton
        Left = 65
        Top = 7
        Width = 49
        Height = 13
        Caption = 'Ext AC'
        TabOrder = 2
      end
      object FMExtDCRadioButton: TRadioButton
        Left = 65
        Top = 26
        Width = 49
        Height = 14
        Caption = 'Ext DC'
        TabOrder = 3
      end
      object FMDualACRadioButton: TRadioButton
        Left = 124
        Top = 7
        Width = 81
        Height = 13
        Caption = 'Int + Ext AC'
        TabOrder = 4
      end
      object FMDualDCRadioButton: TRadioButton
        Left = 124
        Top = 26
        Width = 81
        Height = 14
        Caption = 'Int + Ext DC'
        TabOrder = 5
      end
    end
    object FMRadioButton: TRadioButton
      Left = 7
      Top = 72
      Width = 32
      Height = 13
      Caption = 'FM'
      Checked = True
      TabOrder = 4
      TabStop = True
    end
    object PMRadioButton: TRadioButton
      Left = 7
      Top = 91
      Width = 46
      Height = 14
      Caption = 'Phase'
      TabOrder = 5
    end
  end
  object PowerGroupBox: TGroupBox
    Left = 162
    Top = 0
    Width = 183
    Height = 130
    Caption = 'Sweep'
    TabOrder = 5
    object Label3: TLabel
      Left = 72
      Top = 26
      Width = 10
      Height = 13
      Caption = 'to'
    end
    object Label4: TLabel
      Left = 156
      Top = 26
      Width = 20
      Height = 13
      Caption = 'MHz'
    end
    object label: TLabel
      Left = 52
      Top = 52
      Width = 43
      Height = 13
      Caption = 'Steps of '
    end
    object Label5: TLabel
      Left = 156
      Top = 52
      Width = 17
      Height = 13
      Caption = 'kHz'
    end
    object Label13: TLabel
      Left = 156
      Top = 78
      Width = 13
      Height = 13
      Caption = 'ms'
    end
    object Label14: TLabel
      Left = 78
      Top = 78
      Width = 25
      Height = 13
      Caption = 'Dwell'
    end
    object SweepLabel: TLabel
      Left = 20
      Top = 104
      Width = 57
      Height = 13
      Caption = 'SweepLabel'
    end
    object StartFreqEdit: TEdit
      Left = 7
      Top = 20
      Width = 59
      Height = 21
      TabOrder = 0
      Text = 'StartFreqEdit'
    end
    object StopFreqEdit: TEdit
      Left = 85
      Top = 20
      Width = 65
      Height = 21
      TabOrder = 1
      Text = 'StopFreqEdit'
    end
    object NStepsEdit: TEdit
      Left = 7
      Top = 46
      Width = 39
      Height = 21
      TabOrder = 2
      Text = 'NStepsEdit'
    end
    object StepEdit: TEdit
      Left = 98
      Top = 46
      Width = 52
      Height = 21
      TabOrder = 3
      Text = 'StepEdit'
    end
    object DwellEdit: TEdit
      Left = 111
      Top = 72
      Width = 39
      Height = 21
      TabOrder = 4
      Text = 'DwellEdit'
    end
  end
  object ErrorButton: TButton
    Left = 0
    Top = 253
    Width = 60
    Height = 20
    Caption = 'Error?'
    TabOrder = 6
  end
  object ErrorEdit: TEdit
    Left = 65
    Top = 253
    Width = 280
    Height = 21
    TabOrder = 7
    Text = 'No error'
  end
  object StopSweepButton: TButton
    Left = 351
    Top = 110
    Width = 26
    Height = 20
    Caption = 'Stop'
    TabOrder = 8
  end
  object SweepResetButton: TButton
    Left = 377
    Top = 110
    Width = 34
    Height = 20
    Caption = 'Reset'
    TabOrder = 9
  end
  object SweepStartButton: TButton
    Left = 351
    Top = 84
    Width = 60
    Height = 20
    Caption = 'Sweep (int)'
    TabOrder = 10
  end
end
