//---------------------------------------------------------------------------
#ifndef foldmanH
#define foldmanH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfmFoldMan : public TForm
{
__published:	// IDE-managed Components
        TCheckBox *cbOnTop;
        TComboBox *cbSignals;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TButton *btPlain;
    TButton *btSeg;
    TStaticText *StaticText3;
    TCSpinEdit *seMorLen;
    TSaveDialog *fnResult;
    TEdit *edDstFile;
    TBitBtn *btDstBrowse;
    TSpeedButton *btStart;
        void __fastcall cbOnTopClick(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btPlainClick(TObject *Sender);
        void __fastcall btSegClick(TObject *Sender);
    void __fastcall btDstBrowseClick(TObject *Sender);
private:	// User declarations
        TFileStream *fsFold;
        void __fastcall CreateParams(TCreateParams& Params);
public:		// User declarations
   void __fastcall AddSignal(TForm * Signal, bool Active);
        __fastcall TfmFoldMan(TComponent* Owner);
    __fastcall ~TfmFoldMan();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmFoldMan *fmFoldMan;
//---------------------------------------------------------------------------
#endif
