//---------------------------------------------------------------------------
// Audio signal extremising module
// Author: Michael Shmalko
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "folder.h"
#include <math.h>
#pragma link "AudioIO"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// from g711.c
extern "C"
{
	unsigned char linear2ulaw(short pcm_val);
	short ulaw2linear(unsigned char	u_val);
}
//---------------------------------------------------------------------------
__fastcall TFolder::TFolder(TComponent *AOwner) : TComponent(AOwner)
{
 OnSignal = NULL;
 OnFold = NULL;
 Reset();
 c_level[0] = 1;
 c_level[1] = 1;
 c_level[2] = 1;
 c_level[3] = 8;

 vas_lvl[0] =  254;
 vas_lvl[1] =  3;
 vas_lvl[2] =  10;
 vas_lvl[3] =  4;
}
//---------------------------------------------------------------------------
__fastcall TFolder::~TFolder()
{
}
//---------------------------------------------------------------------------
void __fastcall TFolder::SetCLevel(unsigned char lev[])
{
 memmove(c_level, lev, sizeof(c_level));
}
void __fastcall TFolder::SetVLevel(unsigned char lev[])
{
 memmove(vas_lvl, lev, sizeof(vas_lvl));
}
//---------------------------------------------------------------------------
bool __fastcall TFolder::InBufferReady(PChar Buffer, int &Size)
{
 short *buf_PCM = (short*)Buffer;
 int   size_PCM = Size >> 1;
 
 for (int i = 0; i < size_PCM; i++)
 {
  if (fold_pos >= FOLD_BUF_SIZE - 2)
  {
   if (OnFold) OnFold(fold_pos, b_ptr);
   memmove(fbuf, &fbuf[fold_pos-3], 3*sizeof(TFPoint));
   fold_pos = 2;
  }

  if (val_max(buf_PCM[i]) > 0) // new extremum
  {
   if (abs(ulaw2linear(fbuf[fold_pos - 2].a) - ulaw2linear(fbuf[fold_pos - 1].a)) < vas_lvl[1])
    sil_count+=fbuf[fold_pos-1].m;
   else
    sil_count = 0;
  } else continue; // no extremum goes on

  if (sil_count > vas_lvl[0]*32) // silence detected
  {
   sil_count = vas_lvl[0]*64;
   if (!folding) continue;
   folding = false;
   if (OnSignal) OnSignal(1);
   if (OnFold) OnFold(fold_pos, b_ptr);
   Reset();
   if (OnSignal) OnSignal(2);
   continue;
  }
  if (folding) continue;
  // if not folding, looking for signal
  if ( sil_count < vas_lvl[2]*32) // signal detected
  {
   if (OnSignal) OnSignal(0);
   folding = true;
   memmove(fbuf, &fbuf[fold_pos-3], 3*sizeof(TFPoint));
   fold_pos = 2;
  }
 }
 return true;
}
//---------------------------------------------------------------------------
void __fastcall TFolder::Reset( void )
{
 sil_count = 0;
 fold_pos = 2;
 short *fake_buf = (short *)fbuf;

 for (int i = 0; i < FOLD_BUF_SIZE; ++i)
    fake_buf[i] = 0xFF01;

 last_d = 0;
 folding = false;

 d = last_d = 0;
 tangent[0] = tangent[1] = 1;
 tangent[2] = 0;
}
//---------------------------------------------------------------------------
int __fastcall TFolder::val_max(short value)
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
//---------------------------------------------------------------------------
