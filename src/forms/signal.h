//---------------------------------------------------------------------------
#ifndef signalH
#define signalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "sptecdef.h"
#include "colors.h"
//---------------------------------------------------------------------------
class TfmSignal : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel2;
        TPaintBox *pbMark;
        TPaintBox *pbMarkA;
        TPaintBox *pbFold;
        TScrollBar *sbFold;
        TScrollBar *sbBuf;
        TStatusBar *sBar1;
        void __fastcall pbFoldPaint(TObject *Sender);
        void __fastcall sbFoldChange(TObject *Sender);
        void __fastcall pbFoldMouseDown(TObject *Sender,
        TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall sbBufChange(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
        int fold_size, fold_pos;
        double skale[2], swidth;
        void __fastcall LoadFold(int offset)
        {
         if (sel_seg[0] > 0) sel_seg[0]+=pseg[0] - offset;
         if (sel_seg[1] > 0) sel_seg[1]+=pseg[0] - offset;
         pseg[0] = offset;
         fsFold->Seek(pseg[0]*sizeof(TFPoint), soFromBeginning);
         fold_size = fsFold->Read(fbuf, sizeof(fbuf));
         fold_size = fold_size / sizeof(TFPoint);
        }
        double tskale;
        int __fastcall val_max(const short value);
        unsigned char __fastcall pcm_mod(unsigned char value)
        {
         if (value & 0x80) return value&0x7F;
         else return 0x80 - value;
        };
        int d, last_d, tangent[3]; // третий держит направление
        int tracer;
        int selX[2];
        TColors *Colors;
    bool FSync;
    TNotifyEvent FOnChange;
    void __fastcall SetSkale(int value);
    int __fastcall GetScale( void );
    void __fastcall SetSynch(bool value);
    void __fastcall SetBufFileName(const AnsiString value);
public:		// User declarations
    TFileStream *fsBuf, *fsFold;
    TFPoint fbuf[32768];
    short buf[32768];
    int pseg[2], sel_seg[2];
    AnsiString FPointText;
    AnsiString FoldFileName;
    __fastcall TfmSignal(const AnsiString AFoldFileName,
                         const AnsiString ABufFileName,
                         TColors *AColors,
                         TNotifyEvent OnChange,
                         TComponent* Owner);
    __fastcall ~TfmSignal();
    int __fastcall StepFold();
    void __fastcall DrawMark(int mPos);
    void __fastcall ZoomSel();
    void __fastcall Seek(int fold_start);

    __property int Skale  = { write = SetSkale, read = GetScale };
    __property bool Synch  = { write = SetSynch };
    __property const AnsiString BufFileName  = { write = SetBufFileName };
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSignal *fmSignal;
//---------------------------------------------------------------------------
#endif
