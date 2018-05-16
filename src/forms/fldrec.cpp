//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fldrec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmFldRec *fmFldRec;
//---------------------------------------------------------------------------
__fastcall TfmFldRec::TfmFldRec(TComponent* Owner)
        : TForm(Owner)
{
 int i;
 unsigned char flev[4];

 Folder = new TFolder(this);
 Folder->OnFold = OnFold;
 Folder->OnSignal = OnSignal;
 fsFold = NULL;
 fsPCM = NULL;

 fneSrcFile->InitialDir = GetCurrentDir() + "\\..\\pcm";

 rgParams = new TRegistry();
 rgParams->OpenKey("SOFTWARE\\IMS\\Folder", true);
 if ((i = rgParams->ReadBinaryData("cmpLevel", flev, sizeof(flev))) > 0)
 {
  Folder->SetCLevel(flev);
  edCLev->Text = "";
  for (int j = 0; j < i; j++)
   edCLev->Text = edCLev->Text + AnsiString((int)flev[j]) + ", ";
  edCLev->Text.SetLength(edCLev->Text.Length() - 2);
 }
 if ((i = rgParams->ReadBinaryData("vasLevel", flev, sizeof(flev))) > 0)
 {
  Folder->SetVLevel(flev);
  edVLev->Text = "";
  for (int j = 0; j < i; j++)
   edVLev->Text = edVLev->Text + AnsiString((int)flev[j]) + ", ";
  edVLev->Text.SetLength(edVLev->Text.Length() - 2);
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::CreateParams(TCreateParams& Params)
{
 TForm::CreateParams(Params);
 Params.WndParent = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::OnFold( int fold_size, int b_size )
{
 fsFold->Write(Folder->fbuf, fold_size*sizeof(TFPoint));
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::OnSignal( int Colour )
{
 switch (Colour) {
  case 0:
   shpRec->Brush->Color = clRed; break;
  case 1:
   shpRec->Brush->Color = clLime;
   shpRec->Refresh();
  break;
  case 2:
   shpRec->Brush->Color = clWhite; break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::FormDestroy(TObject *Sender)
{
 delete Folder;
 delete fsFold;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::btStartClick(TObject *Sender)
{
  Folder->Reset();
  pbConverting->Position = 0;

  fsPCM = new TFileStream(fneSrcFile->FileName, fmOpenRead | fmShareDenyNone );
  pbConverting->Max = fsPCM->Size;

  try
  {
   fsFold = new TFileStream(fneDstFile->FileName, fmCreate);
  } catch ( ... ) {
   fsFold = new TFileStream(fneDstFile->FileName, fmOpenWrite | fmShareDenyNone );
   fsFold->Position = 0;
  }
  // extract loop
  int rd = 0, wr = 0;
  while ((rd = fsPCM->Read(buf_PCM, sizeof(buf_PCM))) > 0)
  {
    Folder->InBufferReady((char*)buf_PCM, rd);
    pbConverting->Position = fsPCM->Position;
  }
  pbConverting->Position = 0;

  delete fsFold;
  fsFold = NULL;
  delete fsPCM;
  fsPCM = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::btSetCLevClick(TObject *Sender)
{
 int j, k;
 AnsiString Line, Param;
 unsigned char c_lev[4];
 Line = edCLev->Text;
 k = 0; Param = "";
 for (j = 1; j <= Line.Length(); j++)
 {
  if (Line[j] == ',') { c_lev[k] = Param.Trim().ToIntDef(4); k++; Param = ""; }
  else Param = Param + Line[j];
 }
 c_lev[k] = Param.Trim().ToIntDef(4);
 Folder->SetCLevel(c_lev);
 rgParams->WriteBinaryData("cmpLevel", c_lev, sizeof(c_lev));

 Line = edVLev->Text;
 k = 0; Param = "";
 for (j = 1; j <= Line.Length(); j++)
 {
  if (Line[j] == ',') { c_lev[k] = Param.Trim().ToIntDef(4); k++; Param = ""; }
  else Param = Param + Line[j];
 }
 c_lev[k] = Param.Trim().ToIntDef(4);
 Folder->SetVLevel(c_lev);
 rgParams->WriteBinaryData("vasLevel", c_lev, sizeof(c_lev));
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::cbTopClick(TObject *Sender)
{
 if (cbTop->Checked) FormStyle = fsStayOnTop;
 else FormStyle = fsNormal;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::SpeedButton2Click(TObject *Sender)
{
 gbParams->Visible = !gbParams->Visible;        
}
//---------------------------------------------------------------------------

void __fastcall TfmFldRec::FormClose(TObject *Sender, TCloseAction &Action)
{
 fmFldRec = NULL;
 Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TfmFldRec::btBrowseDstClick(TObject *Sender)
{
    fneDstFile->Execute();
    edDstFile->Text = fneDstFile->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfmFldRec::btBrowseSrcClick(TObject *Sender)
{
    fneSrcFile->Execute();
    edSrcFile->Text = fneSrcFile->FileName;
}
//---------------------------------------------------------------------------

