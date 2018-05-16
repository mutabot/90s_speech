//---------------------------------------------------------------------------
#ifndef morphH
#define morphH

#include <math.h>
#include "foldstore.h"
#define S_BUF_SIZE      65535
//---------------------------------------------------------------------------
class TMorph
{
        TStream   *fsResult;
        TFPoint cur[S_BUF_SIZE]; // текущий звук
        TFPoint dst[S_BUF_SIZE]; // звук в направлении которого морфируем
        TFPoint wrk[S_BUF_SIZE];
        TSample cur_smp, dst_smp;
        int __fastcall load_sample( TFPoint buf[], TSample *sample );
        int __fastcall round(double value)
        {
         int f = floor(value);
         int c = ceil(value);
         double d = value - f;
         if (d < 0.5) return f;
         return c;
        };
        int where_constructed;
public:
        int LengthDev;
        TFoldStore    *FoldStore;
        __fastcall TMorph(TStream   *FResult);
        __fastcall TMorph(TFoldStore *AFoldStore, TStream   *FResult);
        __fastcall ~TMorph();
    int __fastcall Morph(int steps);
    int __fastcall LoadSeg(TSample *smp);
    int __fastcall LoadSeg(TFPoint *seg, int pcount);
};
//---------------------------------------------------------------------------
#endif
 