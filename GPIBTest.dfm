�
 TGPIBTESTFORM 0�  TPF0TGPIBTestFormGPIBTestFormLeftTop� AutoSize	CaptionInteractive GPIB test programClientHeight�ClientWidthDColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style OldCreateOrder	
OnActivateFormActivateOnClose	FormCloseOnCreate
FormCreatePixelsPerInch`
TextHeight 	TGroupBoxGPIBGroupBoxLeft Top WidthHeight� CaptionGPIB statusTabOrder TLabel
LabelCountLefthTopbWidthHeightCaption0Color	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclGreenFont.Height�	Font.NameMS Sans Serif
Font.Style ParentColor
ParentFont  TLabelLabelStatusLeftLTopWidth/HeightCaption	undefinedFont.CharsetDEFAULT_CHARSET
Font.ColorclGreenFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  TLabelLabelStatusBitsLeft�TopWidth`HeightCaption0000000000000000Font.CharsetDEFAULT_CHARSET
Font.ColorclGreenFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  TLabel
LabelErrorLeftTop!Width&HeightCaptionNo errorFont.CharsetDEFAULT_CHARSET
Font.ColorclGreenFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFontWordWrap	  TLabelLabel2Left� TopWidthWHeightCaptionGPIB status (ibsta)  TLabelLabel3LeftTopbWidthNHeightCaptionGPIB byte count  TLabelLabel4LeftTopWidth1HeightCaption
GPIB error  TLabelLabel5LeftTop� WidthCHeightCaptionTimeout value  	TComboBoxTimeOutComboBoxLefthTop|WidthHHeightStylecsDropDownList
ItemHeightTabOrder OnChangeTimeOutComboBoxChangeItems.StringsNone0.01 ms0.03 ms0.1 ms0.3 ms1 ms3 ms10 ms30 ms100 ms300 ms1 s3 s10 s30 s100 s300 s1000 s    TMemo
StatusMemoLeft� Top!WidthHeightuLines.StringsCurrent status : 
ScrollBars
ssVerticalTabOrder   	TGroupBoxDeviceGroupBoxLeft Top� WidthHeight� CaptionDevice commandsTabOrder TLabelLabel1LeftTop'WidthJHeightCaptionDevice address  TLabelLabel6LeftTopAWidth<Height'CaptionEnd of transmission mode (Send)WordWrap	  TLabelLabel7LeftTophWidth6Height4CaptionData termination mode (Receive)WordWrap	  TLabelLabel8LeftTop� WidthKHeightCaptionNumber of bytes to receiveWordWrap	  	TComboBoxEotModeComboBoxLeftoTopHWidthUHeightStylecsDropDownList
ItemHeightTabOrder OnChangeEotModeComboBoxChangeItems.StringsNULLendNLendDABend   	TComboBoxEosModeComboBoxLeftoTopoWidthUHeightStylecsDropDownList
ItemHeightTabOrderOnChangeEosModeComboBoxChangeItems.StringsSTOPendEOS char (0)   TEditNBytesToReceiveEditLeftoTop� WidthHeightTabOrderText72OnExitNBytesToReceiveEditExit  TMemo
MemoOutputLeft� TopbWidthHeightN
ScrollBars
ssVerticalTabOrder  	TCheckBox
CRCheckBoxLeftyTop<Width|HeightCaptionAdd CR characterTabOrder  	TCheckBoxCR_LF_CheckBoxLeftyTopPWidthaHeightCaption	Add CR+LFTabOrder   TEditSendCommandLeft� Top� WidthHeightCtl3D	ParentCtl3DTabOrder TextEdit command to send here  TButton
ButtonSendLeft� Top� Width=HeightCaptionGo sendTabOrderOnClickButtonSendClick  TEditAddressEditLeftnTop� WidthHeightTabOrderText22  TButtonButtonReceiveLeft$Top� Width=HeightCaptionReceiveTabOrderOnClickButtonReceiveClick  	TGroupBoxDeviceStatusGroupBoxLeft TopeWidthHeightUCaptionDevice StatusTabOrder TLabelLabel9LeftTopWidth� HeightCaptionBefore last send  or receive  TLabelLabel10LeftTop'Width� HeightCaption     After last send or receive  TLabelDevStatusBeforeLabelLeft� TopWidth/HeightCaption	undefinedColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclGreenFont.Height�	Font.NameMS Sans Serif
Font.Style ParentColor
ParentFont  TLabelDevStatusAfterLabelLeft� Top'Width/HeightCaption	undefinedColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclGreenFont.Height�	Font.NameMS Sans Serif
Font.Style ParentColor
ParentFont   TButton
QuitButtonLeftTop�Width<HeightCaptionCloseTabOrderOnClickQuitButtonClick  TButtonListenersButtonLeftTopWidth<HeightHintChecks for listenersCaption
Who there?TabOrderOnClickListenersButtonClick  TButtonStatusButtonLeftTop'Width<HeightCaption
Bus statusTabOrder	OnClickStatusButtonClick   