//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("extrude.res");
USEFORM("forms\fldextr.cpp", fmExtrude);
USEUNIT("extruder.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfmExtrude), &fmExtrude);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
