//---------------------------------------------------------------------------
#ifndef ttsH
#define ttsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <ActnList.hpp>
#include "tspeech.h"
#include <Menus.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfmTTS : public TForm
{
__published:	// IDE-managed Components
        TActionList *Actions;
        TAction *DoMorph;
        TPanel *Panel2;
        TMemo *mText;
        TPanel *Panel5;
        TMemo *meMsg;
        TSplitter *Splitter1;
        TAction *acRuleMap;
        TAction *acRuleEdit;
        TAction *acHiRules;
    TButton *btFire;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TBitBtn *btDstBrowse;
    TEdit *edDstFile;
    TSaveDialog *feResult;
    TMainMenu *MainMenu1;
    TMenuItem *PhonemeRules1;
    TMenuItem *Frontend1;
    TMenuItem *Rulesmap1;
        void __fastcall DoMorphExecute(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall acRuleMapExecute(TObject *Sender);
        void __fastcall acRuleEditExecute(TObject *Sender);
        void __fastcall acHiRulesExecute(TObject *Sender);
        void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall btDstBrowseClick(TObject *Sender);
private:	// User declarations
        TSpeech *Speech;
        void __fastcall CreateParams(TCreateParams& Params);
        void __fastcall OnError(const AnsiString& ErrorMsg);
        void __fastcall OnMacros(TStrings *Macros);
        void __fastcall OnMorphem(TStrings *Macros);
public:		// User declarations
        __fastcall TfmTTS(TSpeech *ASpeech, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmTTS *fmTTS;
//---------------------------------------------------------------------------
#endif
