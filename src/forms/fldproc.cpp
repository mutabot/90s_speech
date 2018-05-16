//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fldproc.h"
#include "signal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmFldProc *fmFldProc;
//---------------------------------------------------------------------------
__fastcall TfmFldProc::TfmFldProc(TFoldStore *FoldStore, TComponent* Owner)
        : TForm(Owner)
{
 FFoldStore  = FoldStore;
 FoldProcess = new TFoldProcess();
 sample = NULL;
 fsResult = NULL;
}
//---------------------------------------------------------------------------
__fastcall TfmFldProc::~TfmFldProc()
{
 if (fsResult) delete fsResult;
 if (sample) delete sample;
 delete FoldProcess;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::AddSignal(TForm * Signal, bool Active)
{
 cbSignals->Items->AddObject(Signal->Caption, Signal);
 if (Active) cbSignals->ItemIndex = cbSignals->Items->Count - 1;
}
//---------------------------------------------------------------------------
int __fastcall TfmFldProc::Execute()
{
 Show();
 return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::cbBlockButtonClick(TObject *Sender)
{
 if (!FFoldStore) return;
 sample = new TSample;
// cbBlock->Text = FFoldStore->MapDialog(sample);
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::OnSegment(TFPoint *seg, int pcount, int offset, int dev)
{
 lbReport->Items->AddObject(AnsiString(offset) + ", " + AnsiString(pcount) + ", " + AnsiString(dev), (TObject *)offset);
 if (!fsResult) return;
 if (cbAdapt->Checked) fsResult->Write(&seg[1], pcount*sizeof(TFPoint));
 else fsResult->Write(seg, pcount*sizeof(TFPoint));
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::Button1Click(TObject *Sender)
{
 if (!sample) return;
 TfmSignal *Signal = (TfmSignal *)cbSignals->Items->Objects[cbSignals->ItemIndex];
 if (!Signal) return;
 FFoldStore->Read(sample, fbuf);
 lbReport->Clear();
 if (!fsResult)
  try {
//   fsResult = new TFileStream(feSegments->FileName, fmCreate | fmShareDenyNone);
  } catch ( ... )
  {
//   fsResult = new TFileStream(feSegments->FileName, fmOpenWrite | fmShareDenyNone);
   fsResult->Size = 0;
  }
 pbSearch->Max = Signal->fsFold->Size;
 FoldProcess->SearchSample(Signal->fsFold, fbuf, sample->pcount, OnSegment, OnProgress);
 Pages->ActivePage = tsReport;
 pbSearch->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::seAPassChange(TObject *Sender)
{
// FoldProcess->apass = seAPass->Value;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::seDevPassChange(TObject *Sender)
{
// FoldProcess->devpass = seDevPass->Value;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::feSegmentsAfterDialog(TObject *Sender,
      AnsiString &Name, bool &Action)
{
 if (!Action) return;
 if (fsResult) delete fsResult;
 try {
  fsResult = new TFileStream(Name, fmCreate | fmShareDenyNone);
 } catch ( ... )
 {
  fsResult = new TFileStream(Name, fmOpenWrite | fmShareDenyNone);
  fsResult->Size = 0;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::ClearSignal()
{
  cbSignals->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::FormHide(TObject *Sender)
{
 delete fsResult;
 fsResult = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::OnProgress(int Size, int Pos)
{
 pbSearch->Position = Pos;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldProc::btShowSegClick(TObject *Sender)
{
 if (lbReport->ItemIndex < 0) return;
 TfmSignal *Signal = (TfmSignal *)cbSignals->Items->Objects[cbSignals->ItemIndex];
 if (!Signal) return;
 Signal->Seek((int)lbReport->Items->Objects[lbReport->ItemIndex]);
}
//---------------------------------------------------------------------------


