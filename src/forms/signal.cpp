//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "signal.h"
#include "folder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmSignal *fmSignal;

// from g711.c
extern "C"
{
	unsigned char linear2ulaw(short pcm_val);
	short ulaw2linear(unsigned char	u_val);
}

//---------------------------------------------------------------------------
__fastcall TfmSignal::TfmSignal(const AnsiString AFoldFileName,
                                const AnsiString ABufFileName,
                                TColors *AColors,
                                TNotifyEvent OnChange,
                                TComponent* Owner) : TForm(Owner)
{
 fold_size = 0;
 fold_pos = 1;
 tracer = 0;
 pseg[0] = 1;
 pseg[1] = 12;
 tskale = 1;
 FoldFileName = AFoldFileName;
 if (FoldFileName > "")
 {
  memset(fbuf, 0, sizeof(fbuf));
  fsFold = new TFileStream(FoldFileName, fmOpenRead|fmShareDenyNone );
  sbFold->Max = fsFold->Size / 2;
  pseg[0] = -1;
 } else fsFold = NULL;

 if (ABufFileName > "")
 {
  fsBuf = new TFileStream(ABufFileName, fmOpenRead | fmShareDenyNone );
  sbBuf->Max = fsBuf->Size;
 } else fsBuf = NULL;
 Colors = AColors;
 FSync = false;
 Caption = FoldFileName;
 FOnChange = OnChange;
}
//---------------------------------------------------------------------------
__fastcall TfmSignal::~TfmSignal()
{
 if (fsBuf) delete fsBuf;
 if (fsFold) delete fsFold;
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::pbFoldPaint(TObject *Sender)
{
 int i, j, k, sel_len = 0;
 //if (AudioOut1->Active) return;
 Graphics::TColor SelColor[2] = { clRed, clBlue };
 pbFold->Canvas->Brush->Color = Colors->Bk;
 pbFold->Canvas->FillRect(pbFold->Canvas->ClipRect);
 pbFold->Canvas->Pen->Color = clInfoBk;
 pbFold->Canvas->MoveTo(0, pbFold->Height / 2);
 pbFold->Canvas->LineTo(pbFold->Width, pbFold->Height / 2);
 if (pseg[0] != sbFold->Position) LoadFold(sbFold->Position);
 swidth = 0;
 for (i = 1; ((swidth <= pbFold->Width*tskale) && (i < fold_size)); i++)
  swidth+=fbuf[i].m;
 pseg[1] = i;
 if (swidth == 0) return;
 skale[0] = 1.0 / (double)tskale;
 skale[1] = (double)pbFold->Height / 32768.0;
 j = 0;
 for (i = 1; i <= pseg[1]; i++)
 {
  for (k = 0; k < 2; k++)
   if ( (selX[k] >= 0) && (selX[k] >= j*skale[0]) &&
        (selX[k] <= (j+fbuf[i].m)*skale[0]))
   {   sel_seg[k] = i;  selX[k] = -1;   }
  j+=fbuf[i].m;
 }
 j = 0;
 int iZeroPos = pbFold->Height / 2;

 pbFold->Canvas->MoveTo(0, iZeroPos + ulaw2linear(fbuf[0].a)*skale[1]);
 for (i = 1; i <= pseg[1]; i++)
 {
  j+=fbuf[i].m;
  if ( (i > sel_seg[0]) && (i <= sel_seg[1]) ) sel_len+=fbuf[i].m;
  for (k = 0; k < 2; k++)
   if ( sel_seg[k] == i ) { pbFold->Canvas->Pen->Color = SelColor[k]; break; }
   else pbFold->Canvas->Pen->Color = Colors->Fold;
  pbFold->Canvas->LineTo(j*skale[0], iZeroPos + ulaw2linear(fbuf[i].a)*skale[1]);
 }
 if (tskale < 8)
 {
  j = 0;
  pbFold->Canvas->Rectangle(- 2, iZeroPos + ulaw2linear(fbuf[0].a)*skale[1] - 2,
                              2, iZeroPos + ulaw2linear(fbuf[0].a)*skale[1] + 2);
  for (i = 1; i <= pseg[1]; i++)
  {
   j+=fbuf[i].m;
   for (k = 0; k < 2; k++)
    if ( sel_seg[k] == i ) { pbFold->Canvas->Brush->Color = SelColor[k]; break; }
    else pbFold->Canvas->Brush->Color = clWhite;
   pbFold->Canvas->Rectangle(j*skale[0] - 2, iZeroPos + ulaw2linear(fbuf[i].a)*skale[1] - 2,
                           j*skale[0] + 2, iZeroPos + ulaw2linear(fbuf[i].a)*skale[1] + 2);
  }
 }
 sBar1->Panels->Items[0]->Text = "From: " + AnsiString(pseg[0]) + " To: " + AnsiString(pseg[0] + pseg[1]);
 sBar1->Panels->Items[1]->Text = AnsiString(swidth) + " smp.";
 sBar1->Panels->Items[2]->Text = AnsiString(pseg[1]) + " fld.";
 sBar1->Panels->Items[3]->Text = "Compr. " + AnsiString::FormatFloat("####0.##", swidth / (pseg[1]*2)) + " раз.";
 sBar1->Panels->Items[4]->Text = "Sel. " + AnsiString(pseg[0] + sel_seg[0]) + " - " + AnsiString(pseg[0] + sel_seg[1])
                                + " (" + AnsiString(sel_seg[1] - sel_seg[0]) + ")";
 sBar1->Panels->Items[5]->Text = "Sel. len.:" + AnsiString(sel_len);
 if (sel_seg[0] >= 0)
  FPointText = "W: " + AnsiString((int)fbuf[sel_seg[0]].m) + " A: " + AnsiString((int)ulaw2linear(fbuf[sel_seg[0]].a));
 sbFold->LargeChange = pseg[1] / 1.8;
 if (!fsBuf) return;
 /*if (tracer - sbBuf->Position - 1 >=0 )
   for (i = -1; i < 2; i++)
    Grid1->Cells[i + 1][2] = AnsiString(buf[tracer - sbBuf->Position + i]);*/
 fsBuf->Seek(sbBuf->Position << 1, soFromBeginning);
 sbBuf->LargeChange = swidth / 2;
 pbFold->Canvas->Pen->Color = Colors->PCM;

 int last_X = 0, last_Y = iZeroPos;
 int rd_bytes = swidth * 2;    // multiply by number of bytes per sample
 while (rd_bytes > 0)
 {
    int rd = fsBuf->Read(buf, rd_bytes > sizeof(buf) ? sizeof(buf) : rd_bytes);
    if (rd <= 0) break;
    rd_bytes-=rd;
    pbFold->Canvas->MoveTo(last_X, last_Y);
    for (i = 0; i < (rd >> 1); i++)   // for each sample
    {
      if (i == tracer - sbBuf->Position)
        pbFold->Canvas->Pen->Color = clRed;
      else
        pbFold->Canvas->Pen->Color = Colors->PCM;
      pbFold->Canvas->LineTo(i*skale[0], iZeroPos + buf[i]*skale[1]);
    }
    i--;
    last_X = i*skale[0];
    last_Y = iZeroPos + buf[i]*skale[1];
 }

 pbFold->Canvas->MoveTo((tracer - sbBuf->Position)*skale[0], 0);
 pbFold->Canvas->LineTo((tracer - sbBuf->Position)*skale[0], iZeroPos);
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::sbFoldChange(TObject *Sender)
{
 int i, shift = 0, opos = pseg[0];
 selX[0] = selX[1] = -1;
 if (FSync)
 {
  if (pseg[0] <= sbFold->Position)
   for (i = 1; i <= sbFold->Position - pseg[0]; i++)
    shift+=fbuf[i].m;
  else
  {
   LoadFold(sbFold->Position);
   for (i = 1; i <= opos - sbFold->Position; i++)
    shift-=fbuf[i].m;
  }
  sbBuf->Position+=shift;
 } else {
  pbFold->Repaint();
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::pbFoldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 int i;
 if (Button == mbLeft) i = 0;
 else i = 1;
 selX[i] = X;
 sel_seg[i] = -1;
 pbFold->Repaint();
 if (FOnChange) FOnChange(this);
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::sbBufChange(TObject *Sender)
{
 selX[0] = selX[1] = -1;
 pbFold->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::FormKeyPress(TObject *Sender, char &Key)
{
 if (Key != ' ') return;
 tracer++;
 val_max(buf[tracer - sbBuf->Position]);
 pbFold->Repaint();
}
//---------------------------------------------------------------------------
int __fastcall TfmSignal::val_max(const short value)
{
 short fold_pos_PCM = ulaw2linear(fbuf[fold_pos].a);
 short ex = fold_pos_PCM + last_d;
 int i, var = 0;
 d = value - fold_pos_PCM;  // increment from last extremum
 tangent[0] = d - last_d;       // from last count

 fbuf[fold_pos].m++;
 do {
  if (tangent[0]*tangent[1] > 0) break;  // no fracture
  if (tangent[0]*tangent[1] < 0) { var = 1; break; }// fracture and extremum
  if ((tangent[0] != 0) && (tangent[1] == 0)) // end of plateau
  {
   if (tangent[0]*tangent[2] > 0) // ---- no direction change
   {
    if (fbuf[fold_pos].m > 4) { var = 2; break; } // long plateau
    else var = 4;
    if ( abs(tangent[2]) > 1 ) var|= 8;  // strong fracture -- save the extremum at plateau start
    if ( abs(tangent[0]) > 1 ) var|= 16; // strong fracture -- save the extremum at plateau end
    break;
   }
   if (tangent[0]*tangent[2] < 0) var = 2; // ----- direction changed
   break;
  }
  if (tangent[0] == 0) // plateau
  {
   if (tangent[1] == 0) var = 0; // unambiguously pass -- plateau
   if (tangent[1] != 0) var = 1; // plateau start
   break;
  }
 }
 while (0);
 if (fbuf[fold_pos].m >= MAX_WIDTH) var = -1; // create new extremum

 tangent[1] = tangent[0];                            // ******************
 if (tangent[0] != 0) tangent[2] = tangent[0];       // state change
 last_d = d;                                         // ******************

 switch (var) {
  case 12:
  case 0: return 0; // no extremum
  case 28:
  case 1: break;    // create extremum
  case 2:           // extremum at the middle of plateau
   if (fbuf[fold_pos].m > 6) break;
   if ((i = fbuf[fold_pos].m + fbuf[fold_pos - 1].m) >= MAX_WIDTH) break;
   i = fbuf[fold_pos].m / 2;
   fbuf[fold_pos - 1].m+= i;
   fbuf[fold_pos].m-= i;
  return 0;
  case 4:  // kill both start and end plateau extrumum
   if ((i = fbuf[fold_pos].m + fbuf[fold_pos - 1].m) >= MAX_WIDTH) break;
   fold_pos--;
   fbuf[fold_pos].m = i;
  return 0;
  case 20:  // kill starting and create ending extremum
   if ((i = fbuf[fold_pos].m + fbuf[fold_pos - 1].m) >= MAX_WIDTH) break;
   fold_pos--;
   fbuf[fold_pos].m = i;
  break;
 }

 const char ex_ULAW = linear2ulaw(ex);
 fbuf[fold_pos].a = ex_ULAW;
 // here is new extremum code
 fold_pos++;
 fbuf[fold_pos].a = ex_ULAW;
 fbuf[fold_pos].m = 0;
 last_d = value - ex;
 return 1;
}
/*
int __fastcall TfmSignal::val_max(const unsigned char value)
{
 unsigned char ex = fbuf[fold_pos].a + last_d;
 int i, var = 0;
 d = value - fbuf[fold_pos].a;  // приращение от последнего экстр.
 tangent[0] = d - last_d;       // от последнего отсчета
 
 fbuf[fold_pos].m++;
 do {
  if (tangent[0]*tangent[1] > 0) break;  // нет перелома
  if (tangent[0]*tangent[1] < 0) { var = 1; break; }// перелом и экстремум
  if ((tangent[0] != 0) && (tangent[1] == 0)) // конец площадки
  {
   if (tangent[0]*tangent[2] > 0) // ---- направдение не изменилось
   {
    var = 4;
    if ( abs(tangent[2]) > 2 ) var|= 8;
    if ( abs(tangent[0]) > 2 ) var|= 16; // перегиб сильный -- оставим экстремум вначале
    break;
   }
   if (tangent[0]*tangent[2] < 0) var = 2; // ----- направдение изменилось
   break;
  }
  if (tangent[0] == 0) // площадка
  {
   if (tangent[1] == 0) var = 0; // однозначно пропускаем -- площадка
   if (tangent[1] != 0) var = 1; // заход на площадку
   break;
  }
 }
 while (0);
 if (fbuf[fold_pos].m == 0xFF) var = -1; // проверка на перенос если ширина кончилась будет новый экстремум

 tangent[1] = tangent[0];                            // ******************
 if (tangent[0] != 0) tangent[2] = tangent[0];       // изменили состояние
 last_d = d;                                         // ******************

 switch (var) {
  case 12:
  case 0: return 0; // просто выходим экстремума нет
  case 28:
  case 1: break;    // делаем экстремум
  case 2:           // делаем экстремум в середине площадки
   if (fbuf[fold_pos].m > 6) break;
   if ((i = fbuf[fold_pos].m + fbuf[fold_pos - 1].m) >= 0xFF) break;
   i = fbuf[fold_pos].m / 2;
   fbuf[fold_pos - 1].m+= i;
   fbuf[fold_pos].m-= i;
  return 0;
  case 4:  // убиваем экстремум в начале и в конце площадки
   if ((i = fbuf[fold_pos].m + fbuf[fold_pos - 1].m) >= 0xFF) break;
   fold_pos--;
   fbuf[fold_pos].m = i;
  return 0;
  case 20:  // убиваем экстремум в начале площадки  и создаем новый в конце
   if ((i = fbuf[fold_pos].m + fbuf[fold_pos - 1].m) >= 0xFF) break;
   fold_pos--;
   fbuf[fold_pos].m = i;
  break;
 }

 fbuf[fold_pos].a = ex;
 // теперь создаем новый экстремум
 fold_pos++;
 fbuf[fold_pos].a = ex;
 fbuf[fold_pos].m = 0;
 last_d = value - ex;
 return 1;
}
*/
//---------------------------------------------------------------------------
int __fastcall TfmSignal::StepFold()
{
 tracer++;
 val_max(buf[tracer - sbBuf->Position]);
 pbFold->Repaint();
 return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::SetSkale(int value)
{
 tskale = (double)value / 10.0;
 sel_seg[0] = sel_seg[1] = -1;
 LoadFold(sbFold->Position);
 pbFold->Repaint();
}
//---------------------------------------------------------------------------
int __fastcall TfmSignal::GetScale( void )
{
 return tskale * 10.0;
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::SetSynch(bool value)
{
 FSync = value;
 tracer = sbBuf->Position;
 fold_pos = 0;
 d = last_d = 0;
 tangent[0] = tangent[1] = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::DrawMark(int mPos)
{
 pbMark->Repaint();
 pbMark->Canvas->Rectangle(mPos*skale[0]-2, 0,
                           mPos*skale[0]+2, pbMark->Height);
 pbMarkA->Repaint();
 pbMarkA->Canvas->Rectangle(mPos*skale[0]-2, 0,
                            mPos*skale[0]+2, pbMarkA->Height);
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::ZoomSel()
{
 int i, sel_len = 0;
 if ((sel_seg[0] < 0) || (sel_seg[1] < 0) || (sel_seg[0] > sel_seg[1])) return;
 for (i = sel_seg[0]; i < sel_seg[1]; i++)
  sel_len+=fbuf[i].m;
 tskale = (double)sel_len / (double)pbFold->Width;
 sbFold->Position+=sel_seg[0];
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::SetBufFileName(const AnsiString value)
{
 if (value > "")
 {
  fsBuf = new TFileStream(value, fmOpenRead | fmShareDenyNone );
  sbBuf->Max = fsBuf->Size;
 } else fsBuf = NULL;
 pbFold->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfmSignal::Seek(int fold_start)
{
 sbFold->Position = fold_start;
}
//---------------------------------------------------------------------------


