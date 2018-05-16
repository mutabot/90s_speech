//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "tspeech.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TSpeech::TSpeech(TFoldStore *AFoldStore, const AnsiString RulesDir)
{
 FoldStore = AFoldStore;
 ARules= new TSpeechRules(RulesDir + "\\arules.ini");
 BRules= new TSpeechRules(RulesDir + "\\brules.ini");
 HiRules = new THiRules(RulesDir + "\\hirules.ini");
 OnError = NULL;
 OnMacros = NULL;
 OnMorphem = NULL;
}
//---------------------------------------------------------------------------
__fastcall TSpeech::TSpeech(TFoldStore *AFoldStore)

{
 FoldStore = AFoldStore;
 ARules= new TSpeechRules("arules.ini");
 BRules= new TSpeechRules("brules.ini");
 HiRules = new THiRules("hirules.ini");
 OnError = NULL;
 OnMacros = NULL;
 OnMorphem = NULL;
}
//---------------------------------------------------------------------------
__fastcall TSpeech::~TSpeech()
{
 delete HiRules;
 delete BRules;
 delete ARules;
}
//---------------------------------------------------------------------------
void __fastcall TSpeech::TextToMorphem(TStrings *Text, TStrings *Morphem)
{
 AnsiString Phrase;
 for (int i = 0; i < Text->Count; i++)
 {
  Phrase = Text->Strings[i];
  HiRules->ConvertPhrase(Phrase);
  Morphem->Add(Phrase);
 }
}
//---------------------------------------------------------------------------
void __fastcall TSpeech::MorphemToMacros(TStrings *Morphem, TStrings *Macros)
{
 AnsiString Phrase, Macro, Symbol, Neighbor;
 TSpeechRule *Rule;
 int i, k, plen;
 for (i = 0; i < Morphem->Count; i++)
 {
  Phrase = Morphem->Strings[i];
  if ((plen = Phrase.Length()) == 0) continue;

  k = 1;
  Neighbor=" ";
  do
  {
   if (Neighbor <= " ")
   {
    Symbol = "";
    do {
     Symbol = Symbol+Phrase[k++];
    } while ( k <= plen && (unsigned char)Phrase[k] < 'A' && Phrase[k] != ' ');
    Neighbor = " ";
   } else {
    Symbol = Neighbor;
    Neighbor = Macro;
   }
   Macro = "";
   if ( ((Rule = ARules->FindRule(Symbol, Neighbor)) == NULL) && OnError)
    OnError("Post-rule not found: "+ Neighbor+Symbol+" position: "
                                     +AnsiString(k)+" phrase: " + Phrase);
   else Macro = Rule->Macro;

   if (k <= plen)
   {
    Neighbor="";
    do Neighbor = Neighbor+Phrase[k++];
    while ( k <= plen && (unsigned char)Phrase[k] < 'A' && Phrase[k] != ' ');
   }
   else { Neighbor = " "; k++; }

   if ( ((Rule = BRules->FindRule(Symbol, Neighbor)) == NULL)  && OnError)
    OnError("Pre-rule not found: "+Symbol+Neighbor+" position: "
                                     +AnsiString(k)+" phrase: " + Phrase);
   else Macro = Macro + Rule->Macro;
   if (Macro > "") Macros->Add(Macro);
   Macro = Symbol;
  } while ( k <= plen + 1);
 }
}
//---------------------------------------------------------------------------
void __fastcall TSpeech::Say(TStrings *Text, TStream *Speech)
{
 if (FoldStore->Count == 0) { if ( OnError ) OnError("Segment base is empty!"); return; }

 int i, j, k, stretch;
 if (FoldStore) Morph = new TMorph(FoldStore, Speech);
 TStringList *Macros = new TStringList();
 TStringList *Morphem = new TStringList();
 AnsiString Line, Param[5];
 TSample Sample;
 TextToMorphem(Text, Morphem);
 if (OnMorphem) OnMorphem(Morphem);
 MorphemToMacros(Morphem, Macros);
 if (OnMacros) OnMacros(Macros);
 for (i = 0; i < Macros->Count; i++)
 {
  Line = Macros->Strings[i];
  for (k = 4; k >= 0; k--) Param[k] = "";
  k++;
  for (j = 1; j <= Line.Length(); j++)
  {
   switch (Line[j]) {
    case ',': k++; break;
    case ';':
     Param[0] = Param[0].Trim();
     if (Param[0].Length() < 2)
     {
      if ( OnError ) OnError("Line:" + AnsiString(i)+ " -- invalid segment!");
      for (k = 4; k >= 0; k--) Param[k] = ""; k++;
      break;
     }
     Sample.sound = Param[0][1] - BASE_CHAR_0;
     Param[0] = Param[0].SubString(2, Param[0].Length() - 1);
     Sample.tone = Param[0].ToIntDef(255);
     if (Param[2] > "")
     {
      stretch = Param[2].Trim().ToIntDef(-1);
      if (stretch < 0) {
       if ( OnError ) OnError("Line:" + AnsiString(i)+ " -- invalid strech parameter!");
       for (k = 4; k >= 0; k--) Param[k] = ""; k++;
       break;
      }
      Morph->LengthDev = stretch;
     }
     Morph->LoadSeg(&Sample);

     Morph->Morph(Param[1].Trim().ToIntDef(1));
     for (k = 4; k >= 0; k--) Param[k] = ""; k++;
    break;
    default :  Param[k] = Param[k] + Line[j]; break;
   }
  }
 }
 delete Morphem;
 delete Macros;
 delete Morph;
}
//---------------------------------------------------------------------------
__fastcall TSpeechRules::TSpeechRules(const AnsiString StorageName) : TList()
{
 Storage = new TIniFile(StorageName);
 UpdateItems();
}
//---------------------------------------------------------------------------
__fastcall TSpeechRules::~TSpeechRules()
{
 Clear();
 delete Storage;
}
//---------------------------------------------------------------------------
TSpeechRule* __fastcall TSpeechRules::FindRule(const AnsiString Symbol, const AnsiString Neighbor)
{
 TSpeechRule *Rule;
 AnsiString N, M;
 TList *FitRules = new TList();
 int i, j, k, l, m, ml, nl;
 for (i = 0; i < Count; i++)
 {
  Rule = (TSpeechRule *)Items[i];
  if (Symbol != Rule->Symbol) continue;
  if (Neighbor < " ") N = " ";
  else N = Neighbor;
  if (Rule->Neighbor < " ") M = " ";
  else M = Rule->Neighbor;
  j = 1;
  nl = N.Length();
  ml = M.Length();
  while ( j <= nl )
  {
   k = 1;
   while (j+k <= nl && (unsigned)N[j+k] < 'A' && N[j+k] != ' ') k++;
   if ((l = M.Pos(N.SubString(j, k))) != 0)
   {
    m = 1;
    while (l+m <= ml && (unsigned)M[l+m] < 'A' && M[l+m] != ' ') m++;
    if (m == k) FitRules->Add(Rule);
   }
   j+=k;
  }
 }
 Rule = NULL;
 for (int i = 0; i < FitRules->Count; i++)
 {
  Rule = (TSpeechRule*)FitRules->Items[i];
  if (Rule->Macro > "") break;
 }
 delete FitRules;
 return Rule;
}
//---------------------------------------------------------------------------
void __fastcall TSpeechRules::UpdateItems(void)
{
 TSpeechRule *Rule;
 TStringList *RNames = new TStringList();
 Storage->ReadSections(RNames);
 Clear();
 for (int i = 0; i < RNames->Count; i++)
 {
  Rule = new TSpeechRule;
  Rule->Name = RNames->Strings[i];
  Rule->Symbol = Storage->ReadString(RNames->Strings[i], "Symbol", "");
  Rule->Neighbor = Storage->ReadString(RNames->Strings[i], "Neighbor", "");
  Rule->Macro = Storage->ReadString(RNames->Strings[i], "Macro", "");
  Add(Rule);
 }
 delete RNames;
}
//---------------------------------------------------------------------------
void __fastcall TSpeechRules::Clear(void)
{
 for (int i = 0; i < Count; i++) delete (TSpeechRule*)Items[i];
 TList::Clear();
}
//---------------------------------------------------------------------------
TSpeechRule* __fastcall TSpeechRules::CreateRule(const AnsiString Name,
                                                 const AnsiString Symbol,
                                                 const AnsiString Neighbor)
{
 TSpeechRule* Rule = new TSpeechRule;
 Rule->Name = Name;
 Rule->Symbol = Symbol;
 Rule->Neighbor = Neighbor;
 Add(Rule);
 PostRule(Rule);
 return Rule;
}
//---------------------------------------------------------------------------
void __fastcall TSpeechRules::PostRule(TSpeechRule* Rule)
{
 Storage->EraseSection(Rule->Name);
 Storage->WriteString(Rule->Name, "Symbol", Rule->Symbol);
 Storage->WriteString(Rule->Name, "Neighbor", Rule->Neighbor);
 Storage->WriteString(Rule->Name, "Macro", Rule->Macro);
 Storage->UpdateFile();
}
//---------------------------------------------------------------------------
void __fastcall TSpeechRules::SetMacro(const AnsiString RuleName, const AnsiString Macro)
{
 TSpeechRule *Rule;
 Storage->WriteString(RuleName, "Macro", Macro);
 for (int i = 0; i < Count; i++)
 {
  Rule = (TSpeechRule *)Items[i];
  if (Rule->Name == RuleName) Rule->Macro = Macro;
 }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TSpeechRules::GetMacro(const AnsiString RuleName)
{
 return Storage->ReadString(RuleName, "Macro", "");
}
//---------------------------------------------------------------------------
void __fastcall TSpeechRules::Delete(const AnsiString RuleName)
{
 Storage->EraseSection(RuleName);

 TSpeechRule *Rule;
 int i = 0;
 while(i < Count)
 {
  Rule = (TSpeechRule *)Items[i];
  if (Rule->Name == RuleName)
  {
   delete Rule;
   TList::Delete(i);
  } else i++;
 }
}
//---------------------------------------------------------------------------
void __fastcall TSpeechRules::Rename(const AnsiString RuleName, const AnsiString NewName)
{
 TStringList *Tmp = new TStringList();
 Storage->ReadSectionValues(RuleName, Tmp);
 Storage->EraseSection(RuleName);
 for (int i =0; i < Tmp->Count; i++) Storage->WriteString(NewName, Tmp->Names[i], Tmp->Values[Tmp->Names[i]]);
 delete Tmp;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall THiRules::THiRules(const AnsiString StorageName) : TList()
{
 Storage = new TIniFile(StorageName);
 UpdateItems();
}
//---------------------------------------------------------------------------
__fastcall THiRules::~THiRules()
{
 delete Storage;
}
//---------------------------------------------------------------------------
void __fastcall THiRules::UpdateItems(void)
{
 THiRule *Rule;
 TStringList *RNames = new TStringList();
 Storage->ReadSections(RNames);
 Clear();
 for (int i = 0; i < RNames->Count; i++)
 {
  Rule = new THiRule;
  Rule->Text = RNames->Strings[i];
  Rule->Pos = Storage->ReadInteger(RNames->Strings[i], "Pos", 0);
  Rule->Symbol = Storage->ReadString(RNames->Strings[i], "Symbol", "");
  Add(Rule);
 }
 delete RNames;
}
//---------------------------------------------------------------------------
void __fastcall THiRules::Clear(void)
{
 for (int i = 0; i < Count; i++) delete (THiRule*)Items[i];
 TList::Clear();
}
//---------------------------------------------------------------------------
void __fastcall THiRules::PostRule(const AnsiString Text, int Pos, const AnsiString Symbol)
{
 Storage->WriteString(Text, "Symbol", Symbol);
 Storage->WriteInteger(Text, "Pos", Pos);
}
//---------------------------------------------------------------------------
THiRule* __fastcall THiRules::GetRule(const AnsiString Text)
{
 THiRule* Rule = NULL;
 for (int i = 0; i < Count; i++)
 {
  Rule = (THiRule* )Items[i];
  if (Rule->Text == Text) break;
 }
 return Rule;
}
//---------------------------------------------------------------------------
THiRule* __fastcall THiRules::CreateRule(const AnsiString Text, int Pos, const AnsiString Symbol)
{
 if (Storage->SectionExists(Text)) return NULL;
 THiRule* Rule = new THiRule;
 Rule->Text = Text.LowerCase();
 Rule->Symbol = Symbol;
 Rule->Pos = Pos;
 Add(Rule);
 PostRule(Text, Pos, Symbol);
 return Rule;
}
//---------------------------------------------------------------------------
void __fastcall THiRules::Delete(const AnsiString Text)
{
 for (int i = 0; i < Count; i++)
  if ( ((THiRule*)Items[i])->Text == Text )
  {
   delete (THiRule*)Items[i];
   TList::Delete(i);
   break;
  }
 Storage->EraseSection(Text);
}
//---------------------------------------------------------------------------
void __fastcall THiRules::ConvertPhrase(AnsiString& Phrase)
{
 TRuleIdx* RuleIdx;
 AnsiString phr = Phrase.LowerCase();
 int i, j, k, tlen, plen = Phrase.Length();
 THiRule* Rule;
 TList *RuleList = new TList();
 for (i = 0; i < Count; i++)
 {
  Rule = (THiRule*)Items[i];
  tlen = Rule->Text.Length();
  for (j = 1; j <= plen; j++)
  {
   for (k = 0; j + k <= plen && k < tlen; k++)
   {
    if (Rule->Text[k+1] == '*') continue;
    if (phr[j + k] != Rule->Text[k+1]) break;
   }
   if (k == tlen) // found rule
   {
    RuleIdx = new TRuleIdx;
    RuleIdx->from = j + Rule->Pos - 1;
    RuleIdx->Rule = Rule;
    RuleList->Add(RuleIdx);
   }
  }
 }
 RuleList->Sort(RuleIdxCompare);
 j = 1;
 for (i = 0; i < RuleList->Count; i++)
 {
  RuleIdx = (TRuleIdx*)RuleList->Items[i];
  Phrase.Insert(RuleIdx->Rule->Symbol, RuleIdx->from + j);
  j+=RuleIdx->Rule->Symbol.Length();
  delete RuleIdx;
 }
 delete RuleList;
}
//---------------------------------------------------------------------------
