//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "foldstore.h"
#include "fdir.h"
#include "quest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TFoldStore::TFoldStore(const AnsiString FileName) 
{
 int i;
 Count = 0;
 iSize = sizeof(index) + sizeof(idmap);
 pSize = sizeof(TFPoint);
 FFileName = FileName;
 CFileName = FFileName.SubString(0, FFileName.Length() - 3) + "cmt";
 if (FileExists(FFileName))
 {
  msSamples = new TFileStream(FFileName, fmOpenReadWrite);
  msSamples->Seek(0, soFromBeginning);
  msSamples->Read(idmap, sizeof(idmap));
  msSamples->Read(index, sizeof(index));
  for (i = 0; i < MAX_TONES*MAX_MODIF; i++)
   if (idmap[i] > 0) Count++;
 } else {
  msSamples = new TFileStream(FFileName, fmCreate);
  memset(idmap, 0, sizeof(idmap));
  msSamples->Write(idmap, sizeof(idmap));
  memset(index, 0, sizeof(index));
  msSamples->Write(index, sizeof(index));
 }
 slComments = new TStringList();
 if (FileExists(CFileName)) slComments->LoadFromFile(CFileName);
 fmFoldDir = new TfmFoldDir(this, NULL, NULL);
}
//---------------------------------------------------------------------------
__fastcall TFoldStore::~TFoldStore()
{
 delete fmFoldDir;
 msSamples->Seek(0, soFromBeginning);
 msSamples->Write(idmap, sizeof(idmap));
 msSamples->Write(index, sizeof(index));

 slComments->SaveToFile(CFileName);
 delete slComments;
 delete msSamples;
}
//---------------------------------------------------------------------------
int __fastcall TFoldStore::Read(TSample *sample, TFPoint *buf)
{
 int idx = sample->sound*MAX_MODIF + sample->tone;
 msSamples->Seek(SOffset(idx, sample->pcount), soFromBeginning);
 msSamples->Read(buf, sample->pcount*pSize);
 return sample->pcount;
}
//---------------------------------------------------------------------------
bool __fastcall TFoldStore::SampleExists(TSample *sample)
{
 int idx = sample->sound*MAX_MODIF + sample->tone;
 return index[idx] != 0;
}
//---------------------------------------------------------------------------
int __fastcall TFoldStore::Write(TSample *sample,TFPoint *buf)
{
 int i, idx = sample->sound*MAX_MODIF + sample->tone;
 if (index[idx] != 0) return -1;
 i = 0;
 while (idmap[i] > 0) i++;
 idmap[i] = sample->pcount;
 index[idx] = i;
 msSamples->Position = msSamples->Size;
 msSamples->Write(buf, sample->pcount*pSize);
 Count++;
 return sample->pcount;
}
//---------------------------------------------------------------------------
int __fastcall TFoldStore::Delete(TSample *sample)
{
 int i, idx = sample->sound*MAX_MODIF + sample->tone, base, rdsize, rsize = 0;
 unsigned short pcount;
 unsigned char tbuf[1024];
 if (index[idx] == 0) return -1; 
 DelComment(sample);
 base = SOffset(idx, pcount);
 do
 {
  msSamples->Seek(base + rsize + pcount*pSize, soFromBeginning);
  rdsize = msSamples->Read(tbuf, sizeof(tbuf));
  msSamples->Seek(base + rsize, soFromBeginning);
  msSamples->Write(tbuf, rdsize);
  rsize+=rdsize;
 } while (rdsize == sizeof(tbuf));
 if (msSamples->Size == msSamples->Position) ShowMessage("Warning! Nothing to shift!");
 msSamples->Size = msSamples->Position;
 memmove(&idmap[index[idx]], &idmap[index[idx]+1], 2*(INDEX_COUNT - 1 - index[idx]));
 for (i=0; i < INDEX_COUNT; i++)
  if (index[i] > index[idx]) index[i]--;
 index[idx]=0;
 Count--;
 return rdsize;
}
//---------------------------------------------------------------------------
void __fastcall TFoldStore::SetComment(TSample *sample, const AnsiString Comment)
{
 AnsiString cmtLine = AnsiString((char)(sample->sound + BASE_CHAR_0)) + AnsiString((int)sample->tone);
 if (slComments->IndexOfName(cmtLine) == -1)
  slComments->Add(cmtLine + "=" + Comment);
 else slComments->Values[cmtLine] = Comment;
}
//---------------------------------------------------------------------------
void __fastcall TFoldStore::DelComment(TSample *sample)
{
 int i;
 AnsiString cmtLine = AnsiString((char)(sample->sound + BASE_CHAR_0)) + AnsiString((int)sample->tone);
 if ((i = slComments->IndexOfName(cmtLine)) >= 0)
  slComments->Delete(i);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFoldStore::GetComment(TSample *sample)
{
 AnsiString cmtLine = AnsiString((char)(sample->sound + BASE_CHAR_0)) + AnsiString((int)sample->tone);
 if (slComments->IndexOfName(cmtLine) < 0) return "";
 return slComments->Values[cmtLine];
}
//---------------------------------------------------------------------------
int __fastcall TFoldStore::WriteDialog(TSample *sample, TFPoint *buf)
{
 int i = -1;
 fmFoldDir->sample = sample;
 AnsiString asSample;
 switch (fmFoldDir->ShowModal()) {
  case mrCancel: return -1;
  case mrOk:
   if ((i = Write(sample, buf)) < 0)
   {
    fmQuest = new TfmQuest(NULL);
    asSample = AnsiString((char)(BASE_CHAR_0 + sample->sound)) + AnsiString(sample->tone);
    switch (fmQuest->Execute("Сэмпл [" + asSample + "] существует! \r\n Заменить? ")) {
     case mrNo: return -1;
     case mrYes:
     case mrYesToAll:
      Delete(sample);
      i = Write(sample, buf);
    }
   }
 }
 return i;
}
//---------------------------------------------------------------------------
const AnsiString __fastcall TFoldStore::MapDialog(TSample *sample)
{
 fmFoldDir->sample = sample;
 switch (fmFoldDir->ShowModal()) {
  case mrCancel: return "";
  case mrOk:
    return AnsiString((char)(BASE_CHAR_0 + sample->sound)) + AnsiString(sample->tone);
 }
 return "";
}
//---------------------------------------------------------------------------
int __fastcall TFoldStore::GetWidth(TSample *sample)
{
 TFPoint *fbuf;
 int i, idx = sample->sound*MAX_MODIF + sample->tone, width = 0;
 if (idx == 0) return -1;
 fbuf = new TFPoint[idmap[index[idx]]];
 Read(sample, fbuf);
 for (i = 1; i < idmap[index[idx]]; i++) width+=fbuf[i].m;
 delete fbuf;
 return width;
}
//---------------------------------------------------------------------------
int TFoldStore::SOffset(int idx, unsigned short &pcount)
{
 int i, offset = 0;
 for (i = 0; i < index[idx]; i++)
   offset+=idmap[i];

 pcount = idmap[i];
 return sizeof(index) + sizeof(idmap) + offset*sizeof(TFPoint);
}
//---------------------------------------------------------------------------
