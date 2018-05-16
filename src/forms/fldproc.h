//---------------------------------------------------------------------------
#ifndef fldprocH
#define fldprocH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "foldstore.h"
#include "fldprocess.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfmFldProc : public TForm
{
__published:	// IDE-managed Components
        TProgressBar *pbSearch;
        TPageControl *Pages;
        TTabSheet *TabSheet2;
        TComboBox *cbSignals;
        TPanel *Panel1;
        TButton *Button2;
        TButton *Button1;
        TTabSheet *tsReport;
        TTabSheet *TabSheet1;
        TCheckBox *cbAdapt;
        TRadioGroup *RadioGroup1;
        TListBox *lbReport;
        TLabel *Label1;
        void __fastcall cbBlockButtonClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall seAPassChange(TObject *Sender);
        void __fastcall feSegmentsAfterDialog(TObject *Sender,
          AnsiString &Name, bool &Action);
        void __fastcall seDevPassChange(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall btShowSegClick(TObject *Sender);
private:	// User declarations
        TFoldStore *FFoldStore;
        TFoldProcess *FoldProcess;
        TFileStream *fsResult;
        TFPoint fbuf[1024];
        TSample *sample;
        void __fastcall OnSegment(TFPoint *seg, int pcount, int offset, int dev);
    void __fastcall OnProgress(int Size, int Pos);
public:		// User declarations
        __fastcall TfmFldProc(TFoldStore *FoldStore, TComponent* Owner);
    void __fastcall AddSignal(TForm * Signal, bool Active);
    int __fastcall Execute();
    __fastcall ~TfmFldProc();
    void __fastcall ClearSignal();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmFldProc *fmFldProc;
//---------------------------------------------------------------------------
#endif
