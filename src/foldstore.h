//---------------------------------------------------------------------------
#ifndef foldstoreH
#define foldstoreH
//---------------------------------------------------------------------------
#include "sptecdef.h"
//---------------------------------------------------------------------------
class TFoldStore 
{
        TFileStream *msSamples;
        TStringList   *slComments;
        AnsiString FFileName, CFileName;
        int iSize, pSize;
        unsigned short index[INDEX_COUNT];
        unsigned short idmap[INDEX_COUNT];
        int SOffset(int idx, unsigned short &pcount);
public:
        int Count;
        int __fastcall Read(TSample *sample, TFPoint *buf);
        int __fastcall Write(TSample *sample, TFPoint *buf);
        int __fastcall Delete(TSample *sample);
        void __fastcall SetComment(TSample *sample, const AnsiString Comment);
        void __fastcall DelComment(TSample *sample);
        bool __fastcall SampleExists(TSample *sample);
        AnsiString __fastcall GetComment(TSample *sample);
        __fastcall TFoldStore(const AnsiString FileName);
        __fastcall ~TFoldStore();
    int __fastcall WriteDialog(TSample *sample, TFPoint *buf);
    const AnsiString __fastcall MapDialog(TSample *sample);
    int __fastcall GetWidth(TSample * sample);
};
//---------------------------------------------------------------------------
#endif
