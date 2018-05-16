//---------------------------------------------------------------------------
#ifndef playH
#define playH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RXCtrls.hpp"
//---------------------------------------------------------------------------
class TfmPlay : public TForm
{
__published:	// IDE-managed Components
        TRxSpeedButton *RxSpeedButton1;
        TRxSpeedButton *RxSpeedButton2;
private:	// User declarations
public:		// User declarations
        __fastcall TfmPlay(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmPlay *fmPlay;
//---------------------------------------------------------------------------
#endif
