//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("speech.res");
USEFORM("forms\fldmorph.cpp", fmMorphMain);
USEUNIT("morph.cpp");
USEUNIT("foldstore.cpp");
USEFORM("forms\fdir.cpp", fmFoldDir);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Звуковой морфинг";
                 Application->CreateForm(__classid(TfmMorphMain), &fmMorphMain);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}                                       
//---------------------------------------------------------------------------



