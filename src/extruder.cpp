//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "extruder.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// from g711.c
extern "C"
{
	unsigned char linear2ulaw(short pcm_val);
	short ulaw2linear(unsigned char	u_val);
}

//---------------------------------------------------------------------------
__fastcall TExtruder::TExtruder(TExtrudeEvents *pOnExtrude)
:    m_pOnExtrude(pOnExtrude)
{
 msMorph = new TMemoryStream();
 Morph = new TMorph(NULL, msMorph);
}
//---------------------------------------------------------------------------
__fastcall TExtruder::TExtruder()
:    m_pOnExtrude(NULL)
{
 msMorph = new TMemoryStream();
 Morph = new TMorph(NULL, msMorph);
}
//---------------------------------------------------------------------------
__fastcall TExtruder::~TExtruder()
{
 delete msMorph;
 delete Morph;
}
//---------------------------------------------------------------------------
int __fastcall TExtruder::Extrude(TStream *SFold, TStream *SBuf, int fsize)
{
 int i, j, samples = 0, r_pos = 0, scnt, fold_size = 1;
 double w, wx, d, ov = 128;
 TFPoint fold[FLD_BUF_SIZE];
 short buf[RES_BUF_SIZE];
 if (fsize < 0) fsize = MaxInt;
 while ( fsize > 0 && fold_size > 0 )
 {
  if (msMorph->Size == msMorph->Position)
   fold_size = SFold->Read(fold, sizeof(fold)) / sizeof(TFPoint);
  else
   fold_size = msMorph->Read(fold, sizeof(fold)) / sizeof(TFPoint);
  for (i = 0; i < fold_size; i++)
  {
   scnt = fold[i].m;
   if (scnt == 0) { // seg load   !!!!
     scnt = fold_size - i - 1;
     SFold->Position = SFold->Position - scnt*sizeof(TFPoint);
     scnt = fold[i].a;  // NOT A ULAW VALUE!
     SFold->Read(fold, scnt*sizeof(TFPoint));
     Morph->LoadSeg(fold, scnt);
     break;
   }
   if (scnt == 0xFF) {  // do morph  !!!!
     SFold->Position = SFold->Position - (fold_size - i - 1)*sizeof(TFPoint);
     msMorph->Clear();
     Morph->Morph(fold[i].a);              // NOT A ULAW VALUE
     msMorph->Position = 0;
     break;
   }
   fsize--;
   w = M_PI / (double)scnt; // 0 - pi

   short aPCM = ulaw2linear(fold[i].a); // decode the ULaw value

   d = aPCM - ov;
   for (j = 0; j < scnt; j++)
   {
    if (r_pos == RES_BUF_SIZE)
    {
     if (m_pOnExtrude) m_pOnExtrude->OnBufferReady(buf, r_pos);
     if (SBuf)
     {
        SBuf->Size+=r_pos;
        SBuf->Write(buf, r_pos*sizeof(short));
     }
     samples+=r_pos;
     r_pos = 0;
    }
    wx = -M_PI_2 + w*j;
    buf[r_pos] = ov + round(d*( (1.0 + sin(wx)) / 2.0 ));
    r_pos++;
   }
   ov = aPCM;
  }
 }
 if (m_pOnExtrude) m_pOnExtrude->OnBufferReady(buf, r_pos);
 if (SBuf)
 {
    SBuf->Size+=r_pos;
    SBuf->Write(buf, r_pos*sizeof(short));
 }
 samples+=r_pos;

 return samples;
}
//---------------------------------------------------------------------------

