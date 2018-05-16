//---------------------------------------------------------------------------
#ifndef fldrecH
#define fldrecH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "folder.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <registry.hpp>
#include <Mask.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfmFldRec : public TForm
{
__published:	// IDE-managed Components
        TShape *shpRec;
        TSpeedButton *btStart;
        TLabel *Label5;
        TGroupBox *gbParams;
        TLabel *Label3;
        TEdit *edCLev;
        TButton *btSetCLev;
        TEdit *edVLev;
        TLabel *Label4;
        TCheckBox *cbTop;
        TSpeedButton *SpeedButton2;
    TOpenDialog *fneSrcFile;
    TEdit *edDstFile;
    TBitBtn *btBrowseDst;
    TLabel *Label1;
    TEdit *edSrcFile;
    TBitBtn *btBrowseSrc;
    TSaveDialog *fneDstFile;
    TProgressBar *pbConverting;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btStartClick(TObject *Sender);
        void __fastcall btSetCLevClick(TObject *Sender);
        void __fastcall cbTopClick(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btBrowseDstClick(TObject *Sender);
    void __fastcall btBrowseSrcClick(TObject *Sender);
private:	// User declarations
        TFolder *Folder;
        void __fastcall OnFold( int fold_size, int b_size );
        void __fastcall OnSignal( int Colour );
        TFileStream *fsFold, *fsPCM;
        TRegistry   *rgParams;
        short buf_PCM[2048];
        void __fastcall CreateParams(TCreateParams& Params);
public:		// User declarations
        __fastcall TfmFldRec(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmFldRec *fmFldRec;
//---------------------------------------------------------------------------
#endif
