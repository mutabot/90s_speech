//---------------------------------------------------------------------------
#ifndef questH
#define questH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfmQuest : public TForm
{
__published:	// IDE-managed Components
        TButton *btOk;
        TButton *btCancel;
        TStaticText *StaticText1;
        TButton *Button1;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations

public:		// User declarations
        int __fastcall Execute(const AnsiString aText);
        __fastcall TfmQuest(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmQuest *fmQuest;
//---------------------------------------------------------------------------
#endif
