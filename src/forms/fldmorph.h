//---------------------------------------------------------------------------
#ifndef fldmorphH
#define fldmorphH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "tspeech.h"

#include <ActnList.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TfmMorphMain : public TForm
{
__published:	// IDE-managed Components
        TActionList *Actions;
        TAction *DoMorph;
        TAction *acNewA;
        TAction *acDelA;
        TAction *acSaveA;
        TAction *acCreateTrift;
        TImageList *ilFlash;
        TAction *acSaveMacro;
        TImageList *ilButtons;
        TAction *acActivateLeft;
        TAction *acActivateRight;
        TAction *acNewB;
        TAction *acDelB;
        TAction *acSaveB;
        TPanel *Panel3;
        TEdit *edMacro;
        TEdit *edWord;
        TPanel *Panel1;
        TPanel *Panel7;
        TPanel *Panel9;
        TEdit *edAMask;
        TEdit *edASymb;
        TListView *lvARules;
        TPanel *Panel8;
        TListView *lvBRules;
        TPanel *Panel10;
        TEdit *edBMask;
        TEdit *edBSymb;
        TMemo *meMsg;
        TSplitter *Splitter1;
        TSplitter *Splitter2;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton6;
        TStaticText *StaticText3;
        TStaticText *StaticText4;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
    TCSpinEdit *seLen;
        TButton *Button1;
        TButton *btInsSeg;
        TStaticText *StaticText8;
    TEdit *edSegment;
        TStaticText *StaticText9;
        TBitBtn *edSegmentButton;
    TCSpinEdit *seAdvLen;
    TStaticText *StaticText10;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall edSegmentButtonClick(TObject *Sender);
        void __fastcall btInsSegClick(TObject *Sender);
        void __fastcall acNewAExecute(TObject *Sender);
        void __fastcall acSaveAExecute(TObject *Sender);
        void __fastcall acDelAExecute(TObject *Sender);
        void __fastcall acCreateTriftExecute(TObject *Sender);
        void __fastcall acSaveMacroExecute(TObject *Sender);
        void __fastcall edMacroKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall acActivateRightExecute(TObject *Sender);
        void __fastcall lvARulesEdited(TObject *Sender, TListItem *Item,
          AnsiString &S);
        void __fastcall lvRulesColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall acNewBExecute(TObject *Sender);
        void __fastcall acActivateLeftExecute(TObject *Sender);
        void __fastcall acSaveBExecute(TObject *Sender);
        void __fastcall acDelBExecute(TObject *Sender);
        void __fastcall lvARulesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lvBRulesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lvBRulesChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
        void __fastcall lvBRulesEdited(TObject *Sender, TListItem *Item,
          AnsiString &S);
        void __fastcall lvRulesCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
        void __fastcall edAMaskKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall edBMaskKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall edWordKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lvARulesChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
private:	// User declarations
        TSpeech *Speech;
        TFoldStore *FoldStore;
        int SortColumn;
        AnsiString FCurDir;
        TListView *ActivePage;
        TSpeechRule *ARule, *BRule;
        void __fastcall CreateParams(TCreateParams& Params);
        void __fastcall AssignRule(TListItem *Item, TSpeechRule *Rule);
        void __fastcall Init( void );
public:		// User declarations
        __fastcall TfmMorphMain(TSpeech *ASpeech, TComponent* Owner);
        __fastcall TfmMorphMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMorphMain *fmMorphMain;
//---------------------------------------------------------------------------
#endif
 