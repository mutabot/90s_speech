#ifndef sptecdefH
#define sptecdefH
#include <math.h>

#define BASE_CHAR_0    '@'
#define BASE_CHAR_1    '`'
#define MAX_TONES      32
#define MAX_MODIF      128
#define INDEX_COUNT    MAX_TONES*MAX_MODIF
#define SEG_LENGTH     65536

struct TFPoint
{
        unsigned char m;    // "width"
        unsigned char a;    // amplitude, ULaw encoded.
};

struct TSample
{
 unsigned char sound;
 unsigned char tone;
 unsigned short pcount;
};

struct TMinDev
{
 double mdev;
 double adev;
};

int __fastcall round(double value)
        {
         int f = floor(value);
         int c = ceil(value);
         double d = value - f;
         if (d < 0.5) return f;
         return c;
        };

//---------------------------------------------------------------------------
#endif
