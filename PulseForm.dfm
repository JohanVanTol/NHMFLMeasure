object PulsedDialog: TPulsedDialog
  Left = 263
  Top = 157
  AutoSize = True
  Caption = 'Pulsed Parameters'
  ClientHeight = 430
  ClientWidth = 489
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label10: TLabel
    Left = 8
    Top = 410
    Width = 57
    Height = 13
    Caption = 'SweepType'
  end
  object OKBitButton: TBitBtn
    Left = 410
    Top = 405
    Width = 75
    Height = 25
    TabOrder = 0
    OnClick = OKBitButtonClick
    Kind = bkOK
  end
  object CancelBitButton: TBitBtn
    Left = 329
    Top = 405
    Width = 75
    Height = 25
    TabOrder = 1
    OnClick = CancelBitButtonClick
    Kind = bkCancel
  end
  object PulseGroupBox: TGroupBox
    Left = 0
    Top = 0
    Width = 489
    Height = 273
    Caption = 'Pulses'
    TabOrder = 2
    object Label1: TLabel
      Left = 16
      Top = 48
      Width = 13
      Height = 20
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 74
      Width = 13
      Height = 20
      Caption = 'B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 17
      Top = 100
      Width = 13
      Height = 20
      Caption = 'C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 17
      Top = 127
      Width = 14
      Height = 20
      Caption = 'D'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label12: TLabel
      Left = 3
      Top = 161
      Width = 62
      Height = 16
      Caption = 'E -switch'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label13: TLabel
      Left = 3
      Top = 187
      Width = 47
      Height = 16
      Caption = 'F -Trig'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label14: TLabel
      Left = 3
      Top = 213
      Width = 49
      Height = 16
      Caption = 'G -RF1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label15: TLabel
      Left = 3
      Top = 238
      Width = 49
      Height = 16
      Caption = 'H -Prot'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object StaticText1: TStaticText
      Left = 64
      Top = 24
      Width = 51
      Height = 17
      Caption = 'Delay (ns)'
      TabOrder = 0
    end
    object P3DelayEdit: TMaskEdit
      Left = 64
      Top = 102
      Width = 78
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 1
      Text = '        2000'
    end
    object P4DelayEdit: TMaskEdit
      Left = 64
      Top = 129
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 2
      Text = '        3000'
    end
    object P2DelayEdit: TMaskEdit
      Left = 64
      Top = 75
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 3
      Text = '        1000'
    end
    object P1DelayEdit: TMaskEdit
      Left = 64
      Top = 48
      Width = 80
      Height = 21
      Hint = 'Pulse_A_Delay'
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 4
      Text = '           0'
    end
    object StaticText2: TStaticText
      Left = 256
      Top = 24
      Width = 52
      Height = 17
      Caption = 'Width (ns)'
      TabOrder = 5
    end
    object P1WidthEdit: TMaskEdit
      Left = 256
      Top = 48
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 6
      Text = '         100'
    end
    object P2WidthEdit: TMaskEdit
      Left = 256
      Top = 75
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 7
      Text = '         100'
    end
    object P3WidthEdit: TMaskEdit
      Left = 256
      Top = 102
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 8
      Text = '         100'
    end
    object P4WidthEdit: TMaskEdit
      Left = 256
      Top = 129
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 9
      Text = '         100'
    end
    object P4DelayStepEdit: TMaskEdit
      Left = 160
      Top = 129
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 10
      Text = '           0'
    end
    object P3DelayStepEdit: TMaskEdit
      Left = 160
      Top = 102
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 11
      Text = '           0'
    end
    object P2DelayStepEdit: TMaskEdit
      Left = 160
      Top = 75
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 12
      Text = '           0'
    end
    object P1DelayStepEdit: TMaskEdit
      Left = 160
      Top = 48
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 13
      Text = '           0'
    end
    object P1WidthStepEdit: TMaskEdit
      Left = 354
      Top = 48
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 14
      Text = '           0'
    end
    object P2WidthStepEdit: TMaskEdit
      Left = 354
      Top = 75
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 15
      Text = '           0'
    end
    object P3WidthStepEdit: TMaskEdit
      Left = 354
      Top = 102
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 16
      Text = '           0'
    end
    object P4WidthStepEdit: TMaskEdit
      Left = 354
      Top = 129
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 17
      Text = '           0'
    end
    object StaticText3: TStaticText
      Left = 160
      Top = 24
      Width = 56
      Height = 17
      Caption = 'Delay Step'
      TabOrder = 18
    end
    object StaticText4: TStaticText
      Left = 352
      Top = 24
      Width = 57
      Height = 17
      Caption = 'Width Step'
      TabOrder = 19
    end
    object P1CheckBox: TCheckBox
      Left = 440
      Top = 48
      Width = 40
      Height = 17
      Caption = 'On'
      Checked = True
      State = cbChecked
      TabOrder = 20
    end
    object P2CheckBox: TCheckBox
      Left = 440
      Top = 75
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 21
    end
    object P3CheckBox: TCheckBox
      Left = 440
      Top = 102
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 22
    end
    object P4CheckBox: TCheckBox
      Left = 440
      Top = 129
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 23
    end
    object P5DelayEdit: TMaskEdit
      Left = 64
      Top = 156
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 24
      Text = '        3000'
    end
    object P6DelayEdit: TMaskEdit
      Left = 64
      Top = 183
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 25
      Text = '        3000'
    end
    object P7DelayEdit: TMaskEdit
      Left = 64
      Top = 210
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 26
      Text = '        3000'
    end
    object P8DelayEdit: TMaskEdit
      Left = 64
      Top = 237
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 27
      Text = '        3000'
    end
    object P5DelayStepEdit: TMaskEdit
      Left = 160
      Top = 156
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 28
      Text = '           0'
    end
    object P6DelayStepEdit: TMaskEdit
      Left = 160
      Top = 183
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 29
      Text = '           0'
    end
    object P7DelayStepEdit: TMaskEdit
      Left = 160
      Top = 210
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 30
      Text = '           0'
    end
    object P8DelayStepEdit: TMaskEdit
      Left = 160
      Top = 237
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 31
      Text = '           0'
    end
    object P5WidthEdit: TMaskEdit
      Left = 256
      Top = 156
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 32
      Text = '         100'
    end
    object P6WidthEdit: TMaskEdit
      Left = 256
      Top = 183
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 33
      Text = '         100'
    end
    object P7WidthEdit: TMaskEdit
      Left = 256
      Top = 210
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 34
      Text = '         100'
    end
    object P8WidthEdit: TMaskEdit
      Left = 256
      Top = 237
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 35
      Text = '         100'
    end
    object P5WidthStepEdit: TMaskEdit
      Left = 354
      Top = 156
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 36
      Text = '           0'
    end
    object P6WidthStepEdit: TMaskEdit
      Left = 354
      Top = 183
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 37
      Text = '           0'
    end
    object P7WidthStepEdit: TMaskEdit
      Left = 354
      Top = 210
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 38
      Text = '           0'
    end
    object P8WidthStepEdit: TMaskEdit
      Left = 354
      Top = 237
      Width = 80
      Height = 21
      EditMask = '!999999999999;1;_'
      MaxLength = 12
      TabOrder = 39
      Text = '           0'
    end
    object P5CheckBox: TCheckBox
      Left = 440
      Top = 156
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 40
    end
    object P6CheckBox: TCheckBox
      Left = 440
      Top = 183
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 41
    end
    object P7CheckBox: TCheckBox
      Left = 440
      Top = 210
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 42
    end
    object P8CheckBox: TCheckBox
      Left = 440
      Top = 237
      Width = 40
      Height = 17
      Caption = 'On'
      TabOrder = 43
    end
  end
  object AcqGroupBox: TGroupBox
    Left = 0
    Top = 279
    Width = 489
    Height = 120
    Caption = 'Acquisition'
    TabOrder = 3
    object Label2: TLabel
      Left = 16
      Top = 32
      Width = 75
      Height = 13
      Caption = 'Aquisition Delay'
    end
    object IntergrLabel: TLabel
      Left = 344
      Top = 32
      Width = 36
      Height = 13
      Caption = 'Integrat'
    end
    object Svcan: TLabel
      Left = 344
      Top = 51
      Width = 30
      Height = 13
      Caption = 'Scans'
    end
    object Label6: TLabel
      Left = 32
      Top = 80
      Width = 22
      Height = 13
      Caption = 'Npts'
    end
    object Label7: TLabel
      Left = 176
      Top = 32
      Width = 11
      Height = 13
      Caption = 'ns'
    end
    object Label8: TLabel
      Left = 323
      Top = 32
      Width = 11
      Height = 13
      Caption = 'ns'
    end
    object Label9: TLabel
      Left = 463
      Top = 32
      Width = 11
      Height = 13
      Caption = 'ns'
    end
    object Label11: TLabel
      Left = 176
      Top = 56
      Width = 11
      Height = 13
      Caption = 'us'
    end
    object AcqDelayEdit: TMaskEdit
      Left = 97
      Top = 24
      Width = 71
      Height = 21
      EditMask = '!9999999999;1;_'
      MaxLength = 10
      TabOrder = 0
      Text = '       100'
    end
    object AcqStepEdit: TMaskEdit
      Left = 246
      Top = 24
      Width = 71
      Height = 21
      EditMask = '!9999999999;1;_'
      MaxLength = 10
      TabOrder = 1
      Text = '         0'
    end
    object StaticText5: TStaticText
      Left = 214
      Top = 32
      Width = 26
      Height = 17
      Caption = 'Step'
      TabOrder = 2
    end
    object StaticText6: TStaticText
      Left = 16
      Top = 51
      Width = 27
      Height = 17
      Caption = 'Rate'
      TabOrder = 3
    end
    object RateEdit: TMaskEdit
      Left = 97
      Top = 51
      Width = 71
      Height = 21
      EditMask = '!9999999999;1;_'
      MaxLength = 10
      TabOrder = 4
      Text = '     10000'
    end
    object AverEdit: TMaskEdit
      Left = 287
      Top = 51
      Width = 32
      Height = 21
      EditMask = '!9999;1;_'
      MaxLength = 4
      TabOrder = 5
      Text = '  10'
    end
    object StaticText7: TStaticText
      Left = 216
      Top = 55
      Width = 49
      Height = 17
      Caption = 'Averages'
      TabOrder = 6
    end
    object ScanEdit: TMaskEdit
      Left = 387
      Top = 51
      Width = 70
      Height = 21
      EditMask = '!9999999999;1;_'
      MaxLength = 10
      TabOrder = 7
      Text = '         1'
    end
    object NptsEdit: TMaskEdit
      Left = 124
      Top = 78
      Width = 44
      Height = 21
      EditMask = '!99999;1;_'
      MaxLength = 5
      TabOrder = 8
      Text = ' 1024'
    end
    object QuadratureCheckBox: TCheckBox
      Left = 211
      Top = 78
      Width = 97
      Height = 17
      Hint = 'Reads function3 from infinium'
      Caption = 'Quadrature'
      Checked = True
      State = cbChecked
      TabOrder = 9
    end
    object TwoChannelCheckBox: TCheckBox
      Left = 299
      Top = 78
      Width = 175
      Height = 17
      Caption = '2 Channels (CH1,3 or Math1,2)'
      TabOrder = 10
    end
    object ChACheckBox: TCheckBox
      Left = 211
      Top = 98
      Width = 97
      Height = 17
      Caption = 'Ch A On/Off'
      TabOrder = 11
    end
  end
  object SetButton: TButton
    Left = 233
    Top = 405
    Width = 75
    Height = 25
    Caption = 'Set'
    TabOrder = 4
    OnClick = SetButtonClick
  end
  object IntTimeEdit: TMaskEdit
    Left = 386
    Top = 303
    Width = 71
    Height = 21
    EditMask = '!9999999999;1;_'
    MaxLength = 10
    TabOrder = 5
    Text = '       200'
  end
  object SweepTypeListBox: TComboBox
    Left = 71
    Top = 409
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 6
    Items.Strings = (
      'Time sweep'
      'Field sweep'
      'ENDOR')
  end
end
