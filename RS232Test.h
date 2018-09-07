//---------------------------------------------------------------------------
#ifndef RS232TestH
#define RS232TestH
#ifndef WIN32
#define WIN32
#endif

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
class TRS232TestForm : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *Label2;
    TComboBox *PortComboBox;
    TComboBox *BaudComboBox;
    TLabel *Label3;
    TComboBox *WordlengthComboBox;
    TLabel *Label4;
    TComboBox *ParityComboBox;
    TLabel *Label5;
    TComboBox *StopBitsComboBox;
    TGroupBox *SendGroup;
    TButton *SendButton;
    TEdit *SendEdit;
    TEdit *ErrorEdit;
    TButton *Button1;
    TLabel *Label6;
    TEdit *ResponseEdit;
    TEdit *BufferEdit;
    TLabel *Label7;
    TButton *CloseButton;
	TLabel *Label8;
	TComboBox *TermComboBox;
    void __fastcall CloseButtonClick(TObject *Sender);
    
    void __fastcall SendButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TRS232TestForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRS232TestForm *RS232TestForm;
//---------------------------------------------------------------------------
#endif
