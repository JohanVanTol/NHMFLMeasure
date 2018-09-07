object UserForm: TUserForm
  Left = 192
  Top = 107
  Caption = 'UserForm'
  ClientHeight = 261
  ClientWidth = 545
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 8
    Top = 8
    Width = 388
    Height = 40
    Caption = 
      'Note that for all experiments in the NHMFL user program, a propo' +
      'sal needs to be submitted'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    WordWrap = True
  end
  object UserInfoGroupBox: TGroupBox
    Left = 8
    Top = 69
    Width = 529
    Height = 153
    Caption = 'Please provide the following'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    object Label1: TLabel
      Left = 24
      Top = 32
      Width = 84
      Height = 16
      Caption = 'User Name(s)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 24
      Top = 96
      Width = 118
      Height = 16
      Caption = 'Experiment Number'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object UserNameEdit: TEdit
      Left = 48
      Top = 54
      Width = 457
      Height = 24
      Hint = 'Provide Names of users and/or PI'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      Text = 'UserNameEdit'
    end
    object ExperimentMaskEdit: TMaskEdit
      Left = 48
      Top = 118
      Width = 129
      Height = 24
      Hint = 
        'Provide the number of experiment. Goto users.magnet.fsu.edu and ' +
        'login to find it.'
      AutoSize = False
      EditMask = 'P00000\-E000\-EMR;1;_'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 15
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      Text = 'P     -E   -EMR'
    end
  end
  object OKButton: TBitBtn
    Left = 440
    Top = 228
    Width = 97
    Height = 25
    TabOrder = 0
    OnClick = OKButtonClick
    Kind = bkOK
  end
end
