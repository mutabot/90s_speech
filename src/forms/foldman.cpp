//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "foldman.h"
#include "signal.h"
#include "sptecdef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfmFoldMan *fmFoldMan;
//---------------------------------------------------------------------------
__fastcall TfmFoldMan::TfmFoldMan(TComponent* Owner)
        : TForm(Owner)
{
 fsFold = NULL;
}
//---------------------------------------------------------------------------
__fastcall TfmFoldMan::~TfmFoldMan()
{
 if (fsFold) delete fsFold;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::cbOnTopClick(TObject *Sender)
{
 if (FormStyle == fsStayOnTop) FormStyle = fsNormal;
 else FormStyle = fsStayOnTop;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::btStartClick(TObject *Sender)
{
 if (btStart->Down)
 {
  if (!fsFold) fsFold = new TFileStream(fnResult->FileName, fmCreate);
  btDstBrowse->Enabled = false;
  btPlain->Enabled = true;
  btSeg->Enabled = true;
 } else {
  delete fsFold;
  fsFold = NULL;
  btDstBrowse->Enabled = true;
  btPlain->Enabled = false;
  btSeg->Enabled = false;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 fmFoldMan = NULL;
 Action = caFree;        
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::AddSignal(TForm * Signal, bool Active)
{
 cbSignals->Items->AddObject(Signal->Caption, Signal);
 if (Active) cbSignals->ItemIndex = cbSignals->Items->Count - 1;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::btPlainClick(TObject *Sender)
{
 TfmSignal *Signal = (TfmSignal *)cbSignals->Items->Objects[cbSignals->ItemIndex];
 if (!Signal) return;
 if (Signal->sel_seg[0] < 0 || Signal->sel_seg[1] < 0) return;
 Signal->fsFold->Position = (Signal->pseg[0] + Signal->sel_seg[0])*sizeof(TFPoint);
 fsFold->CopyFrom(Signal->fsFold, (Signal->sel_seg[1] - Signal->sel_seg[0])*sizeof(TFPoint));
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldMan::btSegClick(TObject *Sender)
{
 TFPoint flag;
 TfmSignal *Signal = (TfmSignal *)cbSignals->Items->Objects[cbSignals->ItemIndex];
 if (!Signal) return;
 if (Signal->sel_seg[0] < 0 || Signal->sel_seg[1] < 0) return;
 Signal->fsFold->Position = (Signal->pseg[0] + Signal->sel_seg[0])*sizeof(TFPoint);
 flag.m = 0;
 flag.a = Signal->sel_seg[1] - Signal->sel_seg[0] + 1;
 fsFold->Write(&flag, sizeof(TFPoint));
 fsFold->CopyFrom(Signal->fsFold, flag.a*sizeof(TFPoint));
 flag.m = 0xFF;
 flag.a = seMorLen->Value;
 fsFold->Write(&flag, sizeof(TFPoint));
}
//---------------------------------------------------------------------------

void __fastcall TfmFoldMan::btDstBrowseClick(TObject *Sender)
{
    fnResult->Execute();
    edDstFile->Text = fnResult->FileName;
}
//---------------------------------------------------------------------------


