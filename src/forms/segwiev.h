//---------------------------------------------------------------------------
#ifndef segwievH
#define segwievH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sptecdef.h"
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TfmSegView : public TForm
{
__published:	// IDE-managed Components
        TFormPlacement *FormPlacement1;
        void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
        int ffold_size;
public:		// User declarations
        TFPoint fbuf[SEG_LENGTH];
        __fastcall TfmSegView(int fold size, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSegView *fmSegView;
//---------------------------------------------------------------------------
#endif
