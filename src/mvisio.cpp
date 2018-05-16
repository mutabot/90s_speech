//---------------------------------------------------------------------------
#include <vcl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#pragma hdrstop

#include "mvisio.h"
#include "fdir.h"
#include "signal.h"
#include "tts.h"
#include "fldrec.h"
#include "fldextr.h"
#include "foldman.h"
#include "fldproc.h"
#include "segmentate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AudioIO"
#pragma resource "*.dfm"
TfmVisio *fmVisio;
//---------------------------------------------------------------------------
__fastcall TfmVisio::TfmVisio(TComponent* Owner)
        : TForm(Owner)
{
 TRegistry *Registry = new TRegistry();
 Registry->OpenKey("Software\\IMS\\visio", true);
 try {
  Registry->ReadBinaryData("Colors", &Colors, sizeof(TColors));
 } catch ( ... ) {
  Colors.Bk = clNavy;
  Colors.Fold = clLime;
  Colors.PCM = clYellow;
 }
 Registry->CloseKey();
 delete Registry;

 OpenDialog->InitialDir = GetCurrentDir() + "\\..\\pcm";
 SaveDialog->InitialDir = OpenDialog->InitialDir;
 FoldStore = new TFoldStore(GetCurrentDir() + "\\samples.fol");
 Extruder = new TExtruder(this);
 fmTTS = NULL;
 fmFldRec    = NULL;
 fmFldExtr   = NULL;
 fmSegments  = NULL;
 CurDir = GetCurrentDir();
 Speech = new TSpeech(FoldStore, CurDir);
}
//---------------------------------------------------------------------------
__fastcall TfmVisio::~TfmVisio()
{
 delete Speech;
 delete Extruder;
 delete FoldStore;
 TRegistry *Registry = new TRegistry();
 Registry->OpenKey("Software\\IMS\\visio", true);
 Registry->WriteBinaryData("Colors", &Colors, sizeof(TColors));
 Registry->CloseKey();
 delete Registry;
 if (fmFldProc) delete fmFldProc;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::NOpenClick(TObject *Sender)
{
 OpenDialog->FilterIndex = 1;
 if (!OpenDialog->Execute()) return;
 fmSignal = new TfmSignal(OpenDialog->FileName, "", &Colors, &OnChildChange, this);
 fmSignal->OnClose = FormClose;
// MRUMan->Remove(OpenDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::NOpenBufClick(TObject *Sender)
{
 OpenDialog->FilterIndex = 2;
 if (!OpenDialog->Execute()) return;
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return;
 Signal->BufFileName = OpenDialog->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::tbNextClick(TObject *Sender)
{
 if (ActiveMDIChild) ((TfmSignal*)ActiveMDIChild)->StepFold();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfmVisio::cbScaleChange(TObject *Sender)
{
 int scale;
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return;
 if ( (scale = cbScale->Text.ToIntDef(1)) != 0) Signal->Skale = scale;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::tbSynClick(TObject *Sender)
{
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal)
 {
    tbSyn->Down = false;
    return;
 }
 Signal->Synch = tbSyn->Down;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::NExportClick(TObject *Sender)
{
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return;
 TSample Sample;
 if ((Signal->sel_seg[0] < 0)||(Signal->sel_seg[1] < 0)) return;
 Sample.pcount = (Signal->sel_seg[1] - Signal->sel_seg[0] + 1);
 FoldStore->WriteDialog(&Sample, &Signal->fbuf[Signal->sel_seg[0]]);
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::nSBaseClick(TObject *Sender)
{
 if (!SaveDialog->Execute()) return;
 delete FoldStore;
 FoldStore = new TFoldStore(SaveDialog->FileName);
 nSBase->Caption = "Speech base... (" + SaveDialog->FileName + ")";
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::NColorsClick(TObject *Sender)
{
 fmColor = new TfmColor(this);
 fmColor->Colors = &Colors;
 fmColor->ShowModal();
 delete fmColor;
}
//---------------------------------------------------------------------------
bool __fastcall TfmVisio::AudioOutFillBuffer(PChar Buffer, int &Size)
{
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return false;
 Signal->DrawMark(msPlay->Position >> 1);   // position in bytes, mark is in samples
 Size = msPlay->Read(Buffer, AudioOut1->BufferSize);
 return (Size == AudioOut1->BufferSize) && (msPlay->Position < stoppos);    // all bytes here
}
//---------------------------------------------------------------------------
int TfmVisio::OnBufferReady(short *buf, int size)
{
    if (msPlay)
    {
        int wr = msPlay->Write(buf, size*sizeof(short));
    }
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::tbPlayClick(TObject *Sender)
{
 int i, efrom, esize;
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;

 if (!Signal) return;
 if (AudioOut1->Active == true)
 {
  AudioOut1->StopGracefully();
  tbPlay->Caption = "Play";
  AudioOut1->StopAtOnce();
  AudioOut1->Active = false;
 }
 else
 {
  msPlay = new TMemoryStream();
  if (Signal->sel_seg[0] <= 0) efrom = 0;
  else efrom = Signal->sel_seg[0];

  if (Signal->sel_seg[1] <= 0) esize = Signal->pseg[1];
  else esize = Signal->sel_seg[1] - Signal->sel_seg[0];

  if (esize < 0) return ;   // prevent wierd behaviour

  Signal->fsFold->Position = Signal->sel_seg[0]*sizeof(TFPoint);
  Extruder->Extrude(Signal->fsFold, NULL, esize);
  int startpos = 0;
  for (i = 1; i < efrom; i++) startpos+=Signal->fbuf[i].m;
  msPlay->Position = startpos << 1; // startpos is in samples
  stoppos = startpos;
  for (i = efrom; i < esize; i++) stoppos+=Signal->fbuf[i].m;
  stoppos<<=1;  // stoppos was in samples
  AudioOut1->Start(AudioOut1);
  tbPlay->Caption = "Stop";
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::AudioOutStop(TObject *Sender)
{
 delete msPlay;
 tbPlay->Caption = "Play";
 tbPlay->Down = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::btZoomSelClick(TObject *Sender)
{
 int i, scale, newscale;
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return;
 Signal->ZoomSel();
 i = 10;
 newscale = Signal->Skale;
 while (i < cbScale->Items->Count)
 {
  scale = cbScale->Items->Strings[i].ToIntDef(1);
  if (abs(scale - newscale) < 10) cbScale->Items->Delete(i);
  else i++;
 }
 cbScale->Text = AnsiString(newscale);
 cbScale->Items->Add(cbScale->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::NFileClick(TObject *Sender)
{
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) NOpenBuf->Enabled = false;
 else NOpenBuf->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::NMorphClick(TObject *Sender)
{
 if (!fmTTS) fmTTS = new TfmTTS(Speech, NULL);
 fmTTS->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::acRecExecute(TObject *Sender)
{
 if (!fmFldRec) fmFldRec = new TfmFldRec(0);
 fmFldRec->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::acExtrudeExecute(TObject *Sender)
{
 if (!fmFldExtr) fmFldExtr = new TfmFldExtr(0);
 fmFldExtr->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::OnChildChange(System::TObject* Sender)
{
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return;
 edM->Text = Signal->FPointText;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::acFindInExecute(TObject *Sender)
{
 if (!fmFldProc) fmFldProc = new TfmFldProc(FoldStore, this);
 fmFldProc->ClearSignal();
 for (int i = 0; i < MDIChildCount; i++)
  fmFldProc->AddSignal(MDIChildren[i], MDIChildren[i] == ActiveMDIChild);
 fmFldProc->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::FormClose(TObject *Sender, TCloseAction &Action)
{
 TfmSignal *Signal = (TfmSignal *)Sender;
 Action = caFree;
 if (!Signal) return;
// MRUMan->Strings->Insert(0, Signal->FoldFileName);
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::MRUManClick(TObject *Sender,
      const AnsiString RecentName, const AnsiString Caption, int UserData)
{
 fmSignal = new TfmSignal(RecentName, "", &Colors, &OnChildChange, this);
 fmSignal->OnClose = FormClose;
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::acFoldManExecute(TObject *Sender)
{
 if (!fmFoldMan) fmFoldMan = new TfmFoldMan(this);
 fmFoldMan->cbSignals->Clear();
 for (int i = 0; i < MDIChildCount; i++)
  fmFoldMan->AddSignal(MDIChildren[i], MDIChildren[i] == ActiveMDIChild);
 fmFoldMan->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfmVisio::acReduceExecute(TObject *Sender)
{
 TfmSignal *Signal = (TfmSignal *)ActiveMDIChild;
 if (!Signal) return;
 if (!fmSegments) fmSegments = new TfmSegments(Signal, NULL);
 fmSegments->Show();
}
//---------------------------------------------------------------------------




