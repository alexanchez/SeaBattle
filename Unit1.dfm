object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'SeaBattle'
  ClientHeight = 314
  ClientWidth = 502
  Color = clWindow
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Player1Label: TLabel
    Left = 97
    Top = 56
    Width = 62
    Height = 19
    Alignment = taCenter
    AutoSize = False
    Caption = 'Player1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Player2Label: TLabel
    Left = 360
    Top = 56
    Width = 62
    Height = 19
    Alignment = taCenter
    AutoSize = False
    Caption = 'Player2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 120
    Top = 16
    Width = 313
    Height = 19
    Alignment = taCenter
    AutoSize = False
    Caption = 'Press "New game" to start'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object NewGameButton: TButton
    Left = 8
    Top = 8
    Width = 81
    Height = 25
    Caption = 'New game'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = NewGameButtonClick
  end
  object Button1: TButton
    Left = 467
    Top = 8
    Width = 30
    Height = 25
    TabOrder = 1
    Visible = False
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 439
    Top = 8
    Width = 22
    Height = 25
    TabOrder = 2
    Visible = False
    OnClick = Button2Click
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 700
    OnTimer = Timer1Timer
    Left = 464
    Top = 40
  end
end
