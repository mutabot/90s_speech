//---------------------------------------------------------------------------
#ifndef fldextrH
#define fldextrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <math.h>
#include <Mask.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "extruder.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfmFldExtr : public TForm, public TExtrudeEvents
{
__published:	// IDE-managed Components
        TButton *Button3;
        TProgressBar *pbProgress;
    TEdit *edSrcFile;
    TBitBtn *btBrowse;
    TEdit *edDstFile;
    TBitBtn *btDstBrowse;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TOpenDialog *fnFold;
    TSaveDialog *fnResult;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall fnFoldAfterDialog(TObject *Sender,
          AnsiString &Name, bool &Action);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btBrowseClick(TObject *Sender);
    void __fastcall btDstBrowseClick(TObject *Sender);
private:	// User declarations
        TFileStream *fsFold;
        TFileStream *Result;
        TExtruder *Extruder;
        unsigned char pcm_mod(unsigned char value)
        {
         if (value & 0x80) return value&0x7F;
         else return 0x80 - value;
        }
        int OnBufferReady(short *buf, int size);
        int OnDone(void) { return 0; };
        void __fastcall CreateParams(TCreateParams& Params);
public:		// User declarations
        __fastcall TfmFldExtr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmFldExtr *fmFldExtr;
//---------------------------------------------------------------------------
#endif
