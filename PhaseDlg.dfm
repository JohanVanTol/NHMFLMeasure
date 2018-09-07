object RePhaseDialog: TRePhaseDialog
  Left = 192
  Top = 116
  AutoSize = True
  Caption = 'RePhaseDialog'
  ClientHeight = 140
  ClientWidth = 176
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
    Width = 176
    Height = 92
    Caption = 'Set Phase angles'
    TabOrder = 0
    object Label1: TLabel
      Left = 7
      Top = 20
      Width = 95
      Height = 13
      AutoSize = False
      Caption = 'Modulation Phase 1'
    end
    object Label2: TLabel
      Left = 7
      Top = 72
      Width = 85
      Height = 13
      Caption = 'Microwave Phase'
    end
    object Label3: TLabel
      Left = 7
      Top = 46
      Width = 94
      Height = 13
      Caption = 'Modulation Phase 2'
    end
    object ModPhase1Edit: TEdit
      Left = 124
      Top = 13
      Width = 39
      Height = 21
      TabOrder = 0
      Text = '0'
      OnChange = ModPhase1EditChange
    end
    object WavePhaseEdit: TEdit
      Left = 124
      Top = 65
      Width = 39
      Height = 21
      TabOrder = 1
      Text = '0'
      OnChange = WavePhaseEditChange
    end
    object UpDown1: TUpDown
      Left = 163
      Top = 13
      Width = 10
      Height = 21
      Associate = ModPhase1Edit
      Min = -180
      Max = 360
      TabOrder = 2
    end
    object UpDown2: TUpDown
      Left = 163
      Top = 65
      Width = 10
      Height = 21
      Associate = WavePhaseEdit
      Min = -180
      Max = 360
      TabOrder = 3
    end
    object ModPhase2Edit: TEdit
      Left = 124
      Top = 39
      Width = 39
      Height = 21
      TabOrder = 4
      Text = '0'
      OnChange = ModPhase2EditChange
    end
    object UpDown3: TUpDown
      Left = 163
      Top = 39
      Width = 10
      Height = 21
      Associate = ModPhase2Edit
      Min = -180
      Max = 360
      TabOrder = 5
    end
  end
  object CancelButton: TBitBtn
    Left = 0
    Top = 114
    Width = 85
    Height = 26
    TabOrder = 1
    OnClick = CancelButtonClick
    Kind = bkCancel
  end
  object OkButton: TBitBtn
    Left = 91
    Top = 114
    Width = 85
    Height = 26
    TabOrder = 2
    OnClick = OkButtonClick
    Kind = bkOK
  end
  object AutoPhaseCheckBox: TCheckBox
    Left = 8
    Top = 91
    Width = 168
    Height = 17
    Caption = 'Re-phase Lock-ins on OK'
    TabOrder = 3
  end
end
