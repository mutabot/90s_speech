//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "hirules.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfmHiRules *fmHiRules;
//---------------------------------------------------------------------------
__fastcall TfmHiRules::TfmHiRules(TSpeech *ASpeech, TComponent* Owner)
        : TForm(Owner)
{
 Speech  = ASpeech;
 UpdateItems();
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 fmHiRules = NULL;
 Action = caFree;       
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::acNewRExecute(TObject *Sender)
{
 TListItem *Item;
 if (Speech->HiRules->CreateRule(edText->Text, sePos->Value, edSymbol->Text) != NULL)
 {
  Item = lvHiRules->Items->Add();
  Item->SubItems->Add(AnsiString((short)sePos->Value));
  Item->SubItems->Add(edSymbol->Text);
  Item->Caption = edText->Text;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::acSaveRExecute(TObject *Sender)
{
 if (!lvHiRules->Selected) return;
 Speech->HiRules->Delete(lvHiRules->Selected->Caption);
 if (Speech->HiRules->CreateRule(edText->Text, sePos->Value, edSymbol->Text) != NULL)
 {
  lvHiRules->Selected->SubItems->Add(AnsiString((short)sePos->Value));
  lvHiRules->Selected->SubItems->Add(edSymbol->Text);
  lvHiRules->Selected->Caption = edText->Text;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::acDelRExecute(TObject *Sender)
{
 if (!lvHiRules->Selected) return;
 Speech->HiRules->Delete(lvHiRules->Selected->Caption);
 lvHiRules->Selected->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::UpdateItems( void )
{
 THiRule *Rule;
 TListItem *Item;
 lvHiRules->Items->EndUpdate();
 lvHiRules->Items->Clear();
 for (int i = 0; i < Speech->HiRules->Count; i++)
 {
  Rule = (THiRule *)Speech->HiRules->Items[i];
  Item = lvHiRules->Items->Add();
  Item->SubItems->Add(AnsiString(Rule->Pos));
  Item->SubItems->Add(Rule->Symbol);
  Item->Caption = Rule->Text;
 }
 lvHiRules->Items->BeginUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::lvHiRulesChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
 if (Change != ctState) return;
 THiRule *Rule = Speech->HiRules->GetRule(Item->Caption);
 if (!Rule)
 {
  edText->Clear();
  edSymbol->Clear();
 } else {
  edText->Text = Rule->Text;
  sePos->Value = Rule->Pos;
  edSymbol->Text = Rule->Symbol;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::edTextKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_RETURN && lvHiRules->Selected) acSaveR->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmHiRules::lvHiRulesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_DELETE && lvHiRules->Selected) acDelR->Execute();
}
//---------------------------------------------------------------------------

