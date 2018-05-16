//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "quest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmQuest *fmQuest;
//---------------------------------------------------------------------------
__fastcall TfmQuest::TfmQuest(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
int __fastcall TfmQuest::Execute(const AnsiString aText)
{
 StaticText1->Caption = aText;
 return ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfmQuest::FormClose(TObject *Sender, TCloseAction &Action)
{
 Action = caFree;        
}
//---------------------------------------------------------------------------

