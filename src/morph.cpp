//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "morph.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMorph::TMorph(TStream *FResult)
{
 FoldStore = new TFoldStore(GetCurrentDir() + "\\samples.fol");
 fsResult = FResult;
 memset(&cur_smp, 0, sizeof(TSample));
 load_sample(cur, &cur_smp);
 where_constructed = 0;
 LengthDev = 2;
}
//---------------------------------------------------------------------------
__fastcall TMorph::TMorph(TFoldStore *AFoldStore, TStream *FResult) 
{
 FoldStore = AFoldStore;
 memset(&cur_smp, 0, sizeof(TSample));
 if (FoldStore) load_sample(cur, &cur_smp);
 fsResult = FResult;
 where_constructed = 1;
 LengthDev = 2;
}
//---------------------------------------------------------------------------
__fastcall TMorph::~TMorph()
{
 switch (where_constructed) {
  case 0: delete FoldStore; break;
  case 1: break;
 }
}
//---------------------------------------------------------------------------
int __fastcall TMorph::load_sample( TFPoint buf[], TSample *sample )
{
 memset(buf, 0, S_BUF_SIZE*sizeof(TFPoint));
 return FoldStore->Read(sample, buf);
}
//---------------------------------------------------------------------------
int __fastcall TMorph::Morph(int steps)
{
 int i, j, k, l, cur_len = 0, dst_len = 0;
 double step, w, shift, a[3], ak, ov[3], d;
 double sm[3], sm_[3], lk, K[2];

 sm[0] = sm[1] = sm[2] = sm_[2] = 0;     // 0 - cur, 1 - dst, 2 - wrk
 for (i = 1; i < cur_smp.pcount; i++) cur_len+=cur[i].m;
 for (i = 1; i < dst_smp.pcount; i++) dst_len+=dst[i].m;

 if (steps > 0)
 {
  d = 1;
  if ( abs(cur_len - dst_len) > LengthDev)
  {
   if (cur_len > dst_len) d = (double)(cur_len - LengthDev)/ (double)dst_len;
   else d = (double)(cur_len + LengthDev) / (double)dst_len;
   dst_len = 0;
   for (i = 1; i < dst_smp.pcount; i++)
   {
    if ( (dst[i].m = round( d * (double)dst[i].m )) == 0) dst[i].m = 1;
    dst_len+=dst[i].m;
   }
  }
  cur_len = cur_len*(steps*2);
  dst_len = dst_len*(steps*2);
  lk = (double)dst_len / (double)cur_len;
  K[0] = (lk - 1.0) / (double)cur_len;
  K[1] = (1.0 - 1.0/lk) / (double)dst_len;
 }
 for ( k = 0; k < steps; k++ )
 {
  l = 0;
  ov[0] = cur[0].a; ov[1] = dst[0].a;
  j = 1; i = 1;
  memset(wrk, 0, sizeof(wrk));
  while (1)
  {
   sm_[0] = sm[0] + cur[i].m;
   sm_[0] = sm_[0] + K[0]*sm_[0]*sm_[0];
   sm_[1] = sm[1] + dst[j].m;
   sm_[1] = sm_[1]/lk + K[1]*sm_[1]*sm_[1];
   if ( sm_[0] < sm_[1] )
   {
    step = sm[0]/(double)cur_len; // !!!!!
    wrk[l].m = round(sm_[0] - sm_[2]); //
    ov[0] = a[0] = cur[i].a;
    sm[0]+=cur[i].m;
    i++;
    sm_[2] = sm_[0];
    ov[2] = ov[1];
    d = dst[j].a - ov[1];
    w = (double)dst[j].m*(1.0 + K[1]*(2.0*sm[1] + (double)dst[j].m));
    shift = w - (sm_[1] - sm_[0]);
    ak = step;
   } else {
    step = sm[1]/(double)dst_len; // !!!!!
    wrk[l].m = round(sm_[1] - sm_[2]); //
    ov[1] = a[0] = dst[j].a;
    sm[1]+=dst[j].m;
    j++;
    sm_[2] = sm_[1];
    ov[2] = ov[0];
    d = cur[i].a - ov[0];
    w = (double)cur[i].m*(1.0 + K[0]*(2.0*sm[0] + (double)cur[i].m));
    shift = w - (sm_[0] - sm_[1]);
    ak = 1 - step;
   }
   if (wrk[l].m == 0) continue;
   a[1] = ov[2] + d*( (1 + sin(-M_PI_2 + M_PI*shift/w)) / 2 );
   a[2] = a[0] - (a[0] - a[1])*ak;
   wrk[l].a = round(a[2]);
   l++;
   if (j >= dst_smp.pcount) { sm[0]+=cur[i].m; break; }
   if (i >= cur_smp.pcount)  { sm[1]+=dst[j].m; break; }
  }
  fsResult->Write(wrk, l*sizeof(TFPoint));
 }

 fsResult->Write(&dst[1], (dst_smp.pcount - 1)*sizeof(TFPoint));

 memmove(&cur_smp, &dst_smp, sizeof(TSample));
// load_sample(cur, &cur_smp);
 memmove(cur, dst, sizeof(dst));
 return 0;
}
//---------------------------------------------------------------------------
int __fastcall TMorph::LoadSeg(TSample *smp)
{
 memmove(&dst_smp, smp, sizeof(TSample));
 memset(wrk, 0, sizeof(wrk));
 return load_sample(dst, &dst_smp);
}
//---------------------------------------------------------------------------
int __fastcall TMorph::LoadSeg(TFPoint *seg, int pcount)
{
 dst_smp.pcount = pcount;
 memset(wrk, 0, sizeof(wrk));
 memset(dst, 0, sizeof(dst));
 memmove(dst, seg, pcount*sizeof(TFPoint));
 return pcount*sizeof(TFPoint);
}
//---------------------------------------------------------------------------

