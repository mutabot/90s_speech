//---------------------------------------------------------------------------
#ifndef rulemapH
#define rulemapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "tspeech.h"
//---------------------------------------------------------------------------
class TfmRuleMap : public TForm
{
__published:	// IDE-managed Components
        void __fastcall RGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall RGridDblClick(TObject *Sender);
private:	// User declarations
        TSpeechRules *ARules, *BRules;
        TStringList  *Symbols;
        void __fastcall CreateParams(TCreateParams& Params);
public:		// User declarations
        __fastcall TfmRuleMap(TSpeechRules *ARules, TSpeechRules *BRules, TComponent* Owner);
public:
    void __fastcall UpdateRules();
    __fastcall ~TfmRuleMap();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmRuleMap *fmRuleMap;
//---------------------------------------------------------------------------
#endif
