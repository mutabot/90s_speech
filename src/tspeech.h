//---------------------------------------------------------------------------
#ifndef tspeechH
#define tspeechH

#include <inifiles.hpp>
#include "morph.h"
#include "sptecdef.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TFOnError)(const AnsiString& ErrorMsg);
typedef void __fastcall (__closure *TFOnMacros)(TStrings *Macros);
struct TSpeechRule
{
   AnsiString Name;
   AnsiString Symbol;
   AnsiString Neighbor;
   AnsiString Macro;
};
struct THiRule
{
   AnsiString Text;
   int        Pos;
   AnsiString Symbol;
};
//---------------------------------------------------------------------------
struct TRuleIdx
{
 THiRule* Rule;
 int      from;
};
int __fastcall RuleIdxCompare(void * Item1, void * Item2)
{
 int i1 = ((TRuleIdx*)Item1)->from, i2 = ((TRuleIdx*)Item2)->from;
 if (i1 == i2) return ((TRuleIdx*)Item1)->Rule->Symbol.Length() - ((TRuleIdx*)Item2)->Rule->Symbol.Length();
 else return i1 - i2;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSpeechRules : public TList
{
 TIniFile          *Storage;
 void __fastcall UpdateItems(void);

public:
    __fastcall TSpeechRules(const AnsiString StorageName);
    __fastcall ~TSpeechRules();
    TSpeechRule* __fastcall CreateRule(const AnsiString Name,
                                       const AnsiString Symbol,
                                       const AnsiString Neighbor);
   void __fastcall PostRule(TSpeechRule* Rule);
   void __fastcall SetMacro(const AnsiString RuleName, const AnsiString Macro);
   AnsiString __fastcall GetMacro(const AnsiString RuleName);
   void __fastcall Rename(const AnsiString RuleName, const AnsiString NewName);
   void __fastcall Delete(const AnsiString RuleName);
   void __fastcall Clear(void);
   TSpeechRule* __fastcall FindRule(const AnsiString Symbol, const AnsiString Neighbor);
};
//---------------------------------------------------------------------------
class THiRules : public TList
{
   TIniFile *Storage;
   void __fastcall UpdateItems(void);
public:
    __fastcall THiRules(const AnsiString StorageName);
    __fastcall ~THiRules();
    void __fastcall Clear(void);
    THiRule* __fastcall CreateRule(const AnsiString Text, int Pos, const AnsiString Symbol);
    void __fastcall Delete(const AnsiString Text);
    void __fastcall PostRule(const AnsiString Text, int Pos, const AnsiString Symbol);
    THiRule* __fastcall GetRule(const AnsiString Text);
    void __fastcall ConvertPhrase(AnsiString& Phrase);
};
//---------------------------------------------------------------------------
class TSpeech
{
   TMorph  *Morph;
   void __fastcall MorphemToMacros(TStrings *Morphem, TStrings *Macros);
   void __fastcall TextToMorphem(TStrings *Text, TStrings *Morphem);
public:
    TFoldStore *FoldStore;
    TSpeechRules *ARules;
    TSpeechRules *BRules;
    THiRules     *HiRules;
    TFOnError OnError;
    TFOnMacros OnMacros;
    TFOnMacros OnMorphem;
    __fastcall TSpeech(TFoldStore *AFoldStore, const AnsiString RulesDir);
    __fastcall TSpeech(TFoldStore *AFoldStore);
    __fastcall ~TSpeech();
    void __fastcall Say(TStrings *Text, TStream *Speech);
};
//---------------------------------------------------------------------------
#endif
