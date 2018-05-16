//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "colors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmColor *fmColor;
//---------------------------------------------------------------------------
__fastcall TfmColor::TfmColor(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmColor::btCancelClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TfmColor::btOkClick(TObject *Sender)
{
 Colors->Bk = cbBk->Selected;
 Colors->Fold = cbFold->Selected;
 Colors->PCM =  cbPCM->Selected;
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmColor::FormActivate(TObject *Sender)
{
 cbBk->Selected = Colors->Bk;
 cbFold->Selected = Colors->Fold;
 cbPCM->Selected = Colors->PCM;
}
//---------------------------------------------------------------------------

