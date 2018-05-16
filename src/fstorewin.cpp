//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fstorewin.h"
#include "quest.h"
#include "fdir.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "TFlatButtonUnit"
#pragma link "TFlatComboBoxUnit"
#pragma link "TFlatEditUnit"
#pragma link "TFlatHintUnit"
#pragma link "TFlatSpinEditUnit"
#pragma link "RXSpin"
#pragma link "RXSpin"
#pragma resource "*.dfm"
TfmFSWin *fmFSWin;
//---------------------------------------------------------------------------

__fastcall TfmFSWin::TfmFSWin(TForm *Owner, TFoldStore *FS)
        : TForm(Owner)
{
 FoldStore = FS;
 btAction = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmFSWin::Write(TSample *Sample, TFPoint *buf)
{
 AnsiString asSample;
 int i, sel_len = 0;
 for (i = 1; i < Sample->pcount; i++) sel_len+=buf[i].m;
 edComment->Text = AnsiString(sel_len);
 ShowModal();
 if (!btAction) return;
 Sample->sound = cbSound->Text[1] - BASE_CHAR_0;
 Sample->tone = edSpin->Value;
 if (FoldStore->Write(Sample, buf) < 0)
 {
  fmQuest = new TfmQuest(this);
  asSample = AnsiString((char)(BASE_CHAR_0 + Sample->sound)) + AnsiString(Sample->tone);
  if (!fmQuest->Execute("Сэмпл [" + asSample + "] существует! \r\n Заменить? ")) return;
  FoldStore->Delete(Sample);
  FoldStore->Write(Sample, buf);
 }
 FoldStore->SetComment(Sample, edComment->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfmFSWin::btCancelClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmFSWin::btOkClick(TObject *Sender)
{
 btAction = true;
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmFSWin::btDelClick(TObject *Sender)
{
 TSample Sample;
 Sample.sound = cbSound->Text[1] - BASE_CHAR_0;
 Sample.tone = edSpin->Value;
 FoldStore->Delete(&Sample);
}
//---------------------------------------------------------------------------
void __fastcall TfmFSWin::btMapClick(TObject *Sender)
{
 fmFoldDir = new TfmFoldDir(FoldStore, this);
 fmFoldDir->ShowModal();
 delete fmFoldDir;
}
//---------------------------------------------------------------------------

