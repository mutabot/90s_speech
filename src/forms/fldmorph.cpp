//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fldmorph.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfmMorphMain *fmMorphMain;
//---------------------------------------------------------------------------
__fastcall TfmMorphMain::TfmMorphMain(TComponent* Owner)
        : TForm(Owner)
{
 FCurDir = GetCurrentDir();
 FoldStore = new TFoldStore("samples.fol");
 Speech = new TSpeech(FoldStore, FCurDir);
 Init();
}
//---------------------------------------------------------------------------
__fastcall TfmMorphMain::TfmMorphMain(TSpeech *ASpeech, TComponent* Owner)
           : TForm(Owner)
{
 Speech = ASpeech;
 FoldStore = Speech->FoldStore;
 Init();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::Init( void )
{
 TListItem *Item;
 TSpeechRule *Rule;
 for (int i = 0; i < Speech->ARules->Count; i++)
 {
  Item = lvARules->Items->Add();
  Rule = (TSpeechRule *)Speech->ARules->Items[i];
  AssignRule(Item, Rule);
 }
 for (int i = 0; i < Speech->BRules->Count; i++)
 {
  Item = lvBRules->Items->Add();
  Rule = (TSpeechRule *)Speech->BRules->Items[i];
  AssignRule(Item, Rule);
 }
 ActivePage = lvARules;
 ARule = BRule = NULL;
 SortColumn = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::FormClose(TObject *Sender, TCloseAction &Action)
{
 Action = caFree;
 fmMorphMain = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::edSegmentButtonClick(TObject *Sender)
{
 TSample Sample;
 AnsiString asSeg = FoldStore->MapDialog(&Sample);
 if (asSeg > "") edSegment->Text = asSeg;
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::btInsSegClick(TObject *Sender)
{
 if (edSegment->Text.Length() < 2) return;
 edMacro->SelLength = 0;
 edMacro->SelText = edSegment->Text + ", " + AnsiString((int)seLen->Value) +
                                 ", " + AnsiString((int)seAdvLen->Value) + "; ";
 //else edMacro->SelText = edSegment->Text + ", " + AnsiString((int)seLen->Value) + "; ";
 acSaveMacro->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::AssignRule(TListItem *Item, TSpeechRule *Rule)
{
 Item->SubItems->Clear();
 Item->SubItems->Add(Rule->Symbol);
 Item->SubItems->Add(Rule->Neighbor);
 if (Rule->Macro > "") Item->ImageIndex = -1;
 else Item->ImageIndex = 0;
 Item->Caption = Rule->Name;
};
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acNewAExecute(TObject *Sender)
{
 if (Speech->ARules->FindRule(edASymb->Text, edAMask->Text) != NULL)
 {
  meMsg->Lines->Add("Пост-правило для " +edAMask->Text+edASymb->Text+ " уже есть!");
  return;
 }
 TSpeechRule *Rule;
 TListItem   *Item;
 int itr = 0;
 AnsiString RCaption, R;
 if (edAMask->Text > " ") RCaption = edAMask->Text;
 else RCaption = "_";
 RCaption = RCaption + edASymb->Text;
 R = RCaption;
 while (lvARules->FindCaption(0, R, false, true, false))
 { R = RCaption + AnsiString(itr); itr++; }
 Rule = Speech->ARules->CreateRule(R, edASymb->Text, edAMask->Text);
 Item = lvARules->Items->Add();
 AssignRule(Item, Rule);
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acNewBExecute(TObject *Sender)
{
 if (Speech->BRules->FindRule(edBSymb->Text, edBMask->Text) != NULL)
 {
  meMsg->Lines->Add("Пред-правило для " +edBSymb->Text+edBMask->Text+ " уже есть!");
  return;
 }
 TSpeechRule *Rule;
 TListItem   *Item;
 int itr = 0;
 AnsiString RCaption, R;
 RCaption = edBSymb->Text;
 if (edBMask->Text > " ") RCaption = RCaption + edBMask->Text;
 else RCaption = RCaption + "_";
 R = RCaption;
 while (lvBRules->FindCaption(0, R, false, true, false))
 { R = RCaption + AnsiString(itr); itr++; }
 Rule = Speech->BRules->CreateRule(R, edBSymb->Text, edBMask->Text);
 Item = lvBRules->Items->Add();
 AssignRule(Item, Rule);
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acSaveAExecute(TObject *Sender)
{
 if (!ARule) return;
 ARule->Neighbor =  edAMask->Text;
 ARule->Symbol = edASymb->Text;
 if (lvARules->Selected)
 {
  ARule->Name = lvARules->Selected->Caption;
  AssignRule(lvARules->Selected, ARule);
 } 
 Speech->ARules->PostRule(ARule);
 acActivateLeft->Execute();
 acSaveMacro->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acDelAExecute(TObject *Sender)
{
 TListItem *Item;
 int i = 0;
 while ( i < lvARules->Items->Count )
 {
  Item = lvARules->Items->Item[i];
  if (Item->Selected)
  {
   Speech->ARules->Delete(Item->Caption);
   Item->Delete();
  } else i++;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acCreateTriftExecute(TObject *Sender)
{
 int k, plen;
 AnsiString w, wrd = edWord->Text, Symbol, Macro, Neighbor;
 meMsg->Clear();
 lvARules->Selected = NULL;
 lvBRules->Selected = NULL;

 if ((plen = wrd.Length()) == 0) return;

 k = 1;
 Neighbor="";
 do {
  Neighbor = Neighbor+wrd[k++];
 } while ( k <= plen && (unsigned char)wrd[k] < 'A' && wrd[k] != ' ');
 Macro = " ";
 do
 {
  Symbol = Neighbor;
  Neighbor = Macro;
  if ( Speech->ARules->FindRule(Symbol, Neighbor) == NULL )
  {
   edASymb->Text = Symbol;
   edAMask->Text = Neighbor;
   acNewA->Execute();
   meMsg->Lines->Add("Not found A-rule \""+Neighbor+Symbol+"\" in phrase \""+wrd+"\"!");
  } else meMsg->Lines->Add("A-rule \""+Neighbor+Symbol+"\" in phrase \""+wrd+"\" exists!");

  if (k <= plen)
  {
   Neighbor="";
   do Neighbor = Neighbor+wrd[k++];
   while ( k <= plen && (unsigned char)wrd[k] < 'A' && wrd[k] != ' ');
  }
  else { Neighbor = " "; k++; }

  if ( Speech->BRules->FindRule(Symbol, Neighbor) == NULL )
  {
   meMsg->Lines->Add("Not found B-rule \""+Symbol+Neighbor+"\" in phrase \""+wrd+"\"");
   edBSymb->Text = Symbol;
   edBMask->Text = Neighbor;
   acNewB->Execute();
  } else meMsg->Lines->Add("B-rule \""+Symbol+Neighbor+"\" in phrase \""+wrd+"\" exists!");
  Macro = Symbol;
 } while ( k <= plen + 1);
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acSaveMacroExecute(TObject *Sender)
{
 if (!ActivePage->Selected) return;
 if (edMacro->Text > "") ActivePage->Selected->ImageIndex = -1;
 else ActivePage->Selected->ImageIndex = 0;
 if (ActivePage == lvARules) Speech->ARules->SetMacro(ActivePage->Selected->Caption, edMacro->Text);
 if (ActivePage == lvBRules) Speech->BRules->SetMacro(ActivePage->Selected->Caption, edMacro->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::edMacroKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_RETURN) { acSaveMacro->Execute(); edMacro->SelectAll(); }
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acActivateLeftExecute(TObject *Sender)
{
 ActivePage = lvARules;
 Panel7->Color = clHighlight;
 Panel8->Color = clBtnFace;
 lvBRules->Selected = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acActivateRightExecute(TObject *Sender)
{
 ActivePage = lvBRules;
 Panel8->Color = clHighlight;
 Panel7->Color = clBtnFace;
 lvARules->Selected = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvARulesEdited(TObject *Sender,
      TListItem *Item, AnsiString &S)
{
 Speech->ARules->Rename(Item->Caption, S);
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvRulesColumnClick(TObject *Sender,
      TListColumn *Column)
{
 ActivePage->SortType = Listactns::stNone;
 SortColumn = Column->Index;
 ActivePage->SortType = Listactns::stData;
 ActivePage->AlphaSort();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvRulesCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
 if (SortColumn < 0) return;
 switch (SortColumn) {
  case 0: Compare = Item1->Caption.AnsiCompare(Item2->Caption); break;
  default: Compare =Item1->SubItems->Strings[SortColumn - 1].AnsiCompare(Item2->SubItems->Strings[SortColumn - 1]); break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acSaveBExecute(TObject *Sender)
{
 if (!BRule) return;
 BRule->Neighbor =  edBMask->Text;
 BRule->Symbol = edBSymb->Text;
 if (lvBRules->Selected)
 {
  BRule->Name = lvBRules->Selected->Caption;
  AssignRule(lvBRules->Selected, BRule);
 } 
 Speech->BRules->PostRule(BRule);
 acActivateRight->Execute();
 acSaveMacro->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::acDelBExecute(TObject *Sender)
{
 TListItem *Item;
 int i = 0;
 while ( i < lvBRules->Items->Count )
 {
  Item = lvBRules->Items->Item[i];
  if (Item->Selected)
  {
   Speech->BRules->Delete(Item->Caption);
   Item->Delete();
  } else i++;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvARulesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_DELETE && !lvARules->IsEditing()) acDelA->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvBRulesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_DELETE && !lvBRules->IsEditing()) acDelB->Execute();
 if (Key == VK_TAB) lvARules->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvARulesChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
 if (!lvARules->Selected) return;
 edMacro->Text = Speech->ARules->GetMacro(lvARules->Selected->Caption);
 switch (lvARules->Selected->SubItems->Count) {
  case 2: edAMask->Text = lvARules->Selected->SubItems->Strings[1];
  case 1: edASymb->Text = lvARules->Selected->SubItems->Strings[0];
 }
 ARule = Speech->ARules->FindRule(edASymb->Text, edAMask->Text);
 if (!ARule) {
  acDelA->Enabled = false;
  acSaveA->Enabled = false;
 } else {
  acDelA->Enabled = true;
  acSaveA->Enabled = true;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvBRulesChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
 if (!lvBRules->Selected) return;
 edMacro->Text = Speech->BRules->GetMacro(lvBRules->Selected->Caption);
 switch (lvBRules->Selected->SubItems->Count) {
  case 2: edBMask->Text = lvBRules->Selected->SubItems->Strings[1];
  case 1: edBSymb->Text = lvBRules->Selected->SubItems->Strings[0];
 }
 BRule = Speech->BRules->FindRule(edBSymb->Text, edBMask->Text);
 if (!BRule) {
  acDelB->Enabled = false;
  acSaveB->Enabled = false;
 } else {
  acDelB->Enabled = true;
  acSaveB->Enabled = true;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::lvBRulesEdited(TObject *Sender,
      TListItem *Item, AnsiString &S)
{
 Speech->BRules->Rename(Item->Caption, S);
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::edAMaskKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_RETURN) acSaveA->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::edBMaskKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_RETURN) acSaveB->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmMorphMain::edWordKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_RETURN) { acCreateTrift->Execute(); edWord->SelectAll(); }
}
//---------------------------------------------------------------------------

