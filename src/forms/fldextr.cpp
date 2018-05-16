//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fldextr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmFldExtr *fmFldExtr;
//---------------------------------------------------------------------------
__fastcall TfmFldExtr::TfmFldExtr(TComponent* Owner)
        : TForm(Owner)
{
 fsFold = NULL;
 Result = NULL;
 Extruder = new TExtruder(this);
 fnFold->InitialDir = GetCurrentDir();
 fnResult->InitialDir = GetCurrentDir();
}
//---------------------------------------------------------------------------
void __fastcall TfmFldExtr::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldExtr::FormDestroy(TObject *Sender)
{
 delete Extruder;
 if (fsFold) delete fsFold;
 if (Result) delete Result;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldExtr::Button3Click(TObject *Sender)
{
 if ( (fsFold = new TFileStream(fnFold->FileName, fmOpenRead)) == NULL) return;
 if ( (Result = new TFileStream(fnResult->FileName, fmCreate)) == NULL) return;
 Button3->Enabled = false;
 pbProgress->Position = 0;
 pbProgress->Visible = true;
 pbProgress->Max = fsFold->Size;
 Cursor = crHourGlass;

 Extruder->Extrude(fsFold, Result, -1);

 Button3->Enabled = true;
 pbProgress->Visible = false;
 Cursor = crDefault;
 delete Result;
 delete fsFold;
 fsFold = NULL;
 Result = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldExtr::fnFoldAfterDialog(TObject *Sender,
      AnsiString &Name, bool &Action)
{
 fnResult->FileName = Name.SubString(1, Name.Length() - 3) + "pcm";
 pbProgress->Position = 0;
 pbProgress->Visible = true;
}
//---------------------------------------------------------------------------
int TfmFldExtr::OnBufferReady(short *buf, int size)
{
 pbProgress->Position = fsFold->Position;
 return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldExtr::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 fmFldExtr = NULL;
 Action = caFree;        
}
//---------------------------------------------------------------------------

void __fastcall TfmFldExtr::btBrowseClick(TObject *Sender)
{
    fnFold->Execute();
    edSrcFile->Text = fnFold->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfmFldExtr::btDstBrowseClick(TObject *Sender)
{
    fnResult->Execute();
    edDstFile->Text = fnResult->FileName;
}
//---------------------------------------------------------------------------

