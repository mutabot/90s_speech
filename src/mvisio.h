//---------------------------------------------------------------------------
#ifndef mvisioH
#define mvisioH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ToolWin.hpp>
#include "AudioIO.hpp"
#include <graphics.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
#include "folder.h"
#include "colors.h"
#include "extruder.h"
#include "foldstore.h"
#include "tspeech.h"
//---------------------------------------------------------------------------
class TfmVisio : public TForm, public TExtrudeEvents
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *NFile;
        TMenuItem *NBlock;
        TMenuItem *NOpt;
        TOpenDialog *OpenDialog;
        TMenuItem *NOpenFold;
        TMenuItem *NOpenBuf;
        TMenuItem *NExport;
        TSaveDialog *SaveDialog;
        TMenuItem *N7;
        TMenuItem *NExit;
        TMenuItem *nSBase;
        TMenuItem *NColors;
        TMenuItem *NMorph;
        TMenuItem *NWin;
        TMenuItem *N1;
        TControlBar *ControlBar1;
        TPanel *Panel5;
        TLabel *Label1;
        TComboBox *cbScale;
        TPanel *Panel3;
        TMenuItem *N2;
        TMenuItem *NTalk;
        TMenuItem *NExtrude;
        TPanel *Panel1;
        TActionList *ActionList1;
        TImageList *ImageList1;
        TAction *acMorph;
        TAction *acRec;
        TAction *acExtrude;
        TAction *acExport;
        TMenuItem *NFindIn;
        TMenuItem *NFindHere;
        TMenuItem *N3;
        TAction *acFindIn;
        TAction *acFindHere;
        TAction *acFoldMan;
        TAction *acReduce;
    TAudioOut *AudioOut1;
    TPanel *Panel6;
    TEdit *edM;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton4;
    TSpeedButton *SpeedButton5;
    TSpeedButton *btZoomSel;
    TSpeedButton *tbSyn;
    TSpeedButton *SpeedButton8;
    TSpeedButton *SpeedButton9;
    TSpeedButton *tbNext;
    TSpeedButton *tbPlay;
        void __fastcall NOpenClick(TObject *Sender);
        void __fastcall cbScaleChange(TObject *Sender);
        void __fastcall NOpenBufClick(TObject *Sender);
        void __fastcall tbNextClick(TObject *Sender);
        void __fastcall tbSynClick(TObject *Sender);
        void __fastcall NExportClick(TObject *Sender);
        void __fastcall nSBaseClick(TObject *Sender);
        void __fastcall NColorsClick(TObject *Sender);
        bool __fastcall AudioOutFillBuffer(PChar Buffer, int &Size);
        void __fastcall tbPlayClick(TObject *Sender);
        void __fastcall AudioOutStop(TObject *Sender);
        void __fastcall btZoomSelClick(TObject *Sender);
        void __fastcall NFileClick(TObject *Sender);
        void __fastcall NMorphClick(TObject *Sender);
        void __fastcall acRecExecute(TObject *Sender);
        void __fastcall acExtrudeExecute(TObject *Sender);
        void __fastcall acFindInExecute(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall MRUManClick(TObject *Sender,
          const AnsiString RecentName, const AnsiString Caption,
          int UserData);
        void __fastcall acFoldManExecute(TObject *Sender);
        void __fastcall acReduceExecute(TObject *Sender);
private:	// User declarations
        int stoppos;
        TMemoryStream *msPlay;
        TExtruder *Extruder;
        TFoldStore *FoldStore;
        TColors Colors;
    void __fastcall OnChildChange(System::TObject* Sender);

    // class TExtrudeEvents implementation
    int OnBufferReady(short *buf, int size);
    int OnDone(void) { return 0; };

    AnsiString CurDir;
    TSpeech *Speech;
public:		// User declarations
        __fastcall TfmVisio(TComponent* Owner);
        __fastcall TfmVisio::~TfmVisio();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmVisio *fmVisio;
//---------------------------------------------------------------------------
#endif
