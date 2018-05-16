//---------------------------------------------------------------------------
#ifndef folderH
#define folderH

#define IN_BUF_SIZE             2048
#define FOLD_BUF_SIZE           32768
#define MAX_WIDTH               0xFE

#include "AudioIO.hpp"   // <---- TAudioIO VCL by John Mertus !!!!!!!!! 
#include "sptecdef.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TFOnFold)(int fold_size, int b_size);
typedef void __fastcall (__closure *TFOnSignal)( int Colour );
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TFolder : public TComponent
{
  TStringList *bList;
  bool folding;
  int sil_count;
// signal params
  int d, last_d, tangent[3];

  short c_level[4]; // compression parameters
  short vas_lvl[4]; //  VAS parameters
  int __fastcall val_max(short value);
public:
  TFPoint fbuf[FOLD_BUF_SIZE];
  int fold_pos;
  int b_ptr;

  __fastcall TFolder(TComponent *AOwner);
  __fastcall ~TFolder();
  void __fastcall Reset( void );
  void __fastcall SetCLevel(unsigned char lev[]);
  void __fastcall SetVLevel(unsigned char lev[]);

  bool __fastcall InBufferReady(PChar Buffer, int &Size);
  TFOnFold OnFold;
  TFOnSignal OnSignal;
};

//---------------------------------------------------------------------------
#endif
