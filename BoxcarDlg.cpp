//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BoxcarDlg.h"
#include "IEEEdev.h"
#include "Valid.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBoxcarDialog *BoxcarDialog;
extern SRS245 *Boxcar;
//---------------------------------------------------------------------------
__fastcall TBoxcarDialog::TBoxcarDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBoxcarDialog::Edit1Exit(TObject *Sender)
{
    // Check whether we have a real
    double Set = 0.0;
    if (ValidReal(Edit1->Text, &Set))
           Boxcar->Set(1,Set);
}
//---------------------------------------------------------------------------

int TBoxcarDialog::GetAllData()
{
    double value;
    int Nout = Boxcar->GetNout();
    if (Nout < 8)
    {
        value = Boxcar->Get(1);
        Edit1->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 7)
    {
        value = Boxcar->Get(2);
        Edit2->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 6)
    {
        value = Boxcar->Get(3);
        Edit3->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 5)
    {
        value = Boxcar->Get(4);
        Edit4->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 4)
    {
        value = Boxcar->Get(5);
        Edit5->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 3)
    {
        value = Boxcar->Get(6);
        Edit6->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 2)
    {
        value = Boxcar->Get(7);
        Edit7->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Nout < 1)
    {
        value = Boxcar->Get(8);
        Edit8->Text = FloatToStrF(value, ffFixed, 7,3);
    }
    if (Boxcar->GetB(1))
    {
        B1ValueLabel->Caption = "1";
        B1ToggleButton->Caption = "0";
    }
      else
      {
        B1ValueLabel->Caption = "0";
        B1ToggleButton->Caption = "1";
      }
    if (Boxcar->GetB(2))
    {
        B2ValueLabel->Caption = "1";
        B2ToggleButton->Caption = "0";
    }
      else
      {
        B2ValueLabel->Caption = "0";
        B2ToggleButton->Caption = "1";
      }
//    int D = Boxcar->GetD();
    

}

void __fastcall TBoxcarDialog::FormActivate(TObject *Sender)
{
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::OkButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::CalcelButtonClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::FormCreate(TObject *Sender)
{
    Boxcar->SetNout(0);

}
//---------------------------------------------------------------------------

int TBoxcarDialog::OutputActivate()
{
    int outputs = Boxcar->GetNout();
    if (outputs < 8)
    {
        Edit1->ReadOnly = true;
        IOLabel1->Caption = "IN";
    }
      else
      {
        Edit1->ReadOnly = false;
        IOLabel1->Caption = "OUT";
      }

    if (outputs < 7)
    {
        Edit2->ReadOnly = true;
        IOLabel2->Caption = "IN";
    }
      else
      {
        Edit2->ReadOnly = false;
        IOLabel2->Caption = "OUT";
      }

    if (outputs < 6)
    {
        Edit3->ReadOnly = true;
        IOLabel3->Caption = "IN";
    }
      else
      {
        Edit3->ReadOnly = false;
        IOLabel3->Caption = "OUT";
      }

    if (outputs < 5)
    {
        Edit4->ReadOnly = true;
        IOLabel4->Caption = "IN";
    }
      else
      {
        Edit4->ReadOnly = false;
        IOLabel4->Caption = "OUT";
      }

    if (outputs < 4)
    {
        Edit5->ReadOnly = true;
        IOLabel5->Caption = "IN";
    }
      else
      {
        Edit5->ReadOnly = false;
        IOLabel5->Caption = "OUT";
      }

    if (outputs < 3)
    {
        Edit6->ReadOnly = true;
        IOLabel6->Caption = "IN";
    }
      else
      {
        Edit6->ReadOnly = false;
        IOLabel6->Caption = "OUT";
      }

    if (outputs < 2)
    {
        Edit7->ReadOnly = true;
        IOLabel7->Caption = "IN";
    }
      else
      {
        Edit7->ReadOnly = false;
        IOLabel7->Caption = "OUT";
      }

    if (outputs < 1)
    {
        Edit8->ReadOnly = true;
        IOLabel8->Caption = "IN";
    }
      else
      {
        Edit8->ReadOnly = false;
        IOLabel8->Caption = "OUT";
      }
    return outputs;
}


void __fastcall TBoxcarDialog::NoutComboBoxChange(TObject *Sender)
{
    Boxcar->SetNout(NoutComboBox->ItemIndex);
    OutputActivate();
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::Timer1Timer(TObject *Sender)
{
    GetAllData();
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::ContUpdateButtonExit(TObject *Sender)
{
    Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::UpdateButtonClick(TObject *Sender)
{
    Timer1->Enabled = false;
    GetAllData();    
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::ContUpdateButtonClick(TObject *Sender)
{
    Timer1->Enabled = true;    
}
//---------------------------------------------------------------------------


void __fastcall TBoxcarDialog::Edit2Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit2->Text, &Set))
           Boxcar->Set(2,Set);

}
//---------------------------------------------------------------------------


void __fastcall TBoxcarDialog::Edit3Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit3->Text, &Set))
           Boxcar->Set(3,Set);

}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::Edit4Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit4->Text, &Set))
           Boxcar->Set(4,Set);

}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::Edit5Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit5->Text, &Set))
           Boxcar->Set(5,Set);

}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::Edit6Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit6->Text, &Set))
           Boxcar->Set(6,Set);

}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::Edit7Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit7->Text, &Set))
           Boxcar->Set(7,Set);

}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::Edit8Exit(TObject *Sender)
{
    double Set = 0.0;
    if (ValidReal(Edit8->Text, &Set))
           Boxcar->Set(8,Set);
}
//---------------------------------------------------------------------------


void __fastcall TBoxcarDialog::B1ToggleButtonClick(TObject *Sender)
{
    if (B1INRadioButton->Checked) return; // do nothing if B1 is INPUT
    if (B1ValueLabel->Caption == "1")
    {
        B1ValueLabel->Caption = "0";
        B1ToggleButton->Caption = "1";
        Boxcar->SetB(1,0);
    }
      else
      {
        B1ValueLabel->Caption = "1";
        B1ToggleButton->Caption = "0";
        Boxcar->SetB(1,1);
      }
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::B1OUTRadioButtonClick(TObject *Sender)
{
    Boxcar->SetB(1,0);
    B1ToggleButton->Enabled = true;
    B1ValueLabel->Caption = "0";
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::B1INRadioButtonClick(TObject *Sender)
{
    B1ToggleButton->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::B2OUTRadioButtonClick(TObject *Sender)
{
    Boxcar->SetB(2,0);
    B2ToggleButton->Enabled = true;
    B2ValueLabel->Caption = "0";
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::B2INRadioButtonClick(TObject *Sender)
{
    B2ToggleButton->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TBoxcarDialog::B2ToggleButtonClick(TObject *Sender)
{
    if (B2INRadioButton->Checked) return; // do nothing if B1 is INPUT
    if (B2ValueLabel->Caption == "1")
    {
        B2ValueLabel->Caption = "0";
        B2ToggleButton->Caption = "1";
        Boxcar->SetB(2,0);
    }
      else
      {
        B2ValueLabel->Caption = "1";
        B2ToggleButton->Caption = "0";
        Boxcar->SetB(2,1);
      }

}
//---------------------------------------------------------------------------

