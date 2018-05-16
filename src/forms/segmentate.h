//---------------------------------------------------------------------------
#ifndef segmentateH
#define segmentateH
//---------------------------------------------------------------------------
#include "signal.h"
#include "fldprocess.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfmSegments : public TForm
{
__published:	// IDE-managed Components
        TMemo *meDev;
        TPanel *Panel1;
        TPaintBox *pbParams;
        TSplitter *Splitter1;
    TButton *btStartSeg;
    TButton *btWindow;
    TButton *btReduce;
    TButton *btSave;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btStartSegClick(TObject *Sender);
        void __fastcall btReduceClick(TObject *Sender);
        void __fastcall pbParamsPaint(TObject *Sender);
        void __fastcall btWindowClick(TObject *Sender);
        void __fastcall btSaveClick(TObject *Sender);
private:	// User declarations
        int pcount, segstart;
        TfmSignal *Signal;
        TFoldProcess *FoldProcess;
        TList *ParamsList;
        TStringList *Statistics;
        void __fastcall OnSegment(TFPoint *Segment, int offset, int pcount, int dev);
        void __fastcall OnNoise(TFPoint *fbuf, int offset, int pcount);
        void __fastcall OnParams(TSParams *Params);
        void __fastcall OnCompare(int d, double dlen[], int dpar[]);
        void __fastcall OnRewidth(TObject *Sender);
public:		// User declarations
        __fastcall TfmSegments(TfmSignal* ASignal, TComponent* Owner);
    __fastcall ~TfmSegments();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSegments *fmSegments;
//---------------------------------------------------------------------------
#endif
