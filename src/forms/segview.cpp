//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "segview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmSegView *fmSegView;
//---------------------------------------------------------------------------
__fastcall TfmSegView::TfmSegView(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmSegView::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Action = caFree;        
}
//---------------------------------------------------------------------------
void __fastcall TfmSegView::pbSegPaint(TObject *Sender)
{
 int i, H = pbSeg->Height, W = pbSeg->Width;
 double j;
 pbSeg->Canvas->Brush->Color = clBtnFace;
 pbSeg->Canvas->FillRect(pbSeg->Canvas->ClipRect);

 scale[0] = (double)(W-SEG_BORDER*2) / (double)swidth;
 scale[1] = (double)H / 256.0;

 pbSeg->Canvas->Pen->Color = clInfoBk;
 pbSeg->Canvas->MoveTo(0, H/2);
 pbSeg->Canvas->LineTo(W, H/2);
 pbSeg->Canvas->Pen->Color = clWindowText;
 pbSeg->Canvas->MoveTo(SEG_BORDER, H - fbuf[0].a*scale[1]);
 j=0;
 for (i = 1; i < ffold_size; i++)
 {
  j+=fbuf[i].m*scale[0];
  pbSeg->Canvas->LineTo(SEG_BORDER + j, H - fbuf[i].a*scale[1]);
 }
 pbSeg->Canvas->Brush->Color = clWindow;
 pbSeg->Canvas->Rectangle(SEG_BORDER-2, H - fbuf[0].a*scale[1] - 2,
                          SEG_BORDER+2, H - fbuf[0].a*scale[1] + 2);
 j=0;
 for (i = 1; i < ffold_size; i++)
 {
  j+=fbuf[i].m*scale[0];
  pbSeg->Canvas->Rectangle(SEG_BORDER + j-2, H - fbuf[i].a*scale[1] - 2,
                           SEG_BORDER + j+2, H - fbuf[i].a*scale[1] + 2);
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmSegView::pbScalePaint(TObject *Sender)
{
 pbScale->Canvas->FillRect(pbScale->Canvas->ClipRect);
 for (int i = 0; i < swidth; i+=10)
 {
  pbScale->Canvas->MoveTo(SEG_BORDER+i*scale[0], 0);
  pbScale->Canvas->LineTo(SEG_BORDER+i*scale[0], 2);
  pbScale->Canvas->TextOut(SEG_BORDER+i*scale[0]-1, 1, AnsiString(i));
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmSegView::FormShow(TObject *Sender)
{
 swidth = 0;
 for (int i = 1; i < ffold_size; i++) swidth+=fbuf[i].m;
 if (swidth == 0) return;
 pnCaption->Caption = "Ширина: " + AnsiString(swidth);
}
//---------------------------------------------------------------------------
