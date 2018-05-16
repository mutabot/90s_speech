//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("mvisio.cpp", fmVisio);
USEFORM("forms\quest.cpp", fmQuest);
USEFORM("forms\colors.cpp", fmColor);
USEFORM("forms\fdir.cpp", fmFoldDir);
USEFORM("forms\signal.cpp", fmSignal);
USEFORM("forms\fldmorph.cpp", fmMorphMain);
USEFORM("forms\fldrec.cpp", fmFldRec);
USEFORM("forms\fldextr.cpp", fmFldExtr);
USEFORM("forms\fldproc.cpp", fmFldProc);
USEFORM("forms\foldman.cpp", fmFoldMan);
USEFORM("forms\segview.cpp", fmSegView);
USEFORM("forms\tts.cpp", fmTTS);
USEFORM("forms\rulemap.cpp", fmRuleMap);
USEFORM("forms\hirules.cpp", fmHiRules);
USEFORM("forms\segmentate.cpp", fmSegments);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfmVisio), &fmVisio);
         Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

