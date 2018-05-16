//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fldprocess.h"
#include <math.h>
#include <stdlib.h>


#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))  

//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall TFoldProcess::TFoldProcess() : TObject()
{
 Reset();
 OnParams = NULL;
 OnCompare = NULL;
 OnRewidth = NULL;
 OnNoise = NULL;
 Noise = new TStringList;
}
//---------------------------------------------------------------------------
__fastcall TFoldProcess::~TFoldProcess()
{
 delete Noise;
}
//---------------------------------------------------------------------------
int __fastcall TFoldProcess::InsertFold(TFPoint *fbuf, int pcount)
{
 int i, j, lp = 1, pt = 0, ln, lx, dan, dam, mx = 0, mn = 0;
 rcount = 0; sptr = 0; fold = fbuf;
 rbuf[pt].m = 0;
 for (i = 1; i < pcount; i++)
 {
  dan = abs(fbuf[mx].a - fbuf[i].a);
  ln = 0;
  for (j = i; j > mn; j--) ln+=fbuf[j].m;

  dam = abs(fbuf[mn].a - fbuf[i].a);
  lx = 0;
  for (j = i; j > mx; j--) lx+=fbuf[j].m;

  if ( (dan > 6 && ln > 12) || (lx > 12 && dam > 6) )
  {
   j = min(mn, mx), mx = max(mn, mx); mn = j;
   if (mn != mx)
   {
    rbuf[pt].a = fbuf[mn].a; map[pt] = mn;
    rbuf[pt].m+=fbuf[mn].m;
    for (j = lp; j < mn; j++) rbuf[pt].m+=fbuf[j].m;
    pt++;
    rbuf[pt].a = fbuf[mx].a; map[pt] = mx;
    rbuf[pt].m=fbuf[mx].m;
    for (j = mn+1; j < mx; j++) rbuf[pt].m+=fbuf[j].m;
    pt++; rbuf[pt].m = 0;
    for (j = mx+1; j < i; j++) rbuf[pt].m+=fbuf[j].m;
   } else {
    rbuf[pt].a = fbuf[mx].a; map[pt] = mx;
    rbuf[pt].m+= fbuf[mn].m;
    pt++; rbuf[pt].m = 0;
   }
   mx = i; mn = i; lp = i;
  } else {
   if (fbuf[i].a > fbuf[mx].a) mx = i;
   if (fbuf[i].a < fbuf[mn].a) mn = i;
  }
 }
 lx = 2; j = 1;
 while (lx < pt)
 {
  if (  (rbuf[j-1].a < rbuf[j].a && rbuf[j].a < rbuf[lx].a)
      ||(rbuf[j-1].a > rbuf[j].a && rbuf[j].a > rbuf[lx].a)
      ||( ((rbuf[j].a - rbuf[lx].a)*(rbuf[j].a - rbuf[lx].a) + rbuf[lx].m*rbuf[lx].m) < 20) )
  {
   rbuf[j].m+= rbuf[lx].m;
   rbuf[j].a = rbuf[lx].a;
   map[j] = map[lx];
  } else {
   j++;
   rbuf[j].m = rbuf[lx].m;
   rbuf[j].a = rbuf[lx].a;
   map[j] = map[lx];
  }
  lx++;
 }
 rcount = j;
 //memcpy(fbuf, rbuf, rcount*sizeof(TFPoint));
 return rcount;
}
//---------------------------------------------------------------------------
void __fastcall TFoldProcess::Reset( void )
{
 apass = 8;
 devpass = 128;
 lastseg.width = 0;
 lastseg.pcount = 1;
 sptr = 0;
 minwidth = 45;  maxwidth = 120;
 noiselevel = 3;
}
//---------------------------------------------------------------------------
int __fastcall TFoldProcess::PassNoise( void )
{
 int width, nptr = sptr, from = sptr;
 do {
  width = 0;
  do width+=rbuf[++nptr].m;  while ((rbuf[nptr-1].a < 0x80 || rbuf[nptr].a > 0x80) && nptr < rcount);
 // Noise->Add(AnsiString(width)+","+AnsiString(map[nptr]-map[sptr])+","+
 //            AnsiString(width/(map[nptr]-map[sptr]))+","+AnsiString(nptr-sptr));
  if ( width / (map[nptr] - map[sptr]) < noiselevel) sptr = nptr;
  else break;
 } while (nptr < rcount);
 // Noise->SaveToFile("Noise.txt");
 if (sptr!=from && OnNoise) OnNoise(fold, map[from], map[sptr]);
 return sptr - from;
}
//---------------------------------------------------------------------------
int __fastcall TFoldProcess::Segmentate(TFOnSegment OnSegment)
{
 int i, s2w, cmp, dptr[32], dcnt = 0, left, right, idev;
 struct {
  int dev;
  int left, lastcnt, nextcnt;
 } dev[32];
 if (lastseg.width >= maxwidth || lastseg.width < 1 || rbuf[sptr-1].a < 0x80 || rbuf[sptr].a > 0x80)
 {
  sptr++;
  while ((rbuf[sptr-1].a < 0x80 || rbuf[sptr].a > 0x80) && sptr < rcount) sptr++;
  lastseg.width =0;
  lastseg.pcount=1;
  if ((i = PassNoise()) > 0) { sptr-=2; Rewidth(-1, -1, 999); return -i;}
 }
 while (lastseg.width < maxwidth) {
  i = sptr+lastseg.pcount;
  do {
   lastseg.width+=rbuf[i].m;               // stepping the minimum width
   lastseg.pcount++;
  } while((rbuf[i-1].a < 0x80 || rbuf[i].a > 0x80 || lastseg.width<minwidth) && i++ < rcount);
  // searching the min that is most closer to the curpos + lastseg.width
  nextseg.width = 0;
  nextseg.pcount= 0;
  cmp=1; s2w = 0;
  while (++i < rcount) {
   cmp++;
   s2w+=rbuf[i].m;
   if (rbuf[i-1].a < 0x80 || rbuf[i].a > 0x80) continue;
   if (s2w < lastseg.width || nextseg.width == 0)
   { nextseg.width = s2w; nextseg.pcount=cmp; }
   else break;
  }
  if (lastseg.width-nextseg.width > s2w - lastseg.width)
  { nextseg.width = s2w; nextseg.pcount=cmp; }
  if (lastseg.width < minwidth || lastseg.width > maxwidth) continue;
/*  dev[dcnt].dev = CompareSeg(&fold[map[sptr]],
                             map[sptr+lastseg.pcount] - map[sptr],
                             &fold[map[sptr+lastseg.pcount-1]],
                             map[sptr+lastseg.pcount-1 + nextseg.pcount] - map[sptr+lastseg.pcount-1]);*/
  dev[dcnt].dev = CompareSeg(&rbuf[sptr], lastseg.pcount, &rbuf[sptr+lastseg.pcount-1], nextseg.pcount);
  dev[dcnt].left = sptr;
  dev[dcnt].lastcnt = lastseg.pcount;
  dev[dcnt].nextcnt = nextseg.pcount;
  dcnt++;
 }
 if (i > rcount) return -MaxInt;  // end of buffer
 s2w = 0;
 cmp = dev[0].dev + 1;
 for (i = 0; i < dcnt; i++)
 {
  if (dev[i].dev < 30) dptr[s2w++] = i;
  if (dev[i].dev >= cmp) dptr[s2w++] = i-1;
  cmp = dev[i].dev;
 }
 for (i = 0; i < s2w; i++)
  if (dev[dptr[i]].dev < 30 && (cmp = Validate(&rbuf[dev[dptr[i]].left], dev[dptr[i]].lastcnt)) == 0)
  {
   left = map[dev[dptr[i]].left];
   right = map[dev[dptr[i]].left + dev[dptr[i]].lastcnt - 1];
   idev = dev[dptr[i]].dev;
   if (OnSegment) OnSegment(fold, left, right, idev);
   Rewidth(left, right, idev);
   if (dev[dptr[i]].dev > 25) sptr++;
   else sptr=dev[dptr[i]].left + dev[dptr[i]].lastcnt - 2;
   return idev;
  }

 for (i = 0; i < dcnt; i++)
  if (dev[i].dev < 30 && (cmp = Validate(&rbuf[dev[i].left], dev[i].lastcnt)) == 0)
  {
   left = map[dev[i].left];
   right = map[dev[i].left + dev[i].lastcnt - 1];
   idev = dev[i].dev;
   if (OnSegment) OnSegment(fold, left, right, idev);
   Rewidth(left, right, idev);
   if (idev > 25) sptr++;
   else sptr=dev[i].left + dev[i].lastcnt - 2;
   return idev;
  }
 return -1;
}
//---------------------------------------------------------------------------
void __fastcall TFoldProcess::Rewidth(int left, int right, int idev)
{
 int i, width = 0;
 if (idev < 7)
 {
  for (i = left + 1; i <= right; i++) width+=fold[i].m;
  minwidth = width - (double)width*0.15;
  maxwidth = width + (double)width*0.15;
  if (OnRewidth) OnRewidth(this);
 }
 if (idev > 25)
 {
  if (minwidth > 28)  minwidth = minwidth - (double)minwidth*0.05;
  if (maxwidth < 160) maxwidth = maxwidth + (double)maxwidth*0.05;
 }
}
//---------------------------------------------------------------------------
int __fastcall TFoldProcess::Validate(TFPoint *seg, int pcount)
{
 int i, result = 0, mind = MaxInt, dd, height;
 unsigned char mina=0xFF, maxa=0, maxe, mine;
 if (seg[0].a < seg[pcount-1].a) { mine = seg[0].a; maxe = seg[pcount - 1].a; }
 else { mine = seg[pcount - 1].a; maxe = seg[0].a; }

 for (i = 0; i < pcount; i++) {
  if (mina > seg[i].a) mina = seg[i].a;
  if (maxa < seg[i].a) maxa = seg[i].a;
 }
 height = maxa - mina;
 if (abs(seg[0].a - seg[pcount - 1].a) > height/6) return -1;

 if (mina < mine) return -1;
 if (maxe - mine > maxe - mina) return -1;
 /*for (i = 1; i < pcount; i++)
  if (seg[i-1].a>0x80 && seg[i].a<0x80)
  {
   dd = 1000*abs(seg[i-1].a - seg[i].a) / seg[i].m;
   if (mind >= dd) { mind = dd; result = i; }
  }
 if (result != pcount - 1) return result;*/
 return 0;
}
//---------------------------------------------------------------------------
int __fastcall TFoldProcess::CompareSeg(TFPoint *seg1, int pcount1, TFPoint *seg2, int pcount2)
{
 int i, dpar[5];
 double t, d, dlen[4];
 TSParams *Params1 = GenParams(seg1, pcount1);
 TSParams *Params2 = GenParams(seg2, pcount2);
 if (OnParams) { OnParams(Params1); OnParams(Params2); }
 if (Params2 == NULL || Params1 == NULL) return MaxInt;
 dlen[0] = (double)Params2->length / (double)Params1->length;
 dlen[1] = (double)Params2->llen / (double)Params1->llen;
 dlen[2] = (double)Params2->rlen / (double)Params1->rlen;

 dpar[0] = 100*abs(Params1->ldir - Params2->ldir) / max(Params1->ldir, Params2->ldir);
 dpar[1] = 100*abs(Params1->rdir - Params2->rdir) / max(Params1->rdir, Params2->rdir);

 t=dlen[0];
 for (i=1; i < 3; i++) t= t+dlen[i];
 t=t/3.0;
 dpar[2] = 100*fabs(t - dlen[0]) / max(t, dlen[0]);
 for (i=1; i < 3; i++) dpar[2]= dpar[2]+100*fabs(t - dlen[i]) / max(t, dlen[i]);
 dpar[2] = dpar[2] / 3.0;

 dpar[3]= 100*abs(Params1->pwidth - Params2->pwidth) / max(Params1->pwidth, Params2->pwidth);
 dpar[4]= 100*abs(Params1->width - Params2->width) / max(Params1->width, Params2->width);
 d=dpar[0];
 for (i = 1; i < 5; i++)  d = d + dpar[i];
 d = d / 5.0;
 if (OnCompare) OnCompare(d, dlen, dpar);
 delete Params2;
 delete Params1;
 return d;
}
//---------------------------------------------------------------------------
TSParams* __fastcall TFoldProcess::GenParams(TFPoint *fbuf, int pcount)
{
 if (pcount < 3) return NULL;
 TFPoint tbuf[1024];
 memmove(tbuf, fbuf, pcount*sizeof(TFPoint));
 int i;
 double da, a, top[2] = {0, 0}, bot[2] = {0, 0};

 TSParams *Params = new TSParams;
 Params->length = 0;
 Params->llen = 0;
 Params->rlen = 0;
 Params->ldir = 0;
 Params->rdir = 0;
 Params->pwidth = 0;
 Params->width = 0;
 for (i = 1; i < pcount; i++)
 {
  Params->width+=tbuf[i].m;
  da = tbuf[i].a - tbuf[i-1].a;
  a = sqrt(da*da + tbuf[i].m*tbuf[i].m);
  if (da >= 0)
  {
   Params->llen+=round(a);
   top[0]+=da/a;
   bot[0]+=(double)tbuf[i].m / a;
  } else {
   Params->rlen+=round(a);
   top[1]-=da/a;
   bot[1]+=(double)tbuf[i].m / a;
  }
  Params->length+=round(a);
  Params->pwidth+=tbuf[i].m;
 }
 if (bot[0] != 0) Params->ldir = round(100.0*top[0] / bot[0]);
 else Params->ldir = -1;
 if (bot[1] != 0) Params->rdir = round(100.0*top[1] / bot[1]);
 else Params->rdir = -1;
 Params->pwidth = 100*Params->pwidth / (pcount-1);
 return Params;
}
//---------------------------------------------------------------------------
int* __fastcall TFoldProcess::GenIDParams(TFPoint *fbuf, int pcount)
{
 int *iparams = new TIDParams;
 TSParams* Params = GenParams(fbuf, pcount);
 iparams[0] = 100*(Params->length - Params->llen) / Params->length;
 iparams[1] = 100*(Params->length - Params->rlen) / Params->length;

 iparams[2] = 100*abs(Params->ldir - Params->rdir) / max(Params->ldir, Params->rdir);

 iparams[3] = Params->pwidth/10;
 iparams[4] = Params->width;
 return iparams;
}
//---------------------------------------------------------------------------
int __fastcall TFoldProcess::SearchSample(TStream *Source, TFPoint *seg, int pcount, TFOnSegment OnSegment, TFOnProgress OnProgress)
{
 return 0;
}
//---------------------------------------------------------------------------

