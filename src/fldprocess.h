//---------------------------------------------------------------------------
#ifndef fldprocessH
#define fldprocessH
#define FOLD_BUF_PCOUNT    4096
#define NUM_OF_ID_PARAMS   5
//---------------------------------------------------------------------------
#include "sptecdef.h"
//---------------------------------------------------------------------------
typedef int TIDParams[NUM_OF_ID_PARAMS];
typedef void __fastcall (__closure *TFOnSegment)(TFPoint *Segment, int offset, int pcount, int dev);
typedef void __fastcall (__closure *TFOnNoise)(TFPoint *fbuf, int offset, int pcount);
typedef void __fastcall (__closure *TFOnProgress)(int Size, int Pos);

struct TSParams {
 int length;     // length of the segments line
 int llen;
 int rlen;
 int ldir;
 int rdir;
 int pwidth;
 int width;
};
typedef void __fastcall (__closure *TFOnParams)(TSParams *Params);
typedef void __fastcall (__closure *TFOnCompare)(int d, double dlen[], int dpar[]);


class TFoldProcess : public TObject
{
private:
   struct {
     int width, pcount;
     TFPoint *data;
   } lastseg, nextseg;
   int sptr, rcount;
   TFPoint *fold, rbuf[FOLD_BUF_PCOUNT];
   int map[FOLD_BUF_PCOUNT];
   TStringList *Noise;
   int __fastcall CompareSeg(TFPoint *seg1, int pcount1, TFPoint *seg2, int pcount2);
   int __fastcall Validate(TFPoint *seg, int pcount);
   void __fastcall Rewidth(int left, int right, int idev);
   TSParams* __fastcall GenParams(TFPoint *fbuf, int pcount);
   int __fastcall PassNoise( void );
public:
    TFOnParams OnParams;
    TFOnCompare OnCompare;
    TNotifyEvent OnRewidth;
    TFOnNoise OnNoise;
    int minwidth, maxwidth, noiselevel;
    int apass, devpass;
    __fastcall TFoldProcess();
    int* __fastcall GenIDParams(TFPoint *fbuf, int pcount);
    int __fastcall SearchSample(TStream *Source, TFPoint *seg, int pcount, TFOnSegment OnSegment, TFOnProgress OnProgress);
    int __fastcall InsertFold(TFPoint *fbuf, int pcount);
    void __fastcall Reset( void );
    int __fastcall Segmentate(TFOnSegment OnSegment);
    __fastcall ~TFoldProcess();
};

//---------------------------------------------------------------------------
#endif
 