//---------------------------------------------------------------------------
#ifndef hirulesH
#define hirulesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "tspeech.h"
#include <ActnList.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfmHiRules : public TForm
{
__published:	// IDE-managed Components
        TListView *lvHiRules;
        TStatusBar *StatusBar1;
        TPanel *Panel1;
        TEdit *edText;
        TEdit *edSymbol;
        TActionList *ActionList;
        TAction *acNewR;
        TAction *acSaveR;
        TAction *acDelR;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton3;
    TStaticText *StaticText1;
    TCSpinEdit *sePos;
    TStaticText *StaticText2;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall acNewRExecute(TObject *Sender);
        void __fastcall acSaveRExecute(TObject *Sender);
        void __fastcall acDelRExecute(TObject *Sender);
        void __fastcall lvHiRulesChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
        void __fastcall edTextKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lvHiRulesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
        TSpeech *Speech;
        void __fastcall CreateParams(TCreateParams& Params);
        void __fastcall UpdateItems( void );
public:		// User declarations
        __fastcall TfmHiRules(TSpeech *ASpeech, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmHiRules *fmHiRules;
//---------------------------------------------------------------------------
#endif
