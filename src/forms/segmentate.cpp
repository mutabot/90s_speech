//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "segmentate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmSegments *fmSegments;
//---------------------------------------------------------------------------
__fastcall TfmSegments::TfmSegments(TfmSignal* ASignal, TComponent* Owner)
        : TForm(Owner)
{
 Signal = ASignal;
 FoldProcess = new TFoldProcess();
 FoldProcess->OnParams = OnParams;
 FoldProcess->OnCompare = OnCompare;
 FoldProcess->OnRewidth = OnRewidth;
 FoldProcess->OnNoise = OnNoise;
 pcount = segstart = 0;
 ParamsList = new TList();
 Statistics = new TStringList;
}
//---------------------------------------------------------------------------
__fastcall TfmSegments::~TfmSegments()
{
 delete Statistics;
 for (int i = 0; i < ParamsList->Count; i++)
  delete (TSParams*)ParamsList->Items[i];
 delete ParamsList;
 delete FoldProcess;
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 fmSegments = NULL;
 Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::OnRewidth(TObject *Sender)
{
 meDev->Lines->Add("width: min: " + AnsiString(FoldProcess->minwidth) + " max: " + AnsiString(FoldProcess->maxwidth));
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::OnParams(TSParams *Params)
{
/* meDev->Lines->Add(AnsiString(Params->length)+", "+AnsiString(Params->ldir)+", "+
                   AnsiString(Params->llen)+", "+AnsiString(Params->rdir)+", "+
                   AnsiString(Params->rlen)+", "+AnsiString(Params->pwidth));
 meDev->Lines->Add("  ---  ");*/
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::OnCompare(int d, double dlen[], int dpar[])
{
 meDev->Lines->Add(AnsiString(d)+" || "+
                   AnsiString::FormatFloat("#####0.000", dlen[0])+", "+
                   AnsiString::FormatFloat("#####0.000", dlen[1])+", "+
                   AnsiString::FormatFloat("#####0.000", dlen[2])+" | "+
                   AnsiString(dpar[0])+", "+AnsiString(dpar[1])+" | "+
                   AnsiString(dpar[2])+" | "+AnsiString(dpar[3])+", "+
                   AnsiString(dpar[4]));

}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::btStartSegClick(TObject *Sender)
{
 int dev = FoldProcess->Segmentate(OnSegment);
 meDev->Lines->Add(AnsiString(dev));
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::btWindowClick(TObject *Sender)
{
 int dev;
 while (1) {
  try {
   if ((dev = FoldProcess->Segmentate(OnSegment)) == -MaxInt)
   { meDev->Lines->Add("Window done..."); return; }
  } catch (...) { meDev->Lines->Add("INTERNAL ERROR!"); return; }
  meDev->Lines->Add(AnsiString(dev));
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::btReduceClick(TObject *Sender)
{
 Statistics->Clear();
 segstart = 0;
 pcount = FoldProcess->InsertFold(Signal->fbuf, Signal->pseg[1]);
 Signal->Repaint();
 FoldProcess->Reset();
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::OnNoise(TFPoint *fbuf, int offset, int pcount)
{
 Signal->sel_seg[0] = segstart+offset;
 Signal->sel_seg[1] = segstart+pcount;
 Signal->Repaint();
 meDev->Lines->Add("ШУМ!"); 
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::OnSegment(TFPoint *Segment, int offset, int pcount, int dev)
{
 int *Params;
 AnsiString asParams = "";
 Signal->sel_seg[0] = segstart+offset;
 Signal->sel_seg[1] = segstart+pcount;
 Signal->Repaint();
 if (dev < 20) {
  Params = FoldProcess->GenIDParams(&Segment[offset], pcount - offset + 1);
  if (!Params) { meDev->Lines->Add("Нет стороны!"); return; }
  ParamsList->Add(Params);
  for (int i = 0; i < NUM_OF_ID_PARAMS; i++) asParams = asParams + AnsiString(Params[i]) + "  ";
  Statistics->Add(asParams);
  meDev->Lines->Add(asParams);
  pbParams->Repaint();
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::pbParamsPaint(TObject *Sender)
{
 TColor Clrs[NUM_OF_ID_PARAMS] = { clGreen, clBlue, clRed, clLime, clNavy };
 int i, j, MaxParam, *Params;
 double skale;
 MaxParam = 1;
 while(pbParams->Width / 4 < ParamsList->Count) {
  delete (TIDParams*)ParamsList->Items[0];
  ParamsList->Delete(0);
 }
 for (i = 0; i < ParamsList->Count; i++)
 {
  Params = (int*)ParamsList->Items[i];
  for (j = 0; j < NUM_OF_ID_PARAMS; j++) if (MaxParam < Params[j]) MaxParam = Params[j];
 }
 skale = (double)(pbParams->Height - 10) / (double)MaxParam;
 pbParams->Canvas->MoveTo(4, 0);
 pbParams->Canvas->LineTo(4, pbParams->Height);
 for (i = 0; i < pbParams->Height; i+=10)
  pbParams->Canvas->Rectangle(0, pbParams->Height - i-1, 4, pbParams->Height - i+1);

 for (i = 0; i < ParamsList->Count; i++)
 {
  Params = (int*)ParamsList->Items[i];
  for (j = 0; j < NUM_OF_ID_PARAMS; j++)
  {
   pbParams->Canvas->Brush->Color = Clrs[j];
   pbParams->Canvas->Pen->Color = Clrs[j];
   pbParams->Canvas->Rectangle(8+i*4, pbParams->Height - Params[j]*skale - 2,
                               8+(i+1)*4, pbParams->Height - Params[j]*skale + 2);
  }
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmSegments::btSaveClick(TObject *Sender)
{
 Statistics->SaveToFile("stats.txt");
}
//---------------------------------------------------------------------------

