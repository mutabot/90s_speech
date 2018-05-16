//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "extr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmExtrude *fmExtrude;
//---------------------------------------------------------------------------
__fastcall TfmExtrude::TfmExtrude(TComponent* Owner)
        : TForm(Owner)
{
 fsFold = NULL;
 Result = NULL;
 Extruder = new TExtruder();
 Extruder->OnExtrude = OnExtrude;
}
//---------------------------------------------------------------------------
void __fastcall TfmExtrude::FormDestroy(TObject *Sender)
{
 delete Extruder;
 if (fsFold) delete fsFold;
 if (Result) delete Result;
}
//---------------------------------------------------------------------------
void __fastcall TfmExtrude::Button3Click(TObject *Sender)
{
 int i_read;
 char fold[8192];
 if ( (fsFold = new TFileStream(fnFold->FileName, fmOpenRead)) == NULL) return;
 if ( (Result = new TFileStream(fnResult->FileName, fmCreate)) == NULL) return;
 Button3->Enabled = false;
 pbProgress->Position = 0;
 pbProgress->Visible = true;
 pbProgress->Max = fsFold->Size;
 Cursor = crHourGlass;
 while (fsFold->Position != fsFold->Size)
 {
  i_read = fsFold->Read(fold, sizeof(fold));
  Extruder->Extrude(Extruder->last, fold, i_read);
 }
 Button3->Enabled = true;
 pbProgress->Visible = false;
 Cursor = crDefault;
 delete Result;
 delete fsFold;
 fsFold = NULL;
 Result = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmExtrude::fnFoldAfterDialog(TObject *Sender,
      AnsiString &Name, bool &Action)
{
 fnResult->FileName = Name.SubString(1, Name.Length() - 3) + "pcm";
 pbProgress->Position = 0;
 pbProgress->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmExtrude::OnExtrude(unsigned char *buf, int size)
{
 Result->Write(buf, size);
 pbProgress->Position = fsFold->Position;
}
//---------------------------------------------------------------------------
