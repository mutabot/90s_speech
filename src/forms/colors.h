//---------------------------------------------------------------------------
#ifndef colorsH
#define colorsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
struct TColors
{
 TColor       Bk;
 TColor       Fold;
 TColor       PCM;
};
//---------------------------------------------------------------------------
class TfmColor : public TForm
{
__published:	// IDE-managed Components
        TButton *btOk;
        TButton *btCancel;
        TColorBox *cbPCM;
        TColorBox *cbBk;
        TColorBox *cbFold;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        void __fastcall btCancelClick(TObject *Sender);
        void __fastcall btOkClick(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TColors *Colors;
        __fastcall TfmColor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmColor *fmColor;
//---------------------------------------------------------------------------
#endif
