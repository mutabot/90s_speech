//---------------------------------------------------------------------------
#ifndef extrH
#define extrH
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
//---------------------------------------------------------------------------
class TfmExtrude : public TForm
{
__published:	// IDE-managed Components
        TButton *Button3;
        TFilenameEdit *fnFold;
        TFilenameEdit *fnResult;
        TRxLabel *RxLabel1;
        TRxLabel *RxLabel2;
        TProgressBar *pbProgress;
        TRxLabel *RxLabel3;
        TBevel *Bevel1;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall fnFoldAfterDialog(TObject *Sender,
          AnsiString &Name, bool &Action);
private:	// User declarations
        TFileStream *fsFold;
        TFileStream *Result;
        TExtruder *Extruder;
        unsigned char pcm_mod(unsigned char value)
        {
         if (value & 0x80) return value&0x7F;
         else return 0x80 - value;
        }
        void __fastcall OnExtrude(unsigned char *buf, int size);

public:		// User declarations
        __fastcall TfmExtrude(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmExtrude *fmExtrude;
//---------------------------------------------------------------------------
#endif
