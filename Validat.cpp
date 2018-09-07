//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Validat.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// Validation routines
bool ValidReal(AnsiString S)
{
    int i=1;
    int L = S.Length();
    while ((S[i] == ' ') && (i<L)) i++;   //scan over starting blanks
    if (i==L) return false;  // if empty sting return false
    

    else return true;
}
