//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fdir.h"
#include "quest.h"
#include "segview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmFoldDir *fmFoldDir;
//---------------------------------------------------------------------------
__fastcall TfmFoldDir::TfmFoldDir(TFoldStore *FoldStore, TSample *ASample, TComponent* Owner)
        : TForm(Owner)
{
 FFoldStore = FoldStore;
 tempBrush = new TBrush();
 tempPen = new TPen();
 sample = ASample;
 for (int i = 0; i < 33; i++) marker[i] = -1;
}
//---------------------------------------------------------------------------
__fastcall TfmFoldDir::~TfmFoldDir()
{
 delete tempPen;
 delete tempBrush;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
 if ((ACol == 0) && (ARow == 0)) return;
 if ((ACol == 1) && (ARow == 1))
 {
  tempBrush->Assign(FGrid->Canvas->Brush);
  FGrid->Canvas->Brush->Color = clSilver;
  FGrid->Canvas->Rectangle(Rect.Left + 1, Rect.Top + 1, Rect.Right - 1, Rect.Bottom - 1);
  FGrid->Canvas->Brush->Assign(tempBrush);
  return;
 }
 if (ACol == 0)
 {
  if (!ARow) return;
  FGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 1,
                                   AnsiString((char)(BASE_CHAR_0 + ARow-1)));
  return;
 }
 if (ARow == 0)
 {
  FGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 1, AnsiString::IntToHex(ACol-1, 2));
  return;
 }
 m_sample.sound = ARow - 1;
 m_sample.tone = ACol - 1;
 if (!FFoldStore->SampleExists(&m_sample)) return;
 tempBrush->Assign(FGrid->Canvas->Brush);
 if (marker[ARow] == ACol) FGrid->Canvas->Brush->Color = clRed;
 else FGrid->Canvas->Brush->Color = clYellow;
 FGrid->Canvas->Ellipse(Rect.Left + 2, Rect.Top + 2, Rect.Right - 2, Rect.Bottom - 2);
 FGrid->Canvas->Brush->Assign(tempBrush);
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 int C = -1, R = -1;
// FGrid->MouseToCell(X, Y, C, R);
 if ((C < 1)||(R < 1)) return;
 m_sample.sound = R - 1;
 m_sample.tone = C -1;
 if (!FFoldStore->SampleExists(&m_sample)) FGrid->ShowHint = false;
 else
 {
  FGrid->ShowHint = true;
  FGrid->Hint = AnsiString((char)(BASE_CHAR_0 + m_sample.sound))+":"+AnsiString(m_sample.tone) + " - " + FFoldStore->GetComment(&m_sample);
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
 m_sample.sound = ARow - 1;
 m_sample.tone = ACol -1;
 if (!FFoldStore->SampleExists(&m_sample))
 {
  edComment->Text = ""; edComment->Enabled = false;
  edWidth->Text = AnsiString((char)(BASE_CHAR_0 + m_sample.sound))+":"+AnsiString(m_sample.tone);
 }
 else
 {
  edComment->Enabled = true;
  sample->sound = m_sample.sound;
  sample->tone = m_sample.tone;
  edComment->Text = FFoldStore->GetComment(&m_sample);
  edWidth->Text = AnsiString((char)(BASE_CHAR_0 + m_sample.sound))+":"+AnsiString(m_sample.tone)
                + ", W=" + AnsiString(FFoldStore->GetWidth(&m_sample));
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::edCommentKeyPress(TObject *Sender, char &Key)
{
 switch (Key)
 {
  case 13: edCommentButtonClick(NULL); break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::edCommentButtonClick(TObject *Sender)
{
  int C, R;
/*  for (C = FGrid->Selection.Left; C <= FGrid->Selection.Right; C++)
   for (R = FGrid->Selection.Top; R <= FGrid->Selection.Bottom; R++)
   {
    m_sample.sound = R - 1;
    m_sample.tone = C -1;
    if (!FFoldStore->SampleExists(&m_sample)) continue;
    else FFoldStore->SetComment(&m_sample, edComment->Text);
   }
 edComment->SelectAll();
 */
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::btDelClick(TObject *Sender)
{
 bool delall = false;
 int C, R;
 AnsiString asSample;
/* for (C = FGrid->Selection.Left; C <= FGrid->Selection.Right; C++)
  for (R = FGrid->Selection.Top; R <= FGrid->Selection.Bottom; R++)
  {
   m_sample.sound = R - 1;
   m_sample.tone = C - 1;
   if (FFoldStore->SampleExists(&m_sample) && !delall)
   {
    fmQuest = new TfmQuest(this);
    asSample = AnsiString((char)(BASE_CHAR_0 + m_sample.sound)) + AnsiString(m_sample.tone);
    switch (fmQuest->Execute("Сэмпл [" + asSample + "] существует! \r\n Удалить? ")) {
     case mrNo: continue;
     case mrYesToAll: delall = true;
    }
   }
   FFoldStore->Delete(&m_sample);
  }
  */
 FGrid->Update();
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FormCloseQuery(TObject *Sender, bool &CanClose)
{
// sample->sound = FGrid->Row - 1;
// sample->tone = FGrid->Col - 1;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridDblClick(TObject *Sender)
{
 ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 int C, R;
 if (Button == mbRight)
 {
//  FGrid->MouseToCell(X, Y, C, R);
//  if (R >= FGrid->RowCount) return;
  marker[R] = C;
  FGrid->Repaint();
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if (Button == mbLeft && Shift.Contains(ssCtrl) )
 {
  fmSegView = new TfmSegView(this);
  FFoldStore->Read(&m_sample, fmSegView->fbuf);
  fmSegView->Caption = AnsiString((char)(BASE_CHAR_0 + m_sample.sound)) + ":" + AnsiString((int)m_sample.tone);
  fmSegView->Show(m_sample.pcount);
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFoldDir::FGridKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_DELETE) btDelClick(NULL);
}
//---------------------------------------------------------------------------


