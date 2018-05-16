//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "tts.h"
#include "extruder.h"
#include "rulemap.h"
#include "fldmorph.h"
#include "hirules.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmTTS *fmTTS;
//---------------------------------------------------------------------------
__fastcall TfmTTS::TfmTTS(TSpeech *ASpeech, TComponent* Owner)
        : TForm(Owner)
{
 Speech = ASpeech;
 Speech->OnError = OnError;
 Speech->OnMorphem = OnMorphem;
 Speech->OnMacros = OnMacros;
 fmRuleMap = NULL;
 fmMorphMain = NULL;
 fmHiRules = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::DoMorphExecute(TObject *Sender)
{
 AnsiString FileName;
 mText->Lines->SaveToFile("morph.txt");
 meMsg->Clear();
 if (feResult->FilterIndex == 1) FileName = feResult->FileName;
 else FileName = "~morph~.fld";

 TFileStream *fsResult;
 try
 {
  fsResult = new TFileStream(FileName, fmCreate|fmShareDenyNone);
 } catch ( ... ) {
  fsResult = new TFileStream(FileName, fmOpenWrite|fmShareDenyNone);
  fsResult->Size = 0;
 }
 Speech->Say(mText->Lines, fsResult);

 if (feResult->FilterIndex == 1) { delete fsResult; return; }

 TFileStream *fsPCMResult = new TFileStream(feResult->FileName, fmCreate|fmShareDenyNone);
 TExtruder *Extruder = new TExtruder();
 fsResult->Position = 0;
 Extruder->Extrude(fsResult, fsPCMResult, -1);
 delete Extruder;
 delete fsPCMResult;
 delete fsResult;
 DeleteFile(FileName);
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::FormShow(TObject *Sender)
{
 mText->Lines->LoadFromFile("morph.txt");
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::FormClose(TObject *Sender, TCloseAction &Action)
{
 mText->Lines->SaveToFile("morph.txt");
 fmTTS = NULL;
 Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::acRuleMapExecute(TObject *Sender)
{
 if (!fmRuleMap) fmRuleMap = new TfmRuleMap(Speech->ARules, Speech->BRules, NULL);
 fmRuleMap->UpdateRules();
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::OnError(const AnsiString& ErrorMsg)
{
 meMsg->Lines->Add(ErrorMsg);
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::OnMorphem(TStrings *Macros)
{
 meMsg->Lines->Add("--- Преобразовано в фонемы --- ");
 meMsg->Lines->AddStrings(Macros);
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::OnMacros(TStrings *Macros)
{
 meMsg->Lines->Add("------ Готов макрос ------ ");
 meMsg->Lines->AddStrings(Macros);
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::acRuleEditExecute(TObject *Sender)
{
 if (!fmMorphMain) fmMorphMain = new TfmMorphMain(Speech, NULL);
 fmMorphMain->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::acHiRulesExecute(TObject *Sender)
{
 if (!fmHiRules) fmHiRules = new TfmHiRules(Speech, NULL);
 fmHiRules->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::FormDeactivate(TObject *Sender)
{
 mText->Lines->SaveToFile("morph.txt");
}
//---------------------------------------------------------------------------
void __fastcall TfmTTS::btDstBrowseClick(TObject *Sender)
{
    feResult->Execute();
    edDstFile->Text = feResult->FileName;
}
//---------------------------------------------------------------------------


