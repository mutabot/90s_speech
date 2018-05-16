//---------------------------------------------------------------------------
#ifndef segviewH
#define segviewH

#define SEG_BORDER    8                                                       
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sptecdef.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfmSegView : public TForm
{
__published:	// IDE-managed Components
        TPaintBox *pbSeg;
        TPaintBox *pbScale;
        TPanel *pnCaption;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall pbSegPaint(TObject *Sender);
        void __fastcall pbScalePaint(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
        int ffold_size, swidth;
        double scale[2];
public:		// User declarations
        TFPoint fbuf[SEG_LENGTH];
        void __fastcall Show(int fold_size)
        {
         ffold_size = fold_size;
         TForm::Show();
        };
        __fastcall TfmSegView(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSegView *fmSegView;
//---------------------------------------------------------------------------
#endif
