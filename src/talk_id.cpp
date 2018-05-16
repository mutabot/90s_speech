//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("talk_id.res");
USEFORM("forms\fldrec.cpp", fmMain);
USEUNIT("folder.cpp");
USEUNIT("fold_analys.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfmMain), &fmMain);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
