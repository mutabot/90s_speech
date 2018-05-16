//---------------------------------------------------------------------------
#ifndef extruderH
#define extruderH
#include "sptecdef.h"
#include "morph.h"             // <-- Elementary segments morphing
#define  FLD_BUF_SIZE              1024
#define  RES_BUF_SIZE              8192
//---------------------------------------------------------------------------
class TExtrudeEvents;
class TExtruder
{
    TMemoryStream *msMorph;
    TMorph        *Morph;
public:
    __fastcall TExtruder();
    __fastcall TExtruder(TExtrudeEvents *pOnExtrude);
    int __fastcall Extrude(TStream *Fold, TStream *Buf, int fsize);
    TExtrudeEvents *m_pOnExtrude;
    short last;
    __fastcall ~TExtruder();
};

class TExtrudeEvents
{
public:
    virtual int OnBufferReady(short *buf, int size) = 0;
    virtual int OnDone(void) = 0;
};
#endif
