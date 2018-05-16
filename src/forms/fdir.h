//---------------------------------------------------------------------------
#ifndef fdirH
#define fdirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "foldstore.h"
#include <Mask.hpp>
#include <DBGrids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfmFoldDir : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TEdit *edWidth;
        TDBGrid *FGrid;
        TButton *Button1;
        TButton *btDel;
        TButton *btCancel;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TEdit *edComment;
        TSpeedButton *edCommentButton;
        void __fastcall FGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall FGridMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FGridSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall edCommentKeyPress(TObject *Sender, char &Key);
        void __fastcall edCommentButtonClick(TObject *Sender);
        void __fastcall btDelClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall FGridDblClick(TObject *Sender);
        void __fastcall FGridMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FGridMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FGridKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
        TFoldStore *FFoldStore;
        TBrush *tempBrush;
        TPen   *tempPen;
        void __fastcall CreateParams(TCreateParams& Params)
        {
         TForm::CreateParams(Params);
         Params.WndParent = 0;
        }
        TSample m_sample;
        int marker[33];
public:
    TSample *sample;
    __fastcall TfmFoldDir(TFoldStore *FoldStore, TSample *ASample, TComponent* Owner);
    __fastcall ~TfmFoldDir();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmFoldDir *fmFoldDir;
//---------------------------------------------------------------------------
#endif
