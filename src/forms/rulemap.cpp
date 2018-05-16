//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "rulemap.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmRuleMap *fmRuleMap;
//---------------------------------------------------------------------------
__fastcall TfmRuleMap::TfmRuleMap(TSpeechRules *FARules, TSpeechRules *FBRules, TComponent* Owner)
        : TForm(Owner)
{
 ARules = FARules;
 BRules = FBRules;
 Symbols = new TStringList();
 Symbols->Sorted = true;
}
//---------------------------------------------------------------------------
__fastcall TfmRuleMap::~TfmRuleMap()
{
 delete Symbols;
}
//---------------------------------------------------------------------------
void __fastcall TfmRuleMap::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmRuleMap::UpdateRules()
{
 int i, idx;
 AnsiString Symbol;
 Symbols->Clear();
 for (i = 0; i < ARules->Count; i++)
 {
  Symbol = ((TSpeechRule *)ARules->Items[i])->Symbol;
  if (!Symbols->Find(Symbol, idx)) Symbols->Add(Symbol);
 }
 for (i = 0; i < BRules->Count; i++)
 {
  Symbol = ((TSpeechRule *)BRules->Items[i])->Symbol;
  if (!Symbols->Find(Symbol, idx)) Symbols->Add(Symbol);
 }
// RGrid->ColCount = Symbols->Count + 1;
// RGrid->RowCount = Symbols->Count + 1;
 if (!Visible) Show();
// RGrid->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfmRuleMap::RGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
 AnsiString RText, Symbol, Neighbor;
 int C = ACol - 1, R = ARow - 1;
 if (ACol == 0 && ARow == 0) return;
 if (ACol == 0)
 {
//  RGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 1, Symbols->Strings[R]);
  return;
 }
 if (ARow == 0)
 {
//  RGrid->Canvas->TextOut(Rect.Left + 2, Rect.Top + 1, Symbols->Strings[C]);
  return;
 }
// RGrid->Canvas->Brush->Color = clBackground;
// RGrid->Canvas->Pen->Color = clBackground;
 if (ARules->FindRule(Symbols->Strings[C], Symbols->Strings[R]))      ;
//  RGrid->Canvas->Rectangle(Rect.Left, Rect.Top, Rect.Left + Rect.Width() / 2, Rect.Bottom);

 if (BRules->FindRule(Symbols->Strings[C], Symbols->Strings[R]))            ;
//  RGrid->Canvas->Rectangle(Rect.Left + Rect.Width() / 2, Rect.Top, Rect.Right, Rect.Bottom);

}
//---------------------------------------------------------------------------
void __fastcall TfmRuleMap::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 fmRuleMap = NULL;
 Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TfmRuleMap::RGridDblClick(TObject *Sender)
{
 UpdateRules();
}
//---------------------------------------------------------------------------

