object OImonitorForm: TOImonitorForm
  Left = 0
  Top = 0
  Caption = 'Oxford Instruments Monitor'
  ClientHeight = 161
  ClientWidth = 962
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnDeactivate = FormDeactivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object FieldLabel: TLabel
    Left = 56
    Top = 48
    Width = 89
    Height = 45
    Caption = 'Field'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object TemperatureLabel: TLabel
    Left = 264
    Top = 48
    Width = 240
    Height = 45
    Caption = 'Temperature'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object HeLabel: TLabel
    Left = 528
    Top = 48
    Width = 148
    Height = 45
    Caption = 'He level'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object N2Label: TLabel
    Left = 752
    Top = 48
    Width = 161
    Height = 45
    Caption = 'N2 Level'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 56
    Top = 8
    Width = 135
    Height = 25
    Caption = 'Magnetic Field'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 264
    Top = 8
    Width = 121
    Height = 25
    Caption = 'Temperature'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 528
    Top = 8
    Width = 114
    Height = 25
    Caption = 'Helium level'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 752
    Top = 8
    Width = 134
    Height = 25
    Caption = 'Nitrogen Level'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object MonitorTimer: TTimer
    Interval = 2000
    OnTimer = MonitorTimerTimer
    Left = 64
    Top = 104
  end
end
