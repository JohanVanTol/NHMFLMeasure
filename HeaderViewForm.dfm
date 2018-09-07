object ParamViewForm: TParamViewForm
  Left = 192
  Top = 116
  Width = 538
  Height = 353
  AutoSize = True
  Caption = 'File Parameters'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object HeaderMemo: TMemo
    Left = 0
    Top = 0
    Width = 530
    Height = 293
    Align = alTop
    Lines.Strings = (
      'HeaderMemo')
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object BitBtn1: TBitBtn
    Left = 430
    Top = 304
    Width = 91
    Height = 25
    TabOrder = 1
    OnClick = BitBtn1Click
    Kind = bkOK
  end
end
