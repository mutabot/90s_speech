//---------------------------------------------------------------------------
#ifndef fstorewinH
#define fstorewinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "foldstore.h"
#include "CSPIN.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "TFlatButtonUnit.hpp"
#include "TFlatComboBoxUnit.hpp"
#include "TFlatEditUnit.hpp"
#include "TFlatHintUnit.hpp"
#include "TFlatSpinEditUnit.hpp"
#include "RxSpin.hpp"
#include "RXSpin.hpp"
//---------------------------------------------------------------------------
class TfmFSWin : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TComboBox *cbSound;
        TRxSpinEdit *edSpin;
        TEdit *edComment;
        TButton *btDel;
        TButton *btOk;
        TButton *btCancel;
        TButton *btMap;
        void __fastcall btCancelClick(TObject *Sender);
        void __fastcall btOkClick(TObject *Sender);
        void __fastcall btDelClick(TObject *Sender);
        void __fastcall btMapClick(TObject *Sender);
private:	// User declarations
        TFoldStore *FoldStore;
        bool btAction;
public:		// User declarations
        void __fastcall Write(TSample *Sample, TFPoint *buf);
        __fastcall TfmFSWin(TForm *Owner, TFoldStore *FS);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmFSWin *fmFSWin;
//---------------------------------------------------------------------------
#endif
